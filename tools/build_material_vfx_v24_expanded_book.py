from pathlib import Path
import importlib.util

from docx.enum.text import WD_ALIGN_PARAGRAPH
from docx.shared import Cm


ROOT = Path(__file__).resolve().parents[1]
BASE_SCRIPT = ROOT / "tools" / "build_material_vfx_v23_book_of_shaders_style.py"

spec = importlib.util.spec_from_file_location("material_vfx_v23", BASE_SCRIPT)
base = importlib.util.module_from_spec(spec)
spec.loader.exec_module(base)

OUT = Path.home() / "Downloads" / "Giao_Trinh_Material_VFX_Realtime_V24_ExpandedBook.docx"
base.OUT = OUT


def cover_v24(doc):
    base.p(doc, "Giáo trình", size=12, font=base.SANS, bold=True, color=base.ORANGE, before=66, after=5, align=WD_ALIGN_PARAGRAPH.CENTER)
    title = base.p(doc, "Material VFX Realtime", size=32, font=base.HEAD, bold=True, color=base.BLUE, after=9, align=WD_ALIGN_PARAGRAPH.CENTER)
    title.paragraph_format.line_spacing = 1.0
    base.p(doc, "Tư duy Material, Shader và Production cho Technical VFX Artist", size=12.2, font=base.BODY, italic=True, color=base.MUTED, after=24, align=WD_ALIGN_PARAGRAPH.CENTER)
    rule = base.p(doc, "", before=0, after=22)
    base.border_bottom(rule, color="C98E54", size="10")
    base.p(doc, "Shape · Mask · Motion · Color · Interaction · Cost", size=14.2, font=base.SANS, bold=True, color=base.BLUE, before=8, after=8, align=WD_ALIGN_PARAGRAPH.CENTER)
    base.p(doc, "Unreal Material · Niagara · Shader Graph · HLSL/GLSL · Optimization", size=11.0, font=base.SANS, color=base.INK, after=26, align=WD_ALIGN_PARAGRAPH.CENTER)
    summary = base.p(
        doc,
        "Bản mở rộng: đọc visual như dữ liệu, học toán bằng hình ảnh, hiểu renderer, viết shader khi cần, và biến Material Graph thành một hệ thống production có thể debug.",
        size=10.4,
        font=base.BODY,
        color=base.INK,
        before=16,
        after=22,
        align=WD_ALIGN_PARAGRAPH.CENTER,
    )
    summary.paragraph_format.left_indent = Cm(1.0)
    summary.paragraph_format.right_indent = Cm(1.0)
    base.p(doc, "Phiên bản V24 · Expanded Book · 2026", size=9.3, font=base.SANS, color=base.MUTED, before=8, after=0, align=WD_ALIGN_PARAGRAPH.CENTER)


def make_chapter(title, image, caption, sections, note, case, code):
    return {
        "title": title,
        "image": (image, caption) if image else None,
        "sections": sections,
        "notes": [note] if note else [],
        "case": case,
        "code": code,
    }


