// Bouncy Balls - TRG conversion
// Main menu layout follows the original C++ version reference.
// Gameplay and menu are separate UI groups so the game starts on the menu.

add (ui)
add (collision)
add (physics)
add (controls)

// ---------------------------------------------------------
// ORIGINAL-STYLE MAIN MENU
// Coordinates use the full 0-100 TRG canvas.
// ---------------------------------------------------------
group menu visible true (
    block name menu_background x 0 y 0 w 100 h 100 color #202020 z 0

    text "VERSION 10.45.98" x 70 y 4 size 2.2 color #F2F2F2 font mono
    block name settings_circle x 2 y 2 w 14 h 6 color #71849A shape circle z 2
    text "⚙" x 6.1 y 6.4 size 4.1 color #FFFFFF font mono
    text "SETTINGS" x 17 y 8.4 size 2.1 color #FFFFFF font mono

    text "BOUNCY BALLS" x 19 y 14 size 5.2 color #FFFFFF font mono bold true

    button extras "◇" x 2 y 20 w 16 h 6.5 color #B99AF4 radius 2.7 font mono text_size 0.75 (
        say "Extras"
    )
    text "EXTRAS" x 4.7 y 28.2 size 2.1 color #FFFFFF font mono

    button levels "☷" x 2 y 31.5 w 16 h 6.5 color #A9BAC4 radius 2.7 font mono text_size 0.75 (
        say "Levels"
    )
    text "LEVELS" x 4.7 y 39.8 size 2.1 color #FFFFFF font mono

    button editor "╱" x 2 y 43 w 16 h 6.5 color #74C8E8 radius 2.7 font mono text_size 0.9 (
        say "Editor"
    )
    text "EDITOR" x 4.7 y 51.3 size 2.1 color #FFFFFF font mono

    button shop "SHOP" x 75.5 y 29 w 21.5 h 8.5 color #A7ADB0 radius 3.4 font mono text_size 0.62 (
        say "Shop"
    )

    block name menu_ball_outline x 40.7 y 34.3 w 19 h 8.2 color #111827 shape circle z 3
    block name menu_ball x 41.5 y 34.7 w 17.5 h 7.5 color #FF0000 shape circle z 4

    block name level_pill x 33 y 82.2 w 34 h 4.3 color #426F83 radius 2.3 z 2
    text "LEVEL 1" x 40 y 85.1 size 3.3 color #E8F5FA font mono bold true

    button play "PLAY" x 18 y 90 w 64 h 7.5 color #A8EDB2 radius 3.4 font mono text_size 0.78 (
        group menu visible false (
        )
        group gameplay visible true (
        )
        say "Drag from the ball to aim, then release to launch."
    )
)

// ---------------------------------------------------------
// GAMEPLAY
// ---------------------------------------------------------
group gameplay visible false (
    text "BOUNCY BALLS" x 6 y 6 size 6 color #FFFFFF font mono
    text "Drag from the ball to aim, then release to launch." x 6 y 12 size 2.4 color #CCCCCC font mono

    block z 10 x -1 y -3 w 10 h 108 color #555555 name wall_left
    block z 10 x 90 y -3 w 10 h 108 color #555555 name wall_right
    block z 10 x 9 y -1 w 81 h 4 color #555555 name wall_top
    block z 10 x 9 y 96 w 81 h 4 color #555555 name wall_bottom

    block z 2 x 72 y 85 w 5 h 5 color #39A7FF shape circle name ball
    block z 1 x 12 y 5 w 14 h 5 color #38D878 name finish
    block z 1 x 9 y 72 w 48 h 3 color #F04444 name death_brick
    block z 1 x 55 y 30 w 3 h 20 color #37DCE8 name freeze_pad

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
    physics drag_launch ball 70

    watch_collision ball finish finish_hit
    watch_collision ball death_brick death_hit
    watch_collision ball freeze_pad frozen_hit

    button restart "RESTART" x 72 y 5 w 25 h 9 color #426F83 radius 2.2 font mono text_size 0.5 (
        physics position ball 72 85
        physics reset ball
        game_over = false
        say "Drag from the ball and release to launch."
    )
)

let game_over = false

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
            say "Death brick hit. Press Restart to try again."
        )
    )

    if frozen_hit = true (
        if game_over = false (
            physics freeze ball
            game_over = true
            say "Freeze pad reached. Press Restart to try again."
        )
    )
)
