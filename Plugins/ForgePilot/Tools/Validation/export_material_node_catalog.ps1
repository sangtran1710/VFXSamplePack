[CmdletBinding()]
param(
    [string]$UERoot = "",
    [string]$OutputPath = "Knowledge/Materials/Reference/material_node_catalog_current.json",
    [switch]$IncludeProjectMaterialFunctions
)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

. (Join-Path $PSScriptRoot 'resolve_engine_context.ps1')

function ConvertTo-ForwardSlash {
    param([Parameter(Mandatory = $true)][string]$Value)
    return ($Value -replace '\\', '/')
}

function Get-RelativePathCompat {
    param(
        [Parameter(Mandatory = $true)][string]$BasePath,
        [Parameter(Mandatory = $true)][string]$TargetPath
    )

    $baseFull = [System.IO.Path]::GetFullPath($BasePath)
    $targetFull = [System.IO.Path]::GetFullPath($TargetPath)
    if (-not $baseFull.EndsWith([System.IO.Path]::DirectorySeparatorChar)) {
        $baseFull += [System.IO.Path]::DirectorySeparatorChar
    }

    $baseUri = [System.Uri]::new($baseFull)
    $targetUri = [System.Uri]::new($targetFull)
    $relativeUri = $baseUri.MakeRelativeUri($targetUri)
    return [System.Uri]::UnescapeDataString($relativeUri.ToString()) -replace '/', [System.IO.Path]::DirectorySeparatorChar
}

function Convert-UAssetPathToLongPackagePath {
    param(
        [Parameter(Mandatory = $true)][string]$RootPath,
        [Parameter(Mandatory = $true)][string]$MountPoint,
        [Parameter(Mandatory = $true)][string]$AssetFile
    )

    $root = [System.IO.Path]::GetFullPath($RootPath)
    $file = [System.IO.Path]::GetFullPath($AssetFile)
    $relative = Get-RelativePathCompat -BasePath $root -TargetPath $file
    $relativeNoExt = $relative -replace '\.uasset$', ''
    $packagePath = ConvertTo-ForwardSlash (Join-Path $MountPoint $relativeNoExt)
    $assetName = [System.IO.Path]::GetFileNameWithoutExtension($file)
    return "$packagePath.$assetName"
}

function Get-ExpressionCategory {
    param([Parameter(Mandatory = $true)][string]$NodeName)

    if ($NodeName -match '^(Add|Subtract|Multiply|Divide|Power|SquareRoot|Abs|Frac|Fmod|Floor|Ceil|Round|Sign|Min|Max|Clamp|Saturate|OneMinus|Step|SmoothStep|If|LinearInterpolate|InverseLinearInterpolate|Distance|Length)$') {
        return 'math'
    }
    if ($NodeName -match '(Sine|Cosine|Tangent|Arctangent|Arcsine|Arccosine)') {
        return 'trigonometry'
    }
    if ($NodeName -match '(DotProduct|CrossProduct|Normalize|Transform|WorldPosition|ObjectPosition|ObjectRadius|PixelDepth|SceneDepth|CameraVector|VertexNormal|ReflectionVector|PreSkinned|DistanceToNearestSurface|DistanceField)') {
        return 'space_vector_depth'
    }
    if ($NodeName -match '(Texture|TexCoord|Panner|Rotator|ParticleSubUV|Flipbook|SpeedTree|FontSample|SceneColor|SceneTexture)') {
        return 'texture_uv_sampling'
    }
    if ($NodeName -match '(StaticSwitch|FeatureLevel|QualitySwitch|ShadingPathSwitch|Switch|Bool)') {
        return 'switches_optimization'
    }
    if ($NodeName -match '(Particle|DynamicParameter|ActorPosition|Object|PerInstance|CollectionParameter|Parameter|MaterialAttribute)') {
        return 'data_parameters_attributes'
    }
    if ($NodeName -match '(Fresnel|Noise|Desaturation|BlackBody|DepthFade|Atmospheric|BentNormal|BumpOffset|GIReplace|Custom|Function|Comment)') {
        return 'utility_vfx_common'
    }
    return 'other'
}

function Get-MaterialExpressionNodes {
    param([Parameter(Mandatory = $true)][string]$EngineRoot)

    $materialsHeaderRoot = Join-Path $EngineRoot 'Engine/Source/Runtime/Engine/Classes/Materials'
    if (-not (Test-Path $materialsHeaderRoot)) {
        throw "Material header folder not found: $materialsHeaderRoot"
    }

    $nodes = @()
    $headers = Get-ChildItem -Path $materialsHeaderRoot -Filter 'MaterialExpression*.h' -File | Sort-Object Name
    foreach ($header in $headers) {
        $text = Get-Content -Raw -Path $header.FullName
        $matches = [regex]::Matches($text, 'class\s+(?:ENGINE_API\s+|MinimalAPI\s+)?(UMaterialExpression[A-Za-z0-9_]+)\s*:\s*public\s+([A-Za-z0-9_]+)')
        foreach ($match in $matches) {
            $className = [string]$match.Groups[1].Value
            $baseClass = [string]$match.Groups[2].Value
            $nodeName = $className -replace '^UMaterialExpression', ''
            if ([string]::IsNullOrWhiteSpace($nodeName)) {
                continue
            }
            $nodes += [pscustomobject][ordered]@{
                node_name = $nodeName
                class_name = $className
                base_class = $baseClass
                category_hint = Get-ExpressionCategory -NodeName $nodeName
                header = ConvertTo-ForwardSlash $header.FullName
            }
        }
    }

    return @($nodes | Sort-Object node_name -Unique)
}

