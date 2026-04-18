[CmdletBinding()]
param(
    [string]$SuitePath = "",
    [string]$OutputRoot = "",
    [switch]$RunSemanticOps,
    [string]$HeroMaterialPath,
    [string]$CoreMaterialPath,
    [string]$WorkflowToolName = "run_effect_workflow_v2",
    [switch]$KeepAssets
)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

. (Join-Path $PSScriptRoot 'invoke_bridge.ps1')

function Normalize-BridgeResult {
    param($Value)
    if ($Value -is [string]) {
        return ($Value | ConvertFrom-Json)
    }
    return $Value
}

function New-SafeNameToken {
    param([Parameter(Mandatory = $true)][string]$Value)
    $token = $Value -replace '[^A-Za-z0-9_]', '_'
    $token = $token.Trim('_')
    if ([string]::IsNullOrWhiteSpace($token)) { return 'Task' }
    return $token
}

function Get-OptionalPropertyValue {
    param(
        [Parameter(Mandatory = $true)]$Object,
        [Parameter(Mandatory = $true)][string]$PropertyName
    )
    if ($null -eq $Object) { return $null }
    $property = $Object.PSObject.Properties[$PropertyName]
    if ($null -eq $property) { return $null }
    return $property.Value
}

function Get-DefaultSuitePaths {
    return @(
        (Join-Path $PSScriptRoot 'coreenergy_v2_eval_tasks.json'),
        (Join-Path $PSScriptRoot 'fire_orb_v2_eval_tasks.json'),
        (Join-Path $PSScriptRoot 'magic_orb_v2_eval_tasks.json'),
        (Join-Path $PSScriptRoot 'electric_orb_v2_eval_tasks.json'),
        (Join-Path $PSScriptRoot 'impact_orb_v2_eval_tasks.json'),
        (Join-Path $PSScriptRoot 'explosion_orb_v2_eval_tasks.json')
    )
}

function Get-WorkflowStepNames {
    param($WorkflowResult)
    $steps = @()
    foreach ($step in @($WorkflowResult.bound_execution_plan)) {
        if ($null -eq $step) { continue }
        $toolName = [string]$step.tool_name
        if ($toolName -like 'run_*_material_op' -or $toolName -like 'run_*_niagara_op') {
            $opName = [string]$step.input.op_name
            if (-not [string]::IsNullOrWhiteSpace($opName)) {
                $steps += $opName
                continue
            }
        }
        if (-not [string]::IsNullOrWhiteSpace($toolName)) {
            $steps += $toolName
        }
    }
    return $steps
}

function Get-CompileLogResultsFromWorkflow {
    param($WorkflowResult)
    $results = @()
    foreach ($step in @((Get-OptionalPropertyValue -Object $WorkflowResult -PropertyName 'executed_steps'))) {
        $stepResult = Get-OptionalPropertyValue -Object $step -PropertyName 'result'
        if ($null -eq $step -or $null -eq $stepResult) { continue }
        $compileResult = Get-OptionalPropertyValue -Object $stepResult -PropertyName 'compile_result'
        if ($null -ne $compileResult) {
            $results += $compileResult
            continue
        }
        $compileDiagnostics = Get-OptionalPropertyValue -Object $stepResult -PropertyName 'compile_diagnostics'
        if ($null -ne $compileDiagnostics) {
            $results += $compileDiagnostics
        }
    }
    return $results
}

function Invoke-WorkflowWithRetry {
    param(
        [Parameter(Mandatory = $true)][string]$ToolName,
        [Parameter(Mandatory = $true)][hashtable]$InputObject,
        [int]$TimeoutSeconds = 480,
        [int]$MaxAttempts = 2
    )

    $lastError = $null
    for ($attempt = 1; $attempt -le $MaxAttempts; $attempt++) {
        try {
            return Normalize-BridgeResult (Invoke-ForgePilotTool -ToolName $ToolName -InputObject $InputObject -TimeoutSeconds $TimeoutSeconds)
        }
        catch {
            $lastError = $_
            if ($attempt -lt $MaxAttempts) {
                Start-Sleep -Seconds 2
                Wait-ForgePilotBridge
            }
        }
    }

    throw $lastError
}

if ([string]::IsNullOrWhiteSpace($OutputRoot)) {
    $savedRoot = Resolve-Path (Join-Path $PSScriptRoot '..\..\..\..\Saved')
    $OutputRoot = Join-Path $savedRoot.Path 'ForgePilot\EffectV2Eval'
}

$suitePaths = @()
if ([string]::IsNullOrWhiteSpace($SuitePath)) {
    $suitePaths = @(Get-DefaultSuitePaths)
}
else {
    $suitePaths = @($SuitePath)
}

foreach ($path in $suitePaths) {
    if (-not (Test-Path $path)) {
        throw "Eval suite not found: $path"
    }
}

$timestamp = Get-Date -Format 'yyyyMMdd_HHmmss'
$runRoot = Join-Path $OutputRoot $timestamp
New-Item -ItemType Directory -Force -Path $runRoot | Out-Null

Wait-ForgePilotBridge

$report = [ordered]@{
    suite_name = if ($suitePaths.Count -eq 1) { ((Get-Content -Raw -Path $suitePaths[0] | ConvertFrom-Json).suite_name) } else { 'effect_v2_eval' }
    timestamp = (Get-Date).ToString('o')
    workflow_tool_name = $WorkflowToolName
    run_semantic_ops = [bool]$RunSemanticOps
    keep_assets = [bool]$KeepAssets
    tasks = @()
}

