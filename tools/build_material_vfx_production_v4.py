from pathlib import Path

from docx import Document
from docx.enum.text import WD_ALIGN_PARAGRAPH
from docx.oxml import OxmlElement
from docx.oxml.ns import qn
from docx.shared import Cm, Pt, RGBColor


OUT = Path.home() / "Downloads" / "Giao_Trinh_Material_VFX_Realtime_V4_Production.docx"

BLACK = RGBColor(14, 14, 14)
DARK = RGBColor(42, 42, 42)
MUTED = RGBColor(100, 100, 100)
PAPER = "F5F5F5"
WARN = "F4E7E7"
LINE = "D9D9D9"


def set_font(run, size=10.4, bold=False, italic=False, color=BLACK, name="Arial"):
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


def border_bottom(paragraph):
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
    bottom.set(qn("w:sz"), "7")
    bottom.set(qn("w:space"), "3")
    bottom.set(qn("w:color"), LINE)


def p(doc, text="", size=10.25, bold=False, italic=False, color=BLACK, before=0, after=4.2, align=None):
    para = doc.add_paragraph()
    para.paragraph_format.space_before = Pt(before)
    para.paragraph_format.space_after = Pt(after)
    para.paragraph_format.line_spacing = 1.12
    if align is not None:
        para.alignment = align
    if text:
        run = para.add_run(text)
        set_font(run, size=size, bold=bold, italic=italic, color=color)
    return para


def h_ch(doc, title):
    para = p(doc, title, size=17.2, bold=True, before=12, after=7)
    border_bottom(para)


def h1(doc, title):
    para = p(doc, title, size=13.6, bold=True, color=DARK, before=9, after=4.5)
    border_bottom(para)


def h2(doc, title):
    p(doc, title, size=11.3, bold=True, color=DARK, before=7, after=3)


def body(doc, text):
    p(doc, text, size=10.35, after=4.8)


def bullet(doc, text):
    para = doc.add_paragraph()
    para.paragraph_format.left_indent = Cm(0.55)
    para.paragraph_format.first_line_indent = Cm(-0.25)
    para.paragraph_format.space_after = Pt(2.6)
    para.paragraph_format.line_spacing = 1.08
    run = para.add_run("- " + text)
    set_font(run, size=9.9)


def box(doc, title, lines, fill=PAPER):
    para = p(doc, title, size=9.8, bold=True, color=DARK, before=5, after=1)
    para.paragraph_format.left_indent = Cm(0.24)
    shade(para, fill)
    for line in lines:
        para = doc.add_paragraph()
        para.paragraph_format.left_indent = Cm(0.55)
        para.paragraph_format.right_indent = Cm(0.2)
        para.paragraph_format.space_after = Pt(1.7)
        para.paragraph_format.line_spacing = 1.06
        shade(para, fill)
        run = para.add_run("- " + line)
        set_font(run, size=9.55)


def code(doc, lines, title=None):
    if title:
        p(doc, title, size=8.9, italic=True, color=MUTED, after=1)
    for line in lines:
        para = p(doc, line, size=8.8, after=0.8)
        para.paragraph_format.left_indent = Cm(0.35)
        shade(para, PAPER)
        for run in para.runs:
            set_font(run, size=8.8, name="Consolas")


def cover(doc):
    p(doc, "", before=3)
    p(doc, "Material VFX Realtime", size=24, bold=True, align=WD_ALIGN_PARAGRAPH.CENTER, after=1)
    p(doc, "Production Notes / Dirty Version", size=13, align=WD_ALIGN_PARAGRAPH.CENTER, after=8)
    body(doc, "Bản này không cố nói cho tròn trịa. Nó viết cho lúc bạn mở Unreal, nhìn một graph rối, bị lead nói 'nhìn giả', bị mobile đỏ shader complexity, hoặc một particle không chịu fade dù curve Niagara đã đúng.")
    body(doc, "Nếu bản trước giống sách học sạch sẽ, bản này giống sổ tay của người đã ăn lỗi production: ít màu, chữ đen, nhiều ca hỏng, nhiều mẹo rẻ tiền nhưng chạy được. Không có mục tiêu làm bạn thấy giỏi ngay. Mục tiêu là làm bạn bớt sửa mò.")
    box(doc, "Luật đọc", [
        "Đừng tin material editor preview quá nhiều. Level thật, exposure thật, camera thật mới là bài test.",
        "Đừng thêm màu khi mask grayscale chưa đọc.",
        "Đừng tối ưu bằng cảm giác. Mở shader complexity, overdraw, instruction, texture size, particle count.",
        "Đừng thần thánh hóa best practice. Production đôi khi cần cách xấu nhưng rẻ, miễn bạn hiểu trade-off.",
    ])
    p(doc, "Tông chữ chủ đạo: đen. Màu chỉ dùng cho box nhắc lỗi.", size=8.6, color=MUTED, align=WD_ALIGN_PARAGRAPH.CENTER, before=5)


def toc(doc):
    h1(doc, "Mục lục")
    for item in [
        "1. Cái gì còn thiếu trong các giáo trình shader sạch sẽ",
        "2. Debug trước khi thêm node",
        "3. Toán không chỉ là Dot và Sine",
        "4. Depth, sorting, exposure: ba thứ phá mood nhiều nhất",
        "5. Optimization cho mobile/iGaming",
        "6. Dirty tricks dùng được nếu biết giá phải trả",
        "7. Case study thảm họa",
        "8. Đọc VFXSamplePack theo kiểu production",
        "9. Feedback translation không nịnh bợ",
        "10. Lộ trình luyện thành TA thực chiến",
    ]:
        bullet(doc, item)
    doc.add_page_break()


