from pathlib import Path

from docx import Document
from docx.enum.section import WD_SECTION
from docx.enum.text import WD_ALIGN_PARAGRAPH
from docx.oxml import OxmlElement
from docx.oxml.ns import qn
from docx.shared import Cm, Pt, RGBColor


OUT = Path.home() / "Downloads" / "Giao_Trinh_Material_VFX_Realtime_V16_HumanizedBook.docx"
IMAGE_DIR = Path("Saved") / "material_vfx_v5_images"

INK = RGBColor(23, 23, 23)
MUTED = RGBColor(92, 92, 92)
BLUE = RGBColor(18, 62, 100)
ORANGE = RGBColor(160, 82, 28)
RED = RGBColor(145, 55, 42)
GREEN = RGBColor(48, 110, 72)

BODY = "Georgia"
HEAD = "Aptos Display"
SANS = "Aptos"
CODE = "Consolas"


def set_font(run, size=10.2, name=BODY, bold=False, italic=False, color=INK):
    run.font.name = name
    run._element.rPr.rFonts.set(qn("w:eastAsia"), name)
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


def border_bottom(paragraph, color="D0D0D0", size="4"):
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
    bottom.set(qn("w:space"), "5")
    bottom.set(qn("w:color"), color)


def keep_next(paragraph):
    p_pr = paragraph._p.get_or_add_pPr()
    if p_pr.find(qn("w:keepNext")) is None:
        p_pr.append(OxmlElement("w:keepNext"))


def para(doc, text="", size=10.35, before=0, after=5.4, align=None, style=None, font=BODY, bold=False, italic=False, color=INK):
    p = doc.add_paragraph()
    if style:
        p.style = style
    p.paragraph_format.space_before = Pt(before)
    p.paragraph_format.space_after = Pt(after)
    p.paragraph_format.line_spacing = 1.26
    if align is not None:
        p.alignment = align
    if text:
        r = p.add_run(text)
        set_font(r, size=size, name=font, bold=bold, italic=italic, color=color)
    return p


def body(doc, text):
    return para(doc, text)


def small(doc, text, italic=False, color=MUTED, after=3.2):
    return para(doc, text, size=8.6, font=SANS, italic=italic, color=color, after=after)


def h_part(doc, number, title, subtitle):
    doc.add_page_break()
    p = para(doc, f"PHẦN {number}", size=10, font=SANS, bold=True, color=ORANGE, before=36, after=8, align=WD_ALIGN_PARAGRAPH.CENTER)
    p = para(doc, title, size=25, font=HEAD, bold=True, color=BLUE, after=8, align=WD_ALIGN_PARAGRAPH.CENTER)
    p.paragraph_format.line_spacing = 1.05
    para(doc, subtitle, size=11.5, font=BODY, italic=True, color=MUTED, after=24, align=WD_ALIGN_PARAGRAPH.CENTER)
    border_bottom(p, color="C98845", size="8")
    quote(doc, "Một chương tốt không chỉ đưa thêm node. Nó đổi cách mắt đọc dữ liệu.", "Ghi chú học tập")


def h1(doc, title):
    p = para(doc, title, size=18.5, font=HEAD, bold=True, color=BLUE, before=10, after=7, style="Heading 1")
    p.paragraph_format.line_spacing = 1.08
    border_bottom(p)
    keep_next(p)
    return p


def h2(doc, title):
    p = para(doc, title, size=12.4, font=SANS, bold=True, color=INK, before=8, after=3, style="Heading 2")
    keep_next(p)
    return p


def h3(doc, title, color=BLUE):
    p = para(doc, title, size=10.6, font=SANS, bold=True, color=color, before=5, after=2)
    keep_next(p)
    return p


def bullet(doc, text):
    p = doc.add_paragraph()
    p.paragraph_format.left_indent = Cm(0.52)
    p.paragraph_format.first_line_indent = Cm(-0.23)
    p.paragraph_format.space_after = Pt(2.4)
    p.paragraph_format.line_spacing = 1.12
    r = p.add_run("- " + text)
    set_font(r, size=9.75, name=BODY)


def quote(doc, text, who=None):
    p = para(doc, text, size=11.0, font=BODY, italic=True, color=RGBColor(52, 52, 52), before=4, after=2)
    p.paragraph_format.left_indent = Cm(0.55)
    p.paragraph_format.right_indent = Cm(0.55)
    shade(p, "F5F2EC")
    if who:
        small(doc, "— " + who, italic=True, color=MUTED, after=5)


def callout(doc, title, lines, fill="EEF4FA", color=BLUE):
    h = para(doc, title, size=9.6, font=SANS, bold=True, color=color, before=5, after=1.2)
    h.paragraph_format.left_indent = Cm(0.35)
    h.paragraph_format.right_indent = Cm(0.35)
    shade(h, fill)
    for line in lines:
        p = para(doc, "- " + line, size=9.35, font=BODY, before=0, after=1.4)
        p.paragraph_format.left_indent = Cm(0.65)
        p.paragraph_format.first_line_indent = Cm(-0.25)
        p.paragraph_format.right_indent = Cm(0.35)
        shade(p, fill)


def code(doc, lines, title=None):
    if title:
        small(doc, title, italic=True, after=1.2)
    for line in lines:
        p = para(doc, line, size=8.45, font=CODE, before=0, after=0.4, color=RGBColor(25, 25, 25))
        p.paragraph_format.left_indent = Cm(0.45)
        p.paragraph_format.right_indent = Cm(0.3)
        p.paragraph_format.line_spacing = 1.03
        shade(p, "ECEFF2")


def formula(doc, lines):
    for line in lines:
        p = para(doc, line, size=10.6, font="Cambria Math", before=1, after=1, align=WD_ALIGN_PARAGRAPH.CENTER)
        p.paragraph_format.left_indent = Cm(0.6)
        p.paragraph_format.right_indent = Cm(0.6)
        shade(p, "F1F4F7")


def figure(doc, name, caption, width=13.2):
    path = IMAGE_DIR / name
    if not path.exists():
        return
    p = doc.add_paragraph()
    p.alignment = WD_ALIGN_PARAGRAPH.CENTER
    p.paragraph_format.space_before = Pt(6)
    p.paragraph_format.space_after = Pt(2)
    p.add_run().add_picture(str(path), width=Cm(width))
    small(doc, caption, italic=True, after=6)


def image_only(doc, name, width=11.5):
    path = IMAGE_DIR / name
    if not path.exists():
        return
    p = doc.add_paragraph()
    p.alignment = WD_ALIGN_PARAGRAPH.CENTER
    p.paragraph_format.space_before = Pt(8)
    p.paragraph_format.space_after = Pt(8)
    p.add_run().add_picture(str(path), width=Cm(width))


