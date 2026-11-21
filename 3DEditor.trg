// TRG Desktop 3D Scene Editor
// Workspace layout: hierarchy, viewport, inspector, and asset shelf.
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

// Workspace surfaces
block z 0 x 0 y 0 w 100 h 100 color #11141C name APP_BACKGROUND
block z 1 x 0 y 0 w 100 h 8 color #191E29 name TOP_BAR
block z 1 x 0 y 8 w 18 h 74 color #191E29 name HIERARCHY_PANEL
block z 1 x 80 y 8 w 20 h 74 color #191E29 name INSPECTOR_PANEL
block z 1 x 18 y 8 w 62 h 74 color #202633 name VIEWPORT_SURFACE
block z 1 x 0 y 82 w 100 h 18 color #191E29 name ASSET_SHELF
block z 2 x 18 y 8 w 0.25 h 74 color #343B4A name LEFT_DIVIDER
block z 2 x 79.75 y 8 w 0.25 h 74 color #343B4A name RIGHT_DIVIDER
block z 2 x 0 y 81.75 w 100 h 0.25 color #343B4A name SHELF_DIVIDER
block z 2 x 0 y 7.75 w 100 h 0.25 color #343B4A name TOP_DIVIDER

// Subtle viewport grid and world axes
block z 1 x 22 y 22 w 54 h 0.12 color #2B3443 name GRID_H1
block z 1 x 22 y 32 w 54 h 0.12 color #2B3443 name GRID_H2
block z 1 x 22 y 42 w 54 h 0.12 color #2B3443 name GRID_H3
block z 1 x 22 y 52 w 54 h 0.12 color #2B3443 name GRID_H4
block z 1 x 22 y 62 w 54 h 0.12 color #2B3443 name GRID_H5
block z 1 x 22 y 72 w 54 h 0.12 color #2B3443 name GRID_H6
block z 1 x 28 y 18 w 0.12 h 58 color #2B3443 name GRID_V1
block z 1 x 38 y 18 w 0.12 h 58 color #2B3443 name GRID_V2
block z 1 x 48 y 18 w 0.12 h 58 color #2B3443 name GRID_V3
block z 1 x 58 y 18 w 0.12 h 58 color #2B3443 name GRID_V4
block z 1 x 68 y 18 w 0.12 h 58 color #2B3443 name GRID_V5
block z 2 x 22 y 52 w 54 h 0.2 color #465B79 name WORLD_X_AXIS
block z 2 x 48 y 18 w 0.2 h 58 color #6B4D73 name WORLD_Y_AXIS

// Top toolbar
text "TRG" x 2 y 5 size 2.8 color #FFFFFF
text "SCENE EDITOR" x 6 y 4.7 size 1.55 color #AAB3C5
button save_scene "SAVE" x 67 y 1.5 w 9 h 5 color #343B50 (
    3d save "scenes/level.trgscene"
)
button new_scene "NEW" x 87 y 1.5 w 10 h 5 color #5743A0 (
    3d clear
    loop objectCount > 0 (
        remove objects 0
        objectCount = objectCount - 1
    )
    selectedIndex = 0
    selectedName = ""
    set_text selected_name "Nothing selected"
    set_text object_count "0 objects"
)

// Hierarchy
text "HIERARCHY" x 2 y 12 size 1.6 color #E6EAF2
text "SCENE" x 2 y 16 size 1.15 color #8994A8
text object_count "2 objects" x 14 y 16 size 1.1 color #8994A8 align right
button select_cube "▣  Cube" x 1 y 19 w 16 h 5 color #34314B (
    if objectCount > 0 (
        selectedIndex = 0
        get objects selectedIndex selectedName
        set_text selected_name selectedName
    )
)
button select_sphere "◉  Sphere" x 1 y 25 w 16 h 5 color #292F3D (
    if objectCount > 1 (
        selectedIndex = 1
        get objects selectedIndex selectedName
        set_text selected_name selectedName
    )
)
text "Drag objects in the viewport" x 2 y 34 size 1.0 color #7F8A9E
text "to reposition them." x 2 y 37 size 1.0 color #7F8A9E
text "SCENE FILE" x 2 y 72 size 1.05 color #8994A8
text "scenes/level.trgscene" x 2 y 76 size 0.95 color #B7C0D0