def chapter_1(doc):
    h_ch(doc, "1. Cái còn thiếu trong giáo trình shader sạch sẽ")
    body(doc, "Tài liệu shader thường có một bệnh: nó dạy như thể mọi thứ diễn ra trong phòng thí nghiệm. Node đúng, công thức đúng, output đúng. Production không lịch sự như vậy. Bạn có graph đúng nhưng exposure làm cháy màu. Bạn có alpha đẹp nhưng translucent sorting làm layer đảo. Bạn có WPO đẹp nhưng bounds cắt mất mesh. Bạn có flipbook đúng nhưng compression làm alpha bẩn.")
    body(doc, "Một Technical VFX Artist không được trả tiền để biết node SmoothStep là gì. Người đó được trả tiền để biết khi nào SmoothStep không cứu được visual vì vấn đề nằm ở import setting, blend mode, camera, particle count, hoặc style direction. Đây là khác biệt giữa biết shader và làm shader trong dự án.")
    h1(doc, "Tư duy bớt sạch")
    body(doc, "Best practice là điểm xuất phát, không phải luật tôn giáo. Nếu effect chỉ xuất hiện 0.2 giây, đôi khi một texture bake xấu xí nhưng rẻ tốt hơn một graph procedural thanh lịch. Nếu mobile đang cháy overdraw, một masked/dithered trick có thể đáng cân nhắc dù edge không đẹp bằng translucent. Nếu iGaming cần chạy trên máy yếu và animation lặp hàng giờ, ổn định quan trọng hơn shader clever.")
    box(doc, "Câu hỏi production", [
        "Effect này xuất hiện bao lâu trên màn hình?",
        "Nó spawn một lần hay spam 200 lần?",
        "Người chơi nhìn nó từ khoảng cách nào?",
        "Nó chạy trên PC, console, mobile hay web?",
        "Có ai cần chỉnh nó sau bạn không?",
    ])
    body(doc, "Nếu không trả lời được năm câu này, mọi quyết định shader chỉ là đoán.")


def chapter_2(doc):
    h_ch(doc, "2. Debug trước khi thêm node")
    body(doc, "Người mới thấy lỗi thì thêm node. Người có kinh nghiệm cô lập lỗi. Shader graph càng lớn, việc thêm node càng dễ làm bạn xa nguyên nhân thật. Debug shader giống khám bệnh: nhìn triệu chứng, cô lập hệ, test giả thuyết nhỏ nhất.")
    h1(doc, "Solo grayscale là thói quen bắt buộc")
    body(doc, "Nếu mask không đẹp ở grayscale, bản màu chưa đáng tin. Đừng để emissive, bloom, LUT hoặc color ramp lừa mắt. Nối mask chính ra Emissive. Nối edge mask ra Emissive. Nối DepthFade factor ra Emissive. Nối ParticleColor.a ra Emissive. Bạn cần nhìn từng đường data như xem solo layer trong AE.")
    code(doc, [
        "Debug 1: Emissive = MainMask",
        "Debug 2: Emissive = EdgeBand",
        "Debug 3: Emissive = ParticleColor.a",
        "Debug 4: Emissive = DepthFadeFactor",
        "Debug 5: Emissive = UV checker / NumberGrid",
    ], "Debug outputs")
    h1(doc, "Debug bằng texture xấu")
    body(doc, "Một trick rất đời: dùng checker hoặc NumberGrid thay texture đẹp. Noise đẹp che lỗi UV rất giỏi. NumberGrid thì không. Nó cho bạn biết UV bị xoay, lật, stretch, pan sai hướng hoặc sample nhầm frame. Với SubUV, NumberGrid 3x3 hoặc 4x4 là công cụ học tốt hơn mọi lời giải thích.")
    h1(doc, "Debug theo thứ tự")
    for item in [
        "Material property: blend mode, shading model, two sided.",
        "Texture import: sRGB, compression, alpha, mip, resolution.",
        "Renderer: material slot, SubUV grid, ribbon UV, mesh UV channel.",
        "Niagara binding: Particle Color, Dynamic Parameter, custom data.",
        "Graph lane: UV, mask, color, opacity, depth, WPO.",
        "Scene context: exposure, post-process, lighting, background, camera distance.",
    ]:
        bullet(doc, item)
    body(doc, "Thứ tự này không đẹp, nhưng nó cứu thời gian. Đừng sửa graph nếu texture import sai. Đừng sửa texture nếu Niagara không gửi alpha.")


