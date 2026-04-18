param(
    [string]$BridgeHost = $(if ($env:FORGEPILOT_BRIDGE_HOST) { $env:FORGEPILOT_BRIDGE_HOST } else { "127.0.0.1" }),
    [int]$BridgePort = 0,
    [double]$Timeout = 240.0,
    [string]$SavePath = "/Game/ForgePilot_Audit",
    [string]$NoiseTexturePath = "/Game/Character_Appearance_Dissolve/Textures/Standart/T_Clouds.T_Clouds"
)

$ErrorActionPreference = "Stop"

$BridgeHelper = Join-Path $PSScriptRoot "invoke_bridge.ps1"
if (-not (Test-Path $BridgeHelper)) {
    throw "Bridge helper not found: $BridgeHelper"
}

. $BridgeHelper
if ($BridgePort -le 0) { $BridgePort = Get-ForgePilotBridgePortDefault }

Wait-ForgePilotBridge -BridgeHost $BridgeHost -BridgePort $BridgePort -TimeoutSeconds ([int][Math]::Ceiling($Timeout))

$Stamp = Get-Date -Format "yyyyMMdd_HHmmss"
$RunName = "orb_material_$Stamp"
$OutputDir = Join-Path "$env:USERPROFILE\Downloads\ForgePilotMaterialProbes" $RunName
New-Item -ItemType Directory -Force -Path $OutputDir | Out-Null

$MaterialName = "M_OrbTest_$Stamp"
$MaterialPath = "$SavePath/$MaterialName"
$MaterialObjectPath = "$MaterialPath.$MaterialName"
$ProbeActorLabel = "ForgePilot_OrbProbe_$Stamp"

$Summary = [ordered]@{
    success = $false
    run_id = $Stamp
    output_dir = $OutputDir
    material_path = $MaterialPath
    material_object_path = $MaterialObjectPath
    probe_actor_label = $ProbeActorLabel
    steps = [ordered]@{}
    expressions = [ordered]@{}
    captures = [ordered]@{}
    variants = @()
}

function Invoke-OrbTool {
    param(
        [Parameter(Mandatory = $true)][string]$ToolName,
        [hashtable]$InputObject = @{}
    )

    Invoke-ForgePilotTool -BridgeHost $BridgeHost -BridgePort $BridgePort -ToolName $ToolName -InputObject $InputObject -TimeoutSeconds ([int][Math]::Ceiling($Timeout))
}

function Add-OrbExpr {
    param(
        [Parameter(Mandatory = $true)][string]$Key,
        [Parameter(Mandatory = $true)][string]$Type,
        [int]$X,
        [int]$Y,
        [string]$ParamName = "",
        [string]$DefaultValue = "",
        [string]$TexturePath = ""
    )

    $Input = @{
        material_path = $MaterialObjectPath
        expression_type = $Type
        editor_x = $X.ToString()
        editor_y = $Y.ToString()
        apply_changes = $true
    }
    if ($ParamName) { $Input.param_name = $ParamName }
    if ($DefaultValue) { $Input.default_value = $DefaultValue }
    if ($TexturePath) { $Input.texture_path = $TexturePath }

    $Result = Invoke-OrbTool -ToolName "add_material_expression" -InputObject $Input
    $Summary.expressions[$Key] = $Result
    return [int]$Result.expression_index
}

function Connect-Orb {
    param(
        [Parameter(Mandatory = $true)][int]$SourceIndex,
        [Parameter(Mandatory = $true)][int]$SourceOutputIndex,
        [Parameter(Mandatory = $true)][string]$TargetType,
        [string]$TargetPin = "",
        [int]$TargetIndex = -1,
        [int]$TargetInputIndex = -1
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
        $Input.target_input_index = $TargetInputIndex
    }

    Invoke-OrbTool -ToolName "connect_material_nodes" -InputObject $Input | Out-Null
}

function Json-ObjectString {
    param([hashtable]$Hashtable)
    return ($Hashtable | ConvertTo-Json -Depth 20 -Compress)
}

