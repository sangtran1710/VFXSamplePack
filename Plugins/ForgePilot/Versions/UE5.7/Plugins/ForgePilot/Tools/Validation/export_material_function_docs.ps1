[CmdletBinding()]
param(
    [string]$CatalogPath = "Knowledge/Materials/Reference/material_node_catalog_current.json",
    [string]$OutputRoot = "Docs/MaterialFunctions/Current",
    [string]$KnowledgeOutputPath = "Knowledge/Materials/Reference/material_function_study_index_current.json",
    [switch]$IncludeFunctionInfo,
    [switch]$IncludeInternalGraph,
    [int]$MaxFunctions = 0,
    [int]$TimeoutSeconds = 60
)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

function ConvertTo-ForwardSlash {
    param([Parameter(Mandatory = $true)][string]$Value)
    return ($Value -replace '\\', '/')
}

function Resolve-RepoPath {
    param([Parameter(Mandatory = $true)][string]$Path)

    if ([System.IO.Path]::IsPathRooted($Path)) {
        return $Path
    }

    return Join-Path (Get-Location).Path $Path
}

function ConvertTo-SafeFileName {
    param([Parameter(Mandatory = $true)][string]$Value)

    $safe = $Value -replace '[\\/:*?"<>|]', '_'
    $safe = $safe -replace '\s+', '_'
    $safe = $safe.Trim('_')
    if ([string]::IsNullOrWhiteSpace($safe)) {
        return 'root'
    }

    return $safe
}

function Get-FunctionUsageHint {
    param(
        [Parameter(Mandatory = $true)][string]$Category,
        [Parameter(Mandatory = $true)][string]$Name
    )

    $text = "$Category/$Name"

    if ($text -match 'Blend|AlphaBlend') { return 'Color and alpha compositing. Prefer these when a named blend mode communicates intent better than hand-built math.' }
    if ($text -match 'FlipBook|SubUV|Particle') { return 'Particle and flipbook support. Useful for sprites, flipbooks, particle attributes, and motion-vector flipbook workflows.' }
    if ($text -match 'Textur|UV|Coordinate|LongLat|WorldAligned|BoundingBox') { return 'UV, coordinate, and texture sampling helper. Useful when coordinate logic would clutter the parent material.' }
    if ($text -match 'Opacity|Depth|CameraDepth') { return 'Opacity/depth integration. Useful for soft intersections, camera fades, translucent particles, and environment blending.' }
    if ($text -match 'ImageAdjustment|Contrast|Desatur|Hue|Color|Gradient') { return 'Image and color adjustment. Useful for mask shaping, tint control, packed texture cleanup, and palette operations.' }
    if ($text -match 'WorldPositionOffset|WPO|PivotPainter|ComplexMotion|SpeedTree') { return 'Vertex animation or WPO helper. Useful for mesh motion, pivot painter, foliage, and procedural deformation.' }
    if ($text -match 'DistanceField') { return 'Signed distance field helper. Useful for procedural shapes, SDF composition, and distance-field-driven effects.' }
    if ($text -match 'Procedural|Noise|Voronoi|Generated|Pattern') { return 'Procedural pattern helper. Useful for generated masks, noise, breakup, and textureless VFX looks.' }
    if ($text -match 'Math|Vector|Utility|MakeFloat|BreakOut') { return 'Math/vector utility. Useful as a compact helper for common reusable calculations and packed channel workflows.' }
    if ($text -match 'MaterialLayer|Strata|Shading') { return 'Surface shading or material-layer helper. Use carefully in VFX because these are often heavier or tied to specific material domains.' }
    if ($text -match 'Debug') { return 'Debug/visualization helper. Use for inspection, not final production materials unless explicitly required.' }
    if ($text -match 'ExampleContent|NotInUse|MAXScripts|SupportFiles') { return 'Example/support content. Treat as reference first; validate before production use.' }

    return 'General material function. Inspect pins and internal nodes before using in production automation.'
}

