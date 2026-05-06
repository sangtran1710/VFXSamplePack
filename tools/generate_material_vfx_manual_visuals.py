from pathlib import Path
import math
import random

from PIL import Image, ImageDraw, ImageFont, ImageFilter


OUT = Path("Saved") / "material_vfx_v5_images"
OUT.mkdir(parents=True, exist_ok=True)


def font(size, bold=False):
    candidates = [
        "C:/Windows/Fonts/arialbd.ttf" if bold else "C:/Windows/Fonts/arial.ttf",
        "C:/Windows/Fonts/segoeuib.ttf" if bold else "C:/Windows/Fonts/segoeui.ttf",
    ]
    for path in candidates:
        if Path(path).exists():
            return ImageFont.truetype(path, size)
    return ImageFont.load_default()


F_TITLE = font(52, True)
F_H2 = font(34, True)
F_BODY = font(28)
F_SMALL = font(22)
F_CODE = font(24)


INK = (22, 28, 36)
MUTED = (86, 94, 105)
GRID = (220, 225, 231)
BLUE = (27, 92, 168)
ORANGE = (230, 121, 38)
RED = (207, 64, 52)
GREEN = (47, 143, 92)
CYAN = (43, 160, 190)
YELLOW = (240, 188, 55)


def save(img, name):
    img.save(OUT / name, quality=95)


def label(draw, xy, text, fill=INK, f=F_BODY, anchor=None):
    draw.text(xy, text, fill=fill, font=f, anchor=anchor)


def panel(draw, xy, title, fill=(248, 249, 251), outline=(210, 216, 224)):
    x0, y0, x1, y1 = xy
    draw.rounded_rectangle(xy, radius=26, fill=fill, outline=outline, width=3)
    draw.text((x0 + 28, y0 + 22), title, fill=INK, font=F_H2)


def radial(size=360, power=1.0, color=False):
    img = Image.new("RGB", (size, size), (12, 14, 18) if color else (255, 255, 255))
    pix = img.load()
    cx = cy = size / 2
    for y in range(size):
        for x in range(size):
            dx = (x - cx) / (size * 0.48)
            dy = (y - cy) / (size * 0.48)
            r = min(1.0, math.sqrt(dx * dx + dy * dy))
            m = max(0.0, 1.0 - r)
            v = m ** power
            if color:
                rr = int(min(255, v * 330))
                gg = int(min(255, (v ** 1.5) * 165))
                bb = int(min(255, (v ** 3.0) * 60))
                pix[x, y] = (rr, gg, bb)
            else:
                g = int(v * 255)
                pix[x, y] = (g, g, g)
    if color:
        img = img.filter(ImageFilter.GaussianBlur(1.1))
    return img


def noise_image(w, h, seed=4):
    rng = random.Random(seed)
    base = Image.new("L", (w, h))
    pix = base.load()
    for y in range(h):
        for x in range(w):
            nx = x / w
            ny = y / h
            v = 0
            amp = 1.0
            total = 0
            for octave in range(5):
                scale = 2 ** octave
                sx = int(nx * scale * 9)
                sy = int(ny * scale * 9)
                rng.seed(seed + sx * 928371 + sy * 1237 + octave * 91)
                v += rng.random() * amp
                total += amp
                amp *= 0.5
            pix[x, y] = int(255 * v / total)
    return base.filter(ImageFilter.GaussianBlur(5))


def colorize_fire(mask):
    img = Image.new("RGB", mask.size, (12, 14, 18))
    pix = img.load()
    mp = mask.load()
    for y in range(mask.height):
        for x in range(mask.width):
            m = mp[x, y] / 255
            if m < 0.03:
                continue
            pix[x, y] = (
                int(min(255, 50 + m * 300)),
                int(min(255, m ** 1.3 * 170)),
                int(min(255, m ** 4 * 70)),
            )
    return img.filter(ImageFilter.GaussianBlur(1.2))