def chapter_3(doc):
    h_ch(doc, "3. Toán không chỉ là Dot và Sine")
    body(doc, "Dot, Distance, Sine là cửa vào. Nhưng nếu bạn muốn đi hướng Technical Artist thật, bạn cần hiểu space. Rất nhiều bug shader không phải do công thức sai, mà do bạn lấy dữ liệu ở hai không gian khác nhau rồi bắt chúng nói chuyện.")
    h1(doc, "Space là nguồn bug âm thầm")
    body(doc, "UV space là mặt phẳng 0-1. Object/local space đi theo object. World space nằm trong level. View/screen space phụ thuộc camera. Tangent space phụ thuộc normal map và tangent basis. Nếu bạn dot một normal tangent với một view vector world, graph vẫn compile. Visual chỉ sai một cách khó chịu.")
    code(doc, [
        "Wrong idea:",
        "dot(TangentNormal, ViewDirWS)",
        "",
        "Better idea:",
        "NormalWS = Transform(TangentNormal, Tangent -> World)",
        "dot(normalize(NormalWS), normalize(ViewDirWS))",
    ], "Space mismatch")
    h1(doc, "Ma trận không cần thần bí")
    body(doc, "Ma trận trong shader thường chỉ làm một việc: đổi tọa độ hoặc xoay/scale/translate vector. Bạn không cần học linear algebra như kỹ sư graphics ngay ngày đầu. Nhưng bạn cần hiểu rằng mọi Transform node đang nhân dữ liệu với một ma trận nào đó để chuyển space.")
    body(doc, "Khi làm radial effect trên mesh, nếu dùng UV thì pattern đi theo unwrap. Nếu dùng WorldPosition, pattern đứng trong world. Nếu dùng ObjectPosition/ObjectRadius, pattern scale tương đối với object. Ba lựa chọn này đều đúng trong tình huống khác nhau.")
    box(doc, "Chọn space theo ý đồ", [
        "UV: pattern theo texture unwrap, tốt cho sprite/card/mesh unwrap đẹp.",
        "Object/local: pattern đi theo object, tốt cho shield shell hoặc dissolve theo mesh.",
        "World: pattern bám thế giới, tốt cho height dissolve, global field, wet/ice surface.",
        "Screen/view: pattern theo camera, tốt cho post-process, distortion, screen fade.",
    ])
    h1(doc, "SDF và signed distance thinking")
    body(doc, "Một bước nâng cấp đáng học là nghĩ theo distance field. Ring, circle, box, line, shockwave đều có thể mô tả bằng khoảng cách tới một hình. Khi có distance, bạn dùng SmoothStep để biến nó thành mask. Đây là nền của rất nhiều shader stylized.")
    code(doc, [
        "circleDistance = length(UV - Center) - Radius",
        "circleMask = 1 - smoothstep(0, Softness, circleDistance)",
        "",
        "ringDistance = abs(length(UV - Center) - Radius)",
        "ringMask = 1 - smoothstep(Width, Width + Softness, ringDistance)",
    ], "Distance thinking")
    body(doc, "Cách này giúp bạn bớt phụ thuộc texture. Nhưng đừng lạm dụng. Nếu art cần organic erosion phức tạp, texture/noise bake vẫn có thể tốt hơn.")


def chapter_4(doc):
    h_ch(doc, "4. Depth, sorting, exposure: ba thứ phá mood nhiều nhất")
    h1(doc, "DepthFade không phải thuốc chữa bách bệnh")
    body(doc, "DepthFade làm mềm giao tuyến giữa translucent particle và geometry. Nó không sửa alpha xấu, không sửa card quá lớn, không sửa overdraw, và không sửa sorting. Nếu smoke của bạn vẫn xấu sau DepthFade, có thể vấn đề là sprite placement hoặc alpha vùng trống quá rộng.")
    code(doc, [
        "opacityRaw = alpha * ParticleColor.a",
        "opacitySoft = DepthFade(opacityRaw, FadeDistance)",
        "Opacity = opacitySoft",
    ], "DepthFade baseline")
    h1(doc, "Translucent sorting là lỗi không sạch sẽ")
    body(doc, "Translucent không ghi depth như opaque theo cách bạn mong muốn. Nhiều layer smoke, glass, portal, shield có thể vẽ sai thứ tự. Bạn sẽ thấy layer sau nhảy lên trước, hoặc edge biến mất khi camera đổi góc. Đây không phải lúc nào cũng sửa bằng material. Đôi khi cần đổi sort priority, tách mesh, giảm layer, hoặc đổi thiết kế.")
    box(doc, "Khi nghi sorting", [
        "Xoay camera. Nếu lỗi đổi theo góc, nghi sorting.",
        "Tắt từng layer translucent.",
        "Thử đổi sort priority hoặc renderer order.",
        "Nếu layer không cần translucent thật, cân nhắc masked/dithered hoặc mesh khác.",
    ], fill=WARN)
    h1(doc, "Exposure giết emissive như thế nào")
    body(doc, "Material preview thường nói dối về brightness. Trong level thật có auto exposure, bloom, tone mapping, color grading. Một flare đẹp trong preview có thể cháy trắng trong cave tối hoặc mất hết trên outdoor sáng. Vì vậy, test emissive phải ở nhiều exposure.")
    body(doc, "Dirty trick: làm một test map có ba vùng nền: đen, mid gray, sáng. Đặt effect lên cả ba. Nếu chỉ đọc trên nền đen, additive của bạn chưa production-safe.")


