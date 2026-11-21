// TRG Studio — compact modern IDE toolbar
color accent = #55D6BE

add (ui)

block z 0 x 0 y 0 w 100 h 100 color #0A1019 name TOOLBAR_BACKGROUND
block z 1 x 0 y 0 w 100 h 5 color accent name TOOLBAR_ACCENT
block z 1 x 0 y 95 w 100 h 5 color #142638 name TOOLBAR_BOTTOM
block z 1 x 2 y 15 w 57 h 70 color #111F2D edges round name ACTIONS_PANEL

button menu "MENU" x 3 y 23 w 13 h 54 color #1D3448 text_size 0.27 edges round (
    studio menu
)
button run "RUN" x 17 y 23 w 13 h 54 color #16776F text_size 0.30 edges round (
    studio run
)
button open_2d "2D" x 31 y 23 w 12 h 54 color #1D3448 text_size 0.34 edges round (
    studio 2d
)
button open_3d "3D" x 44 y 23 w 12 h 54 color #1D3448 text_size 0.34 edges round (
    studio 3d
)

text "TRG" x 64 y 58 size 22 color #F1F7FA bold true
text "STUDIO" x 65 y 82 size 7 color accent bold true
text "C++ ENGINE  •  LIVE PREVIEW" x 98 y 39 size 3.2 color #91A9BA align right
