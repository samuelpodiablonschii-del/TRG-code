#pragma once

// Built-in TRG Studio visual editors. Compiled into mobile and desktop builds.
namespace trg_builtin {
inline constexpr const char* editor2D = R"TRGEDITOR2D(// TRG Studio — Bouncy Balls-inspired 2D scene editor
// A canvas-first editor with a compact tool rail and an object inspector.
add (ui)

color accent = #9B6BFF
color panel = #171421
word selectedName = ""
word currentColor = "#8B5CF6"
word backgroundColor = "#FFFFFF"
word currentShape = "square"
word renameInput = ""
word addLabel = ""
word widthChange = ""
word heightChange = ""
word moveX = ""
word moveY = ""
word animDx = ""
word animDy = ""
word animTime = ""
list objects
num objectCount = 0
num selectedIndex = 0

// Main workspace
block z 0 x 0 y 0 w 100 h 100 color #111019 name EDITOR_BACKGROUND
block z 1 x 0 y 0 w 100 h 1 color accent name TOP_ACCENT
block z 1 x 0 y 2 w 100 h 9 color #191622 name TOPBAR
text "TRG" x 3 y 5 size 3.4 color #FFFFFF bold true
text "2D EDITOR" x 12 y 5.5 size 2.1 color #BDA7F5 bold true
text "SCENE" x 97 y 5.8 size 1.5 color #A99DBB align right

// Canvas frame and grid
block z 1 x 20 y 13 w 78 h 57 color #282238 edges round name CANVAS_FRAME
block z 2 x 21 y 14 w 76 h 55 color #FFFFFF edges round name CANVAS
block z 3 x 21 y 14 w 76 h 0.2 color #D9D4E2 name GRID_TOP
block z 3 x 21 y 68.8 w 76 h 0.2 color #D9D4E2 name GRID_BOTTOM
block z 3 x 21 y 14 w 0.2 h 55 color #D9D4E2 name GRID_LEFT
block z 3 x 96.8 y 14 w 0.2 h 55 color #D9D4E2 name GRID_RIGHT
block z 3 x 36 y 14 w 0.15 h 55 color #EEEAF3 name GRID_V1
block z 3 x 51 y 14 w 0.15 h 55 color #EEEAF3 name GRID_V2
block z 3 x 66 y 14 w 0.15 h 55 color #EEEAF3 name GRID_V3
block z 3 x 81 y 14 w 0.15 h 55 color #EEEAF3 name GRID_V4
block z 3 x 21 y 25 w 76 h 0.15 color #EEEAF3 name GRID_H1
block z 3 x 21 y 36 w 76 h 0.15 color #EEEAF3 name GRID_H2
block z 3 x 21 y 47 w 76 h 0.15 color #EEEAF3 name GRID_H3
block z 3 x 21 y 58 w 76 h 0.15 color #EEEAF3 name GRID_H4

// Left tool rail
block z 1 x 2 y 13 w 16 h 57 color #191622 edges round name TOOL_RAIL
text "TOOLS" x 4 y 16 size 1.4 color #A99DBB bold true
button tool_select "SELECT" x 3 y 19 w 14 h 6 color #56368F (
    say "Select an object on the canvas, then drag it to move."
)
button tool_block "+ BLOCK" x 3 y 26 w 14 h 6 color #30243F (
    ui create block auto x 45 y 35 w 14 h 12 color currentColor shape square
    push objects last_ui_object
    length objects objectCount
    selectedIndex = objectCount - 1
    get objects selectedIndex selectedName
    set_text object_label selectedName
    set_text object_count objectCount
)
button tool_circle "CIRCLE" x 3 y 33 w 14 h 6 color #30243F (
    ui create block auto x 45 y 35 w 12 h 12 color currentColor shape circle
    push objects last_ui_object
    length objects objectCount
    selectedIndex = objectCount - 1
    get objects selectedIndex selectedName
    set_text object_label selectedName
    set_text object_count objectCount
)
button tool_button "BUTTON" x 3 y 40 w 14 h 6 color #30243F (
    ask "Button object name:" renameInput
    ask "Button label:" addLabel
    button renameInput addLabel x 45 y 35 w 24 h 9 color currentColor ( say "Button clicked" )
    push objects renameInput
    length objects objectCount
    selectedIndex = objectCount - 1
    selectedName = renameInput
    set_text object_label selectedName
    set_text object_count objectCount
)
button tool_text "TEXT" x 3 y 47 w 14 h 6 color #30243F (
    ask "Text object name:" renameInput
    ask "Text to display:" addLabel
    text renameInput addLabel x 45 y 35 size 3 color #211A30
    push objects renameInput
    length objects objectCount
    selectedIndex = objectCount - 1
    selectedName = renameInput
    set_text object_label selectedName
    set_text object_count objectCount
)
button tool_bg "BG" x 3 y 54 w 14 h 6 color #30243F (
    ask "Canvas background colour (#RRGGBB):" backgroundColor
    ui color CANVAS backgroundColor
)

