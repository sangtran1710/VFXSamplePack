param(
    [string]$BridgeHost = "127.0.0.1",
    [int]$BridgePort = 0
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

if (-not (Test-Path "$PSScriptRoot\invoke_bridge.ps1")) {
    throw "Required bridge helper was not found: $PSScriptRoot\invoke_bridge.ps1"
}

. "$PSScriptRoot\invoke_bridge.ps1"
if ($BridgePort -le 0) { $BridgePort = Get-ForgePilotBridgePortDefault }

Wait-ForgePilotBridge -BridgeHost $BridgeHost -BridgePort $BridgePort -TimeoutSeconds 20

$MaterialName = "M_ForgePilot_IceSurface_A01"
$InstanceName = "MI_ForgePilot_IceSurface_A01_Review"
$SavePath = "/Game/ForgePilot_Test/Materials/Ice"
$MaterialObjectPath = "$SavePath/$MaterialName.$MaterialName"
$InstanceObjectPath = "$SavePath/$InstanceName.$InstanceName"
$OutputDir = Join-Path "$env:USERPROFILE\Downloads\ForgePilotMaterialProbes" "ice_surface_a01_rebuild"
$ProbeActorLabel = "ForgePilot_IceSurface_A01_RebuildProbe"

$DeepCloudTexturePath = "/Game/ImpactParticlesVol1/Textures/T_CloudsBlur.T_CloudsBlur"
$NormalTexturePath = "/Game/GunFX/Textures/PBR/Decals/Ice/T_Ice_Decal_normal_2.T_Ice_Decal_normal_2"

New-Item -ItemType Directory -Force -Path $OutputDir | Out-Null

function Invoke-IceTool {
    param(
        [Parameter(Mandatory = $true)][string]$ToolName,
        [hashtable]$InputObject = @{},
        [int]$TimeoutSeconds = 30
    )

    return Invoke-ForgePilotTool -BridgeHost $BridgeHost -BridgePort $BridgePort -ToolName $ToolName -InputObject $InputObject -TimeoutSeconds $TimeoutSeconds
}

function Add-IceExpr {
    param(
        [Parameter(Mandatory = $true)][System.Collections.IDictionary]$Indices,
        [Parameter(Mandatory = $true)][string]$Key,
        [Parameter(Mandatory = $true)][string]$Type,
        [Parameter(Mandatory = $true)][int]$X,
        [Parameter(Mandatory = $true)][int]$Y,
        [string]$ParamName = "",
        [string]$DefaultValue = "",
        [string]$TexturePath = "",
        [hashtable]$Settings = @{}
    )

    $Input = @{
        material_path = $MaterialObjectPath
        expression_type = $Type
        editor_x = $X
        editor_y = $Y
        apply_changes = $true
    }
    if ($ParamName) { $Input.param_name = $ParamName }
    if ($DefaultValue) { $Input.default_value = $DefaultValue }
    if ($TexturePath) { $Input.texture_path = $TexturePath }
    if ($Settings.Count -gt 0) { $Input.settings = $Settings }

    $Result = Invoke-IceTool -ToolName "add_material_expression" -InputObject $Input
    $Indices[$Key] = [int]$Result.expression_index
    return $Indices[$Key]
}

function Connect-Ice {
    param(
        [Parameter(Mandatory = $true)][int]$SourceIndex,
        [Parameter(Mandatory = $true)][string]$TargetType,
        [int]$SourceOutputIndex = 0,
        [string]$TargetPin = "",
        [int]$TargetIndex = -1,
        [int]$TargetInputIndex = -1,
        [string]$TargetInputName = ""
    )

    $Input = @{
        material_path = $MaterialObjectPath
        source_index = $SourceIndex
        source_output_index = $SourceOutputIndex
        target_type = $TargetType
        apply_changes = $true
    }

    if ($TargetType -eq "material_pin") {
        $Input.target_pin = $TargetPin
    }
    else {
        $Input.target_index = $TargetIndex
        if ($TargetInputName) {
            $Input.target_input_name = $TargetInputName
        }
        else {
            $Input.target_input_index = $TargetInputIndex
        }
    }

    Invoke-IceTool -ToolName "connect_material_nodes" -InputObject $Input | Out-Null
}

function Json-ObjectString {
    param([hashtable]$Object)
    return ($Object | ConvertTo-Json -Depth 10 -Compress)
}

Write-Host "`n=== ICE SURFACE MATERIAL REBUILD ===" -ForegroundColor Cyan

foreach ($AssetPath in @($InstanceObjectPath, $MaterialObjectPath)) {
    try {
        Invoke-IceTool -ToolName "delete_asset" -InputObject @{
            asset_path = $AssetPath
            apply_changes = $true
        } | Out-Null
        Write-Host "Deleted existing asset: $AssetPath" -ForegroundColor DarkGray
    }
    catch {
        Write-Host "Asset not deleted (may not exist): $AssetPath" -ForegroundColor DarkGray
    }
}

Invoke-IceTool -ToolName "create_material" -InputObject @{
    material_name = $MaterialName
    save_path = $SavePath
    base_color = @(0.10, 0.16, 0.20)
    apply_changes = $true
} | Out-Null

$DryRunProperties = Invoke-IceTool -ToolName "set_material_properties" -InputObject @{
    material_path = $MaterialObjectPath
    blend_mode = "Translucent"
    shading_model = "DefaultLit"
    thumbnail_primitive = "Sphere"
    preview_mesh_path = "/Engine/BasicShapes/Sphere.Sphere"
}
$DryRunProperties | ConvertTo-Json -Depth 10

Invoke-IceTool -ToolName "set_material_properties" -InputObject @{
    material_path = $MaterialObjectPath
    blend_mode = "Translucent"
    shading_model = "DefaultLit"
    thumbnail_primitive = "Sphere"
    preview_mesh_path = "/Engine/BasicShapes/Sphere.Sphere"
    apply_changes = $true
} | Out-Null

$Indices = [ordered]@{}

# Parameters
Add-IceExpr -Indices $Indices -Key "ShellTint" -Type "VectorParameter" -X -4300 -Y -980 -ParamName "ShellTint" -DefaultValue "0.14,0.22,0.28,1.0" | Out-Null
Add-IceExpr -Indices $Indices -Key "IceTint" -Type "VectorParameter" -X -4300 -Y -800 -ParamName "IceTint" -DefaultValue "0.34,0.78,0.84,1.0" | Out-Null
Add-IceExpr -Indices $Indices -Key "CrackTint" -Type "VectorParameter" -X -4300 -Y -620 -ParamName "CrackTint" -DefaultValue "0.82,0.95,1.0,1.0" | Out-Null
Add-IceExpr -Indices $Indices -Key "FrostWhiteTint" -Type "VectorParameter" -X -4300 -Y -440 -ParamName "FrostWhiteTint" -DefaultValue "0.84,0.90,0.96,1.0" | Out-Null
Add-IceExpr -Indices $Indices -Key "CrackIntensity" -Type "ScalarParameter" -X -4300 -Y -180 -ParamName "CrackIntensity" -DefaultValue "1.75" | Out-Null
Add-IceExpr -Indices $Indices -Key "FrostAmount" -Type "ScalarParameter" -X -4300 -Y -20 -ParamName "FrostAmount" -DefaultValue "0.42" | Out-Null
Add-IceExpr -Indices $Indices -Key "RoughnessMin" -Type "ScalarParameter" -X -4300 -Y 140 -ParamName "RoughnessMin" -DefaultValue "0.04" | Out-Null
Add-IceExpr -Indices $Indices -Key "RoughnessMax" -Type "ScalarParameter" -X -4300 -Y 300 -ParamName "RoughnessMax" -DefaultValue "0.22" | Out-Null
Add-IceExpr -Indices $Indices -Key "MacroTiling" -Type "ScalarParameter" -X -4300 -Y 460 -ParamName "MacroTiling" -DefaultValue "0.16" | Out-Null
Add-IceExpr -Indices $Indices -Key "DetailTiling" -Type "ScalarParameter" -X -4300 -Y 620 -ParamName "DetailTiling" -DefaultValue "0.95" | Out-Null
Add-IceExpr -Indices $Indices -Key "CrackTiling" -Type "ScalarParameter" -X -4300 -Y 780 -ParamName "CrackTiling" -DefaultValue "0.55" | Out-Null
Add-IceExpr -Indices $Indices -Key "DeepDepth" -Type "ScalarParameter" -X -4300 -Y 940 -ParamName "DeepDepth" -DefaultValue "0.015" | Out-Null
Add-IceExpr -Indices $Indices -Key "NearDepth" -Type "ScalarParameter" -X -4300 -Y 1100 -ParamName "NearDepth" -DefaultValue "0.006" | Out-Null
Add-IceExpr -Indices $Indices -Key "CrackColorStrength" -Type "ScalarParameter" -X -4300 -Y 1260 -ParamName "CrackColorStrength" -DefaultValue "0.38" | Out-Null
Add-IceExpr -Indices $Indices -Key "FrostTintStrength" -Type "ScalarParameter" -X -4300 -Y 1420 -ParamName "FrostTintStrength" -DefaultValue "0.26" | Out-Null
Add-IceExpr -Indices $Indices -Key "EmissiveStrength" -Type "ScalarParameter" -X -4300 -Y 1580 -ParamName "EmissiveStrength" -DefaultValue "0.035" | Out-Null
Add-IceExpr -Indices $Indices -Key "EdgePower" -Type "ScalarParameter" -X -4300 -Y 1740 -ParamName "EdgePower" -DefaultValue "2.4" | Out-Null
Add-IceExpr -Indices $Indices -Key "CenterOpacity" -Type "ScalarParameter" -X -4300 -Y 1900 -ParamName "CenterOpacity" -DefaultValue "0.30" | Out-Null
Add-IceExpr -Indices $Indices -Key "EdgeOpacity" -Type "ScalarParameter" -X -4300 -Y 2060 -ParamName "EdgeOpacity" -DefaultValue "0.78" | Out-Null
Add-IceExpr -Indices $Indices -Key "OpacityFrostBoost" -Type "ScalarParameter" -X -4300 -Y 2220 -ParamName "OpacityFrostBoost" -DefaultValue "0.18" | Out-Null

# UV and sampling
Add-IceExpr -Indices $Indices -Key "TexCoord" -Type "TexCoord" -X -3720 -Y 520 | Out-Null
Add-IceExpr -Indices $Indices -Key "MacroUV" -Type "Multiply" -X -3440 -Y 380 | Out-Null
Add-IceExpr -Indices $Indices -Key "DetailUV" -Type "Multiply" -X -3440 -Y 560 | Out-Null
Add-IceExpr -Indices $Indices -Key "CrackUV" -Type "Multiply" -X -3440 -Y 740 | Out-Null
Add-IceExpr -Indices $Indices -Key "CameraWS" -Type "CameraVectorWS" -X -3720 -Y 980 | Out-Null
Add-IceExpr -Indices $Indices -Key "AxisX" -Type "Constant3" -X -3440 -Y 980 -DefaultValue "1.0,0.0,0.0" | Out-Null
Add-IceExpr -Indices $Indices -Key "AxisY" -Type "Constant3" -X -3440 -Y 1140 -DefaultValue "0.0,1.0,0.0" | Out-Null
Add-IceExpr -Indices $Indices -Key "CamDotX" -Type "DotProduct" -X -3160 -Y 940 | Out-Null
Add-IceExpr -Indices $Indices -Key "CamDotY" -Type "DotProduct" -X -3160 -Y 1100 | Out-Null
Add-IceExpr -Indices $Indices -Key "CameraXY" -Type "AppendVector" -X -2880 -Y 1020 | Out-Null
Add-IceExpr -Indices $Indices -Key "DeepOffset" -Type "Multiply" -X -2580 -Y 900 | Out-Null
Add-IceExpr -Indices $Indices -Key "NearOffset" -Type "Multiply" -X -2580 -Y 1100 | Out-Null
Add-IceExpr -Indices $Indices -Key "DeepUV" -Type "Add" -X -2300 -Y 430 | Out-Null
Add-IceExpr -Indices $Indices -Key "NearUV" -Type "Add" -X -2300 -Y 610 | Out-Null
Add-IceExpr -Indices $Indices -Key "Zero" -Type "Constant" -X -2300 -Y 860 -DefaultValue "0.0" | Out-Null
Add-IceExpr -Indices $Indices -Key "NearNoisePos" -Type "AppendVector" -X -2000 -Y 620 | Out-Null
Add-IceExpr -Indices $Indices -Key "CrackNoisePos" -Type "AppendVector" -X -2000 -Y 860 | Out-Null
Add-IceExpr -Indices $Indices -Key "FrostNoisePos" -Type "AppendVector" -X -2000 -Y 1080 | Out-Null
Add-IceExpr -Indices $Indices -Key "DeepCloudTex" -Type "TextureSample" -X -2000 -Y 320 -TexturePath $DeepCloudTexturePath | Out-Null
Add-IceExpr -Indices $Indices -Key "NearStructureNoise" -Type "Noise" -X -2000 -Y 520 -Settings @{
    noise_function = "SimplexTex"
    scale = 18
    quality = 1
    levels = 4
    output_min = 0
    output_max = 1
    level_scale = 2
    turbulence = $true
    tiling = $false
    repeat_size = 128
} | Out-Null
Add-IceExpr -Indices $Indices -Key "CrackNoise" -Type "Noise" -X -2000 -Y 760 -Settings @{
    noise_function = "VoronoiALU"
    scale = 28
    quality = 1
    levels = 2
    output_min = 0
    output_max = 1
    level_scale = 2
    turbulence = $false
    tiling = $false
    repeat_size = 128
} | Out-Null
Add-IceExpr -Indices $Indices -Key "FrostNoise" -Type "Noise" -X -2000 -Y 980 -Settings @{
    noise_function = "GradientTex"
    scale = 36
    quality = 1
    levels = 3
    output_min = 0
    output_max = 1
    level_scale = 2
    turbulence = $true
    tiling = $false
    repeat_size = 128
} | Out-Null
Add-IceExpr -Indices $Indices -Key "NormalTex" -Type "TextureSample" -X -2000 -Y 1200 -TexturePath $NormalTexturePath | Out-Null

# Mask logic
Add-IceExpr -Indices $Indices -Key "InternalAdd" -Type "Add" -X -1940 -Y 420 | Out-Null
Add-IceExpr -Indices $Indices -Key "InternalSat" -Type "Saturate" -X -1680 -Y 420 | Out-Null
Add-IceExpr -Indices $Indices -Key "CrackInvert" -Type "OneMinus" -X -1940 -Y 760 | Out-Null
Add-IceExpr -Indices $Indices -Key "CrackMul" -Type "Multiply" -X -1680 -Y 760 | Out-Null
Add-IceExpr -Indices $Indices -Key "CrackSat" -Type "Saturate" -X -1420 -Y 760 | Out-Null
Add-IceExpr -Indices $Indices -Key "FrostMul" -Type "Multiply" -X -1680 -Y 980 | Out-Null
Add-IceExpr -Indices $Indices -Key "FrostSat" -Type "Saturate" -X -1420 -Y 980 | Out-Null
Add-IceExpr -Indices $Indices -Key "Fresnel" -Type "Fresnel" -X -1940 -Y -60 | Out-Null
Add-IceExpr -Indices $Indices -Key "RimPow" -Type "Power" -X -1660 -Y -60 | Out-Null
Add-IceExpr -Indices $Indices -Key "CenterMask" -Type "OneMinus" -X -1400 -Y -60 | Out-Null
Add-IceExpr -Indices $Indices -Key "CoreMul" -Type "Multiply" -X -1140 -Y 340 | Out-Null
Add-IceExpr -Indices $Indices -Key "CoreSat" -Type "Saturate" -X -900 -Y 340 | Out-Null
Add-IceExpr -Indices $Indices -Key "CrackCoreMul" -Type "Multiply" -X -1140 -Y 760 | Out-Null
Add-IceExpr -Indices $Indices -Key "CrackCoreSat" -Type "Saturate" -X -900 -Y 760 | Out-Null
Add-IceExpr -Indices $Indices -Key "FrostRimAdd" -Type "Add" -X -1140 -Y 980 | Out-Null
Add-IceExpr -Indices $Indices -Key "FrostRimSat" -Type "Saturate" -X -900 -Y 980 | Out-Null
Add-IceExpr -Indices $Indices -Key "CrackAlphaMul" -Type "Multiply" -X -640 -Y 640 | Out-Null
Add-IceExpr -Indices $Indices -Key "FrostAlphaMul" -Type "Multiply" -X -640 -Y 980 | Out-Null
Add-IceExpr -Indices $Indices -Key "OpacityFrostMul" -Type "Multiply" -X -640 -Y 1260 | Out-Null
Add-IceExpr -Indices $Indices -Key "OpacityMaskAdd" -Type "Add" -X -360 -Y 1260 | Out-Null
Add-IceExpr -Indices $Indices -Key "OpacityMaskSat" -Type "Saturate" -X -120 -Y 1260 | Out-Null

# Output
Add-IceExpr -Indices $Indices -Key "BaseShellCoreLerp" -Type "Lerp" -X -640 -Y 160 | Out-Null
Add-IceExpr -Indices $Indices -Key "CrackColorLerp" -Type "Lerp" -X -360 -Y 340 | Out-Null
Add-IceExpr -Indices $Indices -Key "FinalBaseLerp" -Type "Lerp" -X -80 -Y 520 | Out-Null
Add-IceExpr -Indices $Indices -Key "RoughnessLerp" -Type "Lerp" -X -360 -Y 780 | Out-Null
Add-IceExpr -Indices $Indices -Key "FinalRoughnessLerp" -Type "Lerp" -X -80 -Y 860 | Out-Null
Add-IceExpr -Indices $Indices -Key "OpacityLerp" -Type "Lerp" -X 180 -Y 1260 | Out-Null
Add-IceExpr -Indices $Indices -Key "EmissiveMaskMul" -Type "Multiply" -X -360 -Y -300 | Out-Null
Add-IceExpr -Indices $Indices -Key "EmissiveColorMul" -Type "Multiply" -X -80 -Y -300 | Out-Null

$Indices = [pscustomobject]$Indices

# Wiring
Connect-Ice -SourceIndex $Indices.TexCoord -TargetType "expression" -TargetIndex $Indices.MacroUV -TargetInputIndex 0
Connect-Ice -SourceIndex $Indices.MacroTiling -TargetType "expression" -TargetIndex $Indices.MacroUV -TargetInputIndex 1
Connect-Ice -SourceIndex $Indices.TexCoord -TargetType "expression" -TargetIndex $Indices.DetailUV -TargetInputIndex 0
Connect-Ice -SourceIndex $Indices.DetailTiling -TargetType "expression" -TargetIndex $Indices.DetailUV -TargetInputIndex 1
Connect-Ice -SourceIndex $Indices.TexCoord -TargetType "expression" -TargetIndex $Indices.CrackUV -TargetInputIndex 0
Connect-Ice -SourceIndex $Indices.CrackTiling -TargetType "expression" -TargetIndex $Indices.CrackUV -TargetInputIndex 1

Connect-Ice -SourceIndex $Indices.CameraWS -TargetType "expression" -TargetIndex $Indices.CamDotX -TargetInputIndex 0
Connect-Ice -SourceIndex $Indices.AxisX -TargetType "expression" -TargetIndex $Indices.CamDotX -TargetInputIndex 1
Connect-Ice -SourceIndex $Indices.CameraWS -TargetType "expression" -TargetIndex $Indices.CamDotY -TargetInputIndex 0
Connect-Ice -SourceIndex $Indices.AxisY -TargetType "expression" -TargetIndex $Indices.CamDotY -TargetInputIndex 1
Connect-Ice -SourceIndex $Indices.CamDotX -TargetType "expression" -TargetIndex $Indices.CameraXY -TargetInputIndex 0
Connect-Ice -SourceIndex $Indices.CamDotY -TargetType "expression" -TargetIndex $Indices.CameraXY -TargetInputIndex 1
Connect-Ice -SourceIndex $Indices.CameraXY -TargetType "expression" -TargetIndex $Indices.DeepOffset -TargetInputIndex 0
Connect-Ice -SourceIndex $Indices.DeepDepth -TargetType "expression" -TargetIndex $Indices.DeepOffset -TargetInputIndex 1
Connect-Ice -SourceIndex $Indices.CameraXY -TargetType "expression" -TargetIndex $Indices.NearOffset -TargetInputIndex 0
Connect-Ice -SourceIndex $Indices.NearDepth -TargetType "expression" -TargetIndex $Indices.NearOffset -TargetInputIndex 1

Connect-Ice -SourceIndex $Indices.MacroUV -TargetType "expression" -TargetIndex $Indices.DeepUV -TargetInputIndex 0
Connect-Ice -SourceIndex $Indices.DeepOffset -TargetType "expression" -TargetIndex $Indices.DeepUV -TargetInputIndex 1
Connect-Ice -SourceIndex $Indices.DetailUV -TargetType "expression" -TargetIndex $Indices.NearUV -TargetInputIndex 0
Connect-Ice -SourceIndex $Indices.NearOffset -TargetType "expression" -TargetIndex $Indices.NearUV -TargetInputIndex 1

Connect-Ice -SourceIndex $Indices.NearUV -TargetType "expression" -TargetIndex $Indices.NearNoisePos -TargetInputIndex 0
Connect-Ice -SourceIndex $Indices.Zero -TargetType "expression" -TargetIndex $Indices.NearNoisePos -TargetInputIndex 1
Connect-Ice -SourceIndex $Indices.CrackUV -TargetType "expression" -TargetIndex $Indices.CrackNoisePos -TargetInputIndex 0
Connect-Ice -SourceIndex $Indices.Zero -TargetType "expression" -TargetIndex $Indices.CrackNoisePos -TargetInputIndex 1
Connect-Ice -SourceIndex $Indices.DetailUV -TargetType "expression" -TargetIndex $Indices.FrostNoisePos -TargetInputIndex 0
Connect-Ice -SourceIndex $Indices.Zero -TargetType "expression" -TargetIndex $Indices.FrostNoisePos -TargetInputIndex 1

Connect-Ice -SourceIndex $Indices.DeepUV -TargetType "expression" -TargetIndex $Indices.DeepCloudTex -TargetInputName "Coordinates"
Connect-Ice -SourceIndex $Indices.NearNoisePos -TargetType "expression" -TargetIndex $Indices.NearStructureNoise -TargetInputName "Position"
Connect-Ice -SourceIndex $Indices.CrackNoisePos -TargetType "expression" -TargetIndex $Indices.CrackNoise -TargetInputName "Position"
Connect-Ice -SourceIndex $Indices.FrostNoisePos -TargetType "expression" -TargetIndex $Indices.FrostNoise -TargetInputName "Position"
Connect-Ice -SourceIndex $Indices.DetailUV -TargetType "expression" -TargetIndex $Indices.NormalTex -TargetInputName "Coordinates"

Connect-Ice -SourceIndex $Indices.DeepCloudTex -TargetType "expression" -TargetIndex $Indices.InternalAdd -TargetInputIndex 0 -SourceOutputIndex 1
Connect-Ice -SourceIndex $Indices.NearStructureNoise -TargetType "expression" -TargetIndex $Indices.InternalAdd -TargetInputIndex 1
Connect-Ice -SourceIndex $Indices.InternalAdd -TargetType "expression" -TargetIndex $Indices.InternalSat -TargetInputIndex 0

Connect-Ice -SourceIndex $Indices.CrackNoise -TargetType "expression" -TargetIndex $Indices.CrackInvert -TargetInputIndex 0
Connect-Ice -SourceIndex $Indices.CrackInvert -TargetType "expression" -TargetIndex $Indices.CrackMul -TargetInputIndex 0
Connect-Ice -SourceIndex $Indices.CrackIntensity -TargetType "expression" -TargetIndex $Indices.CrackMul -TargetInputIndex 1
Connect-Ice -SourceIndex $Indices.CrackMul -TargetType "expression" -TargetIndex $Indices.CrackSat -TargetInputIndex 0

Connect-Ice -SourceIndex $Indices.FrostNoise -TargetType "expression" -TargetIndex $Indices.FrostMul -TargetInputIndex 0
Connect-Ice -SourceIndex $Indices.FrostAmount -TargetType "expression" -TargetIndex $Indices.FrostMul -TargetInputIndex 1
Connect-Ice -SourceIndex $Indices.FrostMul -TargetType "expression" -TargetIndex $Indices.FrostSat -TargetInputIndex 0

Connect-Ice -SourceIndex $Indices.Fresnel -TargetType "expression" -TargetIndex $Indices.RimPow -TargetInputIndex 0
Connect-Ice -SourceIndex $Indices.EdgePower -TargetType "expression" -TargetIndex $Indices.RimPow -TargetInputIndex 1
Connect-Ice -SourceIndex $Indices.RimPow -TargetType "expression" -TargetIndex $Indices.CenterMask -TargetInputIndex 0

Connect-Ice -SourceIndex $Indices.InternalSat -TargetType "expression" -TargetIndex $Indices.CoreMul -TargetInputIndex 0
Connect-Ice -SourceIndex $Indices.CenterMask -TargetType "expression" -TargetIndex $Indices.CoreMul -TargetInputIndex 1
Connect-Ice -SourceIndex $Indices.CoreMul -TargetType "expression" -TargetIndex $Indices.CoreSat -TargetInputIndex 0

Connect-Ice -SourceIndex $Indices.CrackSat -TargetType "expression" -TargetIndex $Indices.CrackCoreMul -TargetInputIndex 0
Connect-Ice -SourceIndex $Indices.CenterMask -TargetType "expression" -TargetIndex $Indices.CrackCoreMul -TargetInputIndex 1
Connect-Ice -SourceIndex $Indices.CrackCoreMul -TargetType "expression" -TargetIndex $Indices.CrackCoreSat -TargetInputIndex 0

Connect-Ice -SourceIndex $Indices.FrostSat -TargetType "expression" -TargetIndex $Indices.FrostRimAdd -TargetInputIndex 0
Connect-Ice -SourceIndex $Indices.RimPow -TargetType "expression" -TargetIndex $Indices.FrostRimAdd -TargetInputIndex 1
Connect-Ice -SourceIndex $Indices.FrostRimAdd -TargetType "expression" -TargetIndex $Indices.FrostRimSat -TargetInputIndex 0

Connect-Ice -SourceIndex $Indices.CrackCoreSat -TargetType "expression" -TargetIndex $Indices.CrackAlphaMul -TargetInputIndex 0
Connect-Ice -SourceIndex $Indices.CrackColorStrength -TargetType "expression" -TargetIndex $Indices.CrackAlphaMul -TargetInputIndex 1
Connect-Ice -SourceIndex $Indices.FrostRimSat -TargetType "expression" -TargetIndex $Indices.FrostAlphaMul -TargetInputIndex 0
Connect-Ice -SourceIndex $Indices.FrostTintStrength -TargetType "expression" -TargetIndex $Indices.FrostAlphaMul -TargetInputIndex 1
Connect-Ice -SourceIndex $Indices.FrostSat -TargetType "expression" -TargetIndex $Indices.OpacityFrostMul -TargetInputIndex 0
Connect-Ice -SourceIndex $Indices.OpacityFrostBoost -TargetType "expression" -TargetIndex $Indices.OpacityFrostMul -TargetInputIndex 1
Connect-Ice -SourceIndex $Indices.RimPow -TargetType "expression" -TargetIndex $Indices.OpacityMaskAdd -TargetInputIndex 0
Connect-Ice -SourceIndex $Indices.OpacityFrostMul -TargetType "expression" -TargetIndex $Indices.OpacityMaskAdd -TargetInputIndex 1
Connect-Ice -SourceIndex $Indices.OpacityMaskAdd -TargetType "expression" -TargetIndex $Indices.OpacityMaskSat -TargetInputIndex 0

Connect-Ice -SourceIndex $Indices.ShellTint -TargetType "expression" -TargetIndex $Indices.BaseShellCoreLerp -TargetInputIndex 0
Connect-Ice -SourceIndex $Indices.IceTint -TargetType "expression" -TargetIndex $Indices.BaseShellCoreLerp -TargetInputIndex 1
Connect-Ice -SourceIndex $Indices.CoreSat -TargetType "expression" -TargetIndex $Indices.BaseShellCoreLerp -TargetInputIndex 2
Connect-Ice -SourceIndex $Indices.BaseShellCoreLerp -TargetType "expression" -TargetIndex $Indices.CrackColorLerp -TargetInputIndex 0
Connect-Ice -SourceIndex $Indices.CrackTint -TargetType "expression" -TargetIndex $Indices.CrackColorLerp -TargetInputIndex 1
Connect-Ice -SourceIndex $Indices.CrackAlphaMul -TargetType "expression" -TargetIndex $Indices.CrackColorLerp -TargetInputIndex 2
Connect-Ice -SourceIndex $Indices.CrackColorLerp -TargetType "expression" -TargetIndex $Indices.FinalBaseLerp -TargetInputIndex 0
Connect-Ice -SourceIndex $Indices.FrostWhiteTint -TargetType "expression" -TargetIndex $Indices.FinalBaseLerp -TargetInputIndex 1
Connect-Ice -SourceIndex $Indices.FrostAlphaMul -TargetType "expression" -TargetIndex $Indices.FinalBaseLerp -TargetInputIndex 2

Connect-Ice -SourceIndex $Indices.RoughnessMin -TargetType "expression" -TargetIndex $Indices.RoughnessLerp -TargetInputIndex 0
Connect-Ice -SourceIndex $Indices.RoughnessMax -TargetType "expression" -TargetIndex $Indices.RoughnessLerp -TargetInputIndex 1
Connect-Ice -SourceIndex $Indices.FrostRimSat -TargetType "expression" -TargetIndex $Indices.RoughnessLerp -TargetInputIndex 2
Connect-Ice -SourceIndex $Indices.RoughnessLerp -TargetType "expression" -TargetIndex $Indices.FinalRoughnessLerp -TargetInputIndex 0
Connect-Ice -SourceIndex $Indices.RoughnessMin -TargetType "expression" -TargetIndex $Indices.FinalRoughnessLerp -TargetInputIndex 1
Connect-Ice -SourceIndex $Indices.CrackCoreSat -TargetType "expression" -TargetIndex $Indices.FinalRoughnessLerp -TargetInputIndex 2

Connect-Ice -SourceIndex $Indices.CenterOpacity -TargetType "expression" -TargetIndex $Indices.OpacityLerp -TargetInputIndex 0
Connect-Ice -SourceIndex $Indices.EdgeOpacity -TargetType "expression" -TargetIndex $Indices.OpacityLerp -TargetInputIndex 1
Connect-Ice -SourceIndex $Indices.OpacityMaskSat -TargetType "expression" -TargetIndex $Indices.OpacityLerp -TargetInputIndex 2

Connect-Ice -SourceIndex $Indices.CrackCoreSat -TargetType "expression" -TargetIndex $Indices.EmissiveMaskMul -TargetInputIndex 0
Connect-Ice -SourceIndex $Indices.EmissiveStrength -TargetType "expression" -TargetIndex $Indices.EmissiveMaskMul -TargetInputIndex 1
Connect-Ice -SourceIndex $Indices.EmissiveMaskMul -TargetType "expression" -TargetIndex $Indices.EmissiveColorMul -TargetInputIndex 0
Connect-Ice -SourceIndex $Indices.CrackTint -TargetType "expression" -TargetIndex $Indices.EmissiveColorMul -TargetInputIndex 1

Connect-Ice -SourceIndex $Indices.FinalBaseLerp -TargetType "material_pin" -TargetPin "BaseColor"
Connect-Ice -SourceIndex $Indices.FinalRoughnessLerp -TargetType "material_pin" -TargetPin "Roughness"
Connect-Ice -SourceIndex $Indices.EmissiveColorMul -TargetType "material_pin" -TargetPin "EmissiveColor"
Connect-Ice -SourceIndex $Indices.NormalTex -TargetType "material_pin" -TargetPin "Normal"
Connect-Ice -SourceIndex $Indices.OpacityLerp -TargetType "material_pin" -TargetPin "Opacity"

$ParameterExprIndices = @(
    $Indices.ShellTint, $Indices.IceTint, $Indices.CrackTint, $Indices.FrostWhiteTint,
    $Indices.CrackIntensity, $Indices.FrostAmount, $Indices.RoughnessMin, $Indices.RoughnessMax,
    $Indices.MacroTiling, $Indices.DetailTiling, $Indices.CrackTiling,
    $Indices.DeepDepth, $Indices.NearDepth, $Indices.CrackColorStrength, $Indices.FrostTintStrength,
    $Indices.EmissiveStrength, $Indices.EdgePower, $Indices.CenterOpacity, $Indices.EdgeOpacity, $Indices.OpacityFrostBoost
)

$UvExprIndices = @(
    $Indices.TexCoord, $Indices.MacroUV, $Indices.DetailUV, $Indices.CrackUV,
    $Indices.CameraWS, $Indices.AxisX, $Indices.AxisY, $Indices.CamDotX, $Indices.CamDotY, $Indices.CameraXY, $Indices.DeepOffset, $Indices.NearOffset,
    $Indices.DeepUV, $Indices.NearUV, $Indices.Zero, $Indices.NearNoisePos, $Indices.CrackNoisePos, $Indices.FrostNoisePos,
    $Indices.DeepCloudTex, $Indices.NearStructureNoise, $Indices.CrackNoise, $Indices.FrostNoise, $Indices.NormalTex
)

$MaskExprIndices = @(
    $Indices.InternalAdd, $Indices.InternalSat, $Indices.CrackInvert, $Indices.CrackMul, $Indices.CrackSat,
    $Indices.FrostMul, $Indices.FrostSat, $Indices.Fresnel, $Indices.RimPow, $Indices.CenterMask,
    $Indices.CoreMul, $Indices.CoreSat, $Indices.CrackCoreMul, $Indices.CrackCoreSat,
    $Indices.FrostRimAdd, $Indices.FrostRimSat, $Indices.CrackAlphaMul, $Indices.FrostAlphaMul,
    $Indices.OpacityFrostMul, $Indices.OpacityMaskAdd, $Indices.OpacityMaskSat
)

$OutputExprIndices = @(
    $Indices.BaseShellCoreLerp, $Indices.CrackColorLerp, $Indices.FinalBaseLerp,
    $Indices.RoughnessLerp, $Indices.FinalRoughnessLerp, $Indices.OpacityLerp,
    $Indices.EmissiveMaskMul, $Indices.EmissiveColorMul
)

Invoke-IceTool -ToolName "create_material_comment" -InputObject @{
    material_path = $MaterialObjectPath
    comment_text = "Ice Parameters"
    editor_x = "-4460"
    editor_y = "-1100"
    width = "760"
    height = "3520"
    color_rgba = @(0.96, 0.88, 0.28, 1.0)
    apply_changes = $true
} | Out-Null

Invoke-IceTool -ToolName "create_material_comment" -InputObject @{
    material_path = $MaterialObjectPath
    comment_text = "Ice UV / Sampling"
    editor_x = "-3880"
    editor_y = "180"
    width = "1760"
    height = "1360"
    color_rgba = @(0.44, 0.74, 0.96, 1.0)
    apply_changes = $true
} | Out-Null

Invoke-IceTool -ToolName "create_material_comment" -InputObject @{
    material_path = $MaterialObjectPath
    comment_text = "Ice Masks / Volume"
    editor_x = "-2060"
    editor_y = "-180"
    width = "1600"
    height = "1660"
    color_rgba = @(0.36, 0.82, 0.88, 1.0)
    apply_changes = $true
} | Out-Null

Invoke-IceTool -ToolName "create_material_comment" -InputObject @{
    material_path = $MaterialObjectPath
    comment_text = "Ice Output"
    editor_x = "-760"
    editor_y = "-420"
    width = "1200"
    height = "1920"
    color_rgba = @(0.72, 0.86, 1.0, 1.0)
    apply_changes = $true
} | Out-Null

Invoke-IceTool -ToolName "update_material_comment" -InputObject @{
    material_path = $MaterialObjectPath
    comment_text = "Ice Parameters"
    wrap_expression_indices = $ParameterExprIndices
    padding = "120"
    apply_changes = $true
} | Out-Null

Invoke-IceTool -ToolName "update_material_comment" -InputObject @{
    material_path = $MaterialObjectPath
    comment_text = "Ice UV / Sampling"
    wrap_expression_indices = $UvExprIndices
    padding = "120"
    apply_changes = $true
} | Out-Null

Invoke-IceTool -ToolName "update_material_comment" -InputObject @{
    material_path = $MaterialObjectPath
    comment_text = "Ice Masks / Volume"
    wrap_expression_indices = $MaskExprIndices
    padding = "120"
    apply_changes = $true
} | Out-Null

Invoke-IceTool -ToolName "update_material_comment" -InputObject @{
    material_path = $MaterialObjectPath
    comment_text = "Ice Output"
    wrap_expression_indices = $OutputExprIndices
    padding = "120"
    apply_changes = $true
} | Out-Null

$GraphInfo = Invoke-IceTool -ToolName "get_material_info" -InputObject @{
    material_path = $MaterialObjectPath
}
$GraphInfo | ConvertTo-Json -Depth 12

Invoke-IceTool -ToolName "capture_material_graph_screenshot" -InputObject @{
    material_path = $MaterialObjectPath
    comment_text = "Ice Output"
    output_dir = $OutputDir
    file_name = "ice_surface_output_graph.png"
    image_width = "1800"
    image_height = "1100"
    padding = "220"
} | Out-Null

Invoke-IceTool -ToolName "capture_material_graph_screenshot" -InputObject @{
    material_path = $MaterialObjectPath
    comment_text = "Ice Masks / Volume"
    output_dir = $OutputDir
    file_name = "ice_surface_masks_graph.png"
    image_width = "1800"
    image_height = "1100"
    padding = "220"
} | Out-Null

$ReviewResult = Invoke-IceTool -ToolName "review_material_asset" -InputObject @{
    material_path = $MaterialObjectPath
    creative_mode = "transform"
}
$ReviewResult | ConvertTo-Json -Depth 12

$ValidationResult = Invoke-IceTool -ToolName "validate_material_graph_block" -InputObject @{
    material_path = $MaterialObjectPath
    comment_text = "Ice Output"
    required_root_pins = @("BaseColor", "Roughness", "Normal", "EmissiveColor", "Opacity")
    require_comment = $true
}
$ValidationResult | ConvertTo-Json -Depth 12

$MIResult = Invoke-IceTool -ToolName "create_material_instance" -InputObject @{
    instance_name = $InstanceName
    parent_material_path = $MaterialObjectPath
    save_path = $SavePath
    scalar_overrides = (Json-ObjectString @{
        CrackIntensity = 1.95
        FrostAmount = 0.50
        RoughnessMin = 0.04
        RoughnessMax = 0.20
        EmissiveStrength = 0.045
        CenterOpacity = 0.28
        EdgeOpacity = 0.82
    })
    vector_overrides = (Json-ObjectString @{
        IceTint = @(0.32, 0.82, 0.88, 1.0)
        CrackTint = @(0.90, 0.98, 1.0, 1.0)
        ShellTint = @(0.10, 0.19, 0.25, 1.0)
        FrostWhiteTint = @(0.88, 0.93, 0.98, 1.0)
    })
}
$MIResult | ConvertTo-Json -Depth 10

try {
    Invoke-IceTool -ToolName "spawn_material_probe_actor" -InputObject @{
        actor_label = $ProbeActorLabel
        location = @(0, 0, 120)
        scale = @(2.5, 2.5, 2.5)
    } | Out-Null

    Invoke-IceTool -ToolName "apply_material_to_probe_actor" -InputObject @{
        actor_label = $ProbeActorLabel
        material_path = $MIResult.asset_path
    } | Out-Null

    Invoke-IceTool -ToolName "capture_material_visual_probe" -InputObject @{
        actor_label = $ProbeActorLabel
        output_dir = $OutputDir
        file_name = "ice_surface_probe.jpg"
        frame_actor = $true
        wait_ms = "250"
    } | Out-Null
}
catch {
    Write-Warning "Probe capture skipped: $($_.Exception.Message)"
}

Write-Host "`n=== ICE SURFACE REBUILD COMPLETE ===" -ForegroundColor Green
Write-Host "Material: $MaterialObjectPath" -ForegroundColor Green
Write-Host "Instance: $InstanceObjectPath" -ForegroundColor Green
Write-Host "OutputDir: $OutputDir" -ForegroundColor Green


