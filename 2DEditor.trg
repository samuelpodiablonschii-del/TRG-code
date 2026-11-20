// TRG Studio — touch-first 2D UI designer
// The entire editor layout and its tools are authored in TRG.
add (ui)

color accent = #9B6BFF
color panel = #171421
color canvas = #FFFFFF
word selectedName = ""
word currentColor = #8B5CF6
word backgroundColor = "#FFFFFF"
word currentShape = "square"
word renameInput = ""
word addLabel = ""
word widthChange = ""
word heightChange = ""
word animDx = ""
word animDy = ""
word animTime = ""
list objects
num objectCount = 0
num selectedIndex = 0

block z 0 x 0 y 0 w 100 h 100 color #FFFFFF name EDITOR_BACKGROUND
block z 1 x 0 y 0 w 100 h 1 color accent name EDITOR_ACCENT
block z 1 x 3 y 13 w 94 h 49 color panel edges round name CANVAS_FRAME
block z 2 x 4 y 14 w 92 h 47 color canvas edges round name CANVAS
block z 3 x 4 y 14 w 92 h 0.25 color #282238 name GRID_TOP
block z 3 x 4 y 60 w 92 h 0.25 color #282238 name GRID_BOTTOM
block z 3 x 4 y 14 w 0.25 h 47 color #282238 name GRID_LEFT
block z 3 x 95 y 14 w 0.25 h 47 color #282238 name GRID_RIGHT
block z 3 x 15 y 14 w 0.2 h 47 color #211B30 name GRID_V1
block z 3 x 26 y 14 w 0.2 h 47 color #211B30 name GRID_V2
block z 3 x 37 y 14 w 0.2 h 47 color #211B30 name GRID_V3
block z 3 x 48 y 14 w 0.2 h 47 color #211B30 name GRID_V4
block z 3 x 59 y 14 w 0.2 h 47 color #211B30 name GRID_V5
block z 3 x 70 y 14 w 0.2 h 47 color #211B30 name GRID_V6
block z 3 x 81 y 14 w 0.2 h 47 color #211B30 name GRID_V7
block z 3 x 4 y 24 w 92 h 0.2 color #211B30 name GRID_H1
block z 3 x 4 y 34 w 92 h 0.2 color #211B30 name GRID_H2
block z 3 x 4 y 44 w 92 h 0.2 color #211B30 name GRID_H3
block z 3 x 4 y 54 w 92 h 0.2 color #211B30 name GRID_H4

text "TRG / DESIGN" x 4 y 7 size 3.3 color #211A30 bold true
text "2D CANVAS" x 4 y 11 size 1.5 color #514963
text "TAP TO SELECT  •  DRAG TO MOVE" x 95 y 11 size 1.25 color #514963 align right
text object_label "No object selected" x 4 y 65 size 2.2 color #211A30 bold true
text object_count "0 OBJECTS" x 95 y 65 size 1.4 color #514963 align right

ui create block auto x 38 y 30 w 14 h 13 color accent shape square
push objects last_ui_object
length objects objectCount
selectedIndex = 0
get objects selectedIndex selectedName
set_text object_label selectedName
set_text object_count objectCount

// Creation tools
button add_block "＋ BLOCK" x 3 y 68 w 22 h 7 color #56368F (
    ui create block auto x 38 y 30 w 14 h 12 color currentColor shape square
    push objects last_ui_object
    length objects objectCount
    selectedIndex = objectCount - 1
    get objects selectedIndex selectedName
    set_text object_label selectedName
    set_text object_count objectCount
)
button add_circle "○ CIRCLE" x 27 y 68 w 22 h 7 color #30243F (
    ui create block auto x 38 y 30 w 14 h 12 color currentColor shape circle
    push objects last_ui_object
    length objects objectCount
    selectedIndex = objectCount - 1
    get objects selectedIndex selectedName
    set_text object_label selectedName
    set_text object_count objectCount
)
button add_button "＋ BUTTON" x 51 y 68 w 22 h 7 color #30243F (
    ask "Button object name:" renameInput
    ask "Button label:" addLabel
    button renameInput addLabel x 38 y 31 w 24 h 9 color currentColor ( say "Button clicked" )
    push objects renameInput
    length objects objectCount
    selectedIndex = objectCount - 1
    selectedName = renameInput
    set_text object_label selectedName
    set_text object_count objectCount
)
button add_text "＋ TEXT" x 75 y 68 w 22 h 7 color #30243F (
    ask "Text object name:" renameInput
    ask "Text to display:" addLabel
    text renameInput addLabel x 38 y 35 size 3.2 color #211A30
    push objects renameInput
    length objects objectCount
    selectedIndex = objectCount - 1
    selectedName = renameInput
    set_text object_label selectedName
    set_text object_count objectCount
)

