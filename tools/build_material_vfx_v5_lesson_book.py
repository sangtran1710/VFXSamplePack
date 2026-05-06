from pathlib import Path

from docx import Document
from docx.enum.table import WD_CELL_VERTICAL_ALIGNMENT, WD_TABLE_ALIGNMENT
from docx.enum.section import WD_SECTION
from docx.enum.text import WD_ALIGN_PARAGRAPH
from docx.oxml import OxmlElement
from docx.oxml.ns import qn
from docx.shared import Cm, Pt, RGBColor


OUT = Path.home() / "Downloads" / "Giao_Trinh_Material_VFX_Realtime_V14_GitHubPDF.docx"
IMAGE_DIR = Path("Saved") / "material_vfx_v5_images"

BLACK = RGBColor(16, 16, 16)
SOFT_BLACK = RGBColor(38, 38, 38)
MUTED = RGBColor(92, 92, 92)
WHITE = RGBColor(246, 246, 246)
TITLE_BLUE = RGBColor(20, 61, 96)
LINE = "D7D7D7"
BOX = "F1F3F5"
WARN = "FCE8D8"
DEBUG = "E7F0FA"
MATHBOX = "EEF1F4"
CODEBOX = "E6E8EA"
TABLE_HEAD = "D9EAF7"
PAPER = "FFFFFF"
BODY_FONT = "Aptos"


def set_font(run, size=10.2, bold=False, italic=False, color=BLACK, name="Arial"):
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


def border_bottom(paragraph, color=LINE, size="6"):
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
    bottom.set(qn("w:space"), "4")
    bottom.set(qn("w:color"), color)


def keep_with_next(paragraph):
    p_pr = paragraph._p.get_or_add_pPr()
    if p_pr.find(qn("w:keepNext")) is None:
        p_pr.append(OxmlElement("w:keepNext"))


def p(doc, text="", size=10.15, bold=False, italic=False, color=BLACK, before=0, after=3.6, align=None):
    para = doc.add_paragraph()
    para.paragraph_format.space_before = Pt(before)
    para.paragraph_format.space_after = Pt(after)
    para.paragraph_format.line_spacing = 1.18
    if align is not None:
        para.alignment = align
    if text:
        run = para.add_run(text)
        set_font(run, size=size, bold=bold, italic=italic, color=color, name=BODY_FONT)
    return para


def body(doc, text, after=4.2):
    return p(doc, text, size=10.15, after=after)


def small(doc, text, after=3.0, italic=False):
    return p(doc, text, size=8.55, color=MUTED, after=after, italic=italic)


PART_SUMMARIES = {
    "Phần I. Cách nghĩ của material VFX realtime": "Mở khóa tư duy production: không học node rời rạc, mà đọc effect theo shape, mask, motion, color, interaction và cost.",
    "Phần II. Từ một giá trị 0-1 đến visual đọc được": "Biến value, mask, SmoothStep, Power, edge band và noise thành visual có kiểm soát.",
    "Phần II-B. Từ vựng shader qua bài toán VFX": "Học node như từ vựng shader: mỗi node gắn với một bài toán VFX cụ thể.",
    "Phần III. UV, space và chuyển động": "Nắm UV, panner, polar, space transform và WPO để điều khiển chuyển động và vị trí dữ liệu.",
    "Phần IV. Realtime VFX production": "Đưa material vào runtime thật: blend mode, Niagara contract, SubUV, depth, texture import và optimization.",
    "Phần V. Recipe và case study": "Dựng các recipe production: flare, dissolve, fire/smoke, portal, shield, beam và material bề mặt.",
    "Phần VI. Ghi chú nâng cao cho Technical VFX": "Các chủ đề senior hơn: color pipeline, material function, HLSL, post-process, render target, profiling và master material.",
    "Phần VII. Xưởng thực hành": "Các lab có ràng buộc production để luyện dựng, debug và tối ưu effect.",
    "Phần VIII. Failure atlas và bài tập production": "Học từ triệu chứng lỗi, lần ngược về nguyên nhân và cách xử lý.",
    "Phụ lục. Lookup nhanh khi làm material": "Trang tra cứu nhanh cho parameter, debug mode, Unreal/Unity mapping, range và feedback mơ hồ.",
    "Thuật ngữ": "Giải nghĩa ngắn các thuật ngữ shader/material/VFX thường gặp.",
    "Index chủ đề": "Bản đồ từ khóa để tra cứu theo chủ đề trong sách.",
    "Nguồn tham khảo": "Nguồn tham khảo chính và cách dùng nguồn khi học tiếp."
}


PART_IMAGES = {
    "Phần II. Từ một giá trị 0-1 đến visual đọc được": "mask_to_edge.png",
    "Phần IV. Realtime VFX production": "v11_realtime_contract_simple.png",
    "Phần VIII. Failure atlas và bài tập production": "failure_atlas.png",
}


def h_part(doc, title):
    count = getattr(h_part, "_count", 0)
    h_part._count = count + 1
    para = p(doc, title, size=15.6, bold=True, color=TITLE_BLUE, before=7, after=3.2)
    para.style = doc.styles["Heading 1"]
    for run in para.runs:
        set_font(run, size=15.6, bold=True, color=TITLE_BLUE, name="Aptos Display")
    border_bottom(para, size="8")
    keep_with_next(para)
    summary = PART_SUMMARIES.get(title)
    if summary:
        visual_intent(doc, [summary])
    img = PART_IMAGES.get(title)
    if img:
        figure(doc, img, "Sơ đồ mở chương: data từ particle/Niagara đi vào material, qua luật render, rồi thành pixel.", width_cm=13.2)


def h1(doc, title):
    para = p(doc, title, size=12.8, bold=True, color=TITLE_BLUE, before=6.2, after=3.0)
    para.style = doc.styles["Heading 2"]
    for run in para.runs:
        set_font(run, size=12.8, bold=True, color=TITLE_BLUE, name="Aptos Display")
    border_bottom(para)
    keep_with_next(para)


def blank_verso(doc):
    """Insert an intentional blank verso page before a major part opener."""
    doc.add_page_break()
    para = p(doc, "Trang này cố ý để trống.", size=8.0, color=MUTED, before=300, after=0, align=WD_ALIGN_PARAGRAPH.CENTER)
    for run in para.runs:
        set_font(run, size=8.0, color=MUTED, name=BODY_FONT)


def h2(doc, title):
    para = p(doc, title, size=10.9, bold=True, color=SOFT_BLACK, before=4.6, after=1.8)
    para.style = doc.styles["Heading 3"]
    for run in para.runs:
        set_font(run, size=10.9, bold=True, color=SOFT_BLACK, name="Aptos")
    keep_with_next(para)


def bullet(doc, text, level=0):
    para = doc.add_paragraph()
    para.paragraph_format.left_indent = Cm(0.56 + level * 0.42)
    para.paragraph_format.first_line_indent = Cm(-0.25)
    para.paragraph_format.space_after = Pt(1.8)
    para.paragraph_format.line_spacing = 1.05
    run = para.add_run("- " + text)
    set_font(run, size=9.65, name=BODY_FONT)
    return para


def callout(doc, title, lines, fill=BOX):
    color = TITLE_BLUE if fill == DEBUG else RGBColor(139, 69, 19) if fill == WARN else SOFT_BLACK
    head = p(doc, title, size=9.7, bold=True, color=color, before=3.8, after=0.8)
    head.paragraph_format.left_indent = Cm(0.22)
    head.paragraph_format.right_indent = Cm(0.22)
    shade(head, fill)
    keep_with_next(head)
    for run in head.runs:
        set_font(run, size=9.7, bold=True, color=color, name="Aptos")
    for line in lines:
        para = doc.add_paragraph()
        para.paragraph_format.left_indent = Cm(0.55)
        para.paragraph_format.first_line_indent = Cm(-0.25)
        para.paragraph_format.right_indent = Cm(0.22)
        para.paragraph_format.space_after = Pt(0.8)
        para.paragraph_format.line_spacing = 1.06
        shade(para, fill)
        run = para.add_run("- " + line)
        set_font(run, size=9.25, name=BODY_FONT)


def code(doc, lines, title=None):
    if title:
        small(doc, title, after=1.2, italic=True)
    for line in lines:
        para = p(doc, line, size=8.35, after=0.35)
        para.paragraph_format.left_indent = Cm(0.35)
        para.paragraph_format.right_indent = Cm(0.2)
        shade(para, CODEBOX)
        for run in para.runs:
            set_font(run, size=8.35, color=BLACK, name="Consolas")


def formula(doc, lines, title="Công thức"):
    if title:
        small(doc, title, after=1.1, italic=True)
    for line in lines:
        para = p(doc, line, size=10.0, after=0.8, align=WD_ALIGN_PARAGRAPH.CENTER)
        para.paragraph_format.left_indent = Cm(0.35)
        para.paragraph_format.right_indent = Cm(0.35)
        shade(para, MATHBOX)
        for run in para.runs:
            set_font(run, size=10.0, color=SOFT_BLACK, name="Cambria Math")


def visual_intent(doc, lines):
    callout(doc, "Visual Intent", lines, fill=DEBUG)


def warning_box(doc, lines):
    callout(doc, "Cảnh báo / lỗi thường gặp", lines, fill=WARN)


def set_cell_shading(cell, fill):
    tc_pr = cell._tc.get_or_add_tcPr()
    shd = tc_pr.find(qn("w:shd"))
    if shd is None:
        shd = OxmlElement("w:shd")
        tc_pr.append(shd)
    shd.set(qn("w:fill"), fill)


def set_cell_text(cell, text, size=8.65, bold=False, color=BLACK, font=BODY_FONT):
    cell.text = ""
    p0 = cell.paragraphs[0]
    p0.paragraph_format.space_after = Pt(1.0)
    p0.paragraph_format.line_spacing = 1.06
    run = p0.add_run(text)
    set_font(run, size=size, bold=bold, color=color, name=font)
    cell.vertical_alignment = WD_CELL_VERTICAL_ALIGNMENT.TOP


def set_cell_width(cell, width_cm):
    tc_pr = cell._tc.get_or_add_tcPr()
    tc_w = tc_pr.find(qn("w:tcW"))
    if tc_w is None:
        tc_w = OxmlElement("w:tcW")
        tc_pr.append(tc_w)
    tc_w.set(qn("w:type"), "dxa")
    tc_w.set(qn("w:w"), str(int(width_cm * 567)))


def enable_update_fields(doc):
    settings = doc.settings._element
    update = settings.find(qn("w:updateFields"))
    if update is None:
        update = OxmlElement("w:updateFields")
        settings.append(update)
    update.set(qn("w:val"), "true")


def enable_mirror_margins(doc):
    settings = doc.settings._element
    mirror = settings.find(qn("w:mirrorMargins"))
    if mirror is None:
        mirror = OxmlElement("w:mirrorMargins")
        settings.append(mirror)


def add_page_number_field(paragraph):
    run = paragraph.add_run()
    fld_begin = OxmlElement("w:fldChar")
    fld_begin.set(qn("w:fldCharType"), "begin")
    instr = OxmlElement("w:instrText")
    instr.set(qn("xml:space"), "preserve")
    instr.text = "PAGE"
    fld_sep = OxmlElement("w:fldChar")
    fld_sep.set(qn("w:fldCharType"), "separate")
    cached = OxmlElement("w:t")
    cached.text = "1"
    fld_end = OxmlElement("w:fldChar")
    fld_end.set(qn("w:fldCharType"), "end")
    run._r.append(fld_begin)
    run._r.append(instr)
    run._r.append(fld_sep)
    run._r.append(cached)
    run._r.append(fld_end)
    set_font(run, size=8.0, color=MUTED, name=BODY_FONT)


def add_toc_field(doc):
    para = doc.add_paragraph()
    para.paragraph_format.space_after = Pt(8)
    run = para.add_run()
    fld_begin = OxmlElement("w:fldChar")
    fld_begin.set(qn("w:fldCharType"), "begin")
    instr = OxmlElement("w:instrText")
    instr.set(qn("xml:space"), "preserve")
    instr.text = 'TOC \\o "1-3" \\h \\z \\u'
    fld_sep = OxmlElement("w:fldChar")
    fld_sep.set(qn("w:fldCharType"), "separate")
    text = OxmlElement("w:t")
    text.text = "Mục lục tự động: mở trong Word và chọn Update Table để cập nhật số trang."
    fld_end = OxmlElement("w:fldChar")
    fld_end.set(qn("w:fldCharType"), "end")
    run._r.append(fld_begin)
    run._r.append(instr)
    run._r.append(fld_sep)
    run._r.append(text)
    run._r.append(fld_end)


def figure(doc, filename, caption, width_cm=16.4):
    path = IMAGE_DIR / filename
    if not path.exists():
        return
    count = getattr(figure, "_count", 0) + 1
    figure._count = count
    para = doc.add_paragraph()
    para.paragraph_format.space_before = Pt(4)
    para.paragraph_format.space_after = Pt(1.2)
    para.alignment = WD_ALIGN_PARAGRAPH.CENTER
    run = para.add_run()
    run.add_picture(str(path), width=Cm(width_cm))
    p(doc, f"Hình {count}: {caption}", size=8.2, italic=True, color=MUTED, after=3.8, align=WD_ALIGN_PARAGRAPH.CENTER)


def cover_picture(doc, filename, width_cm=13.8):
    path = IMAGE_DIR / filename
    if not path.exists():
        return
    para = doc.add_paragraph()
    para.paragraph_format.space_before = Pt(0)
    para.paragraph_format.space_after = Pt(0)
    para.alignment = WD_ALIGN_PARAGRAPH.CENTER
    run = para.add_run()
    run.add_picture(str(path), width=Cm(width_cm))


def page_break(doc):
    doc.add_page_break()


def setup_doc():
    doc = Document()
    sec = doc.sections[0]
    sec.top_margin = Cm(1.25)
    sec.bottom_margin = Cm(1.15)
    sec.left_margin = Cm(1.45)
    sec.right_margin = Cm(1.45)
    sec.gutter = Cm(0)
    sec.header_distance = Cm(0.55)
    sec.footer_distance = Cm(0.55)

    styles = doc.styles
    styles["Normal"].font.name = BODY_FONT
    styles["Normal"]._element.rPr.rFonts.set(qn("w:eastAsia"), BODY_FONT)
    styles["Normal"].font.size = Pt(10.15)
    styles["Normal"].font.color.rgb = BLACK
    styles["Normal"].paragraph_format.space_after = Pt(3.6)
    styles["Normal"].paragraph_format.line_spacing = 1.18

    enable_update_fields(doc)
    return doc


def configure_body_section(section):
    section.top_margin = Cm(1.25)
    section.bottom_margin = Cm(1.15)
    section.left_margin = Cm(1.45)
    section.right_margin = Cm(1.45)
    section.gutter = Cm(0)
    section.header_distance = Cm(0.55)
    section.footer_distance = Cm(0.55)
    section.footer.is_linked_to_previous = False
    footer = section.footer.paragraphs[0]
    footer.text = "Giáo Trình Material VFX Realtime"
    footer.alignment = WD_ALIGN_PARAGRAPH.CENTER
    for run in footer.runs:
        set_font(run, size=8.0, color=MUTED, name=BODY_FONT)


def cover(doc):
    cover_picture(doc, "v14_github_pdf_cover.png", width_cm=15.9)
    configure_body_section(doc.add_section(WD_SECTION.NEW_PAGE))

    h1(doc, "Lời nói đầu")
    body(doc, "Cuốn sách này tồn tại vì học material VFX bằng cách hỏi từng node riêng lẻ không đủ. Trong production, vấn đề thật thường đến từ interaction giữa graph, texture import, particle data, blend mode, camera, depth, post-process và performance budget.")
    body(doc, "Cách học ở đây là đi từ visual sang dữ liệu: nhìn effect, tách intent, dựng mask, kiểm UV/space, nối particle data, debug trong scene thật, rồi mới tối ưu. Sách không thay thế việc mở Unreal hoặc Unity và làm bài tập. Nó là bản đồ để bạn biết mình đang học gì, vì sao học, và khi gặp lỗi thì lần ngược về đâu.")
    callout(doc, "Cách dùng sách", [
        "Đọc Front Matter để hiểu bản đồ.",
        "Khi học node, luôn dựng material nhỏ và solo grayscale.",
        "Khi làm production, ưu tiên Failure Atlas, Optimization, Niagara contract và Appendix.",
        "Khi cần tra nhanh, dùng Glossary và Index chủ đề ở cuối sách.",
    ], fill=DEBUG)


