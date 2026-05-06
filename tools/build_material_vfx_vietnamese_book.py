from pathlib import Path

from docx import Document
from docx.enum.text import WD_ALIGN_PARAGRAPH
from docx.oxml import OxmlElement
from docx.oxml.ns import qn
from docx.shared import Cm, Pt, RGBColor


OUT = Path.home() / "Downloads" / "Giao_Trinh_Material_VFX_Realtime.docx"

INK = RGBColor(34, 34, 34)
MUTED = RGBColor(92, 92, 92)
BLUE = RGBColor(22, 72, 112)
TEAL = RGBColor(0, 112, 112)
GOLD = RGBColor(145, 94, 10)
RED = RGBColor(150, 45, 45)
LIGHT_BLUE = "EAF3F8"
LIGHT_TEAL = "E9F5F4"
LIGHT_GOLD = "FFF3D6"
LIGHT_RED = "FCE8E6"
LIGHT_GRAY = "F1F3F5"


def set_font(run, size=9.8, bold=False, italic=False, color=INK, name="Arial"):
    run.font.name = name
    run.font.size = Pt(size)
    run.font.bold = bold
    run.font.italic = italic
    run.font.color.rgb = color


def shade(paragraph, fill):
    p_pr = paragraph._p.get_or_add_pPr()
    shd = p_pr.find(qn("w:shd"))
    if shd is None:
        shd = OxmlElement("w:shd")
        p_pr.append(shd)
    shd.set(qn("w:fill"), fill)


def border_bottom(paragraph, color="D9E2EA", size="8"):
    p_pr = paragraph._p.get_or_add_pPr()
    p_bdr = p_pr.find(qn("w:pBdr"))
    if p_bdr is None:
        p_bdr = OxmlElement("w:pBdr")
        p_pr.append(p_bdr)
    bottom = p_bdr.find(qn("w:bottom"))
    if bottom is None:
        bottom = OxmlElement("w:bottom")
        p_bdr.append(bottom)
    bottom.set(qn("w:val"), "single")
    bottom.set(qn("w:sz"), size)
    bottom.set(qn("w:space"), "3")
    bottom.set(qn("w:color"), color)


def p(doc, text="", size=9.8, color=INK, bold=False, italic=False, before=0, after=3, align=None):
    para = doc.add_paragraph()
    para.paragraph_format.space_before = Pt(before)
    para.paragraph_format.space_after = Pt(after)
    para.paragraph_format.line_spacing = 1.07
    if align is not None:
        para.alignment = align
    if text:
        r = para.add_run(text)
        set_font(r, size=size, bold=bold, italic=italic, color=color)
    return para


def part(doc, title):
    para = p(doc, title.upper(), size=16.5, bold=True, color=BLUE, before=10, after=7)
    border_bottom(para, color="BBD1E1", size="12")


def h1(doc, title):
    para = p(doc, title, size=14.2, bold=True, color=BLUE, before=8, after=5)
    border_bottom(para)


def h2(doc, title):
    p(doc, title, size=11.4, bold=True, color=TEAL, before=6, after=3)


def h3(doc, title):
    p(doc, title, size=10.2, bold=True, color=BLUE, before=4, after=2)


def body(doc, text):
    p(doc, text, size=10.05, after=3.4)


def bullet(doc, text, level=0):
    para = doc.add_paragraph()
    para.paragraph_format.left_indent = Cm(0.55 + level * 0.35)
    para.paragraph_format.first_line_indent = Cm(-0.25)
    para.paragraph_format.space_after = Pt(2.1)
    para.paragraph_format.line_spacing = 1.05
    r = para.add_run("- " + text)
    set_font(r, size=9.55)


def numbered(doc, index, text):
    para = doc.add_paragraph()
    para.paragraph_format.left_indent = Cm(0.72)
    para.paragraph_format.first_line_indent = Cm(-0.42)
    para.paragraph_format.space_after = Pt(2.1)
    para.paragraph_format.line_spacing = 1.05
    r = para.add_run(f"{index}. {text}")
    set_font(r, size=9.55)


def callout(doc, title, lines, fill=LIGHT_BLUE, color=BLUE):
    para = p(doc, title, size=9.6, bold=True, color=color, before=5, after=1)
    para.paragraph_format.left_indent = Cm(0.22)
    para.paragraph_format.right_indent = Cm(0.15)
    shade(para, fill)
    for line in lines:
        para = doc.add_paragraph()
        para.paragraph_format.left_indent = Cm(0.52)
        para.paragraph_format.right_indent = Cm(0.15)
        para.paragraph_format.space_after = Pt(1.4)
        para.paragraph_format.line_spacing = 1.03
        shade(para, fill)
        r = para.add_run("- " + line)
        set_font(r, size=9.15)
    p(doc, "", after=1)


def code(doc, lines, title=None):
    if title:
        p(doc, title, size=8.8, color=MUTED, italic=True, after=1)
    for line in lines:
        para = p(doc, line, size=8.45, after=0.9)
        para.paragraph_format.left_indent = Cm(0.35)
        para.paragraph_format.right_indent = Cm(0.15)
        shade(para, LIGHT_GRAY)
        for run in para.runs:
            set_font(run, size=8.45, name="Consolas")


def term_entry(doc, name, meaning, formula, vfx_use, mistake):
    h3(doc, name)
    body(doc, f"Ý nghĩa: {meaning}")
    if formula:
        code(doc, [formula], "Công thức / hình dung")
    bullet(doc, f"Dùng trong VFX: {vfx_use}")
    bullet(doc, f"Lỗi thường gặp: {mistake}")


def recipe(doc, title, goal, graph, params, mistakes, debug, optimize, exercise):
    h1(doc, title)
    body(doc, f"Mục tiêu visual: {goal}")
    h2(doc, "Graph logic")
    code(doc, graph)
    h2(doc, "Parameter nên expose")
    for item in params:
        bullet(doc, item)
    callout(doc, "Lỗi thường gặp", mistakes, fill=LIGHT_RED, color=RED)
    h2(doc, "Cách debug")
    for item in debug:
        bullet(doc, item)
    h2(doc, "Tối ưu")
    for item in optimize:
        bullet(doc, item)
    callout(doc, "Bài tập nhỏ", exercise, fill=LIGHT_GOLD, color=GOLD)


def lab(doc, title, goal, setup, graph_steps, observe, variations, debug_notes):
    h1(doc, title)
    body(doc, f"Mục tiêu bài lab: {goal}")
    h2(doc, "Setup")
    for item in setup:
        bullet(doc, item)
    h2(doc, "Các bước dựng graph")
    for i, item in enumerate(graph_steps, 1):
        numbered(doc, i, item)
    h2(doc, "Quan sát cần ghi lại")
    for item in observe:
        bullet(doc, item)
    h2(doc, "Biến thể để luyện ứng biến")
    for item in variations:
        bullet(doc, item)
    callout(doc, "Debug note", debug_notes, fill=LIGHT_TEAL)


def family_page(doc, title, identity, core_blocks, common_params, failure_modes, production_note):
    h1(doc, title)
    body(doc, f"Nhận diện visual: {identity}")
    h2(doc, "Block cấu trúc")
    for item in core_blocks:
        bullet(doc, item)
    h2(doc, "Parameter thường cần expose")
    for item in common_params:
        bullet(doc, item)
    callout(doc, "Failure modes", failure_modes, fill=LIGHT_RED, color=RED)
    h2(doc, "Production note")
    body(doc, production_note)


def deep_node(doc, name, role, when_use, avoid, formula, exercise):
    h1(doc, name)
    body(doc, f"Vai trò: {role}")
    h2(doc, "Khi nên dùng")
    for item in when_use:
        bullet(doc, item)
    h2(doc, "Khi nên tránh hoặc cẩn thận")
    for item in avoid:
        bullet(doc, item)
    code(doc, formula, "Công thức / node chain mẫu")
    callout(doc, "Mini exercise", exercise, fill=LIGHT_GOLD, color=GOLD)


def cover(doc):
    p(doc, "", before=4)
    p(doc, "GIÁO TRÌNH MATERIAL VFX REALTIME", size=23, bold=True, color=BLUE, align=WD_ALIGN_PARAGRAPH.CENTER, after=4)
    p(doc, "Unreal Material + Niagara + tư duy shader áp dụng được cho Unity/engine khác", size=11, color=INK, align=WD_ALIGN_PARAGRAPH.CENTER, after=10)
    callout(doc, "Cách dùng quyển này", [
        "Đọc như sách học ngôn ngữ: từ vựng node, ngữ pháp mask/UV, rồi mới recipe production.",
        "Mỗi chương nên đi kèm việc mở một material thật trong Unreal để nhìn node graph.",
        "Khi gặp feedback mới, hãy hỏi: cần sửa shape, mask, motion, color, interaction hay performance?",
    ], fill=LIGHT_GOLD, color=GOLD)
    p(doc, "Biên soạn từ ForgePilot Knowledge, Unreal Material patterns và VFXSamplePack", size=9, color=MUTED, align=WD_ALIGN_PARAGRAPH.CENTER, before=6)


def toc(doc):
    h1(doc, "Mục lục học tập")
    entries = [
        "Phần 1 - Tư duy material realtime: texture, shader, material, renderer, cách bóc effect.",
        "Phần 2 - Từ vựng shader/material: arithmetic, range/logic, vector/space, periodic/radial.",
        "Phần 3 - Các hệ material VFX quan trọng: UV, mask, blend, fresnel, depth, SubUV, WPO, texture authoring.",
        "Phần 4 - Recipe production: flare, dissolve, fire, smoke, portal, shield, beam, projectile, surface family.",
        "Phần 5 - Đọc material trong VFXSamplePack: M_Flare, M_Mesh_Add, M_FlamesCard, M_SubUV_Utilities.",
        "Phần 6 - Workbook thực hành material: 12 bài lab để luyện mask, UV, SubUV, WPO và debug.",
        "Phần 7 - Atlas các family material: fire, smoke, dissolve, portal, shield, beam, projectile, ice/lava/glass/wet.",
        "Phần 8 - Material Function và master material: khi nào tách function, cách expose parameter, optimization.",
        "Phần 9 - Phụ lục công thức nhanh: radial circle, ring, fresnel, dissolve, camera/depth fade, polar swirl, WPO.",
        "Phần 10 - Từ điển node chuyên sâu: Particle Color, Dynamic Parameter, SubUV, Depth, WorldPosition, WPO, HLSL.",
        "Phần 11 - Quy trình đọc graph theo lane: UV, Mask, Color, Opacity, WPO, Interaction, Performance.",
        "Phần 12 - Bài tập tổng hợp theo production: flare pack, dissolve surface, fire card, smoke flipbook, portal, shield, beam, projectile.",
        "Phần 13 - Checklist đi làm và tự học dài hạn.",
        "Phần 14 - Lộ trình luyện 6 tuần.",
        "Phần 15 - Câu hỏi tự kiểm trước khi nói mình hiểu.",
        "Phần 16 - Glossary production cho material VFX.",
        "Phần 17 - Troubleshooting theo triệu chứng.",
        "Phần 18 - Chuyển tư duy giữa Unreal và Unity.",
        "Phần 19 - Quy trình review material trong 10 phút và nhật ký học 30 ngày.",
    ]
    for e in entries:
        bullet(doc, e)
    callout(doc, "Bản đồ học 5 tầng", [
        "Tầng 1: đọc visual thành shape, mask, motion, color, interaction, cost.",
        "Tầng 2: nắm từ vựng node và công thức grayscale trước khi thêm màu.",
        "Tầng 3: hiểu các hệ VFX chính như SubUV, Fresnel, DepthFade, WPO, channel packing.",
        "Tầng 4: dựng recipe production và biết expose parameter cho artist/director.",
        "Tầng 5: đọc material thật, tự rebuild, tối ưu, rồi chuyển tư duy sang engine khác.",
    ], fill=LIGHT_TEAL)
    doc.add_page_break()


