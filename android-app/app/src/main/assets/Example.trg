// TRG Studio demo: try the interpreter, manual registers, controls, and UI.
color accent = #7448C8
num taps = 0
word name = TRG
toggle ready

add (controls)

say "Hello from TRG on Android"
add (ui)

block z 0 x 0 y 0 h 100 w 100 color #26242C name BACKGROUND
block z 1 x 28 y 30 h 40 w 44 color accent name PANEL
text "Tap the button" x 34 y 38 size 5 color #FFFFFF
button hello "Run TRG action" x 32 y 48 w 36 h 14 (
    taps = taps + 1
    say "Button tapped:" taps
    if mobile_tap = press (
        say "Touch input is available as mobile_tap."
    )
)

add (manual)

register counter num
move counter 2
add counter 3
say "Manual register result:" counter

add (animation)

animate PANEL x 8 y 0 fps 60 time 2s
