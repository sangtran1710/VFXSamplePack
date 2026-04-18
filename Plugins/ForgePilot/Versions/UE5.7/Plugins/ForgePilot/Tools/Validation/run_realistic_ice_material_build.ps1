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

Wait-ForgePilotBridge -BridgeHost $BridgeHost -BridgePort $BridgePort -TimeoutSeconds 20

$MaterialName = "M_Ice_Realistic"
$SavePath = "/Game/ForgePilot_Test/Materials/Ice"
$MaterialObjectPath = "$SavePath/$MaterialName.$MaterialName"
$PreviewActorLabel = "ForgePilot_IceRealisticPreview"
$PreviewLightLabel = "ForgePilot_IceRealisticPreviewLight"
$ViewportCaptureName = "ice_realistic_preview"

$DeepIceTexturePath = "/Game/ImpactParticlesVol1/Textures/T_CloudsBlur.T_CloudsBlur"
$NearIceTexturePath = "/Game/GunFX/Textures/PBR/Decals/Ice/T_Ice_Decal_basecolor_2.T_Ice_Decal_basecolor_2"
$IceNormalPath = "/Game/GunFX/Textures/PBR/Decals/Ice/T_Ice_Decal_normal_2.T_Ice_Decal_normal_2"
$CrackTexturePath = "/Game/GunFX/Textures/PBR/Decals/Ice/T_Ice_Decal_ambientocclusion_3.T_Ice_Decal_ambientocclusion_3"
$NoiseTexturePath = "/Game/GunFX/Textures/PBR/Decals/Ice/T_Ice_Decal_roughness_2.T_Ice_Decal_roughness_2"

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
        [string]$TargetType = "expression",
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

Write-Host "`n=== REALISTIC ICE MATERIAL BUILD ===" -ForegroundColor Cyan

try {
    Invoke-IceTool -ToolName "delete_asset" -InputObject @{
        asset_path = $MaterialObjectPath
        apply_changes = $true
    } | Out-Null
    Write-Host "Deleted previous material: $MaterialObjectPath" -ForegroundColor DarkGray
}
catch {
    Write-Host "No previous material to delete." -ForegroundColor DarkGray
}

Invoke-IceTool -ToolName "create_material" -InputObject @{
    material_name = $MaterialName
    save_path = $SavePath
    base_color = @(0.03, 0.10, 0.16)
    apply_changes = $true
} | Out-Null

Invoke-IceTool -ToolName "set_material_properties" -InputObject @{
    material_path = $MaterialObjectPath
    blend_mode = "Opaque"
    shading_model = "DefaultLit"
    thumbnail_primitive = "Sphere"
    preview_mesh_path = "/Engine/BasicShapes/Sphere.Sphere"
    apply_changes = $true
} | Out-Null

$Indices = [ordered]@{}