// Canvas status strip
text object_label "No object selected" x 21 y 72 size 2.1 color #FFFFFF bold true
text object_count "0 OBJECTS" x 97 y 72 size 1.4 color #B8ADC9 align right
text "TAP TO SELECT  •  DRAG TO MOVE" x 21 y 75 size 1.2 color #A99DBB

// Inspector panel
block z 1 x 2 y 78 w 96 h 19 color #191622 edges round name INSPECTOR
text "OBJECT INSPECTOR" x 4 y 80 size 1.5 color #BDA7F5 bold true
button previous "‹" x 4 y 83 w 8 h 5 color #30243F (
    if objectCount > 0 (
        selectedIndex = selectedIndex - 1
            if selectedIndex < 0 (
            selectedIndex = objectCount - 1
        )
        get objects selectedIndex selectedName
        set_text object_label selectedName
    )
)
button next "›" x 13 y 83 w 8 h 5 color #30243F (
    if objectCount > 0 (
        selectedIndex = selectedIndex + 1
        if selectedIndex >= objectCount (
            selectedIndex = 0
        )
        get objects selectedIndex selectedName
        set_text object_label selectedName
    )
)
button rename "NAME" x 22 y 83 w 13 h 5 color #30243F (
    if objectCount > 0 (
        ask "New object name:" renameInput
        ui rename selectedName renameInput
        remove objects selectedIndex
        insert objects selectedIndex renameInput
        selectedName = renameInput
        set_text object_label selectedName
    )
)
button resize "SIZE" x 36 y 83 w 12 h 5 color #56368F (
    if objectCount > 0 (
        ask "Width change (+/-):" widthChange
        ask "Height change (+/-):" heightChange
        ui resize selectedName widthChange heightChange
    )
)
button move "MOVE" x 49 y 83 w 12 h 5 color #30243F (
    if objectCount > 0 (
        ask "Horizontal movement:" moveX
        ask "Vertical movement:" moveY
        ui move selectedName moveX moveY
    )
)
button color "COLOR" x 62 y 83 w 12 h 5 color #30243F (
    if objectCount > 0 (
        if currentColor = "#8B5CF6" ( currentColor = "#3B82F6" )
        else ( if currentColor = "#3B82F6" ( currentColor = "#22C55E" )
        else ( if currentColor = "#22C55E" ( currentColor = "#F59E0B" )
        else ( currentColor = "#8B5CF6" ) ) )
        ui color selectedName currentColor
    )
)
button shape "SHAPE" x 75 y 83 w 20 h 5 color #30243F (
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

button duplicate "DUPLICATE" x 4 y 90 w 18 h 5 color #30243F (
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
button delete "DELETE" x 23 y 90 w 15 h 5 color #652A45 (
    if objectCount > 0 (
        ui delete selectedName
        remove objects selectedIndex
        length objects objectCount
        if objectCount > 0 (
            if selectedIndex >= objectCount ( selectedIndex = objectCount - 1 )
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
button animate "ANIMATE" x 39 y 90 w 16 h 5 color #30243F (
    if objectCount > 0 (
        ask "Animation X movement:" animDx
        ask "Animation Y movement:" animDy
        ask "Animation duration (seconds):" animTime
        animate selectedName x animDx y animDy fps 60 time animTime
    )
)
button copy_code "COPY CODE" x 56 y 90 w 17 h 5 color #56368F (
    say "Select an object and use its creation command in your TRG project."
)
button exit "CODE" x 74 y 90 w 21 h 5 color #652A45 (
    studio code
)

// Starter object
ui create block auto x 45 y 35 w 14 h 12 color accent shape square
push objects last_ui_object
length objects objectCount
selectedIndex = 0
get objects selectedIndex selectedName
set_text object_label selectedName
set_text object_count objectCount
)TRGEDITOR2D";
inline constexpr const char* editor3D = R"TRGEDITOR3D(// TRG Studio — desktop/mobile 3D engine workspace
// Scene hierarchy, central 3D viewport, inspector and asset shelf.
add (ui)
add (3d)

color accent = #9B6BFF
color panel = #171222
color panelRaised = #211832
color muted = #A99ABD
word selectedName = ""
word currentColor = "#8B5CF6"
word renameInput = ""
word scaleFactor = ""
word rotateAmount = ""
list objects
num objectCount = 0
num selectedIndex = 0

