param(
    [string]$BridgeHost = "127.0.0.1",
    [int]$BridgePort = 0
)

Set-StrictMode -Version Latest

if (-not (Test-Path "$PSScriptRoot\invoke_bridge.ps1")) {
    throw "Required bridge helper was not found: $PSScriptRoot\invoke_bridge.ps1"
}

. "$PSScriptRoot\invoke_bridge.ps1"

Wait-ForgePilotBridge -BridgeHost $BridgeHost -BridgePort $BridgePort -TimeoutSeconds 20

$MaterialName = "M_IceShader_Test"
$SavePath = "/Game/ForgePilot_Test/Materials/Ice"
$matPath = "$SavePath/$MaterialName.$MaterialName"

# ============================================================
# Cleanup previous material
# ============================================================
Write-Host "`n=== Cleanup ===" -ForegroundColor Yellow
try {
    Invoke-ForgePilotTool -BridgeHost $BridgeHost -BridgePort $BridgePort -ToolName "delete_asset" -InputObject @{
        asset_path = $matPath
        apply_changes = $true
    }
    Write-Host "  Deleted old material" -ForegroundColor Gray
} catch {
    Write-Host "  No old material to delete" -ForegroundColor Gray
}

# ============================================================
# Find texture
# ============================================================
Write-Host "`n=== Texture Search ===" -ForegroundColor Yellow
$texturePath = ""
foreach ($kw in @("ice", "frost", "snow", "noise", "cloud", "grunge")) {
    if ($texturePath) { break }
    try {
        $r = Invoke-ForgePilotTool -BridgeHost $BridgeHost -BridgePort $BridgePort -ToolName "search_assets" -InputObject @{
            query = $kw
            class_names = "Texture2D"
            max_results = 5
        }
        if ($r.assets -and @($r.assets).Count -gt 0) {
            $texturePath = [string]@($r.assets)[0].path
            Write-Host "  Found ($kw): $texturePath" -ForegroundColor Green
        }
    } catch { }
}
$useNoise = [string]::IsNullOrEmpty($texturePath)
if ($useNoise) { Write-Host "  No texture, using Noise node" -ForegroundColor Yellow }

# ============================================================
# Create material
# ============================================================
Write-Host "`n=== Create Material ===" -ForegroundColor Yellow
Invoke-ForgePilotTool -BridgeHost $BridgeHost -BridgePort $BridgePort -ToolName "create_material" -InputObject @{
    material_name = $MaterialName
    save_path = $SavePath
    base_color = @(0.0, 0.0, 0.0)
    apply_changes = $true
}
Write-Host "  Created $matPath" -ForegroundColor Green

# ============================================================
# Add nodes
# ============================================================
Write-Host "`n=== Add Nodes ===" -ForegroundColor Yellow

# [1] Fresnel
Write-Host "  [1] Fresnel" -ForegroundColor Cyan
Invoke-ForgePilotTool -BridgeHost $BridgeHost -BridgePort $BridgePort -ToolName "add_material_expression" -InputObject @{
    material_path = $matPath; expression_type = "Fresnel"
    editor_x = -700; editor_y = -100; apply_changes = $true
}

# [2] Constant: Fresnel Exp = 2.0
Write-Host "  [2] Constant(2.0) FresnelExp" -ForegroundColor Cyan
Invoke-ForgePilotTool -BridgeHost $BridgeHost -BridgePort $BridgePort -ToolName "add_material_expression" -InputObject @{
    material_path = $matPath; expression_type = "Constant"; default_value = "2.0"
    editor_x = -900; editor_y = -120; apply_changes = $true
}

# [3] Power
Write-Host "  [3] Power" -ForegroundColor Cyan
Invoke-ForgePilotTool -BridgeHost $BridgeHost -BridgePort $BridgePort -ToolName "add_material_expression" -InputObject @{
    material_path = $matPath; expression_type = "Power"
    editor_x = -450; editor_y = -100; apply_changes = $true
}

# [4] Constant: Power Exp = 2.0
Write-Host "  [4] Constant(2.0) PowerExp" -ForegroundColor Cyan
Invoke-ForgePilotTool -BridgeHost $BridgeHost -BridgePort $BridgePort -ToolName "add_material_expression" -InputObject @{
    material_path = $matPath; expression_type = "Constant"; default_value = "2.0"
    editor_x = -650; editor_y = 0; apply_changes = $true
}

# [5] Lerp
Write-Host "  [5] Lerp" -ForegroundColor Cyan
Invoke-ForgePilotTool -BridgeHost $BridgeHost -BridgePort $BridgePort -ToolName "add_material_expression" -InputObject @{
    material_path = $matPath; expression_type = "Lerp"
    editor_x = -50; editor_y = 100; apply_changes = $true
}

# [6] Constant3: Blue tint
Write-Host "  [6] Constant3(0.376,0.668,1.0) BlueTint" -ForegroundColor Cyan
Invoke-ForgePilotTool -BridgeHost $BridgeHost -BridgePort $BridgePort -ToolName "add_material_expression" -InputObject @{
    material_path = $matPath; expression_type = "Constant3"; default_value = "0.376, 0.668, 1.0"
    editor_x = -700; editor_y = 350; apply_changes = $true
}