def chapter_5(doc):
    h_ch(doc, "5. Optimization cho mobile/iGaming")
    body(doc, "Mobile và iGaming không tha cho shader lãng mạn. Nhiều game iGaming cần chạy mượt, lặp animation liên tục, trên máy yếu hoặc web view. Ở đó, một effect đẹp nhưng overdraw nặng là thất bại. Bạn cần nghĩ như người tiết kiệm từng texture fetch.")
    h1(doc, "Channel packing là kỹ năng kiếm tiền")
    body(doc, "Thay vì bốn texture mask, pack bốn mask vào RGBA. Một sample trả về bốn dữ liệu. R là shape, G là breakup, B là edge, A là alpha/height. Cái giá là bạn phải quản lý channel rõ và import đúng. Nếu team không biết R/G/B/A chứa gì, optimization này biến thành bẫy.")
    code(doc, [
        "packed = TextureSample(MaskPack, UV)",
        "shape   = packed.r",
        "breakup = packed.g",
        "edgeSrc = packed.b",
        "alpha   = packed.a",
    ], "One fetch, four masks")
    h1(doc, "Giảm overdraw trước khi xóa math")
    body(doc, "Một Multiply rẻ hơn nhiều so với một sprite translucent khổng lồ phủ nửa màn hình. Nếu shader complexity đỏ, hãy nhìn card size, alpha bounds và particle count trước. Tighten alpha texture, giảm vùng transparent, giảm sprite size, giảm lifetime hoặc spawn count.")
    for item in [
        "Dùng alpha tight: vùng trong suốt không nên vẫn vẽ quá nhiều.",
        "Cắt sprite sheet/crop texture để giảm vùng trống.",
        "Giảm particle lifetime nếu effect tồn tại lâu hơn cảm nhận visual.",
        "Dùng LOD hoặc scalability: mobile bỏ layer distortion/secondary glow.",
        "Bake procedural noise thành texture nếu graph lặp nhiều trên màn hình lớn.",
        "Dùng vertex color hoặc custom data thay texture nếu mesh đã có data rẻ.",
    ]:
        bullet(doc, item)
    h1(doc, "iGaming trick: animation đẹp nhưng shader ngu")
    body(doc, "Trong iGaming, rất nhiều FX có thể là flipbook/sprite sheet được chuẩn bị kỹ, shader cực đơn giản: sample atlas, tint, alpha, maybe additive. Đừng xấu hổ vì shader 'ngu'. Nếu art direction cố định và không cần interaction, texture bake tốt có thể thắng procedural graph đẹp.")
    box(doc, "Mobile budget mindset", [
        "Một texture fetch ít hơn có thể đáng hơn một graph thông minh.",
        "Một layer glow bỏ đi có thể cứu FPS hơn 10 node math.",
        "Một effect spawn 50 lần phải bị xét xử khắt khe hơn hero effect spawn 1 lần.",
    ])


def chapter_6(doc):
    h_ch(doc, "6. Dirty tricks dùng được nếu biết giá")
    body(doc, "Dirty trick không phải hack ngu. Nó là trade-off có ý thức. Bạn dùng nó khi biết nó phá gì, cứu gì, và trong context nào nó chấp nhận được.")
    h1(doc, "Dùng texture bake thay procedural thanh lịch")
    body(doc, "Nếu một swirl portal cần 40 node procedural nhưng camera chỉ thấy 1 giây, bake swirl thành texture hoặc flipbook. Shader chỉ pan/tint/fade. Nhìn kém 'tech' hơn, nhưng production có thể tốt hơn.")
    h1(doc, "Fake lighting bằng gradient")
    body(doc, "Không phải smoke/fire nào cũng cần lit translucency. Đôi khi một gradient theo UV.y hoặc particle color theo lifetime giả được lighting đủ tốt. Rẻ hơn, ổn định hơn, ít phụ thuộc level lighting hơn.")
    h1(doc, "Dither thay translucent")
    body(doc, "Một số platform chịu translucent kém. Dithered masked fade có thể thay alpha translucent trong vài trường hợp. Nó có noise/temporal artifacts, nhưng depth/sorting có thể dễ chịu hơn. Đừng dùng mù; test camera motion.")
    h1(doc, "Pre-multiply fade vào emissive")
    body(doc, "Với additive sprites, nếu opacity không làm fade như mong muốn, nhân fade thẳng vào emissive. Đây không phải trick lạ, nhưng người mới hay quên.")
    code(doc, [
        "fade = ParticleColor.a * CameraFade",
        "Emissive = TextureRGB * ParticleColor.rgb * Intensity * fade",
    ], "Additive fade")
    h1(doc, "Che seam bằng thiết kế")
    body(doc, "Polar UV có seam. Thay vì cố làm seam biến mất hoàn toàn, đặt seam ở vùng bị mask che, xoay seam ra sau camera, hoặc dùng noise tile tốt. Đây là production thinking: không phải mọi lỗi cần giải bằng math phức tạp.")
    box(doc, "Nhớ giá phải trả", [
        "Bake texture: ít dynamic control hơn.",
        "Fake lighting: ít hòa vào scene hơn.",
        "Dither: có artifact.",
        "Masked thay translucent: edge kém mềm.",
        "Che seam bằng mask: phụ thuộc góc nhìn/setup.",
    ], fill=WARN)