# Parameter block
Add-IceExpr -Indices $Indices -Key "TintDeep" -Type "VectorParameter" -X -3200 -Y -650 -ParamName "Ice_TintDeep" -DefaultValue "0.07,0.16,0.26,1.0" | Out-Null
Add-IceExpr -Indices $Indices -Key "TintMid" -Type "VectorParameter" -X -3200 -Y -470 -ParamName "Ice_TintMid" -DefaultValue "0.28,0.56,0.76,1.0" | Out-Null
Add-IceExpr -Indices $Indices -Key "TintCrack" -Type "VectorParameter" -X -3200 -Y -290 -ParamName "Ice_TintCrack" -DefaultValue "0.76,0.88,0.98,1.0" | Out-Null
Add-IceExpr -Indices $Indices -Key "SSSTint" -Type "VectorParameter" -X -3200 -Y -110 -ParamName "Ice_SubsurfaceTint" -DefaultValue "0.05,0.14,0.22,1.0" | Out-Null
Add-IceExpr -Indices $Indices -Key "MacroTiling" -Type "ScalarParameter" -X -3200 -Y 120 -ParamName "Ice_MacroTiling" -DefaultValue "0.12" | Out-Null
Add-IceExpr -Indices $Indices -Key "MicroTiling" -Type "ScalarParameter" -X -3200 -Y 260 -ParamName "Ice_MicroTiling" -DefaultValue "0.18" | Out-Null
Add-IceExpr -Indices $Indices -Key "CrackTiling" -Type "ScalarParameter" -X -3200 -Y 400 -ParamName "Ice_CrackTiling" -DefaultValue "0.10" | Out-Null
Add-IceExpr -Indices $Indices -Key "DeepDepth" -Type "ScalarParameter" -X -3200 -Y 540 -ParamName "Ice_DeepDepth" -DefaultValue "0.004" | Out-Null
Add-IceExpr -Indices $Indices -Key "NearDepth" -Type "ScalarParameter" -X -3200 -Y 680 -ParamName "Ice_NearDepth" -DefaultValue "0.0015" | Out-Null
Add-IceExpr -Indices $Indices -Key "CrackBoost" -Type "ScalarParameter" -X -3200 -Y 820 -ParamName "Ice_CrackBoost" -DefaultValue "1.35" | Out-Null
Add-IceExpr -Indices $Indices -Key "InternalStrength" -Type "ScalarParameter" -X -3200 -Y 960 -ParamName "Ice_InternalStrength" -DefaultValue "0.18" | Out-Null
Add-IceExpr -Indices $Indices -Key "CrackColorStrength" -Type "ScalarParameter" -X -3200 -Y 1100 -ParamName "Ice_CrackColorStrength" -DefaultValue "0.25" | Out-Null
Add-IceExpr -Indices $Indices -Key "RoughnessMin" -Type "ScalarParameter" -X -3200 -Y 1240 -ParamName "Ice_RoughnessMin" -DefaultValue "0.05" | Out-Null
Add-IceExpr -Indices $Indices -Key "RoughnessMax" -Type "ScalarParameter" -X -3200 -Y 1380 -ParamName "Ice_RoughnessMax" -DefaultValue "0.24" | Out-Null
Add-IceExpr -Indices $Indices -Key "OpacityDensity" -Type "ScalarParameter" -X -3200 -Y 1660 -ParamName "Ice_OpacityDensity" -DefaultValue "1.0" | Out-Null
Add-IceExpr -Indices $Indices -Key "RimPower" -Type "ScalarParameter" -X -3200 -Y 1800 -ParamName "Ice_RimPower" -DefaultValue "2.2" | Out-Null
Add-IceExpr -Indices $Indices -Key "RimStrength" -Type "ScalarParameter" -X -3200 -Y 1940 -ParamName "Ice_RimStrength" -DefaultValue "0.04" | Out-Null
Add-IceExpr -Indices $Indices -Key "OpacityEdge" -Type "ScalarParameter" -X -3200 -Y 2080 -ParamName "Ice_OpacityEdge" -DefaultValue "1.0" | Out-Null
Add-IceExpr -Indices $Indices -Key "EmissiveStrength" -Type "ScalarParameter" -X -3200 -Y 2220 -ParamName "Ice_EmissiveStrength" -DefaultValue "0.00" | Out-Null
Add-IceExpr -Indices $Indices -Key "RefractionAmount" -Type "ScalarParameter" -X -3200 -Y 2360 -ParamName "Ice_RefractionAmount" -DefaultValue "1.0" | Out-Null

# UV and fake depth
Add-IceExpr -Indices $Indices -Key "TexCoord" -Type "TexCoord" -X -2700 -Y 250 | Out-Null
Add-IceExpr -Indices $Indices -Key "MacroUV" -Type "Multiply" -X -2450 -Y 170 | Out-Null
Add-IceExpr -Indices $Indices -Key "MicroUV" -Type "Multiply" -X -2450 -Y 340 | Out-Null
Add-IceExpr -Indices $Indices -Key "CrackUV" -Type "Multiply" -X -2450 -Y 510 | Out-Null
Add-IceExpr -Indices $Indices -Key "ReflectionWS" -Type "ReflectionVectorWS" -X -2700 -Y 860 | Out-Null
Add-IceExpr -Indices $Indices -Key "ReflectionTS" -Type "TransformVector" -X -2440 -Y 860 -Settings @{
    source_space = "World"
    dest_space = "Tangent"
} | Out-Null
Add-IceExpr -Indices $Indices -Key "OffsetXY" -Type "ComponentMask" -X -2200 -Y 860 | Out-Null
Add-IceExpr -Indices $Indices -Key "DeepOffset" -Type "Multiply" -X -1940 -Y 780 | Out-Null
Add-IceExpr -Indices $Indices -Key "NearOffset" -Type "Multiply" -X -1940 -Y 980 | Out-Null
Add-IceExpr -Indices $Indices -Key "DeepUV" -Type "Add" -X -1700 -Y 230 | Out-Null
Add-IceExpr -Indices $Indices -Key "NearUV" -Type "Add" -X -1700 -Y 390 | Out-Null

