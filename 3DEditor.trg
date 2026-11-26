// TRG ENGINE — Desktop workspace
// Roblox Studio-inspired docked layout for the native Windows 3D editor.
add (ui)
add (3d)

color accent = #9855FF
color panel = #191226
color panelLight = #251A37
color field = #2B203D
word selectedName = "Cube"
word renameInput = ""
word editValue = ""
word editX = "0"
word editY = "0"
word editZ = "0"
word currentColor = "#8B42F5"
word currentTexture = "smooth"
list objects
num objectCount = 1
num selectedIndex = 0

// Scene
3d box Cube x 0 y 0 z 0 w 2 h 2 d 2 color #8B42F5
push objects "Cube"
3d plane Baseplate x 0 y -1.15 z 0 w 14 h 0.15 d 14 color #343044
push objects "Baseplate"
3d camera Camera x 0 y 3 z 8 w 1 h 1 d 1 color #B79BEA
push objects "Camera"
length objects objectCount

// Full-window base and title strip
block z 0 x 0 y 0 w 100 h 100 color #100B1C name DESKTOP_BACKGROUND
block z 1 x 0 y 0 w 100 h 7 color #171027 name TITLEBAR
block z 2 x 0 y 6.8 w 100 h 0.25 color accent name TITLE_ACCENT
text "◆" x 1.5 y 4.7 size 3.1 color #A65BFF bold true
text "TRG ENGINE" x 4.5 y 4.7 size 2.2 color #F8F3FF bold true
text "File    Home    Model    Terrain    Test    View    Plugins" x 19 y 4.6 size 1.15 color #D9C9F2
text "—    □    ×" x 97 y 4.6 size 1.4 color #BFA8E9 align right

// Main ribbon
block z 1 x 0 y 7.1 w 100 h 9.5 color #151020 name RIBBON
text "TOOLS" x 1.4 y 8.8 size 0.9 color #9D8CB8 bold true
button select_tool "➤  Select" x 1 y 9.5 w 8 h 5.6 color #6030A5 (
    say "Select objects in the scene viewport."
)
button move_tool "✥  Move" x 9.3 y 9.5 w 8 h 5.6 color #2B203D (
    say "Move tool selected. Drag a selected object in the viewport."
)
button scale_tool "⛶  Scale" x 17.6 y 9.5 w 8 h 5.6 color #2B203D (
    say "Scale tool selected."
)
button rotate_tool "⟳  Rotate" x 25.9 y 9.5 w 8 h 5.6 color #2B203D (
    say "Rotate tool selected."
)
block z 2 x 34.5 y 8.2 w 0.18 h 7.5 color #45315E name RIBBON_DIVIDER_A
text "CREATE" x 36 y 8.8 size 0.9 color #9D8CB8 bold true
button part_tool "◆  Part" x 36 y 9.5 w 8 h 5.6 color #2B203D (
    3d box auto x 0 y 0 z 0 w 2 h 2 d 2 color currentColor
    push objects last_3d_object
    length objects objectCount
    selectedIndex = objectCount - 1
    get objects selectedIndex selectedName
    set_text selected_label selectedName
    set_text hierarchy_count objectCount
)
button mesh_tool "⬡  Sphere" x 44.3 y 9.5 w 8 h 5.6 color #2B203D (
    3d sphere auto x 0 y 0 z 0 size 1.7 color #8D62F4
    push objects last_3d_object
    length objects objectCount
    selectedIndex = objectCount - 1
    get objects selectedIndex selectedName
    set_text selected_label selectedName
    set_text hierarchy_count objectCount
)
button terrain_tool "▱  Terrain" x 52.6 y 9.5 w 8 h 5.6 color #2B203D (
    3d plane auto x 0 y -1 z 0 w 8 h 0.2 d 8 color #538B69
    push objects last_3d_object
    length objects objectCount
    selectedIndex = objectCount - 1
    get objects selectedIndex selectedName
    set_text selected_label selectedName
    set_text hierarchy_count objectCount
)
button insert_tool "+  Insert" x 60.9 y 9.5 w 8 h 5.6 color #2B203D (
    3d camera auto x 0 y 2 z 4 w 1 h 1 d 1 color #B79BEA
    push objects last_3d_object
    length objects objectCount
    selectedIndex = objectCount - 1
    get objects selectedIndex selectedName
    set_text selected_label selectedName
    set_text hierarchy_count objectCount
)
block z 2 x 69.5 y 8.2 w 0.18 h 7.5 color #45315E name RIBBON_DIVIDER_B
button play_scene "▶  Play" x 71 y 9.5 w 7 h 5.6 color #245C48 (
    say "Play test started."
)
button stop_scene "■  Stop" x 78.3 y 9.5 w 7 h 5.6 color #573044 (
    say "Play test stopped."
)
button save_scene "▣  Save" x 85.6 y 9.5 w 6 h 5.6 color #2B203D (
    3d save "scene.trgscene"
    say "Scene saved to scene.trgscene."
)
button load_scene "▰  Load" x 91.9 y 9.5 w 7 h 5.6 color #2B203D (
    3d load "scene.trgscene"
    say "Scene loaded from scene.trgscene."
)