def chapter_7(doc):
    h_ch(doc, "7. Case study thảm họa")
    cases = [
        ("Flare đẹp trong editor, vào level cháy trắng", "Nguyên nhân thường là exposure/bloom khác preview. Cách xử: test trong map có exposure thật, giảm intensity, tăng alpha/core control thay vì glow toàn bộ, clamp hoặc scale theo gameplay context."),
        ("Smoke flipbook đúng nhưng frame nhảy sai", "Renderer SubUV grid sai, frame order khác atlas, hoặc texture bleeding do thiếu padding. Cách xử: NumberGrid trước, kiểm SubImageSize, padding, mip."),
        ("Shield hit ripple lệch vị trí", "HitPosition gửi world space nhưng material so với object/local, hoặc mesh pivot/bounds lệch. Cách xử: debug distance(WorldPos, HitPos) ra grayscale, thống nhất space."),
        ("Portal lúc đẹp lúc mất viền", "Translucent sorting hoặc interior opacity lấn edge. Cách xử: tách ring edge rõ, giảm interior, kiểm sort priority, cân nhắc mesh/layer order."),
        ("Dissolve edge răng cưa trên mobile", "Masked clip cứng, texture compression, thiếu dither/AA hoặc mask resolution thấp. Cách xử: kiểm import, tăng softness vừa đủ, dùng dither hoặc texture tốt hơn."),
        ("WPO mesh biến mất khi pulse mạnh", "Bounds không đủ. Cách xử: tăng bounds, giảm WPO amplitude, kiểm LOD/culling."),
        ("Niagara alpha curve không có tác dụng", "Material không dùng ParticleColor.a đúng path, đặc biệt Additive không nhân fade vào Emissive. Cách xử: debug ParticleColor.a ra Emissive."),
        ("Shader complexity đỏ dù graph đơn giản", "Overdraw do sprite lớn, alpha vùng trống, spawn count dày. Cách xử: crop texture, tighten alpha, giảm size/lifetime/spawn."),
    ]
    for title, text in cases:
        h1(doc, title)
        body(doc, text)
        box(doc, "Bài học", [
            "Đừng sửa node trước khi biết lỗi thuộc texture, renderer, scene, Niagara hay graph.",
            "Luôn có debug view đơn giản để chứng minh giả thuyết.",
        ])


def chapter_8(doc):
    h_ch(doc, "8. Đọc VFXSamplePack theo kiểu production")
    body(doc, "Đọc sample pack không phải là ngắm graph. Hãy giả sử bạn phải sửa nó lúc 2 giờ sáng vì build mobile đỏ. Bạn cần biết parameter nào quan trọng, layer nào có thể bỏ, và lỗi nào dễ phát sinh.")
    for title, notes in [
        ("M_Flare", ["Kiểm additive fade: ParticleColor.a có scale Emissive không?", "Kiểm AlphaPower/Intensity có range hợp lý không?", "Test nền sáng, nền tối, camera gần."]),
        ("M_Mesh_Add", ["Tìm WPO và bounds risk.", "Mesh data dùng world/object/UV space nào?", "Layer nào là core visual, layer nào có thể tắt cho low quality?"]),
        ("M_FlamesCard", ["Tách UV motion khỏi alpha silhouette.", "Distortion có phá silhouette không?", "Có crop/tight alpha để giảm overdraw không?"]),
        ("M_SubUV_Utilities", ["Dùng NumberGrid để xác minh frame/local UV.", "Derived 0-1 UV đang giải quyết vấn đề gì?", "Có nguy cơ sample bleed giữa frame không?"]),
    ]:
        h1(doc, title)
        for note_line in notes:
            bullet(doc, note_line)
    body(doc, "Bài tập thật: rebuild một bản nhỏ hơn. Nếu bản nhỏ giữ được 70% cảm giác chính, bạn đã hiểu graph. Nếu không, bạn chỉ đang copy.")


def chapter_9(doc):
    h_ch(doc, "9. Feedback translation không nịnh bợ")
    feedback = [
        ("Mạnh hơn", "Đừng kéo intensity ngay. Hỏi mạnh ở đâu: core, scale, contrast, timing hay screen coverage? Tăng toàn bộ chỉ làm cháy."),
        ("Organic hơn", "Không phải thêm noise vô tội vạ. Organic là variation có hướng. Dùng noise lớn/chậm cộng detail nhỏ/nhanh, thêm random per particle."),
        ("Stylized hơn", "Giảm realism bẩn. Tăng shape rõ, contrast rõ, timing punch, color ramp ít màu nhưng chắc."),
        ("Realistic hơn", "Thêm imperfection, depth interaction, lighting/flipbook tốt hơn. Nhưng realistic thường đắt hơn; chọn detail người chơi thấy."),
        ("Không đọc", "Quay về silhouette. Nếu grayscale từ xa không đọc, mọi color correction là vô nghĩa."),
        ("Nặng", "Đo trước. Overdraw hay instruction? Particle count hay texture fetch? Không có số đo thì chỉ đang đoán."),
    ]
    for title, text in feedback:
        h1(doc, title)
        body(doc, text)
    box(doc, "Một câu nên nói với lead", [
        "Em sẽ thử sửa theo hai hướng: tăng core/contrast trước, không tăng glow toàn bộ để tránh cháy exposure.",
        "Vấn đề này có vẻ là overdraw từ sprite size và alpha bounds, không phải shader instruction.",
        "Hit ripple đang lệch vì data space. Em sẽ debug distance field trước khi sửa visual.",
    ])


def chapter_10(doc):
    h_ch(doc, "10. Lộ trình luyện thành TA thực chiến")
    body(doc, "Nếu bạn muốn lên Technical VFX Artist, hãy học theo lỗi. Mỗi tuần chọn một loại lỗi, dựng nó lên, rồi sửa. Đừng chỉ dựng effect đẹp.")
    plan = [
        ("Tuần 1: Mask và dissolve", "Dựng dissolve đẹp, sau đó cố tình làm edge răng cưa, mask bẩn, threshold sai. Sửa từng lỗi."),
        ("Tuần 2: UV và SubUV", "Dựng panner, polar, flipbook NumberGrid. Cố tình sai pivot, sai grid, sai frame. Học bằng mắt."),
        ("Tuần 3: Particle contract", "Particle Color, Dynamic Parameter, alpha fade. Cố tình bỏ alpha path và debug."),
        ("Tuần 4: Depth/sorting", "Tạo smoke cắt cube, portal nhiều layer, shield translucent. Quan sát sorting/depth lỗi."),
        ("Tuần 5: Optimization", "Tạo effect quá nặng, rồi giảm overdraw, texture fetch, spawn count. Ghi số đo."),
        ("Tuần 6: Rebuild production", "Chọn M_Flare/M_FlamesCard/M_SubUV_Utilities. Rebuild bản nhỏ, tạo low/high quality variants."),
    ]
    for title, text in plan:
        h1(doc, title)
        body(doc, text)
    box(doc, "Kết luận gai góc", [
        "Shader đẹp trong preview chưa có nghĩa dùng được.",
        "Graph thông minh chưa chắc đáng tiền nếu texture bake rẻ hơn.",
        "Best practice không thay thế được judgement.",
        "Bạn lên trình khi biết debug lỗi xấu, không chỉ khi làm demo đẹp.",
    ], fill=WARN)


