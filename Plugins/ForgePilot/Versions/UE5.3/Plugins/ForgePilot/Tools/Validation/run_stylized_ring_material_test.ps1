param(
    [string]$BridgeHost = $(if ($env:FORGEPILOT_BRIDGE_HOST) { $env:FORGEPILOT_BRIDGE_HOST } else { "127.0.0.1" }),
    [int]$BridgePort = 0,
    [double]$Timeout = 240.0,
    [string]$SavePath = "/Game/ForgePilot_Audit",
    [string]$AuraTexturePath = "/Game/Character_Appearance_Dissolve/Textures/Standart/T_Clouds.T_Clouds"
)

$ErrorActionPreference = "Stop"

$BridgeHelper = Join-Path $PSScriptRoot "invoke_bridge.ps1"
if (-not (Test-Path $BridgeHelper)) {
    throw "Bridge helper not found: $BridgeHelper"
}
. $BridgeHelper
if ($BridgePort -le 0) { $BridgePort = Get-ForgePilotBridgePortDefault }

$WindowCaptureHelper = Join-Path $PSScriptRoot "capture_unreal_main_window.ps1"
if (-not (Test-Path $WindowCaptureHelper)) {
    throw "Window capture helper not found: $WindowCaptureHelper"
}

$DismissRestoreHelper = Join-Path $PSScriptRoot "dismiss_unreal_restore_dialog.ps1"
if (Test-Path $DismissRestoreHelper) {
    & $DismissRestoreHelper | Out-Null
    Start-Sleep -Milliseconds 600
}

Wait-ForgePilotBridge -BridgeHost $BridgeHost -BridgePort $BridgePort -TimeoutSeconds ([int][Math]::Ceiling($Timeout))

$Stamp = Get-Date -Format "yyyyMMdd_HHmmss"
$RunName = "stylized_ring_$Stamp"
$OutputDir = Join-Path "$env:USERPROFILE\Downloads\ForgePilotMaterialProbes" $RunName
New-Item -ItemType Directory -Force -Path $OutputDir | Out-Null

$MaterialName = "M_StylizedRing_$Stamp"
$MaterialPath = "$SavePath/$MaterialName"
$MaterialObjectPath = "$MaterialPath.$MaterialName"
$ProbeActorLabel = "ForgePilot_StylizedRingProbe_$Stamp"

$Summary = [ordered]@{
    success = $false
    run_id = $Stamp
    output_dir = $OutputDir
    material_path = $MaterialPath
    material_object_path = $MaterialObjectPath
    probe_actor_label = $ProbeActorLabel
    texture_path = $AuraTexturePath
    steps = [ordered]@{}
    expressions = [ordered]@{}
    variants = @()
    captures = [ordered]@{}
}

function Invoke-RingTool {
    param(
        [Parameter(Mandatory = $true)][string]$ToolName,
        [hashtable]$InputObject = @{}
    )

    Invoke-ForgePilotTool -BridgeHost $BridgeHost -BridgePort $BridgePort -ToolName $ToolName -InputObject $InputObject -TimeoutSeconds ([int][Math]::Ceiling($Timeout))
}

function Add-RingExpr {
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

    $Result = Invoke-RingTool -ToolName "add_material_expression" -InputObject $Input
    $Summary.expressions[$Key] = $Result
    return [int]$Result.expression_index
}

function Add-CustomExpr {
    param(
        [Parameter(Mandatory = $true)][string]$Key,
        [Parameter(Mandatory = $true)][string]$Description,
        [Parameter(Mandatory = $true)][string]$Code,
        [Parameter(Mandatory = $true)][string]$OutputType,
        [Parameter(Mandatory = $true)][string[]]$InputNames,
        [int]$X,
        [int]$Y
    )

    $Result = Invoke-RingTool -ToolName "set_material_custom_node" -InputObject @{
        material_path = $MaterialObjectPath
        description = $Description
        code = $Code
        output_type = $OutputType
        input_names = $InputNames
        editor_x = $X.ToString()
        editor_y = $Y.ToString()
        apply_changes = $true
    }

    $Summary.expressions[$Key] = $Result
    return [int]$Result.expression_index
}

function Connect-Ring {
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

    Invoke-RingTool -ToolName "connect_material_nodes" -InputObject $Input | Out-Null
}