# Texture sampling
Add-IceExpr -Indices $Indices -Key "DeepIceTex" -Type "TextureSample" -X -1450 -Y 160 -TexturePath $DeepIceTexturePath | Out-Null
Add-IceExpr -Indices $Indices -Key "NearIceTex" -Type "TextureSample" -X -1450 -Y 360 -TexturePath $NearIceTexturePath | Out-Null
Add-IceExpr -Indices $Indices -Key "CrackTex" -Type "TextureSample" -X -1450 -Y 560 -TexturePath $CrackTexturePath | Out-Null
Add-IceExpr -Indices $Indices -Key "NoiseTex" -Type "TextureSample" -X -1450 -Y 770 -TexturePath $NoiseTexturePath | Out-Null
Add-IceExpr -Indices $Indices -Key "NormalTex" -Type "TextureSample" -X -1450 -Y 980 -TexturePath $IceNormalPath | Out-Null

# Mask shaping
Add-IceExpr -Indices $Indices -Key "DeepNearAdd" -Type "Add" -X -1130 -Y 240 | Out-Null
Add-IceExpr -Indices $Indices -Key "InternalMul" -Type "Multiply" -X -900 -Y 240 | Out-Null
Add-IceExpr -Indices $Indices -Key "InternalSat" -Type "Saturate" -X -900 -Y 240 | Out-Null
Add-IceExpr -Indices $Indices -Key "CrackInvert" -Type "OneMinus" -X -1290 -Y 560 | Out-Null
Add-IceExpr -Indices $Indices -Key "CrackMul" -Type "Multiply" -X -1130 -Y 560 | Out-Null
Add-IceExpr -Indices $Indices -Key "CrackSat" -Type "Saturate" -X -900 -Y 560 | Out-Null
Add-IceExpr -Indices $Indices -Key "CrackColorMul" -Type "Multiply" -X -670 -Y 560 | Out-Null
Add-IceExpr -Indices $Indices -Key "VolumeAdd" -Type "Add" -X -670 -Y 360 | Out-Null
Add-IceExpr -Indices $Indices -Key "VolumeSat" -Type "Saturate" -X -440 -Y 360 | Out-Null

# Color and scattering
Add-IceExpr -Indices $Indices -Key "BodyColorLerp" -Type "Lerp" -X -200 -Y -120 | Out-Null
Add-IceExpr -Indices $Indices -Key "FinalBaseLerp" -Type "Lerp" -X 80 -Y -120 | Out-Null
Add-IceExpr -Indices $Indices -Key "SubsurfaceLerp" -Type "Lerp" -X 80 -Y 120 | Out-Null
Add-IceExpr -Indices $Indices -Key "Fresnel" -Type "Fresnel" -X -260 -Y -430 | Out-Null
Add-IceExpr -Indices $Indices -Key "RimPowerNode" -Type "Power" -X 0 -Y -430 | Out-Null
Add-IceExpr -Indices $Indices -Key "RimMask" -Type "Multiply" -X 260 -Y -430 | Out-Null
Add-IceExpr -Indices $Indices -Key "EdgeBaseLerp" -Type "Lerp" -X 520 -Y -120 | Out-Null
Add-IceExpr -Indices $Indices -Key "EdgeSSSLerp" -Type "Lerp" -X 520 -Y 120 | Out-Null
Add-IceExpr -Indices $Indices -Key "OpacityLerp" -Type "Lerp" -X 520 -Y 280 | Out-Null
Add-IceExpr -Indices $Indices -Key "CrackEdgeMul" -Type "Multiply" -X 520 -Y -620 | Out-Null
Add-IceExpr -Indices $Indices -Key "EmissiveMaskMul" -Type "Multiply" -X 760 -Y -620 | Out-Null
Add-IceExpr -Indices $Indices -Key "EmissiveColorMul" -Type "Multiply" -X 1000 -Y -620 | Out-Null