// Explorer dock
block z 1 x 0.6 y 17.2 w 17 h 63.8 color #191226 edges round name EXPLORER_PANEL
block z 2 x 0.6 y 17.2 w 17 h 5 color #211832 edges round name EXPLORER_HEADER
text "EXPLORER" x 1.8 y 20.4 size 1.25 color #F4ECFF bold true
text hierarchy_count "3 objects" x 16.5 y 20.4 size 0.85 color #A995C5 align right
text "▼  Workspace" x 1.8 y 26 size 1.1 color #E6DDF4 bold true
text "   ◉  Camera" x 2.8 y 31 size 1.0 color #CFC1E6
text "   ▱  Baseplate" x 2.8 y 36 size 1.0 color #CFC1E6
text "   ◆  Cube" x 2.8 y 41 size 1.0 color #CFC1E6
button select_cube "Select Cube" x 1.5 y 44 w 15 h 4.2 color #392451 (
    selectedName = "Cube"
    set_text selected_label selectedName
    say "Cube selected."
)
button select_baseplate "Select Baseplate" x 1.5 y 49 w 15 h 4.2 color #2B203D (
    selectedName = "Baseplate"
    set_text selected_label selectedName
    say "Baseplate selected."
)
button select_camera "Select Camera" x 1.5 y 54 w 15 h 4.2 color #2B203D (
    selectedName = "Camera"
    set_text selected_label selectedName
    say "Camera selected."
)
text "SERVICES" x 1.8 y 63 size 0.9 color #9D8CB8 bold true
text "▸  Lighting" x 2.8 y 67 size 1.0 color #CFC1E6
text "▸  ReplicatedStorage" x 2.8 y 71 size 0.9 color #CFC1E6
text "▸  ServerScriptService" x 2.8 y 75 size 0.82 color #CFC1E6

// Perspective viewport
block z 1 x 18.1 y 17.2 w 60.5 h 45.3 color #0C0915 edges round name VIEWPORT
text "BASEPLATE  ▾" x 19.5 y 19.8 size 0.9 color #BDA9D9 bold true
text "PERSPECTIVE" x 75.8 y 19.8 size 0.8 color #8D7AA9 align right
text "X  Y  Z" x 76 y 22.2 size 0.8 color #A98BDA align right
text "WASD MOVE  •  Q/E UP/DOWN  •  RIGHT-DRAG ORBIT  •  WHEEL ZOOM" x 48.5 y 60.5 size 0.72 color #817394 align center

// Properties dock
block z 1 x 79.2 y 17.2 w 20.2 h 63.8 color #191226 edges round name PROPERTIES_PANEL
block z 2 x 79.2 y 17.2 w 20.2 h 5 color #211832 edges round name PROPERTIES_HEADER
text "PROPERTIES" x 80.4 y 20.4 size 1.15 color #F4ECFF bold true
text selected_label "Cube (Selected)" x 80.4 y 25.5 size 1.05 color #DCCBFA bold true
block z 2 x 80.3 y 27 w 18 h 0.15 color #45315E name PROPERTIES_DIVIDER
text "TRANSFORM" x 80.4 y 30 size 0.9 color #BDA7F5 bold true
text "Move Offset  X / Y / Z" x 80.4 y 34 size 0.85 color #D8CBEA
button edit_position "Edit Position" x 80.4 y 36 w 17.5 h 4.3 color #302044 (
    ask "Move X offset:" editX
    ask "Move Y offset:" editY
    ask "Move Z offset:" editZ
    3d move selectedName editX editY editZ
    say "Position updated."
)
text "Rotation Delta" x 80.4 y 42 size 0.85 color #D8CBEA
button edit_rotation "Edit Rotation" x 80.4 y 44 w 17.5 h 4.3 color #302044 (
    ask "Rotation X:" editX
    ask "Rotation Y:" editY
    ask "Rotation Z:" editZ
    3d rotate selectedName editX editY editZ
    say "Rotation updated."
)
text "Scale" x 80.4 y 50 size 0.85 color #D8CBEA
button edit_scale "Set Scale" x 80.4 y 52 w 17.5 h 4.3 color #302044 (
    ask "Scale multiplier:" editValue
    3d scale selectedName editValue
    say "Scale updated."
)
text "APPEARANCE" x 80.4 y 59 size 0.9 color #BDA7F5 bold true
button change_color "Material / Color" x 80.4 y 61 w 17.5 h 4.3 color #302044 (
    if currentColor = "#8B42F5" ( currentColor = "#438FF5" )
    else ( if currentColor = "#438FF5" ( currentColor = "#58B985" )
    else ( if currentColor = "#58B985" ( currentColor = "#E2A34D" )
    else ( currentColor = "#8B42F5" ) ) )
    3d color selectedName currentColor
    say "Material color changed."
)
button change_texture "Cycle Texture" x 80.4 y 66 w 17.5 h 4.3 color #302044 (
    if currentTexture = "smooth" ( currentTexture = "grid" )
    else ( if currentTexture = "grid" ( currentTexture = "metal" )
    else ( if currentTexture = "metal" ( currentTexture = "wood" )
    else ( if currentTexture = "wood" ( currentTexture = "stone" )
    else ( currentTexture = "smooth" ) ) ) )
    3d texture selectedName currentTexture
    say "Texture updated."
)
button rename_object "Rename" x 80.4 y 71 w 8.2 h 4.3 color #302044 (
    ask "New object name:" renameInput
    3d rename selectedName renameInput
    selectedName = renameInput
    set_text selected_label selectedName
)
button delete_object "Delete" x 89.2 y 71 w 8.7 h 4.3 color #63304A (
    3d delete selectedName
    remove objects selectedIndex
    length objects objectCount
    if objectCount > 0 (
        selectedIndex = 0
        get objects selectedIndex selectedName
        set_text selected_label selectedName
    )
    else (
        selectedName = ""
        set_text selected_label "No selection"
    )
    set_text hierarchy_count objectCount
)

