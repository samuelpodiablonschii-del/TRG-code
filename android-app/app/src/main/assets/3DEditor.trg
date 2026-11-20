// TRG Studio — touch-first 3D scene designer
// One-finger drag moves a model; drag empty space to orbit.
// Two fingers orbit and zoom at the same time.
add (ui)
add (3d)

color accent = #9B6BFF
word selectedName = ""
word currentColor = #8B5CF6
word backgroundColor = "#FFFFFF"
word renameInput = ""
word scaleFactor = ""
word rotateAmount = ""
list objects
num objectCount = 0
num selectedIndex = 0

block z 0 x 0 y 0 w 100 h 100 color #FFFFFF name EDITOR_BACKGROUND
block z 1 x 0 y 0 w 100 h 1 color accent name EDITOR_ACCENT
block z 1 x 3 y 13 w 94 h 54 color #171421 edges round name VIEWPORT_FRAME
block z 2 x 4 y 14 w 92 h 52 color #FFFFFF edges round name VIEWPORT
block z 3 x 9 y 56 w 82 h 0.2 color #E4E4EA name FLOOR_A
block z 3 x 15 y 60 w 70 h 0.2 color #E4E4EA name FLOOR_B
block z 3 x 22 y 64 w 56 h 0.2 color #E4E4EA name FLOOR_C

text "TRG / SCENE" x 4 y 7 size 3.3 color #211A30 bold true
text "3D VIEWPORT" x 4 y 11 size 1.5 color #514963
text "DRAG OBJECT  •  DRAG EMPTY SPACE TO ORBIT" x 95 y 11 size 1.2 color #514963 align right
text model_label "No model selected" x 4 y 70 size 2.2 color #211A30 bold true
text object_count "0 MODELS" x 95 y 70 size 1.4 color #514963 align right

3d box auto x 0 y 0 z 0 w 3 h 3 d 3 color accent
push objects last_3d_object
length objects objectCount
selectedIndex = 0
get objects selectedIndex selectedName
set_text model_label selectedName
set_text object_count objectCount

button add_box "＋ BOX" x 3 y 73 w 22 h 6 color #56368F (
    3d box auto x -2 y 0 z 0 w 2 h 2 d 2 color currentColor
    push objects last_3d_object
    length objects objectCount
    selectedIndex = objectCount - 1
    get objects selectedIndex selectedName
    set_text model_label selectedName
    set_text object_count objectCount
)
button add_sphere "＋ SPHERE" x 27 y 73 w 22 h 6 color #30243F (
    3d sphere auto x 2 y 0 z 0 size 2 color currentColor
    push objects last_3d_object
    length objects objectCount
    selectedIndex = objectCount - 1
    get objects selectedIndex selectedName
    set_text model_label selectedName
    set_text object_count objectCount
)
button duplicate "DUPLICATE" x 51 y 73 w 21 h 6 color #30243F (
    if objectCount > 0 (
        3d duplicate selectedName auto
        push objects last_3d_object
        length objects objectCount
        selectedIndex = objectCount - 1
        get objects selectedIndex selectedName
        set_text model_label selectedName
        set_text object_count objectCount
    )
)
button delete "DELETE" x 74 y 73 w 23 h 6 color #652A45 (
    if objectCount > 0 (
        3d delete selectedName
        remove objects selectedIndex
        length objects objectCount
        if objectCount > 0 (
            if selectedIndex >= objectCount (
                selectedIndex = objectCount - 1
            )
            get objects selectedIndex selectedName
            set_text model_label selectedName
        )
        else (
            selectedName = ""
            set_text model_label "No model selected"
        )
        set_text object_count objectCount
    )
)

button previous "‹" x 3 y 81 w 10 h 6 color #30243F (
    if objectCount > 0 (
        selectedIndex = selectedIndex - 1
        if selectedIndex < 0 (
            selectedIndex = objectCount - 1
        )
        get objects selectedIndex selectedName
        set_text model_label selectedName
    )
)
button next "›" x 15 y 81 w 10 h 6 color #30243F (
    if objectCount > 0 (
        selectedIndex = selectedIndex + 1
        if selectedIndex >= objectCount (
            selectedIndex = 0
        )
        get objects selectedIndex selectedName
        set_text model_label selectedName
    )
)
button name "NAME" x 27 y 81 w 15 h 6 color #30243F (
    if objectCount > 0 (
        ask "New model name:" renameInput
        3d rename selectedName renameInput
        remove objects selectedIndex
        insert objects selectedIndex renameInput
        selectedName = renameInput
        set_text model_label selectedName
    )
)
button resize "RESIZE" x 44 y 81 w 17 h 6 color #56368F (
    if objectCount > 0 (
        ask "Scale factor (e.g. 1.2 or 0.8):" scaleFactor
        3d scale selectedName scaleFactor
    )
)
button color "COLOR" x 63 y 81 w 15 h 6 color #30243F (
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
button rotate "ROTATE" x 80 y 81 w 17 h 6 color #30243F (
    if objectCount > 0 (
        ask "Rotate around Y in degrees:" rotateAmount
        3d rotate selectedName 0 rotateAmount 0
    )
)
button copy_code "COPY CODE" x 3 y 89 w 28 h 6 color #56368F (
    say "Tap a model, then use COPY CODE to copy its TRG source."
)
button reset_view "RESET VIEW" x 33 y 89 w 28 h 6 color #30243F (
    say "Camera reset is handled by the Android viewport."
)
button info "CONTROLS" x 63 y 89 w 34 h 6 color #30243F (
    say "Drag a model to move it. Drag empty space to look around. Pinch and move two fingers to orbit and zoom."
)
text "PINCH TO ZOOM  •  TWO-FINGER DRAG TO LOOK AROUND" x 4 y 98 size 1.15 color #514963
