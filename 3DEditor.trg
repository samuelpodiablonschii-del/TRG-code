// TRG Desktop 3D Workspace
// A focused scene editor: hierarchy, live 3D viewport, inspector and compact toolbar.
add (ui)
add (3d)

color accent = #8B72F6
word selectedName = ""
word renameInput = ""
word scaleFactor = ""
word rotateXAmount = ""
word rotateYAmount = ""
word rotateZAmount = ""
word moveXAmount = ""
word moveYAmount = ""
word moveZAmount = ""
word currentColor = "#8B72F6"
list objects
num objectCount = 0
num selectedIndex = 0

// Main editor frame
block z 0 x 0 y 0 w 100 h 100 color #10131A name APP_BACKGROUND
block z 1 x 0 y 0 w 100 h 8 color #191E29 name HEADER
block z 1 x 0 y 8 w 19 h 79 color #191E29 name HIERARCHY
block z 1 x 19 y 8 w 58 h 79 color #202633 name VIEWPORT
block z 1 x 77 y 8 w 23 h 79 color #191E29 name INSPECTOR
block z 1 x 0 y 87 w 100 h 13 color #191E29 name STATUS_BAR
block z 2 x 18.85 y 8 w 0.15 h 79 color #343B4A name LEFT_EDGE
block z 2 x 76.85 y 8 w 0.15 h 79 color #343B4A name RIGHT_EDGE
block z 2 x 0 y 86.85 w 100 h 0.15 color #343B4A name BOTTOM_EDGE
block z 2 x 0 y 7.85 w 100 h 0.15 color #343B4A name HEADER_EDGE

// Viewport grid
block z 1 x 22 y 22 w 52 h 0.12 color #2B3443 name GRID_H1
block z 1 x 22 y 32 w 52 h 0.12 color #2B3443 name GRID_H2
block z 1 x 22 y 42 w 52 h 0.12 color #2B3443 name GRID_H3
block z 1 x 22 y 52 w 52 h 0.12 color #2B3443 name GRID_H4
block z 1 x 22 y 62 w 52 h 0.12 color #2B3443 name GRID_H5
block z 1 x 22 y 72 w 52 h 0.12 color #2B3443 name GRID_H6
block z 1 x 27 y 18 w 0.12 h 64 color #2B3443 name GRID_V1
block z 1 x 37 y 18 w 0.12 h 64 color #2B3443 name GRID_V2
block z 1 x 47 y 18 w 0.12 h 64 color #2B3443 name GRID_V3
block z 1 x 57 y 18 w 0.12 h 64 color #2B3443 name GRID_V4
block z 1 x 67 y 18 w 0.12 h 64 color #2B3443 name GRID_V5
block z 2 x 22 y 52 w 52 h 0.2 color #465B79 name WORLD_X_AXIS
block z 2 x 48 y 18 w 0.2 h 64 color #6B4D73 name WORLD_Y_AXIS

// Header and compact scene toolbar
text "TRG" x 2 y 5 size 2.7 color #FFFFFF
text "3D WORKSPACE" x 6.5 y 4.8 size 1.35 color #AAB3C5
text "Scene" x 17 y 4.8 size 1.15 color #8994A8
button new_scene "NEW" x 66 y 1.5 w 7 h 5 color #343B50 (
    3d clear
    loop objectCount > 0 (
        remove objects 0
        objectCount = objectCount - 1
    )
    selectedIndex = 0
    selectedName = ""
    set_text selected_name "No object selected"
    set_text object_count "0"
    set_text status_message "New empty scene"
)
button save_scene "SAVE" x 74 y 1.5 w 7 h 5 color #5743A0 (
    3d save "scenes/level.trgscene"
    set_text status_message "Scene saved"
)
button add_box "＋ CUBE" x 82 y 1.5 w 8 h 5 color #343B50 (
    3d box auto x -2 y 0 z 0 w 2 h 2 d 2 color accent
    push objects last_3d_object
    length objects objectCount
    selectedIndex = objectCount - 1
    get objects selectedIndex selectedName
    set_text selected_name selectedName
    set_text object_count objectCount
    set_text status_message "Cube added to scene"
)
button add_sphere "＋ SPHERE" x 91 y 1.5 w 8 h 5 color #343B50 (
    3d sphere auto x 2 y 0 z 0 size 2 color #3B82F6
    push objects last_3d_object
    length objects objectCount
    selectedIndex = objectCount - 1
    get objects selectedIndex selectedName
    set_text selected_name selectedName
    set_text object_count objectCount
    set_text status_message "Sphere added to scene"
)