def setup_doc():
    doc = Document()
    sec = doc.sections[0]
    sec.top_margin = Cm(1.75)
    sec.bottom_margin = Cm(1.55)
    sec.left_margin = Cm(2.15)
    sec.right_margin = Cm(2.15)
    sec.header_distance = Cm(0.8)
    sec.footer_distance = Cm(0.75)
    styles = doc.styles
    styles["Normal"].font.name = BODY
    styles["Normal"]._element.rPr.rFonts.set(qn("w:eastAsia"), BODY)
    styles["Normal"].font.size = Pt(10.35)
    styles["Normal"].paragraph_format.line_spacing = 1.26
    styles["Normal"].paragraph_format.space_after = Pt(5.4)
    for s in doc.sections:
        s.footer.paragraphs[0].text = "Giáo Trình Material VFX Realtime V15"
        s.footer.paragraphs[0].alignment = WD_ALIGN_PARAGRAPH.CENTER
        for r in s.footer.paragraphs[0].runs:
            set_font(r, size=8.0, name=SANS, color=MUTED)
    return doc


def cover(doc):
    p = para(doc, "Giáo Trình", size=15, font=SANS, bold=True, color=ORANGE, before=46, after=4, align=WD_ALIGN_PARAGRAPH.CENTER)
    para(doc, "Material VFX Realtime", size=30, font=HEAD, bold=True, color=BLUE, after=8, align=WD_ALIGN_PARAGRAPH.CENTER)
    para(doc, "Digital Eyes · Matrix of Art · GPU/CPU Rendering · HLSL/GLSL", size=12, font=BODY, italic=True, color=MUTED, after=18, align=WD_ALIGN_PARAGRAPH.CENTER)
    image_only(doc, "v14_github_pdf_cover.png", width=9.5)
    para(doc, "V15 Long Book Draft · 2026", size=9.5, font=SANS, color=MUTED, before=10, after=0, align=WD_ALIGN_PARAGRAPH.CENTER)
    doc.add_page_break()


def preface(doc):
    h1(doc, "Lời nói đầu")
    body(doc, "Cuốn sách này không cố biến material thành một danh sách node phải học thuộc. Nó xem material như một ngôn ngữ thị giác chạy trên phần cứng thật, bị ép bởi thời gian thật, bị kiểm tra bởi art direction thật, và được cứu rất nhiều lần bởi debug thật.")
    body(doc, "Nếu After Effects giống một bàn dựng nơi bạn có thể kéo thời gian chậm lại để chăm từng frame, realtime shader giống một sân khấu đang diễn. Mọi thứ phải xảy ra trong vài mili giây, dưới exposure của level, qua bloom, qua depth, qua texture compression, qua ngân sách GPU, và qua cả feedback mơ hồ kiểu 'cho nó mạnh hơn nhưng đừng bẩn'.")
    body(doc, "Bản V15 mở rộng theo bốn trục: Digital Eyes, Matrix of Art, cơ bắp CPU/GPU, và ngôn ngữ HLSL/GLSL. Phần còn lại đưa các trục đó vào production: mobile, stylized, iGaming, anime VFX, portal, shield, fire, smoke, dissolve, beam, SubUV, Niagara, Shader Graph và những lỗi khiến một material đẹp trong preview biến thành vấn đề trong game.")
    callout(doc, "Cách đọc", [
        "Đọc chậm phần Digital Eyes. Đây là nơi mắt bắt đầu dịch hình ảnh thành dữ liệu.",
        "Đọc Matrix of Art cùng một material grayscale. Đừng thêm màu quá sớm.",
        "Đọc GPU/CPU khi bạn bắt đầu thấy effect đẹp nhưng frame-time đau.",
        "Đọc HLSL/GLSL khi node graph đã không còn đủ rõ hoặc đủ gọn.",
        "Đọc Production Stories khi bạn muốn hiểu vì sao studio chọn một trick rẻ thay vì một shader hào nhoáng."
    ])
    doc.add_page_break()


SOURCES = [
    "The Book of Shaders: cách dạy fragment shader bằng pixel, tọa độ, color, shaping function, pattern và noise.",
    "Unity URP Unity 6 e-book: shader/VFX recipes, Toon shader, Render Graph, stencil/X-ray, water, volumetric cloud, mobile performance.",
    "Unity Shader Graph và VFX Graph documentation: node-based authoring, scalable realtime effects.",
    "Epic Unreal Engine Material documentation: material graph, parameter expressions, Custom Material Expressions/HLSL.",
    "Epic Niagara documentation và material parameter workflow: Particle Color, Dynamic Parameter, renderer contract.",
    "Real-Time Rendering: graphics pipeline, rasterization, programmable shader stage, GPU pipeline overview.",
    "VFXSamplePack và ForgePilot Knowledge trong workspace: M_Flare, M_Mesh_Add, M_FlamesCard, M_SubUV_Utilities, dissolve, fresnel, portal, forcefield."
]


def sources_page(doc):
    h1(doc, "Nguồn và tinh thần biên soạn")
    body(doc, "Tài liệu này không trích dài từ sách thương mại. Các nguồn dưới đây được dùng để định hướng phạm vi, thuật ngữ và thứ tự học. Nội dung được viết lại theo mục tiêu Technical VFX Artist làm realtime material.")
    for s in SOURCES:
        bullet(doc, s)


def lesson(doc, no, title, story, concept, data, math_lines, graph_lines, production, failures, exercise):
    h1(doc, f"{no}. {title}")
    quote(doc, story, "Production note")
    h2(doc, "Ý chính")
    body(doc, concept)
    h2(doc, "Nhìn bằng dữ liệu")
    body(doc, data)
    if math_lines:
        h2(doc, "Công thức / logic")
        formula(doc, math_lines[:2])
    if graph_lines:
        code(doc, graph_lines, "Sketch node hoặc shader")
    callout(doc, "Production", production, fill="EEF4FA", color=BLUE)
    callout(doc, "Lỗi thường gặp", failures, fill="FBEDE3", color=ORANGE)
    callout(doc, "Bài tập", exercise, fill="EEF7EF", color=GREEN)