def toc(doc):
    h1(doc, "Mục lục nhanh")
    body(doc, "Bản PDF này dùng mục lục tĩnh để đọc trên GitHub. Khi cần học tuần tự, đi từ Phần I đến Phần IV trước. Khi đang debug hoặc làm production, nhảy thẳng tới Failure Atlas, Appendix hoặc Index chủ đề.")
    h2(doc, "Study Map")
    for item in [
        "Phần I. Cách nghĩ của material VFX realtime",
        "Phần II. Từ một giá trị 0-1 đến một visual đọc được",
        "Phần II-B. Từ vựng shader qua bài toán VFX",
        "Phần III. UV, space và chuyển động",
        "Phần IV. Realtime VFX production",
        "Phần V. Recipe và case study",
        "Phần VI. Ghi chú nâng cao cho Technical VFX",
        "Phần VII. Xưởng thực hành",
        "Phần VIII. Failure atlas và bài tập production",
        "Phụ lục. Lookup nhanh khi làm material",
        "Thuật ngữ",
        "Index chủ đề",
        "Nguồn tham khảo",
    ]:
        bullet(doc, item)


def part_i(doc):
    h_part(doc, "Phần I. Cách nghĩ của material VFX realtime")
    h1(doc, "1. Vì sao shader không nên học như danh sách node")
    body(doc, "Một người mới thường hỏi: node này dùng làm gì. Câu hỏi đúng hơn là: node này đang sửa dữ liệu nào để tạo cảm giác visual nào. Add, Multiply, Power, SmoothStep không có giá trị riêng. Chúng có giá trị khi biến một vùng xám thành edge cháy, một gradient thành mask mềm, hoặc một vector thành hướng nhìn.")
    body(doc, "The Book of Shaders mạnh ở chỗ nó không bắt đầu bằng pipeline phức tạp. Nó bắt đầu từ pixel, màu, tọa độ, hàm shaping, pattern, noise. Cách học đó hợp với shader vì shader về bản chất là biến dữ liệu thành hình ảnh. Với VFX realtime, ta giữ tinh thần đó nhưng thêm câu hỏi production: visual này chạy ở đâu, spawn bao nhiêu lần, budget bao nhiêu, ai sẽ chỉnh nó sau mình.")
    body(doc, "Vì vậy, sách này không viết kiểu 'Clamp là gì' rồi chuyển sang node khác. Ta sẽ bắt đầu từ mục tiêu: làm một vệt sáng thở, làm một vòng portal mòn cạnh, làm smoke không cắt vào mặt đất, làm flipbook đọc đúng frame. Sau đó mới kéo node vào đúng vị trí.")
    callout(doc, "Sai lầm phổ biến", [
        "Học node theo tên nhưng không biết nó nằm ở lane nào: UV, mask, color, opacity, depth hay WPO.",
        "Thấy effect chưa đẹp thì thêm noise, thêm glow, thêm color ramp. Kết quả thường là graph rối hơn, không phải visual tốt hơn.",
        "Tin material preview quá nhiều. Preview không có exposure, sorting, depth, spawn count và background thật của game.",
    ], fill=WARN)
    h2(doc, "Một material VFX là một hệ nhỏ")
    body(doc, "Texture chỉ là dữ liệu. Shader là chương trình biến dữ liệu thành pixel. Material là cấu hình shader có texture, parameter và blend mode. Material Instance là bản chỉnh nhanh cho artist. Particle renderer hoặc mesh renderer là nơi quyết định material đó được spawn, xoay, scale, fade và sắp xếp trong scene ra sao.")
    code(doc, [
        "Texture/UV/Particle data",
        "        -> material graph / shader code",
        "        -> renderer: sprite, mesh, ribbon, decal, post-process",
        "        -> frame buffer: depth, color, bloom, tonemapper",
    ], "Pipeline tối giản")
    figure(doc, "v9_shader_lane_diagram.png", "Lane diagram: tách material thành UV, mask, color, opacity/blend, interaction và debug/cost để đọc graph nhanh hơn.")
    body(doc, "Nếu effect sai, đừng mặc định graph sai. Có thể texture import sai, renderer chưa gửi Particle Color, SubUV grid lệch, exposure level khác preview, hoặc bounds của mesh bị cắt vì WPO.")

    h1(doc, "2. Sáu lớp đọc một effect")
    body(doc, "Khi lead nói 'nhìn yếu', 'chưa magical', 'giả', 'không ăn với scene', nếu bạn sửa theo cảm giác chung thì rất dễ lạc. Hãy ép feedback thành sáu lớp.")
    for item in [
        "Shape: hình lớn/silhouette. Người xem có đọc ra vòng, tia, lửa, khói, shield không?",
        "Mask: vùng nào hiện, vùng nào mất, cạnh mềm hay cứng, có breakup không?",
        "Motion: UV chạy, frame flipbook, particle movement, WPO, distortion, timing.",
        "Color: hue, value, saturation, emissive, bloom, tonemapper.",
        "Interaction: depth, collision, lighting, camera fade, scene color, gameplay data.",
        "Cost: overdraw, instruction count, texture fetch, memory, particle count, bounds.",
    ]:
        bullet(doc, item)
    body(doc, "Cách đọc này không chỉ để học. Nó giúp nhận feedback. 'Mạnh hơn' có thể là tăng emissive, tăng area, tăng speed, tăng contrast, thêm impact timing, hoặc giảm background noise. Nếu không hỏi lớp nào, bạn có thể sửa đúng kỹ thuật nhưng sai ý.")
    code(doc, [
        "Feedback: 'make it more powerful'",
        "Shape?       -> scale lớn hơn, silhouette rõ hơn",
        "Motion?      -> speed nhanh hơn, anticipation/impact rõ hơn",
        "Color?       -> value/emissive cao hơn, hue nóng hơn",
        "Mask?        -> edge sắc hơn, core đặc hơn",
        "Interaction? -> screen shake/light/decal/sound sync",
        "Cost?        -> có còn budget không?",
    ], "Dịch feedback thành parameter")

    h1(doc, "3. AE, Spine, Unreal, Unity: cùng visual, khác runtime")
    body(doc, "AE rất mạnh để vẽ, bake và thử visual nhanh. Noise, glow, twirl, turbulent displacement, color correction và keyframe đều thân thiện. Với Spine hoặc iGaming, bake texture/sequence từ AE là hướng rất thực tế vì runtime thường cần ổn định, nhẹ, ít biến số.")
    body(doc, "Game engine mạnh ở chỗ effect sống trong gameplay. Nó biết camera, depth, ánh sáng, vị trí nhân vật, tốc độ projectile, collision, team color, damage state. Một portal trong AE có thể đẹp, nhưng một portal trong Unreal có thể mở theo gameplay, ăn depth, distort scene, spawn particles từ cạnh, đổi màu theo faction và tối ưu theo platform.")
    body(doc, "Kết luận thực dụng: AE hợp để author texture, flipbook, concept motion và polish 2D. Engine material hợp để tương tác realtime, parameter hóa, tối ưu runtime và tạo hiệu ứng không thể bake hết. Technical VFX Artist giỏi không chọn phe. Họ biết khi nào bake, khi nào procedural, khi nào hybrid.")
    callout(doc, "Hybrid thường thắng", [
        "Bake noise/flipbook phức tạp bằng AE/Houdini/EmberGen, sau đó dùng material để tint, fade, distort, edge và depth.",
        "Dùng material procedural cho phần cần phản ứng realtime: dissolve amount, shield hit, portal open, beam length, team color.",
        "Dùng Niagara/Particle System cho timing, spawn, scale, color curve; material giữ vai trò đọc dữ liệu và render sạch.",
    ], fill=DEBUG)


def part_ii(doc):
    h_part(doc, "Phần II. Từ một giá trị 0-1 đến visual đọc được")
    h1(doc, "4. Value, mask và solo grayscale")
    body(doc, "Shader bắt đầu từ value. Một pixel có thể nhận một số 0-1 từ texture, UV, depth, distance, time, vertex color hoặc particle attribute. Khi bạn hiểu value đang đi từ đâu đến đâu, material graph bớt đáng sợ.")
    body(doc, "Thói quen quan trọng nhất: solo grayscale. Trước khi tô màu, trước khi glow, trước khi bloom, hãy nối mask ra Emissive/BaseColor để xem nó như ảnh đen trắng. Mask thấp là vùng biến mất hoặc yếu. Mask cao là vùng hiện hoặc mạnh. Mọi thứ ở giữa là transition.")
    figure(doc, "v9_mask_solo_dissolve.png", "Solo grayscale: tách noise source, visible mask, edge band và final color để không bị bloom/màu che lỗi logic.")
    code(doc, [
        "MainMask -> Emissive",
        "EdgeMask -> Emissive",
        "ParticleColor.a -> Emissive",
        "DepthFadeFactor -> Emissive",
        "UV checker / NumberGrid -> Emissive",
    ], "Các output debug nên thử")
    callout(doc, "Ca lỗi thật", [
        "Dissolve nhìn bẩn: không phải do màu. Noise source có contrast quá cao hoặc compression làm banding.",
        "Flare không fade trong Niagara: material quên nhân ParticleColor.a.",
        "Flipbook nhảy sai: SubUV grid hoặc frame index sai, không phải texture xấu.",
    ], fill=WARN)

    h1(doc, "5. SmoothStep, Power, Lerp: ba node tạo cảm giác")
    body(doc, "Nếu chỉ được chọn ba node để dạy cảm giác shader, tôi chọn SmoothStep, Power và Lerp. SmoothStep biến threshold cứng thành cạnh mềm có kiểm soát. Power làm mask gắt hoặc mềm theo curve. Lerp trộn hai trạng thái bằng một alpha.")
    body(doc, "Hãy nghĩ như chỉnh curve trong AE. Power lớn hơn 1 thường làm vùng sáng/mask cao co lại, tạo core gắt hơn. Power nhỏ hơn 1 làm vùng thấp nâng lên, tạo cảm giác mềm hơn. SmoothStep cho bạn hai mốc: bắt đầu chuyển và kết thúc chuyển.")
    code(doc, [
        "hardMask = step(threshold, mask)",
        "softMask = smoothstep(threshold, threshold + softness, mask)",
        "tightCore = pow(mask, exponent)",
        "mixedColor = lerp(colorA, colorB, mask)",
    ], "Từ value sang cảm giác")
    formula(doc, [
        "softMask = smoothstep(t, t + s, x)",
        "tightCore = mask^Power",
        "mix = (1 - α)A + αB",
    ], "Công thức dạng shader/math")
    figure(doc, "v9_power_shaping_flare.png", "Power node nhìn bằng biểu đồ hàm số và flare mask: power cao co core lại, power thấp mở vùng mềm ra.")
    body(doc, "Trong Unreal, SmoothStep có thể là node function hoặc tự dựng bằng clamp/remap. Trong Unity Shader Graph cũng có Smoothstep node. Trong HLSL/GLSL, smoothstep(edge0, edge1, x) là hàm nền tảng. Ý nghĩa không đổi: tạo transition có biên độ kiểm soát.")
    callout(doc, "Debug nhanh", [
        "Nếu edge quá cứng: tăng softness hoặc giảm contrast trước threshold.",
        "Nếu effect bị sữa/mờ: tăng exponent hoặc dùng SmoothStep hẹp hơn.",
        "Nếu color bị cháy: debug value trước tonemapper, giảm emissive multiplier, kiểm tra exposure.",
    ], fill=DEBUG)

    h1(doc, "6. Edge band: trái tim của dissolve, ring, shockwave")
    body(doc, "Một dissolve đẹp hiếm khi chỉ là mask biến mất. Phần người xem nhớ là edge: đường biên cháy, nhiễu, sáng, lạnh, hoặc có spark. Edge band là vùng nằm gần threshold. Tạo được edge band nghĩa là bạn kiểm soát được nơi effect đang xảy ra.")
    code(doc, [
        "visible = smoothstep(t, t + softness, noise)",
        "wide    = smoothstep(t - width, t + softness, noise)",
        "edge    = saturate(wide - visible)",
        "",
        "Opacity  = visible",
        "Emissive = baseColor * visible + edgeColor * edge * edgeIntensity",
    ], "Dissolve edge band")
    formula(doc, [
        "visible = smoothstep(t, t + s, n)",
        "edge = clamp(smoothstep(t - w, t, n) - visible, 0, 1)",
        "emissive = base · visible + edgeColor · edge · intensity",
    ], "Công thức edge band")
    figure(doc, "v9_mask_solo_dissolve.png", "Mask pipeline: noise grayscale -> visible mask -> edge band -> final emissive; luôn kiểm grayscale trước khi tint màu.")
    body(doc, "Một cách khác là dùng hai threshold gần nhau rồi trừ nhau. Với ring, bạn cũng đang làm edge band quanh một khoảng cách. Với shockwave, edge band chạy theo thời gian. Với portal, edge band nằm ở radial mask cộng noise.")
    callout(doc, "Lỗi production", [
        "EdgeWidth quá lớn: dissolve thành mảng sáng dày, mất cảm giác đang ăn qua bề mặt.",
        "Edge emissive quá cao: bloom nuốt hết shape, feedback sẽ là 'đẹp nhưng không đọc được'.",
        "Noise scale sai: cạnh bị lấm tấm nhỏ quá hoặc blob lớn quá so với scale nhân vật.",
    ], fill=WARN)

    h1(doc, "7. Noise: texture, procedural và cái bẫy 'organic giả'")
    body(doc, "Noise không tự làm effect organic. Noise chỉ là trường giá trị. Organic đến từ scale đúng, contrast đúng, motion đúng và liên hệ với vật thể. Một noise chạy ngang trên lửa có thể nhìn như TV static. Một noise dùng để bóp edge theo hướng chuyển động có thể nhìn sống hơn nhiều.")
    body(doc, "Có ba nguồn noise thường gặp: texture noise bake, procedural noise trong shader, và flipbook/noise sequence. Texture noise rẻ và dễ art direct. Procedural noise linh hoạt nhưng có thể tốn instruction. Flipbook noise có motion đẹp nhưng tốn memory và sampling nếu dùng quá tay.")
    code(doc, [
        "NoiseTexture(UV * Tiling + Time * Speed)",
        " -> contrast / remap",
        " -> threshold / smoothstep",
        " -> mask, edge, distortion strength",
    ], "Noise pipeline cơ bản")
    callout(doc, "Dirty trick hợp lý", [
        "Một texture noise 256x256 packed channel có thể thắng procedural noise dài nếu effect spawn rất nhiều.",
        "Cho noise chạy chậm hơn particle movement thường làm fire/smoke bớt trôi giả.",
        "Dùng hai noise scale khác nhau: scale lớn cho shape, scale nhỏ cho edge breakup.",
    ], fill=DEBUG)