def build():
    doc = Document()
    section = doc.sections[0]
    section.page_width = Cm(21)
    section.page_height = Cm(29.7)
    section.top_margin = Cm(1.35)
    section.bottom_margin = Cm(1.25)
    section.left_margin = Cm(1.55)
    section.right_margin = Cm(1.55)
    doc.styles["Normal"].font.name = "Arial"
    doc.styles["Normal"].font.size = Pt(9.6)
    footer = section.footer.paragraphs[0]
    footer.alignment = WD_ALIGN_PARAGRAPH.CENTER
    r = footer.add_run("Giáo Trình Material VFX Realtime")
    set_font(r, size=8, color=MUTED)

    cover(doc)
    toc(doc)

    part(doc, "Phần 1 - Tư duy material realtime")
    h1(doc, "1. Material là gì trong engine realtime")
    body(doc, "Trong realtime engine, material là lớp logic quyết định một bề mặt, một sprite, một ribbon, một mesh particle hoặc một post-process sẽ biến dữ liệu đầu vào thành pixel như thế nào. Với VFX, material thường không cố mô phỏng vật lý đầy đủ; nó dùng texture, toán học, particle data và camera/depth data để tạo cảm giác chuyển động, sáng, tan, méo, phát sáng hoặc tương tác với thế giới.")
    body(doc, "Điểm khác biệt quan trọng so với AE là material chạy mỗi frame trong game. Nó không chỉ là một hình đã render sẵn. Nó có thể nhận màu từ Niagara, nhận lifetime từ particle, nhận depth từ scene, nhận world position từ mesh, và thay đổi theo camera. Vì vậy học material là học cách biến dữ liệu động thành visual có kiểm soát.")
    code(doc, [
        "Data: texture, UV, time, particle color, depth, world position",
        "-> Shader/material math",
        "-> Output: emissive, opacity, base color, WPO, refraction",
        "-> Renderer: sprite / mesh / ribbon / decal / post-process",
    ], "Pipeline cơ bản")
    callout(doc, "Ghi nhớ", [
        "Material VFX không phải chỉ là màu đẹp. Nó là một hệ thống nhỏ: lấy data, tạo mask, tạo motion, tạo color, rồi xuất pixel.",
        "Nếu hiểu được data nào đang điều khiển visual nào, bạn có thể sửa feedback nhanh hơn rất nhiều.",
    ])

    h1(doc, "2. Texture, shader, material, material instance, renderer")
    body(doc, "Texture là dữ liệu hình ảnh: màu, alpha, noise, normal, emissive mask, flipbook frame, flow vector. Shader là chương trình GPU xử lý dữ liệu đó. Material là asset authoring trong engine, thường được node hóa để sinh shader. Material Instance là bản biến thể expose parameter mà không cần sửa graph gốc. Renderer là nơi material được dùng: sprite renderer, mesh renderer, ribbon renderer, decal renderer hoặc post-process pass.")
    for item in [
        "Texture trả lời câu hỏi: dữ liệu thô nằm ở đâu?",
        "Shader/material trả lời: xử lý dữ liệu đó bằng công thức nào?",
        "Material Instance trả lời: artist chỉnh control nào mà không phá graph?",
        "Renderer trả lời: geometry hoặc screen pass nào đang dùng material?",
        "Niagara/VFX Graph trả lời: particle data nào được gửi vào material?",
    ]:
        bullet(doc, item)
    callout(doc, "Ví dụ", [
        "Một fire flipbook: texture chứa frame lửa; Niagara chọn frame theo lifetime; material đọc ParticleSubUV, nhân ParticleColor, tạo emissive và opacity; Sprite Renderer vẽ card ra màn hình.",
        "Một shield mesh: mesh sphere cung cấp normal/world position; material dùng fresnel tạo rim, noise tạo pattern, depth fade tạo giao nhau mềm; Blueprint/Niagara gửi hit position để tạo ripple.",
    ], fill=LIGHT_TEAL)

    h1(doc, "3. Cách bóc tách mọi effect")
    body(doc, "Một senior Technical VFX Artist không nhìn effect như một đống node. Họ nhìn effect thành các lớp ý nghĩa. Cách bóc tách này giúp bạn học từ VFXSamplePack, debug production, và tự tạo visual mới.")
    layers = [
        ("Shape", "Silhouette chính là gì? Vòng tròn, slash arc, beam dài, smoke puff, mesh shell, portal rim?"),
        ("Mask", "Vùng nào hiện, vùng nào mất, biên mềm hay cứng, dissolve chạy theo noise hay gradient?"),
        ("UV motion", "Chuyển động đến từ panner, rotator, flipbook, flow map, polar UV hay WPO?"),
        ("Color", "Core sáng ở đâu, rim ở đâu, màu nóng/lạnh ra sao, emissive có bloom không?"),
        ("Opacity", "Alpha đến từ texture, ParticleColor, depth fade, camera fade hay threshold?"),
        ("Interaction", "Effect có phản ứng với camera, depth, hit location, socket, velocity hay gameplay parameter không?"),
        ("Optimization", "Cost nằm ở overdraw, texture fetch, instruction, refraction, particle count hay memory?"),
    ]
    for name, desc in layers:
        bullet(doc, f"{name}: {desc}")
    code(doc, [
        "Effect = Shape + Mask + Motion + Color + Opacity + Interaction + Cost",
        "Feedback = thay đổi một hoặc nhiều lớp trong công thức trên",
    ])

    h1(doc, "4. AE/Spine mindset và realtime mindset")
    body(doc, "Kinh nghiệm AE rất có giá trị vì nó rèn mắt về timing, contrast, glow, layer và silhouette. Nhưng AE thường xuất ra kết quả đã được quyết định trước: PNG sequence, flipbook, sprite sheet, video hoặc texture. Realtime material thì phải sống trong game: chạy ở nhiều góc camera, nhiều kích thước, nhiều ánh sáng, nhiều platform.")
    for item in [
        "AE mạnh ở authoring: vẽ shape, layer, keyframe, glow, blur, distortion, plugin.",
        "Engine mạnh ở ứng dụng: particle spawn, socket attach, depth, collision, gameplay parameter, scalability.",
        "Workflow mạnh nhất thường là: AE/Houdini/EmberGen tạo texture hoặc flipbook; Unreal/Unity dùng material + particle để đưa vào gameplay.",
        "Nếu chỉ biết AE, bạn tạo được hình đẹp. Nếu biết material, bạn tạo được hệ thống hình ảnh có thể tái sử dụng và tương tác.",
    ]:
        bullet(doc, item)

    part(doc, "Phần 2 - Từ vựng shader/material")
    h1(doc, "5. Arithmetic - các phép toán nguyên tử")
    body(doc, "Đây là nhóm node có vẻ đơn giản nhưng xuất hiện trong hầu hết material production. Chúng giống chữ cái trong ngôn ngữ shader.")
    arithmetic = [
        ("Add / Subtract", "Cộng hoặc trừ để dịch giá trị, dịch UV, đẩy threshold, cộng nhiều mask.", "shifted = mask - threshold", "dissolve threshold, UV offset, cộng emissive layer", "Cộng quá nhiều layer khiến mask vượt 1 mà không được clamp/saturate."),
        ("Multiply / Divide", "Nhân để scale intensity, opacity, UV, vector hoặc mask.", "emissive = color * mask * intensity", "flare intensity, opacity scale, tiling UV", "Nhân opacity và emissive không có kế hoạch làm effect lúc quá tối lúc quá cháy."),
        ("Power", "Đổi độ cong/contrast của giá trị 0-1.", "tight = pow(mask, exponent)", "flare core, fresnel sharpness, alpha tightening", "Exponent quá cao làm mất chi tiết; quá thấp làm toàn bộ vùng sáng."),
        ("OneMinus", "Đảo mask theo công thức 1 - x.", "inverted = 1 - mask", "inside-out dissolve, đảo fresnel, đảo gradient", "Đảo nhầm mask trước/ sau clamp làm visual khó đoán."),
        ("Abs", "Lấy trị tuyệt đối, bỏ dấu âm và giữ độ lớn.", "band = 1 - abs(x - center)", "mirrored wave, ring band, khoảng cách đến trung tâm", "Dùng Abs mà quên remap khiến band bị gắt hoặc sai biên."),
        ("Frac", "Giữ phần lẻ để tạo loop 0-1.", "loop = frac(time * speed)", "scanline, tiling, loop time, custom frame", "Frac tạo discontinuity; nếu cần mượt phải xử lý crossfade."),
        ("Floor", "Làm tròn xuống, tạo bước rời rạc.", "frame = floor(age * frameCount)", "flipbook frame index, pixelate, stepped animation", "Floor làm motion giật; phù hợp frame/step nhưng không phù hợp flow mềm."),
    ]
    for row in arithmetic:
        term_entry(doc, *row)

    h1(doc, "6. Range và logic - biến số thành control")
    body(doc, "Range/logic là nơi raw value trở thành mask artist-friendly. Đây là trung tâm của dissolve, edge band, falloff, alpha và blend.")
    logic_nodes = [
        ("Clamp", "Giới hạn giá trị trong một khoảng tùy ý.", "clamped = clamp(x, min, max)", "chặn overbright, ổn định mask, remap an toàn", "Clamp quá sớm có thể giết mất dynamic range cần cho contrast."),
        ("Saturate", "Clamp nhanh về 0-1.", "safe = saturate(x)", "alpha, mask, lerp alpha, edge band", "Quên saturate sau subtract có thể sinh opacity âm hoặc alpha > 1."),
        ("Lerp", "Blend A sang B bằng alpha.", "out = lerp(A, B, mask)", "color ramp, texture blend, dissolve state, feature blend", "Mask không sạch làm blend bị bẩn/muddy."),
        ("Step", "Ngưỡng cứng: dưới edge là 0, trên edge là 1.", "hard = step(threshold, mask)", "hard dissolve, toon cut, binary mask", "Step dễ aliasing; dùng SmoothStep nếu biên cần mềm."),
        ("SmoothStep", "Ngưỡng mềm giữa hai giá trị.", "soft = smoothstep(a, b, mask)", "soft edge, depth fade, dissolve band, radial falloff", "a và b quá gần làm biên vẫn gắt; quá xa làm mất lực."),
        ("Min / Max", "Chọn giá trị nhỏ/lớn hơn để combine mask không cần branch.", "combined = max(maskA, maskB)", "gộp nhiều nguồn mask, clip, dominant layer", "Max nhiều noise có thể làm effect đầy đặc, mất silhouette."),
    ]
    for row in logic_nodes:
        term_entry(doc, *row)
    code(doc, [
        "Hard cut:  visible = step(threshold, mask)",
        "Soft cut:  visible = smoothstep(threshold, threshold + softness, mask)",
        "Edge:      edge = smoothstep(t - w, t, mask) - smoothstep(t, t + s, mask)",
    ], "Ba công thức cần thuộc")

    h1(doc, "7. Vector và coordinate space")
    vector_nodes = [
        ("Dot Product", "Đo hai vector cùng hướng đến mức nào.", "d = dot(normal, viewDir)", "fresnel, facing mask, upper-facing mask, light alignment", "Quên normalize vector làm dot sai độ lớn."),
        ("Cross Product", "Tạo vector vuông góc với hai vector đầu vào.", "right = cross(up, forward)", "tangent helper, hướng phụ, frame local", "Ít dùng hơn dot nhưng quan trọng khi cần hướng vuông góc thật."),
        ("Normalize", "Đưa vector về độ dài 1, giữ hướng.", "dir = normalize(target - source)", "direction field, flow direction, hit ripple direction", "Normalize vector gần zero có thể không ổn; cần biết data có hợp lệ không."),
        ("Distance", "Đo khoảng cách giữa hai vị trí.", "dist = distance(worldPos, hitPos)", "hit ripple, sphere mask, aura radius, proximity fade", "Dùng world distance cho object nhỏ/lớn mà không scale-aware sẽ khó tái sử dụng."),
        ("Length", "Đo độ dài vector từ gốc.", "r = length(uv - 0.5)", "radial mask, portal, shockwave, circle", "Không center UV trước khi length thì vòng tròn lệch."),
    ]
    for row in vector_nodes:
        term_entry(doc, *row)
    callout(doc, "Space bug rất phổ biến", [
        "Nếu pattern trượt khi object di chuyển: có thể bạn dùng world space thay vì object/local space.",
        "Nếu mask đổi khi camera xoay: có thể bạn đang phụ thuộc view/screen space.",
        "Nếu radial mask lệch: kiểm tra UV có được center về 0.5 hay chưa.",
    ], fill=LIGHT_RED, color=RED)

    h1(doc, "8. Periodic motion, Atan2 và Polar UV")
    term_entry(doc, "Sine / Cosine", "Biến thời gian thành nhịp lặp mềm.", "pulse = sin(time * speed) * 0.5 + 0.5", "emissive pulse, breathing aura, flicker có nhịp", "Dùng sine quá đều làm effect máy móc; thêm noise hoặc random để tự nhiên hơn.")
    term_entry(doc, "Atan2", "Tính góc từ vector 2D với quadrant đúng.", "angle = atan2(y, x)", "portal swirl, radial wipe, slash arc, circular scan", "Dùng atan(y/x) thay atan2 dễ sai quadrant.")
    term_entry(doc, "Polar UV", "Chuyển UV x/y thành radius và angle.", "polar = float2(angle / TWO_PI, length(uv - center))", "vortex, black hole, magic ring, circular flow", "Nếu không xử lý seam, texture có thể bị đường nối ở góc wrap.")
    code(doc, [
        "centered = UV - 0.5",
        "radius   = length(centered)",
        "angle    = atan2(centered.y, centered.x)",
        "polarUV  = float2(angle / TWO_PI, radius)",
        "swirlUV  = polarUV + float2(time * speed + radius * twist, 0)",
    ], "Công thức polar cơ bản")

    part(doc, "Phần 3 - Các hệ material VFX quan trọng")
    h1(doc, "9. UV motion, Panner, Rotator, Flow Map")
    body(doc, "Rất nhiều VFX không thật sự mô phỏng chuyển động; chúng di chuyển UV. Khi texture sample ở vị trí UV khác theo thời gian, mắt người thấy pattern đang chảy, cuộn, xoay hoặc cháy.")
    for item in [
        "TexCoord là gốc kiểm soát scale/density của texture.",
        "Panner là UV + Time * Speed, dùng cho flow tuyến tính.",
        "Rotator xoay UV quanh pivot, dùng cho vòng phép, swirl, energy ring.",
        "Flow map dùng RG texture làm vector đẩy UV, cho motion giống chất lỏng hơn panner đơn giản.",
        "Nên tách motion lớn và detail nhỏ: một layer chậm cho direction, một layer nhanh cho breakup.",
    ]:
        bullet(doc, item)
    code(doc, [
        "uv_main   = UV * MainTiling + Time * MainSpeed",
        "uv_detail = UV * DetailTiling + Time * DetailSpeed",
        "mask      = TextureA(uv_main).r * TextureB(uv_detail).r",
    ], "Layer UV motion")
    callout(doc, "Bài tập", [
        "Tạo 2 panner ngược hướng nhau trên cùng noise. Blend bằng Multiply hoặc Screen. Quan sát motion bớt rẻ tiền hơn một panner đơn.",
        "Đổi chỉ Tiling trước, sau đó đổi Speed. Hãy ghi lại cảm giác visual khác nhau.",
    ], fill=LIGHT_GOLD, color=GOLD)

    h1(doc, "10. Mask grammar, threshold, dissolve, edge band")
    body(doc, "Mask grammar là phần quan trọng nhất để làm material VFX có kiểm soát. Mask không chỉ là alpha; nó là control signal. Một mask tốt cho phép bạn quyết định vùng nào sáng, vùng nào mờ, vùng nào biến mất, vùng nào là viền nóng.")
    code(doc, [
        "Mask thấp  = vùng biến mất hoặc yếu",
        "Mask cao   = vùng còn hiện hoặc mạnh",
        "Threshold tăng dần -> dissolve chạy qua bề mặt",
        "Edge band  = vùng nằm sát ngưỡng threshold",
    ], "Hình dung dissolve")
    code(doc, [
        "visible = smoothstep(t, t + softness, mask)",
        "edgeA   = smoothstep(t - width, t, mask)",
        "edgeB   = smoothstep(t, t + softness, mask)",
        "edge    = saturate(edgeA - edgeB)",
    ], "Dissolve + edge band")
    for item in [
        "Noise quyết định đường dissolve organic hay graphic.",
        "Gradient quyết định hướng dissolve: từ dưới lên, từ tâm ra, từ hit point lan ra.",
        "Threshold là thời gian hoặc progression.",
        "Softness là độ mềm biên.",
        "Edge width là độ dày vùng phát sáng.",
    ]:
        bullet(doc, item)

    h1(doc, "11. Emissive, opacity, alpha, blend mode")
    body(doc, "VFX thường sống trong Emissive và Opacity nhiều hơn BaseColor/Roughness. Nhưng blend mode quyết định giá trị đó được nhìn như thế nào trên scene.")
    for item in [
        "Additive: cộng sáng vào scene. Rất hợp glow, spark, flare, magic core. Dễ mất shape trên nền sáng.",
        "Translucent: alpha mềm. Hợp smoke, fire card chính, shield, glass. Dễ overdraw và sorting issue.",
        "Masked: cắt cứng nhưng rẻ hơn translucent. Hợp stylized cutout, decal/mesh pattern rõ.",
        "Opaque: ít dùng cho sprite mềm, nhưng tốt cho mesh material cần depth/lighting chuẩn.",
    ]:
        bullet(doc, item)
    callout(doc, "Lỗi thường gặp", [
        "Nghĩ rằng tăng opacity sẽ làm additive fade tốt hơn. Additive chủ yếu đọc qua emissive/intensity.",
        "Dùng translucent lớn chồng nhiều lớp smoke khiến overdraw tăng mạnh.",
        "Dựa vào bloom để đọc shape. Bloom đẹp nhưng shape phải đọc được cả khi bloom yếu.",
    ], fill=LIGHT_RED, color=RED)

    h1(doc, "12. Fresnel, rim, shell, forcefield")
    body(doc, "Fresnel làm bề mặt sáng hơn ở góc nghiêng so với camera. Trong VFX, nó là node nền tảng cho shield, aura, ghost, hologram, energy shell. Có thể dùng node Fresnel có sẵn hoặc tự build bằng dot(normal, viewDir).")
    code(doc, [
        "facing = dot(normalize(NormalWS), normalize(ViewDirWS))",
        "rim    = pow(1 - saturate(facing), FresnelPower)",
        "edge   = rim * EdgeColor * EdgeIntensity",
    ], "Manual fresnel")
    for item in [
        "Power thấp tạo rim rộng, dễ thành full surface glow.",
        "Power cao tạo rim mỏng, có thể mất ở camera xa.",
        "Forcefield cần rim mạnh nhưng center vẫn còn opacity floor để không biến mất ở front view.",
        "Pattern như hex/scanline/noise chỉ nên là layer phụ; silhouette shell phải đọc trước.",
    ]:
        bullet(doc, item)

    h1(doc, "13. DepthFade, SceneDepth, PixelDepth, soft particles")
    body(doc, "Depth là cây cầu giữa effect và world. Nếu không có depth fade, sprite smoke hoặc fire card cắt vào mặt đất/tường rất giả. DepthFade làm opacity mềm khi particle gần geometry.")
    code(doc, [
        "sceneDepth  = độ sâu geometry đã render",
        "pixelDepth  = độ sâu pixel hiện tại của particle/material",
        "diff        = sceneDepth - pixelDepth",
        "softOpacity = saturate(diff / FadeDistance)",
    ], "Hình dung soft particle")
    for item in [
        "FadeDistance nhỏ: intersection gắt hơn, giữ shape tốt hơn.",
        "FadeDistance lớn: mềm hơn, nhưng có thể làm effect mờ quá sớm.",
        "CameraDepthFade xử lý khi camera xuyên gần particle, tránh card cắt vào camera.",
        "SceneDepth/PixelDepth cũng dùng cho shockwave, waterline, portal intersection, contact glow.",
    ]:
        bullet(doc, item)

    h1(doc, "14. SubUV/flipbook và derived 0-1 UV")
    body(doc, "Flipbook lưu nhiều frame trong một atlas. Niagara Sprite Renderer hoặc material function chọn frame theo lifetime. Đây là workflow chính cho fire/smoke/explosion realistic hoặc stylized được bake từ AE/Houdini/EmberGen/Unreal.")
    code(doc, [
        "Atlas 4x4:",
        "+----+----+----+----+",
        "|  0 |  1 |  2 |  3 |",
        "|  4 |  5 |  6 |  7 |",
        "|  8 |  9 | 10 | 11 |",
        "| 12 | 13 | 14 | 15 |",
        "+----+----+----+----+",
    ], "Atlas frame")
    body(doc, "Derived 0-1 UV nghĩa là lấy UV của frame hiện tại rồi đưa về lại 0-1 cục bộ. Điều này rất quan trọng nếu material muốn tính radial mask, edge fade hoặc distortion trong từng frame.")
    for item in [
        "Không derive: UV 0.5,0.5 là tâm của toàn atlas.",
        "Có derive: UV 0.5,0.5 là tâm của frame đang phát.",
        "Không derive: radial mask bị lệch ở frame góc/cạnh.",
        "Có derive: material xử lý mỗi frame như texture riêng.",
    ]:
        bullet(doc, item)

    h1(doc, "15. WPO, VertexNormalWS, mesh deformation")
    body(doc, "World Position Offset di chuyển vertex ở shader. Với VFX, WPO dùng để làm mesh phồng, flutter, dissolve peel, forcefield pulse, boundary wobble. Đây là khác biệt lớn giữa material chỉ đổi pixel và material đổi silhouette.")
    code(doc, [
        "offset = VertexNormalWS * PulseAmount * mask",
        "WorldPositionOffset = offset",
    ])
    for item in [
        "Dùng WPO khi silhouette cần sống, không chỉ texture bên trong.",
        "Dùng VertexNormalWS để đẩy ra ngoài theo normal.",
        "Dùng local/object position để tạo gradient ổn định theo mesh.",
        "Cẩn thận bounds: WPO lớn có thể bị culling nếu bounds không đủ.",
        "Đừng dùng WPO để giải quyết vấn đề mà panner/flipbook đã làm tốt hơn.",
    ]:
        bullet(doc, item)

    h1(doc, "16. Texture authoring và channel packing")
    body(doc, "Trong VFX, texture không chỉ là hình. Nó là data. Một texture RGBA có thể chứa 4 mask khác nhau: emissive, opacity, occlusion, motion, detail noise. Channel packing giúp giảm số texture fetch và memory, nhưng cần naming rõ để không quên kênh nào làm gì.")
    for item in [
        "R: emissive mask, primary noise, crack mask.",
        "G: opacity, occlusion, secondary breakup.",
        "B: detail mask, rim support, thickness.",
        "A: alpha hoặc mask quan trọng; lưu ý alpha có thể tăng memory tùy compression.",
        "Normal atlas nên dùng cho distortion/lit smoke riêng; đừng nhét vào flame unlit rồi kỳ vọng lighting thật.",
    ]:
        bullet(doc, item)
    callout(doc, "Nguyên tắc production", [
        "Nếu texture sheet đã mang motion chính, đừng cố fake motion đó bằng panner.",
        "Nếu effect chạy trên rất nhiều particle, bake noise phức tạp thành texture thay vì procedural nặng.",
        "Luôn document ý nghĩa channel trong tên parameter hoặc comment.",
    ], fill=LIGHT_TEAL)

    part(doc, "Phần 4 - Recipe production")
    recipe(doc, "17. Additive flare / glow", "Tạo đốm sáng, flare, spark, magic mote hoặc core phát sáng đọc rõ trong thời gian ngắn.",
           ["flare = Texture(FlareTexture, UV)", "mask = pow(flare.r, Exponent)", "color = ParticleColor.rgb * Tint * Intensity", "Emissive = color * mask", "Opacity hoặc alpha support = mask * ParticleColor.a * CameraFade"],
           ["Intensity: độ sáng/bloom.", "Exponent: độ gắt của core.", "Tint/Color: màu theo ability.", "CameraFadeLength: tránh cắt gần camera.", "UseParticleColor: cho Niagara điều khiển color/alpha."],
           ["Exponent quá cao làm flare chỉ còn chấm nhỏ.", "Additive trên nền sáng dễ mất shape.", "Không nhân ParticleColor làm Niagara Color Over Life không có tác dụng."],
           ["Preview texture alpha/RGB riêng.", "Tắt bloom tạm để kiểm tra shape thật.", "Đổi ParticleColor alpha trong Niagara xem material có fade theo không."],
           ["Giữ texture sample ít.", "Dùng additive cho accent nhỏ; nếu card lớn cần fade thật, cân nhắc translucent.", "Pack nhiều flare mask vào một atlas nếu dùng nhiều biến thể."],
           ["Tạo 3 instance: soft glow, sharp spark, colored magic mote. Chỉ chỉnh Intensity/Exponent/Tint."])

    recipe(doc, "18. Dissolve / reveal / erosion", "Cho mesh hoặc sprite biến mất/xuất hiện bằng noise/gradient có viền phát sáng.",
           ["mask = Noise hoặc Gradient", "visible = smoothstep(Amount, Amount + Softness, mask)", "edge = smoothstep(Amount - EdgeWidth, Amount, mask) - visible", "OpacityMask/Opacity = visible", "Emissive += edge * EdgeColor * EdgeIntensity"],
           ["Amount: tiến trình dissolve.", "Softness: độ mềm vùng chuyển.", "EdgeWidth: độ dày viền.", "EdgeColor/Intensity: màu và lực viền.", "NoiseScale: kích thước mảng erosion."],
           ["Noise quá nhỏ làm visual rối.", "Edge không clamp/saturate gây âm hoặc cháy.", "Dissolve theo world space làm pattern trượt trên character."],
           ["Visualize mask grayscale trước.", "Animate Amount chậm 0-1 để xem edge band.", "Test object scale khác nhau để biết mask có scale-aware không."],
           ["Dùng packed noise thay vì procedural nặng nếu nhiều mesh.", "Masked rẻ hơn translucent nhưng biên cứng.", "Expose Amount từ Niagara/Blueprint thay vì duplicate timing."],
           ["Làm dissolve từ dưới lên bằng local Z gradient, sau đó multiply thêm noise để biên organic."])

    recipe(doc, "19. Fire card", "Làm flame card stylized hoặc semi-realistic bằng shape, UV motion, breakup, color ramp và opacity.",
           ["shape = FlameShapeTexture(UV).r", "flowUV = UV * Tiling + Time * Speed", "breakup = Noise(flowUV).r", "flameMask = shape * smoothstep(Threshold, Threshold + Softness, breakup)", "Emissive = ColorRamp(flameMask) * Intensity", "Opacity = flameMask * EdgeFade"],
           ["SpeedU/V: hướng chuyển động.", "U/V Scale: mật độ texture.", "FlameIntensity: lực emissive.", "ColorGain/Exponent: contrast màu.", "EdgeFade: mềm biên card.", "DistortionScale: mức heat/turbulence."],
           ["Card boundary lộ vì thiếu edge fade.", "Noise motion ngang quá nhiều làm lửa giống nước.", "Opacity và emissive không cùng mask làm viền sáng bị tách kỳ."],
           ["Tắt color ramp, xem mask trắng đen.", "Kiểm tra UV speed bằng texture checker.", "Xem ở camera xa để biết flame có đọc shape không."],
           ["Dùng flipbook cho motion phức tạp; panner chỉ nên là support.", "Giữ card vừa đủ size để giảm overdraw.", "Tách distortion pass nếu heat haze nặng."],
           ["Tạo 2 layer: flame core nhanh, outer flame chậm. So sánh cảm giác chiều sâu."])

    recipe(doc, "20. Smoke flipbook", "Dùng atlas smoke đã bake để tạo khói mềm, có lifetime, alpha, depth fade và color over life.",
           ["sample = ParticleSubUV(SmokeAtlas)", "alpha = pow(sample.a hoặc luminance(sample.rgb), AlphaPower)", "alpha *= ParticleColor.a", "Opacity = DepthFade(alpha, FadeDistance)", "Emissive rất thấp hoặc BaseColor nếu lit smoke"],
           ["AlphaPower: contrast khói.", "FadeDistance: mềm giao với world.", "CameraFade: mềm gần camera.", "Tint: màu khói.", "SubUV grid: số hàng/cột atlas."],
           ["Dùng additive cho smoke làm khói phát sáng như lửa.", "SubUV grid sai làm frame nhảy lung tung.", "Alpha quá crush làm smoke bị bẩn và gắt."],
           ["Dùng NumberGrid/debug atlas để kiểm tra frame.", "Tắt depth fade để phân biệt lỗi alpha và lỗi intersection.", "Xem particle size và overdraw trong gameplay camera."],
           ["Smoke lớn thường overdraw nặng.", "Giảm spawn count/size ở xa.", "Dùng atlas compression và resolution hợp lý."],
           ["Tạo smoke puff 8x8, chỉnh AlphaPower 1.2, 2.0, 3.0 và ghi cảm giác visual."])

    recipe(doc, "21. Portal / ring / black hole", "Tạo vòng năng lượng, cổng dịch chuyển hoặc hố xoáy bằng radial mask, polar UV, noise flow và depth fade.",
           ["centered = UV - 0.5", "r = length(centered)", "angle = atan2(centered.y, centered.x)", "polarUV = float2(angle / TWO_PI, r)", "noise = Noise(polarUV * Scale + Time * Flow)", "ring = smoothstep(Radius - Width, Radius, r) - smoothstep(Radius, Radius + Width, r)", "Emissive = ring * noise * Color * Intensity"],
           ["RingRadius: bán kính.", "RingThickness: độ dày.", "RingSoftness: mềm biên.", "FlowSpeed: tốc độ xoáy.", "NoiseScale: độ rối.", "DepthFadeDistance: mềm giao scene."],
           ["Portal thành đĩa đặc thay vì ring.", "Noise mạnh hơn silhouette.", "Một panner đơn làm motion rẻ tiền.", "Không xử lý seam polar tạo đường nối."],
           ["Tắt noise, chỉ xem ring shape.", "Xem front và three-quarter shot.", "Animate flow chậm để kiểm tra hướng xoáy."],
           ["Tách portal rim và interior thành 2 layer nếu hero effect.", "Distortion nên là support pass riêng.", "Dùng texture noise thay vì procedural nếu nhiều portal."],
           ["Build ring không noise trước. Khi silhouette đọc rõ, thêm hai noise flow ngược hướng."])

    recipe(doc, "22. Forcefield / shield", "Tạo vỏ năng lượng mỏng, trong suốt, có rim mạnh, pattern phụ và hit ripple.",
           ["rim = pow(1 - dot(NormalWS, ViewDirWS), EdgePower)", "baseShell = OpacityFloor + rim * RimOpacity", "pattern = HexOrNoise(World/ObjectUV)", "hit = ripple(distance(WorldPos, HitPos), Time)", "Emissive = rim * EdgeColor * EdgeIntensity + pattern * PatternIntensity + hit * HitColor", "Opacity = saturate(baseShell + pattern * PatternOpacity + hit)"],
           ["EdgePower: độ mỏng rim.", "OpacityFloor: giữ front view không biến mất.", "PatternContrast: lực pattern.", "HitPosition/HitTime: dữ liệu ripple.", "DepthFadeDistance: giao scene."],
           ["Center quá opaque thành tường đặc.", "Front view biến mất hoàn toàn.", "Pattern chiếm hero read thay vì shell.", "Hit ripple không có data path rõ."],
           ["Tắt pattern, chỉ kiểm tra fresnel shell.", "Kiểm tra front/grazing/three-quarter.", "Gửi hit position cố định để debug ripple."],
           ["Pattern texture rẻ hơn procedural phức tạp.", "Nhiều hit cùng lúc cần render target hoặc giới hạn số hit.", "Translucency lớn cần kiểm tra overdraw."],
           ["Làm shield sphere có rim + opacity floor. Sau đó thêm scanline hoặc hex pattern rất nhẹ."])

    recipe(doc, "23. Energy beam / ribbon", "Tạo beam, ribbon streak, lightning line hoặc energy ray bằng UV theo chiều dài, core, edge và flow.",
           ["u = UV.x  // chiều dài beam", "v = abs(UV.y - 0.5) * 2  // độ dày", "core = 1 - smoothstep(CoreWidth, 1, v)", "tailFade = smoothstep(0, HeadSoftness, u) * (1 - smoothstep(TailStart, 1, u))", "flow = Noise(float2(u * Tiling - Time * Speed, v)).r", "Emissive = core * tailFade * flow * Color * Intensity"],
           ["CoreWidth: độ dày lõi.", "EdgeSoftness: mềm biên.", "FlowSpeed: tốc độ chạy dọc beam.", "Tiling: mật độ streak.", "Head/Tail fade: đầu đuôi."],
           ["Beam đều từ đầu đến đuôi, thiếu head/tail hierarchy.", "Noise chạy ngang làm beam yếu direction.", "Ribbon quá rộng gây overdraw và mất lực."],
           ["Xem UV debug đỏ/xanh để biết U/V đúng hướng.", "Tắt flow, chỉ kiểm tra core/tail.", "Xem beam ở gameplay camera."],
           ["Dùng ribbon renderer cho trail động.", "Giữ material đơn giản, để Niagara quyết định path/timing.", "Tách impact flare ở đầu beam thay vì nhồi vào cùng graph."],
           ["Tạo beam có core trắng mỏng, edge màu, noise chạy theo U. Thêm tail fade."])

    recipe(doc, "24. Projectile core / shell", "Tạo đạn phép/energy bolt có core sáng, shell rim, motion direction và support particles.",
           ["core = Sphere/RadialMask(UV hoặc local position)", "rim = Fresnel hoặc radial edge", "flow = panning noise theo hướng di chuyển", "Emissive = core * CoreColor * CoreIntensity + rim * RimColor * RimIntensity", "Opacity = saturate(coreOpacity + rimOpacity)"],
           ["CoreIntensity, RimIntensity, FlowSpeed, DistortionAmount, FresnelPower, Opacity."],
           ["Projectile chỉ là glow blob không có direction.", "Core quá to che hết shell.", "Không có trail/support particle nên thiếu tốc độ."],
           ["Xem silhouette ở small size.", "Tắt bloom để kiểm tra core/shell.", "Test nhiều màu team/ability."],
           ["Projectile thường xuất hiện nhiều; graph nên gọn.", "Dùng material instance cho rarity/color.", "Trail/ribbon riêng rẻ hơn nhồi tất cả vào projectile material."],
           ["Build projectile 3 layer: core, shell fresnel, trail ribbon. Đổi màu bằng một parameter."])

    recipe(doc, "25. Ice, lava, frosted glass, wet organic", "Nhóm material conceptual giúp mở rộng tư duy từ VFX energy sang bề mặt có tính chất vật liệu.",
           ["Ice = fresnel + refraction + crack mask + cool color + roughness variation", "Lava = dark crust mask + emissive crack + noise erosion + heat color ramp", "FrostedGlass = translucency/refraction + roughness/noise + depth softness", "WetOrganic = specular/wet highlight + vein/noise mask + subsurface-like tint"],
           ["CrackScale, EmissiveCrackIntensity, FresnelPower, RefractionStrength, RoughnessVariation, Wetness."],
           ["Lava crack không có dark crust nên thành energy texture.", "Ice quá emissive thành magic shield thay vì băng.", "Glass thiếu roughness/noise nên quá sạch.", "Organic wet quá glossy thành nhựa."],
           ["Tách mask vật liệu và mask emissive.", "Xem ở ánh sáng khác nhau.", "So sánh bản unlit và lit nếu dùng surface material."],
           ["Không dùng translucency/refraction nếu không thật sự cần.", "Pack crack/roughness/noise vào channel.", "Dùng LOD/material instance cho platform thấp."],
           ["Làm lava bằng một noise mask: vùng thấp là crust tối, vùng cao là crack emissive."])

    part(doc, "Phần 5 - Đọc material trong VFXSamplePack")
    h1(doc, "26. Case study: M_Flare")
    body(doc, "M_Flare là bài nhập môn tốt vì nó chứa tư duy VFX sprite rất rõ: texture flare, particle color, intensity, exponent, additive blend, camera/depth fade. Đây là material nên học đầu tiên trước khi qua fire/smoke phức tạp.")
    for item in [
        "Visual purpose: tạo flare/spark/glow đọc nhanh, thường dùng Additive + Unlit.",
        "Texture lane: Flare Texture cung cấp shape sáng.",
        "Color lane: ParticleColor và Tint/Desaturation quyết định màu.",
        "Contrast lane: Exponent làm core gắt hoặc mềm.",
        "Fade lane: Fade Distance, Camera Fade Length/Offset giúp tránh clipping.",
        "Parameter nên chỉnh: Intensity, Exponent, Desaturation, Fade Distance.",
    ]:
        bullet(doc, item)
    callout(doc, "Bài tập M_Flare", [
        "Đặt Intensity 0.5, 2, 8 và quan sát bloom/độ cháy.",
        "Đặt Exponent 0.8, 2, 5 và quan sát core rộng/hẹp.",
        "Tắt ParticleColor trong Niagara nếu có thể, để thấy vì sao material cần data link.",
    ], fill=LIGHT_GOLD, color=GOLD)

    h1(doc, "27. Case study: M_Mesh_Add")
    body(doc, "M_Mesh_Add là ví dụ tốt cho mesh VFX: additive unlit, noise mask, Z axis gradient và WPO. Nó không chỉ tô màu bề mặt; nó có thể dùng world/local data để tạo boundary, marker hoặc mesh overlay.")
    for item in [
        "Visual purpose: mesh additive overlay, boundary/marker/tech grid.",
        "Noise Texture: T_Hermite2Noise_CenteredTiling tạo breakup.",
        "Noise UV Tile/Speed: scale và motion của noise.",
        "Noise Mask Amount/Exponent: độ mạnh và contrast của mask.",
        "Z Axis Gradient Amount/Exponent: fade theo chiều cao hoặc trục Z.",
        "WPO/Expand/Flatten: thay đổi silhouette/mesh shape.",
        "Case production: dùng cho boundary cylinder/sphere/box và marker target.",
    ]:
        bullet(doc, item)
    callout(doc, "Câu hỏi khi đọc graph", [
        "Mask chính đến từ noise hay Z gradient?",
        "Particle/Niagara có gửi color hoặc parameter vào không?",
        "WPO có cần thiết cho visual này, hay chỉ là support?",
    ], fill=LIGHT_TEAL)

    h1(doc, "28. Case study: M_FlamesCard")
    body(doc, "M_FlamesCard cho thấy một flame card non-flipbook có nhiều lane: shape texture, color gain/exponent, opacity exponent, UV speed/scale, edge fade và distortion. Đây là material tốt để học cách tách opacity, emissive và distortion.")
    for item in [
        "Shape lane: Flame Shape Texture quyết định silhouette lửa.",
        "Motion lane: Speed U/V và U/V Scale điều khiển UV panning.",
        "Color lane: Color Gain, Color Exponent, Flame Intensity tạo độ nóng.",
        "Opacity lane: Opacity Exponent và Edge Fade giữ biên card không lộ.",
        "Distortion lane: Distortion Texture + Distortion Scale tạo turbulence/heat.",
        "Debug: tắt distortion trước, chỉ xem flame mask; sau đó bật color; cuối cùng bật distortion.",
    ]:
        bullet(doc, item)
    callout(doc, "Lỗi thường gặp", [
        "Chỉnh màu khi vấn đề thật sự là mask shape.",
        "Tăng distortion quá mạnh làm lửa mất silhouette.",
        "Quên edge fade nên card boundary bị vuông.",
    ], fill=LIGHT_RED, color=RED)

    h1(doc, "29. Case study: M_SubUV_Utilities")
    body(doc, "M_SubUV_Utilities và NumberGrid là bộ học/debug SubUV. NumberGrid không phải texture VFX đẹp; nó là texture để thấy frame nào đang được sample. Khi nhìn số nhảy đúng, bạn biết renderer/material đang đọc atlas đúng.")
    for item in [
        "NumberGrid: texture debug frame.",
        "NS_SubUV_Utilities: Niagara dùng SubUVAnimation để đổi frame.",
        "M_SubUV_Utilities: material hiển thị frame được renderer chọn.",
        "Derived 0-1 UV: giúp material xử lý mask trong từng frame như texture riêng.",
        "Ứng dụng thật: T_Explosion, T_FireBall, T_SmokePuff, T_ExplosionRoil dạng 8x8 hoặc 8x16.",
    ]:
        bullet(doc, item)
    code(doc, [
        "Renderer SubUV grid sai -> sample nhầm ô",
        "Material dùng atlas UV -> radial/edge fade lệch",
        "Derived frame UV -> local 0-1 cho frame hiện tại",
    ])

    h1(doc, "30. Checklist học và debug material")
    h2(doc, "Checklist đọc một material mới")
    for item in [
        "Material domain/blend mode/shading model là gì?",
        "Output nào được nối: Emissive, Opacity, OpacityMask, WPO, Refraction?",
        "Texture nào là shape, texture nào là noise, texture nào là data packed?",
        "UV có panner/rotator/flow/polar/SubUV không?",
        "Mask chính đi qua Step/SmoothStep/Power/CheapContrast nào?",
        "Niagara data link có ParticleColor/DynamicParameter/ParticleRelativeTime không?",
        "Có DepthFade/CameraDepthFade/SceneDepth để tương tác world không?",
        "Parameter nào artist nên chỉnh đầu tiên?",
    ]:
        bullet(doc, item)
    h2(doc, "Checklist debug")
    for item in [
        "Nếu effect không hiện: kiểm tra blend mode, opacity, particle color alpha, material binding.",
        "Nếu quá sáng: tắt bloom, giảm intensity, kiểm tra additive/emissive.",
        "Nếu lộ card: kiểm tra edge fade, opacity mask, depth fade.",
        "Nếu flipbook sai: kiểm tra atlas grid, SubUV renderer, frame index, ParticleSubUV.",
        "Nếu pattern trượt: kiểm tra coordinate space.",
        "Nếu quá nặng: kiểm tra overdraw, texture fetch, refraction, particle count, bounds.",
    ]:
        bullet(doc, item)

    part(doc, "Phần 6 - Workbook thực hành material")
    body(doc, "Phần này dùng để học như luyện đàn: mỗi bài lab chỉ tập trung một kỹ năng. Khi làm xong, bạn nên có một material instance hoặc một graph nhỏ và một đoạn ghi chú: mình đã điều khiển shape, mask, motion, color, opacity và cost như thế nào.")
    labs = [
        ("31. Lab - Master một mask grayscale", "Hiểu mask như control signal 0-1, không chỉ như ảnh trắng đen.",
         ["Tạo material Unlit đơn giản.", "Dùng một TextureSample noise hoặc Constant/Gradient tự tạo.", "Preview trên plane hoặc sphere."],
         ["Nối mask trực tiếp vào Emissive để nhìn giá trị.", "Thêm Power để tăng/giảm contrast.", "Thêm Saturate sau khi cộng/trừ mask.", "Dùng Lerp giữa đen và màu để thấy mask điều khiển blend."],
         ["Vùng nào là 0, vùng nào là 1.", "Power 0.5, 1, 2, 5 làm hình thay đổi ra sao.", "Mask có bị clip mất chi tiết không."],
         ["Đảo mask bằng OneMinus.", "Combine hai mask bằng Multiply, Add, Max.", "Dùng mask đó điều khiển cả opacity và emissive."],
         ["Nếu không hiểu graph, hãy tạm nối từng mask ra Emissive dạng grayscale. Đừng debug một graph màu khi chưa hiểu mask."]),
        ("32. Lab - Threshold cứng và mềm", "Phân biệt Step, SmoothStep và edge band trong dissolve.",
         ["Dùng noise texture hoặc radial gradient.", "Tạo ScalarParameter Amount, Softness, EdgeWidth."],
         ["visibleHard = Step(Amount, mask).", "visibleSoft = SmoothStep(Amount, Amount + Softness, mask).", "edge = SmoothStep(Amount - EdgeWidth, Amount, mask) - visibleSoft.", "Nối visible vào opacity, edge vào emissive."],
         ["Step có bị răng cưa không.", "Softness rộng làm biên mềm nhưng mất lực thế nào.", "EdgeWidth thay đổi độ dày viền ra sao."],
         ["Dissolve từ trái sang phải bằng UV.x.", "Dissolve từ dưới lên bằng ObjectPosition.Z.", "Dissolve từ tâm ra bằng Distance(UV, 0.5)."],
         ["Nếu edge không hiện, kiểm tra thứ tự subtract và saturate. Edge band là vùng nằm giữa hai ngưỡng, không phải toàn bộ mask."]),
        ("33. Lab - UV panning có chủ ý", "Tạo motion rẻ nhưng đọc được direction.",
         ["Dùng một noise tileable.", "Tạo SpeedU, SpeedV, Tiling parameters.", "Preview trên sprite/card."],
         ["uvA = UV * Tiling + Time * SpeedA.", "uvB = UV * DetailTiling + Time * SpeedB.", "sampleA * sampleB để tạo breakup.", "Dùng mask cuối vào emissive/opacity."],
         ["Một panner đơn có dễ nhìn rẻ không.", "Hai tốc độ khác nhau có tạo chiều sâu hơn không.", "Tiling lớn làm pattern mịn hay rối."],
         ["SpeedA lên, SpeedB ngang.", "Layer lớn chậm, detail nhỏ nhanh.", "Thêm Rotator nhẹ cho swirl."],
         ["Nếu motion không rõ hướng, giảm noise detail và tăng contrast của layer chính trước."]),
        ("34. Lab - Radial mask và ring band", "Tạo vòng tròn, shockwave, portal rim bằng distance.",
         ["Dùng UV trên plane.", "Center UV về 0.5.", "Tạo Radius, Width, Softness."],
         ["r = Length(UV - 0.5).", "inner = SmoothStep(Radius - Width, Radius, r).", "outer = SmoothStep(Radius, Radius + Softness, r).", "ring = inner - outer.", "Emissive = ring * Color * Intensity."],
         ["Ring có đều quanh tâm không.", "Width có thật sự độc lập với Radius không.", "Softness có làm mất silhouette không."],
         ["Animate Radius để thành shockwave.", "Multiply ring với noise để thành magic rim.", "Dùng Polar UV để flow chạy quanh vòng."],
         ["Nếu ring lệch, kiểm tra UV chưa center hoặc texture coordinate bị scale không đều."]),
        ("35. Lab - Fresnel shell", "Hiểu view-dependent rim cho shield/aura/ghost.",
         ["Dùng mesh sphere.", "Material Translucent hoặc Additive Unlit.", "Tạo EdgePower, EdgeIntensity, OpacityFloor."],
         ["rim = pow(1 - dot(NormalWS, ViewDirWS), EdgePower).", "opacity = OpacityFloor + rim * RimOpacity.", "emissive = rim * Color * EdgeIntensity.", "Nối opacity/emissive."],
         ["Front view có biến mất hoàn toàn không.", "Grazing angle có mạnh hơn không.", "Power cao/thấp thay đổi độ dày rim ra sao."],
         ["Thêm noise pattern nhân nhẹ với rim.", "Thêm scanline theo WorldPosition.Z.", "Thêm hit ripple bằng Distance(WorldPos, HitPos)."],
         ["Forcefield tốt cần center vẫn còn một chút opacity floor; nếu không front view sẽ mất shell."]),
        ("36. Lab - DepthFade soft particle", "Làm sprite mềm khi cắt vào mặt đất/tường.",
         ["Dùng sprite/card translucent.", "Đặt card cắt xuyên một mesh plane.", "Tạo FadeDistance parameter."],
         ["alphaBase = TextureAlpha * ParticleColor.a.", "alphaSoft = DepthFade(alphaBase, FadeDistance).", "Opacity = alphaSoft.", "Emissive/BaseColor tùy effect."],
         ["FadeDistance nhỏ/gắt và lớn/mềm khác nhau thế nào.", "Opacity có mất quá nhiều ở gần geometry không.", "Camera gần có clipping không."],
         ["Thêm CameraDepthFade.", "Dùng alphaSoft để giảm cả emissive.", "So sánh smoke và fire khi dùng cùng FadeDistance."],
         ["Nếu effect biến mất toàn bộ, FadeDistance có thể quá lớn hoặc alphaBase đã quá thấp."]),
        ("37. Lab - ParticleColor contract", "Đảm bảo Niagara điều khiển được màu và alpha của material.",
         ["Material sprite Unlit.", "Niagara emitter có Color Over Life hoặc Scale Color.", "Dùng ParticleColor node."],
         ["rgb = TextureRGB * ParticleColor.rgb * Intensity.", "alpha = TextureA * ParticleColor.a.", "Nối rgb vào Emissive, alpha vào Opacity.", "Chỉnh color/alpha trong Niagara."],
         ["Niagara tint có đổi màu material không.", "Alpha over life có fade thật không.", "Additive có cần alpha hay intensity fade không."],
         ["Dùng ParticleColor alpha nhân vào emissive cho additive.", "Thêm DynamicParameter để điều khiển exponent.", "So sánh material có/không ParticleColor."],
         ["Nếu Niagara color không tác dụng, 90% là material chưa nhân ParticleColor hoặc renderer không dùng đúng material."]),
        ("38. Lab - Dynamic Parameter 4 kênh", "Tạo cầu nối 4 float từ Niagara sang material.",
         ["Tạo DynamicParameter node.", "Đặt convention R/G/B/A rõ ràng.", "Trong Niagara set Dynamic Material Parameters."],
         ["R = EmissivePower.", "G = FadeDistance.", "B = DistortionStrength.", "A = AlphaPower.", "Dùng từng kênh trong graph."],
         ["Kênh nào điều khiển gì.", "Default khi Niagara không set có an toàn không.", "Có bị đổi tên lung tung giữa material không."],
         ["Dùng R để pulse intensity.", "Dùng A để làm smoke mềm/gắt theo lifetime.", "Dùng B để tăng distortion lúc impact."],
         ["DynamicParameter mạnh nhưng dễ thành hỗn loạn. Luôn ghi convention trong comment hoặc document."]),
        ("39. Lab - Channel packing", "Dùng một texture RGBA như nhiều mask dữ liệu.",
         ["Chuẩn bị texture có nhiều channel hoặc dùng NumberGrid/noise để test.", "Dùng ComponentMask R/G/B/A.", "Đặt tên parameter/channel rõ."],
         ["R -> emissive mask.", "G -> opacity mask.", "B -> breakup noise.", "A -> edge/support mask.", "Combine từng kênh có chủ đích."],
         ["Kênh nào có contrast tốt.", "Alpha có thật sự cần không.", "ComponentMask có nối đúng kênh không."],
         ["Đảo riêng kênh G.", "Dùng B làm distortion strength.", "Dùng R và G để tạo edge band."],
         ["Nếu visual sai nhưng texture đúng, rất có thể bạn đang đọc nhầm channel hoặc compression đổi dữ liệu."]),
        ("40. Lab - WPO pulse", "Tạo mesh phồng/thở bằng World Position Offset.",
         ["Dùng sphere hoặc mesh đơn giản.", "Material có WPO.", "Tạo PulseAmount, Speed, Mask."],
         ["pulse = sin(Time * Speed) * 0.5 + 0.5.", "offset = VertexNormalWS * pulse * PulseAmount * Mask.", "WorldPositionOffset = offset.", "Emissive/Opacity đọc theo cùng pulse nếu muốn."],
         ["Silhouette có chuyển động không.", "Bounds có bị culling không.", "Pulse có quá đều/máy không."],
         ["Thêm noise vào pulse.", "Mask chỉ phần trên mesh bằng local Z.", "Dùng hit ripple để pulse lan từ điểm va chạm."],
         ["WPO ảnh hưởng silhouette; hãy kiểm tra ở camera xa và nhiều góc, không chỉ material preview."]),
        ("41. Lab - Local vs World space", "Nhận diện lỗi pattern trượt hoặc scale sai.",
         ["Tạo mesh material có noise/gradient.", "Di chuyển, xoay, scale mesh.", "So sánh UV, local, world mapping."],
         ["UV mapping: texture theo unwrap.", "World mapping: pattern đứng yên trong world.", "Local/object mapping: pattern bám object.", "Ghi lại visual khi object di chuyển."],
         ["Pattern có bơi/swim không.", "Scale object có đổi density không.", "Camera có làm pattern đổi không."],
         ["Dissolve character bằng local height.", "World aligned snow/fog bằng world position.", "Object-space corruption bám theo mesh."],
         ["Coordinate-space bug thường nhìn giống UV bug. Hãy test bằng cách di chuyển object trước khi sửa texture."]),
        ("42. Lab - Distortion support pass", "Tách distortion khỏi hero color để dễ kiểm soát.",
         ["Tạo material distortion riêng.", "Dùng normal/noise panner.", "Không để nó gánh hero emissive."],
         ["distortUV = UV + NormalRG * DistortionStrength.", "Refraction/SceneColor offset tùy pipeline.", "Opacity support rất thấp.", "Hero material riêng lo shape/color."],
         ["Distortion có phá silhouette không.", "Có artifact ở cạnh mesh/scene không.", "Strength bao nhiêu là vừa."],
         ["Distortion mạnh tại impact, yếu ở falloff.", "Dùng radial mask điều khiển distortion.", "Tách heat haze cho fire."],
         ["Distortion là gia vị. Nếu nó là layer chính, effect dễ đọc như lỗi màn hình."]),
    ]
    for args in labs:
        lab(doc, *args)

    part(doc, "Phần 7 - Atlas các family material")
    body(doc, "ForgePilot Knowledge chia material VFX thành nhiều family. Đây là cách nghĩ production: không có một master material làm tất cả. Mỗi family có role, graph block và failure mode riêng.")
    family_page(doc, "43. Family - Sprite base / glow / mote",
                "Particle nhỏ, glow mềm, spark, ember, magic mote, flare phụ.",
                ["ParticleColor-driven tint/fade.", "Một texture mask hoặc radial procedural shape.", "Emissive và opacity đơn giản.", "Depth/camera fade nếu card lớn."],
                ["Tint, Intensity, AlphaPower, Softness, CameraFade, DepthFade."],
                ["Glow blob không có shape.", "Additive mất trên nền sáng.", "Không nhận alpha từ Niagara.", "Texture quá soft khiến không đọc ở gameplay camera."],
                "Family này nên rất gọn. Nếu cần motion phức tạp, dùng panner/flipbook nhẹ hoặc tách thành emitter khác.")
    family_page(doc, "44. Family - Beam / ribbon / streak",
                "Đường năng lượng có hướng rõ, thường chạy dọc theo ribbon hoặc beam UV.",
                ["UV theo chiều dài là hero axis.", "Core mỏng + edge mềm.", "Noise/panner chạy theo U.", "Head/tail fade tạo timing và direction."],
                ["CoreWidth, EdgeSoftness, FlowSpeed, Tiling, HeadFade, TailFade, Intensity."],
                ["Beam đều như thanh neon.", "Không có head/tail nên thiếu tốc độ.", "Noise chạy sai hướng.", "Ribbon quá rộng gây overdraw."],
                "Không ép sprite material thành beam. Beam cần directional flow và UV convention ổn định.")
    family_page(doc, "45. Family - Flipbook fire/smoke/explosion",
                "Motion chính nằm trong atlas frame, engine chỉ playback và điều khiển tint/fade.",
                ["ParticleSubUV hoặc flipbook function.", "SubUV grid đúng atlas.", "ParticleColor tint/alpha.", "Alpha/emissive shaping sau sample.", "DepthFade cho smoke/fire cards."],
                ["SubImageSize, PlaybackRate, AlphaPower, EmissivePower, FadeDistance, CameraFade."],
                ["Grid sai.", "Dùng TextureSample thường thay ParticleSubUV.", "Smoke quá emissive.", "Fire additive không fade opacity đúng."],
                "Niagara nên sở hữu timing/frame playback; material sở hữu sample, contrast, fade và polish.")
    family_page(doc, "46. Family - Dissolve / erosion / transform",
                "Bề mặt biến mất/xuất hiện theo mask, thường có viền năng lượng hoặc burn edge.",
                ["Base mask: noise/gradient/local space.", "Threshold Amount.", "Visible mask.", "Edge band.", "Optional WPO/peel/displacement."],
                ["Amount, Softness, EdgeWidth, EdgeIntensity, NoiseScale, Direction, WPOAmount."],
                ["Noise quá chi tiết làm mất direction.", "Edge band không bám threshold.", "Pattern world-space trượt trên character.", "Viền quá sáng che shape."],
                "Dissolve tốt phải có progression rõ. Noise chỉ làm đường biên sống hơn, không thay thế direction.")
    family_page(doc, "47. Family - Portal / radial ring",
                "Vòng năng lượng, summon circle, magic gate, black hole rim.",
                ["Radial space lane.", "Ring band bằng inner/outer thresholds.", "Noise flow radial-aware.", "Emissive rim là hero.", "Depth fade/interior/distortion là support."],
                ["Radius, Thickness, Softness, FlowSpeedA/B, NoiseScale, EmissiveColor, DistortionStrength."],
                ["Thành full disc.", "Noise mạnh hơn rim.", "Single panner rẻ tiền.", "Không có depth fade ở giao scene."],
                "Portal nên đọc silhouette trước. Khi rim chưa đẹp, đừng thêm interior hoặc distortion.")
    family_page(doc, "48. Family - Forcefield / hologram / shield",
                "Vỏ năng lượng mỏng, translucent, view-dependent, có pattern phụ.",
                ["Fresnel shell.", "Opacity floor để front view còn đọc.", "Pattern/scanline/hex rất tiết chế.", "Depth/contact fade.", "Optional hit ripple."],
                ["EdgePower, OpacityFloor, EdgeIntensity, PatternContrast, HitRadius, HitWidth, DepthFadeDistance."],
                ["Center thành tường opaque.", "Front view biến mất.", "Pattern dominate silhouette.", "Không có data path cho hit."],
                "Đừng để shield trở thành texture ồn. Rim và silhouette là visual chính.")
    family_page(doc, "49. Family - Projectile core / shell",
                "Đạn phép/energy bolt cần đọc được hướng, core, shell và tốc độ.",
                ["Core sáng.", "Shell/rim.", "Directional flow.", "Trail/ribbon support.", "Impact flare riêng."],
                ["CoreIntensity, ShellOpacity, FlowSpeed, FresnelPower, TrailFade, Distortion."],
                ["Glow blob không hướng.", "Core quá to.", "Thiếu trail.", "Material quá nặng cho nhiều projectile."],
                "Projectile nên chia layer: core material gọn, trail/ribbon riêng, impact riêng. Đừng nhồi tất cả vào một material.")
    family_page(doc, "50. Family - Ice / crystal",
                "Vật liệu lạnh, sắc, có crack, fresnel, refraction hoặc translucency nhẹ.",
                ["Cool base tint.", "Fresnel edge.", "Crack/noise mask.", "Roughness/refraction variation.", "Optional emissive magic edge."],
                ["IceTint, FresnelPower, CrackScale, RefractionStrength, RoughnessNoise, EdgeIntensity."],
                ["Quá emissive thành energy shield.", "Không có roughness variation nên như nhựa.", "Crack mask quá đều.", "Refraction quá mạnh gây artifact."],
                "Ice VFX cần cân bằng giữa material surface và magic. Nếu dùng trong gameplay nhiều, hạn chế refraction.")
    family_page(doc, "51. Family - Lava / molten crack",
                "Vỏ tối nứt ra ánh nóng, đọc bằng contrast giữa crust và emissive crack.",
                ["Dark crust mask.", "Emissive crack mask.", "Noise erosion.", "Heat color ramp.", "Optional WPO pulse."],
                ["CrackIntensity, CrustDarkness, NoiseScale, HeatRamp, PulseAmount, EdgeGlow."],
                ["Không có crust nên thành energy noise.", "Crack quá đều.", "Emissive cháy trắng mất màu.", "WPO quá mạnh làm mesh phồng giả."],
                "Lava mạnh ở value contrast: tối thật tối, crack thật nóng, nhưng giữ màu cam/đỏ thay vì trắng toàn bộ.")
    family_page(doc, "52. Family - Frosted glass / wet organic",
                "Nhóm bề mặt translucent hoặc glossy cần vật liệu đọc qua roughness, specular, refraction và mask.",
                ["Frosted glass: rough refraction + noise + soft opacity.", "Wet organic: specular/wet highlight + vein/noise + subsurface tint.", "Mask điều khiển vùng ướt/khô hoặc trong/đục."],
                ["Roughness, RefractionStrength, Wetness, VeinContrast, Opacity, FresnelPower."],
                ["Glass quá sạch.", "Wet organic thành nhựa.", "Refraction nặng mà không cần.", "Noise surface không theo object space."],
                "Các family này dễ tốn render hơn sprite VFX. Chỉ dùng feature đắt khi visual thật sự cần.")

    part(doc, "Phần 8 - Material Function và tư duy master material")
    h1(doc, "53. Khi nào dùng Material Function")
    body(doc, "Material Function không tự động rẻ hơn. Nó giúp graph dễ đọc, tái sử dụng và giảm lỗi copy-paste. Shader cost vẫn đến từ node bên trong function. Vì vậy đừng biến mọi phép Add/Multiply thành function; hãy dùng function khi logic đủ lớn hoặc dễ sai nếu dựng tay.")
    for item in [
        "Nên dùng function cho flipbook/SubUV helper, remap range, contrast, radial conversion, world aligned texture, blend mode phức tạp.",
        "Nên giữ inline các node nhỏ: Add, Multiply, Saturate, Power, ComponentMask, ParticleColor, DepthFade.",
        "Function tốt phải expose input rõ: Radius, Width, Softness, FlowSpeed, Intensity.",
        "Function tệ là hộp đen giấu hết control artist cần chỉnh.",
    ]:
        bullet(doc, item)
    code(doc, [
        "Inline tốt:  mask * intensity",
        "Function tốt: BuildRingBand(UV, Radius, Width, Softness)",
        "Function không tốt: MagicEverything(inputA, inputB) nhưng không biết bên trong làm gì",
    ])
    h1(doc, "54. Tư duy master material")
    body(doc, "Master material nên là khuôn logic cho một family, không phải một con quái vật chứa mọi family. Sprite glow, beam, flipbook smoke, portal, shield, dissolve nên có master riêng hoặc module function riêng. Khi master quá lớn, artist sợ chỉnh, shader compile nhiều variant, performance khó đoán.")
    for item in [
        "Chọn family trước: sprite, beam, flipbook, dissolve, shield, portal, surface.",
        "Expose parameter ít nhưng đúng: color, intensity, speed, scale, softness, fade, power.",
        "Đặt tên parameter theo visual purpose, không theo node math mơ hồ.",
        "Dùng Static Switch cẩn thận vì tạo shader variants.",
        "Dùng Material Instance cho màu/style, không sửa master mỗi effect.",
        "Comment graph theo lane: UV, Mask, Color, Opacity, WPO, Depth.",
    ]:
        bullet(doc, item)
    h1(doc, "55. Optimization sâu hơn cho VFX material")
    body(doc, "Optimization trong VFX thường không nằm ở một node đắt duy nhất mà là tổng hợp: overdraw, particle count, texture size, refraction, translucent sorting, instruction count và bounds. Một effect đẹp trong preview có thể thất bại khi spawn 50 lần trong combat.")
    for item in [
        "Overdraw: giảm card size, tighten alpha, giảm layer translucent chồng nhau.",
        "Texture fetch: pack channel, reuse sample, giảm normal/distortion texture nếu không cần.",
        "Instruction: bake noise, tránh procedural quá nặng, không dùng If tùy tiện.",
        "Memory: atlas resolution hợp lý, compression đúng, alpha channel có chủ đích.",
        "Niagara cost: spawn count, lifetime, collision, curl noise, GPU/CPU sim, bounds.",
        "Feature cost: refraction, lit translucency, shadow, scene color, custom depth đều cần lý do.",
    ]:
        bullet(doc, item)
    callout(doc, "Rule of thumb", [
        "Nếu effect xuất hiện một lần trong cinematic, bạn có thể chi nhiều hơn.",
        "Nếu effect spawn liên tục trên nhiều enemy/projectile, graph phải cực kỳ kỷ luật.",
        "Nếu người chơi chỉ thấy 0.2 giây, đừng trả cost cho detail không ai đọc được.",
    ], fill=LIGHT_GOLD, color=GOLD)

    part(doc, "Phần 9 - Phụ lục công thức nhanh")
    formulas = [
        ("Soft radial circle", ["r = length(UV - 0.5)", "circle = 1 - smoothstep(Radius, Radius + Softness, r)"]),
        ("Ring band", ["r = length(UV - 0.5)", "ring = smoothstep(Radius - Width, Radius, r) - smoothstep(Radius, Radius + Softness, r)"]),
        ("Fresnel manual", ["facing = dot(normalize(NormalWS), normalize(ViewDirWS))", "rim = pow(1 - saturate(facing), Power)"]),
        ("Panning noise mask", ["uv = UV * Tiling + Time * Speed", "mask = Texture(Noise, uv).r"]),
        ("Two-layer breakup", ["large = Noise(UV * 2 + Time * 0.1)", "detail = Noise(UV * 12 - Time * 0.4)", "mask = large * detail"]),
        ("Dissolve visible", ["visible = smoothstep(Amount, Amount + Softness, mask)"]),
        ("Dissolve edge", ["edge = smoothstep(Amount - EdgeWidth, Amount, mask) - smoothstep(Amount, Amount + Softness, mask)"]),
        ("Alpha power", ["alpha = pow(TextureAlpha, AlphaPower) * ParticleColor.a"]),
        ("Camera/depth fade stack", ["alpha = TextureAlpha * ParticleColor.a", "alpha *= CameraDepthFade(CameraFadeLength)", "Opacity = DepthFade(alpha, FadeDistance)"]),
        ("Hit ripple", ["d = distance(WorldPos, HitPos)", "ring = smoothstep(Radius - Width, Radius, d) - smoothstep(Radius, Radius + Width, d)"]),
        ("Polar swirl", ["centered = UV - 0.5", "r = length(centered)", "a = atan2(centered.y, centered.x)", "polarUV = float2(a / TWO_PI + Time * Speed + r * Twist, r)"]),
        ("WPO inflate", ["pulse = sin(Time * Speed) * 0.5 + 0.5", "WPO = VertexNormalWS * pulse * Amount * Mask"]),
    ]
    for title, lines in formulas:
        h2(doc, title)
        code(doc, lines)
        body(doc, "Cách dùng: nối công thức vào mask/emissive/opacity trước, sau đó expose parameter để biến nó thành control artist-friendly. Khi công thức chưa đọc rõ ở grayscale, đừng vội thêm màu hoặc bloom.")

    part(doc, "Phần 10 - Từ điển node chuyên sâu")
    body(doc, "Phần này dùng như sổ tay tra cứu khi mở Material Editor. Mỗi node không chỉ là một ô trong graph; nó là một loại động từ. Có node đưa dữ liệu vào, có node làm mask sắc hơn, có node đổi không gian, có node giúp material nói chuyện với Niagara. Khi học, đừng chỉ hỏi node này nối vào đâu. Hãy hỏi: nó đang thay đổi data theo hướng nào, dùng cho lane nào, và có lỗi production nào hay gặp.")
    deep_nodes = [
        (
            "56. Particle Color",
            "Cổng nhận màu và alpha từ Niagara hoặc Cascade vào material. Đây là cầu nối cơ bản nhất giữa particle system và shader.",
            [
                "Khi Niagara cần điều khiển color over life, alpha over life, random tint, hoặc fade từng particle.",
                "Khi một material được dùng cho nhiều emitter khác nhau và muốn emitter tự quyết màu.",
                "Khi sprite flipbook cần giữ texture gốc nhưng vẫn tint theo gameplay hoặc timing.",
            ],
            [
                "Không nên nhân Particle Color lung tung ở nhiều chỗ nếu chưa biết lane nào đang nhận tint.",
                "Với Additive, alpha có thể không đi vào Opacity như Translucent; vẫn cần dùng alpha để scale Emissive nếu muốn fade.",
                "Đừng quên kiểm tra Niagara renderer có Scale Color hoặc Color binding đúng không.",
            ],
            [
                "emissive = textureRGB * ParticleColor.rgb * Intensity",
                "opacity = textureA * ParticleColor.a",
                "additiveFade = emissive * ParticleColor.a",
            ],
            [
                "Tạo một material sprite chỉ có texture * ParticleColor. Trong Niagara, chỉnh Color over Life từ vàng sang đỏ và alpha từ 1 về 0.",
                "Lặp lại với Additive: đưa ParticleColor.a vào Emissive multiplier để thấy fade thật sự.",
            ],
        ),
        (
            "57. Dynamic Parameter",
            "Bốn kênh float để Niagara gửi thông số tùy biến vào material theo từng particle hoặc emitter.",
            [
                "Khi cần mỗi particle có FadeDistance, Power, distortion strength, frame blend, hoặc dissolve amount khác nhau.",
                "Khi một master material cần ít parameter nhưng emitter muốn animate trực tiếp trong Niagara.",
                "Khi muốn tránh tạo quá nhiều material instance chỉ để đổi một thông số runtime.",
            ],
            [
                "Không nên đổi ý nghĩa R/G/B/A tùy tiện giữa các material cùng family.",
                "Nếu control là global cho cả effect, Material Instance parameter có thể đơn giản hơn Dynamic Parameter.",
                "Tên kênh sai hoặc renderer chưa bind sẽ làm graph nhìn đúng nhưng effect không chạy.",
            ],
            [
                "R = PowerEmissive",
                "G = FadeDistance",
                "B = CameraFadeLength",
                "A = AlphaPower",
                "opacity = pow(alpha, A)",
            ],
            [
                "Dựng material flipbook có DynamicParameter.A điều khiển AlphaPower. Trong Niagara, animate A từ 0.8 tới 3.0 để thấy alpha mềm hoặc gắt.",
            ],
        ),
        (
            "58. Particle Relative Time",
            "Giá trị 0-1 đại diện lifetime của particle ngay trong material.",
            [
                "Khi timing nhỏ nằm trong shader: ring mở rộng, dissolve nội bộ, pulse theo tuổi particle.",
                "Khi không muốn Niagara phải expose quá nhiều curve cho một hiệu ứng phụ.",
                "Khi cần đồng bộ một mask trong material với vòng đời particle.",
            ],
            [
                "Không nên vừa animate alpha trong Niagara vừa animate cùng alpha bằng Particle Relative Time nếu không chủ đích.",
                "Với mesh hoặc non-particle material, node này không có cùng ý nghĩa.",
                "Dùng quá nhiều timing trong material có thể khiến emitter khó debug.",
            ],
            [
                "t = ParticleRelativeTime",
                "grow = smoothstep(0.0, 0.35, t)",
                "fade = 1 - smoothstep(0.7, 1.0, t)",
                "opacity = mask * grow * fade",
            ],
            [
                "Làm một flare tự nở rồi tắt chỉ bằng Particle Relative Time, không dùng alpha curve trong Niagara.",
            ],
        ),
        (
            "59. TextureCoordinate",
            "Nguồn UV gốc của mesh/sprite. Đây là bản đồ tọa độ trước khi bị pan, rotate, scale, polar hoặc distort.",
            [
                "Khi cần scale tiling của texture, đọc channel UV khác, hoặc xây UV lane rõ ràng.",
                "Khi debug xem texture bị sai do unwrap, tiling hay do panner/rotator.",
                "Khi làm atlas, decal, mesh strip, ribbon hoặc card flame.",
            ],
            [
                "Không nên scale UV bằng số rải rác nhiều nơi; expose Tiling hoặc gom vào UV lane.",
                "Sprite particle thường có UV 0-1 trên card; mesh có thể unwrap phức tạp hơn.",
                "Sai UV channel là lỗi rất âm thầm trong mesh material.",
            ],
            [
                "uv = TexCoord0 * Tiling + Offset",
                "sample = Texture.Sample(uv)",
            ],
            [
                "Mở một sprite material và thử Tiling 1, 2, 4. Sau đó dùng cùng texture trên mesh để thấy UV unwrap ảnh hưởng ra sao.",
            ],
        ),
        (
            "60. Panner",
            "Dịch UV theo thời gian để tạo cảm giác texture đang chảy.",
            [
                "Fire flow, smoke drift, energy crawl, water ribbon, shield noise, portal interior.",
                "Khi motion có thể giả bằng texture chạy thay vì mô phỏng thật.",
                "Khi cần layer nhiều tốc độ để phá cảm giác lặp.",
            ],
            [
                "Một panner đơn có thể nhìn rẻ nếu người xem thấy texture trượt như giấy.",
                "Đừng pan cả mask chính nếu silhouette cần đứng yên.",
                "Speed quá cao tạo shimmer và aliasing.",
            ],
            [
                "uv1 = UV * Tiling1 + Time * Speed1",
                "uv2 = UV * Tiling2 - Time * Speed2",
                "mask = noise(uv1) * noise(uv2)",
            ],
            [
                "Tạo energy card có hai noise panner ngược chiều. So sánh với một panner đơn.",
            ],
        ),
        (
            "61. Rotator",
            "Xoay UV quanh pivot để tạo swirl, vortex, ring motion hoặc biến thể texture.",
            [
                "Portal ring, magic circle, radial burst, rotating flare, decal năng lượng.",
                "Khi texture có tâm rõ ràng và motion xoay giúp visual đọc tốt hơn pan thẳng.",
                "Khi muốn random rotation theo particle để phá repetition.",
            ],
            [
                "Pivot sai làm texture quay lệch khỏi tâm.",
                "Rotator trên atlas/flipbook dễ làm sai frame nếu không chỉ áp vào UV frame local.",
                "Xoay noise quá đều có thể tạo cảm giác mechanical.",
            ],
            [
                "centered = UV - Pivot",
                "rotated = mul(centered, rotationMatrix(angle)) + Pivot",
                "sample = Texture(rotated)",
            ],
            [
                "Tạo ring mask rồi xoay noise layer ở bên trong. Thử Pivot 0.5,0.5 và Pivot 0,0 để thấy lỗi.",
            ],
        ),
        (
            "62. ParticleSubUV",
            "Node đọc frame atlas theo SubUV setup của sprite renderer. Nó để Niagara quản lý frame playback thay vì tự cắt atlas bằng math thủ công.",
            [
                "Fire/smoke/explosion flipbook mà renderer đã bật SubUV.",
                "Khi cần particle system điều khiển frame index, interpolation hoặc random start frame.",
                "Khi muốn material chỉ lo color, alpha, depth fade và shaping.",
            ],
            [
                "Không dùng ParticleSubUV nếu texture chỉ là static mask hoặc không có renderer SubUV.",
                "Sai SubImageSize trong Niagara sẽ đọc nhầm frame dù material đúng.",
                "Đừng pan UV gốc của atlas nếu chưa hiểu derived 0-1 UV trong từng frame.",
            ],
            [
                "rgb = ParticleSubUV.rgb",
                "alpha = ParticleSubUV.a",
                "emissive = rgb * ParticleColor.rgb * Intensity",
                "opacity = DepthFade(alpha * ParticleColor.a, FadeDistance)",
            ],
            [
                "Dùng NumberGrid 3x3 làm atlas test. Bật SubUV và kiểm tra particle có chạy 1-9 đúng thứ tự không.",
            ],
        ),
        (
            "63. ComponentMask và AppendVector",
            "Hai node nhỏ nhưng rất quan trọng để tách kênh và ghép dữ liệu vector.",
            [
                "Tách R/G/B/A từ packed texture: R mask, G noise, B edge, A alpha.",
                "Ghép UV từ hai scalar, ghép normal/detail vector, hoặc tạo float2 cho custom UV.",
                "Khi cần đọc data theo channel rõ ràng thay vì kéo cả vector vào mọi nơi.",
            ],
            [
                "Đừng giả định alpha luôn chứa opacity; nhiều texture pack alpha có thể là mask khác.",
                "Sai channel packing làm visual sai nhưng graph vẫn compile.",
                "Append sai thứ tự X/Y sẽ làm UV bị đảo hướng.",
            ],
            [
                "mask = PackedTexture.r",
                "noise = PackedTexture.g",
                "edge = PackedTexture.b",
                "uv = append(u, v)",
            ],
            [
                "Tạo một packed texture giả bằng NumberGrid hoặc noise nhiều kênh. Dùng ComponentMask để xem từng kênh ở Emissive.",
            ],
        ),
        (
            "64. DepthFade",
            "Làm mềm giao tuyến giữa translucent particle và geometry trong scene.",
            [
                "Smoke, fire, magic mist, water splash, glow card sát mặt đất.",
                "Khi card xuyên tường/sàn tạo cạnh cứng khó chịu.",
                "Khi cần VFX hòa vào môi trường mà không phải sculpt mesh.",
            ],
            [
                "FadeDistance quá lớn làm particle mất thân, nhìn như bị rỗng.",
                "DepthFade không thay thế được mask tốt; nó chỉ xử lý giao tuyến.",
                "Với opaque/masked path, cần hiểu giới hạn của depth/translucency trong engine.",
            ],
            [
                "softOpacity = DepthFade(OpacityInput, FadeDistance)",
                "Opacity = alpha * ParticleColor.a * softOpacity",
            ],
            [
                "Đặt smoke card cắt qua một cube. So sánh FadeDistance 1, 20, 100.",
            ],
        ),
        (
            "65. SceneDepth và PixelDepth",
            "Hai nguồn dữ liệu depth để so sánh pixel hiện tại với scene hoặc camera distance.",
            [
                "Soft particles nâng cao, intersection glow, water edge, portal cắt qua geometry.",
                "Khi DepthFade không đủ control và cần tự xây depth mask.",
                "Khi cần debug khoảng cách giữa surface đang vẽ và scene phía sau.",
            ],
            [
                "SceneDepth có ràng buộc theo blend mode và rendering path.",
                "Depth math dễ sai space: linear/nonlinear, world/camera, normalized/raw.",
                "Dùng depth quá nhiều trên translucent có thể tốn hoặc khó đoán.",
            ],
            [
                "scene = SceneDepth(UV)",
                "pixel = PixelDepth",
                "diff = scene - pixel",
                "intersection = saturate(diff / FadeDistance)",
            ],
            [
                "Tạo material debug xuất diff ra grayscale để học cách depth thay đổi khi camera tiến gần object.",
            ],
        ),
        (
            "66. WorldPosition",
            "Vị trí pixel hoặc vertex trong world space. Đây là nguồn dữ liệu để tạo mask theo không gian thật.",
            [
                "World-space gradient, dissolve từ vị trí va chạm, ripple theo hit point, mask theo độ cao.",
                "Khi UV của mesh không đáng tin hoặc muốn effect bám theo thế giới.",
                "Khi nhiều object cần cùng một field hoặc cùng hướng gradient.",
            ],
            [
                "World space có thể làm texture đứng yên khi object di chuyển, đôi khi đúng, đôi khi sai.",
                "Large world coordinates có thể gây precision issue trong vài setup.",
                "Không nên dùng world math phức tạp nếu object-space đơn giản là đủ.",
            ],
            [
                "heightMask = saturate((WorldPos.z - MinHeight) / Range)",
                "hitDist = distance(WorldPos, HitPosition)",
                "ring = smoothstep(r - w, r, hitDist) - smoothstep(r, r + w, hitDist)",
            ],
            [
                "Làm dissolve từ dưới lên bằng WorldPosition.z, sau đó thử xoay mesh để thấy khác biệt với ObjectPosition/Object space.",
            ],
        ),
        (
            "67. ObjectPosition và ObjectRadius",
            "Dữ liệu object-level giúp tạo gradient, radial mask hoặc normalization theo kích thước object.",
            [
                "Khi muốn effect scale theo object size thay vì theo world unit cứng.",
                "Khi làm shield shell, aura, mesh dissolve, impact pulse quanh tâm object.",
                "Khi cần object-space-ish control mà không phụ thuộc UV.",
            ],
            [
                "ObjectPosition là tâm bounds, không phải lúc nào cũng là tâm visual mong muốn.",
                "Bounds sai hoặc mesh có pivot lệch sẽ làm mask lệch.",
                "ObjectRadius thay đổi theo bounds, cần kiểm tra với LOD hoặc scale.",
            ],
            [
                "localish = (WorldPos - ObjectPosition) / ObjectRadius",
                "radial = length(localish)",
                "shell = 1 - smoothstep(Radius, Radius + Softness, radial)",
            ],
            [
                "Tạo aura shell trên hai mesh kích thước khác nhau và dùng ObjectRadius để giữ độ dày tương đối ổn định.",
            ],
        ),
        (
            "68. VertexNormalWS",
            "Vector pháp tuyến vertex ở world space, thường dùng để đẩy mesh hoặc tính hướng bề mặt.",
            [
                "World Position Offset inflate, shield pulse, dissolve shell, impact deformation đơn giản.",
                "Khi muốn mesh nở theo hướng bề mặt mà không cần sim.",
                "Khi tạo rim hoặc mask phụ theo hướng normal.",
            ],
            [
                "Vertex density thấp làm WPO méo thô.",
                "Normal bị hard edge có thể làm deformation tách mảng.",
                "WPO lớn cần kiểm tra bounds để tránh bị culling.",
            ],
            [
                "pulse = sin(Time * Speed) * 0.5 + 0.5",
                "WPO = VertexNormalWS * Amount * pulse * Mask",
            ],
            [
                "Đẩy một sphere theo normal bằng sine pulse. Sau đó thử trên cube để thấy hard normal ảnh hưởng ra sao.",
            ],
        ),
        (
            "69. Transform và TransformPosition",
            "Đổi vector hoặc position giữa các không gian: tangent, local, world, view.",
            [
                "Khi normal map, view direction, world direction hoặc object direction cần cùng space để dot/cross.",
                "Khi viết material function dùng lại cho mesh có orientation khác nhau.",
                "Khi debug lỗi Fresnel, triplanar, world aligned hoặc WPO sai hướng.",
            ],
            [
                "Dot hai vector khác space là lỗi kinh điển: graph compile nhưng visual sai.",
                "Transform vector và transform position không giống nhau.",
                "Không gian tangent phụ thuộc UV/tangent basis của mesh.",
            ],
            [
                "N_ws = Transform(TangentNormal, Tangent, World)",
                "V_ws = normalize(CameraPos - WorldPos)",
                "facing = dot(normalize(N_ws), normalize(V_ws))",
            ],
            [
                "Tự tính Fresnel bằng normal world và view direction. Sau đó cố ý dùng sai space để nhận diện bug.",
            ],
        ),
        (
            "70. CheapContrast / RemapValueRange",
            "Nhóm node/function để ép mask mềm thành rõ hơn hoặc đổi khoảng giá trị.",
            [
                "Khi noise quá xám, dissolve chưa có cạnh rõ, alpha cần tighten.",
                "Khi muốn map giá trị 0.2-0.8 thành 0-1 để dễ control.",
                "Khi cần artist slider trực quan hơn thay vì nhiều Multiply/Add rời.",
            ],
            [
                "Contrast quá gắt làm mất detail và tạo aliasing.",
                "Remap không clamp có thể tạo giá trị âm hoặc lớn hơn 1 nếu input vượt range.",
                "Đừng sửa mask bằng contrast khi nguyên nhân là texture/source sai.",
            ],
            [
                "remap = saturate((x - inMin) / (inMax - inMin))",
                "contrast = saturate((x - 0.5) * Contrast + 0.5)",
            ],
            [
                "Lấy một noise mềm, remap vùng 0.35-0.7 thành 0-1, rồi dùng nó làm opacity dissolve.",
            ],
        ),
        (
            "71. VectorToRadialValue / Polar UV",
            "Chuyển UV phẳng thành tọa độ radial để làm ring, portal, spiral hoặc black hole.",
            [
                "Portal ring, shockwave, vortex, magic circle, radial wipe.",
                "Khi motion nên chạy quanh tâm thay vì chạy ngang/dọc.",
                "Khi cần tách radius và angle thành hai trục điều khiển riêng.",
            ],
            [
                "Tâm sai làm radial pattern lệch.",
                "Đường seam của angle có thể lộ nếu texture không tile tốt.",
                "Polar UV có thể méo texture mạnh gần tâm.",
            ],
            [
                "p = UV - 0.5",
                "r = length(p)",
                "a = atan2(p.y, p.x) / TWO_PI",
                "polarUV = float2(a + Time * Speed, r)",
            ],
            [
                "Tạo một ring có noise chạy theo trục angle. Sau đó đảo trục để noise chạy từ tâm ra ngoài.",
            ],
        ),
        (
            "72. CameraDepthFade",
            "Fade particle khi quá gần camera để tránh card cắt màn hình hoặc phủ camera quá gắt.",
            [
                "Flare, smoke, fire, muzzle flash, large sprite gần người chơi.",
                "Khi gameplay camera có thể chui sát effect.",
                "Khi cần VFX không làm người chơi bị lóa hoặc mất thông tin.",
            ],
            [
                "Fade quá dài làm effect biến mất sớm.",
                "Không giải quyết intersection với world geometry; đó là việc của DepthFade.",
                "Nếu dùng additive, cần nhân fade vào emissive để thấy tác dụng.",
            ],
            [
                "nearFade = CameraDepthFade(FadeLength)",
                "emissive *= nearFade",
                "opacity *= nearFade",
            ],
            [
                "Đặt một flare lớn trước camera và so sánh khi không có CameraDepthFade với FadeLength hợp lý.",
            ],
        ),
        (
            "73. WorldAlignedTexture",
            "Sample texture theo world/object projection để giảm phụ thuộc vào UV unwrap.",
            [
                "Mesh dissolve, wet/ice/lava surface, decal-like procedural breakup trên object khó unwrap.",
                "Khi nhiều mesh cần cùng texture scale theo world unit.",
                "Khi UV bị stretch và làm noise mask đọc xấu.",
            ],
            [
                "Texture đứng yên trong world khi object di chuyển nếu không dùng setup phù hợp.",
                "Projection seam/blending có thể lộ ở góc hoặc bề mặt phức tạp.",
                "Đắt hơn sample UV đơn giản; không dùng chỉ vì tiện.",
            ],
            [
                "mask = WorldAlignedTexture(Noise, WorldPos, TextureSize)",
                "dissolve = smoothstep(Amount, Amount + Softness, mask)",
            ],
            [
                "So sánh cùng noise dissolve bằng UV unwrap và WorldAlignedTexture trên mesh bị stretch UV.",
            ],
        ),
        (
            "74. Static Switch Parameter",
            "Công tắc compile-time để bật/tắt nhánh shader và tạo variant material.",
            [
                "Khi một master material có option thật sự khác cost: dùng distortion hay không, dùng Fresnel hay không.",
                "Khi muốn instance chọn feature mà không trả cost runtime của nhánh tắt.",
                "Khi chia family rõ ràng nhưng vẫn giữ chung master.",
            ],
            [
                "Quá nhiều switch tạo bùng nổ shader permutations.",
                "Không dùng switch cho thông số artist cần animate realtime.",
                "Switch làm compile và management nặng nếu master ôm quá nhiều family.",
            ],
            [
                "if USE_DISTORTION:",
                "    opacityUV = UV + distortion",
                "else:",
                "    opacityUV = UV",
            ],
            [
                "Trong một master sprite, tạo switch UseDepthFade. Tạo hai instance và xem shader variant/cost khác nhau ra sao.",
            ],
        ),
        (
            "75. Custom HLSL Node",
            "Viết code shader trực tiếp khi graph node quá dài hoặc cần phép toán đặc thù.",
            [
                "Khi có math lặp lại nhiều, thuật toán radial/spiral phức tạp, hoặc cần loop nhỏ có kiểm soát.",
                "Khi khách hàng/tech art yêu cầu HLSL để tối ưu hoặc đọc rõ công thức.",
                "Khi cần port logic giữa Unreal, Unity HLSL/Shader Graph, hoặc tool nội bộ.",
            ],
            [
                "Không dùng Custom chỉ để làm Add/Multiply bình thường.",
                "Khó debug hơn node graph nếu không comment và đặt input/output rõ.",
                "Một số function/texture sample trong custom có giới hạn theo engine/version.",
            ],
            [
                "float2 p = UV - 0.5;",
                "float r = length(p);",
                "float a = atan2(p.y, p.x);",
                "return smoothstep(Radius - Width, Radius, r) - smoothstep(Radius, Radius + Softness, r);",
            ],
            [
                "Viết custom node tạo ring band, rồi dựng cùng logic bằng node thường. So sánh độ dễ đọc và khả năng expose parameter.",
            ],
        ),
    ]
    for args in deep_nodes:
        deep_node(doc, *args)

    part(doc, "Phần 11 - Quy trình đọc graph theo lane")
    body(doc, "Khi graph lớn, đừng đọc từ trái sang phải một cách hoảng. Hãy đọc theo lane. Lane là đường logic có mục đích riêng: UV tạo tọa độ, Mask tạo hình, Color tạo màu, Opacity quyết định hiện/ẩn, WPO làm biến dạng, Interaction đọc scene hoặc gameplay, Performance kiểm chi phí. Một graph giỏi thường không phải graph ít node nhất; nó là graph mà mỗi lane có lý do rõ.")
    lane_guides = [
        (
            "76. Lane UV",
            [
                "Tìm TexCoord, Panner, Rotator, Polar, Distortion, SubUV hoặc WorldAlignedTexture.",
                "Ghi lại texture nào dùng UV gốc, texture nào dùng UV đã pan, texture nào dùng UV radial.",
                "Hỏi: motion nằm ở texture hay nằm ở particle transform?",
                "Debug bằng cách nối UV hoặc sample checker/NumberGrid vào Emissive.",
                "Nếu hiệu ứng bị trượt sai hướng, kiểm tra sign của speed, scale, pivot và space.",
            ],
            [
                "UV lane trả lời câu hỏi: dữ liệu đang được đọc ở đâu trên texture hoặc trên bề mặt?",
                "Trong VFX, 70% cảm giác motion thường bắt đầu từ UV lane.",
            ],
        ),
        (
            "77. Lane Mask",
            [
                "Tìm alpha texture, noise channel, radial gradient, vertex color, depth mask, Fresnel hoặc threshold.",
                "Xem mask đi qua Power, SmoothStep, Step, Clamp, OneMinus hay Multiply nào.",
                "Tách từng mask ra Emissive grayscale để xem shape thật.",
                "Hỏi: mask này điều khiển silhouette, detail breakup, edge band hay intensity?",
                "Nếu visual bẩn, đừng thêm màu; hãy sửa mask trước.",
            ],
            [
                "Mask lane là ngữ pháp của material. Nó quyết định vùng nào được nói, vùng nào im lặng.",
                "Một effect đẹp thường có mask lớn rõ trước, mask nhỏ giàu chi tiết sau.",
            ],
        ),
        (
            "78. Lane Color và Emissive",
            [
                "Tìm color parameter, Particle Color, texture RGB, gradient, Lerp màu, intensity multiplier.",
                "Kiểm tra màu có bị multiply nhiều lần khiến bẩn hoặc mất hue không.",
                "Xem Emissive có scale theo alpha/fade không, đặc biệt với Additive.",
                "Tách color không có bloom để kiểm tra hue và value thật.",
                "Hỏi: core sáng hơn edge hay edge sáng hơn core?",
            ],
            [
                "Color lane không chỉ là chọn màu đẹp. Nó là hierarchy ánh sáng: đâu là tâm, đâu là rìa, đâu là dư âm.",
                "Trong production, emissive quá mạnh thường che lỗi mask cho tới khi vào gameplay thật.",
            ],
        ),
        (
            "79. Lane Opacity và Blend Mode",
            [
                "Đọc material property trước: Additive, Translucent hay Masked.",
                "Tìm Opacity/Opacity Mask input và xem alpha cuối cùng đến từ đâu.",
                "Kiểm tra ParticleColor.a, DepthFade, CameraDepthFade và alpha power.",
                "Với Additive, nhớ rằng fade có thể cần nhân vào Emissive thay vì chỉ Opacity.",
                "Nếu edge bị cứng, xem threshold/clip value, texture alpha và depth fade.",
            ],
            [
                "Opacity lane trả lời câu hỏi: pixel này có quyền tồn tại bao nhiêu phần trăm?",
                "Blend mode là luật hòa âm với background; đổi blend mode là đổi bản chất visual.",
            ],
        ),
        (
            "80. Lane WPO",
            [
                "Tìm World Position Offset, VertexNormalWS, sine pulse, noise displacement hoặc mesh deformation.",
                "Xem WPO chạy theo vertex mask, normal, world direction hay object direction.",
                "Kiểm tra amplitude có scale theo object/particle không.",
                "Debug bounds nếu mesh biến mất khi WPO quá lớn.",
                "Hỏi: deformation phục vụ silhouette hay chỉ tạo shimmer bề mặt?",
            ],
            [
                "WPO lane đắt không chỉ vì node, mà vì nó làm geometry và bounds trở thành một phần của effect.",
                "Một sprite glow thường không cần WPO; shield shell hoặc portal mesh thì có thể cần.",
            ],
        ),
        (
            "81. Lane Interaction",
            [
                "Tìm SceneDepth, CustomDepth, Distance Field, WorldPosition, hit position, blueprint/Niagara parameter.",
                "Xác định dữ liệu interaction đến từ engine, Niagara hay material tự tính.",
                "Xem có cần soft intersection, occlusion, impact ripple, hoặc dissolve theo gameplay không.",
                "Debug bằng grayscale: distance, depth difference, custom depth mask.",
                "Nếu effect không phản ứng, kiểm tra binding trước khi nghi ngờ math.",
            ],
            [
                "Interaction lane là nơi material vượt khỏi texture đẹp để trở thành realtime effect.",
                "Đây cũng là lane dễ phát sinh bug cross-platform nhất, nên cần test trong level thật.",
            ],
        ),
        (
            "82. Lane Performance",
            [
                "Đếm texture samples, instruction count, translucent layer, overdraw, refraction, lit translucency.",
                "Tìm node có thể bake thành texture hoặc pack channel.",
                "Xem effect spawn bao nhiêu lần, trên màn hình bao lâu, diện tích pixel lớn không.",
                "Kiểm tra shader complexity, quad overdraw, Niagara particle count và bounds.",
                "Chọn tối ưu theo tình huống: giảm card size, giảm layer, giảm texture, giảm feature hoặc giảm spawn.",
            ],
            [
                "Performance lane là tư duy kiếm tiền: effect đẹp nhưng không chạy được trong gameplay thì chưa xong.",
                "Tối ưu tốt không phải xóa hết detail; nó là giữ đúng detail mà người chơi thật sự đọc được.",
            ],
        ),
    ]
    for title, steps, notes in lane_guides:
        h1(doc, title)
        for i, step in enumerate(steps, 1):
            numbered(doc, i, step)
        callout(doc, "Ý nghĩa", notes, fill=LIGHT_TEAL)

    part(doc, "Phần 12 - Bài tập tổng hợp theo production")
    body(doc, "Các bài tập này được thiết kế giống vòng lặp làm nghề: dựng bản đầu, debug, expose parameter, tối ưu, rồi tự review. Không cần làm tất cả trong một ngày. Mỗi bài nên lưu thành một folder nhỏ có material, material instance, Niagara system test và một level preview đơn giản.")
    production_drills = [
        (
            "83. Bài tập Flare Pack",
            "Tạo ba flare: small spark, medium impact, large magical bloom. Cả ba dùng chung master Additive nhưng khác texture, color, exponent, camera fade.",
            [
                "Master có Texture, ParticleColor, Intensity, AlphaPower, CameraFade.",
                "Material Instance đặt tên theo visual purpose: MI_Flare_Spark, MI_Flare_Impact, MI_Flare_Bloom.",
                "Niagara điều khiển size, lifetime, color over life và alpha fade.",
                "Debug ở background sáng và tối để biết additive có bị mất đọc không.",
            ],
            [
                "Flare luôn trắng bệt vì intensity quá cao.",
                "Không fade khi gần camera vì chỉ nối CameraDepthFade vào Opacity trong material additive.",
                "Texture alpha quá mềm khiến flare bị mờ như sương thay vì có core.",
            ],
        ),
        (
            "84. Bài tập Dissolve Surface",
            "Tạo material dissolve cho mesh với noise mask, threshold, edge band và emissive edge. Mục tiêu là có thể reveal hoặc destroy object bằng một scalar Amount.",
            [
                "Dùng noise hoặc packed mask làm nguồn breakup.",
                "Visible mask = SmoothStep(Amount, Amount + Softness, noise).",
                "Edge band = hai SmoothStep trừ nhau quanh threshold.",
                "Expose Amount, Softness, EdgeWidth, EdgeColor, EdgeIntensity.",
                "Test trên sphere, cube và mesh có UV stretch.",
            ],
            [
                "Edge quá dày làm dissolve thành outline cartoon ngoài ý muốn.",
                "Noise scale quá nhỏ khiến bề mặt rã thành bụi khó đọc.",
                "Masked clip quá cứng nhưng không có soft transition hoặc temporal support.",
            ],
        ),
        (
            "85. Bài tập Fire Card",
            "Tạo flame card không dùng flipbook trước: hai noise panner, gradient từ dưới lên, edge fade và emissive ramp.",
            [
                "UV lane: một noise lớn chảy lên, một noise nhỏ chảy nhanh hơn.",
                "Mask lane: vertical gradient giữ gốc lửa dày và ngọn lửa mỏng.",
                "Color lane: core vàng/trắng, mid cam, edge đỏ tối.",
                "Opacity lane: alpha = flameMask * edgeFade * ParticleColor.a.",
                "Thêm optional distortion bằng noise nhỏ nhưng giữ silhouette ổn.",
            ],
            [
                "Lửa trông như texture kéo lên vì thiếu breakup ở silhouette.",
                "Gradient sai chiều làm gốc lửa biến mất trước ngọn.",
                "Color ramp quá đều, không có core nóng.",
            ],
        ),
        (
            "86. Bài tập Smoke Flipbook",
            "Dùng ParticleSubUV cho smoke atlas. Niagara quản lý frame/lifetime; material quản lý tint, alpha power, depth fade và optional lighting.",
            [
                "Renderer bật SubUV và SubImageSize đúng với atlas.",
                "ParticleSubUV RGB/A đi qua ParticleColor.",
                "Alpha có Power để điều chỉnh độ dày.",
                "DepthFade xử lý giao mặt đất/tường.",
                "Thử Unlit smoke nhẹ và DefaultLit smoke nếu project budget cho phép.",
            ],
            [
                "Atlas chạy sai thứ tự vì SubImageSize hoặc frame count sai.",
                "Smoke bị posterize do alpha power quá gắt.",
                "Particle quá lớn gây overdraw nặng dù material đơn giản.",
            ],
        ),
        (
            "87. Bài tập Portal Ring",
            "Tạo portal ring từ radial UV, hai threshold tạo band, noise chạy vòng và depth fade. Mục tiêu là ring đọc rõ trước, detail đọc sau.",
            [
                "Radial lane tách radius và angle.",
                "Ring band = outer threshold - inner threshold.",
                "Noise flow dùng hai panner hoặc polar UV để tránh single-scroll.",
                "Emissive edge mạnh hơn interior fill.",
                "Expose Radius, Width, Softness, FlowSpeed, NoiseScale, EdgeIntensity.",
            ],
            [
                "Portal thành đĩa tròn vì không trừ inner/outer band.",
                "Noise chạy ngang phẳng nên mất cảm giác cổng xoáy.",
                "Ring quá nhiều detail nhỏ làm silhouette không đọc được.",
            ],
        ),
        (
            "88. Bài tập Forcefield Shield",
            "Tạo shield shell có Fresnel rim, impact ripple và noise breakup. Có thể dùng mesh sphere hoặc custom shield mesh.",
            [
                "Base opacity lấy từ Fresnel để rim rõ hơn mặt chính.",
                "Noise mask phá đều bề mặt nhưng không phá mất silhouette.",
                "Hit position truyền từ Blueprint/Niagara vào material để tạo ring distance.",
                "WPO nhỏ theo normal cho pulse, nhớ kiểm tra bounds.",
                "Expose RimPower, RimIntensity, HitRadius, HitWidth, NoiseStrength, WPOAmount.",
            ],
            [
                "Shield thành quả bóng đặc vì opacity quá cao.",
                "Fresnel power sai làm rim biến mất khi nhìn thẳng.",
                "Hit ripple không đúng vị trí vì sai space giữa hit point và WorldPosition.",
            ],
        ),
        (
            "89. Bài tập Energy Beam/Ribbon",
            "Tạo beam gồm core, outer glow và noise crawl. Có thể làm bằng ribbon renderer hoặc mesh strip.",
            [
                "UV dọc beam dùng để pan noise theo chiều bắn.",
                "Core mask mảnh và sáng; outer mask rộng hơn nhưng yếu hơn.",
                "Noise crawl chỉ phá intensity, không phá hoàn toàn chiều đọc của beam.",
                "Fade đầu/cuối bằng UV gradient hoặc particle data.",
                "Expose CoreWidth, OuterWidth, Speed, Intensity, Distortion, EndFade.",
            ],
            [
                "Beam nhìn như thanh chữ nhật vì thiếu edge falloff.",
                "Noise quá mạnh làm beam bị đứt đoạn không chủ đích.",
                "Ribbon twist làm UV đảo nếu renderer/mesh setup sai.",
            ],
        ),
        (
            "90. Bài tập Projectile Core/Shell",
            "Tạo projectile có lõi sáng, shell ngoài, trail ngắn và impact flare dùng chung vài material family.",
            [
                "Core dùng additive sprite hoặc mesh emissive đơn giản.",
                "Shell dùng Fresnel hoặc radial mask để tạo lớp năng lượng bên ngoài.",
                "Trail dùng panning noise/beam material với fade đuôi.",
                "Impact dùng flare pack cộng với ring shockwave nhỏ.",
                "Tối ưu bằng cách reuse texture, material instance và Niagara modules.",
            ],
            [
                "Mọi layer cùng intensity nên projectile không có hierarchy.",
                "Trail quá dày che gameplay target.",
                "Quá nhiều material riêng lẻ khiến iteration chậm và khó maintain.",
            ],
        ),
    ]
    for title, goal, checklist, mistakes in production_drills:
        h1(doc, title)
        body(doc, goal)
        h2(doc, "Checklist thực hiện")
        for item in checklist:
            bullet(doc, item)
        callout(doc, "Lỗi thường gặp", mistakes, fill=LIGHT_RED, color=RED)
        callout(doc, "Cách tự review", [
            "Chụp một frame grayscale của mask chính. Nếu grayscale chưa đọc, bản màu chưa nên được duyệt.",
            "Đổi background sáng/tối, camera gần/xa, spawn ít/nhiều để xem effect có giữ được ý đồ không.",
            "Viết lại graph logic bằng 3 dòng công thức. Nếu không viết được, graph đang thiếu tổ chức.",
        ], fill=LIGHT_TEAL)

    part(doc, "Phần 13 - Checklist đi làm và tự học dài hạn")
    h1(doc, "91. Checklist trước khi giao material")
    for item in [
        "Tên material, texture, parameter và material instance đọc được mục đích visual.",
        "Material có comments hoặc tổ chức lane: UV, Mask, Color, Opacity, Interaction, WPO.",
        "Parameter ít nhưng đủ: không expose mọi số, chỉ expose thứ artist/director cần chỉnh.",
        "Có preview instance default đẹp và neutral, không phụ thuộc setup bí mật trong level.",
        "Blend mode, shading model, two-sided, depth fade, camera fade được chọn có lý do.",
        "Shader complexity và overdraw đã kiểm tra trong context gameplay, không chỉ trong preview.",
        "Texture size, compression, channel packing và alpha usage có lý do rõ.",
        "Niagara binding đã test: Particle Color, Dynamic Parameter, SubUV, renderer material slot.",
    ]:
        bullet(doc, item)
    h1(doc, "92. Cách luyện để master shader VFX")
    body(doc, "Học shader giống học ngoại ngữ ở chỗ phải dùng mỗi ngày. Nhưng khác ở điểm: mỗi từ vựng node phải đi cùng hình ảnh, công thức và lỗi thực tế. Nếu chỉ nhớ định nghĩa, bạn sẽ quên. Nếu dựng được ba biến thể visual từ một node, bạn bắt đầu sở hữu nó.")
    for item in [
        "Mỗi tuần chọn một family: flare, dissolve, fire, smoke, portal, shield, beam.",
        "Mỗi family dựng một bản texture-driven, một bản math-driven, một bản hybrid.",
        "Mỗi ngày chọn một node và viết: nó nhận input gì, trả output gì, dùng cho VFX nào, lỗi gì hay gặp.",
        "Khi xem material của người khác, đừng copy. Hãy redraw lane diagram bằng chữ trước.",
        "Khi nhận feedback, dịch câu nói visual thành layer: shape, mask, motion, color, interaction hoặc cost.",
        "Khi học Unity/Unreal khác nhau, giữ công thức trong đầu; node name có thể đổi nhưng math không đổi.",
    ]:
        bullet(doc, item)
    code(doc, [
        "Visual feedback -> Technical translation",
        '"Làm mạnh hơn"  -> tăng hierarchy: core/edge/intensity/scale/timing?',
        '"Nhìn giả"      -> motion quá đều, mask quá sạch, color quá một lớp?',
        '"Nặng quá"      -> overdraw, texture fetch, particle count, refraction, lit translucency?',
        '"Không đọc"     -> silhouette yếu, contrast sai, timing quá nhanh, background không hợp?',
    ], "Sổ tay dịch feedback")
    callout(doc, "Kỷ luật học", [
        "Một material khó không phải để hỏi đáp cho xong. Nó là bài đọc hiểu. Hãy đọc input, lane, output, rồi tự dựng lại phiên bản nhỏ hơn.",
        "Master shader không có nghĩa là biết mọi node. Nó là khả năng biến visual problem thành data problem, rồi biến data problem thành graph sạch.",
    ], fill=LIGHT_GOLD, color=GOLD)

    part(doc, "Phần 14 - Lộ trình luyện 6 tuần")
    body(doc, "Lộ trình này dành cho người đã làm VFX nhưng muốn nâng cấp lên Technical VFX Artist mạnh về material. Mục tiêu không phải học hết node trong 6 tuần. Mục tiêu là tạo phản xạ: nhìn visual, bóc layer, chọn data, dựng graph, expose parameter, debug và tối ưu.")
    weekly_plan = [
        (
            "93. Tuần 1 - Đọc grayscale và mask",
            [
                "Mỗi ngày dựng 3 mask: radial, linear gradient, noise threshold.",
                "Tập dùng Add, Multiply, Power, OneMinus, Clamp, SmoothStep.",
                "Mọi bài chỉ xuất grayscale vào Emissive, chưa dùng màu.",
                "Tự hỏi: mask này tạo silhouette, edge, breakup hay timing?",
            ],
            [
                "Một file Unreal có 10 material instance mask khác nhau.",
                "Một trang note ghi công thức và hình dung bằng lời cho từng mask.",
            ],
        ),
        (
            "94. Tuần 2 - UV motion",
            [
                "Dựng panner một lớp, hai lớp ngược chiều, rotator, radial UV và distortion UV.",
                "Dùng NumberGrid/checker để debug hướng UV trước khi dùng noise đẹp.",
                "Tập phân biệt motion của texture, motion của particle và motion của mesh.",
                "Làm lại một effect cũ từ AE bằng UV motion realtime.",
            ],
            [
                "Một fire card procedural, một energy crawl, một portal noise chạy vòng.",
                "Một checklist lỗi: pivot sai, speed sai dấu, tiling quá cao, seam polar lộ.",
            ],
        ),
        (
            "95. Tuần 3 - Particle material contract",
            [
                "Dùng Particle Color đúng cho RGB và Alpha.",
                "Dùng Dynamic Parameter có naming cố định cho một material family.",
                "Dựng SubUV flipbook bằng NumberGrid trước, sau đó mới dùng fire/smoke atlas.",
                "Test CameraDepthFade và DepthFade trong level có geometry thật.",
            ],
            [
                "Một Niagara system fire/smoke có SubUV đúng frame.",
                "Một material additive có fade thật bằng ParticleColor.a nhân vào Emissive.",
            ],
        ),
        (
            "96. Tuần 4 - Interaction và world data",
            [
                "Dùng WorldPosition tạo height dissolve và radial hit ripple.",
                "Dùng ObjectPosition/ObjectRadius tạo shell mask ổn định theo scale.",
                "Dùng SceneDepth/PixelDepth hoặc DepthFade để làm soft intersection.",
                "Dựng một shield có hit point truyền vào material.",
            ],
            [
                "Một forcefield shell có Fresnel, noise breakup, hit ripple và WPO nhẹ.",
                "Một note giải thích space: UV, object, world, view, tangent.",
            ],
        ),
        (
            "97. Tuần 5 - Recipe production",
            [
                "Dựng flare, dissolve, fire card, smoke flipbook, portal ring, beam.",
                "Mỗi recipe phải có master material, ít nhất hai material instance và một Niagara preview.",
                "Mỗi recipe có parameter expose đủ để art director chỉnh nhanh.",
                "Mỗi recipe được test trên background sáng/tối và camera gần/xa.",
            ],
            [
                "Một mini library VFX material tái sử dụng được.",
                "Một video hoặc GIF review từng effect ở viewport/gameplay.",
            ],
        ),
        (
            "98. Tuần 6 - Optimization và rebuild",
            [
                "Chọn ba effect nặng nhất và đọc shader complexity/overdraw.",
                "Tối ưu bằng cách giảm card size, giảm translucent layer, pack channel, bake noise hoặc giảm feature.",
                "Rebuild một material của VFXSamplePack từ trí nhớ, sau đó so với bản gốc.",
                "Viết lại graph logic thành sơ đồ ASCII trước khi mở Material Editor.",
            ],
            [
                "Một bản trước/sau optimization có note rõ: đã giảm gì, giữ gì, trade-off gì.",
                "Một checklist cá nhân dùng trước khi giao material cho production.",
            ],
        ),
    ]
    for title, practice, deliverables in weekly_plan:
        h1(doc, title)
        h2(doc, "Bài luyện")
        for item in practice:
            bullet(doc, item)
        callout(doc, "Deliverable cuối tuần", deliverables, fill=LIGHT_TEAL)

    part(doc, "Phần 15 - Câu hỏi tự kiểm trước khi nói mình hiểu")
    body(doc, "Một cách học chắc là tự vấn. Nếu trả lời được các câu dưới đây bằng lời của mình, bạn không chỉ nhớ node mà đang hiểu material như một hệ thống.")
    self_questions = [
        "Nếu bỏ hết màu, mask chính của effect có còn đọc được không?",
        "Texture nào là source shape, texture nào là source detail, texture nào chỉ là support?",
        "Motion đến từ UV panner, rotator, particle transform, mesh animation hay flipbook frame?",
        "Alpha cuối cùng đi qua những node nào trước khi vào Opacity hoặc Emissive fade?",
        "Material dùng Additive, Translucent hay Masked vì lý do visual nào?",
        "Particle Color đang ảnh hưởng RGB, Alpha hay cả hai?",
        "Dynamic Parameter từng kênh R/G/B/A có ý nghĩa ổn định không?",
        "Nếu đổi effect sang Unity Shader Graph, công thức nào giữ nguyên và node name nào sẽ đổi?",
        "Có node nào đang dùng world space nhưng đáng ra nên dùng object/UV space không?",
        "Có texture sample nào có thể pack channel hoặc reuse không?",
        "Có feature nào đắt nhưng người chơi không đọc được trong gameplay không?",
        "Nếu art director nói 'mạnh hơn', bạn sẽ chỉnh intensity, contrast, scale, timing hay silhouette trước?",
        "Nếu effect nhìn giả, nó giả vì motion quá đều, mask quá sạch, color quá phẳng hay interaction thiếu?",
        "Nếu effect bị mất khi gần camera, camera fade hoặc bounds có vấn đề không?",
        "Nếu material đẹp trong preview nhưng xấu trong level, background, lighting, exposure, depth hay overdraw là nghi phạm nào?",
    ]
    for i, question in enumerate(self_questions, 1):
        numbered(doc, i, question)
    callout(doc, "Cách dùng câu hỏi", [
        "Mỗi khi học một material mới như M_Flare, M_Mesh_Add, M_FlamesCard hoặc M_SubUV_Utilities, hãy trả lời ít nhất 8 câu.",
        "Nếu câu trả lời còn mơ hồ, quay lại debug bằng grayscale, NumberGrid, hoặc nối từng lane ra Emissive.",
    ], fill=LIGHT_GOLD, color=GOLD)

    part(doc, "Phần 16 - Glossary production cho material VFX")
    body(doc, "Glossary này không viết theo kiểu từ điển hàn lâm. Mỗi mục là cách một Technical VFX Artist nên hiểu khi làm việc thật: nó ảnh hưởng visual nào, dùng ở đâu, và nên kiểm tra gì.")
    glossary = [
        ("93. Silhouette", "Hình dáng lớn mà người xem đọc được trong nửa giây đầu. Với VFX, silhouette thường quan trọng hơn detail texture. Một fire card đẹp nhưng silhouette vuông vẫn đọc là card."),
        ("94. Core", "Vùng năng lượng chính, thường sáng nhất hoặc dày nhất. Core giúp mắt biết effect đang phát ra từ đâu: tâm projectile, lõi beam, tâm flare, mép portal."),
        ("95. Edge", "Vùng chuyển tiếp ở rìa mask. Edge có thể mềm, cháy sáng, nhiễu, hoặc sắc. Dissolve đẹp thường nhờ edge band có kiểm soát."),
        ("96. Breakup", "Sự phá đều của mask để tránh mảng phẳng. Breakup có thể đến từ noise, vertex color, world aligned texture, flipbook alpha hoặc depth interaction."),
        ("97. Falloff", "Độ giảm dần từ mạnh sang yếu. Fresnel, radial gradient, depth fade, camera fade và alpha power đều là cách tạo falloff."),
        ("98. Overdraw", "Số lần nhiều lớp translucent vẽ chồng lên cùng pixel. Đây là kẻ thù lớn của smoke, fire, magic glow và particle dày."),
        ("99. Shader instruction", "Chi phí phép toán trong shader. Không phải chỉ số duy nhất, nhưng là tín hiệu để biết graph có đang quá nặng không."),
        ("100. Texture fetch", "Chi phí đọc texture. Một texture sample có thể rẻ hơn nhiều math, nhưng nhiều sample lớn trên translucent full-screen vẫn rất đắt."),
        ("101. Channel packing", "Nhét nhiều mask vào R/G/B/A của một texture. Tốt cho memory và fetch, nhưng phải đặt tên rõ để tránh dùng nhầm channel."),
        ("102. Atlas", "Một texture chứa nhiều frame hoặc nhiều sprite. Flipbook fire/smoke là atlas theo frame; packed decal sheet cũng là atlas."),
        ("103. Flipbook", "Animation được bake thành nhiều frame trong atlas. Engine chỉ chọn frame; material thường lo tint, alpha, fade, shaping."),
        ("104. Soft particle", "Particle mềm ở giao tuyến với geometry, thường dùng DepthFade hoặc depth math để tránh cạnh cắt cứng."),
        ("105. Refraction", "Làm méo background qua material translucent. Đẹp cho heat haze, glass, magic distortion nhưng cần dùng có lý do vì cost và sorting."),
        ("106. WPO", "World Position Offset, đẩy vertex trong shader. Dùng cho shield pulse, mesh dissolve, wave, shell inflate; cần kiểm bounds."),
        ("107. Bounds", "Hộp giới hạn để engine culling object/particle. WPO hoặc particle bay quá bounds có thể bị biến mất dù material không sai."),
        ("108. Sorting", "Thứ tự vẽ translucent. Smoke, glass, portal layer có thể sai khi nhiều lớp chồng nhau; đôi khi phải sửa renderer order hoặc thiết kế lại."),
        ("109. Exposure", "Auto exposure/bloom trong scene có thể làm emissive khác xa preview. Test material trong lighting thật, không chỉ trong material editor."),
        ("110. Material Instance", "Nơi artist chỉnh thông số mà không phá master. Một master tốt phải có instance vui để dùng, không cần mở graph vẫn chỉnh được visual."),
        ("111. Master Material", "Khuôn cho một family effect. Master tốt tập trung, có naming rõ, không ôm mọi feature của mọi effect."),
        ("112. Material Function", "Module graph tái sử dụng. Dùng cho logic đủ lớn như radial ring, remap, flipbook helper; không dùng để giấu sự lộn xộn."),
        ("113. Space", "Không gian dữ liệu: UV, object, world, view, tangent. Rất nhiều bug shader đến từ việc trộn hai vector khác space."),
        ("114. Debug view", "Cách nối từng lane ra Emissive để xem grayscale/color trực tiếp. Đây là kỹ năng bắt buộc, giống solo layer trong AE."),
    ]
    for title, explanation in glossary:
        h1(doc, title)
        body(doc, explanation)
        callout(doc, "Tự hỏi khi gặp mục này", [
            "Nó đang ảnh hưởng shape, mask, motion, color, interaction hay cost?",
            "Có cách debug trực tiếp bằng grayscale, NumberGrid hoặc một material instance đơn giản không?",
        ], fill=LIGHT_TEAL)

    part(doc, "Phần 17 - Troubleshooting theo triệu chứng")
    body(doc, "Khi effect hỏng, đừng sửa ngẫu nhiên. Hãy gọi đúng triệu chứng. Mỗi triệu chứng dưới đây có một nhóm nghi phạm thường gặp, và cách debug nhanh nhất là cô lập lane liên quan.")
    issues = [
        (
            "115. Effect nhìn phẳng như ảnh dán",
            [
                "Silhouette quá vuông hoặc quá đều.",
                "Mask không có hierarchy core/edge/detail.",
                "Motion chỉ là một panner thẳng.",
                "Không có depth fade, lighting, fresnel hoặc interaction để bám scene.",
            ],
            [
                "Xem alpha mask trên nền đen/trắng.",
                "Thêm edge falloff hoặc breakup ở silhouette trước khi thêm detail bên trong.",
                "Dùng hai motion layer có tốc độ khác nhau.",
            ],
        ),
        (
            "116. Effect bị bẩn màu",
            [
                "Nhân nhiều texture RGB với nhau làm value tụt và hue bùn.",
                "Particle Color, color ramp và texture color đánh nhau.",
                "Bloom/exposure làm vùng sáng mất hue.",
            ],
            [
                "Tách Color lane không bloom, không alpha.",
                "Giữ texture mask grayscale nếu material đã có color ramp.",
                "Chọn một nguồn quyết định hue chính.",
            ],
        ),
        (
            "117. Effect quá sáng hoặc cháy trắng",
            [
                "Emissive intensity quá cao.",
                "Additive nhiều layer chồng nhau.",
                "Alpha fade không nhân vào emissive.",
                "Exposure/bloom trong level khác preview.",
            ],
            [
                "Giảm intensity theo số lần spawn thực tế.",
                "Test trên background sáng và tối.",
                "Với additive, nhân ParticleColor.a hoặc camera fade vào emissive.",
            ],
        ),
        (
            "118. Dissolve không có cạnh đẹp",
            [
                "Mask source quá mềm hoặc quá noisy.",
                "Edge band tính sai: không lấy vùng quanh threshold.",
                "Softness/EdgeWidth không cân bằng.",
            ],
            [
                "Debug visible mask và edge mask riêng.",
                "Dùng hai SmoothStep trừ nhau để lấy band.",
                "Expose EdgeWidth và EdgeIntensity riêng.",
            ],
        ),
        (
            "119. Flipbook chạy sai frame",
            [
                "SubImageSize không khớp atlas grid.",
                "Frame count hoặc row/column sai.",
                "Material dùng UV math làm lệch atlas thay vì ParticleSubUV.",
            ],
            [
                "Dùng NumberGrid atlas để nhìn số frame.",
                "Kiểm renderer SubUV trước khi sửa material.",
                "Chỉ pan/distort trong frame local nếu đã hiểu derived 0-1 UV.",
            ],
        ),
        (
            "120. Smoke/fire cắt cứng vào mặt đất",
            [
                "Không có DepthFade.",
                "FadeDistance quá thấp.",
                "Opacity mask quá cứng trước khi vào DepthFade.",
            ],
            [
                "Đặt card xuyên qua cube và chỉnh FadeDistance.",
                "Debug opacity trước và sau DepthFade.",
                "Giảm alpha power nếu edge quá gắt.",
            ],
        ),
        (
            "121. Mesh biến mất khi WPO mạnh",
            [
                "Bounds không đủ lớn.",
                "WPO đẩy vertex ngoài vùng culling.",
                "World/object scale làm amplitude quá cao.",
            ],
            [
                "Tăng bounds hoặc giảm WPO amount.",
                "Debug WPO mask bằng grayscale.",
                "Test từ nhiều góc camera và khoảng cách.",
            ],
        ),
        (
            "122. Forcefield hit ripple sai vị trí",
            [
                "Hit position ở world space nhưng graph so với object/UV space.",
                "Object pivot/bounds lệch.",
                "Parameter không được truyền đúng từ Blueprint/Niagara.",
            ],
            [
                "Nối distance(WorldPos, HitPos) ra grayscale.",
                "In/preview HitPos nếu có thể.",
                "Chuẩn hóa space trước khi tính distance.",
            ],
        ),
        (
            "123. Portal ring thành đĩa tròn",
            [
                "Chỉ dùng radial gradient fill, chưa trừ inner/outer threshold.",
                "Noise threshold phủ toàn bộ interior.",
                "Opacity và emissive dùng cùng mask quá rộng.",
            ],
            [
                "Tách ring band bằng outer - inner.",
                "Dùng mask khác cho interior nếu cần.",
                "Giữ edge ring sáng nhất để portal đọc là vòng.",
            ],
        ),
        (
            "124. Material khó chỉnh dù graph đúng",
            [
                "Parameter đặt tên theo math thay vì visual.",
                "Expose quá nhiều số nhỏ.",
                "Không có default instance đẹp.",
                "Graph thiếu comment lane.",
            ],
            [
                "Đổi tên parameter thành Radius, Width, Softness, Intensity, FlowSpeed.",
                "Ẩn hoặc cố định control ít dùng.",
                "Tạo 2-3 preset material instance làm chuẩn.",
            ],
        ),
    ]
    for title, suspects, fixes in issues:
        h1(doc, title)
        h2(doc, "Nghi phạm")
        for item in suspects:
            bullet(doc, item)
        callout(doc, "Cách xử lý nhanh", fixes, fill=LIGHT_GOLD, color=GOLD)

    part(doc, "Phần 18 - Chuyển tư duy giữa Unreal và Unity")
    body(doc, "Node name có thể khác, renderer khác, pipeline khác, nhưng phần lõi của material VFX là data và math. Nếu bạn hiểu công thức, bạn có thể chuyển engine. Nếu chỉ nhớ vị trí node trong một editor, bạn sẽ bị kẹt khi đổi tool.")
    engine_notes = [
        (
            "125. Những thứ gần như giữ nguyên",
            [
                "UV motion: uv * tiling + time * speed.",
                "Mask shaping: saturate, pow, smoothstep, step, lerp.",
                "Radial math: distance/length từ tâm, atan2 cho polar.",
                "Fresnel logic: 1 - dot(normal, viewDir), sau đó pow.",
                "Dissolve logic: threshold mask, edge band quanh threshold.",
                "Flipbook logic: atlas frame lookup, particle system quản lý frame.",
            ],
        ),
        (
            "126. Những thứ phải kiểm tra theo engine",
            [
                "Blend mode và sorting translucent.",
                "Depth texture availability, scene depth sampling, soft particle support.",
                "Particle renderer binding: color, custom data, dynamic parameter tương đương.",
                "Shader graph/HLSL support cho custom function.",
                "Texture import, compression, sRGB, alpha handling.",
                "Mobile/console feature support và cost model.",
            ],
        ),
        (
            "127. Cách học để không phụ thuộc tool",
            [
                "Viết công thức trước khi tìm node.",
                "Tự đặt tên lane giống nhau ở mọi engine: UV, Mask, Color, Opacity, WPO, Depth.",
                "Giữ một folder texture test: NumberGrid, checker, radial gradient, noise pack.",
                "Khi port effect, port grayscale mask trước, port màu và bloom sau.",
                "Tập đọc shader bằng output: nếu output giống, node name không còn đáng sợ.",
            ],
        ),
        (
            "128. Khi nào nên dùng HLSL/code",
            [
                "Khi node graph trở nên dài chỉ để diễn đạt một công thức ngắn.",
                "Khi cần chia sẻ thuật toán giữa Unreal, Unity hoặc tool nội bộ.",
                "Khi cần tối ưu hoặc kiểm soát loop/math rõ hơn.",
                "Khi làm visual phức tạp như vortex, black hole, signed distance field pattern.",
                "Nhưng trước khi code, hãy chắc chắn bạn debug được bản node đơn giản.",
            ],
        ),
    ]
    for title, notes in engine_notes:
        h1(doc, title)
        for item in notes:
            bullet(doc, item)
    callout(doc, "Kết nối với mục tiêu nghề nghiệp", [
        "Technical VFX Artist mạnh không phải vì thuộc một engine duy nhất. Họ mạnh vì hiểu dữ liệu, hiểu shader math, hiểu renderer, và biết trade-off.",
        "Unreal, Unity, proprietary engine hay HLSL đều là các giọng khác nhau của cùng một ngôn ngữ material.",
    ], fill=LIGHT_TEAL)

    part(doc, "Phần 19 - Quy trình review material trong 10 phút")
    body(doc, "Khi deadline gần, bạn không thể đọc lại toàn bộ graph trong một giờ. Quy trình 10 phút này giúp bắt lỗi lớn trước khi gửi cho lead, client hoặc đưa vào gameplay test.")
    review_steps = [
        "Phút 1: nhìn material trong scene thật, không chỉ preview sphere/card. Đổi background sáng và tối nếu có thể.",
        "Phút 2: solo mask chính ra grayscale. Nếu mask không đọc, dừng lại sửa mask.",
        "Phút 3: kiểm Particle Color hoặc parameter binding. Kéo alpha/color trong Niagara xem material có phản ứng đúng không.",
        "Phút 4: kiểm UV motion bằng NumberGrid/checker hoặc giảm texture detail. Xem motion có sai hướng, quá nhanh hoặc lộ tiling không.",
        "Phút 5: kiểm opacity/fade. Camera gần, geometry cắt qua, particle cuối lifetime có biến mất sạch không.",
        "Phút 6: kiểm color/emissive không bloom. Nếu không bloom đã đẹp, bật bloom sẽ chỉ làm tốt hơn; nếu phải nhờ bloom che lỗi, cần sửa.",
        "Phút 7: kiểm scale. Effect ở kích thước gameplay có đọc không, hay chỉ đẹp khi zoom gần?",
        "Phút 8: kiểm overdraw/shader complexity nếu effect spawn nhiều. Đừng tối ưu mù; nhìn mode debug trước.",
        "Phút 9: kiểm naming. Parameter có tên artist hiểu được không, material instance có default hợp lý không?",
        "Phút 10: viết một dòng mô tả graph: source data -> UV/mask -> color/opacity -> interaction -> cost. Nếu không viết nổi, graph cần được tổ chức lại.",
    ]
    for item in review_steps:
        bullet(doc, item)
    code(doc, [
        "Review sentence mẫu:",
        "Noise atlas + ParticleSubUV -> alpha power + DepthFade -> ParticleColor tint -> emissive/opacity.",
        "Radial UV + two panning noises -> ring band -> edge emissive + soft opacity.",
        "WorldPosition distance to HitPos -> ripple band -> shield fresnel opacity + WPO pulse.",
    ], "Một dòng đọc graph")
    callout(doc, "Tiêu chuẩn gửi bài", [
        "Người khác mở material instance lên phải hiểu control chính trong vòng 30 giây.",
        "Một graph production không cần phô diễn node khó. Nó cần cho ra visual đúng, dễ chỉnh, và có cost hợp lý.",
    ], fill=LIGHT_GOLD, color=GOLD)

    h1(doc, "129. Nhật ký học 30 ngày")
    body(doc, "Dùng danh sách này như bài luyện nhỏ. Mỗi dòng chỉ cần 20-40 phút nếu làm đúng phạm vi. Quan trọng nhất là ghi lại công thức, chụp một hình debug grayscale, và lưu material instance.")
    thirty_days = [
        "Ngày 1: tạo radial mask mềm và cứng.",
        "Ngày 2: tạo ring band bằng hai SmoothStep.",
        "Ngày 3: dùng Power để làm alpha gắt/mềm.",
        "Ngày 4: dùng OneMinus đảo mask và giải thích visual đổi ra sao.",
        "Ngày 5: tạo noise threshold dissolve.",
        "Ngày 6: tạo edge emissive cho dissolve.",
        "Ngày 7: rebuild một material dissolve nhỏ từ trí nhớ.",
        "Ngày 8: tạo panner một lớp.",
        "Ngày 9: tạo hai panner ngược chiều.",
        "Ngày 10: tạo rotator đúng pivot.",
        "Ngày 11: tạo polar UV bằng radial/angle.",
        "Ngày 12: tạo portal noise chạy vòng.",
        "Ngày 13: tạo Particle Color fade cho additive.",
        "Ngày 14: tạo Dynamic Parameter điều khiển AlphaPower.",
        "Ngày 15: test CameraDepthFade trên flare lớn.",
        "Ngày 16: test DepthFade với smoke card cắt qua cube.",
        "Ngày 17: tạo SubUV bằng NumberGrid.",
        "Ngày 18: tạo smoke flipbook có alpha power.",
        "Ngày 19: tạo fire card procedural không flipbook.",
        "Ngày 20: tạo forcefield Fresnel shell.",
        "Ngày 21: tạo hit ripple bằng WorldPosition distance.",
        "Ngày 22: tạo WPO pulse theo VertexNormalWS.",
        "Ngày 23: tạo height dissolve bằng WorldPosition.z.",
        "Ngày 24: tạo object-radius shell mask.",
        "Ngày 25: pack ba mask vào RGB và đọc từng channel.",
        "Ngày 26: tối ưu một material bằng cách giảm texture sample.",
        "Ngày 27: tối ưu một Niagara effect bằng cách giảm overdraw.",
        "Ngày 28: đọc M_Flare và viết lane diagram.",
        "Ngày 29: đọc M_FlamesCard hoặc M_Mesh_Add và rebuild bản nhỏ.",
        "Ngày 30: chọn một effect cũ của bạn và viết lại bằng hệ material sạch hơn.",
    ]
    for item in thirty_days:
        bullet(doc, item)

    callout(doc, "Kết luận", [
        "Material VFX là ngôn ngữ. Node là từ vựng, mask/UV/color là ngữ pháp, recipe là mẫu câu, production feedback là bài speaking thật.",
        "Mục tiêu không phải nhớ mọi node. Mục tiêu là nhìn visual và biết nên dùng data nào, mask nào, motion nào, output nào, cost nào.",
    ], fill=LIGHT_GOLD, color=GOLD)

    doc.core_properties.title = "Giáo Trình Material VFX Realtime"
    doc.core_properties.subject = "Unreal Material, Niagara, shader fundamentals, VFXSamplePack"
    doc.core_properties.author = "Codex"
    doc.save(OUT)
    print(OUT)


if __name__ == "__main__":
    build()
