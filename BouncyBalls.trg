// Bouncy Balls - TRG physics prototype
// Uses the shared TRG UI, collision, and physics libraries.

add (ui)
add (collision)
add (physics)

text "BOUNCY BALLS" x 6 y 6 size 6 color #FFFFFF
text "Launch the ball and reach the green finish." x 6 y 12 size 2.4 color #CCCCCC

// Arena borders
block z 10 x -1 y -3 w 10 h 108 color #555555 name wall_left
block z 10 x 90 y -3 w 10 h 108 color #555555 name wall_right
block z 10 x 9 y -1 w 81 h 4 color #555555 name wall_top
block z 10 x 9 y 96 w 81 h 4 color #555555 name wall_bottom

// Level objects
block z 2 x 72 y 85 w 5 h 5 color #39A7FF shape circle name ball
block z 1 x 12 y 5 w 14 h 5 color #38D878 name finish
block z 1 x 9 y 72 w 48 h 3 color #F04444 name death_brick
block z 1 x 50 y 30 w 3 h 20 color #37DCE8 name freeze_pad

// Register the ball as movable and the level objects as solid.
physics body ball
physics solid wall_left
physics solid wall_right
physics solid wall_top
physics solid wall_bottom
physics trigger finish
physics trigger death_brick
physics trigger freeze_pad

physics gravity ball 0
physics drag ball 0
physics bounce ball 1

watch_collision ball finish finish_hit
watch_collision ball death_brick death_hit
watch_collision ball freeze_pad frozen_hit

let game_over = false

button launch "Launch / Restart" x 72 y 5 w 23 h 10 color #7043BE (
    physics position ball 72 85
    physics unfreeze ball
    physics velocity ball -25 -36
    game_over = false
    say "Ball launched!"
)

every 16ms (
    physics step 0.016

    if finish_hit = true (
        if game_over = false (
            physics freeze ball
            game_over = true
            say "Level complete!"
        )
    )

    if death_hit = true (
        if game_over = false (
            physics freeze ball
            game_over = true
            say "Death brick hit. Press Launch / Restart to try again."
        )
    )

    if frozen_hit = true (
        if game_over = false (
            physics freeze ball
            game_over = true
            say "Freeze pad reached. Press Launch / Restart to try again."
        )
    )
)