foreach ($suiteFile in $suitePaths) {
    $suite = Get-Content -Raw -Path $suiteFile | ConvertFrom-Json
    foreach ($task in @($suite.tasks)) {
        $taskReport = [ordered]@{
            suite_name = $suite.suite_name
            id = $task.id
            description = $task.description
            chosen_archetype = $null
            expected_archetype = $task.expected_archetype
            semantic_op_sequence = @()
            planned_execution_sequence = @()
            compile_log_results = @()
            captures = @()
            critic_output = $null
            repair_decision = $null
            final_score = $null
            rebuild_count = 0
            pass = $false
            errors = @()
            created_assets = @()
        }

        $plan = Normalize-BridgeResult (Invoke-ForgePilotTool -ToolName 'resolve_effect_plan_v2' -InputObject @{
            description = [string]$task.description
        })

        $taskReport.chosen_archetype = $plan.resolved_archetype
        $taskReport.planned_execution_sequence = @($plan.execution_plan | ForEach-Object {
            if ($_.tool_name -like 'run_*_op') { [string]$_.input.op_name } else { [string]$_.tool_name }
        })

        if ($RunSemanticOps) {
            if ([string]::IsNullOrWhiteSpace($HeroMaterialPath) -or [string]::IsNullOrWhiteSpace($CoreMaterialPath)) {
                throw "RunSemanticOps requires -HeroMaterialPath and -CoreMaterialPath."
            }

            $taskToken = New-SafeNameToken -Value ([string]$task.id)
            $taskSavePath = '/Game/ForgePilot_Test/V2Platform'
            $suiteToken = New-SafeNameToken -Value ([string]$suite.suite_name)
            $timestampToken = Get-Date -Format 'HHmmssfff'
            $systemName = "NS_${suiteToken}_${taskToken}_Eval_$timestampToken"

            try {
                $workflow = Invoke-WorkflowWithRetry -ToolName $WorkflowToolName -InputObject @{
                    description = [string]$task.description
                    hero_parent_material_path = $HeroMaterialPath
                    core_parent_material_path = $CoreMaterialPath
                    save_path = $taskSavePath
                    system_name = $systemName
                    max_repair_iterations = 1
                    capture_thumbnail = $true
                    thumbnail_filename = "${taskToken}_eval_workflow_thumb"
                    keep_assets = [bool]$KeepAssets
                    apply_changes = $true
                } -TimeoutSeconds 480

                $taskReport.chosen_archetype = [string]$workflow.resolved_archetype
                $taskReport.semantic_op_sequence = @(Get-WorkflowStepNames -WorkflowResult $workflow)
                $taskReport.compile_log_results = @(Get-CompileLogResultsFromWorkflow -WorkflowResult $workflow)
                $taskReport.critic_output = Get-OptionalPropertyValue -Object $workflow -PropertyName 'critic_output'
                $taskReport.repair_decision = Get-OptionalPropertyValue -Object $workflow -PropertyName 'repair_decision'

                $rebuildCount = Get-OptionalPropertyValue -Object $workflow -PropertyName 'rebuild_count'
                if ($null -ne $rebuildCount) { $taskReport.rebuild_count = [int]$rebuildCount }

                $createdAssets = Get-OptionalPropertyValue -Object $workflow -PropertyName 'created_assets'
                if ($null -ne $createdAssets) { $taskReport.created_assets = @($createdAssets) }
                $deletedAssets = Get-OptionalPropertyValue -Object $workflow -PropertyName 'deleted_assets'
                if ($null -ne $deletedAssets) { $taskReport.deleted_assets = @($deletedAssets) }

                $thumbnailPath = Get-OptionalPropertyValue -Object $workflow -PropertyName 'thumbnail_path'
                if (-not [string]::IsNullOrWhiteSpace([string]$thumbnailPath)) {
                    $taskReport.captures += [string]$thumbnailPath
                }
            }
            catch {
                $taskReport.errors += $_.Exception.Message
            }
        }
        else {
            $taskReport.skipped_reason = 'RunSemanticOps not enabled; planner-only baseline'
        }

        if ($taskReport.critic_output -and $task.expected_absent_failure_labels) {
            $labels = @($taskReport.critic_output.failure_labels)
            $presentFailures = @($task.expected_absent_failure_labels | Where-Object { $labels -contains $_ })
            $archetypePenalty = if ($taskReport.chosen_archetype -ne $task.expected_archetype) { 5 } else { 0 }
            $taskReport.final_score = [Math]::Max(0, 25 - ($presentFailures.Count * 5) - $archetypePenalty)
            $taskReport.pass = ($presentFailures.Count -eq 0 -and $taskReport.chosen_archetype -eq $task.expected_archetype)
        }

        $report.tasks += $taskReport
    }
}

$passCount = @($report.tasks | Where-Object { $_.pass }).Count
$report.total_tasks = @($report.tasks).Count
$report.pass_count = $passCount
$report.fail_count = $report.total_tasks - $passCount

$reportPath = Join-Path $runRoot 'report.json'
$report | ConvertTo-Json -Depth 100 | Set-Content -Path $reportPath -Encoding UTF8
Write-Host "Effect V2 eval report written to $reportPath"