// Engine shell and panel layout
block z 0 x 0 y 0 w 100 h 100 color #100C19 name ENGINE_BACKGROUND
block z 1 x 0 y 0 w 100 h 7 color #1B1429 name ENGINE_HEADER
block z 2 x 0 y 7 w 100 h 7 color #241A35 name ENGINE_TOOLBAR
block z 1 x 0 y 14 w 19 h 66 color #1B1429 name HIERARCHY_PANEL
block z 1 x 19 y 14 w 61 h 66 color #0D0A15 name VIEWPORT_PANEL
block z 1 x 80 y 14 w 20 h 66 color #1B1429 name INSPECTOR_PANEL
block z 2 x 0 y 14 w 19 h 5 color #302044 name HIERARCHY_HEADER
block z 2 x 80 y 14 w 20 h 5 color #302044 name INSPECTOR_HEADER
block z 2 x 0 y 80 w 100 h 20 color #1B1429 name ASSET_PANEL
block z 3 x 0 y 80 w 100 h 1 color #6B4A9E name ASSET_DIVIDER

// Viewport grid and axes
block z 3 x 22 y 70 w 55 h 0.25 color #40334F name GRID_LINE_1
block z 3 x 26 y 65 w 47 h 0.2 color #352943 name GRID_LINE_2
block z 3 x 31 y 60 w 37 h 0.2 color #352943 name GRID_LINE_3
block z 3 x 36 y 55 w 27 h 0.2 color #352943 name GRID_LINE_4
block z 3 x 49.8 y 21 w 0.25 h 49 color #49345F name AXIS_Y
block z 3 x 22 y 69.8 w 55 h 0.25 color #49345F name AXIS_X

text "TRG ENGINE" x 2 y 3.5 size 2.7 color #F4ECFF bold true
text "SCENE EDITOR" x 14 y 3.5 size 1.3 color #BBA6D8
text "FILE   EDIT   OBJECT   VIEW   BUILD" x 98 y 3.5 size 1.25 color #D8C9EC align right
text "SELECT" x 2 y 10.5 size 1.5 color #EDE4FA bold true
text "MOVE" x 13 y 10.5 size 1.5 color #BDA6E8
text "ROTATE" x 22 y 10.5 size 1.5 color #BDA6E8
text "SCALE" x 33 y 10.5 size 1.5 color #BDA6E8
text "PLAY" x 98 y 10.5 size 1.5 color #D8C9EC align right

text "HIERARCHY" x 2 y 17 size 1.6 color #EDE4FA bold true
text "Scene" x 2 y 22 size 1.45 color #BDA6E8
text "  Main Camera" x 2 y 27 size 1.25 color #B8A9C9
text "  Directional Light" x 2 y 31 size 1.25 color #B8A9C9
text hierarchy_selected "No object selected" x 2 y 36 size 1.25 color #D8C2FF bold true
text "OBJECTS" x 2 y 43 size 1.2 color #8D7AA8
text object_count "0 OBJECTS" x 2 y 47 size 1.3 color #CBB6EA
text "Select with PREV/NEXT" x 2 y 55 size 1.05 color #8D7AA8

text "SCENE VIEW" x 21 y 16 size 1.4 color #D7C5F4 bold true
text "PERSPECTIVE   |   GRID ON" x 78 y 16 size 1.1 color #9E8AB8 align right
text viewport_status "No object selected" x 50 y 74 size 1.5 color #BDA6E8 align center

text "INSPECTOR" x 82 y 17 size 1.6 color #EDE4FA bold true
text "TRANSFORM" x 82 y 23 size 1.2 color #BDA6E8 bold true
text "Selected:" x 82 y 28 size 1.1 color #8D7AA8
text model_label "No selection" x 82 y 32 size 1.2 color #F0E8FF
text "Scale / Rotate" x 82 y 39 size 1.1 color #8D7AA8
text "MATERIAL" x 82 y 49 size 1.2 color #BDA6E8 bold true
text current_color_label "Material: purple" x 82 y 54 size 1.1 color #D8C9EC
text "Use controls below" x 82 y 61 size 1.05 color #8D7AA8

text "CONTENT BROWSER" x 2 y 83 size 1.5 color #EDE4FA bold true
text "SCENE ASSETS" x 2 y 87 size 1.1 color #8D7AA8
text "Primitives   Materials   Textures" x 2 y 92 size 1.2 color #BDA6E8
text "TRG 3D WORKSPACE" x 98 y 96 size 1.1 color #8D7AA8 align right