# [7] Multiply (edges)
Write-Host "  [7] Multiply (edges)" -ForegroundColor Cyan
Invoke-ForgePilotTool -BridgeHost $BridgeHost -BridgePort $BridgePort -ToolName "add_material_expression" -InputObject @{
    material_path = $matPath; expression_type = "Multiply"
    editor_x = -250; editor_y = 50; apply_changes = $true
}

# [8] Multiply (blue center)
Write-Host "  [8] Multiply (blue)" -ForegroundColor Cyan
Invoke-ForgePilotTool -BridgeHost $BridgeHost -BridgePort $BridgePort -ToolName "add_material_expression" -InputObject @{
    material_path = $matPath; expression_type = "Multiply"
    editor_x = -250; editor_y = 250; apply_changes = $true
}

# [9] Constant: Edge brightness = 5.0
Write-Host "  [9] Constant(5.0) EdgeBright" -ForegroundColor Cyan
Invoke-ForgePilotTool -BridgeHost $BridgeHost -BridgePort $BridgePort -ToolName "add_material_expression" -InputObject @{
    material_path = $matPath; expression_type = "Constant"; default_value = "5.0"
    editor_x = -500; editor_y = 80; apply_changes = $true
}

# [10] Texture or Noise
if ($useNoise) {
    Write-Host "  [10] Noise (texture substitute)" -ForegroundColor Cyan
    Invoke-ForgePilotTool -BridgeHost $BridgeHost -BridgePort $BridgePort -ToolName "add_material_expression" -InputObject @{
        material_path = $matPath; expression_type = "Noise"
        editor_x = -700; editor_y = 180; apply_changes = $true
    }
} else {
    Write-Host "  [10] TextureSampleParameter2D (IceTexture)" -ForegroundColor Cyan
    Invoke-ForgePilotTool -BridgeHost $BridgeHost -BridgePort $BridgePort -ToolName "add_material_expression" -InputObject @{
        material_path = $matPath; expression_type = "TextureSampleParameter2D"
        param_name = "IceTexture"; texture_path = $texturePath
        editor_x = -700; editor_y = 180; apply_changes = $true
    }
}

# Index map (by creation order):
# 0=BaseColor3, 1=Fresnel, 2=FresnelExp, 3=Power, 4=PowerExp,
# 5=Lerp, 6=BlueTint, 7=MulEdges, 8=MulBlue, 9=EdgeBright, 10=Texture

# ============================================================
# Wire graph
# ============================================================
Write-Host "`n=== Wire Graph ===" -ForegroundColor Yellow

# FresnelExp(2) -> Fresnel.ExponentIn [input 0]
Write-Host "  FresnelExp -> Fresnel" -ForegroundColor Cyan
Invoke-ForgePilotTool -BridgeHost $BridgeHost -BridgePort $BridgePort -ToolName "connect_material_nodes" -InputObject @{
    material_path = $matPath; source_index = 2
    target_type = "expression"; target_index = 1; target_input_index = 0
    apply_changes = $true
}

# Fresnel -> Power.Base [input 0]
Write-Host "  Fresnel -> Power.Base" -ForegroundColor Cyan
Invoke-ForgePilotTool -BridgeHost $BridgeHost -BridgePort $BridgePort -ToolName "connect_material_nodes" -InputObject @{
    material_path = $matPath; source_index = 1
    target_type = "expression"; target_index = 3; target_input_index = 0
    apply_changes = $true
}

# PowerExp(2) -> Power.Exponent [input 1]
Write-Host "  PowerExp -> Power.Exp" -ForegroundColor Cyan
Invoke-ForgePilotTool -BridgeHost $BridgeHost -BridgePort $BridgePort -ToolName "connect_material_nodes" -InputObject @{
    material_path = $matPath; source_index = 4
    target_type = "expression"; target_index = 3; target_input_index = 1
    apply_changes = $true
}

# Tex -> MulEdges.A [input 0]
Write-Host "  Texture -> MulEdges.A" -ForegroundColor Cyan
Invoke-ForgePilotTool -BridgeHost $BridgeHost -BridgePort $BridgePort -ToolName "connect_material_nodes" -InputObject @{
    material_path = $matPath; source_index = 10
    target_type = "expression"; target_index = 7; target_input_index = 0
    apply_changes = $true
}

# EdgeBright(5) -> MulEdges.B [input 1]
Write-Host "  EdgeBright -> MulEdges.B" -ForegroundColor Cyan
Invoke-ForgePilotTool -BridgeHost $BridgeHost -BridgePort $BridgePort -ToolName "connect_material_nodes" -InputObject @{
    material_path = $matPath; source_index = 9
    target_type = "expression"; target_index = 7; target_input_index = 1
    apply_changes = $true
}

