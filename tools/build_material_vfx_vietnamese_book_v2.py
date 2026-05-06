from pathlib import Path

from docx import Document
from docx.enum.text import WD_ALIGN_PARAGRAPH
from docx.oxml import OxmlElement
from docx.oxml.ns import qn
from docx.shared import Cm, Pt, RGBColor


OUT = Path.home() / "Downloads" / "Giao_Trinh_Material_VFX_Realtime_V2.docx"

INK = RGBColor(30, 30, 30)
MUTED = RGBColor(92, 92, 92)
BLUE = RGBColor(20, 68, 105)
TEAL = RGBColor(0, 112, 112)
GOLD = RGBColor(142, 91, 12)
RED = RGBColor(145, 45, 45)
PURPLE = RGBColor(85, 70, 125)
LIGHT_BLUE = "EAF3F8"
LIGHT_TEAL = "E8F4F3"
LIGHT_GOLD = "FFF3D6"
LIGHT_RED = "FCE8E6"
LIGHT_GRAY = "F3F5F7"
LIGHT_PURPLE = "F0EEF8"


def set_font(run, size=10, bold=False, italic=False, color=INK, name="Arial"):
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


def border_bottom(paragraph, color="D7E3EC", size="8"):
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


def para(doc, text="", size=10.4, color=INK, bold=False, italic=False, before=0, after=4.6, align=None):
    p = doc.add_paragraph()
    p.paragraph_format.space_before = Pt(before)
    p.paragraph_format.space_after = Pt(after)
    p.paragraph_format.line_spacing = 1.12
    if align is not None:
        p.alignment = align
    if text:
        r = p.add_run(text)
        set_font(r, size=size, bold=bold, italic=italic, color=color)
    return p


def chapter(doc, number, title, promise=None):
    p = para(doc, f"CHƯƠNG {number} - {title.upper()}", size=17, bold=True, color=BLUE, before=13, after=6)
    border_bottom(p, color="BBD1E1", size="12")
    if promise:
        callout(doc, "Mục tiêu của chương", [promise], fill=LIGHT_BLUE, color=BLUE)


def h1(doc, title):
    p = para(doc, title, size=14.2, bold=True, color=BLUE, before=9, after=5)
    border_bottom(p)


def h2(doc, title):
    para(doc, title, size=11.5, bold=True, color=TEAL, before=7, after=3)


def h3(doc, title, color=PURPLE):
    para(doc, title, size=10.5, bold=True, color=color, before=5, after=2)


def body(doc, text):
    para(doc, text, size=10.55, after=4.8)


def bullet(doc, text, level=0):
    p = doc.add_paragraph()
    p.paragraph_format.left_indent = Cm(0.55 + level * 0.35)
    p.paragraph_format.first_line_indent = Cm(-0.25)
    p.paragraph_format.space_after = Pt(2.6)
    p.paragraph_format.line_spacing = 1.08
    r = p.add_run("- " + text)
    set_font(r, size=10.0)


def numbered(doc, i, text):
    p = doc.add_paragraph()
    p.paragraph_format.left_indent = Cm(0.72)
    p.paragraph_format.first_line_indent = Cm(-0.42)
    p.paragraph_format.space_after = Pt(2.6)
    p.paragraph_format.line_spacing = 1.08
    r = p.add_run(f"{i}. {text}")
    set_font(r, size=10.0)


def callout(doc, title, lines, fill=LIGHT_BLUE, color=BLUE):
    p = para(doc, title, size=9.8, bold=True, color=color, before=5, after=1)
    p.paragraph_format.left_indent = Cm(0.22)
    p.paragraph_format.right_indent = Cm(0.18)
    shade(p, fill)
    for line in lines:
        p = doc.add_paragraph()
        p.paragraph_format.left_indent = Cm(0.52)
        p.paragraph_format.right_indent = Cm(0.18)
        p.paragraph_format.space_after = Pt(1.8)
        p.paragraph_format.line_spacing = 1.06
        shade(p, fill)
        r = p.add_run("- " + line)
        set_font(r, size=9.65)
    para(doc, "", after=1)


def code(doc, lines, title=None):
    if title:
        para(doc, title, size=8.9, color=MUTED, italic=True, after=1)
    for line in lines:
        p = para(doc, line, size=8.9, after=1.0)
        p.paragraph_format.left_indent = Cm(0.35)
        p.paragraph_format.right_indent = Cm(0.18)
        shade(p, LIGHT_GRAY)
        for run in p.runs:
            set_font(run, size=8.9, name="Consolas")


def ascii_box(doc, lines, title=None):
    code(doc, lines, title)


def lesson(doc, title, why, explanation, model, formula, mistakes, exercise, recap):
    h1(doc, title)
    callout(doc, "Bạn học để làm gì", why, fill=LIGHT_BLUE, color=BLUE)
    for paragraph in explanation:
        body(doc, paragraph)
    if model:
        ascii_box(doc, model, "Sơ đồ tư duy")
    if formula:
        code(doc, formula, "Công thức / graph logic")
    if mistakes:
        callout(doc, "Lỗi thường gặp", mistakes, fill=LIGHT_RED, color=RED)
    if exercise:
        callout(doc, "Bài tập nhỏ", exercise, fill=LIGHT_GOLD, color=GOLD)
    if recap:
        callout(doc, "Ghi nhớ", recap, fill=LIGHT_TEAL, color=TEAL)


def worked_recipe(doc, title, goal, story, graph, params, debug, optimize, exercise):
    h1(doc, title)
    callout(doc, "Mục tiêu visual", [goal], fill=LIGHT_BLUE, color=BLUE)
    for p in story:
        body(doc, p)
    ascii_box(doc, graph, "Graph đọc theo lane")
    h2(doc, "Parameter nên expose")
    for item in params:
        bullet(doc, item)
    callout(doc, "Cách debug", debug, fill=LIGHT_TEAL)
    callout(doc, "Cách tối ưu", optimize, fill=LIGHT_GOLD, color=GOLD)
    callout(doc, "Bài tập production", exercise, fill=LIGHT_PURPLE, color=PURPLE)


def cover(doc):
    para(doc, "", before=4)
    para(doc, "GIÁO TRÌNH MATERIAL VFX REALTIME", size=22.5, bold=True, color=BLUE, align=WD_ALIGN_PARAGRAPH.CENTER, after=2)
    para(doc, "Bản V2 - học material như học một ngôn ngữ visual", size=12.2, color=INK, align=WD_ALIGN_PARAGRAPH.CENTER, after=8)
    callout(doc, "Lời hứa của quyển này", [
        "Không biến bạn thành người nhớ thật nhiều node. Nó giúp bạn nhìn một effect và biết phải bóc shape, mask, motion, color, interaction và cost như thế nào.",
        "Mỗi chương đi từ lý do visual tới công thức, rồi tới Unreal/Niagara và bài tập. Unity hoặc engine khác chỉ là cách gọi node khác; tư duy data vẫn giữ nguyên.",
        "Quyển này viết cho người đã làm VFX, từng quen AE/Spine/game production, và muốn bước sâu hơn vào Technical VFX Artist.",
    ], fill=LIGHT_GOLD, color=GOLD)
    para(doc, "Biên soạn dựa trên ForgePilot Knowledge, Unreal Material/Niagara patterns và VFXSamplePack", size=8.8, color=MUTED, align=WD_ALIGN_PARAGRAPH.CENTER, before=4)


def toc(doc):
    h1(doc, "Mục lục định hướng")
    entries = [
        "1. Từ AE sang realtime shader: đổi cách nhìn từ hình đã render sang hệ thống chạy mỗi frame.",
        "2. Material là pipeline data-to-pixel: texture, shader, material instance, renderer, Niagara.",
        "3. Từ vựng shader: Add, Multiply, Power, Clamp, Lerp, SmoothStep, Dot, Distance, Sine, Polar.",
        "4. UV và chuyển động: TexCoord, Panner, Rotator, flow map, distortion, radial space.",
        "5. Mask grammar: threshold, dissolve, edge band, contrast, channel packing.",
        "6. Color, opacity và blend mode: emissive, alpha, additive, translucent, masked.",
        "7. Particle data và SubUV: Particle Color, Dynamic Parameter, ParticleSubUV, derived 0-1 UV.",
        "8. Depth, Fresnel, WPO và interaction: soft particle, shield, hit ripple, mesh deformation.",
        "9. Recipe production: flare, dissolve, fire card, smoke flipbook, portal, forcefield, beam, projectile.",
        "10. Đọc VFXSamplePack: M_Flare, M_Mesh_Add, M_FlamesCard, M_SubUV_Utilities.",
        "11. Optimization và production review: overdraw, texture fetch, shader instruction, parameter hygiene.",
        "12. Texture authoring và atlas: texture như dữ liệu shader, flipbook, import settings.",
        "13. Unreal material properties: Blend Mode, Shading Model, Two Sided, feature đắt.",
        "14. Từ điển node chuyên sâu theo production.",
        "15. Troubleshooting theo triệu chứng.",
        "16. Lộ trình luyện tập: 6 tuần và nhật ký 30 ngày.",
        "17. Workshop thực hành có hướng dẫn.",
        "18. Dịch feedback thành hành động shader.",
        "19. Glossary production để tra nhanh.",
        "20. Phụ lục công thức phải nhớ.",
    ]
    for e in entries:
        bullet(doc, e)
    callout(doc, "Cách đọc", [
        "Đọc chương 1-2 trước để có khung đầu. Sau đó học chương 3-8 như học từ vựng và ngữ pháp.",
        "Tới recipe, đừng copy công thức ngay. Hãy che phần graph, tự đoán lane trước, rồi mới so lại.",
        "Khi rảnh ở ngoài máy, chỉ cần đọc phần giải thích và sơ đồ. Khi ngồi Unreal, làm bài tập nhỏ ngay.",
    ], fill=LIGHT_TEAL)
    doc.add_page_break()


