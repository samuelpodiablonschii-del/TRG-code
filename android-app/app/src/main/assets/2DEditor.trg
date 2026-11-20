// TRG Studio — 2D visual editor
// The editor interface and all editing controls are TRG UI.
add (ui)

color accent = #8B5CF6
color panel = #171421
color canvas = #101019
word selectedName = ""
word currentColor = #8B5CF6
word currentShape = "square"
word renameInput = ""
list objects
num objectCount = 0
num selectedIndex = 0

block z 0 x 0 y 0 w 100 h 100 color #0D0B12 name EDITOR_BACKGROUND
block z 1 x 0 y 0 w 100 h 1 color #8B5CF6 name EDITOR_ACCENT
block z 1 x 3 y 13 w 94 h 50 color panel edges round name CANVAS_FRAME
block z 2 x 4 y 14 w 92 h 48 color canvas edges round name CANVAS

// Subtle editing grid.
block z 3 x 15 y 15 w 0.25 h 46 color #252235 name GRID_V1
block z 3 x 26 y 15 w 0.25 h 46 color #252235 name GRID_V2
block z 3 x 37 y 15 w 0.25 h 46 color #252235 name GRID_V3
block z 3 x 48 y 15 w 0.25 h 46 color #252235 name GRID_V4
block z 3 x 59 y 15 w 0.25 h 46 color #252235 name GRID_V5
block z 3 x 70 y 15 w 0.25 h 46 color #252235 name GRID_V6
block z 3 x 81 y 15 w 0.25 h 46 color #252235 name GRID_V7
block z 3 x 5 y 24 w 90 h 0.25 color #252235 name GRID_H1
block z 3 x 5 y 34 w 90 h 0.25 color #252235 name GRID_H2
block z 3 x 5 y 44 w 90 h 0.25 color #252235 name GRID_H3
block z 3 x 5 y 54 w 90 h 0.25 color #252235 name GRID_H4

text "2D EDITOR" x 4 y 8 size 4 color #FFFFFF bold true
text "Design objects • select, move, resize and recolour" x 4 y 11 size 1.7 color #B9B2CA
text object_label "No object selected" x 5 y 69 size 3 color #FFFFFF bold true
text object_count "0 OBJECTS" x 95 y 69 size 2 color #A69CB8 align right

ui create block auto x 43 y 31 w 14 h 13 color accent shape square z 10
push objects last_ui_object
length objects objectCount
selectedIndex = 0
get objects selectedIndex selectedName
set_text object_label selectedName
set_text object_count objectCount

button add_block "＋ BLOCK" x 3 y 71 w 22 h 7 color #5A36A0 (
    ui create block auto x 35 y 32 w 14 h 13 color currentColor shape square z 10
    push objects last_ui_object
    length objects objectCount
    selectedIndex = objectCount - 1
    get objects selectedIndex selectedName
    set_text object_label selectedName
    set_text object_count objectCount
)

button add_circle "○ CIRCLE" x 27 y 71 w 22 h 7 color #34264B (
    ui create block auto x 35 y 32 w 14 h 13 color currentColor shape circle z 10
    push objects last_ui_object
    length objects objectCount
    selectedIndex = objectCount - 1
    get objects selectedIndex selectedName
    set_text object_label selectedName
    set_text object_count objectCount
)

button duplicate "COPY" x 51 y 71 w 21 h 7 color #34264B (
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

button delete "DELETE" x 74 y 71 w 23 h 7 color #5D273C (
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

button rename "RENAME" x 3 y 80 w 22 h 7 color #34264B (
    if objectCount > 0 (
        ask "New object name (letters/numbers/_):" renameInput
        ui rename selectedName renameInput
        remove objects selectedIndex
        insert objects selectedIndex renameInput
        selectedName = renameInput
        set_text object_label selectedName
    )
)

button previous "PREV" x 27 y 80 w 14 h 7 color #34264B (
    if objectCount > 0 (
        selectedIndex = selectedIndex - 1
        if selectedIndex < 0 (
            selectedIndex = objectCount - 1
        )
        get objects selectedIndex selectedName
        set_text object_label selectedName
    )
)

button next "NEXT" x 43 y 80 w 14 h 7 color #34264B (
    if objectCount > 0 (
        selectedIndex = selectedIndex + 1
        if selectedIndex >= objectCount (
            selectedIndex = 0
        )
        get objects selectedIndex selectedName
        set_text object_label selectedName
    )
)

button color "COLOR" x 59 y 80 w 17 h 7 color #34264B (
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

button shape "SHAPE" x 78 y 80 w 19 h 7 color #34264B (
    if objectCount > 0 (
        if currentShape = "square" (
            currentShape = "circle"
        )
        else (
            currentShape = "square"
        )
        ui shape selectedName currentShape
    )
)

button left "←" x 3 y 91 w 13 h 7 color #34264B (
    if objectCount > 0 (
        ui move selectedName -2 0
    )
)
button right "→" x 18 y 91 w 13 h 7 color #34264B (
    if objectCount > 0 (
        ui move selectedName 2 0
    )
)
button up "↑" x 33 y 91 w 13 h 7 color #34264B (
    if objectCount > 0 (
        ui move selectedName 0 -2
    )
)
button down "↓" x 48 y 91 w 13 h 7 color #34264B (
    if objectCount > 0 (
        ui move selectedName 0 2
    )
)
button enlarge "SCALE +" x 63 y 91 w 16 h 7 color #34264B (
    if objectCount > 0 (
        ui resize selectedName 2 2
    )
)
button shrink "SCALE -" x 81 y 91 w 16 h 7 color #34264B (
    if objectCount > 0 (
        ui resize selectedName -2 -2
    )
)