DIGITAL = [
    ("Pixel không phải màu, pixel là kết quả", "Một flare trên màn hình không phải 'cái flare'. Nó là kết quả cuối của texture, UV, alpha, blend mode, particle color, exposure, bloom và background.", "Hãy tập nhìn từng pixel như một câu trả lời. Nó trả lời câu hỏi: tại tọa độ này, shader nhận dữ liệu gì, biến đổi nó ra sao, và render pipeline trộn nó vào frame như thế nào?", "Khi một pixel quá sáng, đừng hỏi ngay màu nào đẹp hơn. Hỏi: mask có quá rộng không, emissive có vượt hierarchy không, bloom có đang khuếch đại lỗi không, background có làm additive mất đọc không?"),
    ("Channel là ngăn kéo dữ liệu", "RGBA không nhất thiết là màu đỏ, xanh lá, xanh dương và alpha. Trong VFX, nó thường là bốn cái ngăn kéo nhỏ để cất dissolve, distortion, edge và opacity.", "Channel packing là cách artist nói chuyện với GPU bằng sự tiết kiệm. Một texture fetch có thể mang bốn mask nếu bạn biết mỗi channel đại diện cho điều gì.", "Mắt production phải đọc được texture như một bảng dữ liệu. R có thể là crack, G là breakup, B là normal-ish distortion, A là soft opacity."),
    ("Mask grayscale là bản nhạc chưa phối", "Một mask đen trắng nhìn rất nhạt, nhưng nó quyết định 70% visual. Màu chỉ là phối khí sau cùng.", "Solo grayscale giúp bạn thấy thật: edge có bẩn không, threshold có chạy đúng hướng không, noise có tile quá rõ không, alpha có bị compression phá không.", "Khi visual khó sửa, hãy tắt màu. Nếu grayscale không đọc được, bản màu chỉ đang che lỗi."),
    ("Time là input, không phải phép màu", "Panner, sine, flipbook, curve Niagara đều chỉ là cách đưa thời gian vào shader.", "Thời gian trong VFX cần có nhịp. Fire nhanh ở core, smoke chậm ở edge, magic ring có pulse, shield hit có decay. Nếu mọi layer chạy cùng speed, effect trông giả.", "Hãy xem time như một lane dữ liệu riêng: phase, speed, offset, lifetime, frame index."),
    ("Data contract giữa engine và material", "Material không tự biết gameplay. Niagara, Blueprint, C# hoặc renderer phải đưa data vào.", "Một shader production sống nhờ hợp đồng rõ: ParticleColor.a là fade, DynamicParam.x là dissolve, y là distortion, z là frame control, w là intensity.", "Không có contract, graph đẹp thành hộp đen. Người khác chỉnh curve mà material không đổi, lỗi không nằm ở nghệ thuật mà nằm ở giao tiếp dữ liệu."),
    ("Preview không phải thế giới thật", "Material preview là phòng thử đồ. Game level mới là đường phố.", "Preview thiếu exposure thật, sorting thật, overdraw thật, camera thật, post-process thật, background thật. Vì vậy shader đẹp trong preview chỉ mới qua vòng gửi xe.", "Một Technical VFX Artist luôn test effect trong scene xấu: nền sáng, nền tối, camera gần, camera xa, nhiều particle, low-spec setting."),
    ("Visual hierarchy là dữ liệu ưu tiên", "Người chơi không đọc tất cả detail. Họ đọc silhouette, direction, timing, value contrast trước.", "Trong một projectile, core và direction quan trọng hơn noise nhỏ. Trong portal, ring đọc trước swirl. Trong shield, rim và hit response đọc trước pattern.", "Tối ưu tốt bắt đầu từ hierarchy: giữ cái người xem thật sự thấy, bỏ cái chỉ artist thấy khi pause."),
    ("Field thay vì hình", "Noise, gradient, distance, depth đều là field: ở mỗi điểm có một giá trị.", "Khi bạn nghĩ theo field, dissolve không còn là texture; nó là một trường giá trị bị cắt bởi threshold. Shield hit không còn là decal; nó là distance field quanh điểm va chạm.", "Tư duy field giúp bạn chuyển giữa Unreal, Unity, HLSL, GLSL mà không phụ thuộc node cụ thể."),
]


MATRIX = [
    ("Remap: đổi thang đo, đổi cảm giác", "Remap là phiên dịch. Một giá trị 0.2-0.6 có thể trở thành 0-1, một noise nhạt có thể thành mask rõ.", "Remap giúp art direction có tay cầm. Không expose raw noise nếu artist cần chỉnh contrast, threshold, intensity.", "remapped = saturate((x - inMin) / (inMax - inMin))"),
    ("Power: bóp ánh sáng vào core", "Power làm falloff đổi tính cách. Power cao co vùng sáng, power thấp mở vùng mềm.", "Flare, rim, radial mask, smoke alpha đều dùng Power để đổi cảm giác từ mềm sang sắc.", "y = pow(saturate(x), p)"),
    ("SmoothStep: lưỡi dao mềm", "Step cắt cứng. SmoothStep cắt có độ mềm. VFX realtime dùng SmoothStep liên tục vì hầu hết effect cần edge có kiểm soát.", "Dissolve edge, ring, foam, shield hit, portal đều có thể bắt đầu từ SmoothStep.", "y = smoothstep(edge0, edge1, x)"),
    ("Edge band: vùng sống sát ngưỡng", "Edge band là khoảng giữa sống và chết. Nó là nơi dissolve phát sáng, portal cháy viền, ice growth lóe lên.", "Đừng tô edge bằng outline tùy tiện. Hãy tính nó từ cùng source mask để nó bám đúng logic.", "edge = smoothstep(t-w, t, m) - smoothstep(t, t+s, m)"),
    ("Dot product: camera đang nhìn mặt nào", "Dot không phải công thức khô. Nó trả lời câu hỏi hai hướng có cùng chiều không.", "Fresnel, rim, lighting fake, directional dissolve, view fade đều cần dot.", "facing = saturate(dot(N, V))"),
    ("Distance: khoảng cách tạo shape", "Distance biến không gian thành mask. Circle, sphere, ring, hit ripple, shockwave đều đến từ distance.", "Khi cần effect lan từ điểm va chạm, hãy nghĩ distance trước texture.", "d = length(pos - center)"),
    ("Atan2 và Polar UV", "Polar UV đổi mặt phẳng thành bán kính và góc. Portal, vortex, ring, radial slash sống ở đây.", "Seam polar là lỗi production thật. Hãy biết nó ở đâu để giấu vào vùng ít thấy hoặc xử lý bằng texture.", "angle = atan2(y, x); r = length(float2(x,y))"),
    ("Sine không chỉ để nhấp nháy", "Sine là dao động. Nó tạo pulse, wave, wobble, breathing, flicker có chu kỳ.", "Dùng sine quá đều sẽ giả. Thêm phase offset, noise hoặc curve để motion bớt máy móc.", "pulse = sin(time * speed + phase) * 0.5 + 0.5"),
    ("Frac/Floor: nhịp, ô, frame", "Frac tạo vòng lặp 0-1. Floor chia bậc. Flipbook, scanline, grid, repeating pulse đều cần chúng.", "Khi animation bị giật, hãy kiểm tra frame index, floor, frac và interpolation.", "frame = floor(frac(t * fps) * count)"),
    ("SDF: vẽ bằng khoảng cách", "Signed Distance Field mô tả shape bằng khoảng cách tới biên. Nó rất hợp cho icon, ring, outline, soft edge.", "SDF làm bạn thấy shape như toán học mềm, không phải bitmap chết.", "mask = smoothstep(width, width + soft, abs(sdf))"),
]