EXTRA_CHAPTERS = [
    make_chapter(
        "Texture không chỉ là hình ảnh",
        "v9_mask_solo_dissolve.png",
        "Một Texture dùng cho Material VFX thường là dữ liệu điều khiển: Mask, Noise, Flow, Height hoặc Frame Index.",
        [
            ("Texture mỹ thuật và Texture dữ liệu khác nhau ở mục đích",
             [
                 "Khi một Texture được dùng làm màu bề mặt, mắt người xem quan tâm tới cảm giác vật liệu: đá, kim loại, da, khói, lửa hoặc vết bẩn. Nhưng trong Material VFX, rất nhiều Texture không còn là tranh để nhìn trực tiếp. Nó là dữ liệu để Shader đọc. Một kênh đỏ có thể là Dissolve Mask, kênh xanh lá là Edge Noise, kênh xanh dương là Distortion Strength, còn Alpha là Opacity.",
                 "Sự khác biệt này thay đổi cách bạn author và import Texture. Texture màu có thể chịu được một chút Compression hoặc Mip nếu mắt vẫn thấy ổn. Texture dữ liệu thì nhạy hơn. Nếu một Height Map bị nén quá mạnh, Parallax sẽ rung. Nếu một Flow Map bị sRGB, hướng vector sẽ sai. Nếu một Mask bị Mip quá mềm, cạnh Dissolve sẽ mất lực ở xa camera.",
                 "Một Technical VFX Artist cần đọc Texture bằng hai mắt: mắt mỹ thuật và mắt dữ liệu. Mắt mỹ thuật hỏi nó có đẹp không. Mắt dữ liệu hỏi giá trị có đúng range không, channel có bị đảo không, Compression có phá logic không, và Material có đang giả định sai về dữ liệu đầu vào không."
             ]),
            ("Channel Packing là kỹ thuật production, không phải mẹo tiết kiệm nhỏ",
             [
                 "Channel Packing nghĩa là đóng nhiều Mask grayscale vào cùng một Texture RGBA. Lý do không chỉ là tiết kiệm file. Trong realtime, mỗi lần sample Texture đều có chi phí băng thông và Cache. Nếu bốn Mask nhỏ được đóng vào một Texture, Material có thể lấy bốn tín hiệu điều khiển bằng một Texture Fetch.",
                 "Ví dụ một Texture cho Slash có thể dùng R cho Core Mask, G cho Edge Breakup, B cho Distortion Noise, A cho Lifetime Fade. Khi đọc Graph, hãy đặt tên rõ: không gọi chung là TextureMask. Hãy tách bằng ComponentMask rồi đặt comment theo nghĩa visual. Người khác mở Material phải biết kênh nào điều khiển cái gì.",
                 "Lỗi thường gặp là pack quá tham. Nếu bốn channel có tần số chi tiết khác nhau, Mip và Compression có thể làm một channel đẹp nhưng channel khác hỏng. Mask mềm cho Smoke và Mask sắc cho Dissolve có thể không nên ở cùng một Texture nếu setting import của chúng cần khác nhau."
             ]),
            ("sRGB, Linear và lỗi nhìn rất nhỏ nhưng sửa rất lâu",
             [
                 "Texture màu thường dùng sRGB vì nó lưu dữ liệu theo cách phù hợp với mắt người. Nhưng Mask, Normal, Height, Flow Map và các Texture dữ liệu thường cần Linear. Nếu bạn để sRGB cho một Noise dùng làm Mask, giá trị giữa 0 và 1 sẽ bị biến đổi. SmoothStep, Power hoặc Threshold sau đó không còn phản ứng như bạn nghĩ.",
                 "Triệu chứng rất dễ nhầm với lỗi mỹ thuật. Dissolve có thể bị bẩn, Edge quá dày, Distortion quá mạnh hoặc Falloff không đều. Người mới thường sửa Threshold, Contrast hoặc Power. Người có kinh nghiệm kiểm tra Texture Import trước. Nếu dữ liệu đầu vào sai, Graph càng thông minh càng khuếch đại lỗi.",
                 "Một bài test nhanh là đưa Texture vào Emissive grayscale và xem histogram bằng mắt. Nếu Mask đáng ra phải có vùng đen/trắng rõ nhưng lại thành xám bẩn, hoặc Flow Map không nằm quanh 0.5 trước khi remap, hãy kiểm tra sRGB, Compression, Mip Gen và Texture Group."
             ]),
        ],
        ("Ghi nhớ", "Khi Texture được dùng như dữ liệu, hãy kiểm tra import setting trước khi chỉnh Graph. Sai dữ liệu đầu vào làm mọi node phía sau trở thành suy đoán.", False),
        [
            "Một Dissolve bị Edge dày bất thường không nhất thiết do SmoothStep sai. Nếu Noise Texture đang bật sRGB, giá trị midtone bị cong, Threshold sẽ quét qua bề mặt theo tốc độ không đều. Sửa import có thể sạch hơn thêm ba node Remap.",
            "Một Flow Map làm Fire trượt lệch hướng có thể không phải do UV. Flow Map thường lưu vector quanh 0.5. Nếu bạn quên remap từ 0..1 sang -1..1, hoặc Compression làm mất độ chính xác, Motion sẽ có cảm giác trôi sai ngay cả khi Panner đúng."
        ],
        [
            "packed = TextureSample(PackedMask, UV)",
            "core   = packed.r",
            "edge   = packed.g",
            "dist   = packed.b * 2 - 1",
            "alpha  = packed.a"
        ],
    ),
    make_chapter(
        "Tọa độ UV - mặt phẳng nhỏ điều khiển rất nhiều thứ",
        "v9_space_vectors.png",
        "UV là không gian 2D nơi Texture được đọc. Khi UV sai, hầu hết logic phía sau đều sai.",
        [
            ("UV là bản đồ, không phải chỉ là cặp số U và V",
             [
                 "Trong Material, UV là cách Shader hỏi Texture: tại điểm này trên mesh hoặc sprite, tôi nên đọc pixel nào? Với mesh, UV thường được unwrap sẵn. Với sprite hoặc card VFX, UV thường là một hình vuông 0..1. Với flipbook, UV lại được chia thành nhiều ô frame. Cùng tên UV, nhưng bối cảnh sử dụng khác nhau.",
                 "Khi bạn đặt Panner vào UV, bạn không làm Texture thật sự di chuyển. Bạn đang thay đổi tọa độ đọc Texture theo thời gian. Texture đứng yên trong memory, nhưng câu hỏi gửi tới Texture thay đổi. Đây là một đổi góc nhìn quan trọng: rất nhiều Motion trong VFX chỉ là Motion của tọa độ đọc dữ liệu.",
                 "Nếu Effect bị trượt sai, hãy debug UV như debug dữ liệu. Đưa UV.x ra màu đỏ, UV.y ra màu xanh lá. Nếu gradient không chạy như mong đợi, đừng chỉnh màu. Tọa độ đầu vào đang sai."
             ]),
            ("Tiling, Offset và Scale là ba thao tác cơ bản nhất",
             [
                 "Tiling nhân UV lên để Texture lặp lại nhiều lần. Offset cộng thêm giá trị để Texture trượt. Scale quanh tâm cần cẩn thận hơn, vì nếu chỉ nhân UV với một số, bạn scale quanh gốc 0,0 chứ không scale quanh tâm 0.5,0.5. Đây là lý do nhiều radial mask hoặc ring bị lệch khi artist mới thử phóng to thu nhỏ.",
                 "Công thức scale quanh tâm là đưa UV về tâm, scale, rồi trả lại. `(UV - 0.5) * Scale + 0.5`. Nếu muốn xoay quanh tâm, logic cũng tương tự: đưa về tâm, nhân ma trận xoay, rồi cộng 0.5. Khi hiểu nguyên tắc này, Rotator không còn là node bí ẩn.",
                 "Trong production, expose Tiling và Offset rất hữu ích, nhưng đừng expose mọi thứ bừa bãi. Một Material Instance có mười tham số UV không tên sẽ làm artist khác sợ dùng. Hãy đặt tên theo mục đích: `FlameNoiseTiling`, `DistortionSpeed`, `RingRotation` thay vì `Scalar1`."
             ]),
            ("Local UV, Atlas UV và Derived 0-1 UV",
             [
                 "Với SubUV hoặc flipbook, có hai loại UV cần phân biệt. Atlas UV là tọa độ trên toàn bộ Texture Sheet. Local UV là tọa độ 0..1 bên trong frame hiện tại. Nếu bạn muốn đọc Texture frame thì Atlas UV đúng. Nếu bạn muốn tạo radial fade riêng trong từng frame, bạn cần Local UV.",
                 "Derived 0-1 UV là cách lấy lại không gian local bên trong frame. Nó rất hữu ích khi renderer đã chọn frame cho bạn, nhưng Material vẫn cần một mask procedural chạy trong ô hiện tại. Nếu không có nó, radial gradient sẽ trải qua cả atlas, gây cảm giác mask nhảy hoặc cắt sai.",
                 "Debug tốt nhất là dùng NumberGrid. Nếu số frame đúng nhưng gradient nằm sai chỗ, lỗi nằm ở local UV. Nếu số frame sai, lỗi nằm ở renderer SubUV hoặc dữ liệu frame từ Niagara. Tách hai lỗi này giúp tiết kiệm rất nhiều thời gian."
             ]),
        ],
        ("Lỗi thường gặp", "Scale UV quanh gốc 0,0 làm pattern bị trôi khỏi tâm. Với radial, ring, vortex hoặc flare, hãy scale quanh 0.5,0.5.", True),
        [
            "Một Portal Ring bị lệch tâm dù Texture đúng thường do UV được scale hoặc rotate quanh gốc. Khi camera hoặc mesh scale thay đổi, lỗi này càng rõ. Sửa bằng cách center UV trước khi biến đổi.",
            "Một Flipbook Fire có Edge Fade bị nhảy qua nhiều frame thường do dùng Atlas UV cho procedural mask. Hãy tạo Local UV trong frame rồi dựng Edge Fade từ Local UV đó."
        ],
        [
            "centered = UV - 0.5",
            "scaled   = centered * Tiling",
            "rotated  = mul(Rotation2x2, scaled)",
            "finalUV  = rotated + 0.5"
        ],
    ),
    make_chapter(
        "Shaping Function - uốn số để uốn hình",
        "v9_power_shaping_flare.png",
        "Cùng một mask radial, các hàm shaping khác nhau tạo cảm giác visual rất khác nhau.",
        [
            ("Shader không chỉ tính đúng; Shader tạo cảm giác",
             [
                 "Một giá trị 0..1 có thể đi thẳng vào Opacity hoặc Emissive. Nhưng nếu đi thẳng, cảm giác thường rất phẳng. Shaping Function là cách uốn giá trị trước khi dùng nó. Power làm core chặt hơn hoặc falloff mềm hơn. SmoothStep làm transition có kiểm soát. Sine biến tuyến tính thành nhịp dao động. Frac biến thời gian thành vòng lặp.",
                 "Đây là nơi toán bắt đầu trở thành art direction. Khi Lead nói Flare cần mạnh hơn, không phải lúc nào cũng là tăng Intensity. Có thể cần làm Falloff gắt hơn bằng Power, giữ core nhỏ hơn bằng SmoothStep, hoặc tách core và glow thành hai curve khác nhau.",
                 "Nếu học node theo tên, Power chỉ là lũy thừa. Nếu học theo visual, Power là cách nén hoặc mở phân bố sáng. Khi `x^2`, vùng midtone bị kéo xuống, core sáng nhỏ lại. Khi `sqrt(x)`, vùng midtone được nâng lên, mask rộng và mềm hơn."
             ]),
            ("SmoothStep là dao mổ của threshold",
             [
                 "Step tạo ranh giới cứng: dưới ngưỡng là 0, trên ngưỡng là 1. SmoothStep tạo ranh giới mềm trong một khoảng. Với Dissolve, SmoothStep giúp bạn kiểm soát edge width. Với Foam, nó giúp bọt xuất hiện từ vùng giao nhau. Với Ring, nó giúp dải sáng có biên đọc được mà không quá răng cưa.",
                 "Công thức quan trọng là đặt hai ngưỡng: bắt đầu và kết thúc. Nếu hai ngưỡng quá gần, cạnh sắc. Nếu xa, cạnh mềm. Khi đưa Threshold vào giữa hai SmoothStep, bạn có thể tạo Edge Band bằng hiệu của hai vùng chuyển tiếp.",
                 "Lỗi thường gặp là dùng SmoothStep như một phép làm đẹp chung chung. Nếu nguồn Mask bẩn, SmoothStep chỉ làm bẩn mềm hơn. Nếu cần Edge sắc mà source quá noise, hãy xử lý source trước: remap, contrast, blur hoặc chọn Noise phù hợp."
             ]),
            ("Frac, Floor và nhịp lặp",
             [
                 "Frac lấy phần thập phân của một giá trị. Khi đưa Time vào Frac, bạn có một saw wave 0..1 lặp lại. Khi nhân Time với Speed trước khi Frac, bạn điều khiển tốc độ vòng lặp. Đây là nền cho rất nhiều motion procedural: pulse, scanline, repeating ring, slash trail hoặc flipbook manual.",
                 "Floor biến giá trị liên tục thành bậc. Nó hữu ích khi bạn cần chọn ô, chọn frame hoặc tạo pattern theo block. Nhưng Floor dễ tạo giật. Trong VFX, giật có thể là chủ ý, ví dụ digital glitch, hoặc là lỗi, ví dụ fire frame không mượt.",
                 "Một trick thực tế là dùng Frac cho nhịp chính và SmoothStep để làm vùng sáng đi qua. Bạn không cần Texture animation phức tạp để tạo scanline hoặc shockwave cơ bản. Bạn chỉ cần biết thời gian đang nằm ở đâu trong vòng lặp."
             ]),
        ],
        ("Ghi nhớ", "Trước khi thêm Texture mới, hãy hỏi liệu vấn đề có thể giải quyết bằng cách uốn lại mask 0..1 đang có không.", False),
        [
            "Một Flare thiếu lực có thể không cần thêm bloom. Hãy vẽ core riêng bằng `pow(mask, 4)` và glow riêng bằng `pow(mask, 0.5)`. Hai lớp này cho phép bạn giữ shape đọc được mà vẫn có vùng sáng mềm.",
            "Một Dissolve edge quá đều có thể thêm Noise nhỏ vào Threshold, nhưng nếu Edge Band chưa được tạo đúng bằng SmoothStep, Noise chỉ làm nó bẩn hơn."
        ],
        [
            "core = pow(radialMask, CorePower)",
            "glow = pow(radialMask, GlowPower)",
            "edge = smoothstep(t - w, t, noise) - smoothstep(t, t + s, noise)",
            "final = core * CoreColor + glow * GlowColor + edge * EdgeColor"
        ],
    ),
    make_chapter(
        "Distance, SDF và tư duy hình học 2D",
        "mask_to_edge.png",
        "Distance biến UV thành hình học. Từ đó bạn có circle, ring, capsule, slash và vùng ảnh hưởng.",
        [
            ("Distance là cách hỏi một điểm cách một hình bao xa",
             [
                 "Trong UV space, mỗi pixel có một vị trí. Nếu lấy khoảng cách từ vị trí đó tới tâm 0.5,0.5, bạn có radial distance. Khi remap distance, bạn có circle mask. Khi lấy `abs(distance - radius)`, bạn có ring. Khi so sánh distance với một width, bạn có vùng ảnh hưởng.",
                 "Đây là nền của rất nhiều VFX không cần Texture: portal ring, shockwave, radial wipe, target indicator, aura, shield hit, expanding pulse. Texture có thể làm nó organic hơn, nhưng hình học gốc đến từ distance.",
                 "SDF, hay Signed Distance Field, đi xa hơn: nó không chỉ nói khoảng cách, mà còn nói bên trong hay bên ngoài hình. Một SDF tốt cho phép bạn tạo edge, outline, bevel, glow và mask rất sạch bằng vài phép SmoothStep."
             ]),
            ("Ring không phải chỉ là hình tròn",
             [
                 "Một ring production thường có nhiều lớp. Lớp đầu là hình học sạch: distance tới radius. Lớp thứ hai là breakup: Noise làm cạnh không đều. Lớp thứ ba là motion: polar UV hoặc panner làm texture chạy quanh vòng. Lớp thứ tư là hierarchy: core sáng, outer glow, particle sparks.",
                 "Nếu bạn bắt đầu bằng Noise, ring dễ bị bẩn. Nếu bắt đầu bằng distance, bạn có cấu trúc đọc được rồi mới thêm organic detail. Đây là nguyên tắc Visual First, Logic Second: hình lớn phải đọc trước, chi tiết nhỏ đến sau.",
                 "Khi ring bị seam ở góc polar, nguyên nhân thường là atan2 tạo giá trị nhảy từ 1 về 0. Nếu Texture chạy ngang qua seam, nó sẽ lộ đường cắt. Có thể giấu seam bằng đặt nó sau camera, dùng texture tile sạch, hoặc xử lý blending quanh seam."
             ]),
            ("Từ circle tới capsule, slash và beam",
             [
                 "Distance không chỉ dùng cho vòng tròn. Nếu tính khoảng cách tới một line segment, bạn có capsule SDF. Capsule rất hữu ích cho slash, beam, laser hoặc trail vì nó cho hình có đầu tròn và thân dài. Nếu thêm taper theo trục dọc, slash sẽ có hướng và lực.",
                 "Một slash stylized tốt thường không bắt đầu bằng noise đẹp. Nó bắt đầu bằng silhouette rõ: thân chính, đầu nhọn hoặc đầu tròn, taper, edge band. Sau đó mới thêm breakup, color ramp và timing. SDF giúp dựng silhouette này bằng toán thay vì vẽ quá nhiều texture riêng.",
                 "Trong engine, SDF cũng hữu ích cho UI VFX, decal, ground indicator và hit area. Khi bạn hiểu Distance, bạn có thể tạo nhiều hình sạch, resolution-independent và dễ parameterize hơn texture bitmap."
             ]),
        ],
        ("Lỗi thường gặp", "Dùng Noise quá sớm làm mất silhouette. Hãy dựng shape sạch bằng Distance trước, rồi dùng Noise để phá cạnh có kiểm soát.", True),
        [
            "Một Portal nhìn bẩn thường không phải vì thiếu texture. Nó thiếu ring geometry rõ. Nếu ring chính chưa đọc ở grayscale, thêm swirl texture chỉ làm mắt khó hiểu hơn.",
            "Một Slash bị yếu thường do taper và body không rõ. Hãy dựng SDF capsule hoặc wedge trước, test black-white, rồi mới thêm color và emissive."
        ],
        [
            "p     = UV - 0.5",
            "r     = length(p)",
            "ring  = 1 - smoothstep(Width, Width + Soft, abs(r - Radius))",
            "edge  = ring * noise"
        ],
    ),
    make_chapter(
        "Polar UV, góc và chuyển động xoáy",
        "subuv_derived_uv.png",
        "Polar UV đổi mặt phẳng vuông thành radius và angle. Đây là nền cho portal, vortex, swirl và radial slash.",
        [
            ("Cartesian và Polar là hai cách nhìn cùng một điểm",
             [
                 "UV bình thường là Cartesian: mỗi điểm có U và V. Polar nhìn điểm bằng hai giá trị khác: khoảng cách tới tâm và góc quanh tâm. Trong Shader, radius thường đến từ `length(UV - center)`, angle thường đến từ `atan2(y, x)`. Khi có radius và angle, bạn có thể tạo motion quanh tâm dễ hơn nhiều.",
                 "Portal, vortex, shockwave và magic circle đều dùng logic này. Texture có thể chạy theo radius để đi từ trong ra ngoài, hoặc chạy theo angle để xoay quanh vòng. Nếu cộng Time vào angle, pattern quay. Nếu cộng Time vào radius, pattern chạy ra/vào tâm.",
                 "Điểm cần nhớ là Polar UV không làm hình tự nhiên hơn. Nó chỉ đổi không gian điều khiển. Hình đẹp hay không phụ thuộc vào mask, texture, speed, falloff và cách bạn xử lý seam."
             ]),
            ("Seam là cái giá của angle",
             [
                 "Angle thường được normalize về 0..1. Nhưng một vòng tròn phải có điểm nối, nơi giá trị nhảy từ 1 về 0. Nếu Texture hoặc gradient không tile sạch, seam sẽ lộ. Đây là lý do nhiều portal nhìn có một đường cắt rất khó chịu.",
                 "Có nhiều cách xử lý. Bạn có thể đặt seam ở phía ít thấy, dùng texture tile theo trục angle, hoặc tạo hai sample lệch seam rồi blend. Với một số effect, seam có thể giấu bằng particle, flare hoặc mesh che. Nhưng điều quan trọng là biết seam đến từ toán, không phải từ Texture xấu.",
                 "Khi debug, hãy output angle ra grayscale. Bạn sẽ thấy một gradient vòng quanh và một đường nhảy. Output radius ra grayscale để kiểm tra tâm có đúng không. Hai hình debug này thường tìm ra lỗi nhanh hơn nhìn final có màu."
             ]),
            ("Twirl và swirl là biến dạng tọa độ",
             [
                 "Twirl thường được làm bằng cách xoay UV theo radius: điểm gần tâm xoay nhiều, điểm xa tâm xoay ít. Về mặt tư duy, bạn không xoay Texture. Bạn xoay câu hỏi gửi tới Texture. Pattern bị vặn vì UV bị vặn.",
                 "Một swirl tốt cần falloff. Nếu mọi vùng xoay như nhau, nó chỉ là rotation. Nếu falloff quá mạnh, tâm bị nát. Nếu không clamp, UV có thể kéo quá xa và tạo sampling lạ. Hãy kiểm soát vùng ảnh hưởng bằng radial mask trước khi cộng distortion.",
                 "Với black hole hoặc vortex, swirl nên đi cùng hierarchy: tâm tối hoặc sáng rõ, ring chuyển động, dust hoặc sparks cho scale, distortion nhẹ trên background nếu renderer cho phép. Một swirl texture đơn lẻ thường chưa đủ tạo cảm giác hút."
             ]),
        ],
        ("Ghi nhớ", "Polar UV nên được debug bằng hai view riêng: Radius và Angle. Nếu một trong hai sai, đừng chỉnh Texture vội.", False),
        [
            "Một vortex có đường cắt lộ rõ thường do angle seam. Đổi texture không giải quyết gốc. Hãy kiểm tra angle grayscale và xem texture có tile ở biên 0/1 không.",
            "Một portal xoay quá giả thường do toàn bộ texture rotate đều. Hãy làm inner swirl nhanh hơn outer ring, hoặc dùng radius để điều khiển speed theo khoảng cách."
        ],
        [
            "p      = UV - 0.5",
            "radius = length(p)",
            "angle  = atan2(p.y, p.x) / TWO_PI + 0.5",
            "polar  = float2(angle + Time * Speed, radius)"
        ],
    ),
    make_chapter(
        "Vector Math - Dot, Cross và hướng nhìn",
        "v9_space_vectors.png",
        "Dot Product biến hướng thành số. Cross Product tạo hướng vuông góc. Đây là nền cho Fresnel, rim, alignment và space reasoning.",
        [
            ("Dot Product là câu hỏi hai hướng giống nhau đến mức nào",
             [
                 "Dot Product giữa hai vector normalized trả về -1..1. Nếu hai hướng cùng chiều, kết quả gần 1. Nếu vuông góc, gần 0. Nếu ngược chiều, gần -1. Trong Material, khi dùng `dot(Normal, ViewDir)`, bạn đang hỏi bề mặt quay về phía camera nhiều hay ít.",
                 "Fresnel thực chất đến từ câu hỏi này. Bề mặt nhìn thẳng có facing cao. Cạnh viền có facing thấp. Lấy `1 - facing`, bạn có rim. Thêm Power, rim sắc hơn hoặc mềm hơn. Khi hiểu Dot, Fresnel không còn là node ma thuật.",
                 "Dot cũng dùng để đo alignment với hướng gió, hướng light giả, hướng beam hoặc hướng impact. Nếu particle cần sáng hơn khi bay cùng một hướng, Dot có thể biến hướng bay thành intensity."
             ]),
            ("Normalize là điều kiện để Dot có ý nghĩa ổn định",
             [
                 "Nếu vector không normalized, Dot không chỉ đo góc mà còn bị ảnh hưởng bởi độ dài vector. Đây là lỗi âm thầm. Một Direction Vector từ dữ liệu tùy ý có thể làm rim hoặc alignment thay đổi theo scale, không theo hướng.",
                 "Normalize đưa vector về độ dài 1. SafeNormalize còn bảo vệ trường hợp vector gần zero. Trong VFX, vector zero xuất hiện nhiều hơn bạn nghĩ: velocity lúc spawn, direction chưa set, hoặc offset bằng nhau. Nếu normalize vector zero, kết quả có thể bất ổn.",
                 "Khi một effect hướng theo velocity bị giật ở frame đầu, hãy kiểm tra velocity có đủ lớn để normalize không. Có thể cần fallback direction hoặc delay một frame."
             ]),
            ("Cross Product và hệ trục phụ",
             [
                 "Cross Product tạo vector vuông góc với hai vector đầu vào. Trong shader VFX, nó hữu ích khi bạn cần dựng basis: một hướng chính, một hướng ngang, một hướng dọc. Beam, ribbon, billboard hoặc procedural slash có thể cần hệ trục như vậy.",
                 "Ví dụ, nếu bạn có beam direction và world up, Cross có thể tạo side vector. Nhưng nếu beam direction gần song song world up, Cross sẽ yếu. Đây là lý do cần fallback hoặc chọn up vector khác khi dựng basis.",
                 "CreateThirdOrthogonalVector và MakeVectorsOrthogonal trong Unreal là các node cùng họ tư duy này. Chúng giúp tạo hoặc sửa hệ vector vuông góc để dữ liệu không bị méo khi dùng cho orientation hoặc deformation."
             ]),
        ],
        ("Lỗi thường gặp", "Dot chỉ đáng tin khi hai vector đã Normalize. Nếu không, bạn đang trộn góc và độ dài trong cùng một phép đo.", True),
        [
            "Một Shield Rim dày mỏng thất thường theo scale có thể do Normal hoặc ViewDir không ở cùng space, hoặc vector chưa normalized. Hãy debug facing trước khi chỉnh Fresnel Power.",
            "Một Beam procedural bị lật khi dựng side vector có thể do Cross Product gần zero. Trường hợp direction gần song song up vector cần fallback."
        ],
        [
            "facing = saturate(dot(normalize(N), normalize(V)))",
            "rim    = pow(1 - facing, RimPower)",
            "side   = normalize(cross(BeamDir, WorldUp))",
            "up     = cross(side, BeamDir)"
        ],
    ),
    make_chapter(
        "Matrix và Space Transform - đổi hệ quy chiếu để giải đúng bài toán",
        "v9_space_vectors.png",
        "Object, World, View, Tangent và Clip Space là những cách đặt câu hỏi khác nhau về cùng một điểm.",
        [
            ("Space là bối cảnh của một con số",
             [
                 "Một vector `float3(0,0,1)` không có nghĩa đầy đủ nếu bạn không biết nó đang ở space nào. Trong Object Space, nó có thể là trục Z local của mesh. Trong World Space, nó là hướng lên thế giới. Trong Tangent Space, nó là normal local của texture. Cùng ba con số, ý nghĩa hoàn toàn khác.",
                 "Nhiều lỗi shader nâng cao đến từ trộn space. Bạn lấy Normal ở Tangent Space nhưng so với ViewDir ở World Space, kết quả Dot có thể vẫn ra số, nhưng số đó không còn đại diện cho góc thật. Shader không báo lỗi vì phép toán hợp lệ. Visual báo lỗi bằng cách nhìn sai.",
                 "Khi đọc Material phức tạp, hãy ghi space bên cạnh dữ liệu: `NormalWS`, `ViewDirWS`, `PositionOS`, `UV`, `ScreenUV`. Việc đặt tên này không phải làm đẹp. Nó ngăn bạn nối nhầm hai hệ quy chiếu."
             ]),
            ("Matrix là công cụ chuyển đổi space",
             [
                 "Matrix có thể được hiểu thực dụng là một bảng mô tả cách đổi tọa độ từ space này sang space khác. Object-to-World đưa điểm từ local mesh ra thế giới. World-to-View đưa thế giới vào camera. View-to-Clip chuẩn bị dữ liệu cho rasterization.",
                 "Trong Material Graph, bạn thường dùng node TransformVector hoặc TransformPosition thay vì tự viết matrix. Nhưng hiểu matrix giúp bạn biết vì sao Transform sai type lại tạo lỗi. Vector chỉ mang hướng, không mang vị trí. Position mang cả translation. Normal cần xử lý cẩn thận hơn khi scale không đều.",
                 "InverseTransformMatrix hoặc Transform3x3Matrix trong danh sách Math node tồn tại vì có lúc bạn cần tự điều khiển phép đổi này. Nhưng với production Material, quy tắc tốt là dùng node high-level trước, chỉ dùng matrix thủ công khi bạn biết rõ dữ liệu đang ở đâu."
             ]),
            ("Screen Space và những hiệu ứng dính camera",
             [
                 "Screen Space dùng tọa độ sau khi hình đã được chiếu lên màn hình. Nó hữu ích cho refraction, screen distortion, heat haze, soft particles, edge detect hoặc post-process style effect. Nhưng nó cũng dễ làm effect dính camera hoặc trượt theo viewport.",
                 "Một distortion screen-space đẹp trong preview có thể khó chịu trong gameplay nếu nó làm UI méo, làm background rung quá mạnh hoặc không ổn định theo resolution. Vì vậy, screen-space effect cần test với camera thật, FOV thật và platform thật.",
                 "Khi một hiệu ứng cần nằm trên bề mặt mesh, ưu tiên Object/World/UV Space. Khi nó cần can thiệp hình ảnh đã render, mới dùng Screen Space. Chọn space đúng thường giảm một nửa độ phức tạp của graph."
             ]),
        ],
        ("Ghi nhớ", "Đừng chỉ hỏi dữ liệu là vector hay scalar. Hãy hỏi nó đang ở space nào. Space sai làm shader sai theo cách rất khó debug.", False),
        [
            "Một Fresnel trên mesh scale không đều có thể nhìn lạ nếu normal transform không đúng. Nếu rim không bám theo camera như mong đợi, kiểm tra Normal Space và View Direction trước.",
            "Một WPO đẩy vertex theo World Up sẽ khác WPO đẩy theo Vertex Normal. Cả hai đều đúng toán, nhưng phục vụ ý đồ visual khác nhau."
        ],
        [
            "ObjectPosition -> TransformPosition(ObjectToWorld) -> WorldPosition",
            "TangentNormal  -> TransformVector(TangentToWorld)  -> NormalWS",
            "ViewVectorWS   -> normalize(CameraPosWS - WorldPos)"
        ],
    ),
    make_chapter(
        "GPU và CPU - cơ bắp thật phía sau một pixel",
        "v9_shader_complexity_overdraw.png",
        "Một Effect không chỉ chạy trong Material. Nó đi qua CPU setup, renderer, GPU rasterization, pixel shading và post-process.",
        [
            ("CPU chuẩn bị công việc, GPU nghiền số lượng lớn",
             [
                 "Trong realtime rendering, CPU thường lo chuẩn bị: game logic, culling, gửi draw call, cập nhật Niagara, set parameter, binding texture và command buffer. GPU nhận các lệnh đó rồi xử lý lượng lớn vertex và pixel song song. Hiểu vai trò này giúp bạn không tối ưu sai nơi.",
                 "Một Material quá phức tạp thường là gánh nặng GPU. Một Niagara system spawn quá nhiều emitter, quá nhiều renderer hoặc quá nhiều draw call có thể làm CPU mệt. Một effect đắt có thể không nằm ở shader instruction mà nằm ở số lượng particle, sort, bounds, hoặc renderer state.",
                 "Khi profile, đừng chỉ nhìn một chỉ số. Instruction count nói một phần. Shader Complexity nói một phần. Draw call nói một phần. GPU time và CPU time mới cho biết bottleneck thật đang nằm ở đâu."
             ]),
            ("Pixel cost phụ thuộc vào diện tích màn hình",
             [
                 "Một shader 80 instruction trên một mesh nhỏ có thể rẻ. Một shader 20 instruction trên smoke card phủ nửa màn hình và chồng 50 lớp có thể rất đắt. Pixel shader chạy theo số pixel được rasterize, không theo cảm giác bạn thấy effect quan trọng hay không.",
                 "Đây là lý do Overdraw nguy hiểm với Translucent VFX. Nhiều lớp alpha chồng nhau nghĩa là cùng một pixel bị shade nhiều lần. GPU không thể đơn giản bỏ qua pixel phía sau như opaque depth test trong nhiều trường hợp translucent.",
                 "Tối ưu VFX thường bắt đầu từ silhouette và bounds: card nhỏ hơn, alpha crop tốt hơn, particle ít hơn, lifetime ngắn hơn, LOD đúng hơn. Sau đó mới đến giảm Texture Fetch hoặc node math."
             ]),
            ("Bandwidth đôi khi đắt hơn phép toán",
             [
                 "Người mới thường sợ phép Multiply, Add, Power hơn Texture Sample. Nhưng trên GPU hiện đại, băng thông texture và cache miss có thể là chi phí lớn. Một vài phép toán rẻ có thể đáng đổi để giảm số texture fetch hoặc giảm kích thước texture.",
                 "Điều này không có nghĩa là Power miễn phí hoặc noise procedural luôn rẻ. Nó có nghĩa là bạn phải nghĩ theo bottleneck. Mobile có thể nhạy với bandwidth và overdraw. Console/PC có ngân sách khác nhưng vẫn không vô hạn. VR còn nghiêm hơn vì độ phân giải và frame rate cao.",
                 "Một shader tốt là shader biết mình đang trả tiền ở đâu. Nếu bạn dùng ba texture chỉ để lấy ba mask grayscale, hãy cân nhắc packing. Nếu bạn dùng procedural noise đắt cho mọi pixel, hãy cân nhắc bake. Không có câu trả lời cố định; có tradeoff."
             ]),
        ],
        ("Cảnh báo performance", "Đừng tối ưu bằng cảm giác. Hãy xác định bottleneck: CPU setup, draw call, particle count, overdraw, texture bandwidth hay shader instruction.", True),
        [
            "Một Win Effect iGaming có rất nhiều sparkle nhỏ có thể bị CPU hoặc draw call nặng hơn shader. Gộp atlas, giảm renderer, dùng pooling và giữ material đơn giản có thể hiệu quả hơn viết shader tối ưu cực đoan.",
            "Một smoke impact lớn thường bị GPU pixel cost. Cắt alpha card, giảm overdraw và LOD particle theo camera distance có thể tiết kiệm hơn xóa vài node Add."
        ],
        [
            "TotalCost ≈ DrawCalls_CPU + Particles_CPU + PixelsCovered * Layers * ShaderCost",
            "For translucent VFX:",
            "Pixel area and layer count often matter before tiny math nodes."
        ],
    ),
    make_chapter(
        "Derivatives, Mip và Aliasing",
        "v9_power_shaping_flare.png",
        "ddx, ddy, fwidth và Mip Mapping quyết định nhiều thứ về độ sạch của pattern nhỏ.",
        [
            ("Aliasing là khi chi tiết vượt quá khả năng lấy mẫu",
             [
                 "Khi một pattern có chi tiết nhỏ hơn pixel màn hình, GPU không thể biểu diễn nó sạch. Kết quả là răng cưa, nhấp nháy, moiré hoặc noise crawl. VFX rất dễ gặp lỗi này vì thường có ring mỏng, line nhỏ, sparkle, scanline, dissolve edge và texture noise tần số cao.",
                 "Mip Mapping giúp Texture chọn phiên bản mờ hơn khi ở xa hoặc khi bị thu nhỏ. Nhưng procedural pattern không tự có mip như Texture. Nếu bạn tự tạo stripe bằng Frac hoặc Step, bạn cần nghĩ tới anti-aliasing bằng derivative hoặc làm edge mềm hơn.",
                 "Một effect nhìn đẹp trong screenshot có thể nhấp nháy trong motion. Vì vậy, hãy test bằng camera di chuyển. Aliasing thường lộ khi có chuyển động, không lộ hết trong frame tĩnh."
             ]),
            ("fwidth là thước đo pixel footprint",
             [
                 "Trong shader code, `ddx` và `ddy` cho biết giá trị thay đổi bao nhiêu giữa các pixel lân cận theo trục màn hình. `fwidth(x)` thường là `abs(ddx(x)) + abs(ddy(x))`. Nó cho bạn biết một giá trị thay đổi nhanh thế nào trên màn hình.",
                 "Khi tạo procedural line, bạn có thể dùng fwidth để làm width thích nghi theo pixel. Edge không còn quá sắc khi zoom xa, giảm flicker. Đây là lý do nhiều SDF text hoặc shape shader dùng derivative để anti-alias.",
                 "Trong Unreal Material Graph, bạn có node DDX/DDY hoặc custom HLSL khi cần. Nhưng hãy dùng có mục đích. Derivative chỉ có ý nghĩa trong pixel shader và có ràng buộc với control flow. Nó không phải thuốc chữa mọi aliasing."
             ]),
            ("Mip bias và Texture detail trong VFX",
             [
                 "VFX đôi khi cần Texture sắc hơn hoặc mềm hơn mặc định. Mip Bias có thể ép texture chọn mip khác. Nhưng lạm dụng negative bias làm texture sắc quá và nhấp nháy. Positive bias làm mềm, có thể mất chi tiết nhưng ổn định hơn.",
                 "Với mask dữ liệu, mip có thể làm edge thay đổi theo distance. Một dissolve edge ở gần đẹp nhưng ở xa quá mềm có thể do mip. Một fire noise ở xa crawl có thể do texture quá chi tiết và mipping chưa phù hợp.",
                 "Production rule: nếu pattern nhỏ hơn pixel, đừng cố giữ nó sắc bằng mọi giá. Một effect ổn định thường tốt hơn một effect sắc trong screenshot nhưng rung khi chơi."
             ]),
        ],
        ("Ghi nhớ", "Anti-aliasing trong shader là giữ visual ổn định khi camera đổi, không chỉ làm cạnh đẹp trong một frame tĩnh.", False),
        [
            "Một magic circle có line mỏng nhấp nháy ở xa cần được xử lý bằng width theo fwidth hoặc LOD variant. Tăng texture resolution không giải quyết nếu line vẫn nhỏ hơn pixel.",
            "Một dissolve noise bị crawl khi camera di chuyển có thể cần mip phù hợp hoặc noise tần số thấp hơn, không phải thêm distortion."
        ],
        [
            "d = abs(distanceToLine)",
            "aa = fwidth(d)",
            "line = 1 - smoothstep(Width, Width + aa, d)"
        ],
    ),
    make_chapter(
        "HLSL/GLSL - khi Node Graph không còn đủ rõ",
        "v11_realtime_contract_simple.png",
        "Code shader không thay thế tư duy visual. Nó chỉ là cách diễn đạt gọn hơn khi logic lặp, dài hoặc cần kiểm soát chính xác.",
        [
            ("Uniform, varying và câu chuyện dữ liệu đi vào shader",
             [
                 "Trong GLSL hay HLSL, Uniform là dữ liệu giống nhau cho nhiều pixel hoặc draw call: thời gian, màu, texture, camera position, parameter. Varying hoặc interpolator là dữ liệu được truyền từ vertex tới pixel và nội suy qua bề mặt: UV, normal, vertex color, custom data.",
                 "Trong Unreal Material Graph, nhiều thứ đã được bọc thành node. Scalar Parameter giống uniform. Texture Object và Sampler là dữ liệu texture. TexCoord là varying. Vertex Color là dữ liệu từ mesh. Hiểu những khái niệm này giúp bạn đọc code và node như hai mặt của cùng một hệ thống.",
                 "Khi Material cần giao tiếp với Niagara, Dynamic Parameter hoặc Particle Color cũng là contract dữ liệu. Code hay node đều phải tôn trọng contract đó: channel nào có ý nghĩa gì, range là bao nhiêu, ai chịu trách nhiệm animate."
             ]),
            ("Function nhỏ tốt hơn một Custom Node khổng lồ",
             [
                 "Custom Node HLSL thường hấp dẫn vì nó làm Graph gọn ngay lập tức. Nhưng nếu code chứa toàn bộ logic trong một khối dài, bạn chỉ chuyển spaghetti từ dây nối sang text. Một function tốt nên nhỏ, có input/output rõ, tên biến theo ý đồ visual và có comment ngắn cho phần khó.",
                 "Ví dụ `RingMask(uv, radius, width, softness)` rõ hơn một khối code tên `CustomExpression0`. `EdgeBand(mask, threshold, width, softness)` rõ hơn việc rải nhiều SmoothStep khắp graph. Code nên làm contract rõ hơn, không giấu logic.",
                 "Với team production, debug view rất quan trọng. Nếu Custom Node trả về final color duy nhất, artist khó biết lỗi nằm ở mask, edge hay distortion. Hãy cho phép xuất intermediate bằng Static Switch hoặc debug parameter khi logic phức tạp."
             ]),
            ("Loop, sampling và giới hạn thực tế",
             [
                 "Một số visual khó làm đẹp bằng node thuần: multi-sample blur, ripple accumulation, raymarch ngắn, repeated ring, bit packing, hoặc hash noise. HLSL diễn đạt các vòng lặp này gọn hơn. Nhưng loop không miễn phí. Mỗi iteration có thể thêm sample và math cho mọi pixel.",
                 "Nếu loop count là constant nhỏ, compiler có thể unroll. Nếu loop phụ thuộc runtime value, performance và compatibility cần kiểm tra kỹ hơn. Trong Material dùng cho production rộng, hãy tránh biến một effect nhỏ thành shader không dự đoán được.",
                 "Một nguyên tắc thực dụng: dùng code khi nó làm logic ngắn hơn, rõ hơn hoặc khả thi hơn. Không dùng code chỉ để trông senior hơn. Node Graph dễ đọc với nhiều artist; HLSL mạnh nhưng cần trách nhiệm debug cao hơn."
             ]),
        ],
        ("Cảnh báo production", "Custom Node không nên là hộp đen. Hãy đặt tên input rõ, clamp range quan trọng, và có đường debug intermediate.", True),
        [
            "Một radial blur cho portal có thể cần loop sample quanh tâm. Làm bằng node sẽ rất rối, HLSL gọn hơn. Nhưng loop 16 sample trên full-screen translucent effect có thể đắt. Cần radius giới hạn, quality parameter hoặc variant thấp.",
            "Một hash noise nhỏ trong shader có thể thay texture sample, nhưng nếu dùng trên mọi pixel lớn và nhiều layer, chi phí ALU cũng tăng. Hãy profile thay vì mặc định code là rẻ."
        ],
        [
            "float RingMask(float2 uv, float radius, float width, float soft)",
            "{",
            "    float r = length(uv - 0.5);",
            "    return 1 - smoothstep(width, width + soft, abs(r - radius));",
            "}"
        ],
    ),
    make_chapter(
        "Blend Mode và Sorting - nơi Material gặp Renderer",
        "v9_failure_depthfade_before_after.png",
        "Blend Mode quyết định pixel của bạn đi vào framebuffer như thế nào. Đây là quyết định render, không chỉ là lựa chọn mỹ thuật.",
        [
            ("Opaque, Masked, Translucent và Additive không cùng một thế giới",
             [
                 "Opaque ghi depth rõ và thường render hiệu quả. Masked cũng có cutout và có thể ghi depth theo pixel được giữ. Translucent thường không ghi depth theo cách opaque, cần sort và dễ overdraw. Additive cộng sáng vào background, rất hợp flare, spark, magic streak nhưng yếu trên nền sáng.",
                 "Khi chọn Blend Mode, bạn đang chọn luật vật lý giả cho effect. Smoke cần alpha mềm nên thường Translucent. Leaf cutout có thể Masked. Spark và energy streak thường Additive. Shield có thể phối hợp Translucent, Additive rim và depth interaction tùy style.",
                 "Đừng chọn Translucent chỉ vì muốn fade. Nếu effect có thể dùng Masked với dither hoặc temporal fade, nó có thể rẻ và ổn định hơn. Nhưng Masked có cạnh cứng và cần art direction phù hợp."
             ]),
            ("Sorting là lỗi production rất thật",
             [
                 "Translucent sorting không đơn giản khi nhiều card giao nhau. Renderer phải quyết định cái nào vẽ trước, nhưng per-object hoặc per-particle sorting không phải lúc nào cũng đúng ở từng pixel. Smoke, glass, magic field hoặc overlapping ribbons đều có thể bị pop hoặc sai thứ tự.",
                 "Một số lỗi nhìn như Material sai nhưng thực ra là sorting. Alpha bị cắt lạ, layer phía sau hiện lên trước, ribbon tự chồng sai. Nếu bạn chỉnh shader mãi không hết, hãy kiểm tra bounds, sort mode, translucency sort priority và cách particle spawn trong không gian.",
                 "Production workaround đôi khi rất thực dụng: giảm overlap, chia effect thành layer, dùng mesh khác, đổi camera-facing, hoặc bake một phần vào flipbook. Không phải mọi lỗi sorting đều được sửa đẹp bằng shader."
             ]),
            ("DepthFade là cầu nối mềm với Scene",
             [
                 "DepthFade so sánh độ sâu của pixel translucent với Scene Depth để làm mềm giao tuyến. Nó cực kỳ hữu ích cho smoke, dust, splash, fire card và magic cloud. Không có DepthFade, card cắt vào sàn như giấy. Có DepthFade, opacity giảm dần gần geometry.",
                 "Nhưng DepthFade không sửa alpha xấu. Nếu card quá rộng, texture không có silhouette, hoặc fade distance sai scale, effect vẫn giả. DepthFade chỉ xử lý giao tuyến với scene, không thay thế shape design.",
                 "Khi debug, hãy nhìn effect sát sàn, sát tường, camera thấp và camera cao. Fade distance đúng ở một góc chưa chắc đúng ở mọi tình huống."
             ]),
        ],
        ("Lỗi thường gặp", "Tăng opacity để sửa translucent yếu có thể làm overdraw và sorting tệ hơn. Hãy kiểm tra blend mode, background và hierarchy trước.", True),
        [
            "Một smoke card nhìn tốt trong material preview nhưng cắt vào terrain trong level cần DepthFade. Nếu thêm DepthFade mà smoke biến mất quá sớm, fade distance quá lớn hoặc scale scene không đúng.",
            "Một additive flare đẹp trên nền đen nhưng mất trên nền trời sáng cần core/hierarchy, không chỉ intensity. Additive phụ thuộc background nhiều hơn artist mới thường nghĩ."
        ],
        [
            "softAlpha = Alpha * DepthFade(FadeDistance)",
            "additive  = Emissive * softAlpha",
            "translucentColor = lerp(SceneColor, Color, softAlpha)"
        ],
    ),
    make_chapter(
        "Niagara và Material - hợp đồng dữ liệu phải rõ",
        "niagara_material_contract.png",
        "Niagara sinh dữ liệu theo thời gian. Material biến dữ liệu đó thành pixel. Hai bên cần cùng hiểu channel nào có ý nghĩa gì.",
        [
            ("Particle Color không chỉ là màu",
             [
                 "Trong rất nhiều VFX, Particle Color là đường dây sống còn giữa Niagara và Material. RGB thường điều khiển tint, Alpha thường điều khiển fade. Nếu Material không nhân Particle Color hoặc không dùng Alpha vào opacity/emissive, curve trong Niagara sẽ không có tác dụng.",
                 "Đây là lỗi kinh điển: artist chỉnh Color Over Life, Alpha Over Life rất kỹ nhưng effect không đổi. Nguyên nhân không nằm ở Niagara. Material đã không đọc dữ liệu đó. Vì vậy, khi mở một Material dùng cho Sprite Renderer, kiểm tra Particle Color nên là bước đầu tiên.",
                 "Với Additive, Alpha có thể không đi vào Opacity theo cách translucent. Nó có thể scale Emissive. Điều quan trọng là Material phải có một path rõ ràng để Niagara fade điều khiển cường độ visual."
             ]),
            ("Dynamic Parameter nên có chuẩn đặt tên",
             [
                 "Dynamic Parameter cho bốn float. Nếu mỗi Material dùng R/G/B/A theo một nghĩa khác nhau, Niagara setup sẽ trở thành bẫy. Một team production nên có convention theo family: ví dụ R là Dissolve, G là Distortion, B là Edge Intensity, A là Softness hoặc Power.",
                 "Không cần một chuẩn duy nhất cho mọi project, nhưng cần ổn định trong cùng hệ. Fire materials nên dùng cùng contract. Shield materials nên dùng cùng contract. Khi artist copy emitter, họ không phải đoán channel nào điều khiển gì.",
                 "Trong Material, hãy đổi tên parameter và comment rõ. Trong Niagara, hãy dùng module hoặc naming tương ứng. Contract dữ liệu là tài liệu sống trong graph, không chỉ nằm trong wiki."
             ]),
            ("Ai sở hữu timing?",
             [
                 "Một lỗi thiết kế phổ biến là cả Niagara và Material đều animate cùng một thứ. Niagara fade alpha theo lifetime, Material cũng dùng Particle Relative Time để fade, rồi kết quả bị nhân đôi. Effect biến mất quá nhanh và không ai biết curve nào chịu trách nhiệm.",
                 "Hãy quyết định ownership. Nếu Niagara sở hữu timing, Material nhận parameter và thực thi. Nếu Material sở hữu một logic nội bộ như pulse hoặc dissolve loop, Niagara chỉ đưa trigger hoặc speed. Càng ít nơi cùng điều khiển một biến, debug càng dễ.",
                 "Khi effect phức tạp, hãy viết một dòng contract: Niagara gửi gì, Material trả lời bằng gì, Parameter range là bao nhiêu, default value là gì. Một phút ghi chú có thể tiết kiệm vài giờ sửa."
             ]),
        ],
        ("Ghi nhớ", "Mỗi Material dùng với Niagara nên có hợp đồng dữ liệu: Particle Color dùng ở đâu, Dynamic Parameter channel nào làm gì, ai sở hữu timing.", False),
        [
            "Một fire flipbook không fade theo lifetime thường do Material bỏ qua ParticleColor.a. Đừng sửa Niagara curve trước khi kiểm tra Material path.",
            "Một shield hit không đồng bộ giữa các variant có thể do Dynamic Parameter R ở Material này là HitAmount, nhưng ở Material khác là Distortion. Chuẩn hóa channel giúp tránh lỗi này."
        ],
        [
            "ParticleColor.rgb -> Tint",
            "ParticleColor.a   -> Alpha or Emissive scale",
            "Dynamic.R         -> Dissolve",
            "Dynamic.G         -> Distortion",
            "Dynamic.B         -> EdgeIntensity",
            "Dynamic.A         -> Softness"
        ],
    ),
    make_chapter(
        "WPO - khi Material bắt đầu đẩy hình học",
        "v9_space_vectors.png",
        "World Position Offset làm vertex di chuyển trong shader. Nó mạnh, nhưng liên quan trực tiếp tới bounds, shadow và silhouette.",
        [
            ("WPO khác Pixel Shader ở chỗ nó thay đổi vị trí vertex",
             [
                 "World Position Offset chạy ở vertex stage và đẩy vertex khỏi vị trí gốc. Nó dùng được cho wind, wobble, shield pulse, mesh dissolve lift, water wave, energy deformation hoặc impact bulge. Vì nó thay đổi hình học, người xem thấy silhouette thật thay đổi, không chỉ màu pixel.",
                 "Nhưng WPO chỉ di chuyển vertex hiện có. Mesh quá ít subdivisions sẽ không biến dạng mượt. Một plane bốn vertex không thể tạo wave chi tiết bằng WPO nếu không có tessellation hoặc mesh đủ vertex. Đây là lỗi rất phổ biến khi artist thử displacement trên geometry nghèo.",
                 "WPO cũng có quan hệ với bounds. Nếu vertex bị đẩy ra ngoài bounds gốc, mesh có thể bị cull sớm hoặc shadow sai. Khi WPO lớn, hãy kiểm tra bounds scale và camera frustum."
             ]),
            ("Đẩy theo Normal, World Up hay Direction?",
             [
                 "Đẩy theo Vertex Normal tạo cảm giác bề mặt phồng hoặc nổ ra từ chính nó. Đẩy theo World Up tạo cảm giác bốc lên hoặc sóng đứng. Đẩy theo custom direction tạo slash, beam hoặc directional impact. Cùng một noise, hướng đẩy khác nhau tạo visual khác hẳn.",
                 "Vì vậy, trước khi viết node, hãy hỏi movement thuộc về object hay world. Fire mesh có thể bốc theo World Up. Shield hit có thể phồng theo Normal. Ground shockwave có thể đẩy radial theo vector từ tâm impact.",
                 "Nếu direction đến từ vector tính toán, hãy normalize và kiểm tra space. WPO sai space tạo cảm giác mesh trượt lạ khi rotate hoặc scale."
             ]),
            ("WPO và cost không chỉ là instruction",
             [
                 "WPO có thể làm vertex shader nặng hơn, nhưng thường rủi ro production lớn hơn nằm ở bounds, shadow, collision expectation và LOD. Mesh biến dạng nhưng collision không đổi có thể làm gameplay hoặc hit effect nhìn lệch.",
                 "Với VFX, WPO nên được dùng để tăng silhouette hoặc tạo chuyển động mà pixel shader khó làm. Nếu deformation không đọc được từ camera gameplay, có thể nó không đáng chi phí và rủi ro.",
                 "Một debug tốt là tắt color/emissive, chỉ xem silhouette và wireframe. Nếu WPO không đọc ở silhouette, nó có thể chỉ là noise trang trí."
             ]),
        ],
        ("Cảnh báo production", "WPO lớn cần kiểm tra bounds. Mesh biến mất khi camera đổi góc thường không phải do opacity; có thể do bounds không bao phủ vertex offset.", True),
        [
            "Một mesh additive bị biến mất khi WPO đẩy mạnh theo noise có thể cần tăng bounds scale. Nhưng tăng quá nhiều làm culling kém hơn. Hãy cân bằng visual và bounds.",
            "Một water ripple bằng WPO trên plane ít vertex sẽ nhìn như vài mảng gãy. Cần mesh density phù hợp hoặc chuyển ripple sang normal/distortion nếu silhouette không cần đổi."
        ],
        [
            "offsetDir = normalize(VertexNormalWS)",
            "wave      = sin(Time * Speed + WorldPos.x * Frequency) * Amplitude",
            "WPO       = offsetDir * wave"
        ],
    ),
    make_chapter(
        "POM, Height và ảo giác chiều sâu",
        "failure_atlas.png",
        "Parallax Occlusion Mapping tạo cảm giác độ sâu bằng nhiều lần sample height, nhưng không thay đổi silhouette thật.",
        [
            ("Height Map là bản đồ độ cao giả",
             [
                 "Height Map lưu độ cao tương đối trên bề mặt. Trắng thường cao hơn, đen thấp hơn, tùy convention. Khi dùng cho Parallax hoặc POM, shader dịch UV theo view direction để giả lập việc nhìn vào bề mặt có chiều sâu.",
                 "POM mạnh ở vật liệu như đá nứt, lava crack, rune khắc, sci-fi panel hoặc mặt đất chi tiết. Nó giúp bề mặt có depth mà không cần geometry thật. Nhưng nó chỉ là ảo giác trong pixel shader; silhouette ngoài của mesh không đổi.",
                 "Nếu camera nhìn sát cạnh mesh, POM có thể lộ vì biên vẫn phẳng. Nếu effect cần hình học thật thay đổi silhouette, WPO hoặc mesh geometry phù hợp hơn."
             ]),
            ("Sample count là chất lượng đổi lấy chi phí",
             [
                 "POM thường ray step qua height map nhiều lần. Nhiều step hơn thì chính xác hơn nhưng đắt hơn. Step ít có thể stair-step, shimmer hoặc sai depth. Trên mobile hoặc effect lớn, POM phải dùng rất cẩn thận.",
                 "Một chiến lược tốt là giảm chất lượng theo distance hoặc angle. Ở xa, chi tiết parallax không đọc rõ, không cần step cao. Ở góc nhìn gần song song bề mặt, lỗi dễ lộ nhưng chi phí cũng tăng. Đây là tradeoff production.",
                 "Đừng dùng POM chỉ vì nó cao cấp. Nếu một normal map hoặc height-based shading đủ đạt visual, hãy giữ shader đơn giản hơn."
             ]),
            ("POM trong VFX nên phục vụ focal point",
             [
                 "Trong VFX, POM hợp khi nó nằm ở focal area: rune circle dưới chân boss, lava crack gần camera, magic panel hoặc portal frame. Nếu nó nằm trong hiệu ứng nhanh 0.3 giây, người chơi có thể không thấy sự khác biệt.",
                 "Khi dùng POM cho lava hoặc crack, hãy tách mask emissive khỏi height logic. Height tạo cảm giác rãnh sâu, emissive tạo lava glow. Nếu dùng chung một texture mà không kiểm soát range, visual dễ bị lẫn: chỗ cao lại sáng sai hoặc crack quá dày.",
                 "Debug bằng grayscale height trước. Nếu height không đọc rõ ở black-white, POM sẽ không cứu được. Nó chỉ khuếch đại dữ liệu có sẵn."
             ]),
        ],
        ("Ghi nhớ", "POM tạo chiều sâu trong pixel, không tạo silhouette thật. Hãy dùng khi viewer thấy bề mặt, không dùng để thay thế geometry trong mọi trường hợp.", False),
        [
            "Một lava crack nhìn phẳng có thể cần height/POM nếu camera gần. Nhưng một fire burst nhanh chỉ cần emissive crack mask và bloom có thể đủ.",
            "Một rune circle dùng POM quá mạnh có thể rung khi camera xa. Hãy LOD sample count hoặc dùng variant không POM cho khoảng cách xa."
        ],
        [
            "viewTS  = normalize(ViewDirTangent)",
            "height  = SampleHeight(UV)",
            "uvPOM   = ParallaxTrace(UV, viewTS, height, StepCount)",
            "color   = SampleBase(uvPOM)"
        ],
    ),
    make_chapter(
        "Noise - organic không có nghĩa là ngẫu nhiên vô tội vạ",
        "v9_mask_solo_dissolve.png",
        "Noise là texture hoặc hàm tạo biến thiên. Nó cần scale, contrast, motion và vai trò rõ.",
        [
            ("Noise có tần số, không chỉ có hình đẹp",
             [
                 "Khi nói một Noise đẹp, hãy hỏi nó có tần số phù hợp không. Noise tần số thấp tạo mảng lớn, hợp smoke breakup, dissolve vùng rộng, cloud motion. Noise tần số cao tạo chi tiết nhỏ, hợp ember, sparkle, edge roughness. Dùng sai tần số làm effect sai scale.",
                 "Một fire card nhỏ dùng noise quá lớn sẽ nhìn như khối bẩn. Một portal lớn dùng noise quá nhỏ sẽ nhìn phẳng. Scale của noise phải liên quan tới scale của effect và camera gameplay.",
                 "Noise cũng có contrast. Một noise quá nhiều midtone làm threshold khó kiểm soát. Một noise quá black-white làm dissolve giật. Hãy remap noise trước khi dùng nó điều khiển opacity hoặc emissive."
             ]),
            ("Layer noise có chủ đích",
             [
                 "Production VFX thường dùng nhiều lớp noise, nhưng mỗi lớp cần vai trò. Một lớp tạo silhouette breakup. Một lớp tạo edge detail. Một lớp tạo motion distortion. Một lớp tạo emissive flicker. Nếu ba lớp noise đều làm cùng một việc, graph chỉ bẩn hơn.",
                 "Khi layer noise, hãy thay scale và speed. Hai noise cùng scale, cùng speed dễ tạo cảm giác pattern trượt đồng loạt. Một noise lớn chậm kết hợp noise nhỏ nhanh thường tự nhiên hơn.",
                 "Đừng để noise phá hierarchy. Nếu noise làm core flare biến mất, làm slash mất shape hoặc làm ring không đọc, nó đang phục vụ chi tiết nhưng phá hình lớn."
             ]),
            ("Procedural noise hay baked noise?",
             [
                 "Procedural noise linh hoạt và không cần texture, nhưng có thể tốn ALU. Baked noise cần texture fetch và memory, nhưng ổn định và dễ art-direct. Trong Unreal Material, nhiều khi texture noise đơn giản là lựa chọn production tốt hơn.",
                 "Nếu noise cần giống nhau giữa nhiều effect, baked texture giúp consistency. Nếu noise cần phản ứng theo world position hoặc tạo pattern không lặp rõ, procedural có lợi. Nếu platform là mobile, hãy profile trước khi chọn.",
                 "Không có câu trả lời tuyệt đối. Câu hỏi đúng là: noise này cần variation gì, có cần animate không, có lặp bị lộ không, cost nằm ở texture hay math, và artist có cần chỉnh nó thường xuyên không."
             ]),
        ],
        ("Lỗi thường gặp", "Thêm noise để làm organic nhưng không giữ shape lớn là cách nhanh nhất biến effect thành đám bẩn.", True),
        [
            "Một dissolve được gọi là chưa organic có thể chỉ cần noise scale phù hợp và edge band rõ, không cần thêm ba noise nữa.",
            "Một smoke card bị lặp pattern có thể cần offset/speed khác nhau cho hai noise layer, hoặc atlas frame khác nhau, thay vì tăng opacity."
        ],
        [
            "large = TextureNoise(UV * 1.5 + Time * 0.05)",
            "small = TextureNoise(UV * 8.0 - Time * 0.20)",
            "mask  = saturate(large * 0.75 + small * 0.25)",
            "edge  = EdgeBand(mask, Threshold, Width, Softness)"
        ],
    ),
    make_chapter(
        "Color, HDR và Bloom - ánh sáng cần hierarchy",
        "v11_imagegen_vfx_atlas.png",
        "Emissive mạnh không tự tạo visual tốt. Mắt cần vùng lõi, vùng chuyển tiếp, màu phụ và nền đọc được.",
        [
            ("HDR cho phép sáng hơn 1, nhưng không miễn trách nhiệm art direction",
             [
                 "Trong Unreal, Emissive có thể vượt quá 1 và đi vào HDR pipeline. Bloom và tonemapper sau đó biến giá trị sáng thành glow trên màn hình. Đây là lý do VFX realtime có thể tạo cảm giác flare, magic, energy rất mạnh.",
                 "Nhưng nếu mọi thứ đều Emissive cao, không còn hierarchy. Người xem thấy một mảng cháy trắng. Một effect tốt thường có core rất sáng nhưng nhỏ, mid glow có màu, outer glow mềm và vài accent. Từng lớp có vai trò.",
                 "Khi debug, hãy tắt Bloom hoặc giảm Exposure để xem shape gốc. Nếu không có bloom mà effect không đọc được gì, bạn đang mượn post-process để che thiếu thiết kế."
             ]),
            ("Palette không chỉ là chọn màu đẹp",
             [
                 "Màu VFX phải phục vụ gameplay, art style và background. Fire không phải lúc nào cũng cam. Magic không phải lúc nào cũng xanh tím. Trong UI hoặc iGaming, màu còn liên quan tới reward, rarity, warning hoặc brand.",
                 "Một palette tốt có tương phản nhiệt độ, value và saturation. Core có thể gần trắng, mid tone mang hue chính, edge hoặc accent mang hue phụ. Nếu tất cả cùng một hue và cùng saturation, effect dễ bị một màu và thiếu chiều sâu.",
                 "Hãy test trên nền sáng, nền tối và nền cùng hue. Một effect xanh trên background xanh cần value contrast hoặc edge khác màu để đọc."
             ]),
            ("Tonemapper và Exposure có thể đổi cảm giác effect",
             [
                 "Material Preview không phải scene thật. Exposure, Bloom, Color Grading và Tonemapper trong level có thể thay đổi cảm giác Emissive. Một flare đẹp ở preview có thể cháy hoặc yếu trong map thật.",
                 "Với production, hãy test effect trong lighting condition thật càng sớm càng tốt. Nếu game có nhiều biome hoặc level khác nhau, cần variant hoặc parameter để giữ readability.",
                 "Đừng đặt Intensity theo con số tuyệt đối rồi tin nó đúng mọi nơi. Đặt theo mục tiêu visual và context. Một effect combat cần đọc trong hỗn loạn khác một ambient magical dust."
             ]),
        ],
        ("Ghi nhớ", "Emissive là hierarchy ánh sáng, không phải một slider càng cao càng tốt.", False),
        [
            "Một projectile core cháy trắng nhưng vẫn yếu có thể thiếu outer shape hoặc color contrast. Giảm core, tăng edge/mid glow có khi đọc tốt hơn tăng intensity.",
            "Một reward sparkle iGaming cần vui và sạch. Nó thường cần timing, palette và silhouette rõ hơn là shader phức tạp."
        ],
        [
            "core  = pow(mask, 6) * CoreColor * CoreIntensity",
            "mid   = pow(mask, 2) * MidColor  * MidIntensity",
            "outer = pow(mask, 0.5) * OuterColor * OuterIntensity",
            "emissive = core + mid + outer"
        ],
    ),
    make_chapter(
        "Optimization Cookbook - tối ưu theo nguyên nhân",
        "overdraw_optimization.png",
        "Tối ưu VFX không bắt đầu bằng xóa node. Nó bắt đầu bằng xác định loại chi phí đang thật sự cao.",
        [
            ("Nếu vấn đề là Overdraw",
             [
                 "Overdraw cao thường đến từ translucent card lớn, nhiều particle chồng nhau, alpha texture có nhiều vùng trong suốt nhưng vẫn phủ pixel, hoặc effect tồn tại quá lâu. Đây là lỗi phổ biến nhất trong smoke, fire, magic cloud, dust và UI sparkle.",
                 "Giải pháp đầu tiên là giảm diện tích pixel: crop texture, dùng mesh ôm silhouette hơn, giảm size theo distance, giảm lifetime, giảm spawn count hoặc dùng LOD. Sau đó mới xem shader instruction.",
                 "Một mẹo thực dụng là nhìn alpha texture ở grayscale. Nếu phần lớn card là đen nhưng card vẫn to, GPU vẫn phải xét vùng đó. Texture đẹp không cứu được card phủ quá rộng."
             ]),
            ("Nếu vấn đề là Texture Fetch",
             [
                 "Texture Fetch cao đến từ nhiều sample, sample nhiều mip/bias, normal/detail/noise chồng quá nhiều, hoặc POM/blur/raymarch sampling lặp. Channel Packing, atlas, reuse sample và bake detail có thể giúp giảm fetch.",
                 "Đừng pack bừa mọi thứ. Pack các mask có cùng resolution, compression và mip requirement. Normal map không nên pack chung tùy tiện với mask grayscale nếu setting khác nhau.",
                 "Nếu một texture chỉ tạo variation rất nhẹ, hãy hỏi có thể thay bằng vertex color, particle random, simple noise hoặc parameter không. Ngược lại, nếu procedural noise quá đắt, bake texture có thể rẻ hơn."
             ]),
            ("Nếu vấn đề là Shader Instruction",
             [
                 "Instruction count cao do nhiều math, branch, custom node, trigonometry, noise procedural, POM hoặc repeated function. Giảm instruction bằng cách bỏ layer không đọc, precompute, bake, dùng Static Switch, quality level hoặc material variant.",
                 "Trigonometry như sin/cos/atan2 có thể cần thiết cho polar, wave, rotation. Không cần sợ chúng, nhưng đừng dùng lặp trong nhiều layer khi một texture hoặc lookup đơn giản đủ.",
                 "Hãy nhớ shader instruction chỉ là một phần. Một shader ít instruction nhưng phủ cả màn hình vẫn đắt. Tối ưu đúng là tối ưu bottleneck, không tối ưu con số dễ thấy."
             ]),
            ("Nếu vấn đề là CPU/Niagara",
             [
                 "Niagara có thể tốn CPU vì spawn/update nhiều particle, collision, events, sorting, nhiều renderer hoặc bounds update. Material rẻ không cứu được một system quản lý quá nhiều thứ mỗi frame.",
                 "Giải pháp gồm pooling, giảm emitter, dùng fixed bounds, giảm collision, dùng GPU particles khi phù hợp, culling theo distance/visibility và giảm update frequency cho effect phụ.",
                 "Một Technical VFX Artist giỏi không chỉ nói shader rẻ. Họ biết effect nằm trong system, system nằm trong level, và level nằm trong frame budget."
             ]),
        ],
        ("Cảnh báo performance", "Không tối ưu theo thói quen. Hãy xác định loại chi phí trước: overdraw, texture fetch, instruction, CPU update, draw call hay memory.", True),
        [
            "Một fire pillar mobile bị đắt có thể cần giảm particle layers, crop alpha, atlas flipbook và bỏ distortion layer, không phải chỉ giảm Power node.",
            "Một portal hero effect trên PC có thể chấp nhận shader phức tạp hơn nếu nó hiếm, nhỏ trên màn hình và là focal point. Budget phụ thuộc context."
        ],
        [
            "Optimization order:",
            "1. Measure GPU/CPU",
            "2. Identify cost type",
            "3. Remove invisible work",
            "4. Make quality tiers",
            "5. Re-test in real scene"
        ],
    ),
]


