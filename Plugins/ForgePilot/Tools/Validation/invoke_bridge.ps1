[CmdletBinding(DefaultParameterSetName = 'Tool')]
param(
    [Parameter(ParameterSetName = 'Ping')]
    [switch]$Ping,

    [Parameter(ParameterSetName = 'ListTools')]
    [switch]$ListTools,

    [Parameter(ParameterSetName = 'Tool')]
    [string]$ToolName,

    [Parameter(ParameterSetName = 'Tool')]
    [string]$ArgsJson = '{}',

    [string]$BridgeHost = '',
    [int]$BridgePort = 0,
    [int]$TimeoutSeconds = 30
)

function Get-ForgePilotBridgeHostDefault {
    if (-not [string]::IsNullOrWhiteSpace($env:FORGEPILOT_BRIDGE_HOST)) {
        return $env:FORGEPILOT_BRIDGE_HOST
    }

    return "127.0.0.1"
}

function Get-ForgePilotBridgeProjectRoot {
    $cursor = [System.IO.Path]::GetFullPath($PSScriptRoot)
    while (-not [string]::IsNullOrWhiteSpace($cursor)) {
        $uproject = Get-ChildItem -LiteralPath $cursor -Filter *.uproject -File -ErrorAction SilentlyContinue | Select-Object -First 1
        if ($null -ne $uproject) {
            return $cursor
        }

        $parent = Split-Path -Parent $cursor
        if ([string]::IsNullOrWhiteSpace($parent) -or $parent -eq $cursor) {
            break
        }
        $cursor = $parent
    }

    return (Get-Location).Path
}

function ConvertTo-ForgePilotBridgeNormalizedPath {
    param([Parameter(Mandatory = $true)][string]$Path)

    $resolved = [System.IO.Path]::GetFullPath($Path)
    return ($resolved -replace '\\', '/').TrimEnd('/')
}

function Get-ForgePilotBridgeProjectDirDefault {
    return ConvertTo-ForgePilotBridgeNormalizedPath -Path (Get-ForgePilotBridgeProjectRoot)
}

function Get-ForgePilotBridgeProjectNameDefault {
    $projectRoot = Get-ForgePilotBridgeProjectRoot
    $uproject = Get-ChildItem -LiteralPath $projectRoot -Filter *.uproject -File -ErrorAction SilentlyContinue | Select-Object -First 1
    if ($null -ne $uproject) {
        return $uproject.BaseName
    }

    return [System.IO.Path]::GetFileName($projectRoot)
}

function Get-ForgePilotBridgeWorkspaceHashHex {
    $normalizedProjectDir = (Get-ForgePilotBridgeProjectDirDefault).ToLowerInvariant()
    $md5 = [System.Security.Cryptography.MD5]::Create()
    try {
        $hashBytes = $md5.ComputeHash([System.Text.Encoding]::UTF8.GetBytes($normalizedProjectDir))
    }
    finally {
        $md5.Dispose()
    }

    return ([System.BitConverter]::ToString($hashBytes)).Replace('-', '').ToLowerInvariant()
}

function Get-ForgePilotBridgeWorkspaceIdDefault {
    $hashHex = Get-ForgePilotBridgeWorkspaceHashHex
    return "fpw_$($hashHex.Substring(0, 12))"
}

function Get-ForgePilotBridgePortDefault {
    if ([string]::IsNullOrWhiteSpace($env:FORGEPILOT_BRIDGE_PORT)) {
        $hashHex = Get-ForgePilotBridgeWorkspaceHashHex
        $hashValue = [Convert]::ToUInt32($hashHex.Substring(0, 8), 16)
        return 19080 + ($hashValue % 1000)
    }

    $parsedPort = 0
    if ([int]::TryParse($env:FORGEPILOT_BRIDGE_PORT, [ref]$parsedPort) -and $parsedPort -gt 0 -and $parsedPort -le 65535) {
        return $parsedPort
    }

    $hashHex = Get-ForgePilotBridgeWorkspaceHashHex
    $hashValue = [Convert]::ToUInt32($hashHex.Substring(0, 8), 16)
    return 19080 + ($hashValue % 1000)
}

function Add-ForgePilotBridgeIdentityFields {
    param(
        [Parameter(Mandatory = $true)][hashtable]$Payload
    )

    if (-not $Payload.ContainsKey('expected_workspace_id')) {
        $Payload['expected_workspace_id'] = Get-ForgePilotBridgeWorkspaceIdDefault
    }
    if (-not $Payload.ContainsKey('expected_project_dir')) {
        $Payload['expected_project_dir'] = Get-ForgePilotBridgeProjectDirDefault
    }
    if (-not $Payload.ContainsKey('expected_project_name')) {
        $Payload['expected_project_name'] = Get-ForgePilotBridgeProjectNameDefault
    }

    return $Payload
}