def build():
    doc = Document()
    section = doc.sections[0]
    section.page_width = Cm(21)
    section.page_height = Cm(29.7)
    section.top_margin = Cm(1.5)
    section.bottom_margin = Cm(1.3)
    section.left_margin = Cm(1.85)
    section.right_margin = Cm(1.85)
    doc.styles["Normal"].font.name = "Arial"
    doc.styles["Normal"].font.size = Pt(10)
    footer = section.footer.paragraphs[0]
    footer.alignment = WD_ALIGN_PARAGRAPH.CENTER
    r = footer.add_run("Giáo Trình Material VFX Realtime V2")
    set_font(r, size=7.8, color=MUTED)

    cover(doc)
    toc(doc)

    chapter(doc, 1, "Đổi cách nhìn: từ AE sang realtime shader", "Hiểu vì sao AE làm texture rất mạnh, còn game engine mạnh ở hệ thống realtime, interaction và tối ưu.")
    lesson(
        doc,
        "1. AE vẽ ra kết quả, material tạo ra luật chơi",
        [
            "Phân biệt khi nào nên dùng AE để author texture và khi nào nên dựng logic trong Unreal/Unity.",
            "Hiểu vì sao realtime material khó hơn: nó sống trong camera, gameplay, renderer và performance budget.",
        ],
        [
            "Trong AE, bạn thường kéo noise, glow, twirl, color correction, keyframe và render ra một chuỗi hình. Bạn đang điều khiển kết quả cuối khá trực tiếp. Với material realtime, bạn không chỉ làm một hình đẹp; bạn viết một luật nhỏ để GPU áp dụng lên rất nhiều pixel mỗi frame. Luật đó có thể nhận lifetime từ particle, depth từ scene, vị trí trong world, màu từ Niagara, hoặc thông số gameplay.",
            "Vì vậy, shader không thay thế AE theo kiểu cái nào hơn cái nào. AE giống xưởng vẽ texture và flipbook rất nhanh. Unreal/Unity giống sân khấu nơi texture đó được đưa vào hệ tương tác. Một flame flipbook có thể sinh từ AE, EmberGen hoặc Houdini; nhưng khi vào engine, material vẫn phải quyết định alpha, depth fade, emissive, camera fade, color over life và cost.",
            "Nếu bạn từng làm AE tốt, đó là lợi thế lớn. Bạn đã có mắt về timing, contrast, glow, layer và motion. Việc học material là chuyển khả năng đó thành data flow: thay vì keyframe một layer, bạn tìm dữ liệu nào có thể điều khiển cùng cảm giác đó trong runtime.",
        ],
        [
            "AE mindset:      layer -> effect -> keyframe -> render frame",
            "Realtime mindset: data -> material math -> renderer -> frame hiện tại",
            "",
            "Texture authoring tốt + Material runtime tốt = VFX production mạnh",
        ],
        [
            "AE noise render -> flipbook texture",
            "Unreal material: ParticleSubUV * ParticleColor -> Emissive/Opacity",
            "Niagara: lifetime, size, frame, color, spawn count",
        ],
        [
            "Nghĩ rằng shader chỉ là AE node graph khác tên. Shader bị ràng buộc bởi GPU, blend mode, depth, renderer và platform.",
            "Cố làm mọi thứ procedural trong engine dù texture bake sẽ rẻ và đẹp hơn.",
            "Bake mọi thứ thành flipbook dù chỉ cần một panner/noise đơn giản là đủ.",
        ],
        [
            "Lấy một effect AE cũ của bạn. Viết ra cái nào nên bake thành texture, cái nào nên để Niagara điều khiển, cái nào nên nằm trong material.",
        ],
        [
            "AE là nơi vẽ tài nguyên rất nhanh. Engine là nơi tài nguyên đó trở thành interactive effect.",
            "Technical VFX Artist giỏi biết chia trách nhiệm giữa texture, material, particle system và gameplay.",
        ],
    )
    lesson(
        doc,
        "2. Sáu lớp để bóc mọi effect",
        [
            "Có một bộ câu hỏi cố định khi nhìn bất kỳ effect nào.",
            "Không bị lạc trong graph lớn vì biết từng nhóm node phục vụ lớp nào.",
        ],
        [
            "Khi nhìn một material phức tạp, đừng bắt đầu bằng việc hỏi node này là gì. Hãy hỏi effect này đang được xây từ những lớp nào. Một VFX realtime gần như luôn có sáu lớp: shape, mask, motion, color, interaction và cost.",
            "Shape là silhouette lớn: vòng tròn portal, tia beam, card lửa, shell shield. Mask quyết định vùng nào hiện, vùng nào mất, cạnh mềm hay gắt. Motion là cảm giác sống: UV chảy, frame flipbook, rotation, WPO pulse. Color là hierarchy ánh sáng và hue. Interaction là phần effect phản ứng với scene/camera/gameplay. Cost là cái giá để mọi thứ chạy trong combat thật.",
            "Khi lead nói 'làm mạnh hơn', họ hiếm khi nói chính xác layer nào. Công việc của bạn là dịch feedback đó. Mạnh hơn có thể là scale lớn hơn, core sáng hơn, edge rõ hơn, timing nhanh hơn, hoặc silhouette đọc tốt hơn. Nếu bạn không bóc layer, bạn sẽ chỉnh lung tung và mất thời gian.",
        ],
        [
            "Effect",
            "  -> Shape: hình lớn đọc trong 0.5 giây",
            "  -> Mask: vùng hiện/mất và cạnh chuyển tiếp",
            "  -> Motion: UV, frame, particle, mesh, time",
            "  -> Color: hue, value, emissive, bloom hierarchy",
            "  -> Interaction: depth, camera, hit position, gameplay data",
            "  -> Cost: overdraw, samples, instructions, particles",
        ],
        [],
        [
            "Chỉnh màu khi vấn đề thật ra là silhouette.",
            "Thêm noise detail khi mask lớn chưa đọc.",
            "Tối ưu shader instruction trong khi overdraw mới là nguyên nhân nặng.",
        ],
        [
            "Mở một effect bất kỳ trong VFXSamplePack. Viết sáu dòng: shape, mask, motion, color, interaction, cost. Không cần mở graph quá lâu; tập nhìn visual trước.",
        ],
        [
            "Sáu lớp này là 'ngữ pháp' của VFX material. Càng luyện, bạn càng đọc graph nhanh.",
        ],
    )

    chapter(doc, 2, "Material là pipeline data-to-pixel", "Nắm sự khác nhau giữa texture, shader, material, material instance, renderer và Niagara data.")
    lesson(
        doc,
        "3. Texture, shader, material, material instance",
        [
            "Nói đúng tên từng phần để không nhầm khi debug.",
            "Biết khi nào lỗi nằm ở texture, khi nào ở material, khi nào ở renderer/Niagara.",
        ],
        [
            "Texture là dữ liệu hình ảnh: RGB, alpha, noise, normal, atlas frame hoặc packed mask. Shader là chương trình GPU xử lý dữ liệu đó. Material là asset authoring trong engine, thường bằng node graph, để sinh shader. Material Instance là bản điều khiển parameter mà không phá graph gốc.",
            "Renderer là nơi material được vẽ: sprite, mesh, ribbon, decal hoặc post-process. Niagara/VFX Graph là nơi sinh particle và gửi dữ liệu vào material. Một lỗi nhìn thấy trên màn hình có thể đến từ bất kỳ chỗ nào trong chuỗi này. Nếu flipbook đọc sai frame, material có thể không sai; renderer SubUV có thể sai grid. Nếu particle không fade, graph có thể thiếu Particle Color alpha, hoặc Niagara không gửi alpha.",
            "Một artist shader giỏi không chỉ biết node. Họ biết hợp đồng giữa các phần. Texture phải chứa data đúng. Material phải dùng data đúng. Instance phải expose control đúng. Renderer phải bind material đúng. Particle system phải gửi dữ liệu đúng.",
        ],
        [
            "Texture / Mesh / Particle Data",
            "          -> Material Graph",
            "          -> Shader compiled by engine",
            "          -> Renderer draws sprite/mesh/ribbon/decal",
            "          -> Final pixels under camera, lighting, depth, exposure",
        ],
        [
            "emissive = TextureRGB * ParticleColor.rgb * EmissiveIntensity",
            "opacity  = TextureAlpha * ParticleColor.a * DepthFade",
        ],
        [
            "Sửa master material khi chỉ cần chỉnh Material Instance.",
            "Đổ lỗi cho shader khi texture import sai sRGB/compression/alpha.",
            "Quên rằng renderer có thể override material hoặc SubUV settings.",
        ],
        [
            "Tạo một material sprite rất đơn giản: texture * ParticleColor. Sau đó chỉ dùng Material Instance và Niagara Color over Life để tạo ba biến thể khác nhau.",
        ],
        [
            "Một material không đứng một mình. Nó nằm trong một hợp đồng production giữa texture, instance, renderer và particle system.",
        ],
    )
    lesson(
        doc,
        "4. Đọc graph theo lane thay vì theo dây nối",
        [
            "Có phương pháp đọc material graph lớn mà không bị rối.",
            "Biết nhóm node theo mục đích visual.",
        ],
        [
            "Graph lớn thường đáng sợ vì dây nối chằng chịt. Nhưng đa số graph production có thể đọc theo lane. Lane UV tạo tọa độ sample texture. Lane Mask tạo vùng hiện/mất. Lane Color tạo hue và intensity. Lane Opacity xử lý alpha và blend. Lane Interaction đọc depth, camera hoặc world data. Lane WPO làm mesh biến dạng. Lane Performance là nơi bạn hỏi cái gì đang đắt.",
            "Khi đọc material của người khác, hãy tìm output trước: Emissive, Opacity, Opacity Mask, WPO, Refraction. Sau đó lần ngược về nguồn data. Nếu thấy một texture sample, hỏi nó dùng UV nào. Nếu thấy SmoothStep/Power, hỏi nó đang reshape mask nào. Nếu thấy Particle Color, hỏi RGB và Alpha đi vào đâu.",
            "Cách đọc này giống solo layer trong AE. Bạn cô lập từng nhóm để hiểu vai trò, sau đó mới nhìn tổng thể. Trong Unreal, cách solo nhanh là nối tạm một lane vào Emissive hoặc Base Color ở grayscale.",
        ],
        [
            "Output: Emissive / Opacity / WPO",
            "  <- Color lane",
            "  <- Mask lane",
            "  <- UV lane",
            "  <- Particle/Depth/World data",
            "",
            "Không đọc graph như mê cung. Đọc như bản đồ đường ống.",
        ],
        [],
        [
            "Đọc từ trái sang phải và bị lạc vào chi tiết nhỏ.",
            "Không solo grayscale nên không biết mask thật đang ra sao.",
            "Không phân biệt lane UV và lane Mask, dẫn tới sửa sai chỗ.",
        ],
        [
            "Trong M_Flare hoặc một material sprite bất kỳ, vẽ lại lane diagram bằng chữ: source texture, UV, mask, color, opacity, fade.",
        ],
        [
            "Một graph dễ hiểu không phải vì ít node. Nó dễ hiểu vì mỗi lane có mục đích rõ.",
        ],
    )

    chapter(doc, 3, "Từ vựng shader: node là động từ", "Hiểu các node toán học như cách biến đổi data, không học vẹt tên node.")
    lessons_math = [
        (
            "5. Add, Multiply, Power: ba động từ đầu tiên",
            [
                "Hiểu Add là dịch giá trị, Multiply là scale/gate, Power là bẻ cong response.",
                "Biết vì sao ba node này xuất hiện trong gần như mọi material VFX.",
            ],
            [
                "Add không chỉ là cộng số. Trong material, Add thường dịch một ngưỡng, đẩy UV, tăng sáng, hoặc cộng hai lớp energy. Multiply là node vừa đơn giản vừa nguy hiểm: nó scale intensity, nhân mask, gate vùng hiện, nhân màu, nhân alpha. Power là cách bạn thay đổi cảm giác falloff: mềm hơn, gắt hơn, tập trung core hơn.",
                "Nếu bạn xem mask như ảnh grayscale, Add làm mask sáng/tối hơn, Multiply làm mask mạnh/yếu hơn hoặc kết hợp hai điều kiện, Power làm vùng sáng bị ép về core hoặc vùng tối lan rộng hơn. Với VFX, đây là ngôn ngữ căn bản để biến một texture bình thường thành một response có ý đồ.",
            ],
            [
                "Add:      x + offset        -> dịch ngưỡng hoặc UV",
                "Multiply: x * scale/mask    -> tăng/giảm hoặc gate",
                "Power:    pow(x, exponent)  -> đổi curve response",
            ],
            [
                "alphaTight = pow(alpha, AlphaPower)",
                "emissive   = color * mask * intensity",
                "uv         = TexCoord * Tiling + Offset",
            ],
            [
                "Dùng Power trên giá trị âm hoặc chưa clamp có thể tạo kết quả khó đoán.",
                "Multiply nhiều màu RGB với nhau có thể làm màu bẩn và tối.",
                "Add intensity không kiểm soát dễ làm emissive cháy trắng.",
            ],
            [
                "Lấy một alpha texture. Xuất alpha gốc, alpha * 2, pow(alpha, 2), pow(alpha, 0.5) ra grayscale để cảm nhận bằng mắt.",
            ],
            [
                "Ba node này là động từ cơ bản: dịch, scale, bẻ cong.",
            ],
        ),
        (
            "6. Clamp, Saturate, Lerp, Step, SmoothStep",
            [
                "Hiểu nhóm node biến giá trị liên tục thành control an toàn.",
                "Biết khác biệt giữa Step cứng và SmoothStep mềm.",
            ],
            [
                "Clamp/Saturate giống hàng rào. Nó giữ giá trị trong 0-1 để mask không chạy quá vùng an toàn. Lerp là pha trộn giữa A và B bằng alpha. Step biến một mask mềm thành ngưỡng cứng: dưới threshold là 0, trên threshold là 1. SmoothStep cũng tạo ngưỡng, nhưng có vùng chuyển mềm giữa hai mốc.",
                "Dissolve, reveal, edge band, clip mask, color ramp đều dựa rất nhiều vào nhóm này. Nếu Add/Multiply/Power là động từ thường ngày, thì Step/SmoothStep là dao cắt trong shader. Cắt quá cứng thì aliasing, cắt quá mềm thì visual thiếu lực.",
            ],
            [
                "noise grayscale",
                "   -> Step(threshold)       = cắt cứng",
                "   -> SmoothStep(a, b)      = cắt mềm",
                "   -> Lerp(colorA,colorB,t) = pha màu theo mask",
            ],
            [
                "visible = smoothstep(Amount, Amount + Softness, mask)",
                "edge = smoothstep(Amount - Width, Amount, mask) - smoothstep(Amount, Amount + Softness, mask)",
                "color = lerp(EdgeColor, CoreColor, coreMask)",
            ],
            [
                "Dùng Step cho smoke/fire alpha thường tạo cạnh răng cưa.",
                "Softness quá lớn làm dissolve mất cảm giác ăn mòn.",
                "Lerp alpha chưa clamp khiến màu blend ngoài ý muốn.",
            ],
            [
                "Dựng một dissolve grayscale bằng SmoothStep. Sau đó đổi sang Step để thấy sự khác biệt ở edge.",
            ],
            [
                "SmoothStep là một trong những công cụ quan trọng nhất để tạo mask production-friendly.",
            ],
        ),
        (
            "7. Dot, Distance, Length: vector không đáng sợ",
            [
                "Hiểu vector math bằng hình dung visual thay vì công thức khô.",
                "Dùng Dot cho hướng nhìn/Fresnel, Distance/Length cho radial/ripple.",
            ],
            [
                "Vector là mũi tên hoặc vị trí. Dot product đo hai hướng giống nhau bao nhiêu. Nếu normal bề mặt gần hướng nhìn, dot cao; nếu bề mặt quay ngang với camera, dot thấp. Từ đó ta tạo Fresnel: vùng rìa sáng hơn vì normal ít hướng vào camera.",
                "Distance đo khoảng cách giữa hai điểm. Length đo độ dài một vector. Một radial mask đơn giản chỉ là lấy UV - center rồi đo length. Hit ripple cũng vậy: lấy distance giữa WorldPosition và HitPosition, sau đó dùng SmoothStep để lấy một band mỏng.",
            ],
            [
                "Fresnel: dot(Normal, ViewDir) -> 1 - value -> Power",
                "Circle:  length(UV - Center)",
                "Ripple:  distance(WorldPos, HitPos)",
            ],
            [
                "facing = dot(normalize(N), normalize(V))",
                "rim = pow(1 - saturate(facing), RimPower)",
                "d = distance(WorldPos, HitPosition)",
                "ring = smoothstep(Radius - Width, Radius, d) - smoothstep(Radius, Radius + Width, d)",
            ],
            [
                "Dot hai vector khác space: một cái tangent, một cái world.",
                "Quên normalize vector trước khi dot.",
                "Distance trong world space nhưng HitPosition truyền vào là local/object space.",
            ],
            [
                "Tạo Fresnel thủ công bằng Dot. Sau đó tạo hit ripple trên sphere bằng WorldPosition distance.",
            ],
            [
                "Vector math trong VFX thường trả lời hai câu: hướng nào và xa bao nhiêu.",
            ],
        ),
        (
            "8. Sine, Cosine, Frac, Polar: tạo nhịp và vòng",
            [
                "Biết cách tạo pulse, loop, radial motion và vortex cơ bản.",
                "Hiểu vì sao periodic math cần được kiểm soát để không bị máy móc.",
            ],
            [
                "Sine và Cosine tạo dao động. Chúng rất hợp cho pulse, shimmer, wave, breathing energy. Nhưng một sine trần thường quá đều, dễ nhìn giả. Vì vậy production hay nhân sine với noise mask, particle random hoặc curve để phá sự hoàn hảo.",
                "Frac giữ phần lẻ của một giá trị, tạo loop 0-1. Polar UV chuyển UV phẳng thành radius và angle. Với portal, black hole, shockwave hoặc magic circle, polar space giúp motion chạy quanh tâm thay vì chạy ngang màn hình.",
            ],
            [
                "Time -> Sine -> pulse 0-1",
                "UV - 0.5 -> atan2 = angle",
                "UV - 0.5 -> length = radius",
                "float2(angle, radius) -> polar texture sample",
            ],
            [
                "pulse = sin(Time * Speed) * 0.5 + 0.5",
                "p = UV - 0.5",
                "r = length(p)",
                "a = atan2(p.y, p.x) / TWO_PI",
                "polarUV = float2(a + Time * Spin, r)",
            ],
            [
                "Pulse quá đều làm effect nhìn như máy.",
                "Polar seam lộ nếu texture không tile theo trục angle.",
                "Gần tâm polar UV có thể bị méo mạnh.",
            ],
            [
                "Tạo một ring có noise chạy quanh tâm bằng polar UV. Sau đó thêm noise nhỏ nhân vào speed hoặc intensity để bớt đều.",
            ],
            [
                "Periodic math tạo nhịp. Polar math tạo không gian vòng.",
            ],
        ),
    ]
    for args in lessons_math:
        lesson(doc, *args)

    chapter(doc, 4, "UV và chuyển động", "Biết tạo cảm giác sống bằng cách điều khiển tọa độ đọc texture.")
    lesson(
        doc,
        "9. TexCoord và Panner: chuyển động rẻ nhất",
        [
            "Hiểu UV là bản đồ đọc texture, không phải bản thân texture.",
            "Dùng panner có chủ đích để tạo flow nhưng không làm effect bị trượt như giấy.",
        ],
        [
            "UV là tọa độ hỏi texture: pixel này nên đọc chỗ nào trên ảnh. TexCoord thường là UV gốc của sprite/mesh. Panner chỉ là UV cộng Time * Speed. Nghe đơn giản, nhưng phần lớn motion VFX rẻ đều bắt đầu từ đây.",
            "Một panner đơn thường lộ vì toàn bộ texture trượt cùng tốc độ. Để motion giàu hơn, dùng hai layer: một noise lớn chậm tạo dòng chính, một noise nhỏ nhanh tạo chi tiết. Khi nhân hoặc blend hai layer, mắt sẽ thấy chuyển động hữu cơ hơn.",
            "Trong sprite VFX, panner thường dùng cho fire, energy crawl, smoke drift, water ribbon. Với mesh, hãy kiểm UV unwrap trước. Nếu UV stretch, panner sẽ stretch theo.",
        ],
        [
            "TexCoord -> scale/tiling -> panner -> texture sample",
            "Layer A: big noise, slow",
            "Layer B: small noise, fast/opposite",
            "A * B -> breakup sống hơn",
        ],
        [
            "uvA = UV * 2 + Time * float2(0, 0.2)",
            "uvB = UV * 8 - Time * float2(0.1, 0.7)",
            "mask = NoiseA(uvA) * NoiseB(uvB)",
        ],
        [
            "Pan cả silhouette mask làm hình lớn trượt mất kiểm soát.",
            "Speed quá cao gây shimmer.",
            "Không dùng checker/NumberGrid để debug hướng UV trước khi sample noise.",
        ],
        [
            "Dựng một energy card bằng hai panner. Sau đó tắt từng layer để cảm nhận vai trò layer lớn và layer nhỏ.",
        ],
        [
            "Panner là rẻ, nhưng panner tốt phải có hierarchy và không làm lộ texture trượt.",
        ],
    )
    lesson(
        doc,
        "10. Rotator, pivot và radial motion",
        [
            "Dùng rotation đúng tâm để tạo flare, portal, magic ring.",
            "Hiểu lỗi pivot sai và seam khi xoay/radial sample.",
        ],
        [
            "Rotator xoay UV quanh một pivot. Nếu pivot là 0.5,0.5 trên sprite card, texture xoay quanh tâm. Nếu pivot sai, texture vừa xoay vừa lắc lệch. Với flare hoặc magic circle, pivot sai là lỗi nhìn thấy ngay.",
            "Rotation thường chỉ nên áp vào layer detail hoặc radial pattern. Nếu xoay cả atlas/flipbook không đúng cách, bạn có thể làm frame lookup sai. Với SubUV, cần hiểu UV local trong từng frame trước khi làm distortion hoặc rotation riêng.",
            "Portal và black hole thường cần motion quanh tâm hơn là pan ngang. Khi đó bạn có thể dùng Rotator trên radial texture, hoặc chuyển hẳn sang polar UV để angle trở thành trục ngang của texture.",
        ],
        [
            "UV -> subtract Pivot -> rotate -> add Pivot -> sample",
            "",
            "Nếu pivot lệch:",
            "texture không chỉ xoay, nó còn orbit quanh điểm sai",
        ],
        [
            "centered = UV - Pivot",
            "rotated = mul(centered, RotationMatrix(Angle)) + Pivot",
            "sample = Texture(rotated)",
        ],
        [
            "Pivot 0,0 thay vì 0.5,0.5 trên sprite.",
            "Xoay texture atlas trước khi cắt frame.",
            "Rotation quá đều làm portal nhìn cơ khí.",
        ],
        [
            "Dùng NumberGrid xoay quanh pivot 0.5,0.5 rồi pivot 0,0. Ghi lại cảm giác visual khác nhau.",
        ],
        [
            "Rotation là motion quanh tâm; pivot là gốc của cảm giác đó.",
        ],
    )
    lesson(
        doc,
        "11. Flow map và distortion: làm texture biết chảy",
        [
            "Hiểu distortion không phải thêm noise bừa vào UV.",
            "Dùng flow map/noise để làm fire, water, shield, portal sống hơn.",
        ],
        [
            "Distortion là thay đổi UV trước khi sample texture. Nếu thêm một noise nhỏ vào UV, texture bị méo. Nếu noise đó có hướng và thời gian, texture có cảm giác bị dòng chảy kéo đi. Flow map là texture hai kênh thường chứa vector hướng, giúp UV chảy theo direction phức tạp hơn panner thẳng.",
            "Với VFX, distortion nên phục vụ visual chính. Fire card có thể dùng noise để làm lưỡi lửa vặn nhẹ, nhưng nếu distortion quá mạnh silhouette sẽ mất đọc. Portal có thể dùng distortion ở interior, trong khi ring edge vẫn phải rõ. Shield có thể méo pattern bề mặt nhưng không nên làm hit ripple sai vị trí.",
        ],
        [
            "UV base",
            "  + distortionVector * strength",
            "  -> sample texture",
            "",
            "Distortion tốt: tăng cảm giác sống",
            "Distortion xấu: phá shape chính",
        ],
        [
            "flow = FlowTexture(UV).rg * 2 - 1",
            "distortedUV = UV + flow * DistortionStrength",
            "mask = Noise(distortedUV)",
        ],
        [
            "Distortion quá mạnh làm texture bị bơi.",
            "Dùng noise grayscale làm vector mà không remap về -1..1.",
            "Distort cả alpha silhouette khi chỉ cần distort detail bên trong.",
        ],
        [
            "Tạo một fire card có silhouette ổn định, chỉ distort detail noise bên trong. So với bản distort toàn bộ alpha.",
        ],
        [
            "Distortion là gia vị. Dùng đúng thì effect sống, dùng quá tay thì effect mất hình.",
        ],
    )

    chapter(doc, 5, "Mask grammar", "Xây mask như xây câu: nguồn mask, threshold, contrast, edge, combine.")
    lesson(
        doc,
        "12. Mask là nơi visual được viết rõ nhất",
        [
            "Nhìn grayscale mask để hiểu effect trước khi thêm màu.",
            "Biết tách mask lớn, mask detail và mask edge.",
        ],
        [
            "Trong material VFX, mask quan trọng ngang hoặc hơn màu. Mask quyết định vùng nào sống, vùng nào chết, vùng nào sáng, vùng nào bị xé, vùng nào mềm vào scene. Nếu mask chưa đẹp ở grayscale, thêm màu, bloom hoặc distortion chỉ làm vấn đề khó thấy hơn.",
            "Một mask production thường có nhiều cấp. Mask lớn giữ silhouette đọc rõ. Mask detail tạo breakup. Mask edge tạo vùng chuyển tiếp có thể phát sáng. Nếu bạn lấy một noise duy nhất làm tất cả, effect dễ bẩn hoặc mất hierarchy.",
            "Hãy tập debug bằng cách nối mask ra Emissive. Trắng là hiện/mạnh, đen là mất/yếu, xám là vùng chuyển. Mắt bạn sẽ học rất nhanh nếu thường xuyên nhìn grayscale.",
        ],
        [
            "Mask lớn: silhouette / vùng chính",
            "Mask detail: noise / breakup",
            "Mask edge: vùng quanh threshold",
            "",
            "FinalMask = BigShape * DetailBreakup * Fade",
        ],
        [
            "visible = smoothstep(Amount, Amount + Softness, noise)",
            "edge = smoothstep(Amount - Width, Amount, noise) - smoothstep(Amount, Amount + Softness, noise)",
            "opacity = shapeMask * visible * ParticleColor.a",
        ],
        [
            "Một noise làm cả shape và detail.",
            "Không solo mask nên sửa màu thay vì sửa shape.",
            "Edge band quá rộng làm effect thành viền cartoon ngoài ý muốn.",
        ],
        [
            "Tạo ba output grayscale riêng: shapeMask, detailMask, edgeMask. Sau đó mới combine thành final.",
        ],
        [
            "Mask là nơi bạn viết câu 'pixel nào được tồn tại và mạnh bao nhiêu'.",
        ],
    )
    lesson(
        doc,
        "13. Dissolve từ ngưỡng tới edge band",
        [
            "Hiểu dissolve như một ngưỡng chạy qua height/noise field.",
            "Tạo edge emissive đúng logic thay vì tô viền tùy tiện.",
        ],
        [
            "Dissolve không phải là làm alpha giảm đều. Dissolve là một threshold chạy qua một field. Field có thể là noise, height map, vertex color, world-space gradient hoặc texture paint. Khi Amount tăng, vùng có giá trị thấp hơn Amount biến mất hoặc hiện ra tùy hướng logic.",
            "Edge band là vùng nằm ngay quanh threshold. Đây là nơi có thể thêm emissive, ember, frost line, magic outline. Công thức tốt thường lấy hai SmoothStep trừ nhau để ra một dải mỏng. Edge band tách khỏi visible mask giúp bạn điều khiển độ dày và độ sáng riêng.",
        ],
        [
            "Noise/Height field",
            "  -> Visible threshold",
            "  -> Edge band quanh threshold",
            "  -> Opacity + Emissive edge",
        ],
        [
            "visible = smoothstep(Amount, Amount + Softness, mask)",
            "edgeA = smoothstep(Amount - EdgeWidth, Amount, mask)",
            "edgeB = smoothstep(Amount, Amount + Softness, mask)",
            "edge = saturate(edgeA - edgeB)",
            "emissive = BaseColor * visible + EdgeColor * edge * EdgeIntensity",
        ],
        [
            "Dùng opacity fade đều nên không có cảm giác ăn mòn.",
            "Edge lấy từ OneMinus visible nên edge quá rộng và không nằm quanh threshold.",
            "Không expose Softness và EdgeWidth riêng.",
        ],
        [
            "Dựng dissolve reveal và dissolve destroy từ cùng một mask. Chỉ đảo logic bằng OneMinus hoặc đổi hướng Amount.",
        ],
        [
            "Dissolve đẹp đến từ field tốt, threshold rõ và edge band có kiểm soát.",
        ],
    )
    lesson(
        doc,
        "14. Channel packing: texture là bộ nhớ dữ liệu",
        [
            "Dùng R/G/B/A như bốn ngăn data thay vì chỉ nhìn texture là ảnh màu.",
            "Biết lợi ích và rủi ro khi pack mask.",
        ],
        [
            "Trong VFX, rất nhiều texture không phải ảnh để nhìn trực tiếp. Nó là container. Kênh R có thể là alpha shape, G là noise detail, B là edge mask, A là soft opacity. Channel packing giúp giảm số texture sample và memory, nhưng đổi lại cần naming và documentation rõ.",
            "Nếu một material dùng packed texture, ComponentMask trở thành node đọc dữ liệu. Khi debug, hãy nối từng kênh ra grayscale. Đừng đoán Alpha là opacity nếu texture authoring không nói vậy. Production bug rất hay đến từ việc repurpose một kênh mà material khác vẫn đang dùng.",
        ],
        [
            "PackedTexture",
            "  R -> Shape",
            "  G -> Breakup",
            "  B -> Edge/Detail",
            "  A -> Soft Alpha hoặc Height",
        ],
        [
            "shape = Packed.r",
            "breakup = Packed.g",
            "edgeSource = Packed.b",
            "alpha = Packed.a",
            "final = shape * smoothstep(Amount, Amount + Softness, breakup)",
        ],
        [
            "Không ghi rõ channel meaning trong tên texture/material comment.",
            "Import sRGB sai cho mask texture.",
            "Dùng compression làm mask bị banding.",
        ],
        [
            "Tạo một material debug có parameter chọn hiển thị R, G, B hoặc A của packed texture.",
        ],
        [
            "Channel packing không chỉ là tối ưu. Nó là cách tổ chức data.",
        ],
    )

    chapter(doc, 6, "Color, opacity và blend mode", "Hiểu vì sao cùng một texture nhưng Additive, Translucent, Masked cho cảm giác hoàn toàn khác.")
    lesson(
        doc,
        "15. Emissive không chỉ là làm sáng",
        [
            "Tạo hierarchy core/edge/glow thay vì tăng intensity toàn bộ.",
            "Biết khi nào bloom đang giúp và khi nào đang che lỗi.",
        ],
        [
            "Emissive trong VFX là ngôn ngữ năng lượng. Nhưng nếu bạn chỉ nhân toàn bộ texture với intensity cao, effect sẽ trắng bệt. Visual tốt thường có hierarchy: core sáng nhất, edge hoặc body thấp hơn, detail vừa đủ, glow phụ trợ. Mắt người đọc thứ tự này rất nhanh.",
            "Bloom làm vùng sáng lan ra, nhưng bloom không sửa được mask xấu. Nếu không bloom mà effect không có shape rõ, bật bloom chỉ làm một đám sáng mờ. Vì vậy nên debug emissive ở intensity vừa phải, trên background sáng và tối, trước khi đẩy bloom.",
        ],
        [
            "Core mask -> high emissive",
            "Body mask -> medium emissive",
            "Edge mask -> accent emissive",
            "ParticleColor -> tint/fade theo lifetime",
        ],
        [
            "core = pow(mask, CorePower)",
            "edge = edgeBand * EdgeIntensity",
            "emissive = CoreColor * core * CoreIntensity + EdgeColor * edge",
        ],
        [
            "Tăng intensity để che silhouette yếu.",
            "Mọi layer cùng sáng nên không có điểm nhấn.",
            "Quên nhân alpha/lifetime vào emissive với Additive.",
        ],
        [
            "Tạo flare có core, mid glow, outer falloff bằng ba mask khác nhau. Bật/tắt từng lớp để xem hierarchy.",
        ],
        [
            "Emissive tốt là ánh sáng có cấu trúc, không phải số intensity lớn.",
        ],
    )
    lesson(
        doc,
        "16. Additive, Translucent, Masked: luật hòa với background",
        [
            "Chọn blend mode theo mục tiêu visual và cost.",
            "Biết các bẫy opacity/fade thường gặp trong particle material.",
        ],
        [
            "Additive cộng ánh sáng vào background. Nó hợp flare, spark, energy, magic glow, nhưng trên nền sáng có thể mất đọc. Translucent pha alpha với background, hợp smoke, fire mềm, glass, portal interior, nhưng dễ tốn overdraw và sorting. Masked cắt cứng theo ngưỡng, hợp dissolve surface, decal stylized hoặc cutout rõ, nhưng edge cần xử lý để không răng cưa.",
            "Một lỗi phổ biến: trong material Additive, bạn nối alpha vào Opacity rồi tự hỏi vì sao fade không đúng. Tùy setup, visual fade chính phải đi vào Emissive multiplier. Với Translucent, Opacity là lane quan trọng. Với Masked, Opacity Mask và clip threshold quyết định pixel sống/chết.",
        ],
        [
            "Additive:    Final = Scene + Emissive",
            "Translucent: Final = lerp(Scene, Material, Opacity)",
            "Masked:      Pixel sống nếu OpacityMask > ClipValue",
        ],
        [
            "additiveEmissive = textureRGB * ParticleColor.rgb * ParticleColor.a * Intensity",
            "transOpacity = textureA * ParticleColor.a * DepthFade",
            "maskedClip = smoothstep(Amount, Amount + Softness, mask)",
        ],
        [
            "Dùng Additive cho smoke dày làm smoke biến mất trên nền sáng.",
            "Dùng Translucent quá nhiều layer gây overdraw nặng.",
            "Dùng Masked cho edge mềm nhưng không có dither/AA support phù hợp.",
        ],
        [
            "Dùng cùng một radial texture tạo ba material: Additive, Translucent, Masked. Viết cảm giác visual khác nhau.",
        ],
        [
            "Blend mode là luật xã hội của pixel: nó quyết định pixel hòa vào thế giới ra sao.",
        ],
    )

    chapter(doc, 7, "Particle data và SubUV", "Kết nối material với Niagara: color, alpha, custom data, lifetime và flipbook atlas.")
    lesson(
        doc,
        "17. Particle Color là hợp đồng sống còn",
        [
            "Không để Niagara color/alpha curve bị vô hiệu vì material quên Particle Color.",
            "Hiểu RGB và Alpha của Particle Color nên đi vào lane nào.",
        ],
        [
            "Particle Color là cầu nối đơn giản nhất giữa Niagara và material. Nếu material không dùng Particle Color, nhiều curve trong Niagara sẽ không có tác dụng. RGB thường tint emissive/base color. Alpha thường scale opacity hoặc emissive fade.",
            "Với Additive, alpha cần được nhân vào emissive nếu bạn muốn particle fade. Với Translucent, alpha thường đi vào Opacity. Nhiều production bug đến từ việc material preview đẹp, nhưng khi vào Niagara thì Color over Life không hoạt động vì graph không nhận Particle Color.",
        ],
        [
            "Niagara Color over Life",
            "      -> Particle Color node",
            "      -> RGB tint",
            "      -> A fade opacity/emissive",
        ],
        [
            "emissive = TextureRGB * ParticleColor.rgb * Intensity",
            "opacity = TextureA * ParticleColor.a",
            "additiveEmissive = emissive * ParticleColor.a",
        ],
        [
            "Chỉ dùng ParticleColor.rgb mà bỏ alpha.",
            "Nhân Particle Color nhiều lần làm màu bị sai.",
            "Niagara renderer binding sai nhưng lại sửa material.",
        ],
        [
            "Tạo một sprite additive. Trong Niagara, animate alpha về 0. Nếu không fade, sửa material cho tới khi fade đúng.",
        ],
        [
            "Particle Color là dây thần kinh giữa emitter và material.",
        ],
    )
    lesson(
        doc,
        "18. Dynamic Parameter: bốn kênh ít nhưng quý",
        [
            "Gửi control từ Niagara vào material mà không tạo nhiều material instance.",
            "Đặt naming ổn định để reuse trong production.",
        ],
        [
            "Dynamic Parameter cho material bốn float R/G/B/A từ particle system. Nó rất hữu ích khi mỗi particle cần control riêng: alpha power, depth fade distance, distortion strength, dissolve amount, emissive exponent. Nhưng vì chỉ có bốn kênh, naming phải có kỷ luật.",
            "Một family fire/smoke có thể quy ước R là PowerEmissive, G là FadeDistance, B là CameraFadeLength, A là AlphaPower. Khi quy ước ổn định, Niagara modules và material instances dễ reuse. Khi mỗi material dùng R/G/B/A theo ý riêng, project rất nhanh thành hỗn loạn.",
        ],
        [
            "DynamicParameter",
            "  R -> PowerEmissive",
            "  G -> FadeDistance",
            "  B -> CameraFadeLength",
            "  A -> AlphaPower",
        ],
        [
            "alpha = pow(TextureAlpha, DynamicParameter.a)",
            "opacity = DepthFade(alpha * ParticleColor.a, DynamicParameter.g)",
            "emissive = pow(TextureRGB, DynamicParameter.r) * Intensity",
        ],
        [
            "Đổi ý nghĩa kênh giữa các materials cùng family.",
            "Dùng Dynamic Parameter cho control global mà Material Instance đã đủ.",
            "Không set default neutral nên material preview bị sai.",
        ],
        [
            "Dựng một material smoke có DynamicParameter.A điều khiển alpha power. Animate A theo lifetime trong Niagara.",
        ],
        [
            "Dynamic Parameter mạnh vì ít. Ít nên phải đặt tên và dùng có kỷ luật.",
        ],
    )
    lesson(
        doc,
        "19. SubUV và derived 0-1 UV",
        [
            "Hiểu ParticleSubUV khác gì sample atlas thủ công.",
            "Biết ý nghĩa derived 0-1 UV khi cần thao tác trong từng frame atlas.",
        ],
        [
            "SubUV là workflow dùng atlas nhiều frame cho particle sprite. Niagara sprite renderer biết frame nào đang chạy, material dùng ParticleSubUV để đọc đúng frame. Nếu grid sai, frame sai, material có thể hoàn toàn đúng nhưng output vẫn sai.",
            "Derived 0-1 UV nghĩa là từ UV atlas lớn, bạn lấy lại UV local bên trong frame hiện tại: góc trái dưới frame là 0,0 và góc phải trên frame là 1,1. Nó hữu ích khi bạn cần áp một hiệu ứng trong từng frame, ví dụ radial fade local, edge fade local hoặc distortion không làm lệch sang frame bên cạnh.",
            "Trong VFXSamplePack, M_SubUV_Utilities và NumberGrid giúp bạn thấy điều này trực quan. NumberGrid 1-9 là texture test tuyệt vời: nếu frame hoặc local UV sai, bạn nhìn ra ngay.",
        ],
        [
            "Atlas 3x3:",
            "+---+---+---+",
            "| 1 | 2 | 3 |",
            "+---+---+---+",
            "| 4 | 5 | 6 |",
            "+---+---+---+",
            "| 7 | 8 | 9 |",
            "+---+---+---+",
            "",
            "Atlas UV = tọa độ toàn sheet",
            "Derived 0-1 UV = tọa độ local trong frame hiện tại",
        ],
        [
            "rgb = ParticleSubUV.rgb",
            "alpha = ParticleSubUV.a",
            "localEdgeFade = LocalFrameUV.x * (1 - LocalFrameUV.x) * LocalFrameUV.y",
        ],
        [
            "SubImageSize trong Niagara không khớp atlas.",
            "Pan UV atlas làm sample tràn sang frame khác.",
            "Không dùng NumberGrid để test frame order.",
        ],
        [
            "Dùng NumberGrid 3x3. Bật SubUV trong Niagara, sau đó thử tạo fade theo local UV trong từng frame.",
        ],
        [
            "ParticleSubUV đọc frame. Derived 0-1 UV cho bạn thao tác an toàn bên trong frame đó.",
        ],
    )

    chapter(doc, 8, "Depth, Fresnel, WPO và interaction", "Đưa material ra khỏi texture phẳng để tương tác với camera, scene và mesh.")
    lesson(
        doc,
        "20. DepthFade và soft particles",
        [
            "Làm particle mềm khi cắt qua geometry.",
            "Hiểu DepthFade không thay thế mask tốt.",
        ],
        [
            "Sprite smoke hoặc fire card cắt qua mặt đất thường tạo đường giao cứng rất giả. DepthFade dùng khoảng cách depth giữa pixel particle và scene để giảm opacity ở giao tuyến. Nó làm particle hòa vào môi trường hơn.",
            "Nhưng DepthFade không phải phép màu. Nếu alpha mask của bạn quá cứng, hoặc card quá lớn, hoặc overdraw quá dày, DepthFade chỉ giảm một lỗi. Bạn vẫn cần mask tốt, size hợp lý, và particle placement tốt.",
        ],
        [
            "Particle pixel depth",
            "Scene geometry depth",
            "Difference nhỏ -> fade opacity xuống",
            "Difference lớn -> giữ opacity",
        ],
        [
            "soft = DepthFade(OpacityInput, FadeDistance)",
            "opacity = alpha * ParticleColor.a * soft",
        ],
        [
            "FadeDistance quá lớn làm particle bị rỗng.",
            "Chỉ dùng DepthFade trên effect không cần soft intersection.",
            "Quên test trong level thật có geometry.",
        ],
        [
            "Đặt smoke card xuyên qua cube. Test FadeDistance 2, 20, 100 và ghi cảm giác.",
        ],
        [
            "DepthFade xử lý giao tuyến. Nó không sửa toàn bộ material.",
        ],
    )
    lesson(
        doc,
        "21. Fresnel: đọc rim, shell và forcefield",
        [
            "Hiểu Fresnel bằng dot normal-view thay vì chỉ kéo node.",
            "Dùng Fresnel cho shield, rim glow, hologram, portal shell.",
        ],
        [
            "Fresnel làm vùng rìa bề mặt sáng hoặc hiện rõ hơn vùng nhìn thẳng. Về bản chất, nó so hướng normal với hướng nhìn camera. Khi normal quay ngang với view direction, rim mạnh. Với shield hoặc hologram, Fresnel giúp shell đọc như lớp năng lượng bao quanh object.",
            "Fresnel mạnh nhất khi đi cùng mask khác: noise breakup, hit ripple, depth fade, opacity control. Nếu chỉ Fresnel đơn, effect có thể sạch quá. Nếu Power quá cao, rim thành viền mỏng; nếu quá thấp, cả object sáng lên.",
        ],
        [
            "NormalWS + ViewDirWS",
            "  -> dot",
            "  -> 1 - saturate",
            "  -> pow(Power)",
            "  -> rim mask",
        ],
        [
            "facing = dot(normalize(NormalWS), normalize(ViewDirWS))",
            "rim = pow(1 - saturate(facing), RimPower)",
            "opacity = rim * NoiseBreakup * ParticleOrInstanceFade",
        ],
        [
            "Dùng normal hoặc view direction sai space.",
            "Fresnel power quá cao khiến effect mất body.",
            "Không nhân với mask/noise nên shield quá sạch.",
        ],
        [
            "Tạo shield sphere bằng Fresnel + noise. Sau đó thêm hit ripple để thấy material trở nên interactive.",
        ],
        [
            "Fresnel là cách nhanh nhất để biến mesh thành shell/rim effect.",
        ],
    )
    lesson(
        doc,
        "22. WorldPosition, hit ripple và WPO",
        [
            "Dùng world data để effect phản ứng với vị trí thật.",
            "Hiểu WPO là deformation theo vertex, không phải pixel magic.",
        ],
        [
            "WorldPosition cho bạn biết pixel đang ở đâu trong thế giới. Từ đó có thể làm height dissolve, ripple từ hit point, mask theo vùng, hoặc procedural texture bám world. Khi gameplay gửi HitPosition vào material, bạn có thể tính distance(WorldPosition, HitPosition) để tạo vòng tròn lan ra.",
            "WPO đẩy vertex, không đẩy pixel. Nó phụ thuộc mật độ geometry và normal. Một sphere nhiều vertex pulse đẹp; một cube hard normal có thể méo theo mảng. WPO lớn cần bounds đúng, nếu không mesh có thể bị culling mất.",
        ],
        [
            "HitPosition từ gameplay",
            "WorldPosition trong material",
            "distance -> ring band -> emissive/opacity/WPO",
            "",
            "VertexNormalWS * Amount -> WPO",
        ],
        [
            "d = distance(WorldPos, HitPos)",
            "ring = smoothstep(Radius - Width, Radius, d) - smoothstep(Radius, Radius + Width, d)",
            "WPO = VertexNormalWS * ring * ImpactAmount",
        ],
        [
            "HitPosition và WorldPosition không cùng space.",
            "WPO làm mesh biến mất vì bounds nhỏ.",
            "Vertex density thấp làm deformation gãy.",
        ],
        [
            "Tạo một shield có hit ripple bằng WorldPosition distance. Sau đó thêm WPO rất nhỏ theo ring.",
        ],
        [
            "Interaction làm material trở thành realtime effect, không còn chỉ là texture đẹp.",
        ],
    )

    chapter(doc, 9, "Recipe production", "Mổ xẻ các effect thường gặp theo mục tiêu, graph logic, debug và tối ưu.")
    worked_recipe(
        doc,
        "23. Additive flare/glow",
        "Tạo một flare đọc rõ core, có falloff đẹp, fade đúng theo particle và không cháy trắng trong gameplay.",
        [
            "Flare là bài học tốt vì nó đơn giản nhưng dạy rất nhiều: alpha power, emissive hierarchy, particle color, additive fade, camera fade. Một flare kém thường là một texture trắng nhân intensity lớn. Một flare tốt có core rõ, mid glow, outer falloff, và biến mất sạch theo lifetime.",
            "Trong Unreal, flare thường dùng Unlit + Additive. Nhưng nếu muốn fade theo Niagara alpha, bạn cần dùng ParticleColor.a để scale emissive. CameraDepthFade cũng nên scale emissive để flare không đập thẳng vào camera.",
        ],
        [
            "UV lane: TexCoord -> sample flare texture",
            "Mask lane: alpha -> pow(alpha, AlphaPower)",
            "Color lane: textureRGB * ParticleColor.rgb * EmissiveIntensity",
            "Fade lane: ParticleColor.a * CameraDepthFade",
            "Output: Emissive = color * mask * fade",
        ],
        [
            "EmissiveIntensity: lực sáng tổng.",
            "AlphaPower: core gắt hay mềm.",
            "CameraFadeLength: fade khi sát camera.",
            "TintColor hoặc ParticleColor: màu theo effect/gameplay.",
        ],
        [
            "Nối alpha ra grayscale để kiểm core/falloff.",
            "Tắt bloom để xem shape thật.",
            "Test trên background sáng, tối và trong level có exposure thật.",
        ],
        [
            "Giảm size card nếu flare gây overdraw lớn.",
            "Dùng texture alpha sạch, không cần nhiều sample.",
            "Nếu spawn hàng loạt, giảm layer glow phụ và để Niagara quản lý lifetime tốt.",
        ],
        [
            "Tạo ba material instance: tiny spark, impact flash, magic bloom. Cùng master, khác AlphaPower/Intensity/Color.",
        ],
    )
    worked_recipe(
        doc,
        "24. Dissolve/reveal/erosion",
        "Tạo dissolve có shape ăn mòn, edge emissive riêng và control Amount dễ animate.",
        [
            "Dissolve production không nên chỉ là opacity fade. Nó cần một field tạo cảm giác vật liệu bị ăn qua bề mặt. Field này có thể là noise texture, world aligned noise, vertex color, height map hoặc texture paint.",
            "Khi Amount chạy, visible mask quyết định phần còn lại. Edge band lấy vùng quanh Amount để tạo ánh sáng, băng, lửa, hoặc magic outline. Nếu edge và visible không tách riêng, bạn khó chỉnh feedback kiểu 'viền mỏng hơn nhưng sáng hơn'.",
        ],
        [
            "Source lane: noise/height/world mask",
            "Visible lane: SmoothStep(Amount, Amount + Softness)",
            "Edge lane: band around Amount",
            "Color lane: base surface + edge emissive",
            "Output: OpacityMask/Opacity + Emissive edge",
        ],
        [
            "Amount: tiến trình dissolve.",
            "Softness: độ mềm của visible transition.",
            "EdgeWidth: độ dày viền.",
            "EdgeIntensity/EdgeColor: lực và màu viền.",
            "NoiseScale: kích thước breakup.",
        ],
        [
            "Debug source mask, visible mask và edge mask riêng.",
            "Animate Amount chậm để xem edge có bám threshold không.",
            "Test trên mesh UV tốt và mesh UV xấu để quyết định UV hay world aligned.",
        ],
        [
            "Bake noise nếu procedural quá nặng.",
            "Dùng Masked cho bề mặt cần cắt rõ, Translucent nếu cần fade mềm.",
            "Pack source masks vào channel nếu có nhiều mask phụ.",
        ],
        [
            "Làm hai bản: dissolve cháy lửa và reveal băng. Giữ công thức Amount/Edge, chỉ đổi source mask và edge color.",
        ],
    )
    worked_recipe(
        doc,
        "25. Fire card procedural",
        "Tạo lửa stylized bằng UV motion, mask dọc, noise breakup và emissive ramp trước khi dùng flipbook.",
        [
            "Fire card procedural giúp bạn hiểu lửa trước khi dựa vào flipbook. Gốc lửa thường dày, nóng và ổn định hơn. Ngọn lửa mỏng, bị xé và biến mất nhanh. Motion chủ yếu đi lên nhưng không đều.",
            "Graph tốt thường có vertical gradient giữ shape, hai noise panner tạo breakup, alpha power để tighten, color ramp tạo core vàng/trắng và edge cam/đỏ. Nếu mọi thứ pan cùng nhau, lửa trông như texture trượt.",
        ],
        [
            "UV lane: two panning noises upward, different scale/speed",
            "Shape lane: vertical gradient bottom strong, top fading",
            "Mask lane: gradient * noise breakup -> alpha",
            "Color lane: core mask -> yellow/white, edge -> orange/red",
            "Output: Emissive + Translucent/Additive opacity",
        ],
        [
            "FlowSpeedLarge, FlowSpeedDetail.",
            "NoiseScaleLarge, NoiseScaleDetail.",
            "AlphaPower, TipFade, EdgeSoftness.",
            "CoreColor, EdgeColor, EmissiveIntensity.",
        ],
        [
            "Nối vertical gradient ra grayscale để chắc gốc/ngọn đúng chiều.",
            "Tắt một noise layer mỗi lần để xem layer nào tạo flow nào.",
            "Test card trên background sáng để tránh lửa chỉ đẹp trên nền đen.",
        ],
        [
            "Một hoặc hai texture noise là đủ cho nhiều lửa stylized.",
            "Nếu cần animation giàu hơn, chuyển sang flipbook thay vì procedural quá nặng.",
            "Giảm card size và alpha vùng trống để giảm overdraw.",
        ],
        [
            "Làm fire card procedural 3 cấp: small torch, magic flame, wall flame. Cùng công thức, khác scale/speed/color.",
        ],
    )
    worked_recipe(
        doc,
        "26. Smoke flipbook",
        "Dùng ParticleSubUV để đọc atlas smoke, giữ timing ở Niagara và shaping ở material.",
        [
            "Smoke flipbook thường nên để texture atlas mang animation chính. Niagara quản lý frame, size, lifetime và color/alpha. Material không nên đánh nhau với Niagara; nó nên đọc ParticleSubUV, tint, shape alpha, depth fade và có optional lighting nhẹ.",
            "Smoke dễ nặng vì overdraw. Một material smoke đơn giản có thể vẫn đắt nếu sprite quá lớn, alpha quá rộng và spawn quá nhiều. Vì vậy debug smoke phải nhìn cả graph và viewport shader complexity.",
        ],
        [
            "Renderer: SubUV enabled, grid đúng",
            "Material: ParticleSubUV RGB/A",
            "Tint/Fade: ParticleColor RGB/A",
            "Alpha shaping: pow(alpha, AlphaPower)",
            "Intersection: DepthFade",
            "Output: Translucent Opacity, low Emissive hoặc lit fallback",
        ],
        [
            "AlphaPower: độ dày smoke.",
            "FadeDistance: soft intersection.",
            "TintColor/ParticleColor: màu khói theo scene.",
            "Softness hoặc OpacityScale: control tổng.",
        ],
        [
            "Dùng NumberGrid để test SubUV trước khi dùng atlas thật.",
            "Nối alpha trước/sau power để xem độ dày.",
            "Test particle count thật, không chỉ một sprite.",
        ],
        [
            "Tighten alpha để giảm vùng trong suốt vẫn vẽ.",
            "Giảm sprite size hoặc spawn count nếu shader complexity đỏ.",
            "Dùng flipbook resolution hợp lý, không vượt nhu cầu màn hình.",
        ],
        [
            "Tạo smoke puff với cùng material nhưng ba Niagara setup: nhanh/tan, chậm/dày, ambient nhẹ.",
        ],
    )
    worked_recipe(
        doc,
        "27. Portal/ring/black hole",
        "Tạo vòng portal có ring band rõ, noise chạy vòng và interior không lấn át edge.",
        [
            "Portal tốt phải đọc là vòng trước khi đọc detail. Nếu graph chỉ tạo full disc noise, nó thành đĩa sáng. Ring band nên lấy từ radial distance hoặc hai threshold: inner và outer. Noise flow có thể chạy theo polar UV để cảm giác xoáy quanh tâm.",
            "Black hole hoặc vortex thường thêm radial distortion và interior swirl. Nhưng ring edge vẫn là anchor visual. Nếu edge mất, người xem không biết cổng nằm ở đâu.",
        ],
        [
            "Radial lane: r = length(UV - 0.5)",
            "Ring lane: outerBand - innerBand",
            "Flow lane: polarUV + panning noise",
            "Color lane: edge emissive strong, interior lower",
            "Depth lane: DepthFade if near geometry",
        ],
        [
            "Radius, Width, Softness.",
            "NoiseScale, FlowSpeed, Spin.",
            "EdgeIntensity, InteriorIntensity.",
            "DistortionStrength, DepthFadeDistance.",
        ],
        [
            "Debug radial distance, ring band, noise flow riêng.",
            "Tắt interior để chắc ring vẫn đọc.",
            "Kiểm seam polar bằng texture tile tốt.",
        ],
        [
            "Dùng một vài noise sample có reuse thay vì nhiều layer độc lập.",
            "Giữ interior opacity thấp để không full-screen overdraw quá nặng.",
            "Nếu portal lớn trên màn hình, cân nhắc mesh/geometry hỗ trợ thay vì sprite card đơn.",
        ],
        [
            "Tạo ba portal: clean sci-fi ring, noisy magic portal, black hole dark center. Giữ chung radial band logic.",
        ],
    )
    worked_recipe(
        doc,
        "28. Forcefield/shield",
        "Tạo shield shell có Fresnel rim, noise breakup và hit ripple tương tác.",
        [
            "Shield là bài tổng hợp hay: Fresnel tạo shell, noise tạo bề mặt năng lượng, hit ripple dùng world position và parameter gameplay, WPO tạo pulse nhỏ. Đây là nơi material trở thành interactive.",
            "Điều khó là giữ shield không thành quả bóng đặc. Opacity nên có rim/fresnel, một ít body pattern, và hit response. Nếu opacity toàn bộ quá cao, shield che gameplay và mất cảm giác năng lượng mỏng.",
        ],
        [
            "Shell lane: Fresnel rim",
            "Pattern lane: noise/hex/scanline mask",
            "Hit lane: distance(WorldPos, HitPos) -> ring",
            "Color lane: base shield color + hit color",
            "WPO lane: VertexNormalWS * pulse/hit",
        ],
        [
            "RimPower, RimIntensity.",
            "NoiseScale, PatternStrength.",
            "HitPosition, HitRadius, HitWidth, HitIntensity.",
            "WPOAmount, PulseSpeed.",
        ],
        [
            "Xuất Fresnel, noise và hit ring riêng.",
            "Nối distance(WorldPos, HitPos) ra grayscale để kiểm space.",
            "Test camera nhìn thẳng và nhìn rìa.",
        ],
        [
            "Giữ opacity thấp để giảm overdraw và giữ gameplay readability.",
            "WPO nhỏ và bounds đủ lớn.",
            "Nếu pattern phức tạp, bake texture hoặc pack channel.",
        ],
        [
            "Dựng shield nhận HitPosition từ parameter. Nếu chưa dùng Blueprint, đặt HitPosition bằng scalar/vector parameter để test tay.",
        ],
    )
    worked_recipe(
        doc,
        "29. Energy beam/ribbon/projectile",
        "Tạo projectile/beam có core, shell, trail và impact hierarchy rõ.",
        [
            "Projectile tốt không phải chỉ là một sprite sáng. Nó thường có core, shell ngoài, trail, spark hoặc impact flare. Mỗi layer phải có vai trò. Core cho hướng và lực. Shell cho style. Trail cho motion. Impact cho feedback.",
            "Beam/ribbon cần edge falloff để không thành thanh chữ nhật. UV dọc beam thường dùng để pan noise theo hướng bay. End fade giúp đầu/cuối mềm hơn. Nếu noise phá quá mạnh, beam mất hướng đọc.",
        ],
        [
            "Core lane: narrow mask, high emissive",
            "Outer lane: wider falloff, lower emissive",
            "Noise lane: panning crawl along beam",
            "End fade: UV gradient at head/tail",
            "Projectile: core + shell + trail + impact flare",
        ],
        [
            "CoreWidth, OuterWidth.",
            "FlowSpeed, NoiseScale.",
            "CoreIntensity, OuterIntensity.",
            "EndFade, DistortionStrength.",
        ],
        [
            "Debug beam mask without color.",
            "View from gameplay distance: does direction read?",
            "Check ribbon twist/UV direction in renderer.",
        ],
        [
            "Use shared master for beam/trail variants.",
            "Reduce layer count for rapid-fire projectiles.",
            "Prefer texture reuse and channel packing.",
        ],
        [
            "Build a projectile family: idle core, moving trail, impact flare. Make them share colors and timing language.",
        ],
    )

    chapter(doc, 10, "Đọc VFXSamplePack như bài đọc hiểu", "Biến material có sẵn thành bài học, không chỉ copy node.")
    sample_cases = [
        (
            "30. M_Flare",
            "M_Flare nên được đọc như bài học về additive sprite: texture alpha tạo shape, Particle Color điều khiển tint/fade, exponent/intensity tạo core và camera fade giúp không che màn hình.",
            [
                "Tìm texture sample và alpha shaping trước.",
                "Kiểm Particle Color RGB/A có đi vào emissive/fade không.",
                "Tìm parameter intensity, exponent/power, camera/depth fade.",
                "Nối alpha ra grayscale để thấy core/falloff.",
                "Tạo ba instance khác nhau để hiểu range parameter.",
            ],
            [
                "Bài tập: rebuild M_Flare tối giản chỉ với Texture, ParticleColor, AlphaPower, Intensity, CameraFade. Sau đó so cảm giác với bản gốc.",
            ],
        ),
        (
            "31. M_Mesh_Add",
            "M_Mesh_Add nên được đọc như bài học mesh additive: mesh cung cấp shape/normal/world position, material dùng noise, gradient hoặc WPO để tạo energy trên bề mặt thay vì sprite phẳng.",
            [
                "Tìm lane mesh data: world position, vertex normal, object position hoặc gradient.",
                "Tìm mask nào giữ vùng chính và mask nào chỉ tạo breakup.",
                "Nếu có WPO, kiểm direction và bounds.",
                "Hỏi vì sao effect dùng mesh additive thay vì sprite.",
                "Test trên mesh khác để thấy UV/normal ảnh hưởng.",
            ],
            [
                "Bài tập: làm một mesh additive pulse trên sphere bằng VertexNormalWS WPO nhỏ + Fresnel/Noise emissive.",
            ],
        ),
        (
            "32. M_FlamesCard",
            "M_FlamesCard là bài học về card flame: UV speed, vertical gradient, edge fade, distortion và emissive ramp phải phối hợp để card không lộ hình chữ nhật.",
            [
                "Tìm vertical gradient hoặc mask theo UV.y.",
                "Tìm panner/noise layer tạo motion lên trên.",
                "Kiểm edge fade ở trái/phải/top để card hòa vào background.",
                "Xem distortion ảnh hưởng detail hay silhouette.",
                "Tách color ramp và alpha mask khi debug.",
            ],
            [
                "Bài tập: rebuild một flame card bằng hai panner và vertical gradient, chưa dùng flipbook.",
            ],
        ),
        (
            "33. M_SubUV_Utilities",
            "M_SubUV_Utilities là bài học kỹ thuật: atlas UV, frame index, NumberGrid và derived 0-1 UV. Nó giúp bạn hiểu SubUV bằng mắt thay vì đoán.",
            [
                "Dùng NumberGrid để test frame order.",
                "Phân biệt atlas UV và local frame UV.",
                "Kiểm SubImageSize trong renderer trước khi sửa material.",
                "Nếu cần effect trong từng frame, dùng derived 0-1 UV.",
                "Không pan atlas UV bừa nếu chưa hiểu local frame.",
            ],
            [
                "Bài tập: tạo fade từ trái qua phải trong từng frame bằng local 0-1 UV, không làm sample sang frame bên cạnh.",
            ],
        ),
    ]
    for title, intro, steps, ex in sample_cases:
        h1(doc, title)
        body(doc, intro)
        h2(doc, "Cách đọc")
        for step in steps:
            bullet(doc, step)
        callout(doc, "Bài tập", ex, fill=LIGHT_GOLD, color=GOLD)

    chapter(doc, 11, "Optimization và production review", "Tối ưu đúng chỗ: overdraw, particle count, texture fetch, shader instruction, bounds và parameter hygiene.")
    lesson(
        doc,
        "34. Tối ưu VFX không bắt đầu từ một node",
        [
            "Biết nhìn tổng cost thay vì chỉ nhìn shader instruction.",
            "Phân biệt cost theo material và cost theo cách effect được spawn.",
        ],
        [
            "Một material rất đơn giản vẫn có thể nặng nếu nó vẽ sprite lớn phủ màn hình và spawn 200 particle. Ngược lại, một graph có vài phép toán thêm có thể không đáng kể nếu effect nhỏ và hiếm. Với VFX, overdraw thường là vấn đề lớn nhất của translucent effects.",
            "Texture fetch, instruction count, refraction, lit translucency, particle count, bounds và sorting đều là một phần của cost. Tối ưu tốt là giữ lại thứ người chơi đọc được, bỏ thứ họ không bao giờ thấy ở khoảng cách gameplay.",
        ],
        [
            "Cost stack:",
            "  screen area / overdraw",
            "  particle count / lifetime",
            "  texture fetch / resolution",
            "  shader instructions",
            "  expensive features: refraction, lit translucency, scene depth",
            "  bounds / sorting / platform",
        ],
        [],
        [
            "Tối ưu math node trong khi card size mới là vấn đề.",
            "Dùng texture 2K cho effect chỉ thấy trong 0.2 giây.",
            "Bật refraction/lit translucency vì đẹp trong preview nhưng quá đắt trong combat.",
        ],
        [
            "Chọn một smoke effect. Giảm 30% card size, giảm spawn count, tighten alpha. So sánh với việc chỉ xóa vài math node.",
        ],
        [
            "Tối ưu là chọn đúng trade-off, không phải làm graph nghèo đi.",
        ],
    )
    lesson(
        doc,
        "35. Parameter hygiene: material dễ chỉnh mới là material production",
        [
            "Expose parameter theo ngôn ngữ visual.",
            "Tránh master material thành hộp đen khó dùng.",
        ],
        [
            "Một material mạnh nhưng khó chỉnh sẽ làm production chậm. Parameter nên nói bằng ngôn ngữ artist/director: Radius, Width, Softness, FlowSpeed, EdgeIntensity, AlphaPower, FadeDistance. Tránh tên kiểu Multiply_03 hoặc NoiseThing nếu người khác không hiểu nó làm gì.",
            "Master material không nên ôm mọi feature. Sprite flare, smoke flipbook, portal ring, shield shell và surface dissolve có thể dùng chung function nhỏ, nhưng không nhất thiết chung một master khổng lồ. Material Instance nên mở ra là hiểu 5-10 control quan trọng nhất.",
        ],
        [
            "Bad parameter:  Scalar_7, MulA, ThingPower",
            "Good parameter: EdgeWidth, AlphaPower, FlowSpeed, CameraFadeLength",
            "",
            "Good master = family rõ + controls ít nhưng đúng",
        ],
        [],
        [
            "Expose mọi số làm instance rối.",
            "Tên parameter theo node math thay vì mục tiêu visual.",
            "Không có default instance đẹp nên người dùng phải đoán range.",
        ],
        [
            "Lấy một material của bạn. Đổi tên parameter để một artist khác nhìn vào hiểu trong 30 giây.",
        ],
        [
            "Technical VFX không chỉ là graph chạy được. Nó là graph người khác dùng được.",
        ],
    )
    h1(doc, "36. Quy trình review 10 phút trước khi gửi")
    review_steps = [
        "Nhìn effect trong level thật, không chỉ preview.",
        "Solo mask chính ra grayscale.",
        "Kiểm Particle Color, Dynamic Parameter hoặc renderer binding.",
        "Kiểm UV motion bằng checker/NumberGrid nếu motion có vấn đề.",
        "Kiểm fade: lifetime, camera, depth, end fade.",
        "Tắt bloom để xem emissive shape thật.",
        "Kiểm scale ở gameplay distance.",
        "Xem shader complexity/overdraw nếu effect spawn nhiều.",
        "Kiểm parameter names và material instance defaults.",
        "Viết một câu đọc graph: source data -> UV/mask -> color/opacity -> interaction -> cost.",
    ]
    for i, step in enumerate(review_steps, 1):
        numbered(doc, i, step)
    callout(doc, "Tiêu chuẩn", [
        "Nếu không thể giải thích graph bằng một câu, graph cần tổ chức lại.",
        "Nếu mask grayscale không đọc, bản màu chưa nên được duyệt.",
        "Nếu người khác không hiểu parameter trong 30 giây, instance chưa production-friendly.",
    ], fill=LIGHT_GOLD, color=GOLD)

    chapter(doc, 12, "Texture authoring và atlas", "Hiểu texture như dữ liệu kỹ thuật, không chỉ là ảnh đẹp.")
    lesson(
        doc,
        "37. Texture VFX nên được vẽ để phục vụ shader",
        [
            "Biết khác nhau giữa texture đẹp để nhìn và texture tốt để material xử lý.",
            "Thiết kế texture theo channel, alpha, tiling, compression và use case.",
        ],
        [
            "Một texture VFX production không nhất thiết phải đẹp khi mở riêng trong Photoshop. Nó phải chứa đúng dữ liệu để shader dùng. Một radial flare alpha tốt có thể rất đơn giản nhưng cho falloff sạch. Một noise dissolve tốt có contrast và scale hợp lý. Một flipbook tốt có frame padding, alpha ổn, motion readable.",
            "Khi author texture, hãy nghĩ ngược từ material. Nếu material cần edge band, source mask phải có gradient đủ mềm quanh threshold. Nếu material cần packed channels, mỗi kênh phải có ý nghĩa riêng và không phá nhau do compression. Nếu material dùng Additive, texture RGB có thể là energy pattern; nếu material dùng Translucent smoke, alpha quan trọng hơn RGB rất nhiều.",
            "AE mạnh ở bước này. Bạn có thể dùng AE để sinh noise loop, flipbook, radial flare, distortion map, flow-looking texture. Nhưng khi đưa vào engine, hãy xem nó là data. Tắt color đẹp một lúc, nhìn alpha/mask bằng grayscale, hỏi: shader sẽ đọc được gì từ texture này?",
        ],
        [
            "Visual idea",
            "  -> texture data cần cho shader",
            "  -> import settings",
            "  -> material sampling",
            "  -> particle/runtime control",
        ],
        [
            "Packed.r = shape mask",
            "Packed.g = breakup noise",
            "Packed.b = edge/detail mask",
            "Packed.a = alpha/height",
        ],
        [
            "Vẽ texture quá đẹp nhưng alpha không usable.",
            "Để sRGB bật cho mask data khiến giá trị grayscale bị sai response.",
            "Không có padding trong atlas, gây bleeding giữa frame/sprite.",
        ],
        [
            "Tạo một texture pack 4 kênh: radial shape, noise breakup, edge detail, soft alpha. Trong material, nối từng kênh ra Emissive để kiểm.",
        ],
        [
            "Texture VFX là tài sản dữ liệu. Đẹp nhất là khi shader đọc nó dễ và rẻ.",
        ],
    )
    lesson(
        doc,
        "38. Flipbook: khi nào bake, khi nào procedural",
        [
            "Chọn đúng giữa flipbook, procedural panner và hybrid.",
            "Hiểu trade-off giữa đẹp, memory, control và performance.",
        ],
        [
            "Flipbook rất mạnh khi motion phức tạp: smoke billow, explosion, fire simulation, magical bloom có frame art-directed. Nó cho visual richness mà procedural material khó tạo rẻ. Đổi lại, flipbook tốn memory, resolution, atlas management và đôi khi khó recolor hoặc retime nếu bake quá cứng.",
            "Procedural material mạnh khi motion có thể diễn đạt bằng UV/noise/math: energy crawl, stylized fire đơn giản, portal ring, shield shimmer, beam trail. Nó nhẹ về memory và dễ expose parameter, nhưng nếu cố procedural hóa mọi chi tiết tự nhiên, graph sẽ dài và vẫn không bằng flipbook đẹp.",
            "Hybrid thường là câu trả lời production: bake motion chính thành flipbook, dùng material để tint, alpha power, depth fade, distortion nhẹ, edge emissive hoặc interaction. Đó là cách giữ visual richness mà vẫn realtime-flexible.",
        ],
        [
            "Flipbook:   đẹp, motion phức tạp, tốn memory, ít procedural control",
            "Procedural: rẻ memory, dễ chỉnh, có thể nhìn giả nếu quá đều",
            "Hybrid:     bake phần khó, runtime phần cần interaction/control",
        ],
        [
            "flipbookRGB/A = ParticleSubUV",
            "alpha = pow(flipbookA, AlphaPower) * ParticleColor.a",
            "emissive = flipbookRGB * ParticleColor.rgb * Intensity",
            "opacity = DepthFade(alpha, FadeDistance)",
        ],
        [
            "Dùng flipbook cho effect chỉ cần một radial mask chạy panner.",
            "Dùng procedural phức tạp để bắt chước smoke sim tự nhiên.",
            "Bake màu quá nặng vào flipbook khiến khó tint theo gameplay.",
        ],
        [
            "Chọn một fire effect. Viết ra ba bản thiết kế: full flipbook, full procedural, hybrid. Ghi ưu/nhược từng bản.",
        ],
        [
            "Bake phần khó quan sát; giữ runtime control cho phần cần phản ứng.",
        ],
    )
    lesson(
        doc,
        "39. Import settings và compression là một phần của shader",
        [
            "Biết vì sao cùng một graph nhưng texture import khác có thể làm visual sai.",
            "Kiểm sRGB, compression, alpha, mip và resolution cho texture VFX.",
        ],
        [
            "Nhiều người học material chỉ nhìn graph, nhưng texture import có thể quyết định toàn bộ response. Mask grayscale thường không nên dùng sRGB vì bạn muốn giá trị data tuyến tính. Normal/flow map cần settings riêng. Alpha bị compression xấu có thể làm edge band bị bẩn. Mipmap có thể làm texture nhỏ xa camera bị mềm hoặc shimmer tùy trường hợp.",
            "Với flipbook, resolution không chỉ là độ nét. Nó ảnh hưởng memory và sampling. Một atlas lớn nhưng sprite trong gameplay nhỏ là lãng phí. Một atlas thiếu padding có thể bleed frame. Một mask có banding sẽ làm dissolve có bậc.",
            "Khi debug material, hãy có checklist texture import. Nếu material đúng công thức nhưng mask response lạ, kiểm sRGB/compression trước khi thêm node sửa sai.",
        ],
        [
            "Mask texture: thường data/linear, compression phù hợp mask",
            "Color texture: sRGB nếu là màu nhìn thấy",
            "Normal/flow: import theo loại dữ liệu vector",
            "Flipbook: padding, mip, resolution, alpha quality",
        ],
        [],
        [
            "Dùng sRGB cho mask dissolve rồi thắc mắc threshold không đều.",
            "Compression làm alpha edge có artifacts.",
            "Không test mip/LOD nên effect xa camera bị nhòe hoặc rung.",
        ],
        [
            "Tạo một mask dissolve, thử bật/tắt sRGB hoặc đổi compression phù hợp. Quan sát threshold thay đổi ra sao.",
        ],
        [
            "Texture settings không phải việc phụ. Nó là một phần của material pipeline.",
        ],
    )

    chapter(doc, 13, "Unreal material properties thật sự quan trọng", "Đọc phần Details của material như đọc một hợp đồng render.")
    lesson(
        doc,
        "40. Blend Mode, Shading Model, Two Sided",
        [
            "Chọn material properties có lý do, không để default theo thói quen.",
            "Hiểu tác động tới visual, sorting, lighting và cost.",
        ],
        [
            "Material graph chỉ là một nửa câu chuyện. Blend Mode quyết định pixel hòa với scene ra sao. Shading Model quyết định material có nhận lighting không. Two Sided quyết định mặt sau của card/mesh có được vẽ không. Với sprite VFX, Unlit + Additive hoặc Unlit + Translucent rất phổ biến. Với smoke lit, có thể cần Default Lit hoặc model phù hợp, nhưng cost và sorting phải được cân nhắc.",
            "Two Sided trên flame/smoke card tránh việc card biến mất khi camera nhìn mặt sau. Nhưng với mesh phức tạp, Two Sided có thể tăng cost hoặc làm lighting không như ý. Masked surface dissolve khác Translucent dissolve ở cách depth/sorting và edge đọc. Mỗi property là một quyết định production, không chỉ là checkbox.",
        ],
        [
            "Additive + Unlit: flare, spark, energy",
            "Translucent + Unlit: smoke/fire mềm, portal, mist",
            "Masked: surface dissolve/cutout rõ",
            "Default Lit: smoke/surface cần nhận lighting",
            "Two Sided: sprite/card cần nhìn từ hai mặt",
        ],
        [],
        [
            "Dùng Translucent cho mọi thứ rồi bị overdraw/sorting.",
            "Dùng Additive cho smoke khiến smoke mất trên nền sáng.",
            "Quên Two Sided cho card flame/smoke.",
        ],
        [
            "Lấy cùng một material card và tạo ba bản Additive, Translucent, Masked. Ghi khác biệt về visual và lỗi.",
        ],
        [
            "Material properties là luật render. Graph đẹp nhưng property sai vẫn ra effect sai.",
        ],
    )
    lesson(
        doc,
        "41. Refraction, scene color và các feature đắt",
        [
            "Biết khi nào distortion/refraction đáng dùng.",
            "Không bật feature đắt chỉ vì preview đẹp.",
        ],
        [
            "Heat haze, magic distortion, glass, portal warp thường cần refraction hoặc scene color. Chúng rất hấp dẫn vì làm effect hòa vào thế giới. Nhưng chúng phụ thuộc render pipeline, depth, translucency order và platform. Một distortion full-screen hoặc nhiều layer có thể đắt hơn bạn nghĩ.",
            "Nguyên tắc: dùng distortion khi nó phục vụ visual chính hoặc gameplay feedback, không phải để che mask yếu. Nếu effect chỉ cần energy shimmer nhỏ, có thể distort một layer nhỏ hoặc dùng mesh/ribbon thay vì sprite lớn. Nếu cần realistic heat haze, tách distortion pass riêng và kiểm trong level thật.",
        ],
        [
            "Distortion pass",
            "  -> đọc scene/background",
            "  -> offset theo normal/noise/flow",
            "  -> hòa với effect chính",
            "",
            "Dùng nhỏ, đúng chỗ, test platform",
        ],
        [
            "offset = (NormalOrNoise.rg * 2 - 1) * DistortionStrength",
            "distortedScene = SceneColor(ScreenUV + offset)",
        ],
        [
            "Distortion quá mạnh làm background bơi.",
            "Dùng refraction trên nhiều particle full-screen.",
            "Không test sorting/translucency trong scene thật.",
        ],
        [
            "Tạo một heat haze nhỏ phía sau fire card. Sau đó tăng sprite size để thấy cost/visual thay đổi.",
        ],
        [
            "Feature đắt phải có lý do visual rõ. Đừng để nó trở thành lớp trang điểm cho graph yếu.",
        ],
    )
    lesson(
        doc,
        "42. Material Function và master material",
        [
            "Tách logic để graph dễ đọc nhưng không giấu mọi thứ trong hộp đen.",
            "Thiết kế master material theo family thay vì ôm tất cả.",
        ],
        [
            "Material Function giúp tái sử dụng logic: remap, radial ring, flipbook helper, flow distortion, fresnel shell. Nhưng function không tự làm shader rẻ hơn. Nó chỉ tổ chức graph. Nếu function giấu quá nhiều và input/output mơ hồ, artist sẽ sợ dùng.",
            "Master material nên phục vụ một family: sprite flare, smoke flipbook, portal ring, shield shell, surface dissolve. Một master ôm mọi feature sẽ có nhiều switch, nhiều parameter, nhiều variant và khó debug. Production-friendly master có default đẹp, parameter ít nhưng đúng, và comment lane rõ.",
        ],
        [
            "Good function: BuildRingBand(UV, Radius, Width, Softness)",
            "Bad function: MagicEverything(A, B, C)",
            "",
            "Good master: một family rõ",
            "Bad master: mọi effect trong một graph khổng lồ",
        ],
        [],
        [
            "Tách function cho node quá nhỏ làm graph khó đọc hơn.",
            "Function không có tên input rõ.",
            "Master có 40 parameter nhưng artist chỉ cần 8.",
        ],
        [
            "Chọn một recipe portal. Tách radial ring band thành function với input/output rõ. Sau đó dùng lại cho shockwave.",
        ],
        [
            "Abstraction tốt giúp production nhanh hơn. Abstraction xấu chỉ giấu sự rối.",
        ],
    )

    chapter(doc, 14, "Từ điển node chuyên sâu theo cách dùng", "Học node bằng vai trò production: input, output, visual purpose và lỗi.")
    node_notes = [
        ("43. Particle Color", "Dữ liệu màu/alpha từ Niagara. RGB thường tint màu, A thường fade opacity hoặc emissive. Nếu Color over Life không hoạt động, kiểm node này đầu tiên."),
        ("44. Dynamic Parameter", "Bốn kênh float từ Niagara. Dùng cho AlphaPower, FadeDistance, DistortionStrength, DissolveAmount. Cần naming ổn định trong cả material family."),
        ("45. TextureCoordinate", "UV gốc của surface/sprite. Dùng để scale tiling, chọn UV channel, debug unwrap. Sai UV channel làm graph đúng nhưng texture sai."),
        ("46. Panner", "UV + Time * Speed. Rẻ và phổ biến cho fire, smoke, water, energy. Một panner đơn dễ lộ, hai layer khác scale/speed thường tốt hơn."),
        ("47. Rotator", "Xoay UV quanh pivot. Dùng cho flare, magic circle, portal. Pivot sai tạo motion lệch tâm."),
        ("48. ParticleSubUV", "Đọc atlas frame từ renderer SubUV. Material không tự chọn frame; Niagara/renderer làm việc đó. Dùng NumberGrid để debug."),
        ("49. ComponentMask", "Tách R/G/B/A từ vector hoặc packed texture. Bắt buộc khi texture là data container."),
        ("50. AppendVector", "Ghép scalar thành vector, thường dùng tạo UV float2 hoặc custom vector data. Sai thứ tự X/Y làm hướng UV đảo."),
        ("51. DepthFade", "Làm mềm giao tuyến translucent với geometry. Hữu ích cho smoke/fire/mist nhưng không thay thế alpha mask tốt."),
        ("52. SceneDepth / PixelDepth", "Dùng cho depth interaction nâng cao. Phải hiểu space/linearization và ràng buộc blend mode."),
        ("53. WorldPosition", "Vị trí pixel trong world. Dùng cho height dissolve, hit ripple, world-space mask, procedural field."),
        ("54. ObjectPosition / ObjectRadius", "Dữ liệu object-level. Hữu ích cho shell mask ổn định theo object scale, nhưng phụ thuộc bounds/pivot."),
        ("55. VertexNormalWS", "Normal vertex ở world space. Dùng WPO inflate/pulse. Vertex density và hard edge ảnh hưởng deformation."),
        ("56. Transform", "Đổi vector/position giữa spaces. Lỗi rất phổ biến là dot hai vector khác space."),
        ("57. Fresnel", "Rim theo góc nhìn. Dùng shield, hologram, rim glow. Power quyết định rim mỏng hay rộng."),
        ("58. SmoothStep", "Tạo threshold mềm. Cực kỳ quan trọng cho dissolve, edge fade, soft mask."),
        ("59. Power", "Bẻ cong alpha/falloff. Dùng cho core shaping, alpha tighten, fresnel response."),
        ("60. Custom HLSL", "Dùng khi graph node quá dài cho một công thức rõ. Không dùng để giấu logic đơn giản chưa debug."),
    ]
    for title, text in node_notes:
        h1(doc, title)
        body(doc, text)
        callout(doc, "Cách tự học node này", [
            "Nối output node hoặc lane liên quan ra grayscale/Emissive.",
            "Tạo ba giá trị parameter cực đoan: thấp, trung bình, cao.",
            "Viết một câu: node này biến input gì thành output gì để phục vụ visual nào?",
        ], fill=LIGHT_TEAL)

    chapter(doc, 15, "Troubleshooting theo triệu chứng", "Gọi đúng bệnh trước khi sửa graph.")
    troubleshooting = [
        ("61. Effect nhìn như ảnh dán", "Thường do silhouette vuông, thiếu depth/intersection, motion quá phẳng hoặc mask không có hierarchy. Solo alpha mask trước, sau đó thêm edge fade hoặc depth fade, cuối cùng mới thêm detail noise."),
        ("62. Effect cháy trắng", "Emissive intensity quá cao, additive layer chồng nhiều, alpha không nhân vào emissive hoặc exposure khác preview. Tắt bloom để kiểm shape thật."),
        ("63. Smoke/fire cắt cứng vào scene", "Thiếu DepthFade, FadeDistance thấp, alpha quá cứng hoặc card đặt xuyên geometry quá sâu. Debug opacity trước và sau DepthFade."),
        ("64. Dissolve không có edge đẹp", "Source mask quá xám, edge band tính sai, EdgeWidth/Softness không cân bằng. Tách visible và edge ra hai grayscale riêng."),
        ("65. Flipbook sai frame", "SubImageSize, frame order hoặc renderer SubUV sai. Dùng NumberGrid trước khi dùng atlas thật."),
        ("66. Portal thành đĩa sáng", "Chưa tách ring band inner/outer, interior quá sáng, opacity dùng full radial fill. Tắt interior, giữ edge ring đọc rõ trước."),
        ("67. Shield thành quả bóng đặc", "Opacity body quá cao, Fresnel power thấp, noise không đủ breakup. Giảm body opacity và tăng rim hierarchy."),
        ("68. Hit ripple sai vị trí", "HitPosition và WorldPosition không cùng space, parameter không bind đúng hoặc pivot/bounds lệch. Debug distance field ra grayscale."),
        ("69. WPO làm mesh biến mất", "Bounds nhỏ, WPO amplitude quá lớn, object scale ảnh hưởng. Giảm WPO và kiểm bounds/culling."),
        ("70. Material đẹp trong editor nhưng xấu trong level", "Exposure, background, scale, lighting, depth, overdraw hoặc Niagara setup khác preview. Luôn test trong level thật."),
    ]
    for title, text in troubleshooting:
        h1(doc, title)
        body(doc, text)
        callout(doc, "Cách debug nhanh", [
            "Tắt màu/bloom nếu nghi ngờ mask.",
            "Nối lane liên quan ra Emissive.",
            "Kiểm renderer/Niagara binding trước khi thêm node sửa sai.",
        ], fill=LIGHT_GOLD, color=GOLD)

    chapter(doc, 16, "Lộ trình luyện tập", "Biến kiến thức thành phản xạ bằng bài tập nhỏ, đều, có kiểm chứng.")
    h1(doc, "37. Lộ trình 6 tuần")
    weeks = [
        ("Tuần 1 - Mask grayscale", "Radial, linear, noise threshold, SmoothStep, Power, edge band. Chưa dùng màu cho tới khi mask đọc rõ."),
        ("Tuần 2 - UV motion", "Panner, two-layer noise, rotator, polar UV, distortion. Dùng checker/NumberGrid để debug hướng."),
        ("Tuần 3 - Particle contract", "Particle Color, Dynamic Parameter, SubUV, CameraDepthFade, DepthFade. Test bằng Niagara thật."),
        ("Tuần 4 - Interaction", "Fresnel, WorldPosition, hit ripple, WPO, shield shell. Tập kiểm space và bounds."),
        ("Tuần 5 - Recipe library", "Flare, dissolve, fire, smoke, portal, shield, beam, projectile. Mỗi recipe có master + 2 instance."),
        ("Tuần 6 - Rebuild và tối ưu", "Rebuild một material VFXSamplePack từ trí nhớ. Sau đó so với bản gốc và tối ưu trong level thật."),
    ]
    for title, desc in weeks:
        h2(doc, title)
        body(doc, desc)
    h1(doc, "38. Nhật ký học 30 ngày")
    days = [
        "Ngày 1: radial mask mềm/cứng.",
        "Ngày 2: ring band bằng hai SmoothStep.",
        "Ngày 3: Power cho alpha gắt/mềm.",
        "Ngày 4: OneMinus đảo logic reveal/destroy.",
        "Ngày 5: noise threshold dissolve.",
        "Ngày 6: edge emissive cho dissolve.",
        "Ngày 7: rebuild dissolve từ trí nhớ.",
        "Ngày 8: panner một lớp.",
        "Ngày 9: hai panner ngược chiều.",
        "Ngày 10: rotator đúng pivot.",
        "Ngày 11: polar UV cơ bản.",
        "Ngày 12: portal noise chạy vòng.",
        "Ngày 13: Particle Color fade cho additive.",
        "Ngày 14: Dynamic Parameter điều khiển AlphaPower.",
        "Ngày 15: CameraDepthFade trên flare lớn.",
        "Ngày 16: DepthFade với smoke card cắt qua cube.",
        "Ngày 17: SubUV bằng NumberGrid.",
        "Ngày 18: smoke flipbook có alpha power.",
        "Ngày 19: fire card procedural.",
        "Ngày 20: forcefield Fresnel shell.",
        "Ngày 21: hit ripple bằng WorldPosition distance.",
        "Ngày 22: WPO pulse theo VertexNormalWS.",
        "Ngày 23: height dissolve bằng WorldPosition.z.",
        "Ngày 24: object-radius shell mask.",
        "Ngày 25: pack mask vào RGB/A.",
        "Ngày 26: giảm texture sample.",
        "Ngày 27: giảm overdraw bằng alpha/card size.",
        "Ngày 28: đọc M_Flare theo lane.",
        "Ngày 29: đọc M_FlamesCard hoặc M_Mesh_Add theo lane.",
        "Ngày 30: viết lại một effect cũ bằng material sạch hơn.",
    ]
    for day in days:
        bullet(doc, day)

    chapter(doc, 17, "Workshop thực hành có hướng dẫn", "Biến từng khái niệm thành bài dựng cụ thể trong Unreal.")
    workshops = [
        (
            "39. Workshop - Mask Gym",
            "Mục tiêu của bài này là luyện mắt với grayscale. Bạn sẽ không dùng màu, không dùng bloom, không dùng texture đẹp. Chỉ dùng mask và công thức. Nếu làm nghiêm túc, đây là bài giúp bạn lên trình nhanh nhất vì mọi shader VFX đều quay về mask.",
            [
                "Tạo material Unlit xuất thẳng mask vào Emissive.",
                "Dựng radial circle, ring band, linear gradient, noise threshold.",
                "Mỗi mask có parameter Radius/Width/Softness hoặc Amount/Power.",
                "Tạo 6 material instance, mỗi instance chỉ đổi parameter.",
                "Chụp viewport grayscale của từng instance để so sánh.",
            ],
            [
                "Bạn phải giải thích được mask nào tạo shape lớn, mask nào tạo edge, mask nào tạo breakup.",
                "Khi tăng Softness, edge mềm ra nhưng không mất toàn bộ silhouette.",
                "Khi tăng Power, core hoặc vùng sáng thay đổi theo cách bạn dự đoán trước.",
            ],
            [
                "Thêm một switch đảo logic reveal/destroy bằng OneMinus.",
                "Thêm một texture NumberGrid để thấy mask đang phủ không gian UV nào.",
            ],
        ),
        (
            "40. Workshop - UV Motion Gym",
            "Bài này tập cho bạn nhìn chuyển động như data UV. Đừng bắt đầu bằng noise đẹp. Bắt đầu bằng checker hoặc NumberGrid để thấy texture đang bị kéo đi đâu, xoay quanh đâu, và lặp ra sao.",
            [
                "Tạo material sample NumberGrid bằng UV gốc.",
                "Thêm Tiling và Offset parameter.",
                "Thêm Panner theo X, Y, rồi hai Panner ngược chiều.",
                "Thêm Rotator với Pivot parameter.",
                "Cuối cùng đổi NumberGrid sang noise để xem motion thật.",
            ],
            [
                "Bạn biết speed dương/âm làm texture chạy hướng nào.",
                "Bạn thấy rõ pivot sai khi Rotator xoay lệch tâm.",
                "Bạn biết layer lớn/chậm và layer nhỏ/nhanh tạo cảm giác khác nhau.",
            ],
            [
                "Chuyển UV sang polar và làm noise chạy vòng.",
                "Dùng Dynamic Parameter để Niagara điều khiển FlowSpeed.",
            ],
        ),
        (
            "41. Workshop - Dissolve Master Nhỏ",
            "Bạn sẽ dựng một master dissolve có thể dùng cho reveal hoặc destroy. Bài này không cần graph khổng lồ; mục tiêu là parameter sạch và edge band đúng.",
            [
                "Nguồn mask: noise texture hoặc world aligned noise.",
                "Visible = SmoothStep(Amount, Amount + Softness, mask).",
                "Edge = band quanh Amount bằng hai SmoothStep trừ nhau.",
                "Output opacity/masked tùy loại material.",
                "Expose Amount, Softness, EdgeWidth, EdgeIntensity, EdgeColor.",
            ],
            [
                "Amount chạy từ 0 tới 1 thì dissolve diễn ra trơn.",
                "EdgeWidth chỉnh được độ dày viền mà không phá Softness.",
                "EdgeIntensity chỉnh được độ sáng viền mà không đổi opacity chính.",
            ],
            [
                "Làm một bản cháy lửa và một bản đóng băng chỉ bằng đổi EdgeColor, noise scale và material instance.",
                "Thêm WorldPosition.z để dissolve từ dưới lên.",
            ],
        ),
        (
            "42. Workshop - Fire Card Không Flipbook",
            "Bài này bắt bạn hiểu cấu trúc lửa trước khi dựa vào atlas. Lửa tốt có gốc dày, ngọn mỏng, motion đi lên nhưng không đều, core nóng và edge tối hơn.",
            [
                "Dùng UV.y tạo vertical gradient: gốc mạnh, ngọn fade.",
                "Thêm hai noise panner đi lên với scale/speed khác nhau.",
                "Multiply gradient với noise để tạo alpha.",
                "Dùng Power để tighten alpha.",
                "Tạo color ramp bằng Lerp: core vàng/trắng, edge cam/đỏ.",
            ],
            [
                "Card không lộ hình chữ nhật khi đặt trên nền tối và sáng.",
                "Ngọn lửa bị xé nhẹ, nhưng gốc vẫn có thân.",
                "Tắt một noise layer vẫn hiểu layer đó đang đóng vai trò gì.",
            ],
            [
                "Thêm distortion chỉ vào detail noise, không phá silhouette chính.",
                "Tạo ba instance: torch, magic flame, wall flame.",
            ],
        ),
        (
            "43. Workshop - Smoke Flipbook Contract",
            "Bài này luyện hợp đồng giữa Niagara renderer và material. Bạn sẽ dùng NumberGrid trước để không đoán frame, sau đó mới chuyển sang smoke atlas.",
            [
                "Tạo Niagara sprite renderer bật SubUV với grid đúng.",
                "Material dùng ParticleSubUV RGB/A.",
                "Nhân RGB/A với ParticleColor.",
                "Alpha đi qua Power và DepthFade.",
                "Test frame playback bằng NumberGrid, sau đó thay bằng smoke atlas.",
            ],
            [
                "NumberGrid chạy đúng thứ tự frame.",
                "Niagara alpha curve thật sự fade smoke.",
                "DepthFade làm giao tuyến với cube/mặt đất mềm hơn.",
            ],
            [
                "Thêm DynamicParameter.A điều khiển AlphaPower.",
                "Tạo material instance smoke mỏng, smoke dày, smoke ambient.",
            ],
        ),
        (
            "44. Workshop - Portal Ring",
            "Portal ring là bài tổng hợp radial math, mask band, noise flow và emissive hierarchy. Mục tiêu là ring đọc rõ trước, interior chỉ là detail phụ.",
            [
                "Tạo radial distance từ UV - 0.5.",
                "Tạo ring band bằng Radius, Width, Softness.",
                "Tạo polar UV để noise chạy quanh vòng.",
                "Edge emissive mạnh hơn interior.",
                "Thêm DepthFade nếu portal gần geometry.",
            ],
            [
                "Tắt interior vẫn thấy portal là một vòng rõ.",
                "Noise motion chạy quanh tâm, không trượt ngang như decal.",
                "Radius/Width chỉnh được hình mà không phá noise.",
            ],
            [
                "Thêm black hole center bằng radial gradient ngược.",
                "Thêm distortion nhỏ ở interior nhưng giữ ring edge ổn định.",
            ],
        ),
        (
            "45. Workshop - Forcefield Có Hit Ripple",
            "Bài này đưa material vào vùng interactive. Bạn sẽ dùng Fresnel cho shell, noise cho pattern, WorldPosition distance cho hit ripple và WPO rất nhỏ để có phản hồi va chạm.",
            [
                "Tạo mesh sphere material Translucent/Unlit hoặc Additive tùy style.",
                "Base opacity = Fresnel * OpacityScale.",
                "Pattern = noise hoặc hex mask nhân nhẹ vào opacity/emissive.",
                "Hit ring = distance(WorldPos, HitPos) quanh Radius.",
                "WPO = VertexNormalWS * HitRing * WPOAmount nhỏ.",
            ],
            [
                "Shield không thành quả bóng đặc.",
                "Hit ring xuất hiện đúng vị trí khi đổi HitPos parameter.",
                "WPO không làm mesh biến mất hoặc méo quá mạnh.",
            ],
            [
                "Kết nối HitPos từ Blueprint hoặc Niagara.",
                "Thêm decay bằng Time/HitTime để ripple tắt dần.",
            ],
        ),
        (
            "46. Workshop - Beam/Ribbon Có Core Và Shell",
            "Beam thường hỏng vì nhìn như thanh chữ nhật. Bài này tập edge falloff, core/shell hierarchy và noise crawl theo chiều beam.",
            [
                "Dùng UV.y hoặc UV.x tùy ribbon setup để xác định chiều beam.",
                "Core mask mảnh, shell mask rộng hơn.",
                "Noise panner chạy dọc beam để tạo energy crawl.",
                "End fade ở đầu/cuối bằng gradient.",
                "Output Additive emissive với ParticleColor fade.",
            ],
            [
                "Beam có core rõ và outer glow mềm.",
                "Noise không phá mất hướng đọc của beam.",
                "Đầu/cuối không cắt thành hình chữ nhật.",
            ],
            [
                "Tạo projectile trail bằng cùng material.",
                "Thêm distortion nhẹ chỉ ở shell.",
            ],
        ),
        (
            "47. Workshop - Projectile Family",
            "Một projectile production là family nhỏ, không phải một material đơn. Bạn sẽ ghép core, shell, trail và impact flare thành cùng ngôn ngữ visual.",
            [
                "Core: additive sprite/mesh sáng, shape gọn.",
                "Shell: fresnel/radial outer layer.",
                "Trail: beam/ribbon có end fade.",
                "Impact: flare + shockwave ring.",
                "Dùng cùng color palette và timing logic.",
            ],
            [
                "Projectile đọc được hướng bay ở gameplay distance.",
                "Impact có punch nhưng không che target quá lâu.",
                "Các layer có hierarchy, không cùng sáng như nhau.",
            ],
            [
                "Tạo ba tier: weak, medium, ultimate projectile bằng cùng family material.",
                "Tối ưu rapid-fire bằng cách giảm layer không cần thiết.",
            ],
        ),
        (
            "48. Workshop - Đọc Và Rebuild M_Flare",
            "Mục tiêu không phải copy M_Flare. Mục tiêu là nhìn graph có sẵn, rút ra grammar, rồi dựng lại bản nhỏ từ trí nhớ.",
            [
                "Mở M_Flare và viết lane diagram bằng chữ.",
                "Tìm texture alpha, Particle Color, intensity, power, fade.",
                "Tạo material mới chỉ có 5 control chính.",
                "So visual với bản gốc trong Niagara test.",
                "Ghi phần nào bản gốc xử lý tốt hơn bản rebuild.",
            ],
            [
                "Bạn giải thích được từng parameter bằng ngôn ngữ visual.",
                "Bản rebuild có fade đúng với Niagara alpha.",
                "Bạn biết thiếu feature nào nếu visual chưa giống.",
            ],
            [
                "Tạo một phiên bản stylized và một phiên bản sci-fi từ cùng rebuild master.",
            ],
        ),
        (
            "49. Workshop - Đọc Và Rebuild M_FlamesCard",
            "Bài này tập đọc một material có motion và mask rõ hơn. Bạn sẽ phân biệt UV motion, mask silhouette, edge fade, color/emissive và distortion.",
            [
                "Chụp hoặc ghi lại các node group chính.",
                "Tách UV lane: panner/tiling/speed.",
                "Tách mask lane: alpha, vertical fade, edge fade.",
                "Tách color lane: core/edge/intensity.",
                "Dựng lại bản tối giản và so với bản gốc.",
            ],
            [
                "Bạn biết layer nào làm ngọn lửa sống, layer nào giữ silhouette.",
                "Bản rebuild không còn là card vuông.",
                "Parameter speed/scale/color dễ chỉnh hơn bản đầu tiên của bạn.",
            ],
            [
                "Đổi style thành blue magic flame mà không sửa graph, chỉ sửa instance.",
            ],
        ),
        (
            "50. Workshop - Optimization Pass",
            "Bài cuối là bài production thật: lấy effect đẹp rồi làm nó chạy hợp lý hơn. Bạn không được xóa bừa. Phải đoán nguyên nhân, kiểm debug view, rồi tối ưu đúng chỗ.",
            [
                "Chọn một effect có nhiều translucent layer.",
                "Xem shader complexity/overdraw trong level test.",
                "Giảm card size hoặc tighten alpha trước.",
                "Giảm texture sample/layer nếu graph thật sự nặng.",
                "Giảm spawn count/lifetime nếu particle system là nguyên nhân.",
            ],
            [
                "Visual giữ được 80-90% cảm giác chính.",
                "Overdraw hoặc cost giảm thấy được trong debug view.",
                "Bạn viết được trade-off: bỏ gì, giữ gì, vì sao.",
            ],
            [
                "Tạo bản before/after và viết review một đoạn như gửi lead: vấn đề, thay đổi, kết quả, rủi ro còn lại.",
            ],
        ),
    ]
    for title, intro, setup, success, extensions in workshops:
        h1(doc, title)
        body(doc, intro)
        h2(doc, "Setup")
        for item in setup:
            bullet(doc, item)
        callout(doc, "Tiêu chuẩn đạt", success, fill=LIGHT_TEAL)
        callout(doc, "Biến thể nâng cấp", extensions, fill=LIGHT_PURPLE, color=PURPLE)

    chapter(doc, 18, "Dịch feedback thành hành động shader", "Biến câu nói mơ hồ của lead/client thành layer cần chỉnh.")
    feedback_cards = [
        (
            "51. 'Làm mạnh hơn'",
            "Đây là feedback rất phổ biến nhưng không chỉ có nghĩa tăng EmissiveIntensity. Mạnh hơn có thể là silhouette lớn hơn, core sáng hơn, timing nhanh hơn, contrast cao hơn, hoặc effect xuất hiện đúng thời điểm hơn. Nếu bạn chỉ kéo intensity, effect có thể cháy trắng mà vẫn không mạnh.",
            [
                "Hỏi layer: shape, color, timing hay camera framing?",
                "Thử tăng core intensity trước, không tăng toàn bộ glow.",
                "Tăng contrast mask hoặc giảm outer falloff nếu effect bị mờ.",
                "Tăng scale/lifetime nếu effect đọc quá nhỏ hoặc quá nhanh.",
            ],
        ),
        (
            "52. 'Nhìn giả quá'",
            "Giả thường đến từ motion quá đều, mask quá sạch, color quá phẳng hoặc thiếu interaction. Đây là nơi kinh nghiệm AE của bạn rất hữu ích: bạn biết khi nào layer thiếu variation. Trong shader, variation có thể đến từ noise scale, particle random, UV speed khác nhau, hoặc world interaction.",
            [
                "Thêm layer noise khác scale/speed, nhưng giữ silhouette chính.",
                "Randomize color/alpha/size trong Niagara.",
                "Thêm depth fade, hit response, hoặc camera fade nếu effect không bám scene.",
                "Giảm pattern quá đều như sine/panner đơn.",
            ],
        ),
        (
            "53. 'Không đọc được'",
            "Không đọc được thường là lỗi hierarchy. Effect có nhiều detail nhưng thiếu hình lớn, hoặc màu hòa vào background. Đừng bắt đầu bằng thêm detail. Hãy quay về grayscale mask và xem silhouette trong 0.5 giây đầu.",
            [
                "Solo mask chính và xem từ xa.",
                "Tăng khác biệt core/edge/background.",
                "Giảm detail nhỏ đang che shape lớn.",
                "Test trên background sáng/tối và gameplay distance.",
            ],
        ),
        (
            "54. 'Nặng quá'",
            "Nặng quá không nói rõ nguyên nhân. Có thể do overdraw, particle count, texture resolution, refraction, lit translucency, scene depth hoặc shader instructions. Technical VFX Artist phải đo trước khi cắt.",
            [
                "Xem shader complexity/overdraw.",
                "Giảm card size/tighten alpha nếu translucent phủ màn hình.",
                "Giảm spawn count/lifetime nếu nhiều particle chồng nhau.",
                "Tắt feature đắt như refraction/lit translucency nếu không cần.",
            ],
        ),
        (
            "55. 'Muốn organic hơn'",
            "Organic không có nghĩa là thêm noise thật nhiều. Organic là variation có nhịp tự nhiên: không quá đều, không quá random. Với material, bạn cần phối noise lớn/chậm với detail nhỏ/nhanh và để shape chính vẫn có ý đồ.",
            [
                "Dùng hai noise scale khác nhau.",
                "Thêm random per-particle cho speed, alpha, scale.",
                "Tránh pattern lặp tile rõ.",
                "Giữ hướng motion chính để effect vẫn đọc.",
            ],
        ),
        (
            "56. 'Muốn stylized/anime hơn'",
            "Stylized thường cần shape rõ, contrast mạnh, timing có punch, color có chủ đích và ít noise bẩn. Đừng làm mọi thứ realistic nếu style cần graphic clarity.",
            [
                "Dùng mask rõ và edge có thiết kế.",
                "Giảm noise li ti, tăng shape lớn.",
                "Dùng color ramp ít màu nhưng value rõ.",
                "Timing nhanh hơn, attack rõ hơn, fade sạch hơn.",
            ],
        ),
        (
            "57. 'Muốn realistic hơn'",
            "Realistic thường cần sự không hoàn hảo có kiểm soát: motion không đều, alpha mềm, depth interaction, lighting hoặc flipbook/sim data tốt hơn. Nhưng realism cũng dễ tốn hơn, nên phải chọn detail người chơi thật sự thấy.",
            [
                "Ưu tiên flipbook/sim texture cho smoke/fire phức tạp.",
                "Thêm DepthFade và lighting nếu cần bám scene.",
                "Giảm color quá saturated nếu style realistic.",
                "Kiểm overdraw vì realistic smoke thường dày.",
            ],
        ),
        (
            "58. 'Viền bị cứng'",
            "Viền cứng có thể do Step/Masked clip, alpha texture quá sắc, thiếu DepthFade hoặc compression/mip làm edge xấu. Cách sửa phụ thuộc viền là silhouette edge, dissolve edge hay scene intersection edge.",
            [
                "Nếu là alpha edge: dùng SmoothStep/Power mềm hơn.",
                "Nếu là giao scene: dùng DepthFade.",
                "Nếu là masked surface: kiểm clip threshold/dither/AA.",
                "Nếu là texture: kiểm alpha compression và mip.",
            ],
        ),
        (
            "59. 'Motion bị rẻ'",
            "Motion rẻ thường là một panner đơn chạy thẳng. Người xem thấy texture trượt như tấm giấy. Cách sửa là tạo nhiều lớp motion có quan hệ, dùng flow/distortion nhẹ, hoặc để particle/mesh motion góp phần.",
            [
                "Thêm layer noise khác tốc độ/hướng.",
                "Distort UV detail thay vì pan toàn bộ silhouette.",
                "Randomize start offset theo particle.",
                "Dùng polar/radial motion nếu effect là vòng.",
            ],
        ),
        (
            "60. 'Màu bị bẩn'",
            "Màu bẩn thường do nhân nhiều RGB texture với nhau, dùng texture color không kiểm soát, hoặc bloom/exposure làm hue cháy. Hãy tách color lane khỏi mask lane.",
            [
                "Dùng grayscale mask + color parameter thay vì texture RGB nếu cần tint sạch.",
                "Tắt bloom để xem hue thật.",
                "Giữ một nguồn quyết định hue chính.",
                "Kiểm sRGB của texture màu và mask.",
            ],
        ),
        (
            "61. 'Không hòa vào scene'",
            "Effect không hòa scene có thể thiếu depth fade, lighting, scale đúng, color temperature đúng, hoặc timing đúng với animation/gameplay. Đây không chỉ là material, mà là quan hệ giữa material và context.",
            [
                "Test trong level thật với exposure thật.",
                "Thêm DepthFade/CameraFade nếu là translucent particle.",
                "Điều chỉnh color value theo lighting/background.",
                "Kiểm scale và spawn position trong gameplay.",
            ],
        ),
        (
            "62. 'Khó chỉnh quá'",
            "Đây là feedback về tool, không phải visual. Một graph có thể đẹp nhưng parameter tệ làm production chậm. Bạn cần đổi parameter thành control có nghĩa visual và tạo material instance default tốt.",
            [
                "Đổi tên parameter theo mục đích: Radius, Width, Softness, FlowSpeed.",
                "Ẩn bớt parameter ít dùng.",
                "Tạo preset instance làm điểm xuất phát.",
                "Comment graph theo lane để người khác đọc được.",
            ],
        ),
    ]
    for title, explanation, actions in feedback_cards:
        h1(doc, title)
        body(doc, explanation)
        h2(doc, "Hành động nên thử")
        for action in actions:
            bullet(doc, action)
        callout(doc, "Câu hỏi tự kiểm", [
            "Feedback này đang nhắm vào shape, mask, motion, color, interaction hay cost?",
            "Có cách debug bằng grayscale hoặc tắt bloom để kiểm trước khi chỉnh tiếp không?",
        ], fill=LIGHT_TEAL)

    chapter(doc, 19, "Glossary production để tra nhanh", "Nắm các từ khóa lead, tech artist và graphics engineer hay dùng khi nói về material VFX.")
    glossary_items = [
        ("63. Silhouette", "Hình lớn của effect. Nếu silhouette không đọc, detail đẹp cũng không cứu được. Fire card, portal ring, beam và shield đều cần silhouette rõ ở gameplay distance."),
        ("64. Core", "Vùng năng lượng chính, thường sáng nhất hoặc dày nhất. Core giúp mắt biết effect bắt đầu từ đâu và lực chính nằm ở đâu."),
        ("65. Edge", "Vùng rìa hoặc chuyển tiếp. Edge có thể mềm, cháy sáng, bị dissolve, hoặc dùng làm accent. Edge tốt giúp effect không phẳng."),
        ("66. Breakup", "Sự phá đều của mask/pattern. Breakup làm material bớt sạch, nhưng quá nhiều breakup sẽ làm mất shape chính."),
        ("67. Falloff", "Độ giảm từ mạnh sang yếu. Radial gradient, Fresnel, alpha power, depth fade đều là falloff dưới nhiều hình thức."),
        ("68. Overdraw", "Khi nhiều lớp translucent vẽ chồng lên cùng pixel. Đây là cost lớn của smoke, fire, magic glow và UI-like VFX."),
        ("69. Sorting", "Thứ tự render của translucent objects. Sorting sai làm smoke, glass, portal hoặc shield layer nhìn đảo lộn."),
        ("70. Bounds", "Vùng giới hạn để engine culling object/particle. WPO lớn hoặc particle bay khỏi bounds có thể làm effect biến mất."),
        ("71. Shader instruction", "Số phép toán shader. Quan trọng, nhưng không phải cost duy nhất. Với translucent, overdraw có thể đáng sợ hơn."),
        ("72. Texture fetch", "Một lần đọc texture. Nhiều texture sample lớn trên effect phủ màn hình sẽ tốn, đặc biệt với translucent."),
        ("73. Atlas", "Texture chứa nhiều frame hoặc nhiều sprite. Cần grid, padding và import setting đúng để tránh bleeding/sai frame."),
        ("74. Flipbook", "Animation bake thành frame trong atlas. Hợp smoke/fire/explosion phức tạp, nhưng tốn memory và ít procedural control hơn."),
        ("75. Soft particle", "Particle mềm ở giao tuyến với scene, thường dùng DepthFade hoặc depth math để tránh cắt cứng."),
        ("76. Rim", "Vùng sáng ở rìa theo góc nhìn, thường tạo bằng Fresnel. Dùng nhiều cho shield, hologram, shell, forcefield."),
        ("77. WPO", "World Position Offset: đẩy vertex bằng shader. Hợp pulse/deform mesh nhưng phụ thuộc vertex density và bounds."),
        ("78. Channel packing", "Đóng nhiều mask vào R/G/B/A. Tốt cho memory/fetch, nhưng cần naming rõ để tránh dùng nhầm."),
        ("79. Material Instance", "Bản chỉnh parameter của master material. Đây là nơi production nên thao tác nhiều nhất, không phải luôn mở master graph."),
        ("80. Master material", "Graph gốc cho một family effect. Master tốt có phạm vi rõ, parameter ít nhưng đúng, default đẹp và comment dễ đọc."),
    ]
    for title, text in glossary_items:
        h1(doc, title)
        body(doc, text)
        callout(doc, "Khi gặp từ này, hãy hỏi", [
            "Nó ảnh hưởng visual layer nào?",
            "Có metric/debug view nào kiểm được không?",
            "Nó đang là vấn đề art direction hay performance?",
        ], fill=LIGHT_BLUE)

    chapter(doc, 20, "Phụ lục công thức phải nhớ", "Các công thức nhỏ nhưng xuất hiện liên tục trong material VFX.")
    formula_cards = [
        ("81. Soft circle", ["r = length(UV - Center)", "circle = 1 - smoothstep(Radius, Radius + Softness, r)"], "Dùng cho flare, shockwave fill, magic circle mềm."),
        ("82. Ring band", ["r = length(UV - Center)", "ring = smoothstep(Radius - Width, Radius, r) - smoothstep(Radius, Radius + Softness, r)"], "Dùng cho portal, shockwave, ripple, impact ring."),
        ("83. Dissolve visible", ["visible = smoothstep(Amount, Amount + Softness, Mask)"], "Dùng cho reveal/destroy theo threshold."),
        ("84. Dissolve edge", ["edge = smoothstep(Amount - EdgeWidth, Amount, Mask) - smoothstep(Amount, Amount + Softness, Mask)"], "Dùng cho viền cháy, viền băng, magic edge."),
        ("85. Fresnel thủ công", ["facing = dot(normalize(NormalWS), normalize(ViewDirWS))", "rim = pow(1 - saturate(facing), RimPower)"], "Dùng cho shield, hologram, rim glow."),
        ("86. Panning noise hai lớp", ["uvA = UV * TilingA + Time * SpeedA", "uvB = UV * TilingB - Time * SpeedB", "mask = NoiseA(uvA) * NoiseB(uvB)"], "Dùng cho fire, energy, shield pattern."),
        ("87. Alpha power", ["alpha = pow(TextureAlpha, AlphaPower) * ParticleColor.a"], "Dùng để làm alpha dày/mỏng, core gắt/mềm."),
        ("88. Camera/depth fade stack", ["fade = ParticleColor.a * CameraDepthFade(CameraFadeLength)", "opacity = DepthFade(alpha * fade, FadeDistance)", "emissive *= fade"], "Dùng cho sprite lớn gần camera hoặc cắt scene."),
        ("89. Hit ripple", ["d = distance(WorldPos, HitPos)", "ring = smoothstep(Radius - Width, Radius, d) - smoothstep(Radius, Radius + Width, d)"], "Dùng cho shield impact, ground ripple, portal reaction."),
        ("90. Polar UV", ["p = UV - 0.5", "r = length(p)", "a = atan2(p.y, p.x) / TWO_PI", "polarUV = float2(a + Time * Spin, r)"], "Dùng cho portal, vortex, black hole, magic circle."),
        ("91. WPO pulse", ["pulse = sin(Time * Speed) * 0.5 + 0.5", "WPO = VertexNormalWS * pulse * Amount * Mask"], "Dùng cho shield shell, energy mesh, mesh pulse."),
        ("92. Remap 0-1", ["remap = saturate((x - InMin) / (InMax - InMin))"], "Dùng khi source mask chỉ có vùng hữu ích trong một khoảng nhỏ."),
    ]
    for title, lines, use in formula_cards:
        h1(doc, title)
        code(doc, lines, "Công thức")
        body(doc, use)
        callout(doc, "Cách học", [
            "Nối output ra grayscale trước khi đưa vào màu hoặc opacity.",
            "Expose ít nhất một parameter chính để cảm nhận range.",
        ], fill=LIGHT_TEAL)

    callout(doc, "Kết luận", [
        "Shader VFX là ngôn ngữ. Node là từ, mask/UV/color là ngữ pháp, recipe là mẫu câu, production feedback là bài nói thật.",
        "Bạn master không phải vì biết mọi node. Bạn master khi nhìn visual phức tạp và biết chuyển nó thành data, mask, motion, output và cost.",
    ], fill=LIGHT_GOLD, color=GOLD)

    doc.core_properties.title = "Giáo Trình Material VFX Realtime V2"
    doc.core_properties.subject = "Unreal Material, Niagara, shader fundamentals, VFXSamplePack"
    doc.core_properties.author = "Codex"
    doc.save(OUT)
    print(OUT)


if __name__ == "__main__":
    build()
