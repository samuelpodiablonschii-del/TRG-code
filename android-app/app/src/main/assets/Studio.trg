// TRG Studio mobile toolbar — the toolbar layout and controls are TRG UI.
color accent = #8B5CF6

add (ui)

block z 0 x 0 y 0 h 100 w 100 color #0D0B12 name TOOLBAR_BACKGROUND
block z 1 x 0 y 0 h 6 w 100 color accent name TOOLBAR_ACCENT
block z 1 x 0 y 96 h 4 w 100 color #21172E name TOOLBAR_BOTTOM

button menu "MENU" x 2 y 18 w 13 h 64 color #5B35A2 text_size 0.32 edges round (
    studio menu
)
button run "RUN" x 16 y 18 w 11 h 64 color #30243F text_size 0.32 edges round (
    studio run
)
button open_2d "2D" x 29 y 18 w 11 h 64 color #30243F text_size 0.38 edges round (
    studio 2d
)
button open_3d "3D" x 42 y 18 w 11 h 64 color #30243F text_size 0.38 edges round (
    studio 3d
)

text "TRG" x 59 y 54 size 23 color #FFFFFF bold true
text "STUDIO" x 60 y 80 size 8 color #BDB0D5 bold true
text "POWERED BY TRG" x 98 y 37 size 5 color #9B8DAF align right