# Surface response
Add-IceExpr -Indices $Indices -Key "RoughnessLerp" -Type "Lerp" -X -200 -Y 470 | Out-Null
Add-IceExpr -Indices $Indices -Key "FinalRoughnessLerp" -Type "Lerp" -X 80 -Y 470 | Out-Null
Add-IceExpr -Indices $Indices -Key "EdgeRoughnessLerp" -Type "Lerp" -X 520 -Y 470 | Out-Null

$Indices = [pscustomobject]$Indices

Invoke-IceTool -ToolName "set_material_expression_property" -InputObject @{
    material_path = $MaterialObjectPath
    expression_index = $Indices.OffsetXY
    property_name = "R"
    property_value = $true
    apply_changes = $true
} | Out-Null

Invoke-IceTool -ToolName "set_material_expression_property" -InputObject @{
    material_path = $MaterialObjectPath
    expression_index = $Indices.OffsetXY
    property_name = "G"
    property_value = $true
    apply_changes = $true
} | Out-Null

Invoke-IceTool -ToolName "set_material_expression_property" -InputObject @{
    material_path = $MaterialObjectPath
    expression_index = $Indices.DeepIceTex
    property_name = "SamplerType"
    property_value = "SAMPLERTYPE_Color"
    apply_changes = $true
} | Out-Null

Invoke-IceTool -ToolName "set_material_expression_property" -InputObject @{
    material_path = $MaterialObjectPath
    expression_index = $Indices.NearIceTex
    property_name = "SamplerType"
    property_value = "SAMPLERTYPE_LinearGrayscale"
    apply_changes = $true
} | Out-Null

Invoke-IceTool -ToolName "set_material_expression_property" -InputObject @{
    material_path = $MaterialObjectPath
    expression_index = $Indices.CrackTex
    property_name = "SamplerType"
    property_value = "SAMPLERTYPE_LinearGrayscale"
    apply_changes = $true
} | Out-Null

Invoke-IceTool -ToolName "set_material_expression_property" -InputObject @{
    material_path = $MaterialObjectPath
    expression_index = $Indices.NoiseTex
    property_name = "SamplerType"
    property_value = "SAMPLERTYPE_LinearGrayscale"
    apply_changes = $true
} | Out-Null

Invoke-IceTool -ToolName "set_material_expression_property" -InputObject @{
    material_path = $MaterialObjectPath
    expression_index = $Indices.NormalTex
    property_name = "SamplerType"
    property_value = "SAMPLERTYPE_Normal"
    apply_changes = $true
} | Out-Null

# Wiring: UV and fake depth
Connect-Ice -SourceIndex $Indices.TexCoord -TargetIndex $Indices.MacroUV -TargetInputIndex 0
Connect-Ice -SourceIndex $Indices.MacroTiling -TargetIndex $Indices.MacroUV -TargetInputIndex 1
Connect-Ice -SourceIndex $Indices.TexCoord -TargetIndex $Indices.MicroUV -TargetInputIndex 0
Connect-Ice -SourceIndex $Indices.MicroTiling -TargetIndex $Indices.MicroUV -TargetInputIndex 1
Connect-Ice -SourceIndex $Indices.TexCoord -TargetIndex $Indices.CrackUV -TargetInputIndex 0
Connect-Ice -SourceIndex $Indices.CrackTiling -TargetIndex $Indices.CrackUV -TargetInputIndex 1