function Get-FunctionRiskHint {
    param(
        [Parameter(Mandatory = $true)][string]$Category,
        [Parameter(Mandatory = $true)][string]$Name
    )

    $text = "$Category/$Name"

    if ($text -match 'NotInUse') { return 'Avoid by default; category indicates retired or unsupported content.' }
    if ($text -match 'ExampleContent|MAXScripts|SupportFiles') { return 'Reference-only by default; verify visual output and packaging before production use.' }
    if ($text -match 'MaterialLayer|Strata') { return 'May require specific material setup or UE shading model support; do not use blindly for particle materials.' }
    if ($text -match 'WorldAligned|TextureVariation|Procedural|DistanceField') { return 'Potentially expensive depending on texture samples or procedural math; keep usage targeted.' }
    if ($text -match 'FlipBook_MotionVectors|SubUV_Function_MipDerivative') { return 'Check texture inputs and motion-vector conventions before wiring; wrong pins can compile but look incorrect.' }
    if ($text -match 'Blend|Hue|Remap|Contrast') { return 'Wire by target_input_name; wrong pin order can still compile with wrong art direction.' }

    return 'Use get_material_function_info and wire by target_input_name for safer automation.'
}

function Get-FunctionInfoSummary {
    param(
        [Parameter(Mandatory = $true)]$InfoResult,
        [bool]$IncludeInternalGraph = $false
    )

    if ($null -eq $InfoResult) {
        return $null
    }

    $inputs = @()
    if ($InfoResult.PSObject.Properties.Name -contains 'function_inputs') {
        foreach ($input in @($InfoResult.function_inputs)) {
            $inputs += [pscustomobject][ordered]@{
                name = [string]$input.name
                type = if ($input.PSObject.Properties.Name -contains 'input_type') { [string]$input.input_type } else { '' }
                description = if ($input.PSObject.Properties.Name -contains 'description') { [string]$input.description } else { '' }
            }
        }
    }

    $outputs = @()
    if ($InfoResult.PSObject.Properties.Name -contains 'function_outputs') {
        foreach ($output in @($InfoResult.function_outputs)) {
            $outputs += [pscustomobject][ordered]@{
                name = [string]$output.name
                description = if ($output.PSObject.Properties.Name -contains 'description') { [string]$output.description } else { '' }
            }
        }
    }

    $summary = [ordered]@{
        description = if ($InfoResult.PSObject.Properties.Name -contains 'description') { [string]$InfoResult.description } else { '' }
        input_count = if ($InfoResult.PSObject.Properties.Name -contains 'input_count') { [int]$InfoResult.input_count } else { $inputs.Count }
        output_count = if ($InfoResult.PSObject.Properties.Name -contains 'output_count') { [int]$InfoResult.output_count } else { $outputs.Count }
        node_count = if ($InfoResult.PSObject.Properties.Name -contains 'node_count') { [int]$InfoResult.node_count } else { 0 }
        dependent_function_count = if ($InfoResult.PSObject.Properties.Name -contains 'dependent_function_count') { [int]$InfoResult.dependent_function_count } else { 0 }
        inputs = $inputs
        outputs = $outputs
    }

    if ($IncludeInternalGraph) {
        $nodes = @()
        if ($InfoResult.PSObject.Properties.Name -contains 'nodes') {
            foreach ($node in @($InfoResult.nodes)) {
                $nodeInputs = @()
                if ($node.PSObject.Properties.Name -contains 'inputs') {
                    foreach ($input in @($node.inputs)) {
                        $nodeInputs += [pscustomobject][ordered]@{
                            input_name = if ($input.PSObject.Properties.Name -contains 'input_name') { [string]$input.input_name } else { '' }
                            property_name = if ($input.PSObject.Properties.Name -contains 'property_name') { [string]$input.property_name } else { '' }
                            connected = if ($input.PSObject.Properties.Name -contains 'connected') { [bool]$input.connected } else { $false }
                            source_index = if ($input.PSObject.Properties.Name -contains 'source_index' -and $null -ne $input.source_index) { [int]$input.source_index } else { $null }
                            source_caption = if ($input.PSObject.Properties.Name -contains 'source_caption') { [string]$input.source_caption } else { '' }
                            source_class = if ($input.PSObject.Properties.Name -contains 'source_class') { [string]$input.source_class } else { '' }
                            output_index = if ($input.PSObject.Properties.Name -contains 'output_index' -and $null -ne $input.output_index) { [int]$input.output_index } else { 0 }
                            mask_label = if ($input.PSObject.Properties.Name -contains 'mask_label') { [string]$input.mask_label } else { '' }
                        }
                    }
                }

                $nodes += [pscustomobject][ordered]@{
                    index = if ($node.PSObject.Properties.Name -contains 'index') { [int]$node.index } else { -1 }
                    class = if ($node.PSObject.Properties.Name -contains 'class') { [string]$node.class } else { '' }
                    caption = if ($node.PSObject.Properties.Name -contains 'caption') { [string]$node.caption } else { '' }
                    node_kind = if ($node.PSObject.Properties.Name -contains 'node_kind') { [string]$node.node_kind } else { '' }
                    desc = if ($node.PSObject.Properties.Name -contains 'desc') { [string]$node.desc } else { '' }
                    input_name = if ($node.PSObject.Properties.Name -contains 'input_name') { [string]$node.input_name } else { '' }
                    input_type = if ($node.PSObject.Properties.Name -contains 'input_type') { [string]$node.input_type } else { '' }
                    output_name = if ($node.PSObject.Properties.Name -contains 'output_name') { [string]$node.output_name } else { '' }
                    description = if ($node.PSObject.Properties.Name -contains 'description') { [string]$node.description } else { '' }
                    inputs = $nodeInputs
                }
            }
        }

        $connections = @()
        if ($InfoResult.PSObject.Properties.Name -contains 'connections') {
            foreach ($connection in @($InfoResult.connections)) {
                $connections += [pscustomobject][ordered]@{
                    source_index = if ($connection.PSObject.Properties.Name -contains 'source_index' -and $null -ne $connection.source_index) { [int]$connection.source_index } else { $null }
                    source_caption = if ($connection.PSObject.Properties.Name -contains 'source_caption') { [string]$connection.source_caption } else { '' }
                    target_caption = if ($connection.PSObject.Properties.Name -contains 'target_caption') { [string]$connection.target_caption } else { '' }
                    target_input = if ($connection.PSObject.Properties.Name -contains 'target_input') { [string]$connection.target_input } else { '' }
                    property_name = if ($connection.PSObject.Properties.Name -contains 'property_name') { [string]$connection.property_name } else { '' }
                    source_output_index = if ($connection.PSObject.Properties.Name -contains 'source_output_index' -and $null -ne $connection.source_output_index) { [int]$connection.source_output_index } else { 0 }
                }
            }
        }

        $comments = @()
        if ($InfoResult.PSObject.Properties.Name -contains 'comments') {
            foreach ($comment in @($InfoResult.comments)) {
                $comments += [pscustomobject][ordered]@{
                    text = if ($comment.PSObject.Properties.Name -contains 'text') { [string]$comment.text } else { '' }
                }
            }
        }

        $summary.nodes = $nodes
        $summary.connections = $connections
        $summary.comments = $comments
    }

    return [pscustomobject]$summary
}