# Tex -> MulBlue.A [input 0]
Write-Host "  Texture -> MulBlue.A" -ForegroundColor Cyan
Invoke-ForgePilotTool -BridgeHost $BridgeHost -BridgePort $BridgePort -ToolName "connect_material_nodes" -InputObject @{
    material_path = $matPath; source_index = 10
    target_type = "expression"; target_index = 8; target_input_index = 0
    apply_changes = $true
}

# BlueTint -> MulBlue.B [input 1]
Write-Host "  BlueTint -> MulBlue.B" -ForegroundColor Cyan
Invoke-ForgePilotTool -BridgeHost $BridgeHost -BridgePort $BridgePort -ToolName "connect_material_nodes" -InputObject @{
    material_path = $matPath; source_index = 6
    target_type = "expression"; target_index = 8; target_input_index = 1
    apply_changes = $true
}

# MulBlue -> Lerp.A [input 0] (blue center)
Write-Host "  MulBlue -> Lerp.A" -ForegroundColor Cyan
Invoke-ForgePilotTool -BridgeHost $BridgeHost -BridgePort $BridgePort -ToolName "connect_material_nodes" -InputObject @{
    material_path = $matPath; source_index = 8
    target_type = "expression"; target_index = 5; target_input_index = 0
    apply_changes = $true
}

# MulEdges -> Lerp.B [input 1] (bright edges)
Write-Host "  MulEdges -> Lerp.B" -ForegroundColor Cyan
Invoke-ForgePilotTool -BridgeHost $BridgeHost -BridgePort $BridgePort -ToolName "connect_material_nodes" -InputObject @{
    material_path = $matPath; source_index = 7
    target_type = "expression"; target_index = 5; target_input_index = 1
    apply_changes = $true
}

# Power -> Lerp.Alpha [input 2]
Write-Host "  Power -> Lerp.Alpha" -ForegroundColor Cyan
Invoke-ForgePilotTool -BridgeHost $BridgeHost -BridgePort $BridgePort -ToolName "connect_material_nodes" -InputObject @{
    material_path = $matPath; source_index = 3
    target_type = "expression"; target_index = 5; target_input_index = 2
    apply_changes = $true
}

# ============================================================
# Material output
# ============================================================
Write-Host "`n=== Output Pins ===" -ForegroundColor Yellow

# Lerp -> EmissiveColor
Write-Host "  Lerp -> EmissiveColor" -ForegroundColor Cyan
Invoke-ForgePilotTool -BridgeHost $BridgeHost -BridgePort $BridgePort -ToolName "connect_material_nodes" -InputObject @{
    material_path = $matPath; source_index = 5
    target_type = "material_pin"; target_pin = "EmissiveColor"
    apply_changes = $true
}

# BaseColor3 (black) -> BaseColor
Write-Host "  Black -> BaseColor" -ForegroundColor Cyan
Invoke-ForgePilotTool -BridgeHost $BridgeHost -BridgePort $BridgePort -ToolName "connect_material_nodes" -InputObject @{
    material_path = $matPath; source_index = 0
    target_type = "material_pin"; target_pin = "BaseColor"
    apply_changes = $true
}

# ============================================================
# Spawn preview sphere
# ============================================================
Write-Host "`n=== Preview ===" -ForegroundColor Yellow
try {
    Invoke-ForgePilotTool -BridgeHost $BridgeHost -BridgePort $BridgePort -ToolName "destroy_actor_by_label" -InputObject @{
        actor_label = "ForgePilot_IceShaderPreview"
        apply_changes = $true
    }
} catch { }

Invoke-ForgePilotTool -BridgeHost $BridgeHost -BridgePort $BridgePort -ToolName "spawn_actor" -InputObject @{
    actor_class = "StaticMeshActor"
    location = @(0, 0, 200)
    actor_label = "ForgePilot_IceShaderPreview"
    apply_changes = $true
}
Write-Host "  Spawned preview sphere" -ForegroundColor Green

try {
    Invoke-ForgePilotTool -BridgeHost $BridgeHost -BridgePort $BridgePort -ToolName "set_actor_material" -InputObject @{
        actor_label = "ForgePilot_IceShaderPreview"
        material_path = $matPath
        material_index = 0
    }
    Write-Host "  Applied material to sphere" -ForegroundColor Green
} catch {
    Write-Host "  Could not apply material to sphere: $($_.Exception.Message)" -ForegroundColor Yellow
}

# ============================================================
Write-Host "`n=== ICE SHADER BUILD COMPLETE ===" -ForegroundColor Green
Write-Host "Material: $matPath" -ForegroundColor Green
Write-Host "Texture: $(if($useNoise){'Noise node'}else{$texturePath})" -ForegroundColor Green
Write-Host "Graph: Fresnel(2)->Power(2)->Lerp.Alpha" -ForegroundColor White
Write-Host "       Tex*Blue(0.37,0.67,1) -> Lerp.A (center)" -ForegroundColor White
Write-Host "       Tex*5.0 -> Lerp.B (bright edges)" -ForegroundColor White
Write-Host "       Lerp -> EmissiveColor" -ForegroundColor White