try {
    $Summary.steps.editor_context = Invoke-OrbTool -ToolName "get_material_editor_context"

    $Summary.steps.create_material = Invoke-OrbTool -ToolName "create_material" -InputObject @{
        material_name = $MaterialName
        save_path = $SavePath
        base_color = @(0.00, 0.09, 0.07)
    }

    $Summary.steps.set_properties = Invoke-OrbTool -ToolName "set_material_properties" -InputObject @{
        material_path = $MaterialObjectPath
        blend_mode = "Opaque"
        shading_model = "Unlit"
        apply_changes = $true
    }

    $Indices = [ordered]@{}

    # Parameters
    $Indices.ColorA = Add-OrbExpr -Key "ColorA" -Type "VectorParameter" -X -2500 -Y -280 -ParamName "Orb_ColorA" -DefaultValue "0.00,0.09,0.07,1.0"
    $Indices.ColorB = Add-OrbExpr -Key "ColorB" -Type "VectorParameter" -X -2500 -Y -120 -ParamName "Orb_ColorB" -DefaultValue "0.10,0.72,0.60,1.0"
    $Indices.RimColor = Add-OrbExpr -Key "RimColor" -Type "VectorParameter" -X -2500 -Y 80 -ParamName "Orb_RimColor" -DefaultValue "0.34,0.96,0.82,1.0"
    $Indices.CoreColor = Add-OrbExpr -Key "CoreColor" -Type "VectorParameter" -X -2500 -Y 260 -ParamName "Orb_CoreColor" -DefaultValue "0.20,0.88,0.70,1.0"
    $Indices.BodyIntensity = Add-OrbExpr -Key "BodyIntensity" -Type "ScalarParameter" -X -2500 -Y 470 -ParamName "Orb_BodyIntensity" -DefaultValue "1.10"
    $Indices.GlobalIntensity = Add-OrbExpr -Key "GlobalIntensity" -Type "ScalarParameter" -X -2500 -Y 610 -ParamName "Orb_GlobalIntensity" -DefaultValue "1.00"
    $Indices.NoiseTex = Add-OrbExpr -Key "NoiseTex" -Type "TextureSampleParameter2D" -X -2500 -Y 790 -ParamName "Orb_NoiseTex" -TexturePath $NoiseTexturePath
    $Indices.Tiling = Add-OrbExpr -Key "Tiling" -Type "ScalarParameter" -X -2500 -Y 990 -ParamName "Orb_Tiling" -DefaultValue "1.35"
    $Indices.ColorContrast = Add-OrbExpr -Key "ColorContrast" -Type "ScalarParameter" -X -2500 -Y 1130 -ParamName "Orb_ColorContrast" -DefaultValue "2.2"
    $Indices.BodyContrast = Add-OrbExpr -Key "BodyContrast" -Type "ScalarParameter" -X -2500 -Y 1270 -ParamName "Orb_BodyContrast" -DefaultValue "3.0"
    $Indices.PanAX = Add-OrbExpr -Key "PanAX" -Type "ScalarParameter" -X -2500 -Y 1490 -ParamName "Orb_PanAX" -DefaultValue "0.08"
    $Indices.PanAY = Add-OrbExpr -Key "PanAY" -Type "ScalarParameter" -X -2500 -Y 1610 -ParamName "Orb_PanAY" -DefaultValue "0.05"
    $Indices.PanBX = Add-OrbExpr -Key "PanBX" -Type "ScalarParameter" -X -2500 -Y 1730 -ParamName "Orb_PanBX" -DefaultValue "-0.04"
    $Indices.PanBY = Add-OrbExpr -Key "PanBY" -Type "ScalarParameter" -X -2500 -Y 1850 -ParamName "Orb_PanBY" -DefaultValue "0.07"
    $Indices.RimPower = Add-OrbExpr -Key "RimPower" -Type "ScalarParameter" -X -1500 -Y 80 -ParamName "Orb_RimPower" -DefaultValue "5.5"
    $Indices.RimIntensity = Add-OrbExpr -Key "RimIntensity" -Type "ScalarParameter" -X -1500 -Y 220 -ParamName "Orb_RimIntensity" -DefaultValue "0.70"
    $Indices.CorePower = Add-OrbExpr -Key "CorePower" -Type "ScalarParameter" -X -1500 -Y 430 -ParamName "Orb_CorePower" -DefaultValue "4.5"
    $Indices.CoreIntensity = Add-OrbExpr -Key "CoreIntensity" -Type "ScalarParameter" -X -1500 -Y 570 -ParamName "Orb_CoreIntensity" -DefaultValue "0.12"

    # UV and noise
    $Indices.TexCoord = Add-OrbExpr -Key "TexCoord" -Type "TexCoord" -X -1900 -Y 930
    $Indices.UVScale = Add-OrbExpr -Key "UVScale" -Type "Multiply" -X -1650 -Y 930
    $Indices.SecondaryScaleConst = Add-OrbExpr -Key "SecondaryScaleConst" -Type "Constant2" -X -1650 -Y 1120 -DefaultValue "1.25,1.25,0.0,1.0"
    $Indices.UVSecondary = Add-OrbExpr -Key "UVSecondary" -Type "Multiply" -X -1420 -Y 1120
    $Indices.PanSpeedA = Add-OrbExpr -Key "PanSpeedA" -Type "AppendVector" -X -1650 -Y 1490
    $Indices.PanSpeedB = Add-OrbExpr -Key "PanSpeedB" -Type "AppendVector" -X -1650 -Y 1730
    $Indices.PannerA = Add-OrbExpr -Key "PannerA" -Type "Panner" -X -1420 -Y 1460
    $Indices.PannerB = Add-OrbExpr -Key "PannerB" -Type "Panner" -X -1420 -Y 1700
    $Indices.SampleB = Add-OrbExpr -Key "SampleB" -Type "TextureSample" -X -1180 -Y 1680 -TexturePath $NoiseTexturePath

    # Body
    $Indices.PowerBody = Add-OrbExpr -Key "PowerBody" -Type "Power" -X -390 -Y 1490
    $Indices.SaturateBody = Add-OrbExpr -Key "SaturateBody" -Type "Saturate" -X -150 -Y 1490
    $Indices.PowerColor = Add-OrbExpr -Key "PowerColor" -Type "Power" -X -390 -Y 1710
    $Indices.SaturateColor = Add-OrbExpr -Key "SaturateColor" -Type "Saturate" -X -150 -Y 1710
    $Indices.LerpColor = Add-OrbExpr -Key "LerpColor" -Type "Lerp" -X 140 -Y 100
    $Indices.BodyMulMask = Add-OrbExpr -Key "BodyMulMask" -Type "Multiply" -X 380 -Y 260
    $Indices.BodyMulIntensity = Add-OrbExpr -Key "BodyMulIntensity" -Type "Multiply" -X 620 -Y 260

    # Rim/Core
    $Indices.FresnelRim = Add-OrbExpr -Key "FresnelRim" -Type "Fresnel" -X -900 -Y 60
    $Indices.PowerRim = Add-OrbExpr -Key "PowerRim" -Type "Power" -X -640 -Y 60
    $Indices.RimMulIntensity = Add-OrbExpr -Key "RimMulIntensity" -Type "Multiply" -X -390 -Y 60
    $Indices.RimMulColor = Add-OrbExpr -Key "RimMulColor" -Type "Multiply" -X -140 -Y 60
    $Indices.FresnelCore = Add-OrbExpr -Key "FresnelCore" -Type "Fresnel" -X -900 -Y 420
    $Indices.OneMinusCore = Add-OrbExpr -Key "OneMinusCore" -Type "OneMinus" -X -640 -Y 420
    $Indices.PowerCore = Add-OrbExpr -Key "PowerCore" -Type "Power" -X -390 -Y 420
    $Indices.CoreMulIntensity = Add-OrbExpr -Key "CoreMulIntensity" -Type "Multiply" -X -140 -Y 420
    $Indices.CoreMulColor = Add-OrbExpr -Key "CoreMulColor" -Type "Multiply" -X 100 -Y 420

    # Final
    $Indices.AddBodyRim = Add-OrbExpr -Key "AddBodyRim" -Type "Add" -X 860 -Y 180
    $Indices.AddFinal = Add-OrbExpr -Key "AddFinal" -Type "Add" -X 1090 -Y 260
    $Indices.MulGlobal = Add-OrbExpr -Key "MulGlobal" -Type "Multiply" -X 1320 -Y 260

    # Wiring
    Connect-Orb -SourceIndex $Indices.TexCoord -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.UVScale -TargetInputIndex 0
    Connect-Orb -SourceIndex $Indices.Tiling -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.UVScale -TargetInputIndex 1
    Connect-Orb -SourceIndex $Indices.UVScale -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.UVSecondary -TargetInputIndex 0
    Connect-Orb -SourceIndex $Indices.SecondaryScaleConst -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.UVSecondary -TargetInputIndex 1

    Connect-Orb -SourceIndex $Indices.PanAX -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.PanSpeedA -TargetInputIndex 0
    Connect-Orb -SourceIndex $Indices.PanAY -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.PanSpeedA -TargetInputIndex 1
    Connect-Orb -SourceIndex $Indices.PanBX -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.PanSpeedB -TargetInputIndex 0
    Connect-Orb -SourceIndex $Indices.PanBY -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.PanSpeedB -TargetInputIndex 1

    Connect-Orb -SourceIndex $Indices.UVScale -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.PannerA -TargetInputIndex 0
    Connect-Orb -SourceIndex $Indices.PanSpeedA -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.PannerA -TargetInputIndex 2
    Connect-Orb -SourceIndex $Indices.UVSecondary -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.PannerB -TargetInputIndex 0
    Connect-Orb -SourceIndex $Indices.PanSpeedB -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.PannerB -TargetInputIndex 2

    Connect-Orb -SourceIndex $Indices.PannerA -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.NoiseTex -TargetInputIndex 0
    Connect-Orb -SourceIndex $Indices.PannerB -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.SampleB -TargetInputIndex 0

    Connect-Orb -SourceIndex $Indices.NoiseTex -SourceOutputIndex 1 -TargetType "expression" -TargetIndex $Indices.PowerBody -TargetInputIndex 0
    Connect-Orb -SourceIndex $Indices.BodyContrast -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.PowerBody -TargetInputIndex 1
    Connect-Orb -SourceIndex $Indices.PowerBody -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.SaturateBody -TargetInputIndex 0

    Connect-Orb -SourceIndex $Indices.SampleB -SourceOutputIndex 1 -TargetType "expression" -TargetIndex $Indices.PowerColor -TargetInputIndex 0
    Connect-Orb -SourceIndex $Indices.ColorContrast -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.PowerColor -TargetInputIndex 1
    Connect-Orb -SourceIndex $Indices.PowerColor -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.SaturateColor -TargetInputIndex 0

    Connect-Orb -SourceIndex $Indices.ColorA -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.LerpColor -TargetInputIndex 0
    Connect-Orb -SourceIndex $Indices.ColorB -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.LerpColor -TargetInputIndex 1
    Connect-Orb -SourceIndex $Indices.SaturateColor -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.LerpColor -TargetInputIndex 2

    Connect-Orb -SourceIndex $Indices.LerpColor -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.BodyMulMask -TargetInputIndex 0
    Connect-Orb -SourceIndex $Indices.SaturateBody -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.BodyMulMask -TargetInputIndex 1
    Connect-Orb -SourceIndex $Indices.BodyMulMask -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.BodyMulIntensity -TargetInputIndex 0
    Connect-Orb -SourceIndex $Indices.BodyIntensity -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.BodyMulIntensity -TargetInputIndex 1

    Connect-Orb -SourceIndex $Indices.FresnelRim -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.PowerRim -TargetInputIndex 0
    Connect-Orb -SourceIndex $Indices.RimPower -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.PowerRim -TargetInputIndex 1
    Connect-Orb -SourceIndex $Indices.PowerRim -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.RimMulIntensity -TargetInputIndex 0
    Connect-Orb -SourceIndex $Indices.RimIntensity -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.RimMulIntensity -TargetInputIndex 1
    Connect-Orb -SourceIndex $Indices.RimMulIntensity -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.RimMulColor -TargetInputIndex 0
    Connect-Orb -SourceIndex $Indices.RimColor -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.RimMulColor -TargetInputIndex 1

    Connect-Orb -SourceIndex $Indices.FresnelCore -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.OneMinusCore -TargetInputIndex 0
    Connect-Orb -SourceIndex $Indices.OneMinusCore -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.PowerCore -TargetInputIndex 0
    Connect-Orb -SourceIndex $Indices.CorePower -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.PowerCore -TargetInputIndex 1
    Connect-Orb -SourceIndex $Indices.PowerCore -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.CoreMulIntensity -TargetInputIndex 0
    Connect-Orb -SourceIndex $Indices.CoreIntensity -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.CoreMulIntensity -TargetInputIndex 1
    Connect-Orb -SourceIndex $Indices.CoreMulIntensity -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.CoreMulColor -TargetInputIndex 0
    Connect-Orb -SourceIndex $Indices.CoreColor -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.CoreMulColor -TargetInputIndex 1

    Connect-Orb -SourceIndex $Indices.BodyMulIntensity -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.AddBodyRim -TargetInputIndex 0
    Connect-Orb -SourceIndex $Indices.RimMulColor -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.AddBodyRim -TargetInputIndex 1
    Connect-Orb -SourceIndex $Indices.AddBodyRim -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.AddFinal -TargetInputIndex 0
    Connect-Orb -SourceIndex $Indices.CoreMulColor -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.AddFinal -TargetInputIndex 1
    Connect-Orb -SourceIndex $Indices.AddFinal -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.MulGlobal -TargetInputIndex 0
    Connect-Orb -SourceIndex $Indices.GlobalIntensity -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.MulGlobal -TargetInputIndex 1

    Connect-Orb -SourceIndex $Indices.MulGlobal -SourceOutputIndex 0 -TargetType "material_pin" -TargetPin "EmissiveColor"
    Connect-Orb -SourceIndex $Indices.BodyMulMask -SourceOutputIndex 0 -TargetType "material_pin" -TargetPin "BaseColor"

    $ParameterExprIndices = @(
        $Indices.ColorA, $Indices.ColorB, $Indices.RimColor, $Indices.CoreColor, $Indices.BodyIntensity,
        $Indices.GlobalIntensity, $Indices.NoiseTex, $Indices.Tiling, $Indices.ColorContrast, $Indices.BodyContrast,
        $Indices.PanAX, $Indices.PanAY, $Indices.PanBX, $Indices.PanBY, $Indices.RimPower, $Indices.RimIntensity,
        $Indices.CorePower, $Indices.CoreIntensity
    )
    $LogicExprIndices = @(
        $Indices.TexCoord, $Indices.UVScale, $Indices.SecondaryScaleConst, $Indices.UVSecondary, $Indices.PanSpeedA,
        $Indices.PanSpeedB, $Indices.PannerA, $Indices.PannerB, $Indices.SampleB,
        $Indices.PowerBody, $Indices.SaturateBody, $Indices.PowerColor, $Indices.SaturateColor,
        $Indices.LerpColor, $Indices.BodyMulMask, $Indices.BodyMulIntensity, $Indices.FresnelRim, $Indices.PowerRim,
        $Indices.RimMulIntensity, $Indices.RimMulColor, $Indices.FresnelCore, $Indices.OneMinusCore, $Indices.PowerCore,
        $Indices.CoreMulIntensity, $Indices.CoreMulColor, $Indices.AddBodyRim, $Indices.AddFinal, $Indices.MulGlobal
    )

    $Summary.steps.parameter_comment = Invoke-OrbTool -ToolName "create_material_comment" -InputObject @{
        material_path = $MaterialObjectPath
        comment_text = "Orb Parameters"
        editor_x = "-2660"
        editor_y = "-420"
        width = "1260"
        height = "2400"
        color_rgba = @(1.0, 0.92, 0.15, 1.0)
        apply_changes = $true
    }
    $Summary.steps.logic_comment = Invoke-OrbTool -ToolName "create_material_comment" -InputObject @{
        material_path = $MaterialObjectPath
        comment_text = "AI Orb Logic"
        editor_x = "-2050"
        editor_y = "-120"
        width = "3800"
        height = "2200"
        color_rgba = @(0.95, 0.90, 0.35, 1.0)
        apply_changes = $true
    }

    $Summary.steps.parameter_comment_wrap = Invoke-OrbTool -ToolName "update_material_comment" -InputObject @{
        material_path = $MaterialObjectPath
        comment_text = "Orb Parameters"
        wrap_expression_indices = $ParameterExprIndices
        padding = "120"
        apply_changes = $true
    }
    $Summary.steps.logic_comment_wrap = Invoke-OrbTool -ToolName "update_material_comment" -InputObject @{
        material_path = $MaterialObjectPath
        comment_text = "AI Orb Logic"
        wrap_expression_indices = $LogicExprIndices
        padding = "140"
        apply_changes = $true
    }

    $Summary.steps.validate_logic = Invoke-OrbTool -ToolName "validate_material_graph_block" -InputObject @{
        material_path = $MaterialObjectPath
        comment_text = "AI Orb Logic"
    }
    $Summary.steps.wiring_summary = Invoke-OrbTool -ToolName "summarize_material_block_wiring" -InputObject @{
        material_path = $MaterialObjectPath
        comment_text = "AI Orb Logic"
    }
    $Summary.steps.simplification_review = Invoke-OrbTool -ToolName "review_material_graph_simplification" -InputObject @{
        material_path = $MaterialObjectPath
        comment_text = "AI Orb Logic"
    }
    $Summary.steps.material_review = Invoke-OrbTool -ToolName "review_material_asset" -InputObject @{
        material_path = $MaterialObjectPath
        creative_mode = $true
    }
    $Summary.steps.material_info = Invoke-OrbTool -ToolName "get_material_info" -InputObject @{
        material_path = $MaterialObjectPath
    }

    $Summary.steps.focus_logic = Invoke-OrbTool -ToolName "focus_material_graph_comment" -InputObject @{
        material_path = $MaterialObjectPath
        comment_text = "AI Orb Logic"
        padding = "220"
    }
    $Summary.steps.zoom_logic = Invoke-OrbTool -ToolName "set_material_graph_zoom" -InputObject @{
        material_path = $MaterialObjectPath
        zoom = "0.85"
    }
    $Summary.captures.logic_graph = Invoke-OrbTool -ToolName "capture_material_graph_screenshot" -InputObject @{
        material_path = $MaterialObjectPath
        comment_text = "AI Orb Logic"
        output_dir = $OutputDir
        file_name = "orb_logic_graph.png"
        image_width = "2000"
        image_height = "1200"
        padding = "220"
    }
    $Summary.steps.focus_parameters = Invoke-OrbTool -ToolName "focus_material_graph_comment" -InputObject @{
        material_path = $MaterialObjectPath
        comment_text = "Orb Parameters"
        padding = "160"
    }
    $Summary.steps.zoom_parameters = Invoke-OrbTool -ToolName "set_material_graph_zoom" -InputObject @{
        material_path = $MaterialObjectPath
        zoom = "1.0"
    }
    $Summary.captures.parameter_graph = Invoke-OrbTool -ToolName "capture_material_graph_screenshot" -InputObject @{
        material_path = $MaterialObjectPath
        comment_text = "Orb Parameters"
        output_dir = $OutputDir
        file_name = "orb_parameters_graph.png"
        image_width = "1500"
        image_height = "1200"
        padding = "180"
    }

    $Variants = @(
        @{
            name = "Soft"
            scalar = @{
                Orb_BodyIntensity = 0.80
                Orb_GlobalIntensity = 0.78
                Orb_BodyContrast = 2.6
                Orb_ColorContrast = 1.8
                Orb_RimIntensity = 0.52
                Orb_CoreIntensity = 0.10
                Orb_Tiling = 1.10
            }
            vector = @{
                Orb_ColorA = @(0.00, 0.07, 0.05, 1.0)
                Orb_ColorB = @(0.07, 0.48, 0.40, 1.0)
                Orb_RimColor = @(0.18, 0.78, 0.66, 1.0)
                Orb_CoreColor = @(0.10, 0.56, 0.44, 1.0)
            }
        },
        @{
            name = "Balanced"
            scalar = @{
                Orb_BodyIntensity = 1.10
                Orb_GlobalIntensity = 1.05
                Orb_BodyContrast = 3.0
                Orb_ColorContrast = 1.9
                Orb_RimPower = 2.8
                Orb_RimIntensity = 1.15
                Orb_CorePower = 2.2
                Orb_CoreIntensity = 0.18
                Orb_Tiling = 1.35
            }
            vector = @{
                Orb_ColorA = @(0.00, 0.09, 0.07, 1.0)
                Orb_ColorB = @(0.14, 0.84, 0.70, 1.0)
                Orb_RimColor = @(0.50, 1.00, 0.88, 1.0)
                Orb_CoreColor = @(0.22, 0.92, 0.74, 1.0)
            }
        },
        @{
            name = "Bright"
            scalar = @{
                Orb_BodyIntensity = 1.30
                Orb_GlobalIntensity = 1.15
                Orb_BodyContrast = 3.2
                Orb_ColorContrast = 1.8
                Orb_RimPower = 2.4
                Orb_RimIntensity = 1.35
                Orb_CorePower = 2.0
                Orb_CoreIntensity = 0.24
                Orb_Tiling = 1.55
            }
            vector = @{
                Orb_ColorA = @(0.01, 0.12, 0.09, 1.0)
                Orb_ColorB = @(0.18, 0.98, 0.82, 1.0)
                Orb_RimColor = @(0.62, 1.0, 0.92, 1.0)
                Orb_CoreColor = @(0.28, 0.98, 0.80, 1.0)
            }
        },
        @{
            name = "Dense"
            scalar = @{
                Orb_BodyIntensity = 1.18
                Orb_GlobalIntensity = 0.92
                Orb_BodyContrast = 4.0
                Orb_ColorContrast = 3.0
                Orb_RimIntensity = 0.74
                Orb_CoreIntensity = 0.12
                Orb_Tiling = 0.95
            }
            vector = @{
                Orb_ColorA = @(0.00, 0.08, 0.06, 1.0)
                Orb_ColorB = @(0.09, 0.66, 0.54, 1.0)
                Orb_RimColor = @(0.30, 0.92, 0.78, 1.0)
                Orb_CoreColor = @(0.18, 0.82, 0.64, 1.0)
            }
        }
    )

    $Summary.steps.spawn_probe_actor = Invoke-OrbTool -ToolName "spawn_material_probe_actor" -InputObject @{
        actor_label = $ProbeActorLabel
        location = @(0, 0, 120)
        scale = @(3.0, 3.0, 3.0)
    }

    foreach ($Variant in $Variants) {
        $VariantName = [string]$Variant.name
        $InstanceName = "MI_OrbTest_${VariantName}_$Stamp"
        $InstanceResult = Invoke-OrbTool -ToolName "create_material_instance" -InputObject @{
            instance_name = $InstanceName
            parent_material_path = $MaterialObjectPath
            save_path = $SavePath
            scalar_overrides = (Json-ObjectString $Variant.scalar)
            vector_overrides = (Json-ObjectString $Variant.vector)
        }

        $ApplyResult = Invoke-OrbTool -ToolName "apply_material_to_probe_actor" -InputObject @{
            actor_label = $ProbeActorLabel
            material_path = $InstanceResult.asset_path
        }
        $CaptureResult = Invoke-OrbTool -ToolName "capture_material_visual_probe" -InputObject @{
            actor_label = $ProbeActorLabel
            output_dir = $OutputDir
            file_name = "orb_${VariantName}.jpg"
            frame_actor = $true
            wait_ms = "200"
        }

        $Summary.variants += [ordered]@{
            name = $VariantName
            instance = $InstanceResult
            apply = $ApplyResult
            capture = $CaptureResult
        }
    }

    if ($Summary.variants.Count -ge 2) {
        $Summary.steps.compare_soft_balanced = Invoke-OrbTool -ToolName "compare_material_probe_captures" -InputObject @{
            image_a_path = $Summary.variants[0].capture.output_path
            image_b_path = $Summary.variants[1].capture.output_path
        }
        $Summary.steps.compare_balanced_bright = Invoke-OrbTool -ToolName "compare_material_probe_captures" -InputObject @{
            image_a_path = $Summary.variants[1].capture.output_path
            image_b_path = $Summary.variants[2].capture.output_path
        }
    }

    $Summary.success = $true
}
finally {
    try {
        $Summary.cleanup = Invoke-OrbTool -ToolName "destroy_actor_by_label" -InputObject @{
            actor_label = $ProbeActorLabel
            apply_changes = $true
        }
    }
    catch {
    }

    $SummaryPath = Join-Path $OutputDir "summary.json"
    $Summary | ConvertTo-Json -Depth 100 | Set-Content -Path $SummaryPath -Encoding UTF8
    Write-Host "Orb material summary written to: $SummaryPath"
}


