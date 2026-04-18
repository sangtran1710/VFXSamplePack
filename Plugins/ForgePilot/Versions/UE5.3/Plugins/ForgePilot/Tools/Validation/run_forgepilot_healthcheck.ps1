[CmdletBinding()]
param(
    [switch]$RunBuild,
    [switch]$RunBridge,
    [switch]$RunAskAssist,
    [switch]$RequireClean,
    [switch]$SkipGitChecks,
    [string]$JsonOut = '',
    [int]$LargeFileThresholdMB = 50,
    [string]$BridgeHost = '',
    [int]$BridgePort = 0,
    [int]$TimeoutSeconds = 30
)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

$ScriptRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
$PluginRoot = Split-Path -Parent (Split-Path -Parent $ScriptRoot)
$ProjectRoot = Split-Path -Parent (Split-Path -Parent $PluginRoot)
$GitAvailable = $false
$GitDir = $null
if (-not $SkipGitChecks) {
    $previousErrorActionPreference = $ErrorActionPreference
    $GitDirRaw = ''
    $gitExitCode = 1
    try {
        $script:ErrorActionPreference = 'Continue'
        $GitDirRaw = (& git -C $ProjectRoot rev-parse --git-dir 2>$null)
        $gitExitCode = $LASTEXITCODE
    }
    catch {
        $GitDirRaw = ''
        $gitExitCode = 1
    }
    finally {
        $script:ErrorActionPreference = $previousErrorActionPreference
    }

    if ($gitExitCode -eq 0 -and -not [string]::IsNullOrWhiteSpace($GitDirRaw)) {
        $GitAvailable = $true
        if ([System.IO.Path]::IsPathRooted($GitDirRaw)) {
            $GitDir = $GitDirRaw
        }
        else {
            $GitDir = Join-Path $ProjectRoot $GitDirRaw
        }
    }
}

$Summary = [ordered]@{
    success = $false
    phase_results = @()
    warnings = @()
    failures = @()
    repo_state = [ordered]@{}
    forbidden_tracked_files = @()
    forbidden_untracked_files = @()
    large_files = @()
}

function Add-PhaseResult {
    param(
        [Parameter(Mandatory = $true)][string]$Phase,
        [Parameter(Mandatory = $true)][string]$Check,
        [Parameter(Mandatory = $true)][string]$Status,
        [string]$Detail = ''
    )

    $record = [pscustomobject]@{
        phase = $Phase
        check = $Check
        status = $Status
        detail = $Detail
    }
    $script:Summary.phase_results += $record

    if ($Status -eq 'FAIL') {
        $script:Summary.failures += "${Phase}/${Check}: $Detail"
    }
    elseif ($Status -eq 'WARN') {
        $script:Summary.warnings += "${Phase}/${Check}: $Detail"
    }
}

function Invoke-Phase {
    param(
        [Parameter(Mandatory = $true)][string]$Phase,
        [Parameter(Mandatory = $true)][string]$Check,
        [Parameter(Mandatory = $true)][scriptblock]$Action
    )

    try {
        & $Action
    }
    catch {
        Add-PhaseResult -Phase $Phase -Check $Check -Status 'FAIL' -Detail $_.Exception.Message
    }
}

function Test-PhaseFailed {
    param([Parameter(Mandatory = $true)][string]$Phase)

    return @($script:Summary.phase_results | Where-Object { $_.phase -eq $Phase -and $_.status -eq 'FAIL' }).Count -gt 0
}

function Invoke-Git {
    param([Parameter(Mandatory = $true)][string[]]$Args)

    $output = & git -C $ProjectRoot @Args 2>&1
    $exitCode = $LASTEXITCODE
    return [pscustomobject]@{
        ExitCode = $exitCode
        Output = @($output)
        Text = (@($output) -join "`n")
    }
}

function Invoke-ChildPowerShell {
    param([Parameter(Mandatory = $true)][string[]]$Arguments)

    $previousErrorActionPreference = $ErrorActionPreference
    $output = @()
    $exitCode = 0
    try {
        $script:ErrorActionPreference = 'Continue'
        $output = & powershell @Arguments 2>&1
        $exitCode = $LASTEXITCODE
    }
    finally {
        $script:ErrorActionPreference = $previousErrorActionPreference
    }

    $outputLines = @($output | ForEach-Object { $_.ToString() })
    return [pscustomobject]@{
        ExitCode = $exitCode
        Output = $outputLines
        Text = ($outputLines -join "`n")
    }
}

