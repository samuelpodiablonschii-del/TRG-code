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
inline constexpr const char* editor3D = R"TRGEDITOR3D(// TRG Studio — professional desktop 3D scene workspace
// Empty scene, docked hierarchy/inspector, asset shelf and viewport tools.
add (ui)
add (3d)

color accent = #9B6BFF
color panel = #191421
color panelRaised = #241B32
word selectedName = ""
word currentColor = "#8B5CF6"
word currentTexture = "grid"
word renameInput = ""
word scaleFactor = ""
word rotateAmount = ""
list objects
num objectCount = 0
num selectedIndex = 0

// Docked editor layout.
block z 0 x 0 y 0 w 100 h 100 color #100D17 name ENGINE_BACKGROUND
block z 1 x 0 y 0 w 100 h 6 color #1B1525 name ENGINE_TITLEBAR
block z 2 x 0 y 6 w 100 h 7 color #241B32 name ENGINE_TOOLBAR
block z 1 x 0 y 13 w 19 h 65 color #1A1423 name HIERARCHY_PANEL
block z 1 x 19 y 13 w 61 h 65 color #0C0A12 name VIEWPORT_PANEL
block z 1 x 80 y 13 w 20 h 65 color #1A1423 name INSPECTOR_PANEL
block z 2 x 0 y 78 w 100 h 22 color #1A1423 name ASSET_BROWSER
block z 3 x 0 y 78 w 100 h 0.35 color #7351A8 name ASSET_DIVIDER
block z 2 x 0 y 13 w 19 h 5 color #302341 name HIERARCHY_HEADER
block z 2 x 80 y 13 w 20 h 5 color #302341 name INSPECTOR_HEADER

text "TRG ENGINE" x 2 y 3.6 size 2.5 color #F5EDFF bold true
text "SCENE EDITOR" x 15 y 3.7 size 1.2 color #BCA8DA
text "PROJECT   EDIT   WINDOW   HELP" x 98 y 3.7 size 1.15 color #CFC1E3 align right

// Compact, non-overlapping tool strip.
text "SELECT" x 2 y 10.5 size 1.25 color #F1E9FC bold true
button reset_view "RESET VIEW" x 20 y 7.7 w 10 h 4.2 color #483363 (
    say "Camera reset. Use W A S D to move, Q/E to move vertically, and right mouse drag to orbit."
)
button save_scene "SAVE" x 31 y 7.7 w 8 h 4.2 color #3B2A55 (
    3d save "scene.trgscene"
)
button load_scene "LOAD" x 40 y 7.7 w 8 h 4.2 color #30243F (
    3d load "scene.trgscene"
    length objects objectCount
    if objectCount > 0 (
        selectedIndex = 0
        get objects selectedIndex selectedName
        set_text model_label selectedName
        set_text hierarchy_selected selectedName
        set_text viewport_status selectedName
    )
    else (
        selectedName = ""
        set_text model_label "No selection"
        set_text hierarchy_selected "No objects in scene"
        set_text viewport_status "Empty scene — drag an asset into the viewport"
    )
    set_text object_count objectCount
)
button code "CODE" x 49 y 7.7 w 8 h 4.2 color #30243F (
    studio code
)

// Scene hierarchy.
text "HIERARCHY" x 1.5 y 16.5 size 1.35 color #EDE4FA bold true
text "▾ Scene" x 2 y 21.5 size 1.3 color #BDA6E8
text hierarchy_selected "No objects in scene" x 2 y 27 size 1.05 color #9F8DB8
text object_count "0 OBJECTS" x 2 y 74 size 1.05 color #9F8DB8

// Viewport overlays; 3D models are clipped to this central panel.
text "PERSPECTIVE" x 21 y 16 size 1.1 color #BDA6E8 bold true
text "GRID  |  WORLD AXES" x 78 y 16 size 1.0 color #8F7EA5 align right
text "WASD MOVE  •  RIGHT-DRAG ORBIT  •  WHEEL ZOOM" x 21 y 20.5 size 0.9 color #7E6B96
text viewport_status "Empty scene — drag an asset into the viewport" x 50 y 73 size 1.2 color #BCA8DA align center