function Write-FunctionDeepDoc {
    param(
        [Parameter(Mandatory = $true)]$Item,
        [Parameter(Mandatory = $true)][string]$OutputPath
    )

    $lines = @()
    $lines += "# $($Item.function_name)"
    $lines += ""
    $lines += "- Path: ``$($Item.package_path)``"
    $lines += "- Category: $($Item.category_hint)"
    $lines += "- Use: $($Item.usage_hint)"
    $lines += "- Risk: $($Item.risk_hint)"
    $lines += "- Inspect status: $(if ($Item.info_pass) { 'PASS' } else { 'FAIL' })"
    if (-not $Item.info_pass) {
        $lines += "- Inspect error: $($Item.info_message)"
    }
    $lines += ""

    if ($null -eq $Item.info) {
        $lines += "No bridge inspection data was captured for this function."
        $lines | Set-Content -Path $OutputPath -Encoding UTF8
        return
    }

    if (-not [string]::IsNullOrWhiteSpace($Item.info.description)) {
        $lines += "## Description"
        $lines += ""
        $lines += $Item.info.description
        $lines += ""
    }

    $lines += "## Inputs"
    $lines += ""
    if (@($Item.info.inputs).Count -eq 0) {
        $lines += "- None"
    }
    else {
        foreach ($input in @($Item.info.inputs)) {
            $typeSuffix = if ([string]::IsNullOrWhiteSpace($input.type)) { '' } else { " [$($input.type)]" }
            $description = if ([string]::IsNullOrWhiteSpace($input.description)) { '' } else { " - $($input.description)" }
            $lines += "- ``$($input.name)``$typeSuffix$description"
        }
    }
    $lines += ""

    $lines += "## Outputs"
    $lines += ""
    if (@($Item.info.outputs).Count -eq 0) {
        $lines += "- None"
    }
    else {
        foreach ($output in @($Item.info.outputs)) {
            $description = if ([string]::IsNullOrWhiteSpace($output.description)) { '' } else { " - $($output.description)" }
            $lines += "- ``$($output.name)``$description"
        }
    }
    $lines += ""

    $hasGraph = ($Item.info.PSObject.Properties.Name -contains 'nodes') -and (@($Item.info.nodes).Count -gt 0)
    if ($hasGraph) {
        $lines += "## Internal Nodes"
        $lines += ""
        $lines += "| Index | Kind | Class | Caption | Semantic Name |"
        $lines += "|---:|---|---|---|---|"
        foreach ($node in @($Item.info.nodes | Sort-Object index)) {
            $semanticName = if (-not [string]::IsNullOrWhiteSpace($node.input_name)) {
                $node.input_name
            }
            elseif (-not [string]::IsNullOrWhiteSpace($node.output_name)) {
                $node.output_name
            }
            else {
                ''
            }
            $caption = ([string]$node.caption) -replace '\|', '/'
            $class = ([string]$node.class) -replace '\|', '/'
            $lines += "| $($node.index) | $($node.node_kind) | ``$class`` | $caption | $semanticName |"
        }
        $lines += ""

        $lines += "## Exact Input Wiring"
        $lines += ""
        foreach ($node in @($Item.info.nodes | Sort-Object index)) {
            $connectedInputs = @($node.inputs | Where-Object { $_.connected })
            if ($connectedInputs.Count -eq 0) {
                continue
            }
            $lines += "### [$($node.index)] $($node.caption)"
            $lines += ""
            foreach ($input in $connectedInputs) {
                $sourceIndex = if ($null -ne $input.source_index) { "[$($input.source_index)]" } else { "[]" }
                $lines += "- ``$($input.input_name)`` <= $sourceIndex $($input.source_caption) output $($input.output_index)"
            }
            $lines += ""
        }

        $lines += "## Connection List"
        $lines += ""
        if (($Item.info.PSObject.Properties.Name -contains 'connections') -and @($Item.info.connections).Count -gt 0) {
            foreach ($connection in @($Item.info.connections)) {
                $sourceIndex = if ($null -ne $connection.source_index) { "[$($connection.source_index)]" } else { "[]" }
                $lines += "- $sourceIndex $($connection.source_caption) output $($connection.source_output_index) -> $($connection.target_caption).$($connection.target_input)"
            }
        }
        else {
            $lines += "- No connection list captured."
        }
        $lines += ""

        $outputNodes = @($Item.info.nodes | Where-Object { $_.node_kind -eq 'function_output' } | Sort-Object index)
        if ($outputNodes.Count -gt 0) {
            $lines += "## Output Trace Roots"
            $lines += ""
            foreach ($outputNode in $outputNodes) {
                $firstInput = @($outputNode.inputs | Where-Object { $_.connected } | Select-Object -First 1)
                if ($firstInput.Count -gt 0) {
                    $sourceIndex = if ($null -ne $firstInput[0].source_index) { "[$($firstInput[0].source_index)]" } else { "[]" }
                    $lines += "- ``$($outputNode.output_name)`` is driven by $sourceIndex $($firstInput[0].source_caption)"
                }
                else {
                    $lines += "- ``$($outputNode.output_name)`` has no captured source."
                }
            }
            $lines += ""
        }
    }

    $lines += "## Agent Guidance"
    $lines += ""
    $lines += "- Instantiate with `add_material_expression` using `expression_type='MaterialFunctionCall'` and this function path."
    $lines += "- Inspect with `get_material_function_info` before production wiring."
    $lines += "- Wire call-site inputs by `target_input_name`; do not guess pin order from screenshots."
    $lines += "- Use this deep graph doc as ground truth for internal structure, not as a promise that the function is cheap."

    $lines | Set-Content -Path $OutputPath -Encoding UTF8
}