FAMILY_NAMES = [
    "Add, Subtract, Multiply, Divide",
    "Abs, Sign, OneMinus, Min, Max",
    "Clamp, Saturate, Step, SmoothStep",
    "Power, SquareRoot, Exponential, Logarithm",
    "Frac, Floor, Ceil, Round, Fmod",
    "Sine, Cosine, Tangent và LinearSine",
    "Arctangent2, Radians, Degrees và Pi",
    "DotProduct, CrossProduct và Normalize",
    "Distance, Length và VectorToRadialValue",
    "Lerp, InverseLerp và RemapValueRange",
    "ComponentMask, AppendVector và AddComponents",
    "If, Switch và logic chọn nhánh",
    "Transform3x3Matrix và TransformToClipSpace",
    "ProjectVectorOntoPlane và MakeVectorsOrthogonal",
    "HsvToRgb, RgbToHsv và FindSaturation",
    "WrapFloat, WrapFloat2, WrapFloat3, WrapFloat4",
]


FAMILY_PROFILES = {
    "Add, Subtract, Multiply, Divide": {
        "role": "Đây là nhóm toán nền của mọi graph. Add/Subtract dịch giá trị và tạo chênh lệch; Multiply/Divide scale cường độ, UV, vector hoặc mask. Trong VFX, phần lớn graph phức tạp vẫn quay về bốn thao tác này.",
        "examples": "Add dùng để offset UV hoặc cộng thêm edge glow. Subtract dùng để so threshold, lấy hiệu giữa distance và radius, hoặc tạo dissolve diff. Multiply dùng để nhân mask với color, nhân alpha với Particle Color, scale distortion. Divide dùng để normalize theo kích thước, nhưng cần tránh mẫu số gần zero.",
        "danger": "Multiply quá nhiều lớp emissive dễ cháy trắng. Divide bằng giá trị nhỏ có thể tạo output bất ổn. Add/Subtract trước hay sau Clamp cũng thay đổi kết quả rất mạnh.",
        "exercise": "Dựng một radial mask, sau đó dùng Multiply để scale intensity, Add để bias mask, Subtract để tạo ring quanh radius, Divide để normalize distance theo một radius parameter.",
        "code": ["diff = Noise - Threshold", "mask = saturate(diff * Contrast + Bias)", "ring = 1 - smoothstep(Width, Width + Soft, abs(Distance - Radius))"],
    },
    "Abs, Sign, OneMinus, Min, Max": {
        "role": "Nhóm này chuyên đảo, gấp, giới hạn và chọn dominance. Nó rất hữu ích khi bạn cần tạo symmetry, ring band, inside/outside logic hoặc mask ưu tiên.",
        "examples": "OneMinus đảo Fresnel hoặc alpha. Abs biến khoảng cách có dấu thành khoảng cách hai phía, cực kỳ hợp cho stripe đối xứng và ring. Sign cho biết bên trái/phải, trong/ngoài theo một trục. Min/Max dùng để gộp mask theo kiểu lấy vùng tối hơn hoặc sáng hơn.",
        "danger": "OneMinus chỉ hợp với dữ liệu 0..1. Nếu input vượt range, output có thể âm. Abs làm mất thông tin hướng, nên đừng dùng nếu bạn còn cần biết bên nào của trục.",
        "exercise": "Tạo một slash mask bằng UV.x, dùng Abs để mirror hai cạnh, dùng OneMinus để đảo vùng chọn, rồi dùng Max để gộp core và edge.",
        "code": ["mirrored = abs(UV.x - 0.5) * 2", "edge = 1 - smoothstep(Width, Width + Soft, mirrored)", "combined = max(core, edge * EdgeBoost)"],
    },
    "Clamp, Saturate, Step, SmoothStep": {
        "role": "Đây là nhóm quản lý range và threshold. Nó quyết định mask bị cắt cứng, chuyển mềm, hay bị giữ an toàn trong 0..1.",
        "examples": "Saturate giữ giá trị trong 0..1. Step tạo cutout cứng cho masked dissolve. SmoothStep tạo edge mềm cho dissolve, foam, soft intersection hoặc shockwave. Clamp dùng khi range không phải 0..1, ví dụ giới hạn depth hoặc distance.",
        "danger": "Clamp quá sớm làm mất dữ liệu. SmoothStep với hai ngưỡng quá gần gây cạnh sắc và aliasing. Step dùng trên translucent smoke thường làm texture bị cứng giả.",
        "exercise": "Dùng cùng một noise để tạo ba biến thể dissolve: Step cứng, SmoothStep mềm, và Edge Band bằng hiệu của hai SmoothStep.",
        "code": ["visible = step(Threshold, Noise)", "soft = smoothstep(Threshold, Threshold + Soft, Noise)", "edge = smoothstep(T - W, T, Noise) - smoothstep(T, T + S, Noise)"],
    },
    "Power, SquareRoot, Exponential, Logarithm": {
        "role": "Nhóm này uốn curve. Nó không đổi hình học gốc, nhưng đổi cảm giác phân bố sáng, alpha, contrast và falloff.",
        "examples": "Power cao làm core flare chặt lại. Power thấp hoặc SquareRoot mở rộng midtone. Exponential hữu ích cho response tăng rất nhanh, ví dụ pulse energy. Logarithm nén dynamic range nhưng ít dùng trực tiếp trong graph VFX phổ thông.",
        "danger": "Power trên giá trị âm có thể gây kết quả không mong muốn. Power quá cao làm mask mất midtone. Exponential dễ tạo giá trị quá lớn và cháy emissive nếu không clamp hoặc scale.",
        "exercise": "Tạo một flare bằng cùng radial mask: core dùng Power 6, glow dùng Power 0.5, edge dùng SmoothStep. So sánh cảm giác khi tắt bloom.",
        "code": ["core = pow(mask, 6)", "glow = pow(mask, 0.5)", "emissive = core * CoreIntensity + glow * GlowIntensity"],
    },
    "Frac, Floor, Ceil, Round, Fmod": {
        "role": "Nhóm này biến giá trị liên tục thành vòng lặp hoặc bậc rời rạc. Nó là nền cho pulse, scanline, frame index, digital glitch và pattern tile.",
        "examples": "Frac(Time * Speed) tạo phase 0..1 lặp lại. Floor chọn ô hoặc frame. Ceil tạo threshold kiểu bật sớm. Fmod giữ giá trị trong chu kỳ tùy ý, hữu ích khi chu kỳ không phải 1.",
        "danger": "Floor/Round gây giật nếu dùng trên motion cần mượt. Frac tạo seam tại điểm reset 1 về 0. Nếu seam lộ trong texture hoặc polar angle, effect sẽ có đường cắt.",
        "exercise": "Tạo một shockwave lặp bằng Frac(Time), dùng SmoothStep quanh phase để tạo ring chạy ra ngoài.",
        "code": ["phase = frac(Time * Speed)", "wave = 1 - smoothstep(Width, Width + Soft, abs(Radius - phase))", "pulse = wave * FadeOverLife"],
    },
    "Sine, Cosine, Tangent và LinearSine": {
        "role": "Nhóm lượng giác tạo nhịp dao động, wave, rotation và periodic motion. Nó đặc biệt hữu ích cho pulse, wobble, shimmer và procedural motion.",
        "examples": "Sine tạo flicker hoặc wave theo time. Cosine dùng cùng Sine để tạo rotation vector. LinearSine trong Unreal thuận tiện cho oscillation 0..1. Tangent ít dùng hơn vì dễ tăng vô hạn gần điểm cực.",
        "danger": "Sine không có art direction nếu dùng bừa. Flicker đều quá nhìn máy móc. Hãy kết hợp phase offset, noise hoặc curve để tránh nhịp quá sạch.",
        "exercise": "Tạo emissive pulse bằng sine, sau đó thêm noise nhỏ vào phase. So sánh pulse máy móc và pulse có variation.",
        "code": ["pulse = sin(Time * Speed + Phase) * 0.5 + 0.5", "flicker = pulse * lerp(0.8, 1.2, Noise)", "emissive *= flicker"],
    },
    "Arctangent2, Radians, Degrees và Pi": {
        "role": "Nhóm này đưa bạn vào thế giới góc. Arctangent2 tạo angle từ vector 2D, Radians/Degrees đổi đơn vị, Pi là hằng số nền cho vòng tròn.",
        "examples": "Atan2 dùng để tạo Polar UV cho portal, vortex, radial wipe. Pi và TwoPi dùng để normalize angle. Degrees/Radians hữu ích khi parameter của artist là độ nhưng công thức cần radian.",
        "danger": "Atan2 tạo seam angle. Nếu texture không tile hoặc seam nằm chỗ dễ thấy, portal sẽ có đường cắt. Radians/Degrees nhầm đơn vị làm rotation sai tốc độ hoặc sai góc.",
        "exercise": "Output angle grayscale từ Atan2, tìm seam, rồi thử xoay seam bằng cách cộng offset vào angle.",
        "code": ["angle = atan2(p.y, p.x)", "angle01 = angle / (2 * PI) + 0.5", "polarUV = float2(angle01 + Time * Speed, length(p))"],
    },
    "DotProduct, CrossProduct và Normalize": {
        "role": "Đây là nhóm đọc hướng. Dot biến góc giữa hai hướng thành số; Cross tạo hướng vuông góc; Normalize làm vector có độ dài ổn định.",
        "examples": "Dot Normal với ViewDir tạo Fresnel. Dot Velocity với Direction tạo alignment. Cross BeamDir với Up tạo side vector. Normalize cần thiết trước khi so hướng.",
        "danger": "Dot không normalize trộn góc với độ dài. Cross gần zero khi hai vector gần song song. Normalize vector zero gây bất ổn.",
        "exercise": "Dựng Fresnel thủ công bằng Dot và OneMinus, sau đó thử đổi NormalWS sang một space khác để thấy lỗi space rõ thế nào.",
        "code": ["facing = saturate(dot(normalize(N), normalize(V)))", "rim = pow(1 - facing, RimPower)", "side = normalize(cross(Direction, WorldUp))"],
    },
    "Distance, Length và VectorToRadialValue": {
        "role": "Nhóm này biến vị trí thành khoảng cách. Nó là nền cho circle, ring, shockwave, radial fade, proximity mask và vùng ảnh hưởng.",
        "examples": "Length(UV - 0.5) tạo radial mask. Distance giữa WorldPos và ImpactPos tạo hit ripple. VectorToRadialValue giúp chuyển vector 2D sang thông tin radial trong một số workflow.",
        "danger": "Distance phụ thuộc space. World distance và UV distance không cùng scale. Nếu dùng world distance nhưng mesh scale hoặc unit không rõ, effect có thể lệch giữa asset.",
        "exercise": "Tạo một hit ripple từ World Position và Impact Position, sau đó tạo cùng logic trong UV để so khác biệt object/world.",
        "code": ["d = distance(WorldPos, ImpactPos)", "ring = 1 - smoothstep(Width, Width + Soft, abs(d - Radius))", "fade = saturate(1 - d / MaxDistance)"],
    },
    "Lerp, InverseLerp và RemapValueRange": {
        "role": "Nhóm này chuyển đổi range và pha trộn. Lerp chọn giữa A và B theo alpha; InverseLerp biến một range bất kỳ thành 0..1; Remap đưa range này sang range khác.",
        "examples": "Lerp dùng để pha màu theo mask, blend giữa hai UV, đổi intensity theo lifetime. InverseLerp dùng để lấy normalized distance trong một khoảng. RemapValueRangeNormalized rất hợp khi dữ liệu đầu vào không nằm sẵn trong 0..1.",
        "danger": "Alpha của Lerp nếu không clamp có thể extrapolate vượt A/B. Remap sai min/max làm mask đảo hoặc response không đúng.",
        "exercise": "Remap PixelDepth hoặc distance thành 0..1 rồi dùng Lerp đổi màu từ core sang edge.",
        "code": ["t = saturate((x - InMin) / (InMax - InMin))", "value = lerp(OutMin, OutMax, t)", "color = lerp(InnerColor, OuterColor, edgeMask)"],
    },
    "ComponentMask, AppendVector và AddComponents": {
        "role": "Nhóm này đóng gói và tháo gỡ vector/channel. Nó là xương sống của Channel Packing, UV construction và debug dữ liệu.",
        "examples": "ComponentMask lấy R/G/B/A hoặc X/Y/Z/W. AppendVector ghép scalar thành vector, ví dụ U và V thành UV. AddComponents cộng R+G+B để đo tổng năng lượng hoặc tạo quick grayscale.",
        "danger": "Lấy nhầm channel là lỗi rất thường gặp. Packed texture phải có naming rõ. Append sai thứ tự U/V làm texture rotate hoặc mirror ngoài ý muốn.",
        "exercise": "Tạo một Packed Mask RGBA, dùng từng channel cho core, edge, distortion và alpha. Xuất từng channel ra grayscale để kiểm tra.",
        "code": ["core = Packed.r", "edge = Packed.g", "dist = Packed.b * 2 - 1", "uv = append(U, V)"],
    },
    "If, Switch và logic chọn nhánh": {
        "role": "Nhóm này chọn đường đi của dữ liệu. Nó hữu ích cho debug mode, quality tier, platform variant hoặc đổi logic theo parameter.",
        "examples": "Static Switch bật/tắt distortion, POM, debug grayscale. If có thể chọn màu hoặc mask theo điều kiện. Switch giúp master material có variant mà không cần nhiều file rời.",
        "danger": "Branch runtime không phải lúc nào cũng rẻ. Một master material có quá nhiều switch có thể khó maintain. Debug switch để quên trong final là lỗi production thật.",
        "exercise": "Thêm DebugMode cho Material: 0 final, 1 UV, 2 Mask, 3 Edge, 4 Cost proxy. Việc này giúp đọc graph nhanh hơn rất nhiều.",
        "code": ["if DebugMode == 1: return float3(UV, 0)", "if DebugMode == 2: return Mask.xxx", "return FinalColor"],
    },
    "Transform3x3Matrix và TransformToClipSpace": {
        "role": "Nhóm này thuộc họ đổi space. Nó dùng khi bạn cần kiểm soát transform vector hoặc đưa vị trí tới clip/screen pipeline.",
        "examples": "Transform3x3Matrix dùng cho vector basis, tangent/world conversion hoặc custom orientation. TransformToClipSpace liên quan tới projection và screen-space effect.",
        "danger": "Position và Vector không transform giống nhau. Dùng matrix sai loại có thể làm direction bị translation hoặc normal sai khi scale không đều.",
        "exercise": "Tạo một vector trong Object Space, transform sang World Space, rồi dùng Dot với ViewDirWS. Sau đó cố tình bỏ transform để thấy Fresnel sai.",
        "code": ["normalWS = mul(TangentToWorld3x3, normalTS)", "clipPos = TransformToClipSpace(worldPos)", "screenUV = clipPos.xy / clipPos.w * 0.5 + 0.5"],
    },
    "ProjectVectorOntoPlane và MakeVectorsOrthogonal": {
        "role": "Nhóm này sửa vector để nằm trên mặt phẳng hoặc tạo hệ vector sạch. Nó hữu ích cho beam, decal, impact direction và orientation.",
        "examples": "ProjectVectorOntoPlane loại bỏ component theo normal, ví dụ lấy hướng trượt trên mặt đất. MakeVectorsOrthogonal giúp tạo basis ít méo hơn cho procedural orientation.",
        "danger": "Nếu vector gần song song normal, projection có thể rất nhỏ. Cần fallback. Basis không orthogonal làm texture hoặc deformation bị shear.",
        "exercise": "Lấy hướng từ impact tới camera, project lên mặt phẳng ground, rồi dùng nó điều khiển slash decal hoặc dust direction.",
        "code": ["onPlane = V - Normal * dot(V, Normal)", "dir = SafeNormalize(onPlane)", "side = normalize(cross(dir, Normal))"],
    },
    "HsvToRgb, RgbToHsv và FindSaturation": {
        "role": "Nhóm màu này giúp chỉnh hue, saturation và value theo cách gần với art direction hơn RGB thô.",
        "examples": "HSV shift dùng cho rarity color, magical variation, rainbow energy hoặc palette cycling. FindSaturation giúp đo màu đang bão hòa tới mức nào để kiểm soát grade.",
        "danger": "Hue shift quá mạnh phá art style. HSV interpolation có thể tạo màu bẩn nếu không kiểm soát value. Với HDR emissive, color grading sau đó cũng ảnh hưởng kết quả.",
        "exercise": "Tạo một energy beam có Hue Offset theo Dynamic Parameter, nhưng giữ Value và Saturation trong range an toàn.",
        "code": ["hsv = RgbToHsv(BaseColor)", "hsv.x = frac(hsv.x + HueOffset)", "rgb = HsvToRgb(hsv)"],
    },
    "WrapFloat, WrapFloat2, WrapFloat3, WrapFloat4": {
        "role": "Wrap giữ giá trị quay vòng trong một khoảng. Nó giống tư duy modulo nhưng tiện khi làm UV, phase hoặc parameter loop.",
        "examples": "Wrap UV offset để tránh giá trị time quá lớn. Wrap phase cho pulse. Wrap vector dùng khi nhiều component cùng cần vòng lặp, ví dụ animated UV set hoặc tiled world coordinate.",
        "danger": "Wrap tạo discontinuity tại điểm reset. Nếu dùng cho gradient hoặc polar angle, seam có thể lộ. Hãy làm texture tile hoặc blend quanh vùng reset nếu cần.",
        "exercise": "Tạo một scanline chạy mãi bằng WrapFloat(Time * Speed, 0, 1), rồi dùng SmoothStep để tạo band mềm.",
        "code": ["phase = WrapFloat(Time * Speed, 0, 1)", "band = smoothstep(phase - W, phase, x) - smoothstep(phase, phase + W, x)"],
    },
}


