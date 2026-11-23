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
inline constexpr const char* editor3D = R"TRGEDITOR3D(// TRG Studio — mobile 3D editor, styled after the supplied reference
add (ui)
add (3d)

color accent = #9B55FF
color background = #100B1C
color panel = #1B142B
color panelLight = #2B1D40
word selectedName = "Cube"
word currentColor = "#8B42F5"
word currentTexture = "smooth"
word renameInput = ""
list objects
num objectCount = 1
num selectedIndex = 0

// Default scene: a selected cube, ready to edit.
3d box Cube x 0 y 0 z 0 w 2 h 2 d 2 color #8B42F5
push objects "Cube"

// App header.
block z 0 x 0 y 0 w 100 h 100 color #100B1C name ENGINE_BACKGROUND
block z 1 x 0 y 0 w 100 h 6.2 color #171027 name ENGINE_TITLEBAR
text "◆" x 4.5 y 4.35 size 3.0 color #A65BFF bold true
text "TRG ENGINE" x 15 y 4.15 size 2.55 color #F8F3FF bold true
text "☰" x 94 y 4.4 size 3.0 color #A65BFF align center

// Main action toolbar.
block z 1 x 0 y 6.2 w 100 h 6.3 color #130D20 name TOOLBAR_BACKGROUND
button select_tool "➤  Select" x 2 y 7.1 w 20 h 4.7 color #6630B5 (
    say "Select an object in the viewport. Drag it to move it."
)
button reset_view "⟳  Reset" x 23 y 7.1 w 19 h 4.7 color #302044 (
    say "Camera reset."
)
button save_scene "▣  Save" x 43 y 7.1 w 18 h 4.7 color #302044 (
    3d save "scene.trgscene"
    say "Scene saved."
)
button load_scene "▰  Load" x 62 y 7.1 w 18 h 4.7 color #302044 (
    3d load "scene.trgscene"
    say "Scene loaded."
)
button code "‹/›  Code" x 81 y 7.1 w 17 h 4.7 color #302044 (
    studio code
)

// Large, uncluttered perspective viewport.
block z 1 x 0.6 y 13.1 w 98.8 h 34.2 color #0C0915 edges round name VIEWPORT_PANEL
block z 2 x 1 y 29 w 98 h 0.12 color #28203A name GRID_H1
block z 2 x 1 y 34 w 98 h 0.12 color #28203A name GRID_H2
block z 2 x 1 y 39 w 98 h 0.12 color #28203A name GRID_H3
block z 2 x 1 y 44 w 98 h 0.12 color #28203A name GRID_H4
block z 2 x 10 y 28 w 40 h 0.12 color #28203A name GRID_D1
block z 2 x 50 y 28 w 40 h 0.12 color #28203A name GRID_D2
block z 2 x 1 y 46 w 98 h 0.12 color #28203A name GRID_HORIZON
text "PERSPECTIVE" x 3 y 15.2 size 0.95 color #A895C6 bold true
text "X  Y  Z" x 96 y 15.2 size 0.9 color #9C83C4 align right
text "DRAG TO MOVE  •  TWO FINGERS TO ORBIT + ZOOM" x 50 y 46.3 size 0.78 color #89799E align center

// Hierarchy / inspector navigation tabs.
block z 1 x 0 y 47.7 w 100 h 5.4 color #160F23 name EDITOR_TABS
button tab_hierarchy "▰  Hierarchy" x 1.5 y 48.2 w 48 h 4.7 color #572A9B (
    set_text panel_heading "HIERARCHY"
    set_text panel_hint "Cube is selected"
)
button tab_inspector "⚙  Inspector" x 50.5 y 48.2 w 48 h 4.7 color #281B3A (
    set_text panel_heading "INSPECTOR"
    set_text panel_hint "Edit the selected object below"
)

// Inspector card, matching the reference's compact stacked form.
block z 1 x 0.7 y 53.4 w 98.6 h 26.1 color #191226 edges round name INSPECTOR_CARD
text "◆" x 5 y 57.3 size 3.0 color #A65BFF
text model_label "Cube (Selected)" x 14 y 57.3 size 1.8 color #F5EEFF bold true
block z 2 x 4 y 59.1 w 92 h 0.12 color #342447 name INSPECTOR_DIVIDER

text "Name" x 4 y 62.3 size 1.35 color #D8CBEA
button rename "Cube" x 28 y 60.5 w 68 h 3.9 color #2B203D (
    if objectCount > 0 (
        ask "Object name:" renameInput
        3d rename selectedName renameInput
        selectedName = renameInput
        set_text model_label selectedName
        set_text hierarchy_selected selectedName
        set_text rename selectedName
    )
)