function Invoke-ForgePilotBridgeRequest {
    param(
        [string]$BridgeHost = $(Get-ForgePilotBridgeHostDefault),
        [int]$BridgePort = $(Get-ForgePilotBridgePortDefault),
        [Parameter(Mandatory = $true)]$Payload,
        [int]$TimeoutSeconds = 30
    )

    $client = [System.Net.Sockets.TcpClient]::new()
    try {
        $connectTask = $client.ConnectAsync($BridgeHost, $BridgePort)
        if (-not $connectTask.Wait([TimeSpan]::FromSeconds($TimeoutSeconds))) {
            throw "Timed out connecting to ForgePilot bridge at ${BridgeHost}:$BridgePort"
        }

        $client.ReceiveTimeout = $TimeoutSeconds * 1000
        $client.SendTimeout = $TimeoutSeconds * 1000

        $stream = $client.GetStream()
        $writer = [System.IO.StreamWriter]::new($stream, [System.Text.UTF8Encoding]::new($false), 4096, $true)
        $writer.NewLine = "`n"
        $writer.AutoFlush = $true
        $reader = [System.IO.StreamReader]::new($stream, [System.Text.Encoding]::UTF8, $false, 4096, $true)

        $identityAwarePayload = Add-ForgePilotBridgeIdentityFields -Payload $Payload
        $jsonPayload = $identityAwarePayload | ConvertTo-Json -Depth 100 -Compress
        $writer.WriteLine($jsonPayload)
        $line = $reader.ReadLine()
        if ([string]::IsNullOrWhiteSpace($line)) {
            throw "Bridge closed the connection without a response."
        }

        return ($line | ConvertFrom-Json)
    }
    finally {
        $client.Dispose()
    }
}

function Wait-ForgePilotBridge {
    param(
        [string]$BridgeHost = $(Get-ForgePilotBridgeHostDefault),
        [int]$BridgePort = $(Get-ForgePilotBridgePortDefault),
        [int]$TimeoutSeconds = 30
    )

    $deadline = (Get-Date).AddSeconds($TimeoutSeconds)
    while ((Get-Date) -lt $deadline) {
        try {
            $response = Invoke-ForgePilotBridgeRequest -BridgeHost $BridgeHost -BridgePort $BridgePort -Payload @{ action = "ping" } -TimeoutSeconds 5
            if ($response.ok) {
                return
            }
        }
        catch {
        }
        Start-Sleep -Milliseconds 500
    }

    throw "Timed out waiting for ForgePilot bridge at ${BridgeHost}:$BridgePort"
}

function Get-ForgePilotBridgeToolNames {
    param(
        [string]$BridgeHost = $(Get-ForgePilotBridgeHostDefault),
        [int]$BridgePort = $(Get-ForgePilotBridgePortDefault),
        [int]$TimeoutSeconds = 30
    )

    $response = Invoke-ForgePilotBridgeRequest -BridgeHost $BridgeHost -BridgePort $BridgePort -Payload @{ action = "list_tools" } -TimeoutSeconds $TimeoutSeconds
    if (-not $response.ok) {
        throw "Bridge list_tools failed: $($response | ConvertTo-Json -Depth 20 -Compress)"
    }

    $names = @()
    foreach ($tool in @($response.tools)) {
        if ($tool -and $tool.name) {
            $names += [string]$tool.name
        }
    }
    return $names
}

function Invoke-ForgePilotTool {
    param(
        [string]$BridgeHost = $(Get-ForgePilotBridgeHostDefault),
        [int]$BridgePort = $(Get-ForgePilotBridgePortDefault),
        [Parameter(Mandatory = $true)][string]$ToolName,
        [hashtable]$InputObject = @{},
        [int]$TimeoutSeconds = 30
    )

    $response = Invoke-ForgePilotBridgeRequest -BridgeHost $BridgeHost -BridgePort $BridgePort -TimeoutSeconds $TimeoutSeconds -Payload @{
        action = "execute_tool"
        tool_name = $ToolName
        input = $InputObject
    }
    if (-not $response.ok) {
        throw "Bridge tool '$ToolName' failed: $($response | ConvertTo-Json -Depth 100 -Compress)"
    }
    return $response.result
}