Connect-Ice -SourceIndex $Indices.ReflectionWS -TargetIndex $Indices.ReflectionTS -TargetInputName "Input"
Connect-Ice -SourceIndex $Indices.ReflectionTS -TargetIndex $Indices.OffsetXY -TargetInputName "Input"
Connect-Ice -SourceIndex $Indices.OffsetXY -TargetIndex $Indices.DeepOffset -TargetInputIndex 0
Connect-Ice -SourceIndex $Indices.DeepDepth -TargetIndex $Indices.DeepOffset -TargetInputIndex 1
Connect-Ice -SourceIndex $Indices.OffsetXY -TargetIndex $Indices.NearOffset -TargetInputIndex 0
Connect-Ice -SourceIndex $Indices.NearDepth -TargetIndex $Indices.NearOffset -TargetInputIndex 1

Connect-Ice -SourceIndex $Indices.MacroUV -TargetIndex $Indices.DeepUV -TargetInputIndex 0
Connect-Ice -SourceIndex $Indices.DeepOffset -TargetIndex $Indices.DeepUV -TargetInputIndex 1
Connect-Ice -SourceIndex $Indices.MicroUV -TargetIndex $Indices.NearUV -TargetInputIndex 0
Connect-Ice -SourceIndex $Indices.NearOffset -TargetIndex $Indices.NearUV -TargetInputIndex 1

# Wiring: texture coordinates
Connect-Ice -SourceIndex $Indices.DeepUV -TargetIndex $Indices.DeepIceTex -TargetInputName "Coordinates"
Connect-Ice -SourceIndex $Indices.NearUV -TargetIndex $Indices.NearIceTex -TargetInputName "Coordinates"
Connect-Ice -SourceIndex $Indices.CrackUV -TargetIndex $Indices.CrackTex -TargetInputName "Coordinates"
Connect-Ice -SourceIndex $Indices.MicroUV -TargetIndex $Indices.NoiseTex -TargetInputName "Coordinates"
Connect-Ice -SourceIndex $Indices.MacroUV -TargetIndex $Indices.NormalTex -TargetInputName "Coordinates"

# Wiring: mask shaping
Connect-Ice -SourceIndex $Indices.DeepIceTex -SourceOutputIndex 1 -TargetIndex $Indices.DeepNearAdd -TargetInputIndex 0
Connect-Ice -SourceIndex $Indices.NearIceTex -SourceOutputIndex 1 -TargetIndex $Indices.DeepNearAdd -TargetInputIndex 1
Connect-Ice -SourceIndex $Indices.DeepNearAdd -TargetIndex $Indices.InternalMul -TargetInputIndex 0
Connect-Ice -SourceIndex $Indices.InternalStrength -TargetIndex $Indices.InternalMul -TargetInputIndex 1
Connect-Ice -SourceIndex $Indices.InternalMul -TargetIndex $Indices.InternalSat -TargetInputIndex 0
Connect-Ice -SourceIndex $Indices.CrackTex -SourceOutputIndex 1 -TargetIndex $Indices.CrackInvert -TargetInputIndex 0
Connect-Ice -SourceIndex $Indices.CrackInvert -TargetIndex $Indices.CrackMul -TargetInputIndex 0
Connect-Ice -SourceIndex $Indices.CrackBoost -TargetIndex $Indices.CrackMul -TargetInputIndex 1
Connect-Ice -SourceIndex $Indices.CrackMul -TargetIndex $Indices.CrackSat -TargetInputIndex 0
Connect-Ice -SourceIndex $Indices.CrackSat -TargetIndex $Indices.CrackColorMul -TargetInputIndex 0
Connect-Ice -SourceIndex $Indices.CrackColorStrength -TargetIndex $Indices.CrackColorMul -TargetInputIndex 1
Connect-Ice -SourceIndex $Indices.InternalSat -TargetIndex $Indices.VolumeAdd -TargetInputIndex 0
Connect-Ice -SourceIndex $Indices.CrackSat -TargetIndex $Indices.VolumeAdd -TargetInputIndex 1
Connect-Ice -SourceIndex $Indices.VolumeAdd -TargetIndex $Indices.VolumeSat -TargetInputIndex 0