function Convert-BridgeResult {
    param($Value)

    if ($Value -is [string]) {
        return ($Value | ConvertFrom-Json)
    }

    return $Value
}

$resolvedCatalogPath = Resolve-RepoPath -Path $CatalogPath
$resolvedOutputRoot = Resolve-RepoPath -Path $OutputRoot
$resolvedKnowledgeOutputPath = Resolve-RepoPath -Path $KnowledgeOutputPath
$resolvedFunctionDocsRoot = Join-Path $resolvedOutputRoot 'Functions'

if (-not (Test-Path $resolvedCatalogPath)) {
    throw "Catalog not found: $resolvedCatalogPath"
}

if ($IncludeFunctionInfo) {
    . (Join-Path $PSScriptRoot 'invoke_bridge.ps1')
    Wait-ForgePilotBridge
}

$catalog = Get-Content -Path $resolvedCatalogPath -Raw | ConvertFrom-Json
$catalogEngineLabel = if ($catalog.PSObject.Properties.Name -contains 'engine' -and -not [string]::IsNullOrWhiteSpace([string]$catalog.engine)) {
    [string]$catalog.engine
}
else {
    'Unreal Engine'
}
$catalogFunctionCandidates = @($catalog.engine_material_functions)
$catalogFunctionCandidateCount = $catalogFunctionCandidates.Count