GPU = [
    ("CPU chuẩn bị sân khấu", "CPU thường quản lý gameplay, transform, culling, submit draw call và dữ liệu cho GPU.", "Khi nhiều effect spawn cùng lúc, CPU có thể nghẽn vì số emitter, component, draw call, sort, bounds update.", "Một shader rẻ không cứu được hệ thống nếu bạn spawn quá nhiều object nhỏ không batch."),
    ("GPU vẽ hàng loạt", "GPU mạnh vì làm rất nhiều việc giống nhau song song. Fragment shader chạy trên vô số pixel.", "Điều này giải thích vì sao full-screen translucent effect đắt: một công thức nhỏ nhân với rất nhiều pixel.", "Cost = số pixel bị vẽ × số layer × độ nặng shader."),
    ("Rasterization: tam giác thành fragment", "Mesh đi qua vertex stage, được chiếu lên màn hình, rồi rasterizer tạo fragment cho pixel covered.", "Material VFX card là vài tam giác rẻ ở geometry, nhưng có thể rất đắt ở pixel nếu alpha rộng.", "Đừng chỉ hỏi mesh bao nhiêu poly. Hỏi nó che bao nhiêu màn hình."),
    ("Vertex shader và WPO", "WPO đẩy vertex, không đẩy pixel tự do. Mesh ít vertex thì deformation thô.", "WPO còn liên quan bounds. Đẩy quá xa mà bounds không đủ, mesh bị cắt khỏi render/culling.", "WPO đẹp trong preview nhưng mất trong game thường là câu chuyện bounds."),
    ("Pixel shader và overdraw", "Pixel shader quyết định màu/opacity. Translucent VFX thường không ghi depth như opaque, nên pixel có thể bị vẽ nhiều lần.", "Overdraw là kẻ thù chính của fire, smoke, glow, magic aura trên mobile.", "Crop alpha, giảm card area, giảm layer trước khi xóa vài instruction nhỏ."),
    ("Texture bandwidth", "Texture sample không chỉ là node. Nó là dữ liệu phải đọc từ memory/cache.", "Channel packing, resolution đúng, mip đúng, compression đúng thường tối ưu tốt hơn tranh luận thêm bớt một Multiply.", "Mobile/iGaming rất nhạy với bandwidth."),
    ("Branch, loop và shader variant", "Static switch tạo variant compile-time. Dynamic branch chạy runtime.", "Một master material quá nhiều switch có thể thành gánh nặng compile và quản lý.", "Tách family material khi visual domain quá khác nhau."),
    ("Tile-based mobile GPU", "Nhiều mobile GPU tối ưu theo tile. Translucent, overdraw, framebuffer fetch và post-process có thể rất nhạy.", "VFX mobile cần nghĩ theo area, layer, bandwidth và blending nhiều hơn là chỉ instruction count.", "Hiệu ứng nhỏ lặp nhiều lần phải rẻ và nhất quán."),
]


SCRIPTING = [
    ("Hello pixel: fragment shader tối giản", "Shader code không thần bí. Nó nhận input và trả output.", "Hello world của shader là trả một màu. Từ đó mở ra UV, texture, time, noise, normal, depth.", "return float4(1, 0, 0, 1);"),
    ("Types: float, float2, float3, float4", "Vector type là ngôn ngữ mẹ đẻ của shader.", "Color là float3/float4, UV là float2, position/normal là float3. Hiểu type giúp đọc HLSL/GLSL nhanh hơn.", "float2 uv; float3 normal; float4 color;"),
    ("Uniform/parameter", "Parameter là dữ liệu ngoài shader đưa vào.", "Unreal scalar/vector parameter, Unity property, GLSL uniform đều phục vụ cùng ý tưởng: cho runtime/artist điều khiển shader.", "uniform float DissolveAmount;"),
    ("Sampler và texture", "Texture không tự xuất hiện trong shader. Bạn sample nó bằng UV.", "Sai UV, sai sRGB, sai sampler state hoặc sai channel sẽ làm texture đúng file nhưng sai nghĩa.", "float4 tex = Texture.Sample(Sampler, uv);"),
    ("Function nhỏ", "Function giúp shader code có câu chữ.", "Remap, edgeBand, polarUV, rotateUV, hash noise nên thành function nhỏ để debug và reuse.", "float remap01(float x, float a, float b) { return saturate((x-a)/(b-a)); }"),
    ("Loop có kiểm soát", "Loop trong material phải thận trọng. Nó hữu ích cho nhiều hit ripple, multi-sample blur nhỏ, hoặc SDF layer.", "Loop đẹp nếu count nhỏ/cố định và output debug được. Loop xấu nếu che giấu cost.", "for (int i=0; i<HIT_COUNT; ++i) { acc += hit(i); }"),
    ("GLSL vs HLSL", "Tên khác, pipeline khác, nhưng tư duy giống: dữ liệu vào, hàm biến đổi, pixel ra.", "GLSL quen với vec3, mix, fract. HLSL quen với float3, lerp, frac. Đừng bị tên làm rối.", "GLSL mix ≈ HLSL lerp; GLSL fract ≈ HLSL frac."),
    ("Custom node trong Unreal", "Custom node hữu ích khi graph rối hơn code.", "Không dùng HLSL để trông senior. Dùng khi nó làm ý tưởng rõ hơn, hoặc node graph không diễn đạt gọn.", "Output custom node phải có bản grayscale/debug."),
]


PRODUCTION = [
    ("Mobile RPG stylized", "Stylized mobile thường chọn shape rõ, color sạch, timing tốt và texture gọn.", "Vì màn hình nhỏ và device đa dạng, effect phải đọc nhanh ở size bé. Một slash đẹp không cần mô phỏng plasma phức tạp; nó cần silhouette, taper và nhịp.", "Giữ core/edge/tail rõ. Bake motion phụ nếu runtime không cần tương tác."),
    ("Anime action impact", "Anime VFX mạnh nhờ graphic design: hard shape, smear, speed line, impact frame, color accent.", "Shader chỉ là một phần. Mesh shape, flipbook, timing Niagara và camera shake cùng tạo cảm giác.", "Đừng dùng noise hữu cơ quá nhiều nếu art style cần graphic clarity."),
    ("iGaming win effect", "iGaming cần sáng, rõ, vui mắt, lặp nhiều và rẻ.", "Sparkle, coin shine, glow sweep thường nên bake texture/sequence. Runtime chỉ tint, scale, fade, chọn frame.", "Đẹp nhưng rẻ thắng đẹp nhưng nặng."),
    ("AAA readability", "AAA không có nghĩa là mọi thứ phức tạp. Nó có nghĩa là effect sống trong scene phức tạp mà vẫn đọc được.", "Hero VFX phải chịu lighting, camera, motion blur, post-process và gameplay readability.", "Profile trong scene thật, không chỉ turntable."),
    ("Fortnite-like stylized realism", "Một số style thành công vì vừa graphic vừa có vật liệu đủ thật.", "VFX cần màu mạnh, shape rõ, nhưng interaction với depth, lighting, world vẫn có cơ sở.", "Đây là vùng Technical VFX Artist có đất diễn: art direction và shader logic gặp nhau."),
    ("Production feedback", "Lead nói 'organic hơn' không có nghĩa là thêm ba noise.", "Có thể họ muốn silhouette bất quy tắc, edge breakup, timing lệch phase, color variation hoặc spawn distribution.", "Dịch feedback thành lane cụ thể trước khi sửa graph."),
    ("Dirty trick hợp pháp", "Production không thưởng cho shader phức tạp nếu một sprite atlas làm tốt hơn.", "Dùng fake depth, baked distortion, precomputed flipbook, vertex color mask, mesh shell, decal hoặc post-process khi nó phù hợp.", "Best practice thật là đạt visual đúng trong budget."),
    ("Review và naming", "Material production phải được người khác mở ra và hiểu.", "Tên parameter như Amount1, Speed2 làm team mất thời gian. Tên tốt nói vai trò: DissolveAmount, EdgeWidth, CoreIntensity.", "Graph sạch là graph có lane, comment và debug path."),
]