def chapter_11(doc):
    h_ch(doc, "11. Field notes: toán nâng cao nhưng dùng được")
    h1(doc, "Matrices trong đầu TA")
    body(doc, "Bạn không cần mở sách đại số tuyến tính để làm VFX mỗi ngày, nhưng bạn cần hiểu ma trận là máy đổi hệ tọa độ. Khi Unreal có node TransformVector hoặc TransformPosition, nó đang giúp bạn chuyển dữ liệu từ một space sang space khác. Sai lầm không phải là không biết nhân ma trận tay. Sai lầm là không biết dữ liệu của mình đang ở space nào.")
    body(doc, "Một ví dụ: shield hit ripple. HitPosition từ gameplay thường là world position. Pixel position trong material nếu dùng Absolute World Position cũng là world. Hai thứ này nói chuyện được. Nhưng nếu bạn lấy ObjectPosition hoặc local vertex position rồi trừ HitPosition world, kết quả lệch. Graph compile, visual sai.")
    code(doc, [
        "Good:",
        "d = distance(WorldPosition, HitPositionWS)",
        "",
        "Suspicious:",
        "d = distance(LocalPosition, HitPositionWS)",
    ], "Space sanity check")
    h1(doc, "Normal không phải lúc nào cũng mượt")
    body(doc, "WPO theo VertexNormalWS trên sphere rất đẹp. Cùng graph đó trên cube có thể vỡ thành từng mảng vì normal hard edge. Đây là lỗi người mới hay tưởng do shader. Thật ra do geometry. Một shader deformation cần mesh phù hợp. Nếu vertex density thấp, WPO không thể tạo chi tiết mượt.")
    box(doc, "Câu hỏi trước khi dùng WPO", [
        "Mesh có đủ vertex không?",
        "Normal có hard edge làm deformation tách mảng không?",
        "Bounds có đủ lớn cho offset không?",
        "Effect có cần WPO thật hay chỉ cần normal/distortion fake?",
    ])
    h1(doc, "Screen UV và depth buffer")
    body(doc, "SceneDepth và SceneColor là vùng dễ gây hiểu nhầm. Bạn đang đọc thông tin đã render trên màn hình, không phải dữ liệu thuần của object. Screen UV phụ thuộc camera. Depth có thể linear hoặc non-linear tùy pipeline/node. Một phép so sánh depth sai scale sẽ tạo edge kỳ lạ.")
    body(doc, "Nếu bạn làm soft intersection thủ công, hãy debug depth difference ra grayscale. Đừng nối thẳng vào opacity rồi đoán. Nếu grayscale toàn trắng hoặc toàn đen, range của bạn sai.")
    code(doc, [
        "diff = SceneDepth - PixelDepth",
        "soft = saturate(diff / FadeDistance)",
        "Opacity = Alpha * soft",
    ], "Manual soft depth idea")
    h1(doc, "Distance field thinking")
    body(doc, "SDF không phải buzzword. Nó là cách nghĩ: thay vì vẽ một vòng tròn bằng texture, bạn tính khoảng cách tới hình. Khi có khoảng cách, bạn tạo mask bằng SmoothStep. Cách nghĩ này dùng được cho circle, ring, line, box, ripple, shockwave và nhiều pattern stylized.")
    code(doc, [
        "circle = 1 - smoothstep(Radius, Radius + Softness, length(UV - Center))",
        "ring = 1 - smoothstep(Width, Width + Softness, abs(length(UV - Center) - Radius))",
        "line = 1 - smoothstep(Width, Width + Softness, abs(UV.y - LineY))",
    ], "SDF-like masks")


def chapter_12(doc):
    h_ch(doc, "12. Anti-patterns: những graph nhìn chuyên nghiệp nhưng khó sống")
    anti = [
        ("Master material ôm mọi thứ", "Một master có flare, smoke, dissolve, portal, shield, beam trong cùng graph thường nghe hấp dẫn lúc đầu. Về sau nó thành mê cung switch, compile variants, parameter rác và không ai dám sửa. Tách theo family thường bớt hào nhoáng nhưng sống lâu hơn."),
        ("Parameter theo tên node", "Scalar_12, MultiplyPower, NoiseThing không giúp ai chỉnh visual. Parameter production nên là Radius, Width, Softness, FlowSpeed, EdgeIntensity, AlphaPower, FadeDistance."),
        ("Noise ở mọi nơi", "Noise không tự làm effect organic. Noise quá nhiều làm mất silhouette, tăng texture fetch và khiến direction yếu. Noise phải có vai trò: breakup, distortion, variation, edge erosion."),
        ("Bloom che lỗi", "Nếu tắt bloom mà effect không đọc, bloom đang trang điểm cho lỗi. Sửa mask/core/falloff trước."),
        ("DepthFade cho mọi particle", "DepthFade tốt cho soft intersection, nhưng không miễn phí và không cần cho mọi effect. Spark nhỏ bay giữa không khí không cần DepthFade."),
        ("Custom HLSL quá sớm", "Custom node đẹp trong portfolio tech, nhưng nếu bạn chưa debug bản node thường thì custom chỉ giấu lỗi. Dùng HLSL khi công thức rõ và graph node thật sự cồng kềnh."),
    ]
    for title, text in anti:
        h1(doc, title)
        body(doc, text)
        box(doc, "Cách sửa", [
            "Viết lại mục tiêu visual bằng một câu.",
            "Tách lane: UV, mask, color, opacity, interaction, cost.",
            "Xóa hoặc bake phần không cần dynamic.",
        ])