NODE_LESSONS = [
    {
        "title": "Add và Subtract: dời điểm đọc, dời ngưỡng, dời nhịp",
        "problem": "Bạn có một mask đúng shape nhưng nó xuất hiện quá sớm, UV bắt đầu sai vị trí, hoặc dissolve cần chạy lệch theo từng particle. Đây là lúc Add/Subtract là công cụ chính, không phải node phụ.",
        "formula": ["shiftedUV = UV + Offset", "shiftedMask = Mask - Threshold", "timeLocal = Time + RandomPerParticle"],
        "use": ["UV offset cho texture pan.", "Dịch threshold dissolve.", "Tạo phase khác nhau cho pulse/sine giữa nhiều particle."],
        "pitfall": ["Cộng offset vào mask sau khi clamp có thể làm mất range.", "Offset UV quá lớn không sai vì texture wrap, nhưng khó debug nếu không dùng grid."],
        "drill": "Tạo một dissolve bằng gradient UV.x. Sau đó dùng Add để delay mỗi particle bằng random."
    },
    {
        "title": "Multiply và Divide: scale lực, scale UV, scale chi phí",
        "problem": "Hầu hết control trong material là nhân. Emissive intensity, opacity strength, tiling, distortion amount, WPO amplitude đều là cùng một ý: scale một giá trị.",
        "formula": ["uv = UV * Tiling", "emissive = Color * Mask * Intensity", "distortedUV = UV + NormalRG * DistortionStrength"],
        "use": ["Tăng giảm lực visual.", "Lặp texture.", "Weight nhiều mask trước khi cộng/trộn."],
        "pitfall": ["Multiply quá nhiều scalar không đặt tên sẽ làm graph khó tune.", "Divide bằng giá trị gần 0 dễ tạo kết quả cực đoan; nên clamp denominator nếu viết HLSL."],
        "drill": "Dùng một mask radial. Nhân vào ba intensity khác nhau: opacity, emissive, distortion. Quan sát mỗi lane thay đổi ra sao."
    },
    {
        "title": "Clamp và Saturate: giữ dữ liệu trong hàng rào",
        "problem": "Mask procedural thường vượt 0-1 sau khi cộng, nhân, trừ. Nếu đưa thẳng vào Lerp alpha hoặc opacity, visual có thể cháy, đảo hoặc tạo artifact.",
        "formula": ["safeMask = saturate(rawMask)", "safeValue = clamp(value, minValue, maxValue)"],
        "use": ["Bảo vệ alpha.", "Giữ factor Lerp hợp lệ.", "Chặn emissive mask trước khi nhân intensity."],
        "pitfall": ["Clamp quá sớm làm mất detail, đặc biệt trước Power/SmoothStep.", "Clamp không sửa nguồn dữ liệu xấu; nó chỉ giấu phần vượt range."],
        "drill": "Tạo noise * 2 - 0.5 rồi xem raw grayscale. Sau đó saturate và so sánh vùng bị mất."
    },
    {
        "title": "Lerp: đổi trạng thái bằng một mask",
        "problem": "Bạn muốn chuyển từ màu lạnh sang nóng, từ texture sạch sang cháy, từ opacity mềm sang cứng. Lerp là cách nói: alpha quyết định A hay B thắng.",
        "formula": ["result = lerp(A, B, alpha)", "alpha = 0 -> A", "alpha = 1 -> B"],
        "use": ["Color ramp đơn giản.", "Blend hai texture bằng dissolve mask.", "Tạo trạng thái before/after trong material instance."],
        "pitfall": ["Alpha không clamp có thể extrapolate ngoài A/B.", "Lerp màu trong không gian màu sai có thể cho cảm giác value kỳ lạ; test trong engine thật."],
        "drill": "Dùng cùng một mask để Lerp màu, roughness và opacity. Xem vì sao một mask có thể điều khiển nhiều lớp visual."
    },
    {
        "title": "Power: chỉnh curve thay vì chỉnh bừa contrast",
        "problem": "Một mask đúng shape nhưng cảm giác quá đều. Power giúp co vùng sáng, mở vùng tối hoặc tạo core rõ mà không cần texture mới.",
        "formula": ["tight = pow(mask, 2.5)", "soft = pow(mask, 0.5)", "rimSharp = pow(fresnel, RimPower)"],
        "use": ["Alpha tightening cho fire.", "Fresnel rim control.", "Emissive core shaping.", "Contrast mask trước threshold."],
        "pitfall": ["Power cao quá làm detail biến mất.", "Input âm vào pow có thể gây kết quả không ổn trong shader code; giữ mask 0-1 nếu cần."],
        "drill": "Solo một radial gradient rồi thử exponent 0.5, 1, 2, 5. Viết lại cảm giác từng version."
    },
    {
        "title": "OneMinus: đảo vai trò trong một giây",
        "problem": "Bạn có mask đúng nhưng vùng sống/chết bị ngược, rim cần sáng ở cạnh thay vì giữa, fade cần gần camera biến mất thay vì hiện lên.",
        "formula": ["inverted = 1 - mask", "rim = 1 - dot(N, V)"],
        "use": ["Invert alpha/mask.", "Tạo Fresnel từ dot normal-view.", "Đảo gradient bottom-up thành top-down."],
        "pitfall": ["Đảo mask sau khi đã nhân nhiều layer có thể không giống đảo source ban đầu.", "OneMinus chỉ có ý nghĩa sạch khi input nằm 0-1."],
        "drill": "Làm height fade từ dưới lên, sau đó OneMinus để đổi thành từ trên xuống."
    },
    {
        "title": "Min và Max: trộn mask không cần branch",
        "problem": "Bạn muốn hai vùng đều có thể làm effect hiện, hoặc muốn lấy phần yếu nhất giữa nhiều điều kiện. Max giống phép OR mềm; Min giống phép AND mềm trong nhiều case mask.",
        "formula": ["combined = max(maskA, maskB)", "intersection = min(maskA, maskB)"],
        "use": ["Gộp nhiều hit mask.", "Giữ vùng giao của rim và noise.", "Chặn opacity theo nhiều điều kiện."],
        "pitfall": ["Max quá nhiều mask làm effect mất hierarchy.", "Min dễ làm mask quá yếu nếu một input luôn thấp."],
        "drill": "Tạo radial mask và noise mask. Dùng Multiply, Min, Max để kết hợp, so sánh ba kết quả."
    },
    {
        "title": "Step và SmoothStep: hard cutoff và soft cutoff",
        "problem": "Dissolve, erosion, ring, shockwave đều cần biến value liên tục thành vùng rõ ràng. Step cắt cứng. SmoothStep cắt mềm.",
        "formula": ["hard = step(threshold, x)", "soft = smoothstep(threshold, threshold + width, x)"],
        "use": ["OpacityMask.", "Dissolve threshold.", "Edge feather.", "Anti-aliased procedural shape."],
        "pitfall": ["Step trên texture nén dễ răng cưa.", "SmoothStep width quá rộng làm effect bị bùn."],
        "drill": "Dùng cùng một noise và threshold. Render Step, SmoothStep width nhỏ, SmoothStep width lớn."
    },
    {
        "title": "Abs: biến khoảng cách hai phía thành một phía",
        "problem": "Bạn muốn ring quanh một radius, stripe hai bên đường giữa, hoặc wave đối xứng. Abs bỏ dấu âm và giữ độ lớn.",
        "formula": ["centerLine = abs(UV.y - 0.5)", "ring = abs(length(UV - 0.5) - Radius)"],
        "use": ["Ring extraction.", "Beam core từ center line.", "Symmetric wave/pulse."],
        "pitfall": ["Abs tạo cusp sắc ở điểm 0; cần SmoothStep nếu muốn mềm.", "Dùng Abs sai có thể mất hướng cần thiết cho motion."],
        "drill": "Tạo beam bằng abs(UV.y - 0.5). Sau đó SmoothStep để tạo core mềm."
    },
    {
        "title": "Frac, Floor, Round: lặp và lượng tử hóa",
        "problem": "Flipbook manual, stripe lặp, scanline, pulsing step đều cần wrap hoặc snap value. Frac giữ phần lẻ. Floor lấy bậc thấp hơn.",
        "formula": ["loopTime = frac(Time * Speed)", "frame = floor(loopTime * FrameCount)", "tileUV = frac(UV * Tiling)"],
        "use": ["Loop procedural.", "Manual atlas/frame index.", "Tiling không dùng sampler wrap.", "Stylized stepped animation."],
        "pitfall": ["Frac tạo seam tại điểm wrap.", "Floor làm chuyển động giật; đó là feature hoặc bug tùy visual."],
        "drill": "Tạo thanh progress dùng frac(Time). Sau đó dùng floor để biến nó thành 8 nấc."
    },
    {
        "title": "Sine và Cosine: nhịp thở, không phải random",
        "problem": "Pulse, blink, breathing glow, wave WPO cần nhịp lặp. Sine cho dao động mượt; nhưng nó quá đều nếu dùng trần trụi.",
        "formula": ["wave = sin(Time * Speed)", "pulse01 = wave * 0.5 + 0.5", "emissive = Base * lerp(Min, Max, pulse01)"],
        "use": ["Emissive pulse.", "WPO wave.", "UV distortion cyclic.", "Blink warning effect."],
        "pitfall": ["Sine quá đều nhìn máy móc.", "Quên remap -1..1 về 0..1 làm alpha âm."],
        "drill": "Tạo glow thở bằng sine. Sau đó nhân với noise chậm để bớt đều."
    },
    {
        "title": "Atan2 và Polar: mở khóa portal",
        "problem": "Khi cần xoay quanh tâm, radial stripe, vortex, portal ring, tọa độ Cartesian UV không thuận tiện. Polar tách radius và angle.",
        "formula": ["d = UV - Center", "r = length(d)", "a = atan2(d.y, d.x)", "polar = float2(a / TWO_PI, r)"],
        "use": ["Portal spin.", "Radial wipe.", "Concentric rings.", "Vortex distortion."],
        "pitfall": ["Angle có seam khi wrap.", "Atan2 có cost; nếu chỉ cần circle mask thì length đủ."],
        "drill": "Tạo texture stripe ngang rồi sample bằng polar UV để biến thành vòng xoay."
    },
    {
        "title": "Length và Distance: từ tọa độ thành hình",
        "problem": "Circle, sphere, hit ripple, proximity fade đều bắt đầu từ khoảng cách. Distance biến vị trí thành một scalar có thể threshold.",
        "formula": ["d2 = length(UV - Center)", "d3 = distance(WorldPos, HitPos)", "mask = 1 - smoothstep(Radius, Radius + Softness, d3)"],
        "use": ["Hit ripple trên shield.", "Radial mask.", "Shockwave.", "Proximity dissolve."],
        "pitfall": ["World distance không scale theo mesh nếu object scale khác nhau.", "Distance 3D có thể đắt hơn cách dùng trục đơn nếu không cần."],
        "drill": "Dùng WorldPosition và một HitPosition parameter để tạo vòng lan trên sphere."
    },
    {
        "title": "Dot và Normalize: hướng nhìn, rim, lighting giả",
        "problem": "Dot cho biết hai hướng giống nhau đến mức nào. Đây là nền của Fresnel, fake lighting, directional mask và facing ratio.",
        "formula": ["N = normalize(NormalWS)", "V = normalize(CameraPosWS - WorldPos)", "facing = saturate(dot(N, V))", "rim = pow(1 - facing, Power)"],
        "use": ["Fresnel/rim.", "Fake light direction.", "Direction-based dissolve.", "View-angle opacity."],
        "pitfall": ["Vector chưa normalize làm dot sai scale.", "Hai vector khác space cho kết quả vô nghĩa nhưng graph vẫn chạy."],
        "drill": "Làm rim bằng dot. Sau đó cố tình dùng vector sai space nếu có thể để nhìn lỗi."
    },
    {
        "title": "Cross: vector vuông góc và basis",
        "problem": "Cross ít dùng hơn Dot trong material VFX, nhưng quan trọng khi cần tạo vector vuông góc, basis tự dựng, hoặc hiểu tangent/binormal.",
        "formula": ["right = normalize(cross(up, forward))", "up2 = normalize(cross(forward, right))"],
        "use": ["Custom orientation.", "Ribbon/beam basis trong shader code.", "Hiểu tangent space và normal mapping."],
        "pitfall": ["Cross hai vector gần song song cho vector rất nhỏ.", "Không normalize lại làm basis yếu."],
        "drill": "Đọc lý thuyết thôi nếu đang dùng Material Graph. Khi sang HLSL custom, dựng basis cho beam billboard."
    },
    {
        "title": "Transform: dịch ngôn ngữ giữa các space",
        "problem": "Một vector không tự nói nó thuộc space nào. Transform là phiên dịch. Không có nó, bạn sẽ trộn world normal với tangent normal, object position với world position.",
        "formula": ["NormalWS = Transform(TangentNormal, Tangent, World)", "PosOS = Transform(WorldPos, World, Object)"],
        "use": ["Fresnel đúng space.", "Object-space noise.", "World-space height mask.", "Normal map lighting."],
        "pitfall": ["Transform position và transform vector không giống nhau; position chịu translation, vector hướng thì không.", "Scale non-uniform có thể làm normal transform phức tạp hơn."],
        "drill": "Tạo noise theo WorldPosition và ObjectPosition trên object đang di chuyển. Quan sát pattern đứng yên hay đi theo object."
    },
    {
        "title": "Texture Sample: không chỉ là lấy ảnh",
        "problem": "Texture sample là đọc dữ liệu từ memory. Nó liên quan đến UV, sampler, mip, compression, sRGB, channel và platform. Nhiều lỗi material đến từ texture, không phải node.",
        "formula": ["data = Texture.Sample(Sampler, UV)", "mask = data.r", "normal = UnpackNormal(data.rgb)"],
        "use": ["Sprite/flipbook.", "Noise/mask.", "Normal/roughness.", "Color ramp/LUT."],
        "pitfall": ["Mask texture bật sRGB làm threshold sai cảm giác.", "Compression phá alpha.", "Texture fetch nhiều hơn cần thiết."],
        "drill": "Import cùng một mask với sRGB on/off rồi dùng threshold. So sánh điểm dissolve."
    },
    {
        "title": "Append/Split/ComponentMask: quản lý channel như dữ liệu",
        "problem": "Shader không chỉ làm với màu RGB. Nó làm với vector. Split để lấy channel, Append để ghép vector. Channel packing dựa trên tư duy này.",
        "formula": ["maskR = packed.r", "uv2 = float2(data.r, data.g)", "color = float3(r, g, b)"],
        "use": ["Channel packing.", "Manual UV distortion từ RG.", "Ghép normal/detail data.", "Debug từng channel."],
        "pitfall": ["Quên channel nào chứa data gì.", "Dùng texture màu nén/sRGB cho data packed làm sai mask."],
        "drill": "Tạo một texture packed: R dissolve, G edge, B distortion, A opacity. Viết note ngay trong material."
    },
    {
        "title": "Desaturate/Luminance: lấy mask từ màu",
        "problem": "Nhiều flipbook không có alpha tốt, hoặc detail nằm trong RGB. Luminance/desaturate cho phép lấy mask từ độ sáng.",
        "formula": ["luma = dot(rgb, float3(0.299, 0.587, 0.114))", "alpha = pow(saturate(luma), AlphaPower)"],
        "use": ["Fire alpha từ RGB.", "Glow mask từ texture màu.", "Debug brightness distribution."],
        "pitfall": ["Luminance không thay thế alpha authoring tốt.", "Color grading texture có thể làm mask thay đổi không mong muốn."],
        "drill": "Với một fire texture, so sánh alpha channel và luminance RGB làm opacity."
    },
]


def part_node_vocabulary(doc):
    h_part(doc, "Phần II-B. Từ vựng shader qua bài toán VFX")
    body(doc, "Phần này là từ vựng, nhưng không viết như từ điển. Mỗi node được giải thích qua một tình huống visual. Khi học, hãy tự hỏi: node này đang sửa lane nào, và nếu bỏ nó đi thì visual hỏng kiểu gì.")
    for index, item in enumerate(NODE_LESSONS, start=8):
        h1(doc, f"{index}. {item['title']}")
        body(doc, item["problem"])
        code(doc, item["formula"], "Công thức/ý tưởng")
        h2(doc, "Dùng trong VFX")
        for use in item["use"]:
            bullet(doc, use)
        callout(doc, "Bẫy", item["pitfall"], fill=WARN)
        callout(doc, "Bài tập ngắn", [item["drill"]], fill=DEBUG)