VFX_TOPICS = [
    ("Additive flare", "Flare sống nhờ falloff, core và background contrast.", "mask = pow(texture.a, Power); emissive = tint * mask * intensity"),
    ("Dissolve reveal", "Dissolve là threshold đi qua field.", "visible = smoothstep(amount, amount+soft, noise); edge = band(noise)"),
    ("Fire card", "Fire cần core nóng, edge mềm, motion đi lên và alpha sạch.", "emissive = fire.rgb * ParticleColor.rgb * intensity"),
    ("Smoke flipbook", "Smoke cần alpha mềm, depth fade và motion chậm.", "opacity = alpha * ParticleColor.a * DepthFade"),
    ("Portal ring", "Portal là radial field cộng polar motion.", "ring = 1 - smoothstep(width, width+soft, abs(r-radius))"),
    ("Forcefield", "Shield cần rim, surface body và hit response.", "rim = pow(1-dot(N,V), power); hit = distance(WorldPos, HitPos)"),
    ("Energy beam", "Beam đọc bằng core, edge, flow và head/tail fade.", "core = 1-smoothstep(w,w+s,abs(uv.y-.5))"),
    ("Ice growth", "Ice không chỉ màu xanh: nó là reveal, roughness, normal và edge lạnh.", "visible = smoothstep(amount, amount+soft, gradient+noise)"),
    ("Lava crack", "Lava là contrast giữa crust tối và crack nóng.", "emissive = crackMask * heatColor * heat"),
    ("Wet surface", "Wet làm base tối hơn, roughness thấp hơn, highlight rõ hơn.", "base = lerp(dry, dry*darken, wet); rough = lerp(dryR, wetR, wet)"),
    ("Heat distortion", "Nhiệt thường thấy qua background bị lệch nhẹ.", "screenUV += (noise.rg*2-1)*mask*strength"),
    ("Shockwave", "Shockwave là ring chạy ra theo thời gian.", "ring = 1-smoothstep(width,width+soft,abs(r-timeRadius))"),
]


def build_lessons_from_topics(doc, start_no, topics, prefix):
    no = start_no
    for title, a, b, c in topics:
        lesson(
            doc, no, f"{prefix}: {title}",
            f"Có những ngày một effect hỏng không phải vì thiếu kỹ thuật, mà vì ta gọi sai tên vấn đề. Bài này đặt lại tên cho vấn đề: {title.lower()}.",
            a + " " + b,
            c + " Khi đã nhìn nó như dữ liệu, bạn có thể rebuild ở Unreal, Unity, Shader Graph hay HLSL mà không bị mắc kẹt vào tên node.",
            [b] if "=" in b else [],
            [b] if "=" in b else [],
            [
                "Đặt tên parameter theo vai trò visual, không theo node.",
                "Luôn có cách solo grayscale hoặc debug output.",
                "Test trong scene thật với background sáng/tối."
            ],
            [
                "Sửa bằng cách thêm layer mới trước khi hiểu layer cũ.",
                "Để motion, mask và color cùng một speed nên visual bị phẳng.",
                "Tin preview quá sớm."
            ],
            [
                "Dựng một material nhỏ chỉ có phần logic chính.",
                "Chụp 3 trạng thái: raw mask, edge/debug, final color.",
                "Viết một câu giải thích cost của bài này."
            ]
        )
        no += 1
    return no


def topic_seed(title):
    clean = title.split(":", 1)[-1].strip()
    clean = clean.replace("Deep Dive", "bài đọc sâu")
    return clean[:72]