def make_shaping_power():
    img = Image.new("RGB", (2400, 1200), "white")
    d = ImageDraw.Draw(img)
    label(d, (80, 54), "Shaping Function: Power thay đổi cảm giác flare", BLUE, F_TITLE)
    label(d, (80, 118), "Visual trước, công thức sau: cùng một radial mask nhưng curve khác nhau tạo core khác nhau.", MUTED, F_BODY)

    panel(d, (80, 190, 1120, 1050), "Biểu đồ hàm số")
    gx0, gy0, gx1, gy1 = 170, 340, 1040, 930
    d.line((gx0, gy1, gx1, gy1), fill=INK, width=4)
    d.line((gx0, gy0, gx0, gy1), fill=INK, width=4)
    for i in range(6):
        x = gx0 + i * (gx1 - gx0) / 5
        y = gy1 - i * (gy1 - gy0) / 5
        d.line((x, gy0, x, gy1), fill=GRID, width=1)
        d.line((gx0, y, gx1, y), fill=GRID, width=1)
    colors = [(80, 130, 230), (40, 40, 40), (230, 120, 38), (205, 62, 62)]
    powers = [0.5, 1.0, 2.0, 4.0]
    for c, pwr in zip(colors, powers):
        pts = []
        for i in range(250):
            x = i / 249
            y = x ** pwr
            pts.append((gx0 + x * (gx1 - gx0), gy1 - y * (gy1 - gy0)))
        d.line(pts, fill=c, width=6)
    legend_y = 950
    for i, (c, pwr) in enumerate(zip(colors, powers)):
        x = gx0 + i * 210
        d.line((x, legend_y, x + 70, legend_y), fill=c, width=8)
        label(d, (x + 82, legend_y - 17), f"pow(x,{pwr:g})", c, F_SMALL)
    label(d, (gx0, gy1 + 36), "0", MUTED, F_SMALL)
    label(d, (gx1 - 20, gy1 + 36), "1", MUTED, F_SMALL)
    label(d, (gx0 - 44, gy0 - 8), "1", MUTED, F_SMALL)

    panel(d, (1220, 190, 2320, 1050), "Kết quả trên radial mask")
    xs = [1280, 1540, 1800, 2060]
    for x, pwr, c in zip(xs, powers, colors):
        mask = radial(210, pwr, False)
        flare = radial(210, pwr, True)
        img.paste(mask, (x, 380))
        img.paste(flare, (x, 650))
        d.rectangle((x, 380, x + 210, 590), outline=(180, 184, 190), width=2)
        d.rectangle((x, 650, x + 210, 860), outline=(180, 184, 190), width=2)
        label(d, (x + 105, 330), f"Power {pwr:g}", c, F_SMALL, "mm")
    label(d, (1260, 902), "Hàng trên: mask grayscale solo. Hàng dưới: emissive flare sau khi tint/bloom giả lập.", MUTED, F_SMALL)
    label(d, (1260, 965), "Power cao co vùng sáng lại. Đừng tăng intensity khi thứ bạn cần là shape/core.", RED, F_SMALL)
    save(img, "v9_power_shaping_flare.png")