// Inspector.
text "INSPECTOR" x 82 y 16.5 size 1.35 color #EDE4FA bold true
text "OBJECT" x 82 y 22 size 1.05 color #9F8DB8 bold true
text model_label "Nothing selected" x 82 y 26 size 1.1 color #F3EAFF
text "TRANSFORM" x 82 y 33 size 1.05 color #BDA6E8 bold true
text "Right-click for full properties" x 82 y 37 size 0.92 color #8F7EA5
button rename "RENAME" x 82 y 40 w 16 h 4 color #30243F (
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
button duplicate "DUPLICATE" x 82 y 45 w 16 h 4 color #30243F (
    if objectCount > 0 (
        3d duplicate selectedName auto
        push objects last_3d_object
        length objects objectCount
        selectedIndex = objectCount - 1
        get objects selectedIndex selectedName
        set_text model_label selectedName
        set_text hierarchy_selected selectedName
        set_text object_count objectCount
    )
)
button delete "DELETE" x 82 y 50 w 16 h 4 color #603044 (
    if objectCount > 0 (
        3d delete selectedName
        remove objects selectedIndex
        length objects objectCount
        if objectCount > 0 (
            if selectedIndex >= objectCount ( selectedIndex = objectCount - 1 )
            get objects selectedIndex selectedName
            set_text model_label selectedName
            set_text hierarchy_selected selectedName
        )
        else (
            selectedName = ""
            set_text model_label "Nothing selected"
            set_text hierarchy_selected "No objects in scene"
        )
        set_text object_count objectCount
    )
)
button material "MATERIAL" x 82 y 55 w 16 h 4 color #483363 (
    if objectCount > 0 (
        if currentColor = "#8B5CF6" ( currentColor = "#4F9CFB" )
        else ( if currentColor = "#4F9CFB" ( currentColor = "#D99A56" )
        else ( if currentColor = "#D99A56" ( currentColor = "#70B68A" )
        else ( currentColor = "#8B5CF6" ) ) )
        3d color selectedName currentColor
        set_text current_color_label currentColor
    )
)
button texture "TEXTURE" x 82 y 60 w 16 h 4 color #30243F (
    if objectCount > 0 (
        if currentTexture = "grid" ( currentTexture = "metal" )
        else ( if currentTexture = "metal" ( currentTexture = "wood" )
        else ( if currentTexture = "wood" ( currentTexture = "smooth" )
        else ( currentTexture = "grid" ) ) )
        3d texture selectedName currentTexture
        set_text texture_label currentTexture
    )
)
text current_color_label "Material: purple" x 82 y 67 size 0.95 color #BDA6E8
text texture_label "Texture: grid" x 82 y 71 size 0.95 color #BDA6E8

// Asset browser. Click an asset or drag it into the viewport.
text "ASSET BROWSER" x 2 y 81.5 size 1.3 color #EDE4FA bold true
text "DRAG INTO VIEWPORT  •  BOXES  •  SPHERES  •  CAMERA  •  SPAWN" x 98 y 81.5 size 0.95 color #8F7EA5 align right

button asset_box "＋  BOX" x 20 y 84 w 14 h 12 color #483363 (
    3d box auto x 0 y 0 z 0 w 2 h 2 d 2 color currentColor
    push objects last_3d_object
    length objects objectCount
    selectedIndex = objectCount - 1
    get objects selectedIndex selectedName
    set_text model_label selectedName
    set_text hierarchy_selected selectedName
    set_text object_count objectCount
    set_text viewport_status selectedName
)
button asset_sphere "◯  SPHERE" x 36 y 84 w 14 h 12 color #30435F (
    3d sphere auto x 0 y 0 z 0 size 1.7 color #4F9CFB
    push objects last_3d_object
    length objects objectCount
    selectedIndex = objectCount - 1
    get objects selectedIndex selectedName
    set_text model_label selectedName
    set_text hierarchy_selected selectedName
    set_text object_count objectCount
    set_text viewport_status selectedName
)
button asset_camera "▣  CAMERA" x 52 y 84 w 14 h 12 color #3B3659 (
    3d camera auto x 0 y 1 z 0 w 1.1 h 0.8 d 1.4 color #B79BEA
    push objects last_3d_object
    length objects objectCount
    selectedIndex = objectCount - 1
    get objects selectedIndex selectedName
    set_text model_label selectedName
    set_text hierarchy_selected selectedName
    set_text object_count objectCount
    set_text viewport_status selectedName
)
button asset_spawn "◆  SPAWN" x 68 y 84 w 14 h 12 color #60452E (
    3d spawn auto x 0 y 0 z 0 size 1 color #E7A85C
    push objects last_3d_object
    length objects objectCount
    selectedIndex = objectCount - 1
    get objects selectedIndex selectedName
    set_text model_label selectedName
    set_text hierarchy_selected selectedName
    set_text object_count objectCount
    set_text viewport_status selectedName
)
button asset_plane "▱  PLANE" x 84 y 84 w 14 h 12 color #315044 (
    3d plane auto x 0 y -1 z 0 w 6 h 0.15 d 6 color #70B68A
    push objects last_3d_object
    length objects objectCount
    selectedIndex = objectCount - 1
    get objects selectedIndex selectedName
    set_text model_label selectedName
    set_text hierarchy_selected selectedName
    set_text object_count objectCount
    set_text viewport_status selectedName
)
)TRGEDITOR3D";
} // namespace trg_builtin