function Invoke-ChildPowerShellWithRetry {
    param(
        [Parameter(Mandatory = $true)][string[]]$Arguments,
        [int]$Attempts = 2,
        [int]$DelaySeconds = 2
    )

    $result = $null
    for ($attempt = 1; $attempt -le $Attempts; $attempt++) {
        $result = Invoke-ChildPowerShell -Arguments $Arguments
        if ($result.ExitCode -eq 0) {
            return $result
        }

        if ($attempt -lt $Attempts) {
            Start-Sleep -Seconds $DelaySeconds
        }
    }

    return $result
}

function Get-TrackedFiles {
    $result = Invoke-Git -Args @('ls-files')
    if ($result.ExitCode -ne 0) {
        throw "git ls-files failed: $($result.Text)"
    }

    return @($result.Output | Where-Object { -not [string]::IsNullOrWhiteSpace($_) } | ForEach-Object { [string]$_ })
}

function Get-UntrackedFiles {
    $result = Invoke-Git -Args @('ls-files', '--others', '--exclude-standard')
    if ($result.ExitCode -ne 0) {
        throw "git ls-files --others failed: $($result.Text)"
    }

    return @($result.Output | Where-Object { -not [string]::IsNullOrWhiteSpace($_) } | ForEach-Object { [string]$_ })
}

function Get-PythonCommand {
    foreach ($Candidate in @('python', 'py')) {
        try {
            & $Candidate --version *> $null
            if ($LASTEXITCODE -eq 0) {
                return $Candidate
            }
        }
        catch {
        }
    }

    return $null
}

function Test-PowerShellSyntax {
    param([Parameter(Mandatory = $true)][string[]]$Paths)

    foreach ($Path in $Paths) {
        $tokens = $null
        $errors = $null
        [void][System.Management.Automation.Language.Parser]::ParseFile($Path, [ref]$tokens, [ref]$errors)
        if ($errors -and $errors.Count -gt 0) {
            $messages = $errors | ForEach-Object { $_.Message }
            throw "PowerShell parse failed for '$Path': $($messages -join '; ')"
        }
    }
}

function Test-ApprovedPluginMarkdownPath {
    param([Parameter(Mandatory = $true)][string]$Path)

    $normalized = $Path -replace '\\', '/'

    $approvedFiles = @(
        'Plugins/ForgePilot/AGENTS.md'
    )
    if ($approvedFiles -contains $normalized) {
        return $true
    }

    $approvedRoots = @(
        'Plugins/ForgePilot/Docs/',
        'Plugins/ForgePilot/Knowledge/',
        'Plugins/ForgePilot/Process/',
        'Plugins/ForgePilot/Tools/Validation/',
        'Plugins/ForgePilot/Tools/Python/'
    )
    foreach ($root in $approvedRoots) {
        if ($normalized.StartsWith($root, [System.StringComparison]::OrdinalIgnoreCase)) {
            return $true
        }
    }

    return $false
}

function Get-ForbiddenRepoPathReason {
    param([Parameter(Mandatory = $true)][string]$Path)

    $normalized = $Path -replace '\\', '/'

    if ($normalized -match '(^|/)AGENTS\.md$' -and $normalized -ne 'Plugins/ForgePilot/AGENTS.md') {
        return 'AGENTS.md is only allowed at Plugins/ForgePilot/AGENTS.md'
    }
    if ($normalized -match '(^|/)__pycache__/') {
        return 'generated Python __pycache__ directory'
    }
    if ($normalized -match '\.pyc$') {
        return 'generated Python bytecode file'
    }
    if ($normalized -like 'Plugins/ForgePilot/Output/*') {
        return 'generated plugin Output directory'
    }
    if ($normalized -like 'Plugins/ForgePilot/Saved/*') {
        return 'generated plugin Saved directory'
    }
    if ($normalized -like 'Plugins/ForgePilot/Intermediate/*') {
        return 'generated plugin Intermediate directory'
    }
    if ($normalized -like 'Content/ForgePilot_Test/*') {
        return 'generated ForgePilot test assets under project Content'
    }
    if ($normalized -like 'Plugins/ForgePilot/Content/ForgePilot_Test/*') {
        return 'generated ForgePilot test assets under plugin Content'
    }
    if ($normalized -match '^Plugins/ForgePilot/.+\.md$' -and -not (Test-ApprovedPluginMarkdownPath -Path $normalized)) {
        return 'loose plugin Markdown outside approved documentation folders'
    }
    if ($normalized -match '^Plugins/ForgePilot/[^/]+\.(json|txt)$') {
        $allowedRootFiles = @(
            'Plugins/ForgePilot/AGENTS.md'
        )
        if (-not ($allowedRootFiles -contains $normalized)) {
            return 'loose plugin root metadata/text file'
        }
    }

    return ''
}