# Wiring: color
Connect-Ice -SourceIndex $Indices.TintDeep -TargetIndex $Indices.BodyColorLerp -TargetInputIndex 0
Connect-Ice -SourceIndex $Indices.TintMid -TargetIndex $Indices.BodyColorLerp -TargetInputIndex 1
Connect-Ice -SourceIndex $Indices.InternalSat -TargetIndex $Indices.BodyColorLerp -TargetInputIndex 2
Connect-Ice -SourceIndex $Indices.BodyColorLerp -TargetIndex $Indices.FinalBaseLerp -TargetInputIndex 0
Connect-Ice -SourceIndex $Indices.TintCrack -TargetIndex $Indices.FinalBaseLerp -TargetInputIndex 1
Connect-Ice -SourceIndex $Indices.CrackColorMul -TargetIndex $Indices.FinalBaseLerp -TargetInputIndex 2
Connect-Ice -SourceIndex $Indices.SSSTint -TargetIndex $Indices.SubsurfaceLerp -TargetInputIndex 0
Connect-Ice -SourceIndex $Indices.TintCrack -TargetIndex $Indices.SubsurfaceLerp -TargetInputIndex 1
Connect-Ice -SourceIndex $Indices.VolumeSat -TargetIndex $Indices.SubsurfaceLerp -TargetInputIndex 2
Connect-Ice -SourceIndex $Indices.Fresnel -TargetIndex $Indices.RimPowerNode -TargetInputIndex 0
Connect-Ice -SourceIndex $Indices.RimPower -TargetIndex $Indices.RimPowerNode -TargetInputIndex 1
Connect-Ice -SourceIndex $Indices.RimPowerNode -TargetIndex $Indices.RimMask -TargetInputIndex 0
Connect-Ice -SourceIndex $Indices.RimStrength -TargetIndex $Indices.RimMask -TargetInputIndex 1
Connect-Ice -SourceIndex $Indices.FinalBaseLerp -TargetIndex $Indices.EdgeBaseLerp -TargetInputIndex 0
Connect-Ice -SourceIndex $Indices.TintDeep -TargetIndex $Indices.EdgeBaseLerp -TargetInputIndex 1
Connect-Ice -SourceIndex $Indices.RimMask -TargetIndex $Indices.EdgeBaseLerp -TargetInputIndex 2
Connect-Ice -SourceIndex $Indices.SubsurfaceLerp -TargetIndex $Indices.EdgeSSSLerp -TargetInputIndex 0
Connect-Ice -SourceIndex $Indices.TintMid -TargetIndex $Indices.EdgeSSSLerp -TargetInputIndex 1
Connect-Ice -SourceIndex $Indices.RimMask -TargetIndex $Indices.EdgeSSSLerp -TargetInputIndex 2
Connect-Ice -SourceIndex $Indices.OpacityDensity -TargetIndex $Indices.OpacityLerp -TargetInputIndex 0
Connect-Ice -SourceIndex $Indices.OpacityEdge -TargetIndex $Indices.OpacityLerp -TargetInputIndex 1
Connect-Ice -SourceIndex $Indices.RimPowerNode -TargetIndex $Indices.OpacityLerp -TargetInputIndex 2
Connect-Ice -SourceIndex $Indices.CrackSat -TargetIndex $Indices.CrackEdgeMul -TargetInputIndex 0
Connect-Ice -SourceIndex $Indices.RimPowerNode -TargetIndex $Indices.CrackEdgeMul -TargetInputIndex 1
Connect-Ice -SourceIndex $Indices.CrackEdgeMul -TargetIndex $Indices.EmissiveMaskMul -TargetInputIndex 0
Connect-Ice -SourceIndex $Indices.EmissiveStrength -TargetIndex $Indices.EmissiveMaskMul -TargetInputIndex 1
Connect-Ice -SourceIndex $Indices.EmissiveMaskMul -TargetIndex $Indices.EmissiveColorMul -TargetInputIndex 0
Connect-Ice -SourceIndex $Indices.TintCrack -TargetIndex $Indices.EmissiveColorMul -TargetInputIndex 1