def chapter_13(doc):
    h_ch(doc, "13. Mobile/iGaming playbook")
    body(doc, "iGaming thường cần hiệu ứng sáng, rõ, chạy mượt, lặp nhiều lần và ít rủi ro. Người xem có thể nhìn các win effects hàng nghìn lần. Shader không cần chứng minh bạn biết math khó. Nó cần ổn định, nhẹ và dễ art-direct.")
    h1(doc, "Win burst")
    body(doc, "Một win burst thường có radial rays, flare, sparkle và glow. Dirty version rẻ: atlas texture có rays/flare, material additive đơn giản, ParticleColor tint/fade, một panner nhẹ cho sparkle. Không cần procedural ray generator nếu art đã cố định.")
    code(doc, [
        "Emissive = AtlasRGB * ParticleColor.rgb * Intensity * ParticleColor.a",
        "Opacity/Fade handled by emissive scale for Additive",
    ])
    h1(doc, "Coin sparkle")
    body(doc, "Sparkle nhỏ xuất hiện nhiều. Đừng dùng material đắt. Một texture star alpha, additive, random rotation/scale trong particle là đủ. Nếu cần variation, dùng Dynamic Parameter để đổi AlphaPower hoặc intensity, không thêm nhiều texture sample.")
    h1(doc, "Looping aura sau UI")
    body(doc, "UI aura thường chạy liên tục, nên cost tích lũy quan trọng. Dùng một texture tile tốt, panner chậm, alpha tight. Nếu aura nằm sau UI, không cần depth interaction. Đừng bật feature 3D chỉ vì graph mẫu có.")
    h1(doc, "Mobile dissolve")
    body(doc, "Dissolve mobile nên ưu tiên texture mask pack và ít sample. Edge emissive có thể lấy từ cùng mask, không cần thêm noise sample thứ hai nếu visual chấp nhận được.")
    code(doc, [
        "packed = Sample(MaskPack)",
        "mask = packed.r",
        "edgeSrc = packed.g",
        "visible = smoothstep(Amount, Amount + Softness, mask)",
        "edge = band(edgeSrc or mask)",
    ], "Low-fetch dissolve")
    box(doc, "Quy tắc iGaming", [
        "Nếu effect luôn giống nhau, bake nhiều hơn.",
        "Nếu effect spawn nhiều, shader phải ngu hơn.",
        "Nếu chạy sau UI, bỏ depth/lighting không cần thiết.",
        "Nếu cần đổi màu theo theme, giữ mask grayscale và tint bằng parameter.",
    ])


def chapter_14(doc):
    h_ch(doc, "14. Texture packing và import: nơi shader chết lặng")
    body(doc, "Nhiều tài liệu shader bỏ qua import settings vì nó không sexy. Nhưng production bug rất hay nằm ở đó. Một mask dissolve sRGB sai sẽ response khác. Một alpha compression xấu sẽ làm edge bẩn. Một atlas thiếu padding sẽ bleed frame.")
    h1(doc, "Packing contract")
    body(doc, "Khi pack channel, hãy viết contract ngay trong material comment hoặc texture name. Ví dụ T_FireMaskPack_RShape_GBreakup_BEdge_AAlpha. Tên dài nhưng cứu người sau. Nếu team phải mở Photoshop để đoán kênh, bạn đã tạo nợ.")
    h1(doc, "sRGB")
    body(doc, "Texture màu thường dùng sRGB vì nó là color. Mask data thường không nên dùng sRGB vì nó là số. Nếu mask đi qua gamma correction, threshold/smoothstep sẽ không phản ứng như bạn nghĩ.")
    h1(doc, "Mip và bleeding")
    body(doc, "Flipbook atlas cần padding. Khi mip giảm, pixel giữa frame có thể bleed sang frame khác. Nếu bạn thấy viền frame lạ hoặc số NumberGrid bị lẫn, đừng sửa shader trước. Kiểm padding/mip/import.")
    h1(doc, "Compression")
    body(doc, "Compression có thể phá alpha mịn. Với dissolve edge, artifact nhỏ cũng rất rõ vì threshold phóng đại lỗi. Nếu edge bẩn, hãy kiểm texture compression trước khi thêm blur/smoothstep lung tung.")
    box(doc, "Import checklist", [
        "Mask texture: sRGB off nếu dùng như data.",
        "Color texture: sRGB on nếu là màu nhìn thấy.",
        "Normal/flow: import theo loại vector data.",
        "Atlas: padding đủ, mip behavior kiểm bằng camera xa.",
        "Alpha critical: kiểm compression artifacts.",
    ])