def part_iii(doc):
    h_part(doc, "Phần III. UV, space và chuyển động")
    h1(doc, "13. UV như tờ giấy 0-1")
    body(doc, "UV là cách nói: pixel này lấy dữ liệu ở đâu trên texture. Với sprite/card, UV thường là tờ giấy 0-1. Góc trái dưới hoặc trái trên tùy convention, nhưng ý chính là một mặt phẳng được trải ra. Khi bạn pan, rotate, scale UV, bạn không di chuyển texture; bạn di chuyển tọa độ dùng để đọc texture.")
    code(doc, [
        "uv = TexCoord",
        "uvScaled = uv * Tiling",
        "uvPanned = uvScaled + Time * Speed",
        "color = Texture.Sample(uvPanned)",
    ], "Panner là cộng offset theo thời gian")
    body(doc, "NumberGrid là texture học UV tốt vì nó cho thấy hướng, scale và frame. Nếu một flipbook hoặc atlas nhìn sai, thay texture thật bằng NumberGrid trước. Đừng debug atlas bằng texture lửa quá mềm vì mắt sẽ bỏ qua lỗi frame.")
    callout(doc, "Derived 0-1 UV là gì", [
        "Khi Sprite Renderer hoặc SubUV đã chọn một ô trong atlas, UV gốc có thể đang nằm trong toàn bộ sheet.",
        "Derived 0-1 UV nghĩa là tính lại UV local của frame hiện tại về vùng 0-1.",
        "Nó hữu ích khi trong mỗi frame bạn vẫn muốn tạo radial mask, edge fade, gradient hoặc distortion local theo frame.",
    ], fill=DEBUG)

    h1(doc, "14. Panner, Rotator, Polar UV, Twirl")
    body(doc, "Panner tạo motion tuyến tính. Rotator xoay UV quanh một tâm. Polar UV đổi mặt phẳng XY thành radius và angle. Twirl làm góc phụ thuộc vào khoảng cách tới tâm. Đây là cùng họ: bóp tọa độ trước khi sample.")
    code(doc, [
        "centered = UV - 0.5",
        "radius = length(centered)",
        "angle = atan2(centered.y, centered.x)",
        "polarUV = float2(angle / TWO_PI, radius)",
    ], "Polar UV concept")
    body(doc, "Portal, vortex, black hole, magic ring thường cần polar. Nhưng polar có seam ở nơi angle wrap. Nếu seam nằm ở vùng thấy rõ, hãy xoay UV hoặc che seam bằng mask/noise. Nếu dùng texture radial bake, seam có thể nằm trong texture thay vì shader.")
    callout(doc, "Chọn motion theo visual", [
        "Fire: UV pan lên, noise pan khác tốc độ, edge flicker nhẹ.",
        "Portal: polar angle pan, radial pulse, distortion quanh tâm.",
        "Beam/ribbon: UV pan dọc theo chiều dài, noise stretch theo trục beam.",
        "Shield: world/object noise chậm, hit mask lan ra theo distance.",
    ], fill=DEBUG)

    h1(doc, "15. Space transform: UV, object, world, view, tangent")
    body(doc, "Space mismatch là nguồn bug âm thầm. UV space nằm trên texture. Object/local space đi theo object. World space nằm trong level. View/screen space phụ thuộc camera. Tangent space thường dùng cho normal map. Graph có thể compile dù bạn trộn sai space, nhưng visual sẽ lệch, trượt hoặc sáng tối vô lý.")
    figure(doc, "v9_space_vectors.png", "Vector space: NormalWS, tangent/bitangent và ViewDir phải được hiểu/transform đúng trước khi dot, cross hoặc Fresnel.")
    code(doc, [
        "Sai ý tưởng:",
        "dot(TangentNormal, ViewDirWorld)",
        "",
        "Ý đúng hơn:",
        "NormalWorld = TransformVector(TangentNormal, Tangent -> World)",
        "rim = 1 - saturate(dot(normalize(NormalWorld), normalize(ViewDirWorld)))",
    ], "Fresnel/rim cần cùng space")
    formula(doc, [
        "facing = clamp(N · V, 0, 1)",
        "rim = (1 - facing)^Power",
    ], "Công thức Fresnel/Rim")
    body(doc, "Một height dissolve dùng WorldPosition.z sẽ chạy theo độ cao trong level. Cùng dissolve đó dùng ObjectPosition hoặc local position sẽ đi theo mesh. Không có lựa chọn nào luôn đúng. Câu hỏi là visual cần bám vào object hay bám vào thế giới.")
    callout(doc, "Quy tắc chọn space", [
        "UV: pattern theo unwrap, tốt cho card/sprite và mesh unwrap sạch.",
        "Object/local: pattern đi theo vật thể, tốt cho shield, dissolve nhân vật, mesh effect.",
        "World: pattern thống nhất trong level, tốt cho snow/wet/height dissolve/global field.",
        "View/screen: tốt cho post-process, screen distortion, camera-facing effect.",
    ], fill=BOX)

    h1(doc, "16. WPO và mesh deformation")
    body(doc, "World Position Offset không đổi texture; nó đẩy vertex. Vì vậy WPO phụ thuộc mật độ vertex. Một plane ít vertex sẽ không uốn mượt dù graph đúng. Một mesh có bounds nhỏ có thể bị cắt khi WPO đẩy ra xa. Đây là lỗi production phổ biến.")
    code(doc, [
        "wave = sin(WorldPosition.x * Frequency + Time * Speed) * Amplitude",
        "offset = VertexNormalWS * wave",
        "WPO = offset",
    ], "WPO wave cơ bản")
    formula(doc, [
        "wave = sin(P.x · frequency + time · speed) · amplitude",
        "offset = N_world · wave",
    ], "Công thức WPO wave")
    body(doc, "Với VFX, WPO thường dùng cho shield ripple, energy mesh, dissolve crumble, portal surface, heat shimmer mesh. Nếu effect chỉ là sprite/card, nhiều khi UV distortion rẻ và đủ. Nếu silhouette cần thay đổi thật, WPO mới đáng.")
    callout(doc, "Check trước khi dùng WPO", [
        "Mesh có đủ vertex chưa?",
        "Bounds có cần tăng không?",
        "Collision có cần khớp visual không? WPO thường không đổi collision.",
        "Platform có chịu được vertex cost không?",
    ], fill=WARN)


def part_iv(doc):
    h_part(doc, "Phần IV. Realtime VFX production")
    h1(doc, "17. Emissive, opacity, additive, translucent, masked")
    body(doc, "Blend mode không phải lựa chọn mỹ thuật thuần túy; nó là quyết định render. Additive cộng sáng vào background, rất hợp glow, flare, magic streak, spark. Translucent cho alpha fade mềm, hợp smoke/fire card mềm nhưng dễ overdraw/sorting. Masked cắt cứng, rẻ và stable hơn nhưng edge có thể thô nếu không dither/AA.")
    code(doc, [
        "Additive:     FinalColor += Emissive * Alpha",
        "Translucent:  FinalColor = lerp(SceneColor, MaterialColor, Alpha)",
        "Masked:       Pixel sống hoặc bị discard theo OpacityMask",
    ], "Mô hình ý niệm, không phải code engine chính xác")
    body(doc, "Emissive không đồng nghĩa đẹp hơn. Trong pipeline có exposure, bloom và tonemapper. Một màu đẹp trong material preview có thể cháy trắng trong level. VFX stylized thường cần kiểm soát value rất chặt: core sáng, edge có hue, outer glow không nuốt silhouette.")
    callout(doc, "Triệu chứng", [
        "Additive biến mất trên nền sáng: vì cộng sáng không tạo contrast đủ.",
        "Translucent layer đảo thứ tự: sorting không ổn, đặc biệt với card lớn chồng nhau.",
        "Masked bị răng cưa: threshold quá cứng, alpha texture thiếu mip/AA, dither không phù hợp.",
    ], fill=WARN)

    h1(doc, "18. Particle Color, Dynamic Parameter và dữ liệu Niagara-material")
    body(doc, "Material và Niagara phải thống nhất dữ liệu truyền qua lại. Nếu Niagara Scale Color chỉnh alpha mà material không nhân ParticleColor.a vào opacity, curve đó vô nghĩa. Nếu material expose Dynamic Parameter nhưng Niagara không bind đúng channel, người chỉnh emitter sẽ tưởng hệ thống hỏng.")
    code(doc, [
        "TextureRGB * ParticleColor.rgb * EmissiveIntensity -> Emissive",
        "TextureA   * ParticleColor.a   -> OpacityBase",
        "DepthFade(OpacityBase, FadeDistance) -> Opacity",
    ], "Baseline cho sprite material")
    figure(doc, "v11_realtime_contract_simple.png", "Sơ đồ contract: Niagara gửi màu, alpha và parameter vào material; material quyết định emissive, opacity, distortion, fade và blend.")
    body(doc, "Dynamic Parameter nên dùng khi Niagara cần gửi thêm số vào material: dissolve amount, emissive power, distortion strength, frame blend, fade distance. Đừng nhồi mọi thứ vào Dynamic Parameter nếu Material Instance hoặc User Parameter đủ rõ hơn.")
    callout(doc, "Tên channel nên ổn định", [
        "R: EmissivePower hoặc DissolveAmount",
        "G: DepthFadeDistance hoặc DistortionStrength",
        "B: CameraFadeDistance hoặc EdgeWidth",
        "A: AlphaPower hoặc SecondaryControl",
    ], fill=DEBUG)

    h1(doc, "19. SubUV, flipbook, atlas grid và derived 0-1 UV")
    body(doc, "Flipbook là cách bake motion vào texture atlas. Niagara chọn frame, material sample frame. Đây là hướng mạnh cho fire/smoke/explosion vì motion phức tạp được bake sẵn. Material không nên cố 'diễn lại' motion flipbook bằng panner nếu sheet đã chứa animation.")
    code(doc, [
        "Niagara owns:",
        "- frame playback",
        "- lifetime, size, rotation, color curve",
        "- spawn rate and timing",
        "",
        "Material owns:",
        "- ParticleSubUV sample",
        "- tint/emissive/opacity shaping",
        "- depth fade, camera fade",
        "- optional distortion/detail mask",
    ], "Hợp đồng flipbook")
    body(doc, "Derived 0-1 UV giải quyết một vấn đề nhỏ nhưng quan trọng: sau khi chọn frame trong atlas, bạn vẫn muốn biết vị trí local bên trong frame. Ví dụ edge fade quanh mỗi flame card, radial mask trong từng frame, hoặc debug grid theo frame local.")
    figure(doc, "subuv_derived_uv.png", "Minh họa: SubUV chọn một frame trong atlas; derived 0-1 UV là tọa độ local bên trong frame đó để tiếp tục tạo mask hoặc gradient.")
    callout(doc, "Ca lỗi atlas", [
        "Texture 4x4 nhưng renderer set 8x8: frame bị cắt sai.",
        "Alpha nằm trong RGB luminance chứ không ở alpha channel: opacity trống hoặc kỳ lạ.",
        "Compression phá alpha mỏng: smoke bị bẩn, fire edge bị blocky.",
        "Không bật SubUV blending: animation giật nếu frame count thấp.",
    ], fill=WARN)

    h1(doc, "20. DepthFade, SceneDepth, sorting và camera fade")
    body(doc, "DepthFade làm mềm giao tuyến giữa translucent material và geometry. Nó không sửa mọi lỗi. Nếu card quá lớn, alpha quá rộng, hoặc sorting sai, DepthFade chỉ che được một phần. Nhưng với smoke/fire/splash, nó gần như bắt buộc nếu effect cắt vào mặt đất hoặc tường.")
    code(doc, [
        "opacityRaw = TextureA * ParticleColor.a",
        "opacitySoft = DepthFade(opacityRaw, FadeDistance)",
        "opacityCamera = opacitySoft * CameraDepthFade(CameraFadeDistance)",
        "Opacity = opacityCamera",
    ], "Soft particle baseline")
    body(doc, "SceneDepth và PixelDepth mở ra nhiều effect: intersection glow, water foam, X-ray, outline, post-process mask. Nhưng depth là dữ liệu camera dependent. Khi camera/FOV/resolution thay đổi, hãy test lại. Đừng chỉ nhìn material preview.")
    callout(doc, "Sorting reality", [
        "Translucent thường không ghi depth như opaque, nên nhiều layer có thể đảo thứ tự.",
        "Particle sort mode, bounds, pivot và distance camera đều ảnh hưởng.",
        "Nếu cần ổn định tuyệt đối trên mobile/iGaming, đôi khi masked/dithered hoặc baked sprite stack tốt hơn translucent phức tạp.",
    ], fill=WARN)

    h1(doc, "21. Channel packing, import setting và texture authoring")
    body(doc, "Optimization lớn nhất nhiều khi không nằm trong graph mà nằm ở texture. Bốn mask grayscale có thể pack vào RGBA của một texture. Một sample thay vì bốn sample. Với iGaming/mobile, đây là khác biệt thật.")
    code(doc, [
        "R = dissolve noise",
        "G = edge breakup",
        "B = distortion mask",
        "A = opacity / soft mask",
    ], "Ví dụ channel packing")
    body(doc, "Nhưng channel packing chỉ tốt nếu import đúng. Mask data thường không nên sRGB. Normal map cần compression đúng. Alpha mỏng cần format phù hợp. Mip có thể làm mask xa bị bẩn. Texture đẹp trong Photoshop chưa chắc đúng trong engine.")
    callout(doc, "Checklist import", [
        "Mask texture: kiểm tra sRGB off nếu dùng như data.",
        "Normal: import type/compression đúng normal.",
        "Flipbook: kiểm tra alpha, mip, compression, grid.",
        "UI/iGaming: cân nhắc atlas, resolution, memory và platform target.",
    ], fill=DEBUG)

    h1(doc, "22. Optimization cho mobile/iGaming")
    body(doc, "Mobile/iGaming không tha thứ cho graph khoe kỹ thuật. Người chơi có thể nhìn effect hàng trăm lần trong một session, trên thiết bị yếu, với UI dày và animation liên tục. Mục tiêu không phải shader thông minh nhất. Mục tiêu là effect ổn định, rõ, nhẹ, dễ tune.")
    body(doc, "Ba kẻ thù chính: overdraw, texture bandwidth và quá nhiều particle. Translucent full-screen glow đẹp trong editor có thể làm GPU đỏ. Một flipbook 2048 với alpha rộng có thể tốn hơn nhiều so với bạn tưởng. Một material nhiều noise procedural có thể không đáng nếu chỉ xuất hiện nhỏ.")
    callout(doc, "Thứ tự tối ưu thực dụng", [
        "Giảm diện tích translucent trước khi giảm vài instruction nhỏ.",
        "Cắt alpha texture sát silhouette, tránh card rỗng quá lớn.",
        "Pack masks, giảm texture fetch, dùng resolution đúng khoảng nhìn.",
        "Bật/tắt feature bằng Static Switch cho variant platform nếu cần.",
        "Bake motion phức tạp thành flipbook khi procedural runtime quá đắt.",
        "Đo bằng shader complexity, profiler, overdraw view, không đo bằng cảm giác.",
    ], fill=WARN)
    code(doc, [
        "Câu hỏi trước khi thêm node:",
        "Effect này spawn bao nhiêu lần?",
        "Nó chiếm bao nhiêu pixel trên màn hình?",
        "Nó có cần chạy trên mobile/web không?",
        "Texture fetch mới có thay được bằng channel pack hoặc bake không?",
    ], "Budget thinking")
    h2(doc, "Ba chiến lược giảm cost mà vẫn giữ visual")
    body(doc, "Chiến lược một: giữ silhouette, bỏ detail phụ. Người chơi thường đọc shape lớn trước. Nếu phải giảm, hãy giữ core shape, timing và màu chính; bỏ layer noise nhỏ, distortion phụ hoặc sparkle thứ cấp.")
    body(doc, "Chiến lược hai: chuyển runtime sang bake. Nếu motion không cần phản ứng gameplay, bake flipbook, texture strip hoặc atlas. Runtime chỉ tint, fade, scale và chọn frame. Đây là hướng thực tế cho UI/iGaming và effect lặp nhiều.")
    body(doc, "Chiến lược ba: chia variant theo platform. PC/console có thể dùng distortion, depth, WPO, nhiều sample. Mobile/web có thể dùng packed texture, ít translucent layer, ít particle và feature switch. Đừng bắt một material gánh mọi platform nếu nó làm cả team khó đo.")
    callout(doc, "Rule of thumb", [
        "Nếu effect rất nhỏ trên màn hình: ưu tiên silhouette và timing, bỏ detail procedural.",
        "Nếu effect phủ nhiều pixel: ưu tiên giảm translucent area và overdraw.",
        "Nếu effect lặp hàng trăm lần: ưu tiên atlas/packing/reuse, giảm material permutation.",
        "Nếu effect là hero moment: được phép đắt hơn, nhưng vẫn cần profile trong scene thật.",
    ], fill=DEBUG)
    figure(doc, "overdraw_optimization.png", "Minh họa: vùng translucent rộng và nhiều lớp chồng nhau làm overdraw tăng mạnh; crop card sát silhouette và giảm overlap thường hiệu quả hơn xóa vài node nhỏ.")
    figure(doc, "v9_shader_complexity_overdraw.png", "Optimization view synthetic: shader complexity/overdraw cần được nhìn bằng area và layer count, không chỉ bằng instruction count.")


def recipe_block(doc, title, goal, graph, params, failures, debug, optimize, exercise):
    h1(doc, title)
    visual_intent(doc, [goal])
    h2(doc, "Technical Implementation")
    code(doc, graph, "Logic node / graph sketch")
    h2(doc, "Parameter nên expose")
    for item in params:
        bullet(doc, item)
    callout(doc, "Lỗi thường gặp", failures, fill=WARN)
    callout(doc, "Debug", debug, fill=DEBUG)
    callout(doc, "Tối ưu", optimize, fill=BOX)
    callout(doc, "Bài tập", exercise, fill=DEBUG)