if ($IncludeFunctionInfo) {
    $scanResult = Convert-BridgeResult (Invoke-ForgePilotTool -ToolName 'scan_assets' -InputObject @{
        folder_path = '/Engine/Functions'
        asset_type = 'MaterialFunction'
        max_results = '2000'
    } -TimeoutSeconds $TimeoutSeconds)

    $functions = @($scanResult.assets | ForEach-Object {
        $category = [string]$_.package_path
        $category = $category -replace '^/Engine/Functions/', ''
        [pscustomobject][ordered]@{
            function_name = [string]$_.name
            package_path = [string]$_.path
            category_hint = $category
        }
    })
}
else {
    $functions = $catalogFunctionCandidates
}

if ($MaxFunctions -gt 0) {
    $functions = @($functions | Select-Object -First $MaxFunctions)
}

New-Item -ItemType Directory -Force -Path $resolvedOutputRoot | Out-Null
New-Item -ItemType Directory -Force -Path $resolvedFunctionDocsRoot | Out-Null
New-Item -ItemType Directory -Force -Path (Split-Path -Parent $resolvedKnowledgeOutputPath) | Out-Null

# Keep generated category docs in sync with the current Asset Registry scan.
Get-ChildItem -Path $resolvedOutputRoot -File -Filter '*.md' | Remove-Item -Force
Get-ChildItem -Path $resolvedFunctionDocsRoot -File -Filter '*.md' | Remove-Item -Force

$studyFunctions = @()
$count = 0
foreach ($function in $functions) {
    $count++
    $functionName = [string]$function.function_name
    $functionPath = [string]$function.package_path
    $category = [string]$function.category_hint
    $usageHint = Get-FunctionUsageHint -Category $category -Name $functionName
    $riskHint = Get-FunctionRiskHint -Category $category -Name $functionName

    $infoPass = $false
    $infoMessage = 'not requested'
    $infoSummary = $null

    if ($IncludeFunctionInfo) {
        try {
            Write-Progress -Activity 'Studying UE material functions' -Status "$count / $($functions.Count): $functionName" -PercentComplete (($count / [math]::Max($functions.Count, 1)) * 100)
            $infoResult = Convert-BridgeResult (Invoke-ForgePilotTool -ToolName 'get_material_function_info' -InputObject @{
                material_function_path = $functionPath
            } -TimeoutSeconds $TimeoutSeconds)
            $infoPass = $true
            $infoMessage = 'ok'
            $infoSummary = Get-FunctionInfoSummary -InfoResult $infoResult -IncludeInternalGraph ([bool]$IncludeInternalGraph)
        }
        catch {
            $infoPass = $false
            $infoMessage = $_.Exception.Message
        }
    }

    $studyFunctions += [pscustomobject][ordered]@{
        function_name = $functionName
        package_path = $functionPath
        category_hint = $category
        usage_hint = $usageHint
        risk_hint = $riskHint
        info_pass = $infoPass
        info_message = $infoMessage
        info = $infoSummary
    }
}