text "Position" x 4 y 67.2 size 1.3 color #D8CBEA
button pos_x "X  0.00" x 28 y 65.1 w 21 h 3.6 color #2B203D (
    say "Drag the selected object in the viewport to change its position."
)
button pos_y "Y  0.00" x 51 y 65.1 w 21 h 3.6 color #2B203D (
    say "Drag the selected object in the viewport to change its position."
)
button pos_z "Z  0.00" x 74 y 65.1 w 22 h 3.6 color #2B203D (
    say "Drag the selected object in the viewport to change its position."
)

text "Scale" x 4 y 71.8 size 1.3 color #D8CBEA
button scale_x "1.00" x 28 y 69.7 w 21 h 3.6 color #2B203D (
    say "Scale editing is available from the selected object's code."
)
button scale_y "1.00" x 51 y 69.7 w 21 h 3.6 color #2B203D (
    say "Scale editing is available from the selected object's code."
)
button scale_z "1.00" x 74 y 69.7 w 22 h 3.6 color #2B203D (
    say "Scale editing is available from the selected object's code."
)

text "Material" x 4 y 76.5 size 1.3 color #D8CBEA
button material "●  Purple                         ▾" x 28 y 74.3 w 68 h 3.8 color #2B203D (
    if objectCount > 0 (
        if currentColor = "#8B42F5" ( currentColor = "#438FF5" )
        else ( if currentColor = "#438FF5" ( currentColor = "#E2A34D" )
        else ( if currentColor = "#E2A34D" ( currentColor = "#58B985" )
        else ( currentColor = "#8B42F5" ) ) )
        3d color selectedName currentColor
        if currentColor = "#8B42F5" ( set_text material "●  Purple                         ▾" )
        else ( if currentColor = "#438FF5" ( set_text material "●  Blue                           ▾" )
        else ( if currentColor = "#E2A34D" ( set_text material "●  Gold                           ▾" )
        else ( set_text material "●  Green                          ▾" ) ) )
    )
)

// Asset browser dock.
block z 1 x 0.7 y 80.2 w 98.6 h 19.2 color #191226 edges round name ASSET_BROWSER
text "◆" x 4 y 83.2 size 2.0 color #A65BFF
text "Asset Browser" x 11 y 83.2 size 1.45 color #F4ECFF bold true
text "›" x 95 y 83.3 size 2.4 color #A65BFF align center

button asset_box "◆\nBox" x 3 y 85 w 17.5 h 12.2 color #302044 (
    3d box auto x 0 y 0 z 0 w 2 h 2 d 2 color currentColor
    push objects last_3d_object
    length objects objectCount
    selectedIndex = objectCount - 1
    get objects selectedIndex selectedName
    set_text model_label selectedName
    set_text hierarchy_selected selectedName
    set_text object_count objectCount
)
button asset_sphere "●\nSphere" x 22 y 85 w 17.5 h 12.2 color #302044 (
    3d sphere auto x 0 y 0 z 0 size 1.7 color #8D62F4
    push objects last_3d_object
    length objects objectCount
    selectedIndex = objectCount - 1
    get objects selectedIndex selectedName
    set_text model_label selectedName
    set_text hierarchy_selected selectedName
    set_text object_count objectCount
)
button asset_camera "◉\nCamera" x 41 y 85 w 17.5 h 12.2 color #302044 (
    3d camera auto x 0 y 1 z 0 w 1.1 h 0.8 d 1.4 color #B79BEA
    push objects last_3d_object
    length objects objectCount
    selectedIndex = objectCount - 1
    get objects selectedIndex selectedName
    set_text model_label selectedName
    set_text hierarchy_selected selectedName
    set_text object_count objectCount
)
button asset_spawn "▶\nSpawn" x 60 y 85 w 17.5 h 12.2 color #302044 (
    3d spawn auto x 0 y 0 z 0 size 1 color #58C78D
    push objects last_3d_object
    length objects objectCount
    selectedIndex = objectCount - 1
    get objects selectedIndex selectedName
    set_text model_label selectedName
    set_text hierarchy_selected selectedName
    set_text object_count objectCount
)
button asset_plane "◇\nPlane" x 79 y 85 w 18 h 12.2 color #302044 (
    3d plane auto x 0 y -1 z 0 w 6 h 0.15 d 6 color #70B68A
    push objects last_3d_object
    length objects objectCount
    selectedIndex = objectCount - 1
    get objects selectedIndex selectedName
    set_text model_label selectedName
    set_text hierarchy_selected selectedName
    set_text object_count objectCount
)
)TRGEDITOR3D";
inline constexpr const char* desktopEditor3D = R"TRGDESKTOP3D(// Native Windows editor owns the desktop 3D workspace and renders the scene UI.
// Start with an empty scene; use the Content Browser to add models, cameras and spawn points.
add (ui)
add (3d)
)TRGDESKTOP3D";

} // namespace trg_builtin