function ConvertTo-ForgePilotHashtable {
    param(
        [Parameter(Mandatory = $true)]$InputObject
    )

    if ($null -eq $InputObject) {
        return @{}
    }

    if ($InputObject -is [System.Collections.IDictionary]) {
        $result = @{}
        foreach ($key in $InputObject.Keys) {
            $value = $InputObject[$key]
            if ($value -is [System.Collections.IDictionary] -or ($value -is [pscustomobject])) {
                $result[[string]$key] = ConvertTo-ForgePilotHashtable -InputObject $value
            }
            elseif ($value -is [System.Collections.IEnumerable] -and -not ($value -is [string])) {
                $items = @()
                foreach ($item in $value) {
                    if ($item -is [System.Collections.IDictionary] -or ($item -is [pscustomobject])) {
                        $items += ,(ConvertTo-ForgePilotHashtable -InputObject $item)
                    }
                    else {
                        $items += ,$item
                    }
                }
                $result[[string]$key] = $items
            }
            else {
                $result[[string]$key] = $value
            }
        }
        return $result
    }

    if ($InputObject -is [pscustomobject]) {
        $result = @{}
        foreach ($property in $InputObject.PSObject.Properties) {
            $value = $property.Value
            if ($value -is [System.Collections.IDictionary] -or ($value -is [pscustomobject])) {
                $result[$property.Name] = ConvertTo-ForgePilotHashtable -InputObject $value
            }
            elseif ($value -is [System.Collections.IEnumerable] -and -not ($value -is [string])) {
                $items = @()
                foreach ($item in $value) {
                    if ($item -is [System.Collections.IDictionary] -or ($item -is [pscustomobject])) {
                        $items += ,(ConvertTo-ForgePilotHashtable -InputObject $item)
                    }
                    else {
                        $items += ,$item
                    }
                }
                $result[$property.Name] = $items
            }
            else {
                $result[$property.Name] = $value
            }
        }
        return $result
    }

    throw "Expected an object-like value that can be converted to a hashtable."
}

function Invoke-ForgePilotBridgeCli {
    param(
        [string]$EffectiveBridgeHost,
        [int]$EffectiveBridgePort,
        [int]$EffectiveTimeoutSeconds,
        [switch]$UsePing,
        [switch]$UseListTools,
        [string]$EffectiveToolName,
        [string]$EffectiveArgsJson
    )

    $selectedModes = @(
        [bool]$UsePing,
        [bool]$UseListTools,
        (-not [string]::IsNullOrWhiteSpace($EffectiveToolName))
    ) | Where-Object { $_ }

    if ($selectedModes.Count -ne 1) {
        throw "Choose exactly one of -Ping, -ListTools, or -ToolName."
    }

    if ($UsePing) {
        $response = Invoke-ForgePilotBridgeRequest -BridgeHost $EffectiveBridgeHost -BridgePort $EffectiveBridgePort -Payload @{ action = 'ping' } -TimeoutSeconds $EffectiveTimeoutSeconds
        $response | ConvertTo-Json -Depth 100
        return
    }

    if ($UseListTools) {
        $response = Invoke-ForgePilotBridgeRequest -BridgeHost $EffectiveBridgeHost -BridgePort $EffectiveBridgePort -Payload @{ action = 'list_tools' } -TimeoutSeconds $EffectiveTimeoutSeconds
        $response | ConvertTo-Json -Depth 100
        return
    }

    $toolInput = @{}
    if (-not [string]::IsNullOrWhiteSpace($EffectiveArgsJson)) {
        $parsedJson = $EffectiveArgsJson | ConvertFrom-Json
        $toolInput = ConvertTo-ForgePilotHashtable -InputObject $parsedJson
    }

    $result = Invoke-ForgePilotTool -BridgeHost $EffectiveBridgeHost -BridgePort $EffectiveBridgePort -ToolName $EffectiveToolName -InputObject $toolInput -TimeoutSeconds $EffectiveTimeoutSeconds
    $result | ConvertTo-Json -Depth 100
}

if ($MyInvocation.InvocationName -ne '.') {
    try {
        $effectiveBridgeHost = if ([string]::IsNullOrWhiteSpace($BridgeHost)) { Get-ForgePilotBridgeHostDefault } else { $BridgeHost }
        $effectiveBridgePort = if ($BridgePort -le 0) { Get-ForgePilotBridgePortDefault } else { $BridgePort }

        Invoke-ForgePilotBridgeCli `
            -EffectiveBridgeHost $effectiveBridgeHost `
            -EffectiveBridgePort $effectiveBridgePort `
            -EffectiveTimeoutSeconds $TimeoutSeconds `
            -UsePing:$Ping `
            -UseListTools:$ListTools `
            -EffectiveToolName $ToolName `
            -EffectiveArgsJson $ArgsJson
    }
    catch {
        Write-Error $_
        exit 1
    }
}