// Selection and properties
button previous "‹" x 3 y 77 w 10 h 7 color #30243F (
    if objectCount > 0 (
        selectedIndex = selectedIndex - 1
        if selectedIndex < 0 (
            selectedIndex = objectCount - 1
        )
        get objects selectedIndex selectedName
        set_text object_label selectedName
    )
)
button next "›" x 15 y 77 w 10 h 7 color #30243F (
    if objectCount > 0 (
        selectedIndex = selectedIndex + 1
        if selectedIndex >= objectCount (
            selectedIndex = 0
        )
        get objects selectedIndex selectedName
        set_text object_label selectedName
    )
)
button name "NAME" x 27 y 77 w 15 h 7 color #30243F (
    if objectCount > 0 (
        ask "New object name (use letters, numbers and _):" renameInput
        ui rename selectedName renameInput
        remove objects selectedIndex
        insert objects selectedIndex renameInput
        selectedName = renameInput
        set_text object_label selectedName
    )
)
button resize "RESIZE" x 44 y 77 w 16 h 7 color #56368F (
    if objectCount > 0 (
        ask "Size change (+/-):" widthChange
        ui resize selectedName widthChange widthChange
    )
)
button color "COLOR" x 62 y 77 w 15 h 7 color #30243F (
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
        ui color selectedName currentColor
    )
)
button shape "SHAPE" x 79 y 77 w 18 h 7 color #30243F (
    if objectCount > 0 (
        if currentShape = "square" ( currentShape = "circle" )
        else ( if currentShape = "circle" ( currentShape = "triangle" )
        else ( if currentShape = "triangle" ( currentShape = "pentagon" )
        else ( if currentShape = "pentagon" ( currentShape = "hexagon" )
        else ( if currentShape = "hexagon" ( currentShape = "star" )
        else ( currentShape = "square" ) ) ) ) )
        ui shape selectedName currentShape
    )
)

// Object actions
button duplicate "DUPLICATE" x 3 y 86 w 21 h 7 color #30243F (
    if objectCount > 0 (
        ui duplicate selectedName auto
        push objects last_ui_object
        length objects objectCount
        selectedIndex = objectCount - 1
        get objects selectedIndex selectedName
        set_text object_label selectedName
        set_text object_count objectCount
    )
)
button delete "DELETE" x 26 y 86 w 17 h 7 color #652A45 (
    if objectCount > 0 (
        ui delete selectedName
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
            set_text object_label "No object selected"
        )
        set_text object_count objectCount
    )
)
button copy_code "COPY CODE" x 45 y 86 w 23 h 7 color #56368F (
    say "Tap an object, then use COPY CODE to copy its TRG source."
)
button animate "ANIMATE" x 70 y 86 w 27 h 7 color #30243F (
    if objectCount > 0 (
        ask "Animation X movement:" animDx
        ask "Animation Y movement:" animDy
        ask "Animation duration (seconds):" animTime
        animate selectedName x animDx y animDy fps 60 time animTime
    )
)

button background "BACKGROUND" x 3 y 94 w 25 h 5 color #56368F (\n    ask "Canvas background colour (#RRGGBB):" backgroundColor\n    ui color CANVAS backgroundColor\n)\nbutton exit "EXIT" x 73 y 94 w 24 h 5 color #652A45 (\n    studio code\n)\ntext "Drag objects to move them. SHAPE cycles through six patterns." x 31 y 97 size 1.1 color #514963
