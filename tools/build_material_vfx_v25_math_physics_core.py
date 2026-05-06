from pathlib import Path
import importlib.util

from docx.enum.text import WD_ALIGN_PARAGRAPH
from docx.shared import Cm


ROOT = Path(__file__).resolve().parents[1]
BASE_SCRIPT = ROOT / "tools" / "build_material_vfx_v24_expanded_book.py"

spec = importlib.util.spec_from_file_location("material_vfx_v24", BASE_SCRIPT)
v24 = importlib.util.module_from_spec(spec)
spec.loader.exec_module(v24)

base = v24.base
OUT = Path.home() / "Downloads" / "Giao_Trinh_Material_VFX_Realtime_V25_MathPhysicsCore.docx"
base.OUT = OUT


def cover_v25(doc):
    base.p(doc, "Giáo trình", size=12, font=base.SANS, bold=True, color=base.ORANGE, before=64, after=5, align=WD_ALIGN_PARAGRAPH.CENTER)
    title = base.p(doc, "Material VFX Realtime", size=32, font=base.HEAD, bold=True, color=base.BLUE, after=9, align=WD_ALIGN_PARAGRAPH.CENTER)
    title.paragraph_format.line_spacing = 1.0
    base.p(doc, "Toán học, vật lý trực quan và tư duy shader cho Technical VFX Artist", size=12.2, font=base.BODY, italic=True, color=base.MUTED, after=24, align=WD_ALIGN_PARAGRAPH.CENTER)
    rule = base.p(doc, "", before=0, after=22)
    base.border_bottom(rule, color="C98E54", size="10")
    base.p(doc, "Space · Vector · Wave · Field · Sampling · Light · Energy", size=14.2, font=base.SANS, bold=True, color=base.BLUE, before=8, after=8, align=WD_ALIGN_PARAGRAPH.CENTER)
    base.p(doc, "Unreal Material · Niagara · HLSL/GLSL · GPU Thinking", size=11.0, font=base.SANS, color=base.INK, after=26, align=WD_ALIGN_PARAGRAPH.CENTER)
    summary = base.p(
        doc,
        "Bản V25 đổi trọng tâm: ít recipe hơn, nhiều nền tảng hơn. Mục tiêu là hiểu vì sao công thức tạo ra hình ảnh, vì sao visual hỏng, và vì sao GPU phải trả giá.",
        size=10.4,
        font=base.BODY,
        color=base.INK,
        before=16,
        after=22,
        align=WD_ALIGN_PARAGRAPH.CENTER,
    )
    summary.paragraph_format.left_indent = Cm(1.0)
    summary.paragraph_format.right_indent = Cm(1.0)
    base.p(doc, "Phiên bản V25 · Math & Physics Core · 2026", size=9.3, font=base.SANS, color=base.MUTED, before=8, after=0, align=WD_ALIGN_PARAGRAPH.CENTER)


def ch(title, image, caption, sections, note=None, case=None, code=None):
    return {
        "title": title,
        "image": (image, caption) if image else None,
        "sections": sections,
        "notes": [note] if note else [],
        "case": case or [],
        "code": code or [],
    }


