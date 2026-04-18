[CmdletBinding()]
param(
    [string]$SavePath = "/Game/ForgePilot_Test/NodeProbe",
    [string]$MaterialNamePrefix = "M_NodeProbe_Material",
    [string]$OutputRoot = "",
    [switch]$KeepAsset
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

function Invoke-ProbeStep {
    param(
        [Parameter(Mandatory = $true)][string]$Label,
        [Parameter(Mandatory = $true)][string]$ToolName,
        [Parameter(Mandatory = $true)][hashtable]$InputObject,
        [int]$TimeoutSeconds = 60
    )

    try {
        $result = Normalize-BridgeResult (Invoke-ForgePilotTool -ToolName $ToolName -InputObject $InputObject -TimeoutSeconds $TimeoutSeconds)
        $message = if ($null -ne $result -and ($result.PSObject.Properties.Name -contains 'message')) {
            [string]$result.message
        }
        else {
            'ok'
        }

        return [pscustomobject][ordered]@{
            label = $Label
            tool = $ToolName
            pass = $true
            message = $message
            result = $result
        }
    }
    catch {
        return [pscustomobject][ordered]@{
            label = $Label
            tool = $ToolName
            pass = $false
            message = $_.Exception.Message
            result = $null
        }
    }
}

if ([string]::IsNullOrWhiteSpace($OutputRoot)) {
    $savedRoot = Resolve-Path (Join-Path $PSScriptRoot '..\..\..\..\Saved')
    $OutputRoot = Join-Path $savedRoot.Path 'ForgePilot\MaterialNodeProbe'
}

$timestamp = Get-Date -Format 'yyyyMMdd_HHmmss'
$runRoot = Join-Path $OutputRoot $timestamp
New-Item -ItemType Directory -Force -Path $runRoot | Out-Null

Wait-ForgePilotBridge

$materialName = '{0}_{1}' -f $MaterialNamePrefix, $timestamp
$createMaterial = Normalize-BridgeResult (Invoke-ForgePilotTool -ToolName 'create_material' -InputObject @{
    material_name = $materialName
    save_path = $SavePath
    base_color = @(0.1, 0.1, 0.1, 1.0)
})

$materialPath = [string]$createMaterial.asset_path
if ([string]::IsNullOrWhiteSpace($materialPath)) {
    throw "Failed to create probe material."
}

$steps = @(
    @{
        label = 'Add'
        tool = 'add_material_expression'
        input = @{
            material_path = $materialPath
            expression_type = 'Add'
            apply_changes = 'true'
        }
    },
    @{
        label = 'Subtract'
        tool = 'add_material_expression'
        input = @{
            material_path = $materialPath
            expression_type = 'Subtract'
            apply_changes = 'true'
        }
    },
    @{
        label = 'Multiply'
        tool = 'add_material_expression'
        input = @{
            material_path = $materialPath
            expression_type = 'Multiply'
            apply_changes = 'true'
        }
    },
    @{
        label = 'Divide'
        tool = 'add_material_expression'
        input = @{
            material_path = $materialPath
            expression_type = 'Divide'
            apply_changes = 'true'
        }
    },
    @{
        label = 'Power'
        tool = 'add_material_expression'
        input = @{
            material_path = $materialPath
            expression_type = 'Power'
            apply_changes = 'true'
        }
    },
    @{
        label = 'SquareRoot'
        tool = 'add_material_expression'
        input = @{
            material_path = $materialPath
            expression_type = 'SquareRoot'
            apply_changes = 'true'
        }
    },
    @{
        label = 'OneMinus'
        tool = 'add_material_expression'
        input = @{
            material_path = $materialPath
            expression_type = 'OneMinus'
            apply_changes = 'true'
        }
    },
    @{
        label = 'Abs'
        tool = 'add_material_expression'
        input = @{
            material_path = $materialPath
            expression_type = 'Abs'
            apply_changes = 'true'
        }
    },
    @{
        label = 'DepthFade'
        tool = 'add_material_expression'
        input = @{
            material_path = $materialPath
            expression_type = 'DepthFade'
            settings_json = '{"opacity_default":1.0,"fade_distance_default":64.0}'
            apply_changes = 'true'
        }
    },
    @{
        label = 'SceneDepth'
        tool = 'add_material_expression'
        input = @{
            material_path = $materialPath
            expression_type = 'SceneDepth'
            settings_json = '{"input_mode":"Coordinates","const_input":[0.5,0.5]}'
            apply_changes = 'true'
        }
    },
    @{
        label = 'PixelDepth'
        tool = 'add_material_expression'
        input = @{
            material_path = $materialPath
            expression_type = 'PixelDepth'
            apply_changes = 'true'
        }
    },
    @{
        label = 'DistanceToNearestSurface'
        tool = 'add_material_expression'
        input = @{
            material_path = $materialPath
            expression_type = 'DistanceToNearestSurface'
            apply_changes = 'true'
        }
    },
    @{
        label = 'ObjectPositionWS'
        tool = 'add_material_expression'
        input = @{
            material_path = $materialPath
            expression_type = 'ObjectPositionWS'
            apply_changes = 'true'
        }
    },
    @{
        label = 'ObjectRadius'
        tool = 'add_material_expression'
        input = @{
            material_path = $materialPath
            expression_type = 'ObjectRadius'
            apply_changes = 'true'
        }
    },
    @{
        label = 'Frac'
        tool = 'add_material_expression'
        input = @{
            material_path = $materialPath
            expression_type = 'Frac'
            apply_changes = 'true'
        }
    },
    @{
        label = 'Fmod'
        tool = 'add_material_expression'
        input = @{
            material_path = $materialPath
            expression_type = 'Fmod'
            apply_changes = 'true'
        }
    },
    @{
        label = 'Ceil'
        tool = 'add_material_expression'
        input = @{
            material_path = $materialPath
            expression_type = 'Ceil'
            apply_changes = 'true'
        }
    },
    @{
        label = 'Floor'
        tool = 'add_material_expression'
        input = @{
            material_path = $materialPath
            expression_type = 'Floor'
            apply_changes = 'true'
        }
    },
    @{
        label = 'Round'
        tool = 'add_material_expression'
        input = @{
            material_path = $materialPath
            expression_type = 'Round'
            apply_changes = 'true'
        }
    },
    @{
        label = 'Sign'
        tool = 'add_material_expression'
        input = @{
            material_path = $materialPath
            expression_type = 'Sign'
            apply_changes = 'true'
        }
    },
    @{
        label = 'Distance'
        tool = 'add_material_expression'
        input = @{
            material_path = $materialPath
            expression_type = 'Distance'
            apply_changes = 'true'
        }
    },
    @{
        label = 'Clamp'
        tool = 'add_material_expression'
        input = @{
            material_path = $materialPath
            expression_type = 'Clamp'
            apply_changes = 'true'
        }
    },
    @{
        label = 'Saturate'
        tool = 'add_material_expression'
        input = @{
            material_path = $materialPath
            expression_type = 'Saturate'
            apply_changes = 'true'
        }
    },
    @{
        label = 'Max'
        tool = 'add_material_expression'
        input = @{
            material_path = $materialPath
            expression_type = 'Max'
            apply_changes = 'true'
        }
    },
    @{
        label = 'Min'
        tool = 'add_material_expression'
        input = @{
            material_path = $materialPath
            expression_type = 'Min'
            apply_changes = 'true'
        }
    },
    @{
        label = 'Lerp'
        tool = 'add_material_expression'
        input = @{
            material_path = $materialPath
            expression_type = 'Lerp'
            apply_changes = 'true'
        }
    },
    @{
        label = 'Step'
        tool = 'add_material_expression'
        input = @{
            material_path = $materialPath
            expression_type = 'Step'
            apply_changes = 'true'
        }
    },
    @{
        label = 'SmoothStep'
        tool = 'add_material_expression'
        input = @{
            material_path = $materialPath
            expression_type = 'SmoothStep'
            apply_changes = 'true'
        }
    },
    @{
        label = 'If'
        tool = 'add_material_expression'
        input = @{
            material_path = $materialPath
            expression_type = 'If'
            settings_json = '{"equals_threshold":0.0001,"const_b":0.0}'
            apply_changes = 'true'
        }
    },
    @{
        label = 'InverseLerp'
        tool = 'add_material_expression'
        input = @{
            material_path = $materialPath
            expression_type = 'InverseLerp'
            settings_json = '{"const_a":0.0,"const_b":1.0,"const_value":0.5,"clamp_result":true}'
            apply_changes = 'true'
        }
    },
    @{
        label = 'Sine'
        tool = 'add_material_expression'
        input = @{
            material_path = $materialPath
            expression_type = 'Sine'
            apply_changes = 'true'
        }
    },
    @{
        label = 'Cosine'
        tool = 'add_material_expression'
        input = @{
            material_path = $materialPath
            expression_type = 'Cosine'
            settings_json = '{"period":1.0}'
            apply_changes = 'true'
        }
    },
    @{
        label = 'Tangent'
        tool = 'add_material_expression'
        input = @{
            material_path = $materialPath
            expression_type = 'Tangent'
            settings_json = '{"period":1.0}'
            apply_changes = 'true'
        }
    },
    @{
        label = 'Arctangent'
        tool = 'add_material_expression'
        input = @{
            material_path = $materialPath
            expression_type = 'Arctangent'
            apply_changes = 'true'
        }
    },
    @{
        label = 'Arcsine'
        tool = 'add_material_expression'
        input = @{
            material_path = $materialPath
            expression_type = 'Arcsine'
            apply_changes = 'true'
        }
    },
    @{
        label = 'Arccosine'
        tool = 'add_material_expression'
        input = @{
            material_path = $materialPath
            expression_type = 'Arccosine'
            apply_changes = 'true'
        }
    },
    @{
        label = 'Atan2'
        tool = 'add_material_expression'
        input = @{
            material_path = $materialPath
            expression_type = 'Atan2'
            apply_changes = 'true'
        }
    },
    @{
        label = 'StaticSwitchParameter'
        tool = 'add_material_expression'
        input = @{
            material_path = $materialPath
            expression_type = 'StaticSwitchParameter'
            param_name = 'UseDistort'
            settings_json = '{"default_value":true,"dynamic_branch":false}'
            apply_changes = 'true'
        }
    },
    @{
        label = 'FeatureLevelSwitch'
        tool = 'add_material_expression'
        input = @{
            material_path = $materialPath
            expression_type = 'FeatureLevelSwitch'
            apply_changes = 'true'
        }
    },
    @{
        label = 'PreSkinnedLocalPosition'
        tool = 'add_material_expression'
        input = @{
            material_path = $materialPath
            expression_type = 'PreSkinnedLocalPosition'
            apply_changes = 'true'
        }
    },
    @{
        label = 'VertexNormalWS'
        tool = 'add_material_expression'
        input = @{
            material_path = $materialPath
            expression_type = 'VertexNormalWS'
            apply_changes = 'true'
        }
    },
    @{
        label = 'WorldPosition'
        tool = 'add_material_expression'
        input = @{
            material_path = $materialPath
            expression_type = 'WorldPosition'
            apply_changes = 'true'
        }
    },
    @{
        label = 'DotProduct'
        tool = 'add_material_expression'
        input = @{
            material_path = $materialPath
            expression_type = 'DotProduct'
            apply_changes = 'true'
        }
    },
    @{
        label = 'CrossProduct'
        tool = 'add_material_expression'
        input = @{
            material_path = $materialPath
            expression_type = 'CrossProduct'
            apply_changes = 'true'
        }
    },
    @{
        label = 'Normalize'
        tool = 'add_material_expression'
        input = @{
            material_path = $materialPath
            expression_type = 'Normalize'
            apply_changes = 'true'
        }
    },
    @{
        label = 'VectorLength'
        tool = 'add_material_expression'
        input = @{
            material_path = $materialPath
            expression_type = 'VectorLength'
            apply_changes = 'true'
        }
    },
    @{
        label = 'ReflectionVector'
        tool = 'add_material_expression'
        input = @{
            material_path = $materialPath
            expression_type = 'ReflectionVector'
            apply_changes = 'true'
        }
    },
    @{
        label = 'Transform'
        tool = 'add_material_expression'
        input = @{
            material_path = $materialPath
            expression_type = 'Transform'
            settings_json = '{"source":"Local","destination":"World"}'
            apply_changes = 'true'
        }
    },
    @{
        label = 'TransformPosition'
        tool = 'add_material_expression'
        input = @{
            material_path = $materialPath
            expression_type = 'TransformPosition'
            settings_json = '{"source":"Local","destination":"World"}'
            apply_changes = 'true'
        }
    },
    @{
        label = 'ComponentMask'
        tool = 'add_material_expression'
        input = @{
            material_path = $materialPath
            expression_type = 'ComponentMask'
            apply_changes = 'true'
        }
    },
    @{
        label = 'AppendVector'
        tool = 'add_material_expression'
        input = @{
            material_path = $materialPath
            expression_type = 'AppendVector'
            apply_changes = 'true'
        }
    },
    @{
        label = 'VectorToRadialValue'
        tool = 'add_material_expression'
        input = @{
            material_path = $materialPath
            expression_type = 'MaterialFunctionCall'
            function_path = '/Engine/Functions/Engine_MaterialFunctions02/Utility/VectorToRadialValue.VectorToRadialValue'
            apply_changes = 'true'
        }
    },
    @{
        label = 'BoundingBoxBased_0-1_UVW'
        tool = 'add_material_expression'
        input = @{
            material_path = $materialPath
            expression_type = 'MaterialFunctionCall'
            function_path = '/Engine/Functions/Engine_MaterialFunctions02/UVs/BoundingBoxBased_0-1_UVW.BoundingBoxBased_0-1_UVW'
            apply_changes = 'true'
        }
    },
    @{
        label = 'CheapContrast'
        tool = 'add_material_expression'
        input = @{
            material_path = $materialPath
            expression_type = 'MaterialFunctionCall'
            function_path = '/Engine/Functions/Engine_MaterialFunctions01/ImageAdjustment/CheapContrast.CheapContrast'
            apply_changes = 'true'
        }
    },
    @{
        label = 'RemapValueRange'
        tool = 'add_material_expression'
        input = @{
            material_path = $materialPath
            expression_type = 'MaterialFunctionCall'
            function_path = '/Engine/Functions/Engine_MaterialFunctions03/Math/RemapValueRange.RemapValueRange'
            apply_changes = 'true'
        }
    },
    @{
        label = 'BreakOutFloat2Components'
        tool = 'add_material_expression'
        input = @{
            material_path = $materialPath
            expression_type = 'MaterialFunctionCall'
            function_path = '/Engine/Functions/Engine_MaterialFunctions02/Utility/BreakOutFloat2Components.BreakOutFloat2Components'
            apply_changes = 'true'
        }
    },
    @{
        label = 'BreakOutFloat3Components'
        tool = 'add_material_expression'
        input = @{
            material_path = $materialPath
            expression_type = 'MaterialFunctionCall'
            function_path = '/Engine/Functions/Engine_MaterialFunctions02/Utility/BreakOutFloat3Components.BreakOutFloat3Components'
            apply_changes = 'true'
        }
    },
    @{
        label = 'BreakOutFloat4Components'
        tool = 'add_material_expression'
        input = @{
            material_path = $materialPath
            expression_type = 'MaterialFunctionCall'
            function_path = '/Engine/Functions/Engine_MaterialFunctions02/Utility/BreakOutFloat4Components.BreakOutFloat4Components'
            apply_changes = 'true'
        }
    },
    @{
        label = 'MakeFloat2'
        tool = 'add_material_expression'
        input = @{
            material_path = $materialPath
            expression_type = 'MaterialFunctionCall'
            function_path = '/Engine/Functions/Engine_MaterialFunctions02/Utility/MakeFloat2.MakeFloat2'
            apply_changes = 'true'
        }
    },
    @{
        label = 'MakeFloat3'
        tool = 'add_material_expression'
        input = @{
            material_path = $materialPath
            expression_type = 'MaterialFunctionCall'
            function_path = '/Engine/Functions/Engine_MaterialFunctions02/Utility/MakeFloat3.MakeFloat3'
            apply_changes = 'true'
        }
    },
    @{
        label = 'MakeFloat4'
        tool = 'add_material_expression'
        input = @{
            material_path = $materialPath
            expression_type = 'MaterialFunctionCall'
            function_path = '/Engine/Functions/Engine_MaterialFunctions02/Utility/MakeFloat4.MakeFloat4'
            apply_changes = 'true'
        }
    },
    @{
        label = 'FlipBook_UniformNonUniform'
        tool = 'add_material_expression'
        input = @{
            material_path = $materialPath
            expression_type = 'MaterialFunctionCall'
            function_path = '/Engine/Functions/Engine_MaterialFunctions02/Texturing/FlipBook_UniformNonUniform.FlipBook_UniformNonUniform'
            apply_changes = 'true'
        }
    },
    @{
        label = 'CustomNode'
        tool = 'set_material_custom_node'
        input = @{
            material_path = $materialPath
            description = 'AdvancedNodeProbeCustom'
            code = 'return InputA;'
            output_type = 'float1'
            input_names = @('InputA')
            allow_custom_hlsl = 'true'
            apply_changes = 'true'
        }
    }
)

$results = @()
$vertexNormalIndex = $null

foreach ($step in $steps) {
    $stepResult = Invoke-ProbeStep -Label $step.label -ToolName $step.tool -InputObject $step.input
    $results += $stepResult

    if ($step.label -eq 'VertexNormalWS' -and $stepResult.pass -and $null -ne $stepResult.result) {
        if ($stepResult.result.PSObject.Properties.Name -contains 'expression_index') {
            $vertexNormalIndex = [int]$stepResult.result.expression_index
        }
    }
}

if ($null -ne $vertexNormalIndex) {
    $results += Invoke-ProbeStep -Label 'WorldPositionOffsetRootPin' -ToolName 'connect_material_nodes' -InputObject @{
        material_path = $materialPath
        source_index = $vertexNormalIndex
        source_output_index = 0
        target_type = 'material_pin'
        target_pin = 'WorldPositionOffset'
        apply_changes = 'true'
    }
}
else {
    $results += [pscustomobject][ordered]@{
        label = 'WorldPositionOffsetRootPin'
        tool = 'connect_material_nodes'
        pass = $false
        message = 'Skipped because VertexNormalWS did not produce an expression index.'
        result = $null
    }
}

$passCount = @($results | Where-Object { $_.pass }).Count
$failures = @($results | Where-Object { -not $_.pass })

$report = [ordered]@{
    timestamp = (Get-Date).ToString('o')
    material_path = $materialPath
    keep_asset = [bool]$KeepAsset
    total_steps = @($results).Count
    pass_count = $passCount
    fail_count = @($failures).Count
    all_passed = (@($failures).Count -eq 0)
    steps = $results
}

$reportPath = Join-Path $runRoot 'report.json'
$report | ConvertTo-Json -Depth 100 | Set-Content -Path $reportPath -Encoding UTF8

Write-Host "Material node probe report written to $reportPath"
Write-Host "Probe material: $materialPath"
Write-Host ("Pass: {0}  Fail: {1}" -f $report.pass_count, $report.fail_count)

if (-not $KeepAsset) {
    try {
        $deleteResult = Normalize-BridgeResult (Invoke-ForgePilotTool -ToolName 'delete_asset' -InputObject @{
            asset_path = $materialPath
        })
        $deletePath = Join-Path $runRoot 'delete_result.json'
        $deleteResult | ConvertTo-Json -Depth 50 | Set-Content -Path $deletePath -Encoding UTF8
        Write-Host "Deleted probe asset and wrote delete log to $deletePath"
    }
    catch {
        Write-Warning ("Failed to delete probe asset {0}: {1}" -f $materialPath, $_.Exception.Message)
    }
}

if (-not $report.all_passed) {
    exit 1
}