// Viewport overlay
text "PERSPECTIVE" x 20 y 12 size 1.1 color #B6C0D2
text "XYZ" x 76 y 12 size 1.1 color #B6C0D2 align right
text "3D VIEWPORT" x 49 y 77 size 1.1 color #8994A8 align center

// Inspector
text "INSPECTOR" x 82 y 12 size 1.6 color #E6EAF2
text selected_name "Nothing selected" x 82 y 17 size 1.35 color #BCA8FF
text "TRANSFORM" x 82 y 24 size 1.15 color #8994A8
text "Position" x 82 y 29 size 1.15 color #D8DEEA
button move_object "EDIT POSITION" x 82 y 31 w 16 h 5 color #343B50 (
    if objectCount > 0 (
        ask "Move X by:" moveXAmount
        ask "Move Y by:" moveYAmount
        ask "Move Z by:" moveZAmount
        3d move selectedName moveXAmount moveYAmount moveZAmount
    )
)
text "Rotation" x 82 y 39 size 1.15 color #D8DEEA
button rotate_object "EDIT ROTATION" x 82 y 41 w 16 h 5 color #343B50 (
    if objectCount > 0 (
        ask "Rotate X by degrees:" rotateXAmount
        ask "Rotate Y by degrees:" rotateYAmount
        ask "Rotate Z by degrees:" rotateZAmount
        3d rotate selectedName rotateXAmount rotateYAmount rotateZAmount
    )
)
text "Scale" x 82 y 49 size 1.15 color #D8DEEA
button scale_object "UNIFORM SCALE" x 82 y 51 w 16 h 5 color #343B50 (
    if objectCount > 0 (
        ask "Scale factor (e.g. 1.2):" scaleFactor
        3d scale selectedName scaleFactor
    )
)
text "MATERIAL" x 82 y 60 size 1.15 color #8994A8
button change_color "CHANGE COLOR" x 82 y 62 w 16 h 5 color #5743A0 (
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
    )
)
button rename_object "RENAME" x 82 y 70 w 7.5 h 5 color #343B50 (
    if objectCount > 0 (
        ask "New object name:" renameInput
        3d rename selectedName renameInput
        remove objects selectedIndex
        insert objects selectedIndex renameInput
        selectedName = renameInput
        set_text selected_name selectedName
    )
)
button delete_object "DELETE" x 90.5 y 70 w 7.5 h 5 color #693747 (
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
            set_text selected_name "Nothing selected"
        )
        set_text object_count objectCount
    )
)

// Asset shelf
text "ASSETS" x 2 y 86 size 1.5 color #E6EAF2
text "PRIMITIVES" x 2 y 91 size 1.0 color #8994A8
button add_box "＋  CUBE" x 18 y 85 w 12 h 6 color #5743A0 (
    3d box auto x -2 y 0 z 0 w 2 h 2 d 2 color accent
    push objects last_3d_object
    length objects objectCount
    selectedIndex = objectCount - 1
    get objects selectedIndex selectedName
    set_text selected_name selectedName
    set_text object_count objectCount
)
button add_sphere "＋  SPHERE" x 31 y 85 w 13 h 6 color #343B50 (
    3d sphere auto x 2 y 0 z 0 size 2 color #3B82F6
    push objects last_3d_object
    length objects objectCount
    selectedIndex = objectCount - 1
    get objects selectedIndex selectedName
    set_text selected_name selectedName
    set_text object_count objectCount
)
button duplicate_object "DUPLICATE" x 46 y 85 w 12 h 6 color #343B50 (
    if objectCount > 0 (
        3d duplicate selectedName auto
        push objects last_3d_object
        length objects objectCount
        selectedIndex = objectCount - 1
        get objects selectedIndex selectedName
        set_text selected_name selectedName
        set_text object_count objectCount
    )
)
button previous_object "PREVIOUS" x 61 y 85 w 9 h 6 color #343B50 (
    if objectCount > 0 (
        selectedIndex = selectedIndex - 1
        if selectedIndex < 0 (
            selectedIndex = objectCount - 1
        )
        get objects selectedIndex selectedName
        set_text selected_name selectedName
    )
)
button next_object "NEXT" x 72 y 85 w 7 h 6 color #343B50 (
    if objectCount > 0 (
        selectedIndex = selectedIndex + 1
        if selectedIndex >= objectCount (
            selectedIndex = 0
        )
        get objects selectedIndex selectedName
        set_text selected_name selectedName
    )
)
text "TRG 3D SCENE" x 83 y 89 size 1.0 color #8994A8

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