MATH_PHYSICS_CHAPTERS = [
    ch(
        "Số, range và normalization - vật liệu bắt đầu từ 0 đến 1",
        None,
        "",
        [
            ("Vì sao 0..1 xuất hiện khắp shader",
             [
                 "Material VFX dùng rất nhiều giá trị normalized vì GPU cần một ngôn ngữ chung để trộn dữ liệu. Alpha, mask, UV, gradient, lifetime, depth fade, dissolve amount và particle age thường được đưa về 0..1. Khi mọi thứ cùng range, bạn có thể Multiply, Lerp, SmoothStep và Remap mà không phải đoán scale quá nhiều.",
                 "Nhưng 0..1 không có nghĩa là đơn giản. Một giá trị 0.5 có thể là nửa vòng đời particle, giữa texture, midtone của noise, hoặc nửa khoảng cách fade. Cùng con số nhưng ý nghĩa phụ thuộc vào bối cảnh. Người làm shader giỏi luôn hỏi: giá trị này đo cái gì, đơn vị là gì, range thật là bao nhiêu?",
                 "Nếu dữ liệu không được normalize, graph rất dễ trở thành chuỗi sửa lỗi. Bạn thêm Multiply để giảm, Add để bù, Clamp để giữ, Power để chỉnh lại cảm giác. Đôi khi toàn bộ phần sau chỉ đang che việc input range sai ngay từ đầu."
             ]),
            ("Remap là kỹ năng sống còn",
             [
                 "Remap nghĩa là đổi một khoảng giá trị sang khoảng khác. Ví dụ Depth có thể nằm trong world unit, nhưng bạn cần nó thành 0..1 để fade. Distance từ impact tới pixel có thể là 0..500 cm, nhưng ring mask cần biết nó nằm ở đâu so với radius và width.",
                 "Công thức cơ bản là `(x - min) / (max - min)`. Sau đó Saturate để giới hạn 0..1 nếu cần. Khi hiểu công thức này, InverseLerp và RemapValueRange không còn là node mơ hồ. Chúng chỉ là cách đưa dữ liệu về ngôn ngữ chung.",
                 "Remap đúng giúp parameter có nghĩa production. Một `FadeDistance` tính bằng centimeter dễ hiểu hơn một scalar bí ẩn. Một `NoiseContrast` sau khi remap rõ range sẽ dễ tune hơn nhiều so với texture noise thả thẳng vào opacity."
             ]),
            ("Precision và lỗi nhỏ thành visual lớn",
             [
                 "Shader chạy trên số floating point. Số này có giới hạn precision. Với nhiều effect bình thường, bạn không cần nghĩ quá sâu. Nhưng với world position lớn, UV cực nhỏ, POM, VAT, depth, hoặc vector normalize gần zero, precision bắt đầu thành vấn đề thật.",
                 "Một lỗi precision thường không hiện như crash. Nó hiện như rung, shimmer, seam, banding, vertex nhảy hoặc mask không ổn định. Vì vậy, khi visual rung nhưng graph nhìn đúng, hãy nghĩ tới scale dữ liệu và precision.",
                 "Quy tắc thực dụng: giữ dữ liệu trong range hợp lý càng lâu càng tốt. Đừng để giá trị Time tăng vô hạn nếu chỉ cần phase lặp. Đừng dùng world position khổng lồ cho noise nếu có thể chuyển về local hoặc modulo."
             ]),
        ],
        ("Ghi nhớ", "Trước khi hỏi node nào cần dùng, hãy hỏi dữ liệu đang nằm trong range nào. Range sai làm graph đúng toán nhưng sai visual.", False),
        [
            "Một dissolve chạy quá nhanh ở cuối không nhất thiết do curve xấu. Có thể noise không phân bố đều trong 0..1, hoặc threshold đang quét qua vùng nhiều midtone quá dày.",
            "Một depth fade khó tune có thể vì bạn đang dùng distance world unit trực tiếp thay vì remap về 0..1 trước khi shaping."
        ],
        [
            "t = saturate((x - inMin) / (inMax - inMin))",
            "value = lerp(outMin, outMax, t)",
            "mask = smoothstep(edgeStart, edgeEnd, t)"
        ],
    ),
    ch(
        "Hàm số như bút vẽ - curve điều khiển cảm giác",
        "v9_power_shaping_flare.png",
        "Một hàm số nhỏ có thể thay đổi cảm giác ánh sáng, độ mềm, tốc độ và nhịp của effect.",
        [
            ("Linear hiếm khi đủ đẹp",
             [
                 "Linear nghĩa là đầu vào tăng bao nhiêu thì đầu ra tăng bấy nhiêu. Nó dễ hiểu và dễ debug, nhưng hình ảnh tự nhiên hiếm khi phản ứng tuyến tính. Ánh sáng, khói, lửa, nước, shockwave và motion đều cần curve để có cảm giác.",
                 "Power, SmoothStep, Sine, Exponential và Log không phải chỉ là toán. Chúng là cách bạn thiết kế cảm giác response. Một alpha fade tuyến tính có thể nhìn cơ học. Một fade dùng SmoothStep có ease in/ease out mềm hơn. Một pulse dùng sine có nhịp tự nhiên hơn saw wave.",
                 "Học function nên luôn đi kèm hình. Hãy vẽ curve hoặc xuất gradient ra màn hình. Nếu bạn không thấy curve, bạn sẽ khó đoán vì sao visual thay đổi."
             ]),
            ("Độ dốc của curve quyết định vùng nhạy",
             [
                 "Một curve có vùng dốc lớn nghĩa là input thay đổi nhỏ nhưng output thay đổi mạnh. Đây là lý do Threshold hoặc Power có thể khiến effect rất nhạy với parameter. Khi artist kéo slider một chút mà visual đổi quá nhiều, không phải UI tệ; curve có thể quá dốc ở vùng đang dùng.",
                 "SmoothStep giảm vấn đề này bằng cách làm đầu và cuối phẳng hơn. Nhưng nếu width quá nhỏ, nó lại gần Step. Power cao cũng tạo vùng gần 1 rất nhạy. Bạn cần biết vùng nào của curve đang được production tune thường xuyên.",
                 "Một parameter tốt không chỉ đúng toán. Nó phải dễ điều khiển. Nếu một slider 0..1 chỉ hữu ích trong khoảng 0.47..0.53, hãy remap để vùng hữu ích rộng hơn."
             ]),
            ("Curve chaining",
             [
                 "Trong material, bạn hiếm khi dùng một function đơn lẻ. Bạn có thể remap noise, contrast bằng Multiply/Add, Saturate, rồi Power, rồi SmoothStep. Chuỗi này tạo cảm giác nhưng cũng làm graph khó đoán nếu không debug từng đoạn.",
                 "Khi một mask hỏng, hãy output từng stage: raw, remapped, shaped, final. Nếu raw đã xấu, đừng sửa final. Nếu raw tốt nhưng shaped xấu, curve sai. Nếu shaped tốt nhưng final xấu, có thể blend/color/opacity path đang làm hỏng.",
                 "Đây là tư duy shader như phòng lab: kiểm tra biến trung gian. Không khác gì vật lý thí nghiệm, chỉ là dụng cụ đo của bạn là grayscale output."
             ]),
        ],
        ("Ghi nhớ", "Curve là art direction. Một Power node không chỉ làm 'mạnh hơn'; nó đổi phân bố giá trị và vùng nhạy của parameter.", False),
        [
            "Một flare có core yếu có thể cần curve chặt hơn chứ không cần intensity cao hơn. Ngược lại, smoke bị cứng có thể cần curve mềm hơn hoặc source alpha ít contrast hơn.",
            "Một dissolve slider khó tune thường do threshold đang đi qua noise distribution không đều. Hãy xem histogram bằng mắt qua grayscale."
        ],
        [
            "linear = x",
            "ease   = x * x * (3 - 2 * x)",
            "core   = pow(x, 5)",
            "soft   = pow(x, 0.5)"
        ],
    ),
    ch(
        "Vector là hướng, lực và quan hệ không gian",
        "v9_space_vectors.png",
        "Vector không chỉ là float3. Nó là hướng nhìn, normal, velocity, force, offset và basis.",
        [
            ("Vector có độ dài và hướng",
             [
                 "Một scalar chỉ có độ lớn. Một vector có cả độ lớn và hướng. Trong VFX, vector xuất hiện dưới nhiều tên: Normal, View Direction, Velocity, Acceleration, World Position Offset, Flow Map, Wind Direction, Impact Direction. Nếu đọc chúng như màu RGB, bạn sẽ bỏ lỡ ý nghĩa vật lý.",
                 "Độ dài vector có thể biểu thị tốc độ, cường độ lực hoặc khoảng cách. Hướng vector quyết định motion hoặc orientation. Normalize tách hướng khỏi độ dài. Length lấy độ dài. Dot so hướng. Cross tạo hướng vuông góc.",
                 "Khi debug vector, hãy tách hai câu hỏi: hướng có đúng không và độ dài có đúng không. Một velocity đúng hướng nhưng quá dài sẽ làm particle kéo vệt quá mạnh. Một normal đúng độ dài nhưng sai space sẽ làm Fresnel sai."
             ]),
            ("Dot Product là phép đo quan hệ",
             [
                 "Dot Product trả lời câu hỏi hai hướng giống nhau đến mức nào. Nó là nền của Fresnel, rim light, fake lighting, alignment, directional mask và nhiều effect reactive. Nếu Dot gần 1, hai hướng gần cùng chiều. Nếu gần 0, chúng vuông góc. Nếu âm, chúng ngược chiều.",
                 "Trong VFX, Dot giúp biến quan hệ hình học thành mask. Normal so với ViewDir tạo rim. Velocity so với WindDir tạo streak mạnh hơn khi bay cùng hướng. BeamDir so với SurfaceNormal có thể tạo impact intensity tùy góc va chạm.",
                 "Điều kiện quan trọng là vector phải cùng space và thường phải normalized. Nếu không, Dot vẫn ra số nhưng số đó không còn là góc thuần."
             ]),
            ("Cross Product và basis",
             [
                 "Cross Product tạo vector vuông góc với hai vector đầu vào. Nó giúp dựng hệ trục phụ cho beam, ribbon, slash, decal projection hoặc procedural mesh orientation. Trong vật lý hình học, bạn thường cần một forward, một side và một up.",
                 "Nếu forward gần song song với up, Cross có thể gần zero. Đây là lý do nhiều hệ orientation cần fallback. Một shader không xử lý trường hợp hiếm có thể chạy đẹp 99 frame rồi lật ở frame quan trọng nhất.",
                 "Khi dựng basis, hãy nghĩ như dựng camera nhỏ cho effect. Nó nhìn theo hướng nào, ngang là đâu, dọc là đâu, và dữ liệu nào nằm trong space đó?"
             ]),
        ],
        ("Ghi nhớ", "Vector luôn phải đi kèm câu hỏi: nó đang ở space nào, độ dài có nghĩa gì, và hướng của nó dùng để quyết định visual nào.", False),
        [
            "Một shield rim sai khi mesh xoay thường là lỗi space: Normal ở một space, ViewDir ở space khác.",
            "Một beam bị xoắn hoặc lật có thể do basis dựng bằng Cross Product không có fallback khi hai vector gần song song."
        ],
        [
            "dir = normalize(Target - Origin)",
            "alignment = saturate(dot(dir, normalize(WindDir)))",
            "side = normalize(cross(dir, WorldUp))",
            "up = cross(side, dir)"
        ],
    ),
    ch(
        "Đạo hàm, gradient và cảm giác thay đổi",
        "v9_power_shaping_flare.png",
        "Đạo hàm cho biết một giá trị đổi nhanh thế nào. Trong shader, đó là chìa khóa của edge, anti-aliasing và flow.",
        [
            ("Đạo hàm không chỉ dành cho toán học trên giấy",
             [
                 "Nếu một giá trị thay đổi chậm, hình ảnh mềm. Nếu nó đổi nhanh, hình ảnh sắc. Edge của mask thực chất là vùng giá trị thay đổi mạnh. Gradient của height map cho bạn hướng dốc. fwidth cho biết giá trị thay đổi bao nhiêu trên pixel lân cận.",
                 "Khi bạn nhìn một dissolve edge, bạn đang nhìn vùng đạo hàm cao của mask quanh threshold. Khi bạn tạo normal từ height, bạn đang dùng gradient của height. Khi bạn anti-alias một line procedural, bạn dùng derivative để biết line nhỏ hơn pixel chưa.",
                 "Hiểu đạo hàm giúp bạn bớt xem edge như phép màu. Edge không phải một màu thêm vào. Nó là vùng chuyển tiếp của dữ liệu."
             ]),
            ("Gradient là hướng tăng nhanh nhất",
             [
                 "Gradient của một field cho biết hướng giá trị tăng nhanh nhất. Với height map, gradient cho hướng dốc. Với distance field, gradient chỉ hướng ra ngoài shape. Với flow hoặc potential field, gradient có thể dùng để dẫn motion.",
                 "Trong Material VFX, bạn có thể không tự tính gradient mỗi ngày, nhưng tư duy này rất quan trọng. Một normal map là encoded gradient của bề mặt nhỏ. Một flow map là hướng vector trên mặt phẳng. Một SDF có thể cho edge direction và outline rất sạch.",
                 "Khi một texture height tạo cảm giác sai, hãy hỏi gradient của nó có hợp visual không. Height đẹp ở grayscale chưa chắc tạo normal đẹp nếu slope phân bố không hợp."
             ]),
            ("fwidth và anti-aliasing procedural",
             [
                 "Procedural shape như circle, stripe, ring, grid rất dễ alias nếu dùng Step cứng. fwidth giúp bạn biết khoảng chuyển tiếp nên mềm bao nhiêu theo pixel footprint. Ở gần, edge có thể sắc. Ở xa, edge cần mềm hơn để không nhấp nháy.",
                 "Đây là khác biệt giữa shader demo đẹp trong một frame và effect dùng được trong game. Game có camera di chuyển, resolution khác nhau, motion blur, TAA và post-process. Edge quá mỏng sẽ rung.",
                 "Khi làm line, ring hoặc magic circle, hãy nghĩ tới anti-aliasing ngay từ đầu. Nếu shape nhỏ hơn pixel, tăng texture resolution không giải quyết gốc."
             ]),
        ],
        ("Ghi nhớ", "Edge là nơi dữ liệu đổi nhanh. Anti-aliasing là kiểm soát tốc độ đổi đó theo kích thước pixel.", False),
        [
            "Một magic circle nhấp nháy ở xa thường cần fwidth-based edge, không phải texture 4K.",
            "Một height-to-normal nhìn gắt có thể do gradient quá mạnh; giảm height scale hoặc blur height trước khi tạo normal."
        ],
        [
            "d = abs(distanceToShape)",
            "aa = fwidth(d)",
            "edge = 1 - smoothstep(width, width + aa, d)"
        ],
    ),
    ch(
        "Sóng - nhịp, phase, frequency và interference",
        "v9_power_shaping_flare.png",
        "Sóng là mô hình đơn giản để nghĩ về pulse, ripple, shimmer, flicker, scanline và energy flow.",
        [
            ("Time không phải animation; phase mới là animation",
             [
                 "Time tăng mãi. Phase là vị trí trong một chu kỳ. Khi bạn dùng `sin(Time * Speed)`, bạn biến time thành phase tuần hoàn. Khi dùng `frac(Time * Speed)`, bạn tạo saw wave 0..1 lặp lại. Phase giúp effect có nhịp.",
                 "Một ripple không chỉ là ring lớn dần. Nó là phase chạy qua distance field. Một scanline không chỉ là texture panner. Nó là phase so với tọa độ. Khi hiểu phase, bạn có thể tạo motion bằng công thức thay vì phụ thuộc hoàn toàn vào flipbook.",
                 "Phase cũng giúp sync nhiều layer. Fire flicker, glow pulse và particle burst có thể cùng phase hoặc lệch phase. Lệch phase tạo cảm giác tự nhiên hơn; cùng phase tạo cảm giác có chủ đích hoặc máy móc."
             ]),
            ("Frequency và wavelength",
             [
                 "Frequency là số chu kỳ trong một đơn vị. Wavelength là khoảng cách giữa hai đỉnh sóng. Trong shader, tăng frequency làm pattern dày hơn. Với UV, frequency liên quan tới tiling. Với time, frequency liên quan tới tốc độ dao động.",
                 "Một lỗi phổ biến là dùng frequency quá cao cho effect nhỏ hoặc camera xa. Pattern dày hơn pixel sẽ alias. Ngược lại, frequency quá thấp làm effect thiếu detail. Scale visual phải hợp camera gameplay.",
                 "Khi tune wave, hãy tách spatial frequency và temporal frequency. Pattern dày mỏng là chuyện không gian. Chạy nhanh chậm là chuyện thời gian. Trộn hai thứ này trong một parameter làm effect khó điều khiển."
             ]),
            ("Interference và layered motion",
             [
                 "Khi hai sóng cộng nhau, chúng có thể tăng cường hoặc triệt tiêu. Đây là interference. Trong VFX, bạn có thể dùng nhiều sine với speed/phase khác nhau để tạo flicker phong phú hơn một sine đơn.",
                 "Nhưng nhiều layer không tự nhiên nếu chúng không có vai trò. Một wave lớn chậm có thể là breath chính. Một wave nhỏ nhanh là shimmer. Một noise nhẹ phá nhịp đều. Mỗi lớp nên giải quyết một cảm giác.",
                 "Nếu effect trông máy móc, hãy kiểm tra phase và frequency. Có thể mọi thứ đang cùng nhịp. Nếu effect trông bẩn, có thể quá nhiều wave cạnh tranh cùng một vùng visual."
             ]),
        ],
        ("Ghi nhớ", "Animation procedural thường là quản lý phase. Time chỉ là nguyên liệu thô.", False),
        [
            "Một shield pulse có thể dùng distance từ hit point cộng phase theo Time để tạo ring lan ra. Không cần flipbook nếu shape đơn giản.",
            "Một energy beam nhìn sống hơn khi có wave lớn chậm cho body và wave nhỏ nhanh cho edge, thay vì một panner đều."
        ],
        [
            "phase = Time * Speed - Distance * Frequency",
            "wave = sin(phase) * 0.5 + 0.5",
            "ring = wave * smoothstep(Radius, Radius + Soft, Distance)"
        ],
    ),
    ch(
        "Trường lực và vector field - motion có cấu trúc",
        "v9_space_vectors.png",
        "Field là giá trị tồn tại ở mọi điểm trong không gian. Nó có thể là scalar field, vector field hoặc distance field.",
        [
            ("Scalar field và vector field",
             [
                 "Một scalar field gán một số cho mỗi điểm: nhiệt độ, density, distance, mask, pressure. Một vector field gán một vector cho mỗi điểm: hướng gió, flow, velocity, force. Trong shader, texture noise là scalar field; flow map là vector field; distance-to-center là scalar field procedural.",
                 "VFX đầy field dù bạn không gọi tên. Smoke density là scalar field. Flow map nước là vector field. SDF portal là distance field. Forcefield shield có thể là field phản ứng theo hit point.",
                 "Tư duy field giúp bạn không nghĩ effect như từng pixel rời rạc. Bạn nghĩ cả không gian có luật. Pixel chỉ là điểm lấy mẫu của luật đó."
             ]),
            ("Divergence và curl ở mức trực quan",
             [
                 "Divergence đo field đang tỏa ra hay hút vào. Một explosion có divergence dương: motion đi ra khỏi tâm. Một black hole hoặc vortex hút vào có divergence âm hoặc motion hướng vào tâm. Curl đo xu hướng xoáy quanh. Smoke swirl và vortex cần curl rõ.",
                 "Bạn không nhất thiết phải tính divergence/curl chính xác trong Material mỗi ngày. Nhưng hiểu khái niệm giúp bạn thiết kế motion. Nếu muốn energy hút vào portal, vector field phải có thành phần inward. Nếu muốn nó xoáy, thêm thành phần tangential.",
                 "Một motion đẹp thường kết hợp radial và tangential. Chỉ radial thì giống nổ hoặc hút. Chỉ tangential thì giống quay tại chỗ. Kết hợp đúng tạo vortex."
             ]),
            ("Flow map là vector field được vẽ tay",
             [
                 "Flow map thường lưu vector 2D trong RG. Giá trị 0.5,0.5 là không chuyển động; đỏ/xanh lá lệch khỏi 0.5 là hướng. Shader remap RG từ 0..1 sang -1..1 rồi cộng vào UV. Texture không di chuyển; UV bị đẩy theo field.",
                 "Flow map mạnh vì artist có thể vẽ hướng motion. Water, energy, smoke distortion, magic portal đều có thể dùng. Nhưng Flow map cần import đúng: thường không sRGB, compression phù hợp, và debug vector bằng màu hoặc mũi tên nếu có tool.",
                 "Nếu flow quá mạnh, silhouette bị méo. Nếu quá yếu, không thấy motion. Nếu field không tile, seam sẽ lộ. Flow map là vật lý giả, nhưng vẫn cần luật."
             ]),
        ],
        ("Ghi nhớ", "Muốn motion có cảm giác, hãy nghĩ tới field: điểm này nên đi hướng nào, mạnh bao nhiêu, và vì sao.", False),
        [
            "Một portal hút vào tâm cần vector inward cộng swirl tangential. Nếu chỉ xoay texture, nó không có cảm giác hút.",
            "Một heat distortion nên làm méo background bên trong vùng nóng, không làm méo cả silhouette card."
        ],
        [
            "p = UV - 0.5",
            "radial = -normalize(p)",
            "tangent = float2(-radial.y, radial.x)",
            "flow = radial * Inward + tangent * Swirl"
        ],
    ),
    ch(
        "Ánh sáng vật lý tối giản cho VFX Artist",
        "v11_imagegen_vfx_atlas.png",
        "Không cần trở thành rendering engineer, nhưng cần hiểu energy, falloff, scattering, absorption và exposure.",
        [
            ("Intensity và energy không giống cảm giác sáng",
             [
                 "Trong HDR pipeline, giá trị emissive cao hơn 1 có thể tạo bloom. Nhưng cảm giác sáng của người xem không chỉ phụ thuộc số emissive. Nó phụ thuộc background, exposure, tonemapping, bloom threshold, size vùng sáng và contrast với xung quanh.",
                 "Một điểm sáng nhỏ có thể cảm giác mạnh hơn một vùng lớn vừa sáng nếu core rõ và falloff đẹp. Đây là lý do flare tốt cần hierarchy: core, mid glow, outer glow. Vật lý thật có falloff; art direction chọn falloff đọc được trong game.",
                 "Khi effect yếu, đừng chỉ tăng intensity. Hãy xem core có đủ nhỏ không, falloff có đúng không, color có tương phản không, và background có nuốt effect không."
             ]),
            ("Scattering và absorption ở mức trực quan",
             [
                 "Smoke, fog, fire và magic cloud đều gợi cảm giác môi trường tham gia vào ánh sáng. Scattering là ánh sáng bị tán xạ trong môi trường. Absorption là ánh sáng bị hấp thụ. Trong realtime material đơn giản, bạn không mô phỏng đầy đủ, nhưng bạn có thể giả bằng opacity, depth fade, soft particles, color ramp và view-dependent lighting.",
                 "Smoke thường sáng ở rìa hoặc khi backlit, tối ở vùng dày. Fire thường có core nóng sáng và outer smoke tối hơn. Magic mist có thể dùng Fresnel hoặc depth để tạo cảm giác volume nhẹ. Những quyết định này dựa trên vật lý cảm nhận, không chỉ màu đẹp.",
                 "Nếu khói trông như decal phẳng, có thể thiếu depth interaction, thiếu lighting variation hoặc alpha quá đều. Nếu lửa trông như ảnh dán, có thể thiếu core temperature hierarchy và motion."
             ]),
            ("Exposure làm effect đổi đời",
             [
                 "Auto exposure hoặc exposure setting trong level có thể làm emissive thay đổi cảm giác mạnh. Một effect test trong preview không đại diện cho scene thật. Đây là lý do nhiều studio có test map cho VFX với nền sáng, tối, midtone và lighting khác nhau.",
                 "Về mặt vật lý giả, exposure là camera quyết định scene sáng tối. Về mặt production, exposure là biến ngoại cảnh có thể phá effect. Technical VFX Artist cần kiểm tra trong context, không chỉ trong material editor.",
                 "Một cách làm chắc là tạo parameter theo layer: core intensity, glow intensity, alpha, color. Khi exposure khác, bạn có nhiều tay nắm để tune mà không phá toàn bộ effect."
             ]),
        ],
        ("Ghi nhớ", "Emissive không phải ánh sáng tự do. Nó đi qua exposure, bloom, tonemapper và background trước khi thành cảm giác trong mắt người xem.", False),
        [
            "Một magic projectile cháy trắng trong dark test map nhưng biến mất ở snow level cần contrast và hierarchy, không chỉ tăng emissive.",
            "Một smoke plume nhìn phẳng có thể cần view/light variation, không phải thêm noise."
        ],
        [
            "core = pow(mask, 6) * CoreHDR",
            "glow = pow(mask, 1.5) * GlowHDR",
            "outer = pow(mask, 0.4) * OuterTint",
            "emissive = core + glow + outer"
        ],
    ),
    ch(
        "Màu sắc - từ RGB tới cảm giác nhiệt, hiếm và nguy hiểm",
        "v11_imagegen_vfx_atlas.png",
        "Màu trong VFX là dữ liệu thị giác: value, hue, saturation, temperature, rarity và gameplay meaning.",
        [
            ("RGB là cách lưu, không phải cách nghĩ duy nhất",
             [
                 "RGB nói màu bằng ba kênh đỏ, xanh lá, xanh dương. Nó phù hợp với màn hình và shader, nhưng artist thường nghĩ bằng hue, saturation và value. Khi chỉnh VFX, bạn cần cả hai cách nhìn. RGB để biết dữ liệu trong shader. HSV/HSL để nói chuyện art direction.",
                 "Hue đổi loại màu. Saturation đổi độ rực. Value đổi độ sáng. Một effect có thể cần hue lạnh nhưng value cao, hoặc hue nóng nhưng saturation thấp. Nếu chỉ kéo RGB, bạn dễ làm màu bẩn hoặc mất contrast.",
                 "Trong shader, HsvToRgb và RgbToHsv hữu ích khi cần hue shift, rarity color hoặc palette cycling. Nhưng đừng lạm dụng hue rotate cho mọi thứ; palette game cần consistency."
             ]),
            ("Value contrast đọc trước hue",
             [
                 "Trong gameplay, mắt đọc value contrast rất nhanh. Một effect màu đẹp nhưng value gần background sẽ biến mất. Vì vậy, hãy kiểm tra grayscale. Nếu ở grayscale effect không đọc, màu có thể chỉ đang che lỗi.",
                 "Stylized VFX thường dùng value hierarchy rất rõ: core gần trắng, body màu chính, edge/accent màu phụ, outer glow mềm. Realistic VFX có thể subtle hơn nhưng vẫn cần vùng đọc chính.",
                 "iGaming và mobile càng cần value rõ vì effect thường nằm trên UI nhiều màu. Một win sparkle phải đọc nhanh trong vài trăm millisecond. Màu đẹp nhưng value yếu là thất bại production."
             ]),
            ("Nhiệt độ màu và vật lý cảm nhận",
             [
                 "Lửa thật có quan hệ nhiệt độ: vùng nóng nhất thường sáng và gần trắng/vàng, vùng nguội hơn cam/đỏ, smoke tối hơn. Energy fantasy có thể phá vật lý này, nhưng nếu phá có chủ đích thì vẫn thuyết phục.",
                 "Màu lạnh thường gợi magic, ice, sci-fi hoặc plasma. Màu nóng gợi fire, lava, explosion, danger. Nhưng ý nghĩa màu còn phụ thuộc art style và game language. Nếu game dùng đỏ cho enemy danger, đừng dùng đỏ cho heal effect trừ khi có lý do mạnh.",
                 "Một Technical VFX Artist nên hiểu màu như hệ thống tín hiệu. Shader chỉ tạo màu; production quyết định màu đó nói gì."
             ]),
        ],
        ("Ghi nhớ", "Luôn kiểm tra effect ở grayscale. Hue làm nó đẹp; value làm nó đọc được.", False),
        [
            "Một portal tím xanh đẹp nhưng không đọc trên background đêm có thể cần value contrast, rim sáng hoặc silhouette edge.",
            "Một fire effect có core cam đều từ trong ra ngoài thường thiếu cảm giác nhiệt. Thêm core gần trắng và outer smoke tối có thể thuyết phục hơn."
        ],
        [
            "hsv = RgbToHsv(color)",
            "hsv.x = frac(hsv.x + HueShift)",
            "hsv.y *= SaturationScale",
            "color = HsvToRgb(hsv)"
        ],
    ),
    ch(
        "Nhiệt, lửa và khói - vật lý đủ dùng cho shader",
        "v11_imagegen_vfx_atlas.png",
        "Không cần mô phỏng fluid đầy đủ để làm VFX tốt, nhưng cần hiểu buoyancy, turbulence, density và cooling.",
        [
            ("Lửa là vùng phản ứng, không chỉ là texture cam",
             [
                 "Lửa thật là vùng khí nóng phát sáng do phản ứng hóa học. Trong realtime VFX, ta thường giả bằng flipbook, emissive gradient, alpha mask, distortion và particle motion. Nhưng nếu hiểu vật lý, bạn sẽ biết nên đặt gì ở đâu.",
                 "Vùng nóng nhất thường sáng nhất và có thể gần trắng/vàng. Vùng ngoài nguội hơn chuyển cam/đỏ rồi thành smoke. Motion đi lên do buoyancy: khí nóng nhẹ hơn không khí xung quanh nên bốc lên. Turbulence làm mép lửa vỡ và cuộn.",
                 "Một fire card phẳng thường hỏng vì thiếu phân lớp nhiệt: core, lick, smoke, distortion, sparks. Thêm noise không thay thế được hierarchy này."
             ]),
            ("Khói là density và ánh sáng",
             [
                 "Khói không phát sáng như lửa. Nó che, tán xạ và nhận ánh sáng. Shader smoke thường cần alpha mềm, depth fade, soft intersection và variation theo lighting hoặc view. Nếu dùng emissive quá nhiều, smoke thành magic cloud.",
                 "Density quyết định khói dày hay mỏng. Nhưng opacity trong shader không tương đương hoàn toàn density vật lý. Trong production, bạn giả density bằng alpha, texture contrast, particle count và layering. Quá nhiều layer gây overdraw và làm smoke bẩn.",
                 "Smoke tốt thường có silhouette lớn mềm, detail nhỏ vừa đủ, motion chậm hơn fire và fade theo lifetime. Nếu mọi frame đều cùng contrast, smoke nhìn như sprite sheet bị phát lại."
             ]),
            ("Turbulence và noise có scale vật lý",
             [
                 "Turbulence trong fire/smoke có eddy lớn và nhỏ. Noise lớn điều khiển shape tổng thể. Noise nhỏ thêm chi tiết. Nếu noise nhỏ mạnh quá, effect bị bẩn; nếu chỉ có noise lớn, effect thiếu sống.",
                 "Motion cũng có scale. Lửa nhỏ flicker nhanh. Smoke lớn trôi chậm. Explosion có phase đầu rất nhanh rồi giảm. Khi timing sai, effect nhìn giả dù texture đẹp.",
                 "Hãy nghĩ theo năng lượng: nguồn năng lượng mạnh tạo motion nhanh, sáng hơn, distortion mạnh hơn. Khi năng lượng giảm, color nguội, alpha tan, motion chậm lại hoặc khuếch tán."
             ]),
        ],
        ("Ghi nhớ", "Lửa, khói và explosion nên được tune theo energy over time: nóng lên, bốc lên, vỡ ra, nguội đi, tan đi.", False),
        [
            "Một fire loop nhìn giả thường do mọi frame có cùng energy. Hãy dùng curve để core sáng giảm/tăng theo nhịp và smoke xuất hiện ở cuối.",
            "Một smoke impact quá đắt có thể do cố giả density bằng nhiều layer alpha. Texture tốt và particle ít hơn thường rẻ hơn."
        ],
        [
            "heat = EmissiveCore * LifetimeCooling",
            "buoyancyOffset = WorldUp * Heat * RiseStrength",
            "turbulence = NoiseLarge * 0.7 + NoiseSmall * 0.3"
        ],
    ),
    ch(
        "Nước, bọt và khúc xạ - surface physics cho VFX",
        "v9_failure_depthfade_before_after.png",
        "Water VFX thường xoay quanh normal, Fresnel, depth, refraction, foam và flow.",
        [
            ("Nước đọc bằng normal và reflection nhiều hơn màu",
             [
                 "Nước trong suốt hoặc bán trong suốt thường không có màu mạnh như vật rắn. Người xem nhận ra nước qua phản xạ, khúc xạ, normal ripple, Fresnel và tương tác với bờ/đá/vật thể. Một material nước xanh đơn giản dễ nhìn như gel nếu thiếu các tín hiệu này.",
                 "Normal map hoặc procedural normal tạo cảm giác mặt nước gợn. Flow map làm gợn chuyển động có hướng. Fresnel làm cạnh nhìn xiên sáng hơn. Depth làm nước đổi màu theo độ sâu. Những lớp này cùng nhau tạo cảm giác nước.",
                 "Với stylized water, bạn có thể đơn giản hóa nhưng không nên bỏ quan hệ chính: surface motion, edge foam, depth/readability và highlight."
             ]),
            ("Foam là nơi nước tương tác",
             [
                 "Foam thường xuất hiện ở nơi nước va vào bờ, vật thể, sóng gãy hoặc vùng chuyển động mạnh. Trong shader, foam có thể đến từ depth difference, distance field, vertex color, texture mask hoặc particle event.",
                 "Foam tốt không chỉ là noise trắng. Nó có birth, motion và dissolve. Nó xuất hiện ở edge, di chuyển một chút, vỡ ra rồi tan. Nếu foam luôn đứng yên, surface mất cảm giác vật lý.",
                 "DepthFade và SceneDepth giúp tạo foam quanh giao tuyến, nhưng cần tune theo scale. Nếu distance quá lớn, foam thành halo giả. Nếu quá nhỏ, foam cắt cứng."
             ]),
            ("Refraction và distortion có giới hạn",
             [
                 "Refraction làm background bị lệch khi nhìn qua nước hoặc heat haze. Nó rất hữu ích nhưng dễ bị lạm dụng. Distortion mạnh làm gameplay khó đọc, UI méo, sorting lộ hoặc cost tăng.",
                 "Vật lý trực quan: khúc xạ mạnh khi normal thay đổi mạnh và khi môi trường có contrast phía sau. Trong game, distortion nên phục vụ cảm giác vật liệu, không làm người chơi khó chịu.",
                 "Debug bằng cách tắt color và chỉ xem distortion. Nếu silhouette hoặc background méo quá mức, giảm strength hoặc giới hạn bằng mask."
             ]),
        ],
        ("Ghi nhớ", "Nước không chỉ là màu xanh. Nó là surface normal, Fresnel, depth, foam và motion có hướng.", False),
        [
            "Một water ribbon stylized thường cần edge foam rõ hơn refraction phức tạp. Trong gameplay nhanh, foam silhouette đọc tốt hơn distortion tinh tế.",
            "Một heat haze dùng distortion toàn card sẽ làm méo cả silhouette. Hãy mask distortion vào vùng nóng thật."
        ],
        [
            "foam = smoothstep(FoamStart, FoamEnd, DepthDifference)",
            "distUV = ScreenUV + NormalRG * RefractionStrength * Mask",
            "color = lerp(DeepColor, ShallowColor, depth01) + foam"
        ],
    ),
    ch(
        "Va chạm, impulse và hit response",
        "v9_space_vectors.png",
        "Nhiều VFX cần phản ứng với một điểm va chạm: shield hit, ripple, scorch, projectile impact, ground shockwave.",
        [
            ("Impact là dữ liệu không gian và thời gian",
             [
                 "Một hit response thường cần ít nhất ba dữ liệu: vị trí impact, thời điểm impact và cường độ impact. Material có thể dùng vị trí để tính distance field, thời điểm để tính phase, cường độ để scale emissive/WPO/distortion.",
                 "Nếu chỉ có texture animation, effect khó bám đúng điểm hit. Nếu có Impact Position trong Material Parameter Collection, Dynamic Parameter hoặc render target, shader có thể phản ứng đúng vị trí.",
                 "Trong Niagara, hit event có thể spawn particle ở điểm va chạm. Trong Material, hit point có thể tạo ripple trên shield mesh. Hai cách không đối lập; chúng giải quyết hai lớp khác nhau."
             ]),
            ("Impulse và decay",
             [
                 "Vật lý trực quan của va chạm là impulse mạnh rồi decay. Lúc đầu sáng, méo, rung hoặc nở nhanh. Sau đó năng lượng giảm, ring lan ra, glow tắt, smoke/dust còn lại. Nếu effect chỉ bật/tắt, nó thiếu cảm giác lực.",
                 "Decay có thể là exponential, smoothstep hoặc curve trong Niagara. Với material-only response, bạn có thể dùng `age = Time - HitTime`, rồi tính intensity giảm theo age. Ring radius tăng theo age, width có thể mở rộng, alpha giảm.",
                 "Một hit response tốt thường có nhiều layer thời gian: flash cực ngắn, ring trung bình, afterglow dài hơn. Layering theo thời gian làm impact có lực mà không cần shader quá phức tạp."
             ]),
            ("Direction của impact",
             [
                 "Không phải va chạm nào cũng radial đều. Projectile có hướng. Slash có hướng. Explosion có normal bề mặt. Nếu bạn đưa direction vào effect, visual có thể kể câu chuyện rõ hơn: sparks bay theo hướng, scorch kéo dài, shield ripple lệch.",
                 "Dot giữa impact direction và surface normal giúp scale intensity theo góc. Projection vector lên mặt phẳng giúp tạo decal direction. Cross Product giúp dựng side axis cho slash mark.",
                 "Khi không có direction, radial response là fallback tốt. Khi có direction, hãy dùng nó để tăng specificity của effect."
             ]),
        ],
        ("Ghi nhớ", "Hit response cần position, time và energy. Có direction thì càng tốt.", False),
        [
            "Một shield hit chỉ đổi màu toàn bộ mesh sẽ yếu. Tính distance từ hit point để tạo ring local làm nó thuyết phục hơn.",
            "Một ground impact có dust radial nhưng không có upward impulse sẽ thiếu lực. Particle motion và material ring nên cùng kể một hướng năng lượng."
        ],
        [
            "age = Time - HitTime",
            "radius = age * WaveSpeed",
            "ring = 1 - smoothstep(Width, Width + Soft, abs(distance(WorldPos, HitPos) - radius))",
            "intensity = exp(-age * Decay)"
        ],
    ),
    ch(
        "Noise, randomness và xác suất thị giác",
        "v9_mask_solo_dissolve.png",
        "Random trong VFX không phải vô trật tự. Nó là cách phá lặp có kiểm soát.",
        [
            ("Random cần distribution",
             [
                 "Một random value 0..1 có distribution. Nếu phân bố đều, mọi giá trị có xác suất gần nhau. Nhưng visual tự nhiên thường không đều. Spark nhỏ nhiều hơn spark lớn. Smoke mềm nhiều hơn edge sắc. Flicker nhẹ thường xuyên hơn flash cực mạnh.",
                 "Vì vậy, sau random thường cần shaping. `pow(rand, 2)` làm giá trị lớn hiếm hơn. `sqrt(rand)` làm giá trị cao xuất hiện nhiều hơn. Curve biến random thành distribution có ý đồ.",
                 "Khi effect nhìn quá đều, có thể thiếu random. Khi effect nhìn bẩn, có thể random không được shape hoặc không được giới hạn."
             ]),
            ("Noise là random có tương quan không gian",
             [
                 "White noise thay đổi ngẫu nhiên từng pixel, thường quá gắt. Perlin/Voronoi/Simplex noise có tương quan không gian: điểm gần nhau có giá trị liên quan. Đây là lý do noise tạo mảng, cell, cloud hoặc organic edge.",
                 "Một dissolve tốt thường cần noise có scale phù hợp và distribution hợp threshold. Một fire cần noise chạy theo hướng motion. Một smoke cần noise mềm hơn và ít contrast hơn. Noise không có vai trò cụ thể sẽ làm effect bẩn.",
                 "Hãy phân biệt noise dùng cho shape, noise dùng cho detail, noise dùng cho motion và noise dùng cho flicker. Mỗi loại cần scale, speed và contrast khác nhau."
             ]),
            ("Random theo particle và random theo pixel",
             [
                 "Random theo particle giữ mỗi particle có variation riêng: size, color, lifetime, frame offset. Random theo pixel tạo texture/detail bên trong particle. Hai cấp này khác nhau. Nếu mọi particle dùng cùng texture phase, effect lặp. Nếu random theo pixel quá mạnh, particle mất shape.",
                 "Niagara rất mạnh ở random theo particle. Material rất mạnh ở random theo pixel/UV/world position. Kết hợp đúng tạo variation có cấu trúc.",
                 "Một quy tắc tốt: random lớn ở cấp particle để phá lặp tổng thể, random nhỏ ở material để phá cạnh và detail."
             ]),
        ],
        ("Ghi nhớ", "Random đẹp là random có distribution, scale và vai trò. Random vô kiểm soát chỉ tạo nhiễu.", False),
        [
            "Sparkle iGaming cần random timing nhưng vẫn phải đọc nhịp reward. Nếu random quá đều, nó máy móc; nếu quá loạn, nó mất cảm giác thiết kế.",
            "Dissolve organic cần noise tốt hơn, không nhất thiết cần nhiều noise hơn."
        ],
        [
            "r = Random01(ParticleID)",
            "size = lerp(MinSize, MaxSize, pow(r, 2))",
            "flicker = lerp(0.8, 1.3, Noise(UV + Time))"
        ],
    ),
    ch(
        "Sampling theory - vì sao hình bị răng cưa, rung và moiré",
        "v9_power_shaping_flare.png",
        "GPU lấy mẫu hình ảnh rời rạc. Nếu tín hiệu quá nhỏ hoặc quá nhanh, nó không thể được tái tạo sạch.",
        [
            ("Pixel là mẫu, không phải thế giới liên tục",
             [
                 "Shader có thể nghĩ bằng công thức liên tục, nhưng màn hình là lưới pixel. Một line mỏng hơn pixel sẽ lúc hiện lúc mất. Một pattern dày quá sẽ tạo moiré. Một texture chi tiết cao ở xa sẽ shimmer. Đây không phải lỗi nhỏ; đó là hệ quả của sampling.",
                 "Nyquist nói đơn giản: để tái tạo một tín hiệu, tần số lấy mẫu phải đủ cao so với tần số tín hiệu. Trong VFX, nếu stripe, ring, noise hoặc sparkle có chi tiết nhỏ hơn khả năng sampling của pixel, nó sẽ alias.",
                 "Anti-aliasing là làm tín hiệu phù hợp với pixel footprint. Mip map, fwidth, smooth edge, lower frequency và LOD đều là cách giảm tín hiệu quá cao."
             ]),
            ("Temporal sampling",
             [
                 "Game không chỉ lấy mẫu theo không gian, mà còn theo thời gian. 60 FPS nghĩa là mỗi frame cách nhau 16.6 ms. Nếu effect đổi quá nhanh giữa hai frame, motion có thể giật, strobe hoặc biến mất.",
                 "Một flicker quá nhanh có thể nhìn như noise lỗi. Một scanline chạy quá nhanh có thể alias theo thời gian. Một flipbook frame rate không hợp có thể giật. TAA có thể làm mờ hoặc ghost effect nhỏ.",
                 "Vì vậy, timing phải được test trong motion. Screenshot không đủ. Một effect đẹp khi pause có thể tệ khi camera và animation chạy."
             ]),
            ("LOD là sampling có trách nhiệm",
             [
                 "Khi effect ở xa, người xem không thấy chi tiết nhỏ. GPU vẫn phải trả tiền nếu bạn giữ mọi layer. LOD không chỉ là performance; nó còn giúp giảm aliasing. Bỏ chi tiết nhỏ ở xa làm hình ổn định hơn.",
                 "Texture mip là LOD tự động cho texture. Particle LOD, material quality switch và shader variant là LOD ở cấp hệ thống. Một Technical VFX Artist nên thiết kế effect có chất lượng giảm dần, không đột ngột gãy.",
                 "Nếu detail không đọc ở kích thước gameplay, hãy bỏ hoặc bake. Chất lượng không nằm ở số lớp, mà ở những lớp người xem thật sự đọc được."
             ]),
        ],
        ("Ghi nhớ", "Aliasing là dấu hiệu tín hiệu vượt quá khả năng lấy mẫu. Sửa bằng cách giảm tần số, tăng width, dùng mip/fwidth/LOD.", False),
        [
            "Một magic circle line art cần width theo distance hoặc fwidth. Texture độ phân giải cao không cứu line nhỏ hơn pixel.",
            "Một sparkle field nhấp nháy có thể cần giới hạn tốc độ flicker hoặc random phase mềm hơn."
        ],
        [
            "pixelWidth = fwidth(value)",
            "softEdge = smoothstep(threshold - pixelWidth, threshold + pixelWidth, value)",
            "visibleDetail = detail * DistanceLODFade"
        ],
    ),
    ch(
        "Ổn định số và defensive shader",
        None,
        "",
        [
            ("Shader cũng cần phòng thủ",
             [
                 "Một shader production không chỉ chạy đúng với input đẹp. Nó phải chịu được input xấu vừa phải: vector gần zero, texture missing, parameter ngoài range, camera rất gần, particle scale bằng 0, time lớn, hoặc platform precision thấp.",
                 "Defensive shader không có nghĩa là nhồi Clamp khắp nơi. Nó nghĩa là biết chỗ nào có rủi ro. Divide cần bảo vệ mẫu số. Normalize cần SafeNormalize. Power cần input hợp lệ. Remap cần min/max không trùng. POM cần step count hợp lý.",
                 "Nếu không phòng thủ, lỗi có thể rất khó tìm: pixel đen, NaN-like artifact, mesh biến mất, bloom nổ, hoặc shader compile variant không ổn."
             ]),
            ("Clamp đúng chỗ",
             [
                 "Clamp quá sớm làm mất dynamic range. Clamp quá muộn có thể để giá trị xấu lan qua nhiều phép toán. Hãy clamp ở boundary của contract: sau khi remap input về 0..1, trước khi đưa vào Power nhạy, hoặc trước output alpha/emissive nếu cần giới hạn.",
                 "Saturate rất tiện nhưng cũng dễ che lỗi. Nếu một value thường xuyên bị saturate ở 0 hoặc 1, có thể range đầu vào sai. Debug bằng heatmap hoặc grayscale để xem giá trị có bị kẹp quá nhiều không.",
                 "Một parameter UI nên có min/max hợp lý. Đừng để artist kéo DistortionStrength tới 100 nếu shader không được thiết kế cho range đó."
             ]),
            ("NaN, Inf và artifact khó chịu",
             [
                 "Trong shader, chia cho zero, normalize zero vector, sqrt giá trị âm, log giá trị không hợp lệ hoặc power với input âm có thể tạo kết quả không ổn. Tùy engine/platform, nó có thể thành đen, trắng, spark artifact hoặc behavior khó đoán.",
                 "HLSL custom node càng cần cẩn thận. Node graph đôi khi có node SafeNormalize hoặc Saturate tiện sẵn; code tự viết thì bạn phải tự bảo vệ. Một dòng nhỏ như `max(x, 1e-4)` có thể cứu nhiều case.",
                 "Defensive shader là một phần của seniority. Không phải vì bạn sợ toán, mà vì production input luôn bẩn hơn demo."
             ]),
        ],
        ("Ghi nhớ", "Shader tốt không chỉ đúng với case đẹp. Nó có boundary rõ và không sụp khi input hơi xấu.", False),
        [
            "Một material beam bị artifact khi length gần zero cần SafeNormalize hoặc fallback direction.",
            "Một dissolve Power trên mask âm sau Subtract có thể cho kết quả sai. Saturate hoặc remap trước khi Power."
        ],
        [
            "safeDenom = max(abs(denom), 1e-4)",
            "value = numerator / safeDenom",
            "dir = length(v) > 1e-4 ? normalize(v) : fallbackDir",
            "mask = pow(saturate(mask), Power)"
        ],
    ),
    ch(
        "Từ vật lý sang art direction - biết đủ để bẻ luật",
        "v11_imagegen_vfx_atlas.png",
        "VFX game không cần đúng vật lý tuyệt đối. Nó cần đúng cảm giác, đúng gameplay và đúng ngân sách.",
        [
            ("Vật lý là điểm tựa, không phải nhà tù",
             [
                 "Nếu làm fire, hiểu nhiệt và buoyancy giúp bạn biết lửa nên bốc lên, sáng ở core và nguội dần. Nhưng stylized fire có thể đi ngang, đổi màu xanh, hoặc tạo shape như brush stroke. Nó bẻ luật nhưng vẫn thuyết phục nếu có logic nội bộ.",
                 "Nếu làm portal, vật lý thật không có portal fantasy rõ ràng. Nhưng bạn vẫn dùng toán/vật lý trực quan: swirl, inward flow, energy buildup, rim, distortion, depth cue. Người xem không cần công thức thật; họ cần cảm giác hệ thống có luật.",
                 "Master shader không phải là người luôn làm đúng physics. Đó là người biết physics đủ sâu để chọn luật nào giữ, luật nào bỏ, và bỏ với lý do art direction rõ."
             ]),
            ("Gameplay thắng simulation",
             [
                 "Một effect đúng vật lý nhưng che enemy attack là effect tệ. Một smoke realistic nhưng làm mất readability trong combat cần giảm opacity, đổi color, rút lifetime hoặc thay motion. Game VFX phục vụ trải nghiệm tương tác.",
                 "Material cần đọc trong camera thật, FOV thật, speed thật. Nếu visual đẹp trong turntable nhưng không đọc trong gameplay, shader chưa hoàn thành.",
                 "Vì vậy, mỗi quyết định vật lý phải đi qua ba câu hỏi: người chơi có đọc được không, gameplay có bị cản không, performance có chịu nổi không?"
             ]),
            ("Ngọn ngành để ứng biến feedback",
             [
                 "Khi lead nói 'mạnh hơn', người chỉ biết recipe sẽ tăng intensity. Người hiểu ngọn ngành sẽ hỏi mạnh hơn ở layer nào: core sáng hơn, silhouette rõ hơn, motion nhanh hơn, impact có impulse hơn, hay color contrast tốt hơn?",
                 "Khi feedback là 'organic hơn', bạn có thể nghĩ tới noise scale, field motion, phase offset, breakup ở edge, variation theo particle, hoặc timing decay. Không cần hỏi công thức mới; bạn dịch feedback thành biến kỹ thuật.",
                 "Đó là lý do học toán và vật lý đáng hơn học thuộc recipe. Recipe cho bạn một kết quả. Nguyên lý cho bạn khả năng tạo kết quả mới."
             ]),
        ],
        ("Ghi nhớ", "Hiểu nguyên lý để bẻ luật có kiểm soát. VFX game cần thuyết phục, đọc được và chạy được.", False),
        [
            "Một stylized slash không cần motion blur vật lý đúng, nhưng cần hướng lực, taper, timing và value contrast đúng.",
            "Một black hole fantasy không cần mô phỏng hấp dẫn thật, nhưng cần inward flow, lensing/distortion, core hierarchy và particle scale cue."
        ],
        [
            "Feedback -> Layer:",
            "stronger  -> core / silhouette / timing / contrast",
            "organic   -> noise scale / phase offset / field variation",
            "heavier   -> slower decay / stronger impulse / larger mass cue"
        ],
    ),
]


base.cover = cover_v25

# Insert the math/physics core after the first two conceptual chapters so the book
# moves from visual thinking into principles before returning to production/atlas.
original = list(base.CHAPTERS)
insert_at = 2
base.CHAPTERS = original[:insert_at] + MATH_PHYSICS_CHAPTERS + original[insert_at:]


if __name__ == "__main__":
    base.build()