if ($GitAvailable) {
Invoke-Phase -Phase 'repo_state' -Check 'git status' -Action {
    $branchResult = Invoke-Git -Args @('branch', '--show-current')
    if ($branchResult.ExitCode -ne 0) {
        throw "git branch failed: $($branchResult.Text)"
    }

    $statusResult = Invoke-Git -Args @('status', '--porcelain=v2', '--branch')
    if ($statusResult.ExitCode -ne 0) {
        throw "git status failed: $($statusResult.Text)"
    }

    $stashResult = Invoke-Git -Args @('stash', 'list')
    $stashCount = if ([string]::IsNullOrWhiteSpace($stashResult.Text)) { 0 } else { @($stashResult.Output).Count }
    $statusLines = @($statusResult.Output)
    $dirtyLines = @($statusLines | Where-Object { -not ([string]$_).StartsWith('#') })
    $unmerged = @($dirtyLines | Where-Object { ([string]$_).StartsWith('u ') })
    $ahead = 0
    $behind = 0
    foreach ($line in $statusLines) {
        if ($line -match '^# branch\.ab \+([0-9]+) -([0-9]+)') {
            $ahead = [int]$Matches[1]
            $behind = [int]$Matches[2]
        }
    }

    $script:Summary.repo_state = [ordered]@{
        branch = [string]($branchResult.Text.Trim())
        ahead = $ahead
        behind = $behind
        dirty_count = @($dirtyLines).Count
        stash_count = $stashCount
        merge_in_progress = Test-Path (Join-Path $GitDir 'MERGE_HEAD')
        rebase_in_progress = (Test-Path (Join-Path $GitDir 'rebase-merge')) -or (Test-Path (Join-Path $GitDir 'rebase-apply'))
        unmerged_count = @($unmerged).Count
    }

    if ($script:Summary.repo_state.merge_in_progress -or $script:Summary.repo_state.rebase_in_progress -or $script:Summary.repo_state.unmerged_count -gt 0) {
        throw "Repository has an active merge/rebase or unmerged paths."
    }

    if ($RequireClean -and $script:Summary.repo_state.dirty_count -gt 0) {
        throw "Working tree is dirty and -RequireClean was specified."
    }

    $detail = "branch=$($script:Summary.repo_state.branch), ahead=$ahead, behind=$behind, dirty=$($script:Summary.repo_state.dirty_count), stashes=$stashCount"
    if ($script:Summary.repo_state.dirty_count -gt 0) {
        Add-PhaseResult -Phase 'repo_state' -Check 'working tree dirty' -Status 'WARN' -Detail $detail
    }
    else {
        Add-PhaseResult -Phase 'repo_state' -Check 'git status' -Status 'PASS' -Detail $detail
    }
}

Invoke-Phase -Phase 'plugin_hygiene' -Check 'forbidden tracked files' -Action {
    $trackedFiles = Get-TrackedFiles
    $forbidden = @($trackedFiles | ForEach-Object {
        $reason = Get-ForbiddenRepoPathReason -Path $_
        if (-not [string]::IsNullOrWhiteSpace($reason)) {
            "$_ ($reason)"
        }
    } | Sort-Object)
    $script:Summary.forbidden_tracked_files = $forbidden
    if ($forbidden.Count -gt 0) {
        throw "Forbidden tracked files found: $($forbidden -join ', ')"
    }

    Add-PhaseResult -Phase 'plugin_hygiene' -Check 'forbidden tracked files' -Status 'PASS' -Detail 'No forbidden tracked files found.'
}

Invoke-Phase -Phase 'plugin_hygiene' -Check 'forbidden untracked files' -Action {
    $untrackedFiles = Get-UntrackedFiles
    $forbidden = @($untrackedFiles | ForEach-Object {
        $reason = Get-ForbiddenRepoPathReason -Path $_
        if (-not [string]::IsNullOrWhiteSpace($reason)) {
            "$_ ($reason)"
        }
    } | Sort-Object)
    $script:Summary.forbidden_untracked_files = $forbidden
    if ($forbidden.Count -gt 0) {
        throw "Forbidden untracked files found: $($forbidden -join ', ')"
    }

    Add-PhaseResult -Phase 'plugin_hygiene' -Check 'forbidden untracked files' -Status 'PASS' -Detail 'No forbidden untracked files found.'
}
}
else {
    $Summary.repo_state = [ordered]@{
        git_available = $false
        project_root = $ProjectRoot
        skipped = $true
    }
    Add-PhaseResult -Phase 'repo_state' -Check 'git status' -Status 'WARN' -Detail 'No git repository detected or -SkipGitChecks was specified; git-dependent checks skipped.'
    Add-PhaseResult -Phase 'plugin_hygiene' -Check 'forbidden tracked files' -Status 'WARN' -Detail 'Skipped because git is unavailable.'
    Add-PhaseResult -Phase 'plugin_hygiene' -Check 'forbidden untracked files' -Status 'WARN' -Detail 'Skipped because git is unavailable.'
}

