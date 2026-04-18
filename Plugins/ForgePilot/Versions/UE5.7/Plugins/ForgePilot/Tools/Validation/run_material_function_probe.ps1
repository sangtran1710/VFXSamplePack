[CmdletBinding()]
param(
    [string]$SavePath = "/Game/ForgePilot_Test/FunctionProbe",
    [string]$MaterialNamePrefix = "M_FunctionProbe_Material",
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
    $OutputRoot = Join-Path $savedRoot.Path 'ForgePilot\MaterialFunctionProbe'
}

$timestamp = Get-Date -Format 'yyyyMMdd_HHmmss'
$runRoot = Join-Path $OutputRoot $timestamp
New-Item -ItemType Directory -Force -Path $runRoot | Out-Null

Wait-ForgePilotBridge

$materialName = '{0}_{1}' -f $MaterialNamePrefix, $timestamp
$createMaterial = Normalize-BridgeResult (Invoke-ForgePilotTool -ToolName 'create_material' -InputObject @{
    material_name = $materialName
    save_path = $SavePath
    base_color = @(0.02, 0.02, 0.02, 1.0)
})

$materialPath = [string]$createMaterial.asset_path
if ([string]::IsNullOrWhiteSpace($materialPath)) {
    throw "Failed to create material function probe material."
}

$functions = @(
    @{
        label = 'VectorToRadialValue'
        path = '/Engine/Functions/Engine_MaterialFunctions02/Utility/VectorToRadialValue.VectorToRadialValue'
        category = 'coordinates_radial'
    },
    @{
        label = 'LongLatToUV'
        path = '/Engine/Functions/Engine_MaterialFunctions01/Coordinates/LongLatToUV.LongLatToUV'
        category = 'coordinates_spherical'
    },
    @{
        label = 'BoundingBoxBased_0-1_UVW'
        path = '/Engine/Functions/Engine_MaterialFunctions02/UVs/BoundingBoxBased_0-1_UVW.BoundingBoxBased_0-1_UVW'
        category = 'coordinates_object_bounds'
    },
    @{
        label = 'CameraDepthFade'
        path = '/Engine/Functions/Engine_MaterialFunctions01/Opacity/CameraDepthFade.CameraDepthFade'
        category = 'depth_opacity'
    },
    @{
        label = 'CheapContrast'
        path = '/Engine/Functions/Engine_MaterialFunctions01/ImageAdjustment/CheapContrast.CheapContrast'
        category = 'mask_shaping'
    },
    @{
        label = 'CheapContrast_RGB'
        path = '/Engine/Functions/Engine_MaterialFunctions01/ImageAdjustment/CheapContrast_RGB.CheapContrast_RGB'
        category = 'mask_shaping'
    },
    @{
        label = 'RemapValueRange'
        path = '/Engine/Functions/Engine_MaterialFunctions03/Math/RemapValueRange.RemapValueRange'
        category = 'math_range'
    },
    @{
        label = 'RemapValueRangeNormalized'
        path = '/Engine/Functions/Engine_MaterialFunctions03/Math/RemapValueRangeNormalized.RemapValueRangeNormalized'
        category = 'math_range'
    },
    @{
        label = 'FlipBook_UniformNonUniform'
        path = '/Engine/Functions/Engine_MaterialFunctions02/Texturing/FlipBook_UniformNonUniform.FlipBook_UniformNonUniform'
        category = 'flipbook'
    },
    @{
        label = 'FlipBook_MotionVectors'
        path = '/Engine/Functions/Engine_MaterialFunctions02/Texturing/FlipBook_MotionVectors.FlipBook_MotionVectors'
        category = 'flipbook'
    },
    @{
        label = 'SubUV_Function'
        path = '/Engine/Functions/Engine_MaterialFunctions03/Texturing/SubUV_Function.SubUV_Function'
        category = 'flipbook'
    },
    @{
        label = 'SubUV_Function_MipDerivative'
        path = '/Engine/Functions/Engine_MaterialFunctions03/Texturing/SubUV_Function_MipDerivative.SubUV_Function_MipDerivative'
        category = 'flipbook'
    },
    @{
        label = 'BreakOutFloat2Components'
        path = '/Engine/Functions/Engine_MaterialFunctions02/Utility/BreakOutFloat2Components.BreakOutFloat2Components'
        category = 'component_packing'
    },
    @{
        label = 'BreakOutFloat3Components'
        path = '/Engine/Functions/Engine_MaterialFunctions02/Utility/BreakOutFloat3Components.BreakOutFloat3Components'
        category = 'component_packing'
    },
    @{
        label = 'BreakOutFloat4Components'
        path = '/Engine/Functions/Engine_MaterialFunctions02/Utility/BreakOutFloat4Components.BreakOutFloat4Components'
        category = 'component_packing'
    },
    @{
        label = 'MakeFloat2'
        path = '/Engine/Functions/Engine_MaterialFunctions02/Utility/MakeFloat2.MakeFloat2'
        category = 'component_packing'
    },
    @{
        label = 'MakeFloat3'
        path = '/Engine/Functions/Engine_MaterialFunctions02/Utility/MakeFloat3.MakeFloat3'
        category = 'component_packing'
    },
    @{
        label = 'MakeFloat4'
        path = '/Engine/Functions/Engine_MaterialFunctions02/Utility/MakeFloat4.MakeFloat4'
        category = 'component_packing'
    },
    @{
        label = 'Blend_Screen'
        path = '/Engine/Functions/Engine_MaterialFunctions03/Blends/Blend_Screen.Blend_Screen'
        category = 'blend_modes'
    },
    @{
        label = 'Blend_Overlay'
        path = '/Engine/Functions/Engine_MaterialFunctions03/Blends/Blend_Overlay.Blend_Overlay'
        category = 'blend_modes'
    },
    @{
        label = 'Blend_ColorDodge'
        path = '/Engine/Functions/Engine_MaterialFunctions03/Blends/Blend_ColorDodge.Blend_ColorDodge'
        category = 'blend_modes'
    },
    @{
        label = 'Blend_LinearDodge'
        path = '/Engine/Functions/Engine_MaterialFunctions03/Blends/Blend_LinearDodge.Blend_LinearDodge'
        category = 'blend_modes'
    },
    @{
        label = 'WorldAlignedTexture'
        path = '/Engine/Functions/Engine_MaterialFunctions01/Texturing/WorldAlignedTexture.WorldAlignedTexture'
        category = 'world_aligned'
    },
    @{
        label = 'WorldAlignedNormal'
        path = '/Engine/Functions/Engine_MaterialFunctions01/Texturing/WorldAlignedNormal.WorldAlignedNormal'
        category = 'world_aligned'
    },
    @{
        label = 'WorldAlignedBlend'
        path = '/Engine/Functions/Engine_MaterialFunctions01/AlphaBlend/WorldAlignedBlend.WorldAlignedBlend'
        category = 'world_aligned'
    },
    @{
        label = 'HueShift'
        path = '/Engine/Functions/Engine_MaterialFunctions02/HueShift.HueShift'
        category = 'color_adjust'
    },
    @{
        label = 'TextureVariation'
        path = '/Engine/Functions/Engine_MaterialFunctions03/Texturing/TextureVariation.TextureVariation'
        category = 'texture_variation'
    },
    @{
        label = 'CameraWorldBlend'
        path = '/Engine/Functions/Engine_MaterialFunctions03/Texturing/CameraWorldBlend.CameraWorldBlend'
        category = 'camera_blend'
    }
)