def make_mask_solo():
    img = Image.new("RGB", (2400, 1150), "white")
    d = ImageDraw.Draw(img)
    label(d, (80, 54), "Solo Grayscale: đọc dissolve qua từng mask", BLUE, F_TITLE)
    label(d, (80, 118), "Nếu chỉ nhìn bản màu cuối cùng, bloom có thể che lỗi threshold, edge band hoặc alpha.", MUTED, F_BODY)
    w, h = 420, 420
    noise = noise_image(w, h, seed=12)
    threshold = 135
    visible = noise.point(lambda v: 255 if v > threshold else 0)
    soft = noise.point(lambda v: int(max(0, min(255, (v - 105) / 60 * 255))))
    edge = noise.point(lambda v: 255 if threshold <= v <= threshold + 22 else 0).filter(ImageFilter.GaussianBlur(2))
    final = colorize_fire(Image.composite(soft, edge, edge))
    panels = [
        ("1. Noise source", noise.convert("RGB"), "Dữ liệu thô, chưa có ý nghĩa VFX."),
        ("2. Visible mask", visible.convert("RGB"), "Vùng trắng còn hiện, vùng đen biến mất."),
        ("3. Edge band", edge.convert("RGB"), "Chỉ vùng sát threshold được đốt sáng."),
        ("4. Final color", final, "Màu chỉ nên đến sau khi mask đã đúng."),
    ]
    x0 = 90
    for i, (title, im, cap) in enumerate(panels):
        x = x0 + i * 570
        panel(d, (x, 210, x + 500, 990), title)
        img.paste(im, (x + 40, 330))
        d.rectangle((x + 40, 330, x + 40 + w, 330 + h), outline=(160, 166, 174), width=3)
        label(d, (x + 40, 800), cap, INK, F_SMALL)
    d.line((615, 540, 680, 540), fill=BLUE, width=8)
    d.polygon([(680, 540), (650, 520), (650, 560)], fill=BLUE)
    d.line((1185, 540, 1250, 540), fill=BLUE, width=8)
    d.polygon([(1250, 540), (1220, 520), (1220, 560)], fill=BLUE)
    d.line((1755, 540, 1820, 540), fill=BLUE, width=8)
    d.polygon([(1820, 540), (1790, 520), (1790, 560)], fill=BLUE)
    label(d, (90, 1040), "Công thức: edge = smoothstep(t - w, t, mask) - smoothstep(t, t + soft, mask)", MUTED, F_CODE)
    save(img, "v9_mask_solo_dissolve.png")


def make_space_vectors():
    img = Image.new("RGB", (2400, 1350), "white")
    d = ImageDraw.Draw(img)
    label(d, (80, 58), "Space Transform: vector phải cùng ngôn ngữ", BLUE, F_TITLE)
    label(d, (80, 122), "Dot, Fresnel, fake lighting sẽ sai nếu Normal, ViewDir, LightDir không ở cùng space.", MUTED, F_BODY)
    panel(d, (120, 230, 2280, 1180), "Minh họa vector")
    origin = (960, 760)
    def arrow(end, color, text, tpos):
        d.line((origin[0], origin[1], end[0], end[1]), fill=color, width=12)
        ang = math.atan2(end[1] - origin[1], end[0] - origin[0])
        head = []
        for a in [0, 2.45, -2.45]:
            head.append((end[0] - 38 * math.cos(ang + a), end[1] - 38 * math.sin(ang + a)))
        d.polygon(head, fill=color)
        label(d, tpos, text, color, F_H2)
    d.ellipse((820, 640, 1100, 880), fill=(226, 231, 237), outline=(150, 158, 168), width=4)
    d.arc((820, 640, 1100, 880), 200, 340, fill=(130, 138, 148), width=5)
    label(d, (865, 900), "surface point", MUTED, F_SMALL)
    arrow((1430, 760), RED, "X / Tangent", (1450, 735))
    arrow((960, 360), GREEN, "Z / NormalWS", (1005, 345))
    arrow((600, 980), (40, 105, 210), "Y / Bitangent", (335, 970))
    arrow((1730, 430), CYAN, "ViewDirWS -> Camera", (1395, 500))
    d.rounded_rectangle((1780, 245, 2050, 390), radius=22, fill=(20, 30, 44))
    d.ellipse((1840, 285, 1890, 335), fill=(80, 180, 255))
    label(d, (1832, 430), "Camera", MUTED, F_SMALL)
    label(d, (160, 1030), "Sai thường gặp: dot(TangentNormal, ViewDirWS) vẫn compile, nhưng visual sẽ trượt hoặc rim sai.", RED, F_BODY)
    label(d, (160, 1090), "Quy tắc: normalize vector và transform về cùng space trước khi dot/cross.", GREEN, F_BODY)
    save(img, "v9_space_vectors.png")