Invoke-Phase -Phase 'plugin_structure' -Check 'required plugin paths' -Action {
    $requiredPaths = @(
        'Plugins/ForgePilot/AGENTS.md',
        'Plugins/ForgePilot/Docs',
        'Plugins/ForgePilot/Source',
        'Plugins/ForgePilot/Tools/Validation',
        'Plugins/ForgePilot/Tools/MCP',
        'Plugins/ForgePilot/Knowledge'
    )
    $missing = @()
    foreach ($relativePath in $requiredPaths) {
        if (-not (Test-Path (Join-Path $ProjectRoot $relativePath))) {
            $missing += $relativePath
        }
    }
    if ($missing.Count -gt 0) {
        throw "Missing required plugin paths: $($missing -join ', ')"
    }

    Add-PhaseResult -Phase 'plugin_structure' -Check 'required plugin paths' -Status 'PASS' -Detail 'Required plugin paths exist.'
}

Invoke-Phase -Phase 'syntax_static' -Check 'PowerShell validation scripts parse' -Action {
    $scripts = @(Get-ChildItem -Path (Join-Path $PluginRoot 'Tools/Validation') -Filter '*.ps1' -File | ForEach-Object { $_.FullName })
    if ($scripts.Count -eq 0) {
        throw "No validation PowerShell scripts found."
    }

    Test-PowerShellSyntax -Paths $scripts
    Add-PhaseResult -Phase 'syntax_static' -Check 'PowerShell validation scripts parse' -Status 'PASS' -Detail "Parsed $($scripts.Count) scripts."
}

Invoke-Phase -Phase 'contract_static' -Check 'mutation tool contract audit' -Action {
    $auditScript = Join-Path $ScriptRoot 'run_forgepilot_tool_contract_audit.ps1'
    if (-not (Test-Path $auditScript)) {
        throw "Tool contract audit script not found: $auditScript"
    }

    $auditResult = Invoke-ChildPowerShell -Arguments @('-ExecutionPolicy', 'Bypass', '-File', $auditScript)
    if ($auditResult.ExitCode -ne 0) {
        throw "Mutation tool contract audit failed. $($auditResult.Text)"
    }

    Add-PhaseResult -Phase 'contract_static' -Check 'mutation tool contract audit' -Status 'PASS' -Detail 'Mutation tool contracts passed static audit.'
}

Invoke-Phase -Phase 'syntax_static' -Check 'Python plugin scripts compile' -Action {
    $python = Get-PythonCommand
    if (-not $python) {
        Add-PhaseResult -Phase 'syntax_static' -Check 'Python plugin scripts compile' -Status 'WARN' -Detail 'No python command found on PATH; skipped.'
        return
    }

    if ($GitAvailable) {
        $pythonFiles = @(Get-TrackedFiles | Where-Object {
            $_ -like 'Plugins/ForgePilot/*' -and $_.ToLowerInvariant().EndsWith('.py')
        })
    }
    else {
        $pythonFiles = @(Get-ChildItem -Path $PluginRoot -Filter '*.py' -File -Recurse | ForEach-Object {
            $_.FullName.Substring($ProjectRoot.Length + 1)
        })
    }

    foreach ($relativePath in $pythonFiles) {
        $fullPath = Join-Path $ProjectRoot $relativePath
        $compileOutput = & $python -m py_compile $fullPath 2>&1
        if ($LASTEXITCODE -ne 0) {
            throw "python -m py_compile failed for '$relativePath': $compileOutput"
        }
    }

    $scopeLabel = if ($GitAvailable) { 'tracked' } else { 'discovered' }
    Add-PhaseResult -Phase 'syntax_static' -Check 'Python plugin scripts compile' -Status 'PASS' -Detail "Compiled $($pythonFiles.Count) $scopeLabel Python files."
}

