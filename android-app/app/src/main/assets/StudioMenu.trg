// TRG Studio — workspace menu
// This complete menu layout is authored in TRG and rendered by the TRG UI engine.
add (ui)

block z 0 x 0 y 0 w 100 h 100 color #0D0B12 name MENU_BACKGROUND
block z 1 x 0 y 0 w 100 h 1 color #8B5CF6 name MENU_ACCENT
block z 1 x 3 y 15 w 94 h 69 color #171421 edges round name MENU_PANEL
text "TRG STUDIO" x 6 y 8 size 4.5 color #FFFFFF bold true
text "WORKSPACE MENU" x 6 y 13 size 2 color #A99DBB
text "Choose a tool or an IDE action" x 94 y 13 size 1.7 color #847A94 align right

button run "RUN PROGRAM" x 5 y 20 w 43 h 8 color #5B35A2 text_size 0.3 (
    studio run
)
button open_2d "2D EDITOR" x 52 y 20 w 43 h 8 color #322542 text_size 0.3 (
    studio 2d
)
button open_3d "3D EDITOR" x 5 y 30 w 43 h 8 color #322542 text_size 0.3 (
    studio 3d
)
button code "BACK TO CODE" x 52 y 30 w 43 h 8 color #322542 text_size 0.3 (
    studio code
)
button example "LOAD EXAMPLE" x 5 y 40 w 43 h 8 color #322542 text_size 0.3 (
    studio example
)
button import "IMPORT FILE / MEDIA" x 52 y 40 w 43 h 8 color #322542 text_size 0.22 (
    studio import
)
button export "EXPORT SOURCE" x 5 y 50 w 43 h 8 color #322542 text_size 0.3 (
    studio export
)
button package "PACKAGE TRGPACK" x 52 y 50 w 43 h 8 color #322542 text_size 0.25 (
    studio package
)
button preview "PREVIEW PROGRAM UI" x 5 y 60 w 43 h 8 color #322542 text_size 0.22 (
    studio preview
)
button dictionary "COMMAND DICTIONARY" x 52 y 60 w 43 h 8 color #322542 text_size 0.22 (
    studio dictionary
)
button settings "EDITOR SETTINGS" x 5 y 70 w 43 h 8 color #322542 text_size 0.25 (
    studio settings
)
button debug "EXPLAIN LAST ERROR" x 52 y 70 w 43 h 8 color #322542 text_size 0.22 (
    studio debug
)
button closemenu "CLOSE MENU" x 5 y 87 w 90 h 8 color #5B273C text_size 0.3 (
    studio closemenu
)
