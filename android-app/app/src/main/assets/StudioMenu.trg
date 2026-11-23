// TRG Studio — redesigned workspace menu
add (ui)

block z 0 x 0 y 0 w 100 h 100 color #100C19 name MENU_BACKGROUND
block z 1 x 0 y 0 w 100 h 1.2 color #9B6BFF name MENU_ACCENT
block z 1 x 3 y 17 w 94 h 70 color #171222 edges round name MENU_PANEL
text "TRG STUDIO" x 6 y 8 size 4.7 color #F4ECFF bold true
text "YOUR DEVELOPMENT WORKSPACE" x 6 y 13 size 1.9 color #9B6BFF bold true
text "Choose an action" x 94 y 13 size 1.8 color #A99ABD align right

button run "RUN PROGRAM" x 5 y 21 w 43 h 8.5 color #6B46A8 text_size 0.28 (
    studio run
)
button open_2d "2D EDITOR" x 52 y 21 w 43 h 8.5 color #30243F text_size 0.28 (
    studio 2d
)
button open_3d "3D EDITOR" x 5 y 31 w 43 h 8.5 color #30243F text_size 0.28 (
    studio 3d
)
button code "BACK TO CODE" x 52 y 31 w 43 h 8.5 color #30243F text_size 0.28 (
    studio code
)
button example "LOAD EXAMPLE" x 5 y 41 w 43 h 8.5 color #30243F text_size 0.28 (
    studio example
)
button import "IMPORT FILE / MEDIA" x 52 y 41 w 43 h 8.5 color #30243F text_size 0.22 (
    studio import
)
button export "EXPORT SOURCE" x 5 y 51 w 43 h 8.5 color #30243F text_size 0.25 (
    studio export
)
button package "CREATE TRGPACK" x 52 y 51 w 43 h 8.5 color #30243F text_size 0.25 (
    studio package
)
button preview "FULL-SCREEN PREVIEW" x 5 y 61 w 43 h 8.5 color #30243F text_size 0.22 (
    studio preview
)
button dictionary "COMMAND DICTIONARY" x 52 y 61 w 43 h 8.5 color #30243F text_size 0.22 (
    studio dictionary
)
button settings "EDITOR SETTINGS" x 5 y 71 w 43 h 8.5 color #30243F text_size 0.25 (
    studio settings
)
button debug "EXPLAIN LAST ERROR" x 52 y 71 w 43 h 8.5 color #30243F text_size 0.22 (
    studio debug
)
button closemenu "CLOSE MENU" x 5 y 89 w 90 h 7 color #4A294F text_size 0.26 (
    studio closemenu
)
