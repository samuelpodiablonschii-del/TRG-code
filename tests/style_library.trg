add (ui)
add (style)

style name glow (
    background #171329
    text #F8F7FF
    glow #A88AFF 0.8
    radius 12
    opacity 0.9
    font sans
    size 0.6
    bold true
    align center
)

style name label_style (
    text #C8B7FF
    opacity 0.95
    font mono
    size 6
    bold true
    align center
)

block name panel x 10 y 10 w 40 h 20 color #FFFFFF
button action "Run" x 10 y 40 w 30 h 12 (
    say "clicked"
)
text title "TRG styles" x 10 y 60 size 5 color #FFFFFF

style apply panel glow
style apply action glow
style apply title label_style

say "TRG_STYLE_LIBRARY_PASSED"