// Scene hierarchy
text "SCENE HIERARCHY" x 2 y 12 size 1.35 color #E6EAF2
text "OBJECTS" x 2 y 17 size 1.0 color #8994A8
text object_count "2" x 16 y 17 size 1.0 color #8994A8 align right
button select_cube "▣  Cube" x 1 y 20 w 17 h 5 color #34314B (
    if objectCount > 0 (
        selectedIndex = 0
        get objects selectedIndex selectedName
        set_text selected_name selectedName
        set_text status_message "Selected first scene object"
    )
)
button select_sphere "◉  Sphere" x 1 y 26 w 17 h 5 color #292F3D (
    if objectCount > 1 (
        selectedIndex = 1
        get objects selectedIndex selectedName
        set_text selected_name selectedName
        set_text status_message "Selected second scene object"
    )
)
text "Select an object to" x 2 y 36 size 1.0 color #7F8A9E
text "edit its properties." x 2 y 39 size 1.0 color #7F8A9E
text "SCENE FILE" x 2 y 75 size 1.0 color #8994A8
text "level.trgscene" x 2 y 79 size 1.0 color #B7C0D0

// Viewport labels
text "PERSPECTIVE VIEW" x 21 y 12 size 1.0 color #B6C0D2
text "X  Y  Z" x 73 y 12 size 1.0 color #B6C0D2 align right
text "Drag-free preview • TRG 3D scene" x 48 y 83 size 0.95 color #8994A8 align center

// Inspector panel
text "INSPECTOR" x 79 y 12 size 1.4 color #E6EAF2
text selected_name "Cube" x 79 y 17 size 1.2 color #BCA8FF
text "TRANSFORM" x 79 y 24 size 1.0 color #8994A8
text "Position" x 79 y 29 size 1.0 color #D8DEEA
button move_object "EDIT POSITION" x 79 y 31 w 19 h 4.5 color #343B50 (
    if objectCount > 0 (
        ask "Move X by:" moveXAmount
        ask "Move Y by:" moveYAmount
        ask "Move Z by:" moveZAmount
        3d move selectedName moveXAmount moveYAmount moveZAmount
        set_text status_message "Position updated"
    )
)
text "Rotation" x 79 y 38 size 1.0 color #D8DEEA
button rotate_object "EDIT ROTATION" x 79 y 40 w 19 h 4.5 color #343B50 (
    if objectCount > 0 (
        ask "Rotate X by degrees:" rotateXAmount
        ask "Rotate Y by degrees:" rotateYAmount
        ask "Rotate Z by degrees:" rotateZAmount
        3d rotate selectedName rotateXAmount rotateYAmount rotateZAmount
        set_text status_message "Rotation updated"
    )
)
text "Scale" x 79 y 47 size 1.0 color #D8DEEA
button scale_object "UNIFORM SCALE" x 79 y 49 w 19 h 4.5 color #343B50 (
    if objectCount > 0 (
        ask "Scale factor (e.g. 1.2):" scaleFactor
        3d scale selectedName scaleFactor
        set_text status_message "Scale updated"
    )
)
text "MATERIAL" x 79 y 57 size 1.0 color #8994A8
button change_color "CYCLE COLOR" x 79 y 59 w 19 h 4.5 color #5743A0 (
    if objectCount > 0 (
        if currentColor = "#8B72F6" (
            currentColor = "#3B82F6"
        )
        else (
            if currentColor = "#3B82F6" (
                currentColor = "#22C55E"
            )
            else (
                if currentColor = "#22C55E" (
                    currentColor = "#F59E0B"
                )
                else (
                    currentColor = "#8B72F6"
                )
            )
        )
        3d color selectedName currentColor
        set_text status_message "Material colour updated"
    )
)
button rename_object "RENAME" x 79 y 68 w 9 h 4.5 color #343B50 (
    if objectCount > 0 (
        ask "New object name:" renameInput
        3d rename selectedName renameInput
        remove objects selectedIndex
        insert objects selectedIndex renameInput
        selectedName = renameInput
        set_text selected_name selectedName
        set_text status_message "Object renamed"
    )
)
button delete_object "DELETE" x 89 y 68 w 9 h 4.5 color #693747 (
    if objectCount > 0 (
        3d delete selectedName
        remove objects selectedIndex
        length objects objectCount
        if objectCount > 0 (
            if selectedIndex >= objectCount (
                selectedIndex = objectCount - 1
            )
            get objects selectedIndex selectedName
            set_text selected_name selectedName
        )
        else (
            selectedName = ""
            set_text selected_name "No object selected"
        )
        set_text object_count objectCount
        set_text status_message "Object deleted"
    )
)

// Bottom status strip
text "READY" x 2 y 92 size 1.0 color #8B72F6
text status_message "Scene loaded" x 9 y 92 size 1.0 color #B7C0D0
text "TRG SCENE" x 91 y 92 size 1.0 color #8994A8 align right

// Starter scene
3d box Cube x -2 y 0 z 0 w 2 h 2 d 2 color #8B72F6
push objects last_3d_object
3d sphere Sphere x 2 y 0 z 0 size 2 color #3B82F6
push objects last_3d_object
length objects objectCount
selectedIndex = 0
get objects selectedIndex selectedName
set_text selected_name selectedName
set_text object_count objectCount