def part_v(doc):
    h_part(doc, "Phần V. Recipe và case study")
    figure(doc, "v11_imagegen_vfx_atlas.png", "Imagegen concept atlas: flare/mask, dissolve, portal/polar UV và shield/fresnel. Đây là hình minh họa để học concept, không phải screenshot engine.")
    recipe_block(
        doc,
        "23. Additive flare/glow",
        "Flare tốt không chỉ là texture sáng. Nó cần silhouette đọc nhanh, center/core rõ, falloff mềm, và không biến mất hoàn toàn trên nền sáng. Additive hợp cho flare vì nó cộng sáng, nhưng chính vì vậy nó yếu trên background đã sáng.",
        [
            "FlareTextureRGB * ParticleColor.rgb -> tint",
            "FlareTextureA or Luma -> pow(mask, Contrast)",
            "tint * mask * Intensity -> Emissive",
            "CameraFade/DistanceFade optional -> Emissive multiplier",
        ],
        ["Intensity", "Color/Tint", "Contrast/Power", "Softness", "CameraFadeDistance", "Size from Niagara"],
        ["Cháy trắng trong level do exposure/bloom.", "Mất trên background sáng vì additive thiếu contrast.", "Texture flare có quá nhiều empty transparent area gây overdraw."],
        ["Solo mask grayscale.", "Test trên nền tối, nền sáng, và scene thật.", "Tắt bloom tạm thời để xem shape thật."],
        ["Cắt texture sát hơn.", "Dùng texture resolution vừa đủ.", "Nếu chỉ là UI/iGaming, cân nhắc bake glow vào sprite."],
        ["Tạo flare một texture, một ParticleColor, một Power. Làm ba version: soft, sharp, overbright. Ghi lại khác biệt ở scene sáng."]
    )
    recipe_block(
        doc,
        "24. Dissolve/reveal/erosion",
        "Dissolve là bài học về threshold. Một trường noise hoặc gradient được so với Amount. Amount tăng thì vùng sống/chết thay đổi. Visual production nằm ở edge band, hướng dissolve, và sự đồng bộ với particle/gameplay.",
        [
            "source = Noise(UV * Tiling + Time * Speed)",
            "visible = smoothstep(Amount, Amount + Softness, source)",
            "edge = smoothstep(Amount - EdgeWidth, Amount, source) - visible",
            "Opacity/OpacityMask = visible",
            "Emissive += edge * EdgeColor * EdgeIntensity",
        ],
        ["DissolveAmount", "Softness", "EdgeWidth", "EdgeColor", "EdgeIntensity", "NoiseTiling", "NoisePanSpeed"],
        ["Edge quá dày nhìn như tô viền.", "Noise không theo scale mesh.", "Amount không expose cho Niagara/Blueprint nên không sync gameplay.", "Masked edge răng cưa trên mobile."],
        ["Nối source ra grayscale.", "Nối visible ra grayscale.", "Nối edge ra grayscale.", "Scrub Amount chậm từ 0 đến 1 và xem vùng nào chết trước."],
        ["Pack noise/edge vào channel.", "Dùng masked cho vật thể opaque nếu translucent không cần.", "Giảm texture fetch bằng một noise đa kênh."],
        ["Làm dissolve từ trái sang phải bằng gradient UV.x, sau đó trộn thêm noise 30%. So sánh với noise thuần."]
    )
    recipe_block(
        doc,
        "25. Fire card và smoke flipbook",
        "Fire/smoke flipbook nên để atlas giữ motion chính. Material chỉ nên shape, tint, fade và xử lý giao tuyến scene. Nếu material cố panner quá nhiều trên flipbook, motion bake dễ bị bẩn.",
        [
            "ParticleSubUV RGB * ParticleColor.rgb -> color",
            "pow(color or alpha, EmissivePower) * Intensity -> Emissive",
            "ParticleSubUV A or Luma -> pow(alpha, AlphaPower)",
            "alpha * ParticleColor.a -> opacityRaw",
            "DepthFade(opacityRaw, FadeDistance) -> Opacity",
        ],
        ["EmissivePower", "Intensity", "AlphaPower", "DepthFadeDistance", "CameraFadeDistance", "Tint", "SubUV grid in renderer"],
        ["Quên ParticleColor làm Niagara fade không có tác dụng.", "Atlas grid sai.", "Smoke dùng emissive mạnh như fire.", "Alpha channel trống nhưng material vẫn lấy alpha."],
        ["Thay atlas bằng NumberGrid.", "Kiểm tra frame playback trong Niagara.", "Solo alpha/luma.", "Tắt DepthFade để xem opacity raw."],
        ["Cắt alpha texture sát.", "Giảm spawn/card size.", "Dùng lit smoke chỉ khi budget cho phép.", "Bật SubUV blending nếu giúp giảm frame count."],
        ["Tạo cùng một atlas giả 4x4 bằng NumberGrid. Set sai grid 3x3 rồi quan sát lỗi, sau đó sửa lại."]
    )
    recipe_block(
        doc,
        "26. Portal/ring/black hole",
        "Portal thường là radial effect: distance từ tâm, angle quanh tâm, noise bóp cạnh, emissive edge, distortion hoặc scene color. Black hole thêm cảm giác hút bằng UV distortion/twirl và falloff về tâm.",
        [
            "centered = UV - Center",
            "r = length(centered)",
            "angle = atan2(centered.y, centered.x)",
            "ring = 1 - smoothstep(Width, Width + Softness, abs(r - Radius))",
            "swirlUV = UV + normalize(centered) * Distortion * ring",
            "Emissive = ring * Color * Intensity",
        ],
        ["Radius", "Width", "Softness", "SpinSpeed", "NoiseTiling", "DistortionStrength", "EdgeIntensity"],
        ["Seam polar lộ rõ.", "Ring quá đều nhìn CG.", "Distortion quá mạnh làm scene bẩn.", "Bloom nuốt hết center detail."],
        ["Solo radius gradient.", "Solo ring mask.", "Tắt noise để kiểm tra shape sạch.", "Đặt portal trước background có pattern để xem distortion."],
        ["Bake một số detail radial vào texture.", "Dùng một noise packed thay nhiều noise.", "Giảm translucent area ngoài ring."],
        ["Làm ring không texture bằng distance. Sau đó thêm một noise nhẹ vào Radius để tạo cạnh organic."]
    )
    recipe_block(
        doc,
        "27. Forcefield/shield",
        "Shield hay forcefield sống nhờ rim/fresnel, hit mask và surface breakup. Nếu chỉ dùng Fresnel xanh cộng glow, shield sẽ nhìn như sphere plastic. Cần có thông tin va chạm hoặc ít nhất là mask động để tạo cảm giác phản ứng.",
        [
            "rim = pow(1 - dot(NormalWS, ViewDirWS), RimPower)",
            "surfaceNoise = Noise(World/ObjectPosition * Tiling + Time * Speed)",
            "hitMask = 1 - smoothstep(HitRadius, HitRadius + Softness, distance(WorldPos, HitPosition))",
            "Emissive = rim * RimColor * RimIntensity + hitMask * HitColor * HitIntensity",
            "Opacity = saturate(rim * RimOpacity + surfaceNoise * SurfaceOpacity + hitMask)",
        ],
        ["RimPower", "RimIntensity", "SurfaceOpacity", "NoiseTiling", "HitPosition", "HitRadius", "HitIntensity"],
        ["Normal/ViewDir không cùng space.", "Shield chỉ hiện ở rim, không có surface body.", "Hit mask không decay theo thời gian.", "WPO ripple bị bounds cắt."],
        ["Solo Fresnel.", "Solo hit distance mask.", "Đặt camera nhiều góc.", "Test nhiều shield chồng nhau để xem sorting."],
        ["Object-space noise nếu shield di chuyển.", "Giảm translucent fill area.", "Tắt WPO trên platform thấp nếu không cần silhouette."],
        ["Tạo shield sphere chỉ bằng Fresnel. Sau đó thêm một hit mask theo vị trí world và so sánh cảm giác gameplay."]
    )
    recipe_block(
        doc,
        "28. Energy beam/ribbon/projectile",
        "Beam/ribbon là bài toán UV dọc chiều dài. Người xem cần đọc hướng, tốc độ và core. Một beam tốt thường có core sáng, edge mềm, noise kéo dài theo trục, và mask đầu/đuôi để không cắt cụt.",
        [
            "uv = RibbonUV or MeshUV",
            "core = 1 - smoothstep(CoreWidth, CoreWidth + Softness, abs(uv.y - 0.5))",
            "flow = Noise(float2(uv.x * LengthTiling - Time * Speed, uv.y * WidthTiling))",
            "opacity = core * smoothstep(HeadFade, 1, uv.x) * smoothstep(TailFade, 0, uv.x)",
            "Emissive = (core + flow * FlowAmount) * Color * Intensity",
        ],
        ["CoreWidth", "Softness", "Speed", "LengthTiling", "FlowAmount", "Intensity", "HeadFade", "TailFade"],
        ["UV ribbon không ổn định.", "Beam quá đều vì noise không stretch theo chiều dài.", "Additive beam mất trên nền sáng.", "Đầu/đuôi cắt thẳng."],
        ["Nối UV.x/UV.y ra màu để xem ribbon data.", "Solo core mask.", "Freeze time để kiểm tra shape không bị motion che."],
        ["Dùng texture strip nhỏ lặp lại.", "Hạn chế nhiều layer translucent cùng kích thước.", "Dùng mesh beam nếu ribbon sorting khó kiểm soát."],
        ["Tạo beam bằng một gradient ngang và một noise pan. Sau đó thêm head/tail fade để bỏ cảm giác bị cắt."]
    )
    recipe_block(
        doc,
        "29. Ice, lava, frosted glass, wet organic",
        "Nhóm material bề mặt này không chỉ là VFX sprite. Nó cần hiểu roughness, normal, refraction, subsurface/opacity, lighting và environment. Với Technical VFX Artist, mục tiêu đầu tiên là biết lớp nào tạo cảm giác chất liệu.",
        [
            "Ice: normal detail + fresnel + roughness variation + blue/white scatter cue",
            "Lava: crack mask + hot emissive core + dark cooled crust + slow noise motion",
            "Frosted glass: rough refraction cue + normal breakup + opacity/blur approximation",
            "Wet organic: darkened base + high specular/wetness mask + subtle normal flow",
        ],
        ["Roughness", "NormalStrength", "FresnelPower", "EmissiveCrack", "WetnessMask", "Refraction/DistortionStrength"],
        ["Chỉ đổi màu xanh rồi gọi là ice.", "Lava không có contrast giữa crust và crack.", "Glass dùng translucent quá đắt cho platform.", "Wet material không ăn với lighting scene."],
        ["Xem buffer/preview roughness-normal nếu engine hỗ trợ.", "Test dưới nhiều lighting.", "Tắt post-process để xem material gốc."],
        ["Bake masks.", "Pack roughness/wetness/crack.", "Dùng approximation thay refraction thật khi budget thấp."],
        ["Lấy một plane. Làm lava chỉ bằng crack mask: phần crack emissive, phần còn lại dark crust. Sau đó thêm noise motion rất chậm."]
    )

    h1(doc, "30. Đọc VFXSamplePack theo kiểu production")
    h2(doc, "M_Flare")
    body(doc, "Đọc M_Flare theo lane: texture lane tạo shape, Particle Color lane cho Niagara tint/fade, Power/Intensity lane tạo punch, camera fade lane tránh flare dính vào camera. Khi chỉnh, đừng bắt đầu bằng màu. Bắt đầu bằng mask và contrast.")
    callout(doc, "Bài tập M_Flare", [
        "Solo texture alpha hoặc luminance.",
        "Tắt Particle Color để thấy vì sao Niagara không nên bị bỏ qua.",
        "Tạo ba Material Instance: soft UI glow, gameplay hit flash, magical sparkle.",
    ], fill=DEBUG)
    h2(doc, "M_Mesh_Add")
    body(doc, "Mesh additive thường dùng cho effect cần silhouette theo mesh: energy shell, portal mesh, slash mesh. Hãy tìm noise mask, Z/world/object gradient, Fresnel/rim, WPO nếu có. Mesh additive dễ đẹp trong preview nhưng cháy trong level vì emissive và exposure.")
    callout(doc, "Bài tập M_Mesh_Add", [
        "Tách UV lane, mask lane, emissive lane, WPO lane.",
        "Nếu có gradient theo Z, đổi thử sang object/local và world để thấy khác biệt.",
        "Tăng WPO rồi kiểm tra bounds/culling.",
    ], fill=DEBUG)
    h2(doc, "M_FlamesCard")
    body(doc, "Flame card nên đọc theo: atlas/sample, UV speed/detail, alpha shaping, particle fade, edge fade, distortion. Lửa không chỉ sáng; nó cần nhịp. Nếu UV/detail chạy không cùng cảm giác với flipbook, flame sẽ trôi giả.")
    callout(doc, "Bài tập M_FlamesCard", [
        "Thay texture bằng NumberGrid để kiểm UV.",
        "Solo alpha trước DepthFade.",
        "Giảm emissive xuống thấp để xem shape lửa có còn đọc được không.",
    ], fill=DEBUG)
    h2(doc, "M_SubUV_Utilities")
    body(doc, "M_SubUV_Utilities là bài học về atlas và derived UV. Khi SubUV được định nghĩa ở Sprite Renderer, material cần biết frame hiện tại và đôi khi cần UV local 0-1 bên trong frame. Đây là công cụ để debug và author logic per-frame như radial fade, local gradient, frame border.")
    code(doc, [
        "Atlas UV: tọa độ trên toàn sheet",
        "Frame index: ô hiện tại do renderer/Niagara chọn",
        "Derived 0-1 UV: tọa độ local bên trong ô đó",
        "",
        "Dùng NumberGrid để thấy ngay đang đọc toàn atlas hay frame local.",
    ], "SubUV mental model")

    micro_recipes_inline(doc)

    h1(doc, "32. Lộ trình luyện tập 12 tuần")
    for item in [
        "Tuần 1-2: value, mask, clamp, saturate, lerp, power, smoothstep. Mỗi ngày làm một material grayscale.",
        "Tuần 3-4: UV motion, panner, rotator, polar, twirl. Làm portal/ring không texture.",
        "Tuần 5-6: noise, dissolve, edge band, channel packing. Làm ba kiểu reveal.",
        "Tuần 7-8: Particle Color, Dynamic Parameter, SubUV, DepthFade. Làm fire/smoke flipbook baseline.",
        "Tuần 9: space transform, Fresnel, shield hit mask, WPO.",
        "Tuần 10: optimization. Đo overdraw, instruction, texture fetch. Làm mobile/iGaming variant.",
        "Tuần 11: đọc lại VFXSamplePack, viết breakdown từng material bằng sáu lớp.",
        "Tuần 12: làm một effect hero nhỏ: portal hit hoặc projectile impact, có version PC và low-spec.",
    ]:
        bullet(doc, item)
    body(doc, "Mỗi tuần nên có một output nhỏ, không chỉ đọc. Shader học giống ngôn ngữ: biết từ chưa đủ, phải đặt câu. 'Đặt câu' trong shader là dựng graph, debug, phá nó, sửa nó, rồi giải thích lại được vì sao nó hoạt động.")


