# TRG

## Build and run

In the developer project folder, run `build.bat` to build `TRG.exe`. It builds a Windows desktop app (no terminal window), embeds the purple TRG icon, and statically links the MinGW runtime. It can also use MSVC from a Developer Command Prompt. If `TRG.exe` is already running and Windows prevents replacing it, the script builds `TRG-fixed.exe`; close the old instance and run the script again to update `TRG.exe`. The installer does not include C++ source or build scripts.

Run `build-installer.bat` after building `TRG-fixed.exe` to create `TRG-Installer.exe`. The installer reports the package's storage size, asks for administrator permission to install under `C:\Program Files\TRG-code`, and shows progress while installing. It includes the runtime, icon, README, and interactive example, but not C++ source code. It associates `.trg` files with TRG and places both a TRG desktop shortcut (using the purple icon) and `TRG-Example.trg` on the Desktop. Existing desktop files are preserved; if `TRG.lnk` already exists, the installer creates a uniquely named shortcut instead. The installer uses its embedded files and does not download anything from the internet.

After installation, double-click a `.trg` file to open it with TRG. Before installation, drag a file onto the app or click **Open .trg**. TRG displays output and errors in its window and stays open until you close it. `ask` opens an input dialog. You can also run a file from a command prompt:

```text
TRG.exe hello.trg
```

Package a source file as a single-file executable:

```text
TRG.exe compile hello.trg
```

This creates `hello.exe`, which contains the TRG app and source program. It runs without a separate `.trg` file, but is an interpreter package rather than native machine-code compilation. Packaged apps use the purple TRG icon and show their output in a window.

Open `TRG-Example.trg` from your Desktop for a clickable tour. Each button explains the feature, shows the relevant code, and runs it; UI and animation examples appear in the display area.

## Android app

The Android Studio project is in `android-app`. Open that folder in Android Studio or build a debug APK from PowerShell:

```text
cd android-app
gradlew.bat assembleDebug
```

The APK is written to `android-app\app\build\outputs\apk\debug\app-debug.apk`. TRG Studio's black-and-purple top-left menu button is drawn from `android-app\app\src\main\assets\Studio.trg` and executed by the C++ interpreter. The editor fills the main screen with line numbers. Choose **Run** from the menu to switch to a full-screen output view; reopen the same menu and choose **Back to code** to return. The menu also contains import, export, example, package, and UI preview actions; the output screen is only shown after choosing Run. Android hosts the TRG renderer, source-editing text widget, input dialog, and system document pickers.

**Package .trgpack** saves the current source and a short readme in an importable archive. It is not a standalone native executable, an APK compiler, or machine-code compilation; open the package in TRG Studio to edit or run it. Android keyboard/controller controls and Windows-only features are not available in this mobile runtime.

The Android `studio menu` bridge command opens the native action list from the TRG-defined toolbar button. Android handles platform operations such as document pickers and editor focus; the bundled TRG program defines the toolbar UI, while C++ interprets user programs and returns their output. The editor includes a plain-text command dictionary, syntax colors and typo underlines, new-line indentation, `.trg`/`.txt` tabs, and settings for font size and the color wheel. The program UI preview fills the screen and keeps an on-screen menu button.

UI text labels can be declared inside `add (ui)`:

```text
text "TRG Studio" x 8 y 10 size 8 color #FFFFFF
```

## Language

Comments start with `//`. Variables receive defaults when declared without a value: `num` is `0`, `word` is `none`, `toggle` is `false`, and `color` is `#222222`.

```text
num score
word player = "Alex"
toggle ready = true
color gray = #333333

say "Player:" player "Score:" score
ask "What is your name?" player
```

Colors accept `#RRGGBB`, `0xRRGGBB`, or `0xAARRGGBB`. A group's `default TYPE VALUE` setting applies only inside that group; variables declared there remain available afterward.

```text
group round
{
    default num 10
    num score
    say score
}

ask "What is 1+1?" answer
if answer = 2 (
    say "correct"
)
else (
    say "wrong"
)

if score > 0 {
    say "The score is positive"
}
else {
    say "The score is zero"
}
```

Commands are case-insensitive, so `say`, `Say`, and `SAY` work the same. Numeric expressions support parentheses, exponentiation with `^`, and square roots with `sqrt(VALUE)`:

```text
num area = 3 * 3 ^ 2
num side = sqrt(area)
```

Use `wait 10s` or `wait 100ms` to pause. Durations are seconds by default; add `ms` for milliseconds.
`random MIN MAX RESULT` creates a random number. Whole-number bounds produce an integer; decimal bounds produce a decimal.

`every 1s { ... }` and `every 100ms ( ... )` schedule a block to repeat at that interval while the program UI stays responsive. Use `stop` to end all scheduled work or `break` in a repeating block to remove that repeating task.