if ($IncludeFunctionInfo) {
    Write-Progress -Activity 'Studying UE material functions' -Completed
}

$categoryDocs = @()
$groups = $studyFunctions | Group-Object category_hint | Sort-Object Name
foreach ($group in $groups) {
    $safeName = ConvertTo-SafeFileName -Value $group.Name
    $fileName = "$safeName.md"
    $outputPath = Join-Path $resolvedOutputRoot $fileName
    $relativeDocPath = ConvertTo-ForwardSlash (Join-Path $OutputRoot $fileName)

    $lines = @()
    $lines += "# $($group.Name)"
    $lines += ""
    $lines += "Engine: $catalogEngineLabel"
    $lines += "Function count: $($group.Count)"
    $lines += ""
    $lines += "## Agent Use"
    $lines += ""
    $representative = @($group.Group | Select-Object -First 1)
    if ($representative.Count -gt 0) {
        $lines += $representative[0].usage_hint
        $lines += ""
        $lines += "Risk: $($representative[0].risk_hint)"
    }
    $lines += ""
    $lines += "## Functions"
    $lines += ""

    foreach ($item in @($group.Group | Sort-Object function_name)) {
        $safeFunctionDocName = '{0}__{1}.md' -f (ConvertTo-SafeFileName -Value $item.category_hint), (ConvertTo-SafeFileName -Value $item.function_name)
        $functionDocPath = Join-Path $resolvedFunctionDocsRoot $safeFunctionDocName
        $functionDocRelativePath = ConvertTo-ForwardSlash (Join-Path (Join-Path $OutputRoot 'Functions') $safeFunctionDocName)
        $item | Add-Member -NotePropertyName deep_doc_path -NotePropertyValue $functionDocRelativePath -Force
        if ($IncludeFunctionInfo) {
            Write-FunctionDeepDoc -Item $item -OutputPath $functionDocPath
        }

        $lines += "### $($item.function_name)"
        $lines += ""
        $lines += "- Path: ``$($item.package_path)``"
        if ($IncludeFunctionInfo) {
            $lines += "- Deep graph doc: [$($item.function_name)](Functions/$safeFunctionDocName)"
        }
        $lines += "- Use: $($item.usage_hint)"
        $lines += "- Risk: $($item.risk_hint)"
        $lines += "- Study status: $(if ($item.info_pass) { 'inspected through get_material_function_info' } elseif ($IncludeFunctionInfo) { 'inspect failed' } else { 'catalog only' })"
        if ($IncludeFunctionInfo -and -not $item.info_pass) {
            $lines += "- Inspect error: $($item.info_message)"
        }

        if ($null -ne $item.info) {
            $lines += "- Inputs: $($item.info.input_count)"
            $lines += "- Outputs: $($item.info.output_count)"
            $lines += "- Internal nodes: $($item.info.node_count)"
            if (-not [string]::IsNullOrWhiteSpace($item.info.description)) {
                $lines += "- Description: $($item.info.description)"
            }

            $inputNames = @($item.info.inputs | Select-Object -First 12 | ForEach-Object {
                if ([string]::IsNullOrWhiteSpace($_.type)) { $_.name } else { "$($_.name) [$($_.type)]" }
            })
            if ($inputNames.Count -gt 0) {
                $lines += "- Input pins: $($inputNames -join ', ')"
            }

            $outputNames = @($item.info.outputs | Select-Object -First 12 | ForEach-Object { $_.name })
            if ($outputNames.Count -gt 0) {
                $lines += "- Output pins: $($outputNames -join ', ')"
            }
        }

        $lines += ""
    }

    $lines | Set-Content -Path $outputPath -Encoding UTF8

    $categoryDocs += [pscustomobject][ordered]@{
        category = $group.Name
        function_count = $group.Count
        doc_path = $relativeDocPath
    }
}