function Json-ObjectString {
    param([hashtable]$Hashtable)
    return ($Hashtable | ConvertTo-Json -Depth 20 -Compress)
}

try {
    $Summary.steps.editor_context = Invoke-RingTool -ToolName "get_material_editor_context"

    $Summary.steps.create_material = Invoke-RingTool -ToolName "create_material" -InputObject @{
        material_name = $MaterialName
        save_path = $SavePath
        base_color = @(0.0, 0.0, 0.0)
    }

    $Summary.steps.set_properties = Invoke-RingTool -ToolName "set_material_properties" -InputObject @{
        material_path = $MaterialObjectPath
        blend_mode = "Translucent"
        shading_model = "Unlit"
        two_sided = $true
        thumbnail_primitive = "Plane"
        preview_mesh_path = "/Engine/BasicShapes/Plane.Plane"
        apply_changes = $true
    }

    $Indices = [ordered]@{}

    $Indices.TexCoord = Add-RingExpr -Key "TexCoord" -Type "TexCoord" -X -2600 -Y 260
    $Indices.TX = Add-RingExpr -Key "TX" -Type "ScalarParameter" -X -2600 -Y 580 -ParamName "TX" -DefaultValue "2.0"
    $Indices.TY = Add-RingExpr -Key "TY" -Type "ScalarParameter" -X -2600 -Y 720 -ParamName "TY" -DefaultValue "2.0"
    $Indices.AppendXY = Add-RingExpr -Key "AppendXY" -Type "AppendVector" -X -2280 -Y 650
    $Indices.RingThickness = Add-RingExpr -Key "RingThickness" -Type "ScalarParameter" -X -2600 -Y 900 -ParamName "RingThickness" -DefaultValue "2.2"
    $Indices.Layers = Add-RingExpr -Key "Layers" -Type "ScalarParameter" -X -2600 -Y 1040 -ParamName "Layers" -DefaultValue "4.0"
    $Indices.IntensityMul = Add-RingExpr -Key "IntensityMul" -Type "ScalarParameter" -X -2600 -Y 1180 -ParamName "IntensityMul" -DefaultValue "1.35"
    $Indices.OpacityIntensity = Add-RingExpr -Key "OpacityIntensity" -Type "ScalarParameter" -X -2600 -Y 1320 -ParamName "OpacityIntensity" -DefaultValue "1.0"
    $Indices.HotPower = Add-RingExpr -Key "HotPower" -Type "ScalarParameter" -X -2600 -Y 1460 -ParamName "HotPower" -DefaultValue "6.0"
    $Indices.DarkColor = Add-RingExpr -Key "DarkColor" -Type "VectorParameter" -X -2600 -Y 1660 -ParamName "DarkColor" -DefaultValue "[0.03,0.12,0.42,1.0]"
    $Indices.MidColor = Add-RingExpr -Key "MidColor" -Type "VectorParameter" -X -2600 -Y 1840 -ParamName "MidColor" -DefaultValue "[0.18,0.75,1.00,1.0]"
    $Indices.HotColor = Add-RingExpr -Key "HotColor" -Type "VectorParameter" -X -2600 -Y 2020 -ParamName "HotColor" -DefaultValue "[4.0,4.0,4.0,1.0]"
    $Indices.AuraTex = Add-RingExpr -Key "AuraTex" -Type "TextureSampleParameter2D" -X -1480 -Y 240 -ParamName "AuraTex" -TexturePath $AuraTexturePath

    $RadialUVCode = @"
float2 uv = UV.xy;
float2 centered = uv - float2(0.5, 0.5);
float angle = atan2(centered.y, centered.x);
float angle01 = angle / 6.28318530718 + 0.5;
float radius = saturate(length(centered) * 2.0);
return float2(angle01 * max(Tiling.x, 0.01), radius * max(Tiling.y, 0.01));
"@
    $Indices.RadialUV = Add-CustomExpr -Key "RadialUV" -Description "AI_RadialUV" -Code $RadialUVCode -OutputType "Float2" -InputNames @("UV", "Tiling") -X -1760 -Y 240

    $RingMaskCode = @"
float2 centered = UV.xy - float2(0.5, 0.5);
float radial = saturate(length(centered) * 2.0);
float ring = saturate(sin(radial * 3.14159265));
ring = pow(max(ring, 0.0001), max(Thickness, 0.1));
return ring;
"@
    $Indices.RingMask = Add-CustomExpr -Key "RingMask" -Description "AI_RingMask" -Code $RingMaskCode -OutputType "Float1" -InputNames @("UV", "Thickness") -X -1760 -Y 740

    $Indices.StylizedMul = Add-RingExpr -Key "StylizedMul" -Type "Multiply" -X -1160 -Y 500
    $Indices.PosterizeMul = Add-RingExpr -Key "PosterizeMul" -Type "Multiply" -X -900 -Y 500
    $Indices.Floor = Add-RingExpr -Key "Floor" -Type "Floor" -X -640 -Y 500
    $Indices.Divide = Add-RingExpr -Key "Divide" -Type "Divide" -X -380 -Y 500
    $Indices.Boost = Add-RingExpr -Key "Boost" -Type "Multiply" -X -120 -Y 500
    $Indices.Saturate = Add-RingExpr -Key "Saturate" -Type "Saturate" -X 140 -Y 500
    $Indices.BaseLerp = Add-RingExpr -Key "BaseLerp" -Type "Lerp" -X 480 -Y 300
    $Indices.HotPow = Add-RingExpr -Key "HotPow" -Type "Power" -X 480 -Y 720
    $Indices.HotMul = Add-RingExpr -Key "HotMul" -Type "Multiply" -X 760 -Y 720
    $Indices.FinalAdd = Add-RingExpr -Key "FinalAdd" -Type "Add" -X 1040 -Y 440
    $Indices.EmissiveMul = Add-RingExpr -Key "EmissiveMul" -Type "Multiply" -X 1320 -Y 440
    $Indices.OpacityMul = Add-RingExpr -Key "OpacityMul" -Type "Multiply" -X 1320 -Y 760

    Connect-Ring -SourceIndex $Indices.TX -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.AppendXY -TargetInputIndex 0
    Connect-Ring -SourceIndex $Indices.TY -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.AppendXY -TargetInputIndex 1

    Connect-Ring -SourceIndex $Indices.TexCoord -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.RadialUV -TargetInputIndex 0
    Connect-Ring -SourceIndex $Indices.AppendXY -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.RadialUV -TargetInputIndex 1
    Connect-Ring -SourceIndex $Indices.RadialUV -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.AuraTex -TargetInputIndex 0

    Connect-Ring -SourceIndex $Indices.TexCoord -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.RingMask -TargetInputIndex 0
    Connect-Ring -SourceIndex $Indices.RingThickness -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.RingMask -TargetInputIndex 1

    Connect-Ring -SourceIndex $Indices.AuraTex -SourceOutputIndex 1 -TargetType "expression" -TargetIndex $Indices.StylizedMul -TargetInputIndex 0
    Connect-Ring -SourceIndex $Indices.RingMask -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.StylizedMul -TargetInputIndex 1
    Connect-Ring -SourceIndex $Indices.StylizedMul -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.PosterizeMul -TargetInputIndex 0
    Connect-Ring -SourceIndex $Indices.Layers -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.PosterizeMul -TargetInputIndex 1
    Connect-Ring -SourceIndex $Indices.PosterizeMul -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.Floor -TargetInputIndex 0
    Connect-Ring -SourceIndex $Indices.Floor -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.Divide -TargetInputIndex 0
    Connect-Ring -SourceIndex $Indices.Layers -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.Divide -TargetInputIndex 1
    Connect-Ring -SourceIndex $Indices.Divide -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.Boost -TargetInputIndex 0
    Connect-Ring -SourceIndex $Indices.IntensityMul -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.Boost -TargetInputIndex 1
    Connect-Ring -SourceIndex $Indices.Boost -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.Saturate -TargetInputIndex 0

    Connect-Ring -SourceIndex $Indices.DarkColor -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.BaseLerp -TargetInputIndex 0
    Connect-Ring -SourceIndex $Indices.MidColor -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.BaseLerp -TargetInputIndex 1
    Connect-Ring -SourceIndex $Indices.Saturate -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.BaseLerp -TargetInputIndex 2

    Connect-Ring -SourceIndex $Indices.Saturate -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.HotPow -TargetInputIndex 0
    Connect-Ring -SourceIndex $Indices.HotPower -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.HotPow -TargetInputIndex 1
    Connect-Ring -SourceIndex $Indices.HotColor -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.HotMul -TargetInputIndex 0
    Connect-Ring -SourceIndex $Indices.HotPow -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.HotMul -TargetInputIndex 1

    Connect-Ring -SourceIndex $Indices.BaseLerp -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.FinalAdd -TargetInputIndex 0
    Connect-Ring -SourceIndex $Indices.HotMul -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.FinalAdd -TargetInputIndex 1
    Connect-Ring -SourceIndex $Indices.FinalAdd -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.EmissiveMul -TargetInputIndex 0
    Connect-Ring -SourceIndex $Indices.Saturate -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.EmissiveMul -TargetInputIndex 1
    Connect-Ring -SourceIndex $Indices.Saturate -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.OpacityMul -TargetInputIndex 0
    Connect-Ring -SourceIndex $Indices.OpacityIntensity -SourceOutputIndex 0 -TargetType "expression" -TargetIndex $Indices.OpacityMul -TargetInputIndex 1

    Connect-Ring -SourceIndex $Indices.EmissiveMul -SourceOutputIndex 0 -TargetType "material_pin" -TargetPin "EmissiveColor"
    Connect-Ring -SourceIndex $Indices.OpacityMul -SourceOutputIndex 0 -TargetType "material_pin" -TargetPin "Opacity"

    $ParameterIndices = @(
        $Indices.TX, $Indices.TY, $Indices.RingThickness, $Indices.Layers,
        $Indices.IntensityMul, $Indices.OpacityIntensity, $Indices.HotPower,
        $Indices.DarkColor, $Indices.MidColor, $Indices.HotColor, $Indices.AuraTex
    )
    $LogicIndices = @(
        $Indices.TexCoord, $Indices.AppendXY, $Indices.RadialUV, $Indices.RingMask,
        $Indices.StylizedMul, $Indices.PosterizeMul, $Indices.Floor, $Indices.Divide,
        $Indices.Boost, $Indices.Saturate, $Indices.BaseLerp, $Indices.HotPow,
        $Indices.HotMul, $Indices.FinalAdd, $Indices.EmissiveMul, $Indices.OpacityMul
    )

    $Summary.steps.parameters_comment = Invoke-RingTool -ToolName "create_material_comment" -InputObject @{
        material_path = $MaterialObjectPath
        comment_text = "Stylized Ring Parameters"
        editor_x = "-2680"
        editor_y = "120"
        width = "1200"
        height = "2200"
        color_rgba = @(0.15, 0.35, 0.75, 1.0)
        apply_changes = $true
    }
    $Summary.steps.logic_comment = Invoke-RingTool -ToolName "create_material_comment" -InputObject @{
        material_path = $MaterialObjectPath
        comment_text = "AI Stylized Ring Logic"
        editor_x = "-2280"
        editor_y = "80"
        width = "3600"
        height = "1100"
        color_rgba = @(0.95, 0.90, 0.35, 1.0)
        apply_changes = $true
    }

    $Summary.steps.parameters_wrap = Invoke-RingTool -ToolName "update_material_comment" -InputObject @{
        material_path = $MaterialObjectPath
        comment_text = "Stylized Ring Parameters"
        wrap_expression_indices = $ParameterIndices
        padding = "120"
        apply_changes = $true
    }
    $Summary.steps.logic_wrap = Invoke-RingTool -ToolName "update_material_comment" -InputObject @{
        material_path = $MaterialObjectPath
        comment_text = "AI Stylized Ring Logic"
        wrap_expression_indices = $LogicIndices
        padding = "160"
        apply_changes = $true
    }

    $Summary.steps.layout = @{
        success = $true
        action = "layout_material_expressions"
        skipped = $true
        reason = "manual_lane_layout_preserved"
    }

    $Summary.steps.validate_logic = Invoke-RingTool -ToolName "validate_material_graph_block" -InputObject @{
        material_path = $MaterialObjectPath
        comment_text = "AI Stylized Ring Logic"
        required_root_pins = @("EmissiveColor", "Opacity")
        require_comment = $true
    }
    $Summary.steps.wiring_summary = Invoke-RingTool -ToolName "summarize_material_block_wiring" -InputObject @{
        material_path = $MaterialObjectPath
        comment_text = "AI Stylized Ring Logic"
    }
    $Summary.steps.simplification_review = Invoke-RingTool -ToolName "review_material_graph_simplification" -InputObject @{
        material_path = $MaterialObjectPath
        comment_text = "AI Stylized Ring Logic"
    }
    $Summary.steps.material_review = Invoke-RingTool -ToolName "review_material_asset" -InputObject @{
        material_path = $MaterialObjectPath
        creative_mode = "stylized_ring"
    }
    $Summary.steps.material_info = Invoke-RingTool -ToolName "get_material_info" -InputObject @{
        material_path = $MaterialObjectPath
    }

    $Summary.captures.graph = Invoke-RingTool -ToolName "capture_material_graph_screenshot" -InputObject @{
        material_path = $MaterialObjectPath
        comment_text = "AI Stylized Ring Logic"
        output_dir = $OutputDir
        file_name = "stylized_ring_graph.png"
        image_width = "2200"
        image_height = "1200"
        padding = "220"
    }

    $Variants = @(
        @{
            name = "Default"
            scalar = @{
                TX = 2.0
                TY = 2.0
                RingThickness = 2.2
                Layers = 4.0
                IntensityMul = 1.35
                OpacityIntensity = 1.0
                HotPower = 6.0
            }
        },
        @{
            name = "Sharper"
            scalar = @{
                TX = 2.4
                TY = 2.0
                RingThickness = 2.8
                Layers = 5.0
                IntensityMul = 1.55
                OpacityIntensity = 1.08
                HotPower = 7.0
            }
        },
        @{
            name = "Broader"
            scalar = @{
                TX = 1.6
                TY = 1.4
                RingThickness = 1.7
                Layers = 4.0
                IntensityMul = 1.2
                OpacityIntensity = 0.92
                HotPower = 5.5
            }
        }
    )

    $Summary.steps.spawn_probe_actor = Invoke-RingTool -ToolName "spawn_material_probe_actor" -InputObject @{
        actor_label = $ProbeActorLabel
        mesh_path = "/Engine/BasicShapes/Plane.Plane"
        location = @(1200, 0, 240)
        rotation = @(-90, 0, 0)
        scale = @(5.0, 5.0, 5.0)
    }

    foreach ($Variant in $Variants) {
        $VariantName = [string]$Variant.name
        $InstanceName = "MI_StylizedRing_${VariantName}_$Stamp"
        $InstanceResult = Invoke-RingTool -ToolName "create_material_instance" -InputObject @{
            instance_name = $InstanceName
            parent_material_path = $MaterialObjectPath
            save_path = $SavePath
            scalar_overrides = (Json-ObjectString $Variant.scalar)
            texture_overrides = ('{"AuraTex":"' + $AuraTexturePath + '"}')
        }

        $ApplyResult = Invoke-RingTool -ToolName "apply_material_to_probe_actor" -InputObject @{
            actor_label = $ProbeActorLabel
            material_path = $InstanceResult.asset_path
            material_index = "0"
        }

        $FrameResult = Invoke-RingTool -ToolName "frame_probe_actor_for_capture" -InputObject @{
            actor_label = $ProbeActorLabel
        }
        Start-Sleep -Milliseconds 700

        $ViewportPath = Join-Path $OutputDir ("ring_{0}_viewport.png" -f $VariantName)
        & $WindowCaptureHelper -OutputPath $ViewportPath -SettleDelayMs 400 | Out-Null

        $Summary.variants += [ordered]@{
            name = $VariantName
            instance = $InstanceResult
            apply = $ApplyResult
            frame = $FrameResult
            viewport_capture = $ViewportPath
        }
    }

    $Summary.success = $true
}
finally {
    try {
        $Summary.cleanup = Invoke-RingTool -ToolName "destroy_actor_by_label" -InputObject @{
            actor_label = $ProbeActorLabel
            apply_changes = $true
        }
    }
    catch {
    }

    $SummaryPath = Join-Path $OutputDir "summary.json"
    $Summary | ConvertTo-Json -Depth 100 | Set-Content -Path $SummaryPath -Encoding UTF8
    Write-Host "Stylized ring material summary written to: $SummaryPath"
}