Invoke-Phase -Phase 'large_file_report' -Check 'large tracked files' -Action {
    if (-not $GitAvailable) {
        Add-PhaseResult -Phase 'large_file_report' -Check 'large tracked files' -Status 'WARN' -Detail 'Skipped because git is unavailable.'
        return
    }

    $thresholdBytes = [int64]$LargeFileThresholdMB * 1024 * 1024
    $largeFiles = @()
    foreach ($relativePath in (Get-TrackedFiles | Where-Object { $_ -like 'Plugins/ForgePilot/*' -or $_ -like 'Content/VFX/*' })) {
        $fullPath = Join-Path $ProjectRoot $relativePath
        if (-not (Test-Path $fullPath -PathType Leaf)) {
            continue
        }

        $item = Get-Item $fullPath
        if ($item.Length -ge $thresholdBytes) {
            $largeFiles += [pscustomobject]@{
                path = $relativePath
                size_bytes = $item.Length
                size_mb = [math]::Round(($item.Length / 1MB), 2)
            }
        }
    }

    $script:Summary.large_files = $largeFiles
    if ($largeFiles.Count -gt 0) {
        Add-PhaseResult -Phase 'large_file_report' -Check 'large tracked files' -Status 'WARN' -Detail "$($largeFiles.Count) tracked files exceed ${LargeFileThresholdMB}MB."
    }
    else {
        Add-PhaseResult -Phase 'large_file_report' -Check 'large tracked files' -Status 'PASS' -Detail "No tracked plugin/VFX files exceed ${LargeFileThresholdMB}MB."
    }
}

if ($RunBuild) {
    Invoke-Phase -Phase 'optional_build' -Check 'Unreal build' -Action {
        $buildScript = Join-Path $ScriptRoot 'build_unreal_with_recovery.ps1'
        if (-not (Test-Path $buildScript)) {
            throw "Build script not found: $buildScript"
        }

        $buildResult = Invoke-ChildPowerShell -Arguments @('-ExecutionPolicy', 'Bypass', '-File', $buildScript)
        if ($buildResult.ExitCode -ne 0) {
            throw "Build failed with exit code $($buildResult.ExitCode). $($buildResult.Text)"
        }

        Add-PhaseResult -Phase 'optional_build' -Check 'Unreal build' -Status 'PASS' -Detail 'Detected editor target build succeeded.'
    }
}

if ($RunBridge -or $RunAskAssist) {
    Invoke-Phase -Phase 'optional_bridge' -Check 'bridge ping/list_tools' -Action {
        $invokeBridge = Join-Path $ScriptRoot 'invoke_bridge.ps1'
        if (-not (Test-Path $invokeBridge)) {
            throw "Bridge helper not found: $invokeBridge"
        }

        $bridgeArgs = @('-ExecutionPolicy', 'Bypass', '-File', $invokeBridge, '-Ping', '-TimeoutSeconds', "$TimeoutSeconds")
        if (-not [string]::IsNullOrWhiteSpace($BridgeHost)) {
            $bridgeArgs += @('-BridgeHost', $BridgeHost)
        }
        if ($BridgePort -gt 0) {
            $bridgeArgs += @('-BridgePort', "$BridgePort")
        }
        $pingResult = Invoke-ChildPowerShellWithRetry -Arguments $bridgeArgs -Attempts 2 -DelaySeconds 2
        if ($pingResult.ExitCode -ne 0) {
            throw "Bridge ping failed. Start Unreal with ForgePilot bridge enabled, then retry. $($pingResult.Text)"
        }

        $listArgs = @('-ExecutionPolicy', 'Bypass', '-File', $invokeBridge, '-ListTools', '-TimeoutSeconds', "$TimeoutSeconds")
        if (-not [string]::IsNullOrWhiteSpace($BridgeHost)) {
            $listArgs += @('-BridgeHost', $BridgeHost)
        }
        if ($BridgePort -gt 0) {
            $listArgs += @('-BridgePort', "$BridgePort")
        }
        $listResult = Invoke-ChildPowerShellWithRetry -Arguments $listArgs -Attempts 2 -DelaySeconds 2
        if ($listResult.ExitCode -ne 0) {
            throw "Bridge list_tools failed. $($listResult.Text)"
        }

        Add-PhaseResult -Phase 'optional_bridge' -Check 'bridge ping/list_tools' -Status 'PASS' -Detail 'Bridge responded to ping and list_tools.'
    }
}