# Wiring: roughness
Connect-Ice -SourceIndex $Indices.RoughnessMin -TargetIndex $Indices.RoughnessLerp -TargetInputIndex 0
Connect-Ice -SourceIndex $Indices.RoughnessMax -TargetIndex $Indices.RoughnessLerp -TargetInputIndex 1
Connect-Ice -SourceIndex $Indices.NoiseTex -SourceOutputIndex 1 -TargetIndex $Indices.RoughnessLerp -TargetInputIndex 2
Connect-Ice -SourceIndex $Indices.RoughnessLerp -TargetIndex $Indices.FinalRoughnessLerp -TargetInputIndex 0
Connect-Ice -SourceIndex $Indices.RoughnessMax -TargetIndex $Indices.FinalRoughnessLerp -TargetInputIndex 1
Connect-Ice -SourceIndex $Indices.CrackSat -TargetIndex $Indices.FinalRoughnessLerp -TargetInputIndex 2
Connect-Ice -SourceIndex $Indices.FinalRoughnessLerp -TargetIndex $Indices.EdgeRoughnessLerp -TargetInputIndex 0
Connect-Ice -SourceIndex $Indices.RoughnessMin -TargetIndex $Indices.EdgeRoughnessLerp -TargetInputIndex 1
Connect-Ice -SourceIndex $Indices.RimMask -TargetIndex $Indices.EdgeRoughnessLerp -TargetInputIndex 2

# Material outputs
Connect-Ice -SourceIndex $Indices.EdgeBaseLerp -TargetType "material_pin" -TargetPin "BaseColor"
Connect-Ice -SourceIndex $Indices.EdgeRoughnessLerp -TargetType "material_pin" -TargetPin "Roughness"
Connect-Ice -SourceIndex $Indices.NormalTex -SourceOutputIndex 0 -TargetType "material_pin" -TargetPin "Normal"
Connect-Ice -SourceIndex $Indices.EmissiveColorMul -TargetType "material_pin" -TargetPin "EmissiveColor"

$ParameterExprIndices = @(
    $Indices.TintDeep, $Indices.TintMid, $Indices.TintCrack, $Indices.SSSTint,
    $Indices.MacroTiling, $Indices.MicroTiling, $Indices.CrackTiling,
    $Indices.DeepDepth, $Indices.NearDepth, $Indices.CrackBoost, $Indices.InternalStrength, $Indices.CrackColorStrength,
    $Indices.RoughnessMin, $Indices.RoughnessMax, $Indices.OpacityDensity,
    $Indices.RimPower, $Indices.RimStrength, $Indices.OpacityEdge, $Indices.EmissiveStrength, $Indices.RefractionAmount
)

$LogicExprIndices = @(
    $Indices.TexCoord, $Indices.MacroUV, $Indices.MicroUV, $Indices.CrackUV,
    $Indices.ReflectionWS, $Indices.ReflectionTS, $Indices.OffsetXY, $Indices.DeepOffset, $Indices.NearOffset,
    $Indices.DeepUV, $Indices.NearUV, $Indices.DeepIceTex, $Indices.NearIceTex, $Indices.CrackTex,
    $Indices.NoiseTex, $Indices.NormalTex, $Indices.DeepNearAdd, $Indices.InternalMul, $Indices.InternalSat, $Indices.CrackInvert, $Indices.CrackMul,
    $Indices.CrackSat, $Indices.CrackColorMul, $Indices.VolumeAdd, $Indices.VolumeSat, $Indices.BodyColorLerp, $Indices.FinalBaseLerp,
    $Indices.SubsurfaceLerp, $Indices.Fresnel, $Indices.RimPowerNode, $Indices.RimMask,
    $Indices.EdgeBaseLerp, $Indices.EdgeSSSLerp, $Indices.OpacityLerp, $Indices.CrackEdgeMul, $Indices.EmissiveMaskMul, $Indices.EmissiveColorMul,
    $Indices.RoughnessLerp, $Indices.FinalRoughnessLerp,
    $Indices.EdgeRoughnessLerp
)