MICRO_RECIPES = [
    ("Heat distortion", "Không cần màu mạnh. Dùng normal/noise RG để offset scene hoặc UV, mask theo gradient nóng gần nguồn.", ["distort = (noiseRG * 2 - 1) * Strength", "distort *= heatMask", "SceneUV += distort"], "Quá mạnh sẽ làm background bẩn, đặc biệt sau UI."),
    ("Slash arc stylized", "Slash cần hướng, đầu đuôi mềm và core sáng. UV.x thường là chiều dài slash, UV.y là bề rộng.", ["core = 1 - smoothstep(W, W + S, abs(UV.y - 0.5))", "tail = smoothstep(0, TailSoft, UV.x) * (1 - smoothstep(1 - HeadSoft, 1, UV.x))", "Emissive = core * tail * Color * Intensity"], "Đừng để cả ribbon sáng đều; cần taper."),
    ("Impact shockwave", "Shockwave là ring theo radius chạy ra ngoài. Nó có thể là decal, mesh ring, hoặc post-process radial.", ["r = length(UV - 0.5)", "ring = 1 - smoothstep(Width, Width + Soft, abs(r - TimeRadius))"], "Nếu Width không scale theo tốc độ, ring sẽ lúc dày lúc mỏng."),
    ("Ground crack lava", "Crack mask kiểm soát vùng nóng. Crust tối giữ contrast. Emissive chỉ nằm trong khe.", ["crack = smoothstep(T, T + S, CrackMask)", "Emissive = crack * LavaColor * Heat", "Base = lerp(CrustColor, HotColor, crack)"], "Lava rẻ tiền thường cháy toàn bề mặt thay vì chỉ crack."),
    ("Electric arc", "Điện cần segment, jitter và nhịp. Texture strip hoặc spline/ribbon thường dễ kiểm soát hơn procedural thuần.", ["arcMask = TextureStrip(UV + jitter)", "pulse = pow(frac(Time * Speed), PulsePower)", "Emissive = arcMask * pulse * Color"], "Random quá mượt sẽ thành magic ribbon, không giống điện."),
    ("Magic orb", "Orb thường có core, shell, surface noise, rim và particle accent. Đừng dồn tất cả vào một material nếu Niagara làm tốt phần accent.", ["core = radialMask", "rim = fresnel", "surface = noise * shellMask", "Emissive = core + rim + surface"], "Orb dễ thành quả bóng phát sáng nếu thiếu layer scale khác nhau."),
    ("Suction core / black hole", "Cảm giác hút đến từ radial motion, twirl và vật thể/particle đi vào tâm. Material chỉ là một phần.", ["dir = normalize(UV - Center)", "twirl = angle + r * Twist", "opacity = 1 - smoothstep(Inner, Outer, r)"], "Twirl mạnh nhưng particle không bị hút sẽ thiếu thuyết phục."),
    ("Water foam edge", "Foam thường lấy từ depth intersection hoặc flow/shore mask. Shape foam cần breakup noise.", ["intersection = 1 - saturate(SceneDepth - PixelDepth)", "foam = smoothstep(T, T + S, intersection + noise * Amount)"], "Foam trắng phẳng không có breakup nhìn như viền UI."),
    ("Wet surface reveal", "Wetness thường làm base tối hơn, roughness thấp hơn, specular rõ hơn. Không chỉ là đổi màu xanh.", ["wet = mask", "BaseColor = lerp(DryBase, DryBase * Darken, wet)", "Roughness = lerp(DryRough, WetRough, wet)"], "Wet material phải test dưới lighting thật."),
    ("Ice growth", "Ice reveal cần hướng lan, edge lạnh/sáng và normal/roughness detail sau khi hiện.", ["visible = smoothstep(Amount, Amount + Soft, gradient + noise)", "edge = band(visible)", "Roughness/Normal blend theo visible"], "Chỉ tint xanh không tạo cảm giác ice."),
    ("X-ray/stencil glow", "Object được đánh dấu bằng stencil/custom depth rồi post-process hoặc material đọc mask để glow.", ["mask = StencilOrCustomDepth", "edge = Sobel(mask) or depth/normal edge", "Color = Scene + edge * Glow"], "Cần quy ước stencil để không xung đột với hệ khác."),
    ("Casino win sparkle", "iGaming sparkle cần rẻ, rõ, lặp đẹp. Thường bake sprite/sequence, runtime chỉ tint/scale/fade.", ["sprite = FlipbookFrame", "spark = sprite.a * ParticleColor.a", "Emissive = sprite.rgb * Tint * Intensity"], "Đừng dùng graph procedural nặng cho sparkle xuất hiện hàng trăm lần."),
    ("Coin shine sweep", "Một highlight sweep trên coin/UI có thể là gradient di chuyển qua mask kim loại.", ["sweep = smoothstep(Pos, Pos + Soft, UV.x + UV.y * Angle)", "band = sweep - smoothstep(Pos + Width, Pos + Width + Soft, UV.x)", "Color += band * ShineColor"], "Sweep quá đều sẽ nhìn như layer AE chưa hòa vào asset."),
    ("Projectile shell/core", "Projectile đọc tốt khi có core, shell, trail và head. Core thường additive, shell có rim/noise.", ["core = radial/mesh mask", "shell = fresnel * noise", "trail = ribbon UV flow"], "Một blob sáng không cho người chơi đọc hướng bay."),
    ("Decal scorch/burn", "Scorch là decal/mask bám world, cần fade mép và variation để không lặp.", ["mask = TextureA", "edgeFade = vertexColor or radial", "BaseColor/Roughness blend theo mask"], "Decal quá sắc cạnh sẽ lộ plane/decal bounds."),
    ("Stylized outline pulse", "Outline có thể từ inverted hull, post-process depth/normal, hoặc mesh shell. Pulse dùng sine/curve.", ["outline = edgeMask", "pulse = sin(Time * Speed) * 0.5 + 0.5", "Color = outline * lerp(ColorA, ColorB, pulse)"], "Outline post-process cần test resolution và camera stack."),
]


def micro_recipes_inline(doc):
    h1(doc, "31. Micro-recipes production")
    body(doc, "Phần này là lookup nhanh. Mỗi recipe được viết như một card ngắn: ý đồ, logic node, và lưu ý production. Cách này giữ tốc độ tra cứu nhưng tránh bảng rộng bị vỡ khi render/in giấy.")
    for idx, (name, idea, graph, warning) in enumerate(MICRO_RECIPES):
        h2(doc, name)
        callout(doc, "Visual Intent", [idea], fill=DEBUG)
        code(doc, graph, "Logic node")
        callout(doc, "Production note", [warning], fill=WARN)
    body(doc, "Khi một micro-recipe trở thành effect chính của dự án, hãy tách nó thành recipe đầy đủ: Visual Intent, Technical Implementation, parameter, debug, optimization và failure case.")


ADVANCED_NOTES = [
    {
        "title": "Color space, exposure và tonemapper",
        "body": [
            "Một trong những bước trưởng thành của VFX Artist là ngừng tin màu trong preview. Material output đi qua lighting, exposure, bloom, tonemapper, LUT và UI compositing. Với emissive VFX, chỉ cần thay exposure là cảm giác core, hue và bloom đổi rất mạnh.",
            "sRGB và linear cũng ảnh hưởng cách bạn đọc texture. Texture màu thường được decode từ sRGB sang linear để shading đúng. Texture mask/data thường không nên xử lý như màu. Nếu một dissolve threshold đổi cảm giác khi bật/tắt sRGB, đó không phải phép màu; đó là dữ liệu bị đọc theo curve khác.",
            "Trong production, hãy chốt màu trong scene thật. Nếu game có nhiều map với exposure khác nhau, material cần parameter hoặc clamp để không bị cháy ở map tối và mất ở map sáng."
        ],
        "code": ["mask texture: thường sRGB off", "albedo/color texture: thường sRGB on", "emissive debug: test trước và sau bloom/tonemapper"],
        "rules": ["Đừng dùng emissive intensity để sửa mọi vấn đề màu.", "Giữ core/edge/outer glow có hierarchy value rõ.", "Tạo preset cho nền sáng và nền tối nếu effect quan trọng."]
    },
    {
        "title": "Material Function: đóng gói ý tưởng, không đóng gói rối",
        "body": [
            "Material Function tốt là một câu shader nhỏ có ý nghĩa rõ: tạo edge band, remap 0-1, polar UV, soft particle opacity, channel unpack. Function xấu là một graph lớn bị nhét vào hộp để không ai nhìn thấy.",
            "Khi function có input/output rõ, bạn có thể tái dùng và debug. Khi function chứa quá nhiều side effect, nó làm material khó đoán hơn. Đừng tạo abstraction chỉ để graph trông sạch. Tạo abstraction khi nó giảm lỗi lặp lại.",
            "Một block dissolve edge nên nhận source mask, amount, width, softness và trả visible/edge. Đừng bắt nó tự quyết màu, emissive, opacity, Niagara binding cùng lúc."
        ],
        "code": ["MF_EdgeBand(mask, amount, width, softness)", " -> VisibleMask", " -> EdgeMask"],
        "rules": ["Function nhỏ hơn thường sống lâu hơn.", "Input đặt tên theo vai trò artist hiểu được.", "Output nên debug được bằng grayscale."]
    },
    {
        "title": "Static Switch, Dynamic Branch và variant platform",
        "body": [
            "Không phải toggle nào cũng như nhau. Static Switch tạo shader permutation, phù hợp bật/tắt feature theo material instance hoặc platform build nếu bạn chấp nhận số variant. Dynamic branch chạy runtime, có thể hữu ích nhưng không phải miễn phí.",
            "Với VFX, static switch hay dùng cho: dùng normal/distortion hay không, high-quality depth interaction, mobile low-spec, texture mode vs procedural mode. Đừng tạo quá nhiều permutation vì build time và shader compile có thể thành vấn đề.",
            "Một master material nên đủ linh hoạt cho family effect, nhưng không nên thành con quái vật phục vụ mọi thứ. Nếu fire, shield và wet surface dùng chung một master khổng lồ, có thể bạn đang tối ưu nhầm chỗ."
        ],
        "code": ["STATIC_USE_DISTORTION", "STATIC_USE_DEPTH_FADE", "STATIC_MOBILE_LOW_SPEC"],
        "rules": ["Switch feature lớn, không switch từng node nhỏ vô nghĩa.", "Đo instruction của từng variant.", "Tách master material nếu domain visual quá khác nhau."]
    },
    {
        "title": "HLSL/custom node: khi node graph không đủ sạch",
        "body": [
            "Node graph rất tốt cho authoring và team art. HLSL tốt khi bạn cần công thức gọn, loop nhỏ có kiểm soát, hàm tái dùng, hoặc thuật toán khó diễn đạt bằng node. Nhưng HLSL cũng tăng yêu cầu review, portability và debug.",
            "Đừng nhảy sang HLSL chỉ vì muốn trông senior. Một graph rõ ràng thường tốt hơn custom node mù. Nhưng nếu một polar/twist/SDF phức tạp làm graph thành spaghetti, custom function có thể làm ý tưởng sạch hơn.",
            "Ví dụ thực tế hơn: một shield nhận nhiều hit cùng lúc. Dựng bằng node graph vẫn được, nhưng nếu bạn muốn 8 hit ripple có cùng logic, decay riêng, radius riêng, intensity riêng, graph sẽ rất nhanh thành lặp dây. Một loop HLSL nhỏ có thể sạch hơn, miễn là engine/pipeline cho phép và team review được.",
            "Bitmask cũng là vùng node graph kém tiện. Nếu bạn pack nhiều feature flag vào một integer hoặc channel, HLSL bitwise giúp bật/tắt logic rõ hơn. Đây không phải thứ dùng mỗi ngày trong Material Graph, nhưng là loại công cụ phân biệt TA biết code với artist chỉ biết node.",
            "Khi viết HLSL cho material VFX, ưu tiên input/output rõ, range rõ, clamp rõ. Tránh hidden dependency vào texture/sampler nếu engine material system không quản lý tốt."
        ],
        "code": [
            "float acc = 0;",
            "for (int i = 0; i < HIT_COUNT; ++i)",
            "{",
            "    float d = distance(WorldPos, HitPos[i]);",
            "    float ring = 1 - smoothstep(Width, Width + Soft, abs(d - Radius[i]));",
            "    acc += ring * HitIntensity[i] * HitDecay[i];",
            "}",
            "float featureOn = (FeatureMask & FEATURE_RIPPLE) != 0;",
            "return saturate(acc) * featureOn;"
        ],
        "rules": ["HLSL phải dễ đọc hơn graph nó thay thế.", "Loop phải có count nhỏ/cố định nếu dùng trong material.", "Bitwise/packing chỉ dùng khi team hiểu convention.", "Có bản debug grayscale cho output chính."]
    },
    {
        "title": "Distortion và refraction: đẹp nhưng dễ bẩn",
        "body": [
            "Distortion làm người xem cảm thấy nhiệt, lực, nước, không khí bị bẻ cong. Nhưng distortion quá mạnh rất nhanh biến thành noise bẩn, nhất là khi background nhiều chi tiết hoặc UI ở phía sau.",
            "Distortion thường dùng normal map hoặc vector RG để offset scene color/UV. Strength nên phụ thuộc mask, depth hoặc falloff. Không nên distort toàn card nếu chỉ edge/center cần hiệu ứng.",
            "Refraction thật có thể đắt hoặc phụ thuộc render pipeline. Với mobile/iGaming, nhiều khi fake distortion bằng texture/mesh offset đủ dùng hơn."
        ],
        "code": ["distort = (NormalRG * 2 - 1) * DistortionStrength", "distort *= DistortionMask", "sceneUV = ScreenUV + distort"],
        "rules": ["Mask distortion chặt.", "Test trên background có line/UI.", "Có low-spec path tắt distortion."]
    },
    {
        "title": "Post-process, stencil, decal: material không chỉ nằm trên mesh",
        "body": [
            "Unity URP và Unreal đều có đường để làm fullscreen/post-process hoặc custom render feature/pass. Đây là nơi outline, x-ray, screen hit flash, color grading, depth mask và một số effect stylized sống tốt hơn so với mesh material.",
            "Stencil cho phép đánh dấu object rồi xử lý riêng. Decal cho phép dán thông tin lên world. Post-process cho phép can thiệp theo screen. Mỗi loại có chi phí và giới hạn khác nhau.",
            "Technical VFX Artist không cần viết renderer từ ngày đầu, nhưng cần biết khi nào material trên mesh không phải công cụ đúng."
        ],
        "code": ["Mesh material: effect bám object", "Decal: effect bám world/surface", "Post-process: effect bám camera/screen", "Stencil: chọn object/vùng cần xử lý"],
        "rules": ["Đừng ép mọi thứ vào translucent sprite.", "Post-process phải test resolution/camera stack.", "Stencil/decal cần quy ước team để tránh xung đột."]
    },
    {
        "title": "Render target và bake texture từ engine",
        "body": [
            "UE và Unity đều có cách render ra texture/render target. Điều này quan trọng vì đôi khi bạn muốn bake simulation, mask, hoặc preview thành texture để dùng lại. Không phải mọi effect phải tính mỗi frame trong material.",
            "UE có thể xuất texture/render target qua tool/editor workflow hoặc capture. Unity cũng có RenderTexture và pipeline tooling. Câu hỏi production là: dữ liệu có cần realtime không, hay bake một lần là đủ?",
            "Với iGaming hoặc UI VFX, bake từ AE/engine/Houdini rồi runtime chỉ tint/fade có thể rẻ và ổn định hơn nhiều."
        ],
        "code": ["Realtime procedural mỗi frame", "        vs", "Bake once -> sample texture cheaply at runtime"],
        "rules": ["Bake khi motion không cần phản ứng gameplay.", "Giữ source project để sửa lại bake.", "Đừng bake quá độ phân giải cần thiết."]
    },
    {
        "title": "Distance Fields và signed distance thinking",
        "body": [
            "Distance field là cách lưu hoặc tính khoảng cách tới bề mặt/hình. Với VFX, tư duy distance giúp tạo ring, shockwave, soft intersection, proximity effect và mask theo khoảng cách.",
            "Signed distance có dấu: trong/ngoài. Khi có distance, SmoothStep biến nó thành mask mềm. Đây là lý do SDF rất hợp với shape procedural và stylized graphics.",
            "Unreal có Mesh Distance Fields cho một số kỹ thuật scene interaction. Nhưng feature này có setting, memory và platform constraint. Đừng assume mọi project bật sẵn."
        ],
        "code": ["d = length(UV - Center) - Radius", "mask = 1 - smoothstep(0, Softness, d)", "edge = 1 - smoothstep(Width, Width + Softness, abs(d))"],
        "rules": ["Distance tốt cho shape sạch.", "Noise tốt cho breakup.", "Kết hợp cả hai thường ra visual production hơn."]
    },
    {
        "title": "Normal, roughness và fake lighting cho VFX",
        "body": [
            "Không phải VFX nào cũng unlit. Smoke lit, ice, wet, glass, energy mesh có thể cần normal/roughness để ăn scene. Nhưng lit material cũng phụ thuộc light setup và có thể kém ổn định hơn unlit stylized.",
            "Normal map không chỉ để bề mặt gồ ghề. Nó còn có thể làm distortion, fake flow, fake volume hoặc phản ứng rim. Roughness quyết định highlight rộng/hẹp, rất quan trọng cho wet/ice/glass.",
            "Khi làm stylized, nhiều khi fake lighting bằng dot với light direction parameter đủ control hơn là phụ thuộc toàn bộ scene lighting."
        ],
        "code": ["fakeLight = saturate(dot(NormalWS, LightDirWS))", "color = lerp(ShadowColor, LightColor, fakeLight)", "rim = pow(1 - dot(NormalWS, ViewDirWS), RimPower)"],
        "rules": ["Lit khi cần ăn scene.", "Unlit khi cần art direction ổn định.", "Fake lighting khi cần control và rẻ."]
    },
    {
        "title": "Profiling: đo đúng thứ đang giết frame",
        "body": [
            "Optimization không bắt đầu bằng giảm node ngẫu nhiên. Nó bắt đầu bằng đo. Với VFX, bạn cần nhìn shader complexity, overdraw, GPU profiler, texture memory, particle count, draw call và spawn timing.",
            "Instruction count quan trọng, nhưng translucent overdraw có thể quan trọng hơn. Một material 40 instruction phủ nửa màn hình có thể đắt hơn material 120 instruction nhỏ xíu. Texture bandwidth cũng có thể là nút cổ chai trên mobile.",
            "Hãy tạo thói quen ghi lại before/after. Nếu một thay đổi không đo được tốt hơn, nó chỉ là cảm giác."
        ],
        "code": ["Cost = pixels covered * shader cost * layers", "Texture cost = fetch count * resolution/format/cache behavior", "Particle cost = count * bounds/sorting/update/render"],
        "rules": ["Giảm pixel area trước khi ám ảnh vài instruction.", "Tối ưu đúng platform.", "Luôn giữ visual target khi tối ưu."]
    },
    {
        "title": "Tổ chức master material cho team",
        "body": [
            "Master material dùng trong team phải có parameter đọc được. Artist không nên mở graph chỉ để đổi edge width. Programmer không nên đoán Dynamic Parameter B đang làm gì. Lead không nên nhận 20 material trùng logic chỉ khác tên.",
            "Một master VFX tốt thường có phần base rõ: texture/sample, tint, opacity, depth fade. Feature optional nên bật theo nhóm: distortion, edge, fresnel, WPO, flipbook. Parameter nên có prefix hoặc category rõ.",
            "Nếu material được dùng trong production dài hạn, hãy thêm debug mode. Debug mode có thể output UV, mask, edge, opacity, particle alpha. Nó tiết kiệm nhiều giờ."
        ],
        "code": ["DebugMode 0 = final", "DebugMode 1 = main mask", "DebugMode 2 = edge mask", "DebugMode 3 = opacity", "DebugMode 4 = particle alpha"],
        "rules": ["Parameter tên theo vai trò.", "Group theo lane.", "Debug output là feature production, không phải xa xỉ."]
    },
]