$readmeLines = @()
$readmeLines += "# Current Material Function Study"
$readmeLines += ""
$readmeLines += "Generated for ForgePilot so Codex, Claude, and other AI agents can choose Material Functions instead of rebuilding large expression chains by hand."
$readmeLines += ""
$readmeLines += "## Rules For Agents"
$readmeLines += ""
$readmeLines += "- Treat Material Functions as reusable graph blocks made from Material Expressions."
$readmeLines += "- Prefer functions for complex reusable logic: flipbooks, radial UVs, world aligned mapping, blend modes, remap/contrast, procedural helpers, and packed channel helpers."
$readmeLines += "- Keep simple math inline with expressions when it is only one or two nodes."
$readmeLines += '- Before wiring a MaterialFunctionCall, call `get_material_function_info` and connect by `target_input_name` rather than guessed pin index.'
$readmeLines += "- A function is not automatically cheaper at shader runtime; it mainly improves graph readability, reuse, and consistency."
$readmeLines += ""
$readmeLines += "## Category Docs"
$readmeLines += ""
foreach ($doc in $categoryDocs) {
    $readmeLines += "- [$($doc.category)]($([System.IO.Path]::GetFileName($doc.doc_path))) - $($doc.function_count) functions"
}
$readmeLines += ""
$readmeLines += "## Source"
$readmeLines += ""
$readmeLines += "- Catalog: ``$CatalogPath``"
$readmeLines += '- Exporter: `Tools/Validation/export_material_function_docs.ps1`'
$readmeLines += "- IncludeFunctionInfo: $([bool]$IncludeFunctionInfo)"

$readmePath = Join-Path $resolvedOutputRoot 'README.md'
$readmeLines | Set-Content -Path $readmePath -Encoding UTF8

$passCount = @($studyFunctions | Where-Object { $_.info_pass }).Count
$failCount = if ($IncludeFunctionInfo) { @($studyFunctions | Where-Object { -not $_.info_pass }).Count } else { 0 }

$knowledgeIndex = [ordered]@{
    generated_at = (Get-Date).ToString('o')
    engine = $catalogEngineLabel
    purpose = 'Material Function study index for AI agents. Use this with material_function_usage_strategy.json and the category markdown docs.'
    catalog_reference = ConvertTo-ForwardSlash $CatalogPath
    docs_root = ConvertTo-ForwardSlash $OutputRoot
    readme = ConvertTo-ForwardSlash (Join-Path $OutputRoot 'README.md')
    exporter = 'Tools/Validation/export_material_function_docs.ps1'
    include_function_info = [bool]$IncludeFunctionInfo
    include_internal_graph = [bool]$IncludeInternalGraph
    catalog_function_candidate_count = $catalogFunctionCandidateCount
    actual_material_function_count = $studyFunctions.Count
    excluded_non_material_function_candidate_count = if ($IncludeFunctionInfo) { [math]::Max(0, $catalogFunctionCandidateCount - $studyFunctions.Count) } else { 0 }
    total_functions = $studyFunctions.Count
    category_count = $categoryDocs.Count
    info_pass_count = $passCount
    info_fail_count = $failCount
    agent_rules = @(
        'Use this study index to choose candidate MaterialFunctions by category and intent.',
        'Use get_material_function_info immediately before wiring any selected MaterialFunctionCall.',
        'Wire MaterialFunctionCall inputs by target_input_name instead of raw input index.',
        'Prefer functions for reusable blocks, not for trivial one-node math.'
    )
    categories = $categoryDocs
    functions = $studyFunctions
}

$knowledgeIndex | ConvertTo-Json -Depth 100 | Set-Content -Path $resolvedKnowledgeOutputPath -Encoding UTF8

Write-Host "Material function docs written to $resolvedOutputRoot"
Write-Host "Material function study index written to $resolvedKnowledgeOutputPath"
Write-Host ("Functions: {0}  Categories: {1}  InfoPass: {2}  InfoFail: {3}" -f $studyFunctions.Count, $categoryDocs.Count, $passCount, $failCount)
if ($IncludeFunctionInfo) {
    Write-Host ("CatalogCandidates: {0}  ExcludedNonMaterialFunctionCandidates: {1}" -f $catalogFunctionCandidateCount, [math]::Max(0, $catalogFunctionCandidateCount - $studyFunctions.Count))
}

if ($IncludeFunctionInfo -and $failCount -gt 0) {
    exit 1
}