For simple beginner-friendly data shapes, `struct` and `class` both declare fields, and `new` creates a named record. Fields can be read or updated with dot notation:

```text
struct Player
{
    word name = "Alex"
    num score = 0
}

new Player hero
hero.score = 10
say hero.name "has" hero.score "points"
```

These are simple field containers; they do not yet define methods or inheritance.

String helpers are `upper VALUE RESULT`, `lower VALUE RESULT`, `length VALUE RESULT`, `contains VALUE SEARCH RESULT`, and `replace VALUE SEARCH REPLACEMENT RESULT`. Lists store text values and use indexes that start at zero:

```text
list names = "Alex", "Sam"
push names "Taylor"
get names 1 selected
remove names 0
```

Project files and TRG modules are enabled with top-level `add` lines, without a following code block. File commands stay inside the project folder. Windows uses the folder containing the running `.trg` file; Android Studio uses the app's private project folder. Import assets in Android Studio before using them.

```text
add (files)
read_file "notes.txt" notes
append_file "notes.txt" "New note"
add (plugin.trg)
```

`add (plugin.trg)` executes that project-local file at that point and makes its functions available to the rest of the program. `add (all)` enables all built-in libraries. File commands are `read_file PATH VARIABLE`, `write_file PATH TEXT...`, `append_file PATH TEXT...`, `file_exists PATH VARIABLE`, and `delete_file PATH`.

TRG Web builds simple pages with TRG commands instead of writing HTML, JavaScript, or CSS. It displays through Android's WebView or the Windows default browser. Android's WebView and current Windows browsers can also load self-contained WebAssembly modules from the project folder:

```text
add (web)
web_page "Welcome"
web_heading "Hello"
web_text "A page made with TRG commands."
web_button "More" "This button is interactive."
web_show
run_wasm "module.wasm"
```

`play_audio FILE`, `show_image FILE`, and `play_video FILE` open supported media from the project folder.

`add (manual)` is the language's basic low-level/register mode; it is built into the TRG runtime, not a separate library. It supports named registers plus `move`, `add`, `subtract`, `multiply`, and `divide`. It is not a full assembler: it does not currently provide arbitrary CPU instructions, machine-code output, or an "ultra assembly" mode.

```text
add (manual)
{
    register {counter} num
    move {counter} 5
    add {counter} 2
    subtract {counter} 1
    say counter
}
```

Windows UI blocks use percentages of the TRG window's canvas for position and size. The output panel remains available below the canvas.

```text
add (ui)
{
    block z 1 x 0 y 0 h 100 w 100 color gray edges round name style
    sprite portrait "portrait.png" x 40 y 20 w 20 h 30
}

add (animation)
{
    animate style x +10 y +10 fps 60 time 2s
}
```

The name declared on a block, button, or sprite is a handle you can use on later lines. For example, `style y +10` moves the named UI item down by 10 percentage points; `style x 25` sets its x position. Use `edges round` or `edges square` on blocks and buttons. A sprite file must be in the project folder; Android users can import the image into TRG Studio first.

For simple games, `add (collision)` enables automatic rectangle overlap checks and movement helpers. `watch_collision A B hit` keeps `hit` updated every scheduled tick, `collides A B hit` checks immediately, and `move_ui player x +1 y 0` moves a named item:

```text
add (ui)
{
    block x 0 y 40 w 10 h 10 color #44AAFF name player
    block x 30 y 40 w 10 h 10 color #FF4444 name wall
}

add (collision)
watch_collision player wall hit
every 100ms
{
    move_ui player x +1 y 0
    if hit = true
    {
        move_ui player x -1 y 0
    }
}
```

UI buttons run their code when clicked:

```text
add (ui)
{
    button greet "Say hello" x 10 y 10 w 30 h 12 color #7043BE opacity 0.8
    {
        say "Hello from a button"
    }
}
```

Buttons accept an optional `color COLOR` (`#RRGGBB` or `0xRRGGBB`) and `opacity VALUE` from `0` (transparent) to `1` (fully opaque). Both desktop and Android previews use these settings.

`add (controls)` declares `key_*` variables for letters, digits, function keys, modifiers, navigation, and numpad keys. On Windows, conditions refresh those values as `press` or `released`. It also declares Xbox/XInput button states such as `controller_a`, `controller_lt`, and `controller_dpad_up`, plus normalized stick axes such as `controller_left_stick_x`. Controller state is read from the first connected XInput controller. Mobile variables (`mobile_tap`, `mobile_hold`, swipes, and buttons) are declared as `unavailable` on Windows.

```text
add (controls)
{
}

toggle was_pressed = false
loop true
{
    if key_x = press
    {
        if was_pressed = false
        {
            say "X was pressed"
            was_pressed = true
        }
    }
    else
    {
        was_pressed = false
    }
    if key_escape = press
    {
        break
    }
}
```