def part_advanced_notes(doc):
    h_part(doc, "Phần VI. Ghi chú nâng cao cho Technical VFX")
    body(doc, "Phần này không bắt bạn học graphics programming đầy đủ ngay. Nó chỉ chỉ ra những vùng mà Technical VFX Artist sẽ đụng khi đi xa hơn material cơ bản. Mỗi mục là một quyết định production thường gặp.")
    for i, note in enumerate(ADVANCED_NOTES, start=33):
        h1(doc, f"{i}. {note['title']}")
        for paragraph in note["body"]:
            body(doc, paragraph)
        code(doc, note["code"], "Công thức/ghi nhớ")
        callout(doc, "Quy tắc thực dụng", note["rules"], fill=DEBUG)


LABS = [
    {
        "title": "Lab 1 - Flare đọc được trên nền sáng",
        "goal": "Tạo một additive flare vẫn đọc shape trên nền sáng và nền tối.",
        "constraints": ["Một texture hoặc procedural radial.", "Tối đa hai texture sample.", "Phải nhân ParticleColor.rgb/a.", "Có debug mode cho mask."],
        "steps": ["Dựng radial/core mask.", "Dùng Power để tạo core.", "Tạo outer glow mềm.", "Test với background trắng/xám/đen."],
        "review": ["Shape còn đọc khi bloom tắt không?", "Có cháy trắng toàn bộ không?", "Particle alpha có fade đúng không?"]
    },
    {
        "title": "Lab 2 - Dissolve có edge thật",
        "goal": "Tạo dissolve/reveal có hướng, noise và edge band, không chỉ hard cutoff.",
        "constraints": ["Một noise packed hoặc một procedural noise.", "Amount điều khiển được từ parameter.", "EdgeWidth và Softness tách riêng.", "Có version masked và translucent nếu engine cho phép."],
        "steps": ["Dùng gradient làm hướng chính.", "Trộn noise nhẹ.", "Tạo visible mask.", "Tạo edge bằng hai threshold.", "Gắn edge vào emissive."],
        "review": ["Edge có quá dày không?", "Dissolve có direction hay random?", "Amount 0/1 có sạch không?"]
    },
    {
        "title": "Lab 3 - Fire card baseline",
        "goal": "Tạo material fire flipbook tối giản nhưng production-safe.",
        "constraints": ["Dùng ParticleSubUV hoặc workflow tương đương.", "ParticleColor điều khiển tint/fade.", "DepthFade có parameter.", "Không dùng smoke emissive mạnh."],
        "steps": ["Set SubUV grid đúng.", "Sample RGB/alpha.", "Power alpha.", "Nhân ParticleColor.", "DepthFade opacity.", "Tune trong Niagara."],
        "review": ["Atlas frame đúng không?", "Fade lifetime có hoạt động không?", "Giao với sàn có mềm không?"]
    },
    {
        "title": "Lab 4 - Portal không texture",
        "goal": "Tạo portal ring bằng distance/polar/noise trước khi dùng texture đẹp.",
        "constraints": ["Không dùng texture trong pass đầu.", "Có Radius, Width, Softness.", "Có spin hoặc pulse.", "Có debug radius/ring/edge."],
        "steps": ["Tính centered UV.", "Tạo ring bằng length/abs/smoothstep.", "Thêm angle/pulse.", "Thêm noise nếu cần.", "Tune emissive."],
        "review": ["Ring có seam không?", "Không texture mà vẫn đọc portal không?", "Noise có làm shape mất kiểm soát không?"]
    },
    {
        "title": "Lab 5 - Shield có hit reaction",
        "goal": "Tạo shield sphere có rim, surface body và hit ripple theo vị trí.",
        "constraints": ["Fresnel/rim là một layer, không phải toàn bộ.", "HitPosition/HitRadius là parameter.", "Có decay/timing từ Niagara hoặc Blueprint.", "Có low-spec tắt WPO."],
        "steps": ["Tạo Fresnel.", "Tạo surface noise object/world.", "Tạo distance mask từ hit position.", "Thêm emissive hit.", "Optional WPO ripple."],
        "review": ["Shield có đọc khi nhìn chính diện không?", "Hit có đúng vị trí không?", "Bounds có bị cắt không?"]
    },
    {
        "title": "Lab 6 - Beam/ribbon có đầu đuôi mềm",
        "goal": "Tạo beam chạy dọc UV, có core, flow và head/tail fade.",
        "constraints": ["Phải debug UV.x và UV.y.", "Core không phụ thuộc texture.", "Flow texture/noise stretch theo chiều dài.", "Không cắt cụt đầu đuôi."],
        "steps": ["Visualize UV.", "Tạo core bằng abs(UV.y - 0.5).", "Tạo flow bằng noise pan UV.x.", "Tạo head/tail fade bằng SmoothStep.", "Tune additive/translucent."],
        "review": ["Beam có hướng không?", "Đầu/đuôi có mềm không?", "UV ribbon có ổn khi beam dài/ngắn?"]
    },
    {
        "title": "Lab 7 - Mobile/iGaming variant",
        "goal": "Lấy một effect đẹp và tạo bản nhẹ hơn không mất silhouette chính.",
        "constraints": ["Giảm texture fetch hoặc pack channel.", "Giảm translucent area.", "Có static switch/variant low-spec.", "Ghi before/after cost bằng tool của engine."],
        "steps": ["Chọn effect.", "Đo cost.", "Tắt feature phụ.", "Pack mask.", "Cắt alpha/card.", "Đo lại."],
        "review": ["Visual chính còn đọc không?", "Cost giảm ở đúng metric không?", "Có đổi behavior gameplay không?"]
    },
    {
        "title": "Lab 8 - Đọc lại một material thật",
        "goal": "Mở M_Flare, M_Mesh_Add, M_FlamesCard hoặc M_SubUV_Utilities và viết breakdown như tài liệu production.",
        "constraints": ["Không sửa graph ở bước đầu.", "Phải chia lane.", "Phải ghi parameter nên expose.", "Phải ghi ba lỗi có thể xảy ra."],
        "steps": ["Chụp mental map.", "Tìm UV lane.", "Tìm mask lane.", "Tìm color/opacity/depth/WPO.", "Viết cách debug.", "Đề xuất low-spec variant."],
        "review": ["Người khác đọc breakdown có sửa material được không?", "Bạn có phân biệt visual purpose và implementation không?", "Bạn có đoán được feedback sẽ sửa lane nào không?"]
    },
]


def part_labs(doc):
    h_part(doc, "Phần VII. Xưởng thực hành")
    body(doc, "Các lab dưới đây là bài tập có constraint, vì production luôn có constraint. Đừng chỉ làm bản đẹp nhất. Hãy làm bản đọc được, debug được, và có đường giảm chất lượng khi cần.")
    for i, lab in enumerate(LABS, start=1):
        h1(doc, lab["title"])
        body(doc, "Mục tiêu: " + lab["goal"])
        callout(doc, "Ràng buộc", lab["constraints"], fill=WARN)
        h2(doc, "Các bước")
        for step in lab["steps"]:
            bullet(doc, step)
        callout(doc, "Review", lab["review"], fill=DEBUG)


FAILURES = [
    {
        "name": "Flare đẹp trong preview nhưng mất trong level",
        "symptom": "Trong material preview flare sáng và mềm. Vào level có nền sáng, nó gần như biến mất hoặc chỉ còn một vùng trắng không có shape.",
        "cause": "Additive phụ thuộc background. Trên nền sáng, cộng thêm sáng không tạo contrast đủ. Exposure và bloom cũng có thể làm core cháy, mất gradient.",
        "fix": ["Test trên nền sáng/tối/scene thật.", "Tăng shape contrast thay vì chỉ tăng intensity.", "Thêm core rõ hoặc outer rim có hue khác.", "Cân nhắc translucent hoặc hybrid nếu cần đọc trên mọi nền."],
        "lesson": "Additive không có alpha như translucent. Nó là quyết định render, không phải nút 'làm đẹp'."
    },
    {
        "name": "Smoke cắt vào sàn như giấy",
        "symptom": "Smoke card mềm ở giữa nhưng giao với sàn/tường có cạnh cắt thẳng, nhìn rất game.",
        "cause": "Opacity không dùng DepthFade hoặc FadeDistance quá nhỏ. Sprite quá lớn và alpha rỗng nhiều, nên giao tuyến lộ rõ.",
        "fix": ["Nối opacity raw qua DepthFade.", "Tăng FadeDistance theo scale scene.", "Cắt alpha texture sát hơn.", "Đặt emitter tránh xuyên geometry quá sâu."],
        "lesson": "Soft particle dựa vào dữ liệu depth. Không có depth thì material không biết smoke đang giao với bề mặt nào."
    },
    {
        "name": "Niagara chỉnh alpha nhưng material không đổi",
        "symptom": "Curve Scale Color trong Niagara fade về 0 nhưng sprite vẫn hiện.",
        "cause": "Material không nhân ParticleColor.a vào opacity hoặc emissive. Hoặc renderer không gửi đúng binding.",
        "fix": ["Solo ParticleColor.a ra Emissive.", "Kiểm tra material có ParticleColor node.", "Nhân ParticleColor.rgb vào tint và ParticleColor.a vào opacity.", "Kiểm renderer material slot đúng."],
        "lesson": "Particle Color là đường điều khiển cơ bản từ Niagara sang material. Bỏ nó ra khỏi graph thì nhiều curve trong emitter mất tác dụng."
    },
    {
        "name": "Dissolve có công thức đúng nhưng nhìn rẻ",
        "symptom": "Object biến mất đúng theo Amount, nhưng visual giống wipe filter rẻ, không có lực.",
        "cause": "Chỉ có hard mask, thiếu edge band, thiếu scale noise hợp lý, thiếu secondary detail hoặc timing.",
        "fix": ["Tạo edge bằng hai threshold.", "Dùng edge color/emissive vừa đủ.", "Trộn gradient hướng với noise để dissolve có direction.", "Sync particle spawn/spark với edge nếu cần."],
        "lesson": "Dissolve không bán bằng vùng mất. Nó bán bằng cạnh đang ăn qua bề mặt."
    },
    {
        "name": "Portal có seam lộ ở một phía",
        "symptom": "Vòng portal xoay đẹp nhưng có một đường cắt dọc hoặc một vùng nhảy texture.",
        "cause": "Polar angle wrap từ -pi sang pi. Texture hoặc UV pan không che seam.",
        "fix": ["Xoay seam ra vùng khuất.", "Che seam bằng mask/noise.", "Dùng texture radial tile tốt.", "Nếu không cần angle, dùng distance ring thay polar."],
        "lesson": "Polar UV mạnh nhưng luôn có seam. Nó cần được đặt và che có chủ ý."
    },
    {
        "name": "Shield Fresnel nhìn như nhựa",
        "symptom": "Sphere có viền xanh sáng nhưng không có cảm giác energy shield hay forcefield.",
        "cause": "Chỉ có rim theo view angle, thiếu surface body, hit reaction, noise, pulse hoặc gameplay data.",
        "fix": ["Thêm surface noise opacity rất nhẹ.", "Thêm hit mask theo WorldPosition.", "Tạo ripple/WPO nhỏ khi bị đánh.", "Giữ rim là một lớp, không phải toàn bộ material."],
        "lesson": "Fresnel là accent. Shield cần phản ứng."
    },
    {
        "name": "WPO đẹp nhưng mesh bị biến mất từng góc camera",
        "symptom": "Khi vertex bị đẩy mạnh, mesh/effect bị cắt hoặc biến mất ở rìa camera.",
        "cause": "Bounds của mesh/material không bao phủ vị trí sau WPO. Renderer culling dựa trên bounds cũ.",
        "fix": ["Tăng bounds/scale bounds nếu engine cho phép.", "Giảm amplitude.", "Test ở nhiều góc camera.", "Nếu chỉ cần detail bề mặt, dùng normal/UV distortion thay WPO."],
        "lesson": "WPO đổi visual vertex, nhưng culling vẫn cần bounds đúng."
    },
    {
        "name": "Flipbook fire nhảy sai frame",
        "symptom": "Lửa nhấp nháy, frame bị cắt, hoặc thấy nhiều frame cùng lúc.",
        "cause": "SubUV grid không khớp atlas, frame count sai, hoặc material sample atlas thường thay vì ParticleSubUV.",
        "fix": ["Thay atlas bằng NumberGrid.", "Kiểm SubImageSize trong renderer.", "Kiểm SubUV Animation module.", "Kiểm material dùng ParticleSubUV nếu workflow yêu cầu."],
        "lesson": "Atlas là data layout. Layout sai thì mọi polish sau đó vô nghĩa."
    },
    {
        "name": "Material quá đẹp nhưng mobile đỏ shader complexity",
        "symptom": "PC chạy ổn, mobile/profile view báo overdraw hoặc GPU cost cao.",
        "cause": "Translucent area quá lớn, nhiều layer full-screen, texture fetch nhiều, procedural noise đắt, particle count cao.",
        "fix": ["Giảm diện tích alpha trước.", "Cắt texture sát silhouette.", "Pack mask channel.", "Bake motion nếu procedural đắt.", "Tạo low-spec variant bằng static switch."],
        "lesson": "Với translucent VFX, pixel area thường giết performance mạnh hơn vài node math nhỏ."
    },
    {
        "name": "Color đẹp trong material nhưng sai sau post-process",
        "symptom": "Trong preview màu đúng, vào level bị cháy, xám, hoặc lệch hue.",
        "cause": "Exposure, bloom, LUT, tonemapper và background thay đổi cảm nhận màu. Emissive quá cao làm hue bị kéo về trắng.",
        "fix": ["Test trong level thật.", "Giảm emissive và giữ value hierarchy.", "Dùng debug view/tắt post tạm thời.", "Đặt màu theo role: core, edge, outer glow."],
        "lesson": "Màu VFX realtime không kết thúc ở material output. Nó đi qua camera pipeline."
    },
    {
        "name": "Noise organic nhưng không art-direct được",
        "symptom": "Effect có detail phức tạp nhưng lead muốn đổi hướng/nhịp/scale thì rất khó chỉnh.",
        "cause": "Quá nhiều noise procedural trộn với nhau không có parameter rõ. Không tách shape lớn và detail nhỏ.",
        "fix": ["Tách macro shape và micro breakup.", "Expose tiling/speed/contrast theo layer.", "Dùng texture authoring nếu cần art direction chính xác.", "Viết comment/nhóm node trong graph."],
        "lesson": "Procedural mạnh khi có control. Không có control thì nó chỉ là random đắt tiền."
    },
    {
        "name": "Graph đúng nhưng người sau không sửa nổi",
        "symptom": "Effect chạy được, nhưng mỗi feedback nhỏ mất lâu vì không biết node nào điều khiển gì.",
        "cause": "Không đặt tên parameter, không group lane, mọi scalar là hằng số rải rác, logic lặp không đóng thành function.",
        "fix": ["Group theo UV/mask/color/opacity/depth/WPO.", "Đặt tên parameter theo role.", "Tạo material function cho block lặp.", "Viết debug output hoặc MI preset."],
        "lesson": "Maintainability là kỹ năng production. Shader đẹp nhưng không chỉnh được là nợ kỹ thuật."
    },
]


