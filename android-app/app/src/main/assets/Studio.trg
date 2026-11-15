// Persistent top-left menu button for the TRG Studio editor and run screens.
color purple = #8B5CF6

add (ui)

block z 0 x 0 y 0 h 100 w 100 color #0D0B12 name TOOLBAR_BACKGROUND
block z 1 x 0 y 0 h 5 w 100 color purple name TOOLBAR_ACCENT
block z 2 x 0 y 96 h 4 w 100 color #21172E name TOOLBAR_BOTTOM

button menu "MENU" x 2 y 15 w 15 h 70 (
    studio menu
)

text "TRG Studio" x 21 y 63 size 25 color #FFFFFF
text "C++ powered" x 76 y 63 size 14 color #B7A9C9