function Get-EngineMaterialFunctions {
    param([Parameter(Mandatory = $true)][string]$EngineRoot)

    $functionsRoot = Join-Path $EngineRoot 'Engine/Content/Functions'
    if (-not (Test-Path $functionsRoot)) {
        throw "Engine material functions folder not found: $functionsRoot"
    }

    $functions = @()
    $assets = Get-ChildItem -Path $functionsRoot -Filter '*.uasset' -File -Recurse | Sort-Object FullName
    foreach ($asset in $assets) {
        $relativeFolder = Get-RelativePathCompat -BasePath $functionsRoot -TargetPath $asset.DirectoryName
        $category = ConvertTo-ForwardSlash $relativeFolder
        if ([string]::IsNullOrWhiteSpace($category) -or $category -eq '.') {
            $category = 'root'
        }

        $functions += [pscustomobject][ordered]@{
            function_name = $asset.BaseName
            package_path = Convert-UAssetPathToLongPackagePath -RootPath (Join-Path $EngineRoot 'Engine/Content') -MountPoint '/Engine' -AssetFile $asset.FullName
            category_hint = $category
        }
    }

    return $functions
}

function Get-ProjectMaterialFunctions {
    $projectContentRoot = Resolve-Path (Join-Path $PSScriptRoot '..\..\..\..\Content')
    $assets = Get-ChildItem -Path $projectContentRoot.Path -Filter '*.uasset' -File -Recurse |
        Where-Object { $_.BaseName -like 'MF_*' -or $_.DirectoryName -match 'MaterialFunction|MaterialFunctions|Functions' } |
        Sort-Object FullName

    $functions = @()
    foreach ($asset in $assets) {
        $functions += [pscustomobject][ordered]@{
            function_name = $asset.BaseName
            package_path = Convert-UAssetPathToLongPackagePath -RootPath $projectContentRoot.Path -MountPoint '/Game' -AssetFile $asset.FullName
            category_hint = 'project_content'
        }
    }

    return $functions
}

$resolvedUERoot = Get-ForgePilotInstalledUERoot -PreferredRoot $UERoot
$engineInfo = Get-ForgePilotEngineVersionTag -UERoot $resolvedUERoot
$resolvedOutput = if ([System.IO.Path]::IsPathRooted($OutputPath)) {
    $OutputPath
}
else {
    Join-Path (Get-Location).Path $OutputPath
}

$outputDir = Split-Path -Parent $resolvedOutput
New-Item -ItemType Directory -Force -Path $outputDir | Out-Null

$expressionNodes = @(Get-MaterialExpressionNodes -EngineRoot $resolvedUERoot)
$engineFunctions = @(Get-EngineMaterialFunctions -EngineRoot $resolvedUERoot)
$projectFunctions = if ($IncludeProjectMaterialFunctions) { @(Get-ProjectMaterialFunctions) } else { @() }
$projectFunctions = @($projectFunctions)

$expressionCategoryCounts = @{}
foreach ($group in ($expressionNodes | Group-Object category_hint)) {
    $expressionCategoryCounts[$group.Name] = $group.Count
}

$functionCategoryCounts = @{}
foreach ($group in ($engineFunctions | Group-Object category_hint)) {
    $functionCategoryCounts[$group.Name] = $group.Count
}

$catalog = [ordered]@{
    generated_at = (Get-Date).ToString('o')
    engine = $engineInfo.Label
    engine_slug = $engineInfo.Slug
    ue_root = ConvertTo-ForwardSlash $resolvedUERoot
    purpose = 'Broad material node catalog for ForgePilot. Use this as a discovery index, not as permission to use every node in one graph.'
    usage_rules = @(
        'Start from curated VFX taxonomies and recipes first.',
        'Use this catalog to discover candidate native MaterialExpression nodes and engine MaterialFunctions.',
        'Before relying on a node in production automation, add it to the material node probe or verify it through the bridge.',
        'Prefer small readable graphs and reusable MaterialFunctions over huge one-off node chains.'
    )
    native_material_expression_count = $expressionNodes.Count
    engine_material_function_count = $engineFunctions.Count
    project_material_function_count = $projectFunctions.Count
    expression_category_counts = $expressionCategoryCounts
    function_category_counts = $functionCategoryCounts
    native_material_expressions = $expressionNodes
    engine_material_functions = $engineFunctions
    project_material_functions = $projectFunctions
    supporting_knowledge = @(
        'Knowledge/Materials/Techniques/vfx_artist_essential_node_taxonomy.json',
        'Knowledge/Materials/Techniques/vfx_artist_advanced_node_taxonomy.json',
        'Knowledge/Materials/Techniques/vfx_artist_material_math_taxonomy.json'
    )
    validation_script = 'Tools/Validation/run_material_node_probe.ps1'
}

$catalog | ConvertTo-Json -Depth 100 | Set-Content -Path $resolvedOutput -Encoding UTF8

Write-Host "Material node catalog written to $resolvedOutput"
Write-Host "Engine: $($engineInfo.Label)"
Write-Host ("Native MaterialExpression nodes: {0}" -f $expressionNodes.Count)
Write-Host ("Engine MaterialFunctions: {0}" -f $engineFunctions.Count)
Write-Host ("Project MaterialFunctions: {0}" -f $projectFunctions.Count)