Invoke-IceTool -ToolName "create_material_comment" -InputObject @{
    material_path = $MaterialObjectPath
    comment_text = "Ice Parameters"
    editor_x = "-3360"
    editor_y = "-760"
    width = "900"
    height = "2300"
    color_rgba = @(0.95, 0.88, 0.30, 1.0)
    apply_changes = $true
} | Out-Null

Invoke-IceTool -ToolName "create_material_comment" -InputObject @{
    material_path = $MaterialObjectPath
    comment_text = "Ice Fake Volume + Surface"
    editor_x = "-2820"
    editor_y = "-220"
    width = "3200"
    height = "1700"
    color_rgba = @(0.55, 0.78, 0.96, 1.0)
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
    comment_text = "Ice Fake Volume + Surface"
    wrap_expression_indices = $LogicExprIndices
    padding = "140"
    apply_changes = $true
} | Out-Null

try {
    Invoke-IceTool -ToolName "layout_material_expressions" -InputObject @{
        material_path = $MaterialObjectPath
        layout_mode = "horizontal"
        start_x = "-3200"
        start_y = "-650"
        spacing = "240"
        apply_changes = $true
    } | Out-Null
}
catch {
    Write-Host "Layout pass skipped: $($_.Exception.Message)" -ForegroundColor Yellow
}

$MaterialInfo = Invoke-IceTool -ToolName "get_material_info" -InputObject @{
    material_path = $MaterialObjectPath
}

$CompileResult = Invoke-IceTool -ToolName "compile_material_and_get_errors" -InputObject @{
    material_path = $MaterialObjectPath
}

try {
    Invoke-IceTool -ToolName "destroy_actor_by_label" -InputObject @{
        actor_label = $PreviewActorLabel
        apply_changes = $true
    } | Out-Null
}
catch {
}

try {
    Invoke-IceTool -ToolName "destroy_actor_by_label" -InputObject @{
        actor_label = $PreviewLightLabel
        apply_changes = $true
    } | Out-Null
}
catch {
}

Invoke-IceTool -ToolName "spawn_actor" -InputObject @{
    actor_class = "Sphere"
    location = @(0, 0, 180)
    actor_label = $PreviewActorLabel
    apply_changes = $true
} | Out-Null

Invoke-IceTool -ToolName "spawn_actor" -InputObject @{
    actor_class = "PointLight"
    location = @(180, -120, 320)
    actor_label = $PreviewLightLabel
    apply_changes = $true
} | Out-Null

try {
    Invoke-IceTool -ToolName "set_actor_material" -InputObject @{
        actor_label = $PreviewActorLabel
        material_path = $MaterialObjectPath
        material_index = 0
    } | Out-Null
}
catch {
    Write-Host "Preview actor material assignment skipped: $($_.Exception.Message)" -ForegroundColor Yellow
}

$CaptureResult = $null
try {
    $CaptureResult = Invoke-IceTool -ToolName "capture_viewport_screenshot" -InputObject @{
        filename = $ViewportCaptureName
    }
}
catch {
    Write-Host "Viewport capture skipped: $($_.Exception.Message)" -ForegroundColor Yellow
}

Write-Host "`n=== ICE BUILD COMPLETE ===" -ForegroundColor Green
Write-Host "Material: $MaterialObjectPath" -ForegroundColor Green
Write-Host "Preview Actor: $PreviewActorLabel" -ForegroundColor Green
Write-Host "Preview Light: $PreviewLightLabel" -ForegroundColor Green
Write-Host "Compile Succeeded: $($CompileResult.compile_succeeded)" -ForegroundColor White
Write-Host "Expression Count: $($MaterialInfo.expression_count)" -ForegroundColor White
Write-Host "Connections: $(@($MaterialInfo.connections).Count)" -ForegroundColor White
if (-not $CompileResult.compile_succeeded) {
    Write-Host "Compile Errors: $($CompileResult.errors | ConvertTo-Json -Depth 10 -Compress)" -ForegroundColor Red
}
if ($CaptureResult) {
    Write-Host "Viewport Capture Result: $($CaptureResult | ConvertTo-Json -Depth 10 -Compress)" -ForegroundColor White
}