def make_lane_diagram():
    img = Image.new("RGB", (2400, 1200), "white")
    d = ImageDraw.Draw(img)
    label(d, (80, 54), "Lane Diagram: đọc material theo làn dữ liệu", BLUE, F_TITLE)
    label(d, (80, 118), "Đừng đọc graph bằng cách đuổi dây spaghetti. Tách shader thành các lane có mục đích rõ.", MUTED, F_BODY)
    lanes = [
        ("UV Lane", "TexCoord -> Panner -> DistortUV", (229, 243, 255)),
        ("Mask Lane", "Texture/Noise -> SmoothStep -> Edge", (238, 246, 232)),
        ("Color Lane", "ParticleColor -> Ramp -> Emissive", (255, 243, 224)),
        ("Opacity/Blend", "Alpha -> DepthFade -> Final Opacity", (248, 232, 232)),
        ("Interaction", "SceneDepth / DynamicParam / HitPos", (239, 235, 252)),
        ("Debug/Cost", "Solo mask -> Complexity -> Variant", (236, 240, 244)),
    ]
    y = 220
    for idx, (name, flow, fill) in enumerate(lanes):
        d.rounded_rectangle((120, y, 2280, y + 120), radius=22, fill=fill, outline=(185, 193, 204), width=3)
        label(d, (160, y + 35), name, BLUE if idx < 3 else INK, F_H2)
        parts = flow.split(" -> ")
        x = 610
        for i, part in enumerate(parts):
            d.rounded_rectangle((x, y + 26, x + 330, y + 92), radius=16, fill="white", outline=(168, 176, 188), width=2)
            label(d, (x + 20, y + 43), part, INK, F_SMALL)
            if i < len(parts) - 1:
                d.line((x + 340, y + 60, x + 395, y + 60), fill=MUTED, width=4)
                d.polygon([(x + 395, y + 60), (x + 372, y + 47), (x + 372, y + 73)], fill=MUTED)
            x += 400
        y += 150
    label(d, (120, 1135), "Production use: review feedback thường chỉ cần sửa một lane, không phải phá toàn graph.", MUTED, F_BODY)
    save(img, "v9_shader_lane_diagram.png")