def varied_lesson(doc, no, title, description, equation=""):
    key = topic_seed(title)
    story_templates = [
        f"{key}: khi mở một graph cũ, đừng vội sửa dây nối. Việc đầu tiên là đoán xem người trước đó đang cố bảo vệ cảm giác visual nào.",
        f"{key}: lead hiếm khi gọi đúng tên node. Họ nói cảm giác chưa đúng, còn TA phải dịch câu đó thành dữ liệu có thể kiểm tra.",
        f"{key}: có những trick nhìn rất nhỏ trong editor nhưng quyết định cả readability. Nó thường bị xem nhẹ cho tới khi phá scene thật.",
        f"{key}: nếu shader là một câu văn, đây là một từ khóa. Dùng đúng thì câu rõ; dùng sai thì cả đoạn graph bắt đầu nói lắp.",
        f"{key}: trong review, câu hỏi hay nhất không phải 'node nào thiếu', mà là nó đang phục vụ shape, motion, color hay cost.",
        f"{key}: một effect đẹp thường có một phần rất bình thường đứng sau, đó là dữ liệu sạch và quyết định rõ.",
        f"{key}: khi deadline sát, artist rất dễ thêm layer. Bài này nhắc ta làm ngược lại: bỏ nhiễu trước, giữ ý chính trước.",
        f"{key}: ở cấp senior, đây không còn là công thức. Nó là quyết định về cost, cảm giác, readability và quyền kiểm soát.",
    ]
    data_templates = [
        f"Với {key}, hãy ghi ra input trước khi mở material: texture/channel nào, UV nào, parameter nào, particle data nào, và output đi vào emissive, opacity hay WPO.",
        f"Đọc {key} bằng năm câu hỏi: giá trị nằm trong range nào, nó có cần clamp không, nó đang ở space nào, ai điều khiển nó, và nó có bị post-process đổi nghĩa không.",
        f"Trong engine thật, {key} không sống riêng. Nó đi qua import setting, material instance, renderer, camera, exposure và platform profile.",
        f"Nếu cần debug {key}, hãy tạo ba màn hình nhỏ: raw data, shaped mask, final composite. Ba màn hình đó thường tìm ra lỗi nhanh hơn 20 phút nhìn spaghetti.",
        f"Coi {key} như một lane. Lane đó có thể bị bẩn ở source, sai ở transform, quá mạnh ở shaping, hoặc bị mất nghĩa khi blend vào frame.",
        f"Đừng mô tả {key} bằng cảm giác trước. Mô tả bằng dữ liệu trước, rồi mới quay lại cảm giác. Thứ tự đó làm feedback bớt mơ hồ.",
    ]
    production_sets = [
        [f"{key}: expose ít parameter nhưng đúng vai trò visual.", "Giữ một debug output để người khác solo được mask.", "Test trên nền sáng, nền tối và scene có bloom thật."],
        [f"{key}: quyết định trước bản high-spec và low-spec.", "Nếu cost đến từ diện tích pixel, giảm card/layer trước.", "Nếu cost đến từ texture, kiểm tra packing, mip và compression."],
        [f"{key}: đặt tên parameter như người khác sẽ dùng nó lúc 2 giờ sáng.", "Group parameter theo Shape, Motion, Color, Interaction, Cost.", "Đừng để material instance thành bảng điều khiển máy bay."],
        [f"{key}: nếu visual cần art-directed timing, để Niagara/curve điều khiển.", "Material nên nhận data rõ thay vì tự đoán gameplay.", "Khi curve không tác dụng, kiểm tra Particle Color/Dynamic Parameter trước."],
        [f"{key}: luôn hỏi effect xuất hiện bao nhiêu lần trên một frame.", "Một hero moment được phép đắt hơn UI sparkle lặp hàng trăm lần.", "Cost phải được đánh giá trong context, không phải trong preview trống."],
        [f"{key}: giữ hierarchy đọc được trước khi thêm chi tiết.", "Core, edge, tail, accent không nên cạnh tranh cùng một value.", "Nếu tắt bloom mà shape biến mất, shader đang mượn bloom quá nhiều."],
    ]
    failure_sets = [
        [f"{key} dễ hỏng khi source mask không được solo.", "Màu và bloom có thể che lỗi threshold.", "Compression có thể phá alpha mỏng."],
        [f"{key} dễ hỏng khi space bị trộn: UV, object, world, view.", "Dữ liệu đúng công thức nhưng sai space vẫn cho visual sai.", "WPO/depth/fresnel đặc biệt nhạy với lỗi này."],
        [f"{key} dễ hỏng vì preview quá sạch.", "Scene thật có exposure, depth, sorting, UI và background phức tạp.", "Hãy test ở điều kiện xấu trước khi gọi là xong."],
        [f"{key} dễ hỏng khi parameter quá rộng range.", "Artist kéo một slider quá xa và shader thành effect khác.", "Clamp hoặc remap range theo mục tiêu visual."],
        [f"{key} dễ hỏng khi được dùng như thuốc chữa mọi feedback.", "Thêm noise không tự động làm organic.", "Thêm intensity không tự động làm mạnh hơn."],
        [f"{key} dễ hỏng khi runtime data không có hợp đồng.", "Niagara/Blueprint đổi giá trị nhưng material không nghe.", "Debug bằng màu giả cho từng channel truyền vào."],
    ]
    exercise_sets = [
        [f"Dựng bản tối giản của {key} trong một material trắng đen.", "Chụp raw mask và final result.", "Viết một câu: lỗi dễ gặp nhất là gì."],
        [f"Làm hai version của {key}: một bản đẹp và một bản rẻ.", "Đo hoặc ước lượng overdraw/texture fetch.", "Ghi tradeoff bằng ngôn ngữ production."],
        [f"Tạo preset subtle, readable, overdone cho {key}.", "So sánh trong scene sáng và tối.", "Xóa preset overdone sau khi hiểu vì sao nó hỏng."],
        [f"Viết pseudo-code cho {key} trước khi nối node.", "Sau đó dựng node graph đúng pseudo-code.", "Nếu graph khó đọc hơn code, cân nhắc Material Function/HLSL."],
        [f"Đưa {key} vào một case mobile/iGaming.", "Giảm layer hoặc bake một phần motion.", "Ghi lại phần nào giữ visual nhiều nhất."],
        [f"Đọc một material thật và tìm dấu vết của {key}.", "Không sửa graph ở lượt đầu.", "Chỉ chia lane và ghi parameter nên expose."],
    ]
    def localize(lines):
        prefixes = [
            f"{key}: ",
            f"Với {key}, ",
            f"Khi xử lý {key}, ",
            f"Trong bài {key}, ",
        ]
        out = []
        for j, line in enumerate(lines):
            if line.startswith(key) or line.startswith(f"Với {key}") or line.startswith(f"Khi xử lý {key}") or line.startswith(f"Trong bài {key}"):
                out.append(line)
            else:
                out.append(prefixes[(no + j) % len(prefixes)] + line[:1].lower() + line[1:])
        return out
    i = no % len(story_templates)
    lesson(
        doc, no, title,
        story_templates[i],
        description,
        data_templates[no % len(data_templates)],
        [equation] if equation else [],
        [equation] if equation else [],
        localize(production_sets[no % len(production_sets)]),
        localize(failure_sets[no % len(failure_sets)]),
        localize(exercise_sets[no % len(exercise_sets)])
    )


def expand_topic(doc, no, title, description, equation):
    varied_lesson(doc, no, title, description, equation)


