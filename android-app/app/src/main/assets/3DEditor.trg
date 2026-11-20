// TRG Studio — 3D scene editor
// Scene, viewport UI, and all editing controls are implemented in TRG.
add (ui)
add (3d)

color accent = #8B5CF6
word selectedName = ""
word currentColor = #8B5CF6
num zMove = 0
word renameInput = ""
list objects
num objectCount = 0
num selectedIndex = 0

block z 0 x 0 y 0 w 100 h 100 color #0D0B12 name EDITOR_BACKGROUND
block z 1 x 0 y 0 w 100 h 1 color #8B5CF6 name EDITOR_ACCENT
block z 1 x 3 y 13 w 94 h 59 color #171421 edges round name VIEWPORT_FRAME
block z 2 x 4 y 14 w 92 h 57 color #101019 edges round name VIEWPORT
// Floor-plane guide lines.
block z 3 x 12 y 53 w 76 h 0.25 color #252235 name FLOOR_A
block z 3 x 17 y 59 w 66 h 0.25 color #252235 name FLOOR_B
block z 3 x 24 y 65 w 52 h 0.25 color #252235 name FLOOR_C
text "3D SCENE EDITOR" x 4 y 8 size 4 color #FFFFFF bold true
text "Create and transform 3D scene primitives" x 4 y 11 size 1.7 color #B9B2CA
text object_label "No model selected" x 5 y 74 size 2.8 color #FFFFFF bold true
text object_count "0 MODELS" x 95 y 74 size 1.9 color #A69CB8 align right

3d box auto x 0 y 0 z 0 w 3 h 3 d 3 color accent
push objects last_3d_object
length objects objectCount
selectedIndex = 0
get objects selectedIndex selectedName
set_text object_label selectedName
set_text object_count objectCount

button add_box "＋ BOX" x 3 y 78 w 22 h 6 color #5A36A0 (
    3d box auto x -1 y 0 z 0 w 2 h 2 d 2 color currentColor
    push objects last_3d_object
    length objects objectCount
    selectedIndex = objectCount - 1
    get objects selectedIndex selectedName
    set_text object_label selectedName
    set_text object_count objectCount
)
button add_sphere "SPHERE" x 27 y 78 w 22 h 6 color #34264B (
    3d sphere auto x 1 y 0 z 0 size 2 color currentColor
    push objects last_3d_object
    length objects objectCount
    selectedIndex = objectCount - 1
    get objects selectedIndex selectedName
    set_text object_label selectedName
    set_text object_count objectCount
)
button duplicate "COPY" x 51 y 78 w 21 h 6 color #34264B (
    if objectCount > 0 (
        3d duplicate selectedName auto
        push objects last_3d_object
        length objects objectCount
        selectedIndex = objectCount - 1
        get objects selectedIndex selectedName
        set_text object_label selectedName
        set_text object_count objectCount
    )
)
button delete "DELETE" x 74 y 78 w 23 h 6 color #5D273C (
    if objectCount > 0 (
        3d delete selectedName
        remove objects selectedIndex
        length objects objectCount
        if objectCount > 0 (
            if selectedIndex >= objectCount (
                selectedIndex = objectCount - 1
            )
            get objects selectedIndex selectedName
            set_text object_label selectedName
        )
        else (
            selectedName = ""
            set_text object_label "No model selected"
        )
        set_text object_count objectCount
    )
)
button rename "NAME" x 3 y 85 w 12 h 6 color #34264B (
    if objectCount > 0 (
        ask "New model name (letters/numbers/_):" renameInput
        3d rename selectedName renameInput
        remove objects selectedIndex
        insert objects selectedIndex renameInput
        selectedName = renameInput
        set_text object_label selectedName
    )
)

button previous "PREV" x 17 y 85 w 12 h 6 color #34264B (
    if objectCount > 0 (
        selectedIndex = selectedIndex - 1
        if selectedIndex < 0 (
            selectedIndex = objectCount - 1
        )
        get objects selectedIndex selectedName
        set_text object_label selectedName
    )
)
button next "NEXT" x 31 y 85 w 12 h 6 color #34264B (
    if objectCount > 0 (
        selectedIndex = selectedIndex + 1
        if selectedIndex >= objectCount (
            selectedIndex = 0
        )
        get objects selectedIndex selectedName
        set_text object_label selectedName
    )
)
button move_left "X -" x 45 y 85 w 10 h 6 color #34264B (
    if objectCount > 0 (
        3d move selectedName -0.5 0 0
    )
)
button move_right "X +" x 57 y 85 w 10 h 6 color #34264B (
    if objectCount > 0 (
        3d move selectedName 0.5 0 0
    )
)
button move_up "Y +" x 69 y 85 w 10 h 6 color #34264B (
    if objectCount > 0 (
        3d move selectedName 0 0.5 0
    )
)
button move_down "Y -" x 81 y 85 w 10 h 6 color #34264B (
    if objectCount > 0 (
        3d move selectedName 0 -0.5 0
    )
)
button rotate "ROTATE" x 3 y 92 w 17 h 6 color #34264B (
    if objectCount > 0 (
        3d rotate selectedName 0 15 0
    )
)
button scale_up "SCALE +" x 22 y 92 w 18 h 6 color #34264B (
    if objectCount > 0 (
        3d scale selectedName 1.1
    )
)
button scale_down "SCALE -" x 42 y 92 w 18 h 6 color #34264B (
    if objectCount > 0 (
        3d scale selectedName 0.9
    )
)
button color "COLOR" x 62 y 92 w 16 h 6 color #34264B (
    if objectCount > 0 (
        if currentColor = "#8B5CF6" (
            currentColor = "#3B82F6"
        )
        else (
            if currentColor = "#3B82F6" (
                currentColor = "#22C55E"
            )
            else (
                currentColor = "#8B5CF6"
            )
        )
        3d color selectedName currentColor
    )
)
button raise "Z +/-" x 80 y 92 w 17 h 6 color #34264B (
    if objectCount > 0 (
        if zMove = 0 (
            3d move selectedName 0 0 0.5
            zMove = 1
        )
        else (
            3d move selectedName 0 0 -0.5
            zMove = 0
        )
    )
)