// Bottom toolbox and output dock
block z 1 x 18.1 y 63.2 w 60.5 h 16.8 color #191226 edges round name TOOLBOX_PANEL
block z 2 x 18.1 y 63.2 w 60.5 h 4.5 color #211832 edges round name TOOLBOX_HEADER
text "TOOLBOX  /  ASSETS" x 19.5 y 66.2 size 1.15 color #F4ECFF bold true
text "Models     Parts     Cameras     Spawn" x 76.8 y 66.2 size 0.8 color #A995C5 align right
button asset_box "◆  Box" x 19.5 y 69 w 10 h 8 color #302044 (
    3d box auto x 0 y 0 z 0 w 2 h 2 d 2 color currentColor
    push objects last_3d_object
    length objects objectCount
    selectedIndex = objectCount - 1
    get objects selectedIndex selectedName
    set_text selected_label selectedName
    set_text hierarchy_count objectCount
)
button asset_sphere "●  Sphere" x 30.2 y 69 w 10 h 8 color #302044 (
    3d sphere auto x 0 y 0 z 0 size 1.7 color #8D62F4
    push objects last_3d_object
    length objects objectCount
    selectedIndex = objectCount - 1
    get objects selectedIndex selectedName
    set_text selected_label selectedName
    set_text hierarchy_count objectCount
)
button asset_camera "◉  Camera" x 40.9 y 69 w 10 h 8 color #302044 (
    3d camera auto x 0 y 2 z 4 w 1 h 1 d 1 color #B79BEA
    push objects last_3d_object
    length objects objectCount
    selectedIndex = objectCount - 1
    get objects selectedIndex selectedName
    set_text selected_label selectedName
    set_text hierarchy_count objectCount
)
button asset_spawn "▶  Spawn" x 51.6 y 69 w 10 h 8 color #245C48 (
    3d spawn auto x 0 y 0 z 0 size 1 color #58C78D
    push objects last_3d_object
    length objects objectCount
    selectedIndex = objectCount - 1
    get objects selectedIndex selectedName
    set_text selected_label selectedName
    set_text hierarchy_count objectCount
)
button asset_plane "▱  Plane" x 62.3 y 69 w 10 h 8 color #302044 (
    3d plane auto x 0 y -1 z 0 w 6 h 0.15 d 6 color #70B68A
    push objects last_3d_object
    length objects objectCount
    selectedIndex = objectCount - 1
    get objects selectedIndex selectedName
    set_text selected_label selectedName
    set_text hierarchy_count objectCount
)
button duplicate_object "Duplicate" x 73 y 69 w 4.2 h 8 color #4A3470 (
    3d duplicate selectedName auto
    push objects last_3d_object
    length objects objectCount
    selectedIndex = objectCount - 1
    get objects selectedIndex selectedName
    set_text selected_label selectedName
    set_text hierarchy_count objectCount
)

// Output/status strip
block z 1 x 0.6 y 81.5 w 98.8 h 17.5 color #151020 edges round name OUTPUT_PANEL
text "OUTPUT" x 1.8 y 84.5 size 1.0 color #BDA7F5 bold true
text "TRG Engine ready.  •  Scene workspace loaded." x 1.8 y 88 size 1.0 color #D9C9F2
text "CTRL+S Save Scene     F5 Play     ESC Stop" x 97 y 88 size 0.8 color #8D7AA9 align right
text "READY" x 1.8 y 95 size 0.85 color #75D6A4 bold true
text "TRG ENGINE  •  DESKTOP EDITION" x 97 y 95 size 0.75 color #8D7AA9 align right
