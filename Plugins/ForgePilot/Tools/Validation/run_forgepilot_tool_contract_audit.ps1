Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

$ScriptRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
$PluginRoot = Split-Path -Parent (Split-Path -Parent $ScriptRoot)
$SourceRoot = Join-Path $PluginRoot 'Source\ForgePilot\Private'

function Add-Finding {
    param(
        [Parameter(Mandatory = $true)][string]$Check,
        [Parameter(Mandatory = $true)][string]$Detail
    )

    $script:Findings += [pscustomobject]@{
        check = $Check
        detail = $Detail
    }
}

function Get-FileText {
    param([Parameter(Mandatory = $true)][string]$Path)

    if (-not (Test-Path $Path)) {
        throw "Required source file not found: $Path"
    }

    return Get-Content $Path -Raw
}

$Findings = @()
$CheckedFiles = @()

$SceneRegistry = Join-Path $SourceRoot 'VFXToolRegistry_Scene.cpp'
$BlueprintRegistry = Join-Path $SourceRoot 'VFXToolRegistry_Blueprint.cpp'
$MaterialRegistry = Join-Path $SourceRoot 'VFXToolRegistry_Material.cpp'
$NiagaraRegistry = Join-Path $SourceRoot 'VFXToolRegistry_Niagara.cpp'

$SceneContent = Get-FileText -Path $SceneRegistry
$BlueprintContent = Get-FileText -Path $BlueprintRegistry
$MaterialContent = Get-FileText -Path $MaterialRegistry
$NiagaraContent = Get-FileText -Path $NiagaraRegistry
$CheckedFiles += @($SceneRegistry, $BlueprintRegistry, $MaterialRegistry, $NiagaraRegistry)

$AllRegistryContent = @($SceneContent, $BlueprintContent, $MaterialContent, $NiagaraContent) -join "`n"

$DefaultTrueApplyMatches = Select-String -Path @($SceneRegistry, $BlueprintRegistry, $MaterialRegistry, $NiagaraRegistry) -SimpleMatch 'ParseOptionalBoolField(Input, TEXT("apply_changes"), true)'
foreach ($Match in @($DefaultTrueApplyMatches)) {
    Add-Finding -Check 'parse_default_true' -Detail "$($Match.Path):$($Match.LineNumber): $($Match.Line.Trim())"
}

$SchemaDefaultTrueMatches = Select-String -Path @($SceneRegistry, $BlueprintRegistry, $MaterialRegistry, $NiagaraRegistry) -Pattern 'apply_changes.*Default:\s*true|apply_changes.*default:\s*true|apply_changes.*Default true|apply_changes.*default true'
foreach ($Match in @($SchemaDefaultTrueMatches)) {
    Add-Finding -Check 'schema_default_true' -Detail "$($Match.Path):$($Match.LineNumber): $($Match.Line.Trim())"
}

$RequiredMarkers = @(
    [pscustomobject]@{
        Tool = 'create_material'
        File = $MaterialRegistry
        Markers = @(
            'Def.Name = TEXT("create_material")',
            'Props->SetObjectField(TEXT("apply_changes")',
            'const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);',
            'MatResult->SetBoolField(TEXT("dry_run"), !bApplyChanges);',
            'MatResult->SetBoolField(TEXT("will_mutate_asset"), bApplyChanges);'
        )
    },
    [pscustomobject]@{
        Tool = 'compile_blueprint_with_feedback'
        File = $BlueprintRegistry
        Markers = @(
            'Def.Name = TEXT("compile_blueprint_with_feedback")',
            'Props->SetObjectField(TEXT("apply_changes")',
            'const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);',
            'Result->SetBoolField(TEXT("dry_run"), true);',
            'Result->SetBoolField(TEXT("will_mutate_asset"), true);'
        )
    },
    [pscustomobject]@{
        Tool = 'create_empty_niagara_system'
        File = $NiagaraRegistry
        Markers = @(
            'Def.Name = TEXT("create_empty_niagara_system")',
            'Props->SetObjectField(TEXT("apply_changes")',
            'const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);',
            'Result->SetBoolField(TEXT("dry_run"), !bApplyChanges);',
            'Result->SetBoolField(TEXT("will_mutate_asset"), bApplyChanges);'
        )
    },
    [pscustomobject]@{
        Tool = 'run_effect_workflow_v2'
        File = $SceneRegistry
        Markers = @(
            'Def.Name = TEXT("run_effect_workflow_v2")',
            'Props->SetObjectField(TEXT("apply_changes"), MakeStringProperty(TEXT("Optional bool string. Default: false.")));'
        )
    },
    [pscustomobject]@{
        Tool = 'run_suction_core_v2_workflow'
        File = $SceneRegistry
        Markers = @(
            'Def.Name = TEXT("run_suction_core_v2_workflow")',
            'Props->SetObjectField(TEXT("apply_changes"), MakeStringProperty(TEXT("Optional bool string. Default: false. When false, returns a bound execution plan without creating assets.")));',
            'const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);'
        )
    }
)

foreach ($Requirement in $RequiredMarkers) {
    $FileContent = switch ($Requirement.File) {
        $SceneRegistry { $SceneContent }
        $BlueprintRegistry { $BlueprintContent }
        $MaterialRegistry { $MaterialContent }
        $NiagaraRegistry { $NiagaraContent }
        default { Get-FileText -Path $Requirement.File }
    }

    foreach ($Marker in $Requirement.Markers) {
        if ($FileContent -notmatch [regex]::Escape($Marker)) {
            Add-Finding -Check 'missing_marker' -Detail "$($Requirement.Tool): missing marker '$Marker' in $($Requirement.File)"
        }
    }
}

$Summary = [ordered]@{
    success = ($Findings.Count -eq 0)
    checked_files = $CheckedFiles
    finding_count = $Findings.Count
    findings = $Findings
}

$Summary | ConvertTo-Json -Depth 10

if (-not $Summary.success) {
    exit 1
}