def chapter_15(doc):
    h_ch(doc, "15. Material QA sheet trước khi giao")
    body(doc, "Đây là checklist không lịch sự nhưng thực dụng. Dùng nó trước khi gửi effect cho lead hoặc commit vào project.")
    for item in [
        "Tên material/instance/parameter có đọc được không?",
        "Có instance default đẹp không, hay mở lên là hỏng?",
        "ParticleColor.rgb và ParticleColor.a có được dùng đúng không?",
        "Dynamic Parameter kênh R/G/B/A có naming ổn định không?",
        "Texture import sRGB/compression/alpha đã kiểm chưa?",
        "Blend mode có lý do chưa, hay copy từ material khác?",
        "Additive fade có scale Emissive không?",
        "DepthFade có cần thật không?",
        "WPO có kiểm bounds chưa?",
        "SubUV đã test bằng NumberGrid chưa?",
        "Shader complexity/overdraw đã xem trong level thật chưa?",
        "Có low-quality path cho mobile hoặc spam effect không?",
        "Graph có lane rõ không, hay dây nối thành spaghetti?",
    ]:
        bullet(doc, item)
    body(doc, "Nếu bạn trả lời 'chưa' ở hơn ba dòng, material chưa nên gọi là production-ready.")


def chapter_16(doc):
    h_ch(doc, "16. Bài tập khó hơn bài đẹp")
    body(doc, "Bài tập đẹp dễ đánh lừa. Bài tập lỗi mới làm bạn lên trình. Dưới đây là các bài nên làm khi muốn chuyển từ artist biết shader sang TA biết production.")
    drills = [
        ("Alpha không fade", "Tạo material additive cố tình không dùng ParticleColor.a. Sau đó debug và sửa."),
        ("Sorting hell", "Tạo ba translucent planes giao nhau với portal/smoke. Xoay camera và ghi lỗi."),
        ("Overdraw trap", "Tạo 100 smoke sprites lớn alpha mềm. Tối ưu bằng alpha tight, size, spawn count."),
        ("Space bug", "Tạo hit ripple sai bằng local/world mismatch, rồi sửa bằng WorldPosition."),
        ("Import bug", "Tạo dissolve mask sRGB on/off và so threshold response."),
        ("Bounds bug", "WPO sphere pulse mạnh tới mức bị cull, rồi sửa bounds/amplitude."),
    ]
    for title, text in drills:
        h1(doc, title)
        body(doc, text)
    box(doc, "Cách ghi bài", [
        "Triệu chứng là gì?",
        "Giả thuyết đầu tiên là gì?",
        "Debug view nào chứng minh?",
        "Fix gì được chọn?",
        "Trade-off còn lại là gì?",
    ])
    h1(doc, "Rubric tự chấm")
    body(doc, "Mỗi bài shader nên tự chấm theo năm điểm. Không cần điểm số đẹp. Cần thành thật, vì production không quan tâm bạn dùng node khó đến đâu.")
    for item in [
        "Visual read: nhìn xa 2 giây có hiểu effect làm gì không?",
        "Control: có 5-10 parameter thật sự hữu ích hay một đống scalar rác?",
        "Robustness: đổi camera, exposure, background, scale có còn ổn không?",
        "Cost: overdraw, texture fetch, particle count có hợp bối cảnh không?",
        "Maintainability: người khác mở material có sửa được mà không chửi không?",
    ]:
        bullet(doc, item)
    body(doc, "Nếu một effect đạt visual nhưng fail maintainability, nó vẫn là nợ. Nếu nó chạy mượt nhưng không đọc visual, nó vẫn là fail. Technical VFX là đứng giữa hai áp lực đó, không được trốn bên nào.")
    box(doc, "Câu cuối", [
        "Đừng cố viết shader trông thông minh. Hãy viết shader sửa được, chạy được, và phục vụ visual thật.",
        "Khi bạn biết cố tình chọn một cách xấu vì nó đúng với budget, bạn bắt đầu có judgement.",
    ], fill=WARN)


def build():
    doc = Document()
    section = doc.sections[0]
    section.page_width = Cm(21)
    section.page_height = Cm(29.7)
    section.top_margin = Cm(1.45)
    section.bottom_margin = Cm(1.25)
    section.left_margin = Cm(1.75)
    section.right_margin = Cm(1.75)
    doc.styles["Normal"].font.name = "Arial"
    doc.styles["Normal"].font.size = Pt(10.2)
    footer = section.footer.paragraphs[0]
    footer.alignment = WD_ALIGN_PARAGRAPH.CENTER
    run = footer.add_run("Material VFX Realtime - Production Notes")
    set_font(run, size=7.8, color=MUTED)

    cover(doc)
    toc(doc)
    chapter_1(doc)
    chapter_2(doc)
    chapter_3(doc)
    chapter_4(doc)
    chapter_5(doc)
    chapter_6(doc)
    chapter_7(doc)
    chapter_8(doc)
    chapter_9(doc)
    chapter_10(doc)
    chapter_11(doc)
    chapter_12(doc)
    chapter_13(doc)
    chapter_14(doc)
    chapter_15(doc)
    chapter_16(doc)

    doc.core_properties.title = "Material VFX Realtime - Production Notes"
    doc.core_properties.subject = "Vietnamese production-focused shader VFX notes"
    doc.core_properties.author = "Codex"
    doc.save(OUT)
    print(OUT)


if __name__ == "__main__":
    build()