def smoke_blob(w, h, fade_floor=False):
    img = Image.new("RGBA", (w, h), (0, 0, 0, 0))
    rng = random.Random(8)
    for i in range(90):
        x = rng.randint(int(w * 0.25), int(w * 0.75))
        y = rng.randint(int(h * 0.10), int(h * 0.95))
        r = rng.randint(45, 105)
        alpha = rng.randint(15, 42)
        layer = Image.new("RGBA", (w, h), (0, 0, 0, 0))
        dd = ImageDraw.Draw(layer)
        dd.ellipse((x - r, y - r, x + r, y + r), fill=(210, 214, 216, alpha))
        layer = layer.filter(ImageFilter.GaussianBlur(r // 3))
        img.alpha_composite(layer)
    floor_y = int(h * 0.78)
    pix = img.load()
    for y in range(h):
        for x in range(w):
            r, g, b, a = pix[x, y]
            if y > floor_y:
                if fade_floor:
                    factor = max(0, 1 - (y - floor_y) / 100)
                    pix[x, y] = (r, g, b, int(a * factor))
                else:
                    pix[x, y] = (r, g, b, 0)
    return img


def make_depthfade_failure():
    img = Image.new("RGB", (2400, 1200), (245, 247, 250))
    d = ImageDraw.Draw(img)
    label(d, (80, 56), "Failure Atlas: smoke cắt vào sàn vs DepthFade", BLUE, F_TITLE)
    label(d, (80, 120), "Ảnh phải cho thấy triệu chứng trước khi giải thích node. Đây là kiểu visual cần có trong failure atlas.", MUTED, F_BODY)
    for i, (title, fixed) in enumerate([("A. Lỗi: hard intersection", False), ("B. Sửa: DepthFade mềm", True)]):
        x = 120 + i * 1140
        panel(d, (x, 210, x + 1020, 1010), title, fill=(255, 255, 255))
        bg = Image.new("RGB", (860, 560), (35, 39, 45))
        bd = ImageDraw.Draw(bg)
        bd.polygon([(0, 410), (860, 330), (860, 560), (0, 560)], fill=(74, 78, 84))
        bd.line((0, 410, 860, 330), fill=(135, 141, 150), width=4)
        smoke = smoke_blob(860, 560, fade_floor=fixed)
        bg = bg.convert("RGBA")
        bg.alpha_composite(smoke)
        img.paste(bg.convert("RGB"), (x + 80, 330))
        if fixed:
            label(d, (x + 95, 920), "Opacity *= DepthFade(FadeDistance)", GREEN, F_CODE)
        else:
            label(d, (x + 95, 920), "Opacity không biết geometry phía sau", RED, F_CODE)
            d.line((x + 230, 720, x + 780, 670), fill=RED, width=8)
            label(d, (x + 250, 735), "đường cắt lộ rõ", RED, F_SMALL)
    save(img, "v9_failure_depthfade_before_after.png")


def make_shader_complexity():
    img = Image.new("RGB", (2400, 1200), "white")
    d = ImageDraw.Draw(img)
    label(d, (80, 54), "Optimization View: overdraw nhìn bằng shader complexity", BLUE, F_TITLE)
    label(d, (80, 118), "Minh họa synthetic theo convention xanh -> vàng -> đỏ -> trắng: vùng phủ nhiều layer tốn hơn.", MUTED, F_BODY)
    colors = [(0, 160, 70), (220, 210, 40), (230, 80, 40), (245, 245, 245)]
    for i, c in enumerate(colors):
        d.rectangle((1760 + i * 120, 135, 1860 + i * 120, 175), fill=c, outline=(80, 80, 80))
    label(d, (1760, 185), "rẻ", MUTED, F_SMALL)
    label(d, (2120, 185), "đắt", MUTED, F_SMALL)
    for i, title in enumerate(["A. Card alpha rộng, nhiều overlap", "B. Crop sát silhouette, ít layer hơn"]):
        x = 120 + i * 1140
        panel(d, (x, 240, x + 1020, 1030), title)
        canvas = Image.new("RGB", (860, 560), (12, 14, 18))
        cd = ImageDraw.Draw(canvas, "RGBA")
        rng = random.Random(20 + i)
        count = 32 if i == 0 else 12
        radius = 150 if i == 0 else 90
        for n in range(count):
            cx = rng.randint(120, 740)
            cy = rng.randint(120, 450)
            if i == 0:
                col = (220, 80, 40, 78)
            else:
                col = (55, 190, 90, 95)
            cd.ellipse((cx - radius, cy - radius, cx + radius, cy + radius), fill=col)
        if i == 0:
            cd.rectangle((260, 160, 640, 470), outline=(245, 245, 245, 210), width=8)
            label(cd, (275, 180), "white hot", (255, 255, 255, 235), F_SMALL)
        else:
            cd.rectangle((330, 220, 530, 420), outline=(250, 220, 80, 220), width=6)
        img.paste(canvas, (x + 80, 360))
        note = "Giảm area thường thắng giảm vài instruction." if i == 1 else "Instruction thấp vẫn đắt nếu phủ nhiều pixel."
        label(d, (x + 80, 950), note, RED if i == 0 else GREEN, F_BODY)
    save(img, "v9_shader_complexity_overdraw.png")


def make_realtime_contract_simple():
    img = Image.new("RGB", (2400, 1150), "white")
    d = ImageDraw.Draw(img)
    label(d, (80, 56), "Niagara -> Material: hợp đồng dữ liệu tối thiểu", BLUE, F_TITLE)
    label(d, (80, 122), "Sơ đồ này cố ý phẳng và ít trang trí: mục tiêu là đọc được data flow, không phải xem render đẹp.", MUTED, F_BODY)

    def box(x, y, w, h, title, lines, fill, stroke=(190, 198, 208)):
        d.rounded_rectangle((x, y, x + w, y + h), radius=18, fill=fill, outline=stroke, width=3)
        label(d, (x + 26, y + 22), title, BLUE, F_H2)
        yy = y + 82
        for line in lines:
            label(d, (x + 34, yy), "- " + line, INK, F_SMALL)
            yy += 42

    def arrow(x0, y0, x1, y1, color=BLUE, text=None):
        d.line((x0, y0, x1, y1), fill=color, width=8)
        d.polygon([(x1, y1), (x1 - 28, y1 - 16), (x1 - 28, y1 + 16)], fill=color)
        if text:
            label(d, ((x0 + x1) / 2 - 70, y0 - 38), text, color, F_SMALL)

    box(90, 250, 460, 540, "Particle / Niagara", [
        "Particle Color RGB",
        "Particle Alpha",
        "DynamicParam R/G/B/A",
        "SubUV frame index",
        "Size / age / random"
    ], (235, 245, 255))

    box(690, 190, 530, 660, "Material contract", [
        "RGB -> tint/emissive color",
        "A -> opacity or dissolve",
        "R -> dissolve amount",
        "G -> distortion strength",
        "B -> edge/emissive boost",
        "SubUV -> atlas sampling"
    ], (245, 247, 250))

    box(1370, 250, 430, 540, "Render rules", [
        "Blend mode",
        "DepthFade",
        "Soft particle fade",
        "Camera fade",
        "Overdraw budget"
    ], (255, 247, 235))

    box(1930, 250, 380, 540, "Final pixels", [
        "Emissive",
        "Opacity",
        "Distortion",
        "Masked / translucent",
        "Debug views"
    ], (238, 248, 240))

    arrow(550, 520, 690, 520, BLUE, "data")
    arrow(1220, 520, 1370, 520, ORANGE, "shader")
    arrow(1800, 520, 1930, 520, GREEN, "output")

    # Simple channel strip.
    x0, y0 = 690, 900
    channels = [
        ("R", "Dissolve", RED),
        ("G", "Distort", GREEN),
        ("B", "Edge boost", BLUE),
        ("A", "Soft fade", MUTED),
    ]
    for i, (ch, desc, col) in enumerate(channels):
        x = x0 + i * 380
        d.rounded_rectangle((x, y0, x + 310, y0 + 95), radius=16, fill=(250, 251, 252), outline=col, width=4)
        label(d, (x + 24, y0 + 24), ch, col, F_H2)
        label(d, (x + 86, y0 + 32), desc, INK, F_SMALL)

    label(d, (90, 1040), "Production rule: nếu Niagara chỉnh curve mà material không đổi, kiểm tra ngay Particle Color và Dynamic Parameter trước.", RED, F_BODY)
    save(img, "v11_realtime_contract_simple.png")


def make_professional_cover():
    img = Image.new("RGB", (2480, 3508), (246, 247, 249))
    d = ImageDraw.Draw(img)
    title_font = font(122, True)
    sub_font = font(46)
    meta_font = font(34)
    small_font = font(28)
    code_font = font(30)

    navy = (18, 49, 78)
    blue = (28, 94, 170)
    orange = (230, 110, 38)
    paper = (246, 247, 249)
    ink = (24, 29, 36)
    muted = (88, 96, 107)
    line = (207, 214, 222)

    d.rectangle((0, 0, 2480, 3508), fill=paper)
    d.rectangle((0, 0, 2480, 390), fill=navy)
    d.rectangle((0, 3110, 2480, 3508), fill=navy)
    d.rectangle((0, 390, 120, 3110), fill=blue)
    d.rectangle((120, 390, 152, 3110), fill=orange)

    d.text((260, 560), "Giáo Trình", fill=blue, font=sub_font)
    d.text((260, 645), "Material VFX", fill=navy, font=title_font)
    d.text((260, 785), "Realtime", fill=navy, font=title_font)
    d.line((260, 960, 1420, 960), fill=orange, width=9)
    d.text((260, 1015), "Technical VFX Artist Handbook", fill=ink, font=sub_font)
    d.text((260, 1085), "Unreal Material · Niagara · Shader Graph · HLSL", fill=muted, font=meta_font)

    plate = (260, 1260, 2220, 2550)
    d.rounded_rectangle(plate, radius=34, fill=(255, 255, 255), outline=line, width=4)
    d.rectangle((260, 1260, 2220, 1365), fill=(232, 238, 246))
    d.text((315, 1292), "Visual-first shader study map", fill=navy, font=meta_font)

    d.rounded_rectangle((330, 1460, 840, 2005), radius=22, fill=(249, 250, 252), outline=line, width=3)
    d.text((365, 1490), "Mask -> Edge", fill=blue, font=meta_font)
    for i, pwr in enumerate([1.0, 2.2]):
        mask = radial(180, pwr, False)
        img.paste(mask, (385 + i * 230, 1595))
        d.rectangle((385 + i * 230, 1595, 565 + i * 230, 1775), outline=(160, 168, 178), width=2)
    d.line((440, 1850, 735, 1850), fill=orange, width=8)
    d.polygon([(735, 1850), (705, 1832), (705, 1868)], fill=orange)
    d.text((380, 1900), "shape before color", fill=muted, font=small_font)

    d.rounded_rectangle((985, 1460, 1495, 2005), radius=22, fill=(249, 250, 252), outline=line, width=3)
    d.text((1020, 1490), "UV / Space", fill=blue, font=meta_font)
    cx, cy = 1240, 1720
    for r in [55, 105, 155, 205]:
        d.ellipse((cx - r, cy - r, cx + r, cy + r), outline=(160, 173, 189), width=3)
    for a in range(0, 360, 30):
        rad = math.radians(a)
        d.line((cx, cy, cx + 215 * math.cos(rad), cy + 215 * math.sin(rad)), fill=(218, 224, 232), width=2)
    d.line((cx, cy, cx + 220, cy), fill=(210, 54, 54), width=7)
    d.line((cx, cy, cx, cy - 220), fill=(48, 143, 91), width=7)
    d.text((1040, 1900), "polar, object, world, view", fill=muted, font=small_font)

    d.rounded_rectangle((1640, 1460, 2150, 2005), radius=22, fill=(249, 250, 252), outline=line, width=3)
    d.text((1675, 1490), "Runtime contract", fill=blue, font=meta_font)
    nodes = [
        ((1690, 1610, 1860, 1685), "Niagara", (233, 244, 255)),
        ((1970, 1610, 2110, 1685), "Material", (255, 245, 232)),
        ((1830, 1810, 2010, 1885), "Pixels", (236, 248, 239)),
    ]
    for rect, text, fill in nodes:
        d.rounded_rectangle(rect, radius=16, fill=fill, outline=(170, 180, 192), width=3)
        d.text((rect[0] + 20, rect[1] + 22), text, fill=ink, font=small_font)
    d.line((1860, 1647, 1970, 1647), fill=blue, width=6)
    d.polygon([(1970, 1647), (1945, 1632), (1945, 1662)], fill=blue)
    d.line((2040, 1685, 1950, 1810), fill=orange, width=6)
    d.polygon([(1950, 1810), (1950, 1780), (1978, 1797)], fill=orange)
    d.text((1688, 1900), "ParticleColor / DynamicParam", fill=muted, font=small_font)

    d.rounded_rectangle((330, 2140, 2150, 2435), radius=22, fill=(24, 29, 36), outline=(24, 29, 36), width=3)
    code_lines = [
        "mask = smoothstep(threshold, threshold + softness, noise)",
        "edge = smoothstep(t - w, t, mask) - smoothstep(t, t + s, mask)",
        "emissive = tint * mask * intensity",
    ]
    y = 2195
    for line in code_lines:
        d.text((390, y), line, fill=(232, 238, 246), font=code_font)
        y += 70
    d.text((390, 2380), "Read graph by lanes: Shape · Mask · Motion · Color · Interaction · Cost", fill=(112, 188, 255), font=small_font)

    d.text((260, 2800), "Production edition", fill=blue, font=meta_font)
    d.text((260, 2870), "Built for PDF reading, real debugging, and shader decisions.", fill=ink, font=small_font)
    d.text((260, 2985), "V14 · GitHub PDF edition · 2026", fill=muted, font=meta_font)
    d.text((260, 3235), "Prepared for Technical VFX study", fill=(235, 240, 247), font=meta_font)
    d.text((260, 3300), "VFXSamplePack · ForgePilot Knowledge", fill=(178, 190, 205), font=small_font)
    save(img, "v14_github_pdf_cover.png")


if __name__ == "__main__":
    make_shaping_power()
    make_mask_solo()
    make_space_vectors()
    make_lane_diagram()
    make_depthfade_failure()
    make_shader_complexity()
    make_realtime_contract_simple()
    make_professional_cover()
    print("Generated V9 manual visuals in", OUT)