$results = @()

foreach ($function in $functions) {
    $functionPath = [string]$function.path
    $label = [string]$function.label

    $infoResult = Invoke-ProbeStep -Label "$label info" -ToolName 'get_material_function_info' -InputObject @{
        material_function_path = $functionPath
    }

    $callResult = Invoke-ProbeStep -Label "$label call" -ToolName 'add_material_expression' -InputObject @{
        material_path = $materialPath
        expression_type = 'MaterialFunctionCall'
        function_path = $functionPath
        apply_changes = 'true'
    }

    $results += [pscustomobject][ordered]@{
        label = $label
        category = [string]$function.category
        function_path = $functionPath
        info_pass = [bool]$infoResult.pass
        call_pass = [bool]$callResult.pass
        pass = ([bool]$infoResult.pass -and [bool]$callResult.pass)
        info_message = [string]$infoResult.message
        call_message = [string]$callResult.message
        info = $infoResult.result
        call = $callResult.result
    }
}

$passCount = @($results | Where-Object { $_.pass }).Count
$failures = @($results | Where-Object { -not $_.pass })

$report = [ordered]@{
    timestamp = (Get-Date).ToString('o')
    material_path = $materialPath
    keep_asset = [bool]$KeepAsset
    total_functions = @($results).Count
    pass_count = $passCount
    fail_count = @($failures).Count
    all_passed = (@($failures).Count -eq 0)
    functions = $results
}

$reportPath = Join-Path $runRoot 'report.json'
$report | ConvertTo-Json -Depth 100 | Set-Content -Path $reportPath -Encoding UTF8

Write-Host "Material function probe report written to $reportPath"
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