def family_chapter(name, idx):
    profile = FAMILY_PROFILES[name]
    return make_chapter(
        f"Atlas node Unreal - {name}",
        None,
        "",
        [
            ("Ý nghĩa trong Material",
             [
                 profile["role"],
                 "Khi đọc nhóm node này trong một Material có sẵn, đừng đọc từ trái sang phải như đọc dây điện. Hãy hỏi nó đang nằm trong lane nào: UV, Mask, Color, Opacity, Interaction hay Cost. Cùng một phép toán đặt ở lane khác nhau sẽ có ý nghĩa khác nhau.",
                 "Một cách học chắc là luôn xuất kết quả trung gian ra grayscale. Nếu mắt bạn không nhìn thấy node đang đổi dữ liệu ra sao, bạn sẽ chỉ đang tin vào tên node."
             ]),
            ("Ví dụ production",
             [
                 profile["examples"],
                 "Trong một graph production, hãy đặt tên parameter theo vai trò visual chứ không theo tên node. `CorePower`, `EdgeSoftness`, `DistortionStrength` hoặc `FramePhase` hữu ích hơn `Scalar A` và `Scalar B`.",
                 profile["exercise"]
             ]),
            ("Lỗi thường gặp",
             [
                 profile["danger"],
                 "Lỗi thứ hai là sửa visual bằng node cuối cùng trong chain. Nhiều khi vấn đề nằm ở source data, không nằm ở node shaping. Noise sai import, UV sai space hoặc Particle Color chưa nhân vào sẽ làm cả chain phía sau sai.",
                 "Lỗi thứ ba là không ghi chú range. Một node nhận dữ liệu 0..1, -1..1 hay world-unit sẽ cần cách debug khác nhau. Range không rõ làm Material khó dùng lại."
             ]),
        ],
        None,
        [],
        profile["code"],
    )


EXTRA_CHAPTERS.extend(family_chapter(name, i) for i, name in enumerate(FAMILY_NAMES, 1))

base.cover = cover_v24
base.CHAPTERS = base.CHAPTERS + EXTRA_CHAPTERS


if __name__ == "__main__":
    base.build()