if ($RunAskAssist) {
    if (Test-PhaseFailed -Phase 'optional_bridge') {
        Add-PhaseResult -Phase 'optional_ask_assist' -Check 'Ask + Assist validation' -Status 'FAIL' -Detail 'Skipped because bridge preflight failed.'
    }
    else {
        Invoke-Phase -Phase 'optional_ask_assist' -Check 'Ask + Assist validation' -Action {
            $askAssistScript = Join-Path $ScriptRoot 'run_ask_assist_validation.ps1'
            if (-not (Test-Path $askAssistScript)) {
                throw "Ask + Assist validation script not found: $askAssistScript"
            }

            $askArgs = @('-ExecutionPolicy', 'Bypass', '-File', $askAssistScript, '-TimeoutSeconds', "$TimeoutSeconds")
            if (-not [string]::IsNullOrWhiteSpace($BridgeHost)) {
                $askArgs += @('-BridgeHost', $BridgeHost)
            }
            if ($BridgePort -gt 0) {
                $askArgs += @('-BridgePort', "$BridgePort")
            }
            $askResult = Invoke-ChildPowerShell -Arguments $askArgs
            if ($askResult.ExitCode -ne 0) {
                throw "Ask + Assist validation failed. $($askResult.Text)"
            }

            Add-PhaseResult -Phase 'optional_ask_assist' -Check 'Ask + Assist validation' -Status 'PASS' -Detail 'Ask + Assist validation completed.'
        }
    }
}

$failCount = @($Summary.phase_results | Where-Object { $_.status -eq 'FAIL' }).Count
$Summary.success = ($failCount -eq 0)

Write-Host ''
Write-Host '=== ForgePilot Healthcheck ==='
$Summary.phase_results | Format-Table -AutoSize -Wrap

if ($Summary.failures.Count -gt 0) {
    Write-Host ''
    Write-Host 'Failures:'
    $Summary.failures | ForEach-Object { Write-Host " - $_" }
}

if ($Summary.warnings.Count -gt 0) {
    Write-Host ''
    Write-Host 'Warnings:'
    $Summary.warnings | ForEach-Object { Write-Host " - $_" }
}

if ($Summary.forbidden_tracked_files.Count -gt 0) {
    Write-Host ''
    Write-Host 'Forbidden tracked files:'
    $Summary.forbidden_tracked_files | ForEach-Object { Write-Host " - $_" }
}

if ($Summary.forbidden_untracked_files.Count -gt 0) {
    Write-Host ''
    Write-Host 'Forbidden untracked files:'
    $Summary.forbidden_untracked_files | ForEach-Object { Write-Host " - $_" }
}

if ($Summary.large_files.Count -gt 0) {
    Write-Host ''
    Write-Host "Large tracked files (>= ${LargeFileThresholdMB}MB):"
    $Summary.large_files | ForEach-Object { Write-Host " - $($_.path) ($($_.size_mb) MB)" }
}

if (-not [string]::IsNullOrWhiteSpace($JsonOut)) {
    $jsonPath = if ([System.IO.Path]::IsPathRooted($JsonOut)) { $JsonOut } else { Join-Path (Get-Location) $JsonOut }
    $jsonParent = Split-Path -Parent $jsonPath
    if (-not [string]::IsNullOrWhiteSpace($jsonParent) -and -not (Test-Path $jsonParent)) {
        New-Item -ItemType Directory -Force -Path $jsonParent | Out-Null
    }
    $Summary | ConvertTo-Json -Depth 20 | Set-Content -Path $jsonPath -Encoding UTF8
}

if (-not $Summary.success) {
    exit 1
}

exit 0