def part_failure_atlas(doc):
    h_part(doc, "Phần VIII. Failure atlas và bài tập production")
    h1(doc, "35. Mười hai ca hỏng material thường gặp")
    body(doc, "Phần này cố tình viết từ triệu chứng. Trong production, bạn thường không bắt đầu bằng một bài học sạch. Bạn bắt đầu bằng một effect đang hỏng và một câu feedback không rõ. Hãy luyện thói quen đi từ triệu chứng về nguyên nhân có thể đo được.")
    figure(doc, "v9_failure_depthfade_before_after.png", "Failure before-after: smoke hard intersection với mặt đất và bản sửa bằng DepthFade mềm.")
    for item in FAILURES:
        h2(doc, item["name"])
        body(doc, "Triệu chứng: " + item["symptom"])
        body(doc, "Nguyên nhân thường gặp: " + item["cause"])
        callout(doc, "Cách xử lý", item["fix"], fill=DEBUG)
        callout(doc, "Bài học", [item["lesson"]], fill=BOX)

    h1(doc, "33. Bài tập đọc graph theo lane")
    body(doc, "Khi mở một material lạ, đừng chạy theo dây nối từ trái qua phải. Hãy tìm lane. Một graph rối thường trở nên đọc được khi bạn chia nó thành các dòng dữ liệu độc lập.")
    for item in [
        "UV lane: TexCoord, Panner, Rotator, Polar, custom UV, SubUV.",
        "Texture lane: texture sample, channel mask, sRGB/data, normal unpack.",
        "Mask lane: clamp, power, smoothstep, threshold, edge band, noise contrast.",
        "Color lane: tint, particle color, color ramp, emissive multiplier.",
        "Opacity lane: alpha, particle alpha, depth fade, camera fade, opacity mask.",
        "Interaction lane: scene depth, world position, hit position, distance field, vertex color.",
        "Cost lane: texture fetch, translucent area, instruction count, switches, platform variants.",
    ]:
        bullet(doc, item)
    code(doc, [
        "Khi review material:",
        "1. Solo mask chính.",
        "2. Solo opacity cuối.",
        "3. Solo emissive trước intensity.",
        "4. Solo depth/camera fade.",
        "5. Thay texture bằng grid/noise đơn giản.",
        "6. Ghi lại parameter nào thật sự ảnh hưởng visual.",
    ], "Checklist đọc graph")
    callout(doc, "Bài tập production", [
        "Chọn M_Flare hoặc M_FlamesCard. Chụp graph bằng mắt, không cần screenshot.",
        "Viết lại thành sáu lane ở trên.",
        "Tạo ba câu feedback giả: 'mạnh hơn', 'ít giả hơn', 'nhẹ hơn cho mobile'.",
        "Với mỗi feedback, chỉ ra parameter/lane cần sửa trước.",
    ], fill=DEBUG)


def appendix_lookup(doc):
    h_part(doc, "Phụ lục. Lookup nhanh khi làm material")
    h1(doc, "Parameter naming nên rõ vai trò")
    body(doc, "Tên parameter tốt làm graph tự giải thích. Tên parameter xấu làm người sau phải mở graph và dò dây. Với material VFX, nên đặt tên theo lane và mục đích, không đặt theo cảm giác mơ hồ.")
    for item in [
        "UV_Tiling, UV_Speed, UV_Rotation, UV_DistortionStrength.",
        "Mask_Threshold, Mask_Softness, Mask_Contrast, Mask_Invert.",
        "Edge_Width, Edge_Softness, Edge_Color, Edge_Intensity.",
        "Emissive_Intensity, Emissive_Power, Emissive_Color.",
        "Opacity_Power, Opacity_FadeDistance, Camera_FadeDistance.",
        "WPO_Amplitude, WPO_Frequency, WPO_Speed.",
        "Debug_Mode, Debug_OverrideColor, Debug_ShowMask.",
    ]:
        bullet(doc, item)
    callout(doc, "Tên nên tránh", [
        "Power nếu trong graph có năm loại power khác nhau.",
        "Amount nếu không biết amount của dissolve, edge, distortion hay opacity.",
        "Color2, Color3, ParamA nếu material được dùng bởi người khác.",
    ], fill=WARN)

    h1(doc, "Debug mode lookup")
    body(doc, "Một master material có debug mode tiết kiệm thời gian hơn rất nhiều so với việc ngắt dây thủ công. Debug mode có thể là Static Switch trong bản dev hoặc parameter runtime nếu cost chấp nhận được.")
    code(doc, [
        "0 = Final",
        "1 = UV / NumberGrid",
        "2 = Main mask",
        "3 = Edge mask",
        "4 = Opacity before DepthFade",
        "5 = DepthFade factor",
        "6 = ParticleColor alpha",
        "7 = WPO amplitude / vertex mask",
    ], "Debug output đề xuất")
    body(doc, "Nếu debug output 2 đúng nhưng final sai, lỗi thường ở color/emissive/blend/post-process. Nếu debug output 2 sai, đừng sửa màu. Quay lại mask source, UV, texture import hoặc threshold.")

    h1(doc, "Unreal và Unity: dịch khái niệm, không dịch từng node")
    body(doc, "Unreal Material và Unity Shader Graph khác UI, khác naming, khác pipeline, nhưng phần lớn tư duy giống nhau. Bạn cần dịch ý tưởng: value, mask, UV, space, blend, depth, particle data.")
    for item in [
        "Unreal Particle Color tương đương ý tưởng vertex/particle color data từ renderer sang material.",
        "Niagara Dynamic Parameter tương đương custom data/material property binding tùy hệ particle.",
        "DepthFade trong Unreal tương đương soft particle/depth intersection logic trong pipeline khác.",
        "Material Function trong Unreal tương đương Sub Graph/Custom Function ở Unity Shader Graph.",
        "Custom node/HLSL trong Unreal tương đương Custom Function/HLSL include trong Unity khi pipeline cho phép.",
    ]:
        bullet(doc, item)
    callout(doc, "Cách học engine-neutral", [
        "Viết công thức trước: mask = smoothstep(t, t+s, noise).",
        "Sau đó mới tìm node tương ứng trong engine.",
        "Nếu node không có, dựng bằng Add/Subtract/Divide/Clamp hoặc custom HLSL.",
    ], fill=DEBUG)

    h1(doc, "Checklist review material trước khi giao")
    for item in [
        "Có nhân ParticleColor nếu material dùng cho particle không?",
        "Texture mask có import như data chưa?",
        "Có debug được main mask, edge mask, opacity không?",
        "Blend mode đúng mục tiêu chưa: additive, translucent, masked hay opaque?",
        "Translucent area có quá lớn không?",
        "DepthFade/CameraFade có cần không?",
        "Parameter có tên rõ và range hợp lý không?",
        "Có test trong level thật với exposure/post-process thật chưa?",
        "Có variant low-spec hoặc cách tắt feature đắt không?",
        "Người khác có thể sửa graph sau bạn không?",
    ]:
        bullet(doc, item)
    h1(doc, "Range gợi ý để bắt đầu")
    body(doc, "Các range này không phải luật. Chúng là điểm xuất phát để material không bay quá xa ngay từ lần đầu chỉnh. Sau khi visual đúng hướng, hãy tune theo scene, scale và platform.")
    for item in [
        "Emissive_Intensity: bắt đầu 1-5 cho effect thường, hero có thể cao hơn nhưng phải test exposure.",
        "Opacity_Power: 1.2-3.0 cho fire/smoke; quá cao dễ làm alpha bị cắt bẩn.",
        "Edge_Width: 0.02-0.12 trong normalized mask; lớn hơn dễ thành mảng sáng dày.",
        "DepthFadeDistance: 16-192 tùy scale scene; smoke thường cần lớn hơn spark/flare.",
        "Noise_Tiling: scale lớn cho shape, scale nhỏ cho breakup; đừng dùng một noise làm mọi việc.",
        "DistortionStrength: bắt đầu rất thấp. Nếu người xem nhận ra distortion như lỗi hình, thường là quá mạnh.",
    ]:
        bullet(doc, item)
    h1(doc, "Khi nhận feedback mơ hồ")
    body(doc, "Feedback production hiếm khi nói bằng tên node. Hãy dịch nó thành lane trước khi mở graph. Nếu không dịch, bạn sẽ sửa theo cảm giác và dễ làm effect tệ hơn.")
    for item in [
        "'Mạnh hơn' có thể là shape lớn hơn, core sáng hơn, speed nhanh hơn, impact rõ hơn hoặc sound/camera sync tốt hơn.",
        "'Organic hơn' thường là mask breakup, timing không đều, scale noise đúng, hoặc motion bớt tuyến tính.",
        "'Đắt quá' không có nghĩa xóa node ngẫu nhiên; cần biết đang đắt vì overdraw, texture, instruction hay particle count.",
        "'Không ăn scene' thường liên quan exposure, lighting, depth interaction, color palette hoặc scale.",
    ]:
        bullet(doc, item)


def glossary(doc):
    h_part(doc, "Thuật ngữ")
    body(doc, "Glossary này viết ngắn để tra nhanh. Khi gặp thuật ngữ trong graph hoặc review, hãy quay lại đây trước khi tìm công thức sâu hơn.")
    terms = [
        ("Additive", "Blend mode cộng sáng vào scene. Mạnh cho glow/flare/spark, yếu trên nền sáng nếu thiếu contrast."),
        ("Alpha", "Kênh hoặc giá trị điều khiển độ trong suốt/opacity. Với particle, thường phải nhân ParticleColor.a."),
        ("Atlas", "Texture chứa nhiều frame/tile trong một ảnh lớn. Flipbook fire/smoke thường dùng atlas."),
        ("Blend Mode", "Cách material trộn với framebuffer: opaque, masked, translucent, additive."),
        ("Bounds", "Hộp bao dùng cho culling. WPO đẩy vertex quá xa có thể bị cắt nếu bounds không đủ."),
        ("Channel Packing", "Đóng nhiều mask grayscale vào R/G/B/A của một texture để giảm texture fetch."),
        ("DepthFade", "Kỹ thuật làm mềm giao tuyến giữa translucent effect và geometry dựa trên depth buffer."),
        ("Draw Call", "Một lệnh render gửi tới GPU. Nhiều material/mesh/state khác nhau có thể tăng draw call."),
        ("Dynamic Parameter", "Dữ liệu runtime từ Niagara/particle system truyền vào material qua các channel parameter."),
        ("Emissive", "Màu tự phát sáng trong material. Đi qua exposure/bloom/tonemapper nên phải test trong scene thật."),
        ("Fresnel/Rim", "Hiệu ứng phụ thuộc góc nhìn, thường dùng dot giữa normal và view direction."),
        ("HLSL", "Shader language dùng trong nhiều pipeline DirectX/Unreal/Unity. Hữu ích khi graph lặp hoặc cần logic code."),
        ("Masked", "Blend mode cắt pixel sống/chết theo opacity mask. Ổn định hơn translucent nhưng edge cứng hơn."),
        ("Mip", "Các phiên bản texture nhỏ hơn dùng khi nhìn xa. Mip sai có thể làm mask/alpha bẩn."),
        ("Niagara", "Hệ thống VFX particle của Unreal. Niagara thường sở hữu spawn, lifetime, color, scale, SubUV playback."),
        ("Overdraw", "Một pixel bị vẽ nhiều lần, thường do nhiều translucent layer chồng nhau. Đây là cost lớn của VFX."),
        ("Particle Color", "Màu/alpha từ particle renderer truyền vào material. Là dây điều khiển tint/fade cơ bản."),
        ("Panner", "UV offset theo thời gian. Thường dùng cho fire, beam, energy flow, water detail."),
        ("Shader Instruction", "Số bước tính toán shader. Quan trọng, nhưng với translucent VFX không thay thế việc đo overdraw."),
        ("SmoothStep", "Hàm threshold mềm giữa hai ngưỡng. Cực quan trọng cho dissolve, edge, ring, falloff."),
        ("SubUV", "Cách particle renderer chọn một sub-image/frame trong atlas flipbook."),
        ("Translucent", "Blend mode alpha mềm. Hợp smoke/fire nhưng dễ sorting/overdraw."),
        ("UV", "Tọa độ dùng để đọc texture. Thay đổi UV nghĩa là thay đổi nơi sample, không phải di chuyển texture thật."),
        ("WPO", "World Position Offset, đẩy vertex trong material. Phụ thuộc vertex density và bounds."),
    ]
    for term, desc in terms:
        h2(doc, term)
        body(doc, desc)


def subject_index(doc):
    h_part(doc, "Index chủ đề")
    body(doc, "Index này là index theo chủ đề, không thay thế TOC tự động. Nó giúp lật nhanh tới nhóm kiến thức cần đọc khi đang debug hoặc làm production.")
    groups = [
        ("Debug", ["solo grayscale", "Debug Mode", "NumberGrid", "Failure Atlas", "ParticleColor.a", "DepthFade factor"]),
        ("Material math", ["SmoothStep", "Power", "Lerp", "Dot", "Distance", "Atan2", "Polar UV", "SDF"]),
        ("Niagara / Particle", ["Particle Color", "Dynamic Parameter", "SubUV", "Scale Color", "Sprite Renderer", "flipbook"]),
        ("Optimization", ["Overdraw", "Instruction Count", "Texture Fetch", "Channel Packing", "Mobile/iGaming", "low-spec variant"]),
        ("Production failures", ["sorting", "bounds/WPO", "exposure", "atlas grid", "sRGB mask", "translucent area"]),
        ("Recipes", ["flare", "dissolve", "fire card", "smoke flipbook", "portal", "forcefield", "beam/ribbon", "ice/lava/wet"]),
        ("Advanced", ["HLSL", "bitmask", "render target", "post-process", "stencil", "decal", "distance fields"]),
    ]
    for name, terms in groups:
        h2(doc, name)
        body(doc, ", ".join(terms))


def references(doc):
    h_part(doc, "Nguồn tham khảo")
    body(doc, "Các nguồn dưới đây được dùng để định hướng cấu trúc học, thuật ngữ và phạm vi kỹ thuật. Nội dung trong giáo trình này là diễn giải lại theo mục tiêu Technical VFX Artist dùng Unreal/Unity, không trích dài từ sách thương mại.")
    refs = [
        "Patricio Gonzalez Vivo và Jen Lowe, The Book of Shaders - nguồn cảm hứng chính cho cách dạy shader bằng pixel, tọa độ, shaping function, pattern và noise: https://thebookofshaders.com/",
        "Unity, Create popular shaders and visual effects with the Universal Render Pipeline, Unity 6 edition - tham khảo phạm vi URP, Shader Graph, Render Graph, water, volumetric clouds, stencil/X-ray, toon/outline, post-processing: https://unity.com/en/resources/create-shaders-visual-effects-urp-unity-6",
        "Packt, Unreal Engine 5 Shaders and Effects Cookbook, 2nd Edition - tham khảo hướng recipe Unreal Material và sample code đi kèm: https://github.com/PacktPublishing/Unreal-Engine-5-Shaders-and-Effects-Cookbook-2nd-Edition",
        "Jason Gregory, Game Engine Architecture, 4th Edition - tham khảo tư duy engine/runtime, pipeline, hệ thống và performance ở tầm rộng: https://www.routledge.com/Game-Engine-Architecture-Two-Volume-Set/Gregory/p/book/9781041162599",
        "Tomas Akenine-Moller, Eric Haines, Naty Hoffman, Real-Time Rendering, 4th Edition - tham khảo nền tảng graphics/rendering, pipeline, transforms, shading, texturing: https://www.routledge.com/Real-Time-Rendering-Fourth-Edition/Akenine-Moller-Haines-Hoffman/p/book/9780429225406",
        "Wolfgang Engel, GPU Zen 3 - tham khảo cấp senior về kỹ thuật GPU/rendering hiện đại: https://forums.developer.nvidia.com/t/gpu-zen-3-book/316564",
        "Eric Lengyel, Projective Geometric Algebra Illuminated - tham khảo nâng cao về hình học và biến đổi, không phải yêu cầu bắt buộc cho giai đoạn material VFX cơ bản: https://projectivegeometricalgebra.org/",
        "ForgePilot Knowledge trong project VFXSamplePack - các JSON về dissolve, SubUV flipbook, portal ring, forcefield, fresnel, panning UV, taxonomy node/material math.",
    ]
    for ref in refs:
        bullet(doc, ref)
    h2(doc, "Ghi chú về việc học từ nguồn")
    body(doc, "Không nên đọc tất cả cùng lúc. Với mục tiêu hiện tại, ưu tiên The Book of Shaders để rèn cách nhìn shader, Unity URP e-book để so sánh pipeline hiện đại, UE5 cookbook để lấy recipe Unreal, và ForgePilot/VFXSamplePack để luyện đọc graph thật. Real-Time Rendering và Game Engine Architecture là đường dài, đọc theo vấn đề khi cần hiểu sâu hơn.")


def build():
    h_part._count = 0
    figure._count = 0
    doc = setup_doc()
    cover(doc)
    toc(doc)
    part_i(doc)
    part_ii(doc)
    part_node_vocabulary(doc)
    part_iii(doc)
    part_iv(doc)
    part_v(doc)
    part_advanced_notes(doc)
    part_labs(doc)
    page_break(doc)
    part_failure_atlas(doc)
    appendix_lookup(doc)
    glossary(doc)
    subject_index(doc)
    references(doc)
    OUT.parent.mkdir(parents=True, exist_ok=True)
    doc.save(OUT)
    print(OUT)


if __name__ == "__main__":
    build()