def core_book(doc):
    h_part(doc, "I", "DIGITAL EYES - Nhìn thế giới qua dữ liệu", "Trước khi học node, học cách nhìn effect như dữ liệu chạy qua thời gian.")
    n = 1
    for t in DIGITAL:
        expand_topic(doc, n, t[0], t[1] + " " + t[2] + " " + t[3], "")
        n += 1
    figure(doc, "v9_shader_lane_diagram.png", "Lane diagram: đọc material theo làn dữ liệu thay vì đuổi theo dây nối.", width=13.2)

    h_part(doc, "II", "THỊ GIÁC TOÁN HỌC - The Matrix of Art", "Toán shader không phải bài kiểm tra; nó là cách nặn cảm giác visual.")
    for t in MATRIX:
        expand_topic(doc, n, t[0], t[1] + " " + t[2], t[3])
        n += 1
    figure(doc, "v9_power_shaping_flare.png", "Power shaping: cùng một radial mask nhưng cảm giác flare đổi hoàn toàn.", width=13.2)
    figure(doc, "v9_mask_solo_dissolve.png", "Solo grayscale giúp kiểm tra logic trước khi màu và bloom che lỗi.", width=13.2)

    h_part(doc, "III", "CƠ BẮP - GPU & CPU Rendering", "Shader đẹp phải sống trên phần cứng thật, không chỉ trên ý tưởng.")
    for t in GPU:
        expand_topic(doc, n, t[0], t[1] + " " + t[2], t[3])
        n += 1
    figure(doc, "v9_shader_complexity_overdraw.png", "Overdraw/complexity: diện tích pixel và số layer thường quan trọng hơn vài instruction nhỏ.", width=13.2)

    h_part(doc, "IV", "NGÔN NGỮ CỦA THẦN THÁNH - HLSL/GLSL Scripter", "Khi bạn biết code shader, node graph không còn là nhà tù.")
    for t in SCRIPTING:
        expand_topic(doc, n, t[0], t[1] + " " + t[2], t[3])
        n += 1

    h_part(doc, "V", "Material VFX Grammar", "Các hệ material production: UV, mask, blend, depth, SubUV, WPO, Niagara contract.")
    grammar = []
    bases = [
        ("UV motion", "UV là địa chỉ đọc texture. Panner, rotator, polar, flow map đều là cách thay địa chỉ đó theo thời gian.", "uv = uv + speed * time"),
        ("Mask grammar", "Mask quyết định vùng nào sống, chết, mềm, sắc, cháy edge hoặc bị clip.", "visible = smoothstep(t, t+s, mask)"),
        ("Blend mode", "Additive, translucent, masked không chỉ là mỹ thuật. Chúng đổi cách GPU trộn pixel và đổi cost.", "final = src * alpha + dst * (1-alpha)"),
        ("Fresnel/rim", "Rim cho biết camera nhìn bề mặt ở góc nghiêng hay trực diện.", "rim = pow(1-saturate(dot(N,V)), power)"),
        ("DepthFade", "DepthFade làm mềm giao tuyến giữa translucent card và geometry.", "opacity *= saturate((sceneDepth-pixelDepth)/fadeDistance)"),
        ("SubUV flipbook", "Flipbook giao animation chính cho texture atlas và renderer.", "frameUV = localUV / grid + frameOffset"),
        ("WPO", "World Position Offset thay đổi vertex để mesh thở, rung, ripple hoặc dissolve có hình.", "pos += normal * offset"),
        ("Channel packing", "Một texture có thể mang nhiều mask nếu channel được quy ước rõ.", "R=dissolve; G=edge; B=distort; A=opacity"),
        ("Particle Color", "Particle Color là dây điều khiển màu/alpha cơ bản từ Niagara vào material.", "emissive *= ParticleColor.rgb; opacity *= ParticleColor.a"),
        ("Dynamic Parameter", "Dynamic Parameter là 4 lane float runtime cho material VFX.", "x=dissolve; y=distort; z=frame; w=intensity"),
    ]
    for b in bases:
        grammar.append(b)
    for t in grammar:
        expand_topic(doc, n, t[0], t[1], t[2])
        n += 1
    figure(doc, "v11_realtime_contract_simple.png", "Niagara-Material contract: nếu dữ liệu không đi qua, curve đẹp cũng vô nghĩa.", width=13.2)

    h_part(doc, "VI", "Production Stories - Mobile, Stylized, AAA, iGaming", "Câu chuyện production giúp shader không bị tách khỏi thị trường và art direction.")
    for t in PRODUCTION:
        expand_topic(doc, n, t[0], t[1] + " " + t[2], t[3])
        n += 1

    h_part(doc, "VII", "Recipes - Từ ý đồ tới graph", "Recipe không phải copy node. Recipe là đường đi từ visual intent tới dữ liệu.")
    for title, desc, eq in VFX_TOPICS:
        expand_topic(doc, n, title, desc, eq)
        n += 1
    figure(doc, "v11_imagegen_vfx_atlas.png", "Atlas concept dùng để đọc nhóm effect: flare, dissolve, portal, shield.", width=13.2)

    h_part(doc, "VIII", "Failure Atlas - Học từ những ca đau", "Một TA giỏi không chỉ dựng được effect; họ lần ngược được lỗi.")
    failures = [
        ("Smoke cắt vào sàn", "Triệu chứng: card smoke giao với ground như bị kéo dao. Nguyên nhân thường là thiếu DepthFade, alpha quá rộng hoặc pivot/sorting chưa ổn.", "opacity = DepthFade(alpha, fadeDistance)"),
        ("Fire cháy trắng", "Triệu chứng: trong preview đẹp nhưng vào level thành mảng trắng. Nguyên nhân thường là emissive hierarchy kém, exposure/bloom khuếch đại quá mạnh.", "emissive = min(core + edge, maxBudget)"),
        ("Shield mất khi nhìn gần", "Triệu chứng: mesh biến mất hoặc bị cắt khi WPO/ripple mạnh. Nguyên nhân có thể là bounds.", "increase bounds or reduce WPO amplitude"),
        ("Dissolve răng cưa", "Triệu chứng: edge bị stair-step. Nguyên nhân: threshold quá cứng, mask resolution thấp, compression hoặc masked AA không đủ.", "visible = smoothstep(t, t+soft, mask)"),
        ("Flipbook sai frame", "Triệu chứng: fire nhảy ô hoặc đọc cả atlas. Nguyên nhân: grid mismatch, SubUV setup sai, frame interpolation không đúng.", "check grid, frame count, local 0-1 UV"),
        ("Mobile tụt FPS khi win effect", "Triệu chứng: nhiều sparkle làm frame-time tăng. Nguyên nhân: overdraw, spawn count, texture lớn, additive layer nhiều.", "reduce area, bake, pack, cap spawn"),
        ("Polar seam lộ", "Triệu chứng: portal có đường rách. Nguyên nhân: atan2/angle wrap seam đi qua vùng thấy rõ.", "hide seam, rotate seam, mask seam"),
        ("Material Instance khó dùng", "Triệu chứng: artist chỉnh parameter nhưng không hiểu kết quả. Nguyên nhân: tên mơ hồ, range sai, group kém.", "name by visual role and range"),
    ]
    for t in failures:
        expand_topic(doc, n, t[0], t[1], t[2])
        n += 1
    figure(doc, "v9_failure_depthfade_before_after.png", "Failure Atlas: smoke hard intersection trước và sau DepthFade.", width=13.2)

    h_part(doc, "IX", "Labs - Luyện như production", "Bài tập giúp biến từ vựng thành phản xạ.")
    labs = [
        ("Lab 1: 30 mask grayscale", "Mỗi ngày dựng một mask từ gradient/noise/distance. Không màu, không bloom.", ""),
        ("Lab 2: 10 shaping functions", "So sánh Power, SmoothStep, remap, contrast trên cùng một radial mask.", ""),
        ("Lab 3: Portal không texture", "Dùng distance, atan2, sine và noise nhẹ để dựng ring.", ""),
        ("Lab 4: Fire card baseline", "Dùng flipbook/texture giả, Particle Color, DepthFade, CameraFade.", ""),
        ("Lab 5: Shield hit", "Dùng Fresnel và distance-to-hit position để tạo ripple.", ""),
        ("Lab 6: Mobile variant", "Tạo bản high và low: tắt distortion, giảm layer, pack mask.", ""),
        ("Lab 7: Đọc M_Flare", "Mở material thật, chia lane, viết breakdown.", ""),
        ("Lab 8: Đọc M_SubUV_Utilities", "Dùng NumberGrid để phân biệt atlas UV và local frame UV.", ""),
        ("Lab 9: HLSL edgeBand", "Viết function edgeBand nhỏ rồi so với graph node.", ""),
        ("Lab 10: Production review", "Đưa effect vào scene sáng/tối, đo overdraw, ghi lại lỗi.", ""),
    ]
    for t in labs:
        expand_topic(doc, n, t[0], t[1], t[2])
        n += 1

    # Extra deepening pass: make the book genuinely long without blank pages.
    h_part(doc, "X", "Deep Dives - Bài đọc sâu", "Mỗi bài đào sâu một từ vựng production và gắn nó với một quyết định thật.")
    deep_topics = []
    seeds = DIGITAL + MATRIX + GPU + SCRIPTING
    for round_idx in range(5):
        for base in seeds:
            name = base[0]
            deep_topics.append((
                f"Deep Dive {round_idx+1}: {name}",
                f"{name} không đứng một mình. Ở lần đọc sâu này, hãy xem nó trong ba bối cảnh: authoring, runtime và review.",
                f"Khi authoring, bạn cần giá trị dễ thấy. Khi runtime, bạn cần dữ liệu ổn định. Khi review, bạn cần giải thích bằng visual language thay vì chỉ nói tên node.",
                base[-1] if "=" in base[-1] else "",
            ))
    for t in deep_topics:
        expand_topic(doc, n, t[0], t[1] + " " + t[2], t[3])
        n += 1

    h_part(doc, "XI", "Production Casebook - Chuyện thật trong nhiều style", "Các case ngắn giúp nối shader với art direction, platform và thị trường.")
    game_lines = [
        ("Stylized open-world action", "Nhóm game kiểu Genshin/Honkai thường cần VFX sạch, màu có hierarchy, shape anime rõ và motion dễ đọc trên mobile lẫn PC.", "giữ shape lớn, giảm noise nhỏ"),
        ("Battle royale stylized", "Nhóm game kiểu Fortnite dùng visual graphic, readable từ xa, không quá realistic nhưng interaction với world vẫn rõ.", "silhouette trước, detail sau"),
        ("Tactical shooter", "Nhóm game kiểu Valorant cần VFX rất kỷ luật: đẹp nhưng không che thông tin gameplay.", "opacity budget và readability"),
        ("MOBA", "Nhóm game kiểu League/Dota cần skill đọc trong combat đông, camera xa, nhiều layer chồng nhau.", "shape, team color, timing"),
        ("Mobile RPG", "Mobile RPG cần spectacle nhưng phải scale được trên thiết bị yếu.", "bake, atlas, cap spawn"),
        ("iGaming/casino", "iGaming cần win effect sáng, rõ, rẻ, lặp hàng nghìn lần mà không mỏi mắt.", "sprite sequence và packed mask"),
        ("Anime slash game", "Slash anime cần timing và graphic shape hơn là simulation phức tạp.", "hard shape, smear, taper"),
        ("Realistic military", "Realistic VFX cần material phản ứng với lighting, smoke/dust có depth và color temperature đúng.", "depth, exposure, tonemapper"),
        ("AR/mobile camera", "Effect bám camera thật cần kiểm soát contrast và lighting không đoán trước.", "background test và clamp"),
        ("UI hero moment", "UI VFX cần alpha sạch, size ổn định, không phụ thuộc depth scene.", "bake và deterministic timing"),
        ("Cinematic trailer", "Trailer có thể đắt hơn runtime, nhưng nếu asset quay lại game phải tách high/low path.", "hero path và runtime path"),
        ("Live-service event", "Event VFX cần dễ reskin, đổi màu, đổi icon, đổi motif mà không rewrite shader.", "parameter groups và palette"),
    ]
    effects = [
        ("flare", "core quá cháy làm mất icon chính", "pow falloff + intensity clamp"),
        ("dissolve", "edge chạy sai hướng với animation", "gradient + noise controlled"),
        ("portal", "polar seam lộ giữa màn hình", "rotate seam + mask"),
        ("shield", "hit ripple không đọc trong combat", "stronger value hierarchy"),
        ("beam", "beam che gameplay", "tail/head fade + opacity cap"),
        ("smoke", "smoke cắt sàn và che UI", "DepthFade + crop alpha"),
        ("coin shine", "highlight nhìn như layer AE dán lên", "mask theo asset contour"),
        ("ice growth", "chỉ tint xanh nên không ra ice", "roughness/normal/reveal edge"),
        ("lava", "toàn bề mặt cháy như plastic", "crack mask + dark crust"),
        ("shockwave", "ring dày mỏng thất thường", "width independent of speed"),
        ("heat distortion", "background bị bẩn", "masked subtle distortion"),
        ("projectile", "không đọc hướng bay", "core/shell/trail separation"),
    ]
    case_id = 1
    for line in game_lines:
        for eff in effects[:6]:
            title = f"Case {case_id}: {line[0]} - {eff[0]}"
            desc = f"{line[1]} Với effect dạng {eff[0]}, lỗi hay gặp là {eff[1]}. Quyết định production nên bắt đầu từ {line[2]}, sau đó mới chọn texture, node hoặc HLSL."
            eq = eff[2]
            expand_topic(doc, n, title, desc, eq)
            n += 1
            case_id += 1
    h_part(doc, "XII", "Appendix - Lookup nhanh", "Trang tra cứu khi đang làm material và không muốn đọc lại cả chương.")
    lookups = [
        ("Feedback dictionary", "Mạnh hơn = tăng contrast/value hierarchy/timing. Organic hơn = breakup silhouette/phase/noise distribution. Cao cấp hơn = hierarchy sạch, ít bẩn, interaction tốt.", ""),
        ("Parameter naming", "Amount, Width, Softness, Intensity, Tiling, Speed, Phase, DistortionStrength, EdgeColor, FadeDistance.", ""),
        ("Debug modes", "Raw UV, Raw Mask, Visible, Edge, Opacity before fade, DepthFade factor, ParticleColor, DynamicParameter.", ""),
        ("Mobile checklist", "Area nhỏ, layer ít, alpha crop, packed texture, mip đúng, sRGB data off, spawn capped.", ""),
        ("Unreal/Unity mapping", "Lerp/Mix, Frac/Fract, Saturate/Clamp01, Dynamic Parameter/Custom data, Niagara/VFX Graph.", ""),
        ("When to use HLSL", "Dùng khi graph lặp, thuật toán rõ hơn bằng code, hoặc cần function nhỏ tái dùng. Không dùng để che một graph chưa hiểu.", ""),
    ]
    for t in lookups:
        expand_topic(doc, n, t[0], t[1], t[2])
        n += 1
    h_part(doc, "XIII", "Kết luận - Từ node sang tư duy", "Mục tiêu cuối cùng không phải thuộc nhiều node hơn, mà là nhìn được vấn đề trước khi mở graph.")
    h1(doc, "331. Con đường tiếp theo")
    body(doc, "Nếu đọc hết cuốn này, bạn không cần nhớ mọi công thức ngay. Điều cần giữ lại là cách phân rã vấn đề: visual intent, dữ liệu vào, không gian, mask, motion, color, interaction và cost. Khi lead nói effect chưa đủ lực, bạn không còn đoán mò. Bạn hỏi nó thiếu lực ở silhouette, timing, value contrast, camera interaction hay gameplay readability.")
    body(doc, "Technical VFX Artist mạnh không phải vì biết nhiều trick nhất. Họ mạnh vì biết trick nào đáng dùng, trick nào nên bỏ, và khi nào nên nói với team rằng vấn đề không nằm ở material. Có lúc câu trả lời là shader. Có lúc là texture authoring. Có lúc là Niagara timing. Có lúc là art direction. Có lúc là GPU budget.")
    body(doc, "Từ đây, hướng học hợp lý là chọn một effect thật trong project, viết breakdown một trang, dựng lại bản tối giản, rồi làm ba biến thể: đẹp hơn, rẻ hơn, và dễ tune hơn. Nếu bạn làm được ba biến thể đó, bạn không chỉ học material; bạn đang học production.")
    callout(doc, "Checklist cuối", [
        "Luôn solo grayscale trước khi tin final color.",
        "Luôn biết dữ liệu đang ở UV, object, world, view hay screen space.",
        "Luôn test trong scene thật, không chỉ preview.",
        "Luôn có low-spec thought, kể cả khi chưa phải build mobile.",
        "Luôn viết parameter để người khác dùng được."
    ], fill="EEF4FA", color=BLUE)


def build():
    doc = setup_doc()
    cover(doc)
    preface(doc)
    sources_page(doc)
    core_book(doc)
    OUT.parent.mkdir(parents=True, exist_ok=True)
    doc.save(OUT)
    print(OUT)


if __name__ == "__main__":
    build()