// Start with a model in the viewport.
3d box auto x 0 y 0 z 0 w 3 h 3 d 3 color accent
push objects last_3d_object
length objects objectCount
selectedIndex = 0
get objects selectedIndex selectedName
set_text model_label selectedName
        set_text hierarchy_selected selectedName
        set_text viewport_status selectedName
set_text object_count objectCount
set_text current_color_label currentColor

// Hierarchy and object creation controls.
button add_box "＋ BOX" x 21 y 19 w 10 h 4 color #56368F (
    3d box auto x -2 y 0 z 0 w 2 h 2 d 2 color currentColor
    push objects last_3d_object
    length objects objectCount
    selectedIndex = objectCount - 1
    get objects selectedIndex selectedName
    set_text model_label selectedName
        set_text hierarchy_selected selectedName
        set_text viewport_status selectedName
    set_text object_count objectCount
)
button add_sphere "＋ SPHERE" x 32 y 19 w 13 h 4 color #30243F (
    3d sphere auto x 2 y 0 z 0 size 2 color currentColor
    push objects last_3d_object
    length objects objectCount
    selectedIndex = objectCount - 1
    get objects selectedIndex selectedName
    set_text model_label selectedName
        set_text hierarchy_selected selectedName
        set_text viewport_status selectedName
    set_text object_count objectCount
)
button duplicate "DUPLICATE" x 46 y 19 w 12 h 4 color #30243F (
    if objectCount > 0 (
        3d duplicate selectedName auto
        push objects last_3d_object
        length objects objectCount
        selectedIndex = objectCount - 1
        get objects selectedIndex selectedName
        set_text model_label selectedName
        set_text hierarchy_selected selectedName
        set_text viewport_status selectedName
        set_text object_count objectCount
    )
)
button delete "DELETE" x 59 y 19 w 10 h 4 color #652A45 (
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
            set_text hierarchy_selected selectedName
            set_text viewport_status selectedName
        )
        else (
            selectedName = ""
            set_text model_label "No object selected"
            set_text hierarchy_selected "No object selected"
            set_text viewport_status "No object selected"
        )
        set_text object_count objectCount
    )
)

// Object selection and inspector actions.
button previous "PREV" x 2 y 60 w 7 h 4 color #30243F (
    if objectCount > 0 (
        selectedIndex = selectedIndex - 1
        if selectedIndex < 0 (
            selectedIndex = objectCount - 1
        )
        get objects selectedIndex selectedName
        set_text model_label selectedName
        set_text hierarchy_selected selectedName
        set_text viewport_status selectedName
    )
)
button next "NEXT" x 10 y 60 w 7 h 4 color #30243F (
    if objectCount > 0 (
        selectedIndex = selectedIndex + 1
        if selectedIndex >= objectCount (
            selectedIndex = 0
        )
        get objects selectedIndex selectedName
        set_text model_label selectedName
        set_text hierarchy_selected selectedName
        set_text viewport_status selectedName
    )
)
button rename "RENAME" x 82 y 35 w 16 h 4 color #30243F (
    if objectCount > 0 (
        ask "New object name:" renameInput
        3d rename selectedName renameInput
        remove objects selectedIndex
        insert objects selectedIndex renameInput
        selectedName = renameInput
        set_text model_label selectedName
        set_text hierarchy_selected selectedName
        set_text viewport_status selectedName
    )
)
button scale "SCALE" x 82 y 42 w 16 h 4 color #56368F (
    if objectCount > 0 (
        ask "Scale factor (e.g. 1.2 or 0.8):" scaleFactor
        3d scale selectedName scaleFactor
    )
)
button rotate "ROTATE" x 82 y 45 w 16 h 4 color #30243F (
    if objectCount > 0 (
        ask "Rotation around Y in degrees:" rotateAmount
        3d rotate selectedName 0 rotateAmount 0
    )
)
button material "CHANGE MATERIAL" x 82 y 57 w 16 h 4 color #56368F (
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
        set_text model_label selectedName
        set_text hierarchy_selected selectedName
        set_text viewport_status selectedName
        set_text current_color_label currentColor
    )
)

// Viewport and project actions.
button reset_view "RESET VIEW" x 70 y 19 w 9 h 4 color #30243F (
    say "Camera view reset."
)
button load_scene "LOAD SCENE" x 70 y 24 w 9 h 4 color #30243F (
    3d load "scene.trgscene"
)
button save_scene "SAVE SCENE" x 70 y 29 w 9 h 4 color #56368F (
    3d save "scene.trgscene"
)
button controls "CONTROLS" x 70 y 34 w 9 h 4 color #30243F (
    say "Drag a model to move it. Drag empty space to orbit. Pinch to zoom on touch screens."
)
)TRGEDITOR3D"
} // namespace trg_builtin
