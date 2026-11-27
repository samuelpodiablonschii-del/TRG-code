# TRG

`code.txt` is an exact, automatically synchronized copy of the latest C++ interpreter source in `TRG.cpp`.

## Build and run

Run `build.bat` from the repository root to build the Windows desktop applications. It supports MinGW-w64 or MSVC from a Visual Studio Developer Command Prompt. It creates `Checker.exe`, `UI-app.exe`, `Console-app.exe`, and `TRG.exe` (the editor). The two app executables are compiled as separate UI and terminal launch modes.

Run `build-installer.bat` to build the applications and create `TRG-Installer.exe` in one step. The installer embeds Checker, both app executables, TRG Studio, the purple icon, README, and the interactive example. It installs under `C:\Program Files\TRG Code`, registers `.trg` files to open with `Checker.exe`, and adds **Edit in TRG** to the right-click menu. The Desktop shortcut opens TRG Studio; the example file is placed on the Desktop and launches through Checker. Existing Desktop files are preserved, and the installer does not download anything from the internet.

When a `.trg` file is opened, `Checker.exe` reads the source and looks for the same UI library declarations used by the interpreter: `add (ui)`, `add (animation)`, or `add (all)`. It starts `UI-app.exe` for those programs and `Console-app.exe` for programs without a UI library. Checker exits after launching the selected app. No background runtime or Windows startup task is installed.

The Windows desktop editor has **Open**, **Save**, **Run**, **Preview**, **New**, **Compile**, **Export .TRG**, **Dictionary**, **2D Editor**, and **3D Engine** buttons. The built-in visual editors are embedded in the TRG runtime, so opening them does not replace the current source tab or appear as a separate `.trg` document. The editor uses the purple TRG theme, a Consolas code editor, syntax highlighting for commands, strings, numbers, comments, and colours, automatic bracket pairing, and four-space block indentation. **Compile** saves the active source and packages the TRG runtime plus program into a standalone `.exe` beside it; this is an interpreter package, not native machine-code compilation. **Export .TRG** saves a copy of the current source as a TRG file. **Dictionary** opens a scrollable, selectable language reference. The output pane is a scrollable, selectable terminal that follows new output automatically; TRG `ask` prompts use its inline input field instead of a separate dialog.

The desktop **3D Engine** opens its embedded native workspace directly in TRG Studio. Its Roblox Studio-inspired desktop layout has a TRG Engine title bar and Home/Model/Terrain/View/Plugins tabs, a tool ribbon, left Explorer, central perspective viewport with an XYZ orientation gizmo, right Properties inspector, bottom Toolbox/Assets dock, and Output terminal. The ribbon provides Select, Move, Rotate, Scale, Part, Sphere, Camera, Spawn, Plane, Play preview, Stop, Save, and Load. Assets can be clicked or dragged into the viewport. Select objects in the Explorer or viewport; right-drag to orbit, use the mouse wheel to zoom, use **W/A/S/D** to move the camera, **Q/E** to move vertically, and the arrow keys to look around. Click a Properties row to edit the object's name, absolute position, rotation, size, material color, texture, or visibility. Right-click an object for rename, duplicate, delete, texture, focus, and transform actions. Scene files use the versioned `.trgscene` format. The renderer is a TRG-native primitive editor, not full Unity, Unreal, or Roblox Studio feature parity.

After installation, double-click a `.trg` file to open it with TRG. Before installation, drag a file onto the app or click **Open .trg**. TRG displays output and errors in its window and stays open until you close it. `ask` opens an input dialog. You can also run a file from a command prompt:

```text
TRG.exe hello.trg
```

Package a source file as a single-file executable:

```text
TRG.exe compile hello.trg
```

This creates `hello.exe`, which contains the TRG app and source program. It runs without a separate `.trg` file, but is an interpreter package rather than native machine-code compilation. Packaged apps use the purple TRG icon. Launching a `.trg` file or a compiled app that enables `add (ui)` opens a separate full-window TRG UI app with the editor, code pane, and terminal hidden. Programs without `add (ui)` open in a dedicated purple TRG Terminal window; the desktop editor's own Preview button remains available when you run a project from the editor.

Open `TRG-Example.trg` from your Desktop for a clickable tour. Each button explains the feature, shows the relevant code, and runs it; UI and animation examples appear in the display area.

## Interpreter performance and asynchronous networking

TRG precomputes matching block boundaries for each source body before execution. Nested control-flow blocks reuse that index, and reusable function, repeating-task, and UI-button bodies retain their own compiled block maps. This avoids repeatedly scanning large source vectors when entering the same blocks.

For network operations, the existing `http_get` and `http_post` commands remain synchronous for compatibility. Use `http_get_async` or `http_post_async` to queue a request on TRG's small worker pool without blocking the interpreter's UI/update thread:

```trg
add (network)
http_get_async "https://example.com" request
word response = ""
word status = pending

loop status = pending (
    http_result request response status
    if status = pending (
        wait 5ms
    )
)

if status = complete (
    say response
)
http_forget request
```

The status returned by `http_result` is `pending`, `complete`, `error`, or `unknown`. Completed request entries can be released with `http_forget`.

## Android app

The Android Studio project is in `android-app`. Install Android Studio with a JDK (17 or newer; the CI build uses Java 21) and these SDK components: Android SDK Platform 35, Build-Tools 36.0.0, NDK 28.2.13676358, and CMake 3.22.1. Then double-click `build-apk.bat` in the repository root. It detects common Android Studio JDK/SDK locations, checks the required SDK components, runs the Gradle wrapper, and opens File Explorer with the generated APK selected. The first build needs internet access to download Gradle and Android/Gradle dependencies. You can also build from PowerShell:

```text
cd android-app
gradlew.bat assembleDebug
```

The APK is written to `android-app\app\build\outputs\apk\debug\app-debug.apk`. TRG Studio's black-and-purple top-left menu button is drawn from `android-app\app\src\main\assets\Studio.trg` and executed by the C++ interpreter. The editor fills the main screen with line numbers. Choose **Run** from the menu to switch to a full-screen output view; reopen the same menu and choose **Back to code** to return. The menu also contains import, export, example, package, and UI preview actions; the output screen is only shown after choosing Run. Android hosts the TRG renderer, source-editing text widget, input dialog, and system document pickers.

**Package .trgpack** saves the current source and a short readme in an importable archive. It is not a standalone native executable, an APK compiler, or machine-code compilation; open the package in TRG Studio to edit or run it. Android keyboard/controller controls and Windows-only features are not available in this mobile runtime.

### TRG-authored mobile workbench

The mobile workspace menu and toolbar are TRG UI programs in `android-app/app/src/main/assets`:
- `Studio.trg` defines the persistent toolbar.
- `StudioMenu.trg` defines the workspace menu.
- `2DEditor.trg` and `3DEditor.trg` define the visual editor screens.

Android hosts these TRG-authored screens in a drawing surface and handles platform actions such as document pickers and text input. The visual layout, buttons, editor controls, and object-editing behavior are TRG programs; they are not separate native Android editor screens. The toolbar provides direct Run, 2D, and 3D actions, and the workspace menu keeps the existing import, export, package, dictionary, settings, and preview actions.

The 2D editor is a canvas editor: tap an object to select it, then drag it directly to reposition it. Its scene canvas is white by default, new objects use purple, and the Background control changes the canvas colour. It can create blocks, circles, UI buttons, and text labels; duplicate and delete items; rename objects with **Name**; resize an item with one **Resize** action; change colour and shape; and add position animations to blocks, buttons, text, and sprites. **Copy Code** copies a ready-to-paste creation command, including the required library line and the object's `name`, to the Android clipboard.

The 3D library also supports portable scene files: `3d save "scenes/level.trgscene"` writes the current 3D object list, and `3d load "scenes/level.trgscene"` restores it. Scene loading validates the complete file before replacing the current scene, and paths are restricted to the project directory. The versioned `.trgscene` format preserves object names, groups, primitive types, colours, transforms, opacity, and visibility. This is scene persistence for the current box/sphere renderer, not yet a full Unity-style editor or mesh/physics pipeline.

The 3D editor creates box and sphere primitives on a white scene by default; its Background control changes the viewport colour. Drag a model to move it, drag empty viewport space to orbit the camera, and use two fingers to orbit and pinch-zoom together. **Reset View** restores the starting camera. It also supports selection, duplication, deletion, naming, resizing, rotation, colour changes, and **Copy Code** for the selected model, including the rotation command when needed. Both editors use the same dark, high-contrast visual theme and a fixed-width monospace typeface.

These screens use the general-purpose UI and 3D libraries documented below. The mobile 3D view now has interactive camera orbit and zoom for the existing box/sphere renderer. It is still a lightweight primitive editor, not a general mesh importer, shader pipeline, or full 3D physics engine.

The Android `studio menu` bridge connects TRG-authored toolbar and workspace-menu buttons to platform actions. Android handles platform operations such as document pickers and editor focus; the bundled TRG programs define the visible toolbar, workspace menu, and editor screens, while C++ interprets user programs and returns their output. The editor includes a black command dictionary with a purple outline and bold white monospace text, syntax colours and typo underlines, new-line indentation, `.trg`/`.txt` tabs, and settings for font size and the colour wheel. Program previews fill the screen and include a floating X control to return to code. The program UI preview fills the screen and keeps an on-screen menu button.

UI text labels can be declared inside `add (ui)`:

```text
text "TRG Studio" x 8 y 10 size 8 color #FFFFFF
```

### Orientation, screen dimensions, and automatic image sizing

TRG exposes the live window dimensions and orientation to every program. The values update when the desktop window is resized:

- `screen_width` and `screen_height` are the client-area dimensions in pixels.
- `orientation` (also `screen_orientation`) is `"landscape"`, `"portrait"`, or `"square"`.
- `landscape`, `portrait`, `is_landscape`, and `is_portrait` are boolean values.
- `aspect_ratio` is reduced automatically, such as `"16:9"`, `"3:4"`, or `"1:1"`; `aspect_width` and `aspect_height` are its numeric parts.

```text
if orientation = "landscape" (
    say "Landscape layout"
)
if portrait = true (
    say "Portrait layout"
)
say "Current ratio:" aspect_ratio
```

UI image coordinates and dimensions can be omitted and use automatic defaults. Images support optional `x`, `y`, `w`/`width`, and `h`/`height` properties. Set a dimension to `auto`, or omit it, to preserve the source image's intrinsic aspect ratio. For example, `w 5 h auto` fixes the width to five percent of the UI canvas and calculates the height from the image file's own ratio. With both dimensions automatic, the image is shown at its natural pixel size, scaled down only when needed to fit the canvas.

```text
add (ui)
image logo "assets/logo.png" x 5 y 5 w 5 h auto
```

Omitted block and button coordinates retain their default automatic positions and sizes; `auto` is accepted for their optional x/y/w/h properties as well.

## Learn TRG the easy way

You can start coding without learning variable types first. TRG infers the value type when you use `var` (`let` remains accepted as a legacy alias).

Your first program:

```text
word name = "Sam"
num score = 10

say "Hello" name
say "Your score is" score

score = score + 5
say "New score:" score
```

That's the basics:
- `var` creates a variable. Numbers, text, and true/false values are detected automatically. The older `let` spelling remains supported.
- `say` displays something. It is the easy-to-remember version of `say`.
- `ask` asks the user a question and stores their answer.
- `if` checks something; `else` runs when it is not true.
- `loop` repeats code while a condition is true.
- Use `(` and `)` around blocks.

A small interactive program:

```text
var name = "there"
ask "What is your name?" name

if name = "Sam" (
    say "Welcome back!"
)
else (
    say "Nice to meet you," name
)
```

You can still use the typed declarations (`num`, `word`, `toggle`, and `color`) and `say`. They work exactly as before. The shorter beginner syntax is just another way to write TRG.

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

Colors accept `#RRGGBB`, `0xRRGGBB`, or `0xAARRGGBB`.

### Blocks use parentheses

Every block now uses `(` and `)`. Curly braces are not part of TRG and produce an error. Libraries do not take a block at all: `add (json)` applies to the whole file.

```text
if score > 0 (
    say "The score is positive"
)
else (
    say "The score is zero"
)

loop score > 0 (
    score = score - 1
)

every 1s (
    say "tick"
)
```

### Groups and defaults

A group's `default TYPE VALUE` setting applies only inside that group. Variables declared inside a group stay available afterward, and they are remembered as members of that group so `json` can serialize them. Variables in nested groups also belong to the outer group.

```text
group round (
    default num 10
    num score
    say score

    group inner (
        num bonus = 5
    )
)
```

### Conditions and expressions

Commands are case-insensitive, so `say`, `Say`, and `SAY` work the same. Numeric expressions support parentheses, exponentiation with `^`, and square roots with `sqrt(VALUE)`:

```text
num area = 3 * 3 ^ 2
num side = sqrt(area)
```

### Functions and return values

Define reusable functions with `define`. Parameters are local to each call, and declarations made inside a function do not leak into the caller. A function can return a typed value with `return`; capture it with `-> RESULT_VARIABLE`.

```text
define add_bonus amount (
    num private_value = 100
    return amount + 5
)

add_bonus 10 -> total
say "Total:" total
```

A function can also be called without capturing its result. Assignments to variables that already exist in the caller remain visible, while newly declared function variables stay local. Runtime errors inside functions include the function call chain and call-site line numbers.

### Typed lists

Lists can contain numbers, text, toggles, other lists, and dictionaries. Values retrieved from a list keep their original type.

```text
list inventory = "key", 3, true
push inventory 12
get inventory 1 count
say "Count:" count

for each item in inventory (
    say item
)
```

Use `push LIST VALUE`, `get LIST INDEX VARIABLE`, `remove LIST INDEX`, `insert LIST INDEX VALUE`, and `pop LIST RESULT` to edit lists. `split` also creates a list, and `join` combines its values into text.

### Dictionaries

Dictionaries store typed values under text keys:

```text
dictionary player
dict_set player name "Sam"
dict_set player score 42
dict_get player score score
dict_has player name has_name
dict_count player entries
dict_keys player keys
dict_remove player name
```

The commands are `dict_set`, `dict_get`, `dict_has`, `dict_remove`, `dict_keys`, and `dict_count`. Dictionary values can be numbers, text, toggles, lists, or other dictionaries.

Use `wait 10s` or `wait 100ms` to pause. Durations are seconds by default; add `ms` for milliseconds.

`random MIN MAX RESULT` creates a random number. Whole-number bounds produce an integer; decimal bounds produce a decimal.

`every 1s ( ... )` and `every 100ms ( ... )` schedule a block to repeat at that interval while the program UI stays responsive. Use `stop` to end all scheduled work or `break` in a repeating block to remove that repeating task.

### Database

TRG includes a portable embedded database engine. It needs no external server or database package and stores data in a compact project-local `.trgdb` file. Tables have named columns and records; values are stored as text so a program can choose its own types. Changes are saved automatically, with an explicit save command available too.

```text
add (database)
database open "game.trgdb"
table create players name score level
record add players name "Alex" score 120 level 3
record add players name "Jordan" score 95 level 2
record get players 0 name player_name
say "First player:" player_name
query players score >= 100 winners
say "Matching rows:" query_count
record update players 0 score 150
record delete players 1
database save
```

Commands:
- `database open FILE`, `database save`, `database close`, `database tables`
- `table create NAME COLUMN...`, `table delete NAME`, `table count NAME RESULT`
- `record add TABLE FIELD VALUE...`, `record get TABLE INDEX FIELD RESULT`
- `record update TABLE INDEX FIELD VALUE`, `record delete TABLE INDEX`
- `query TABLE FIELD OP VALUE RESULT` where OP is `=`, `!=`, `>`, `<`, `>=`, or `<=`
- SQL-like query: `sql select from TABLE where FIELD OP VALUE into RESULT`

Query results are a list of matching row indexes; `query_count` contains the number of matches. Database files are restricted to the current TRG project folder. This is TRG's own embedded engine, not a drop-in implementation of SQLite, Oracle SQL, Firebase, or a network database server. Cloud sync and remote backends can be added as optional adapters.

### Records

`record` declares a beginner-friendly field container. `struct` and `class` are kept as aliases for older programs, and `new` creates a named instance. Fields can be read or updated with dot notation:

```text
record Player (
    word name = "Alex"
    num score = 0
)

new Player hero
hero.score = 10
say hero.name "has" hero.score "points"
```

These are simple field containers; they do not yet define methods or inheritance.

### Lists and strings

String helpers include `upper VALUE RESULT`, `lower VALUE RESULT`, `trim VALUE RESULT`, `length VALUE RESULT`, `contains VALUE SEARCH RESULT`, `index_of VALUE SEARCH RESULT`, `replace VALUE SEARCH REPLACEMENT RESULT`, and `substring VALUE START LENGTH RESULT`. `index_of` returns `-1` when the text is not found. `split VALUE DELIMITER RESULT` converts text into a list.

Lists store text values and use indexes that start at zero. `push` adds to the end, `insert` adds at an index, `get` reads an item, `remove` deletes an item, `pop` removes the last item and stores it in a variable, and `join` combines a list into text:

```text
list names = "Alex", "Sam"
push names "Taylor"
insert names 1 "Jordan"
get names 1 selected
index_of selected "or" found_at
pop names last_name
join names ", " all_names
split "red|green|blue" "|" colors
trim "  hello  " clean_text
substring "TRG language" 0 3 prefix
say selected found_at last_name all_names
```

## Libraries

Libraries are file-wide. Put `add (...)` anywhere in the file and every line in that file can use it. Do not put a block after a library line.

```text
add (files)
add (json)
add (all)
```

`add (all)` enables every built-in library, including `json`, `math`, `ai`, `binary`, `regex`, `compression`, `time`, `convert`, `stats`, `units`, `style`, and `3d`. Everyday text and number commands such as `find`, `replace_text`, `add_numbers`, and `square_root` work without adding a library.

The PowerShell, Raspberry, and accessibility libraries are also registered:

```text
add (powershell)
powershell get_date current_date

add (raspberry)
pin_mode 15 output
digital_write 15 high
digital_read 14 button_state

add (accessibility)
popup "Hello from TRG"
tap 500 300
open_app "Calculator"
autoclick 500 300 interval 100ms
stop_autoclick
```

Command style:
- PowerShell commands keep the `powershell` prefix because they invoke a separate shell.
- Raspberry commands do not repeat `raspberry` on every line. Use direct commands such as `pin_mode`, `digital_write`, and `analog_read`.
- Accessibility commands do not repeat `accessibility`. Use direct commands such as `popup`, `tap`, `swipe`, `open_app`, and `autoclick`.

**Implementation status:** the three library names are recognised by the library scanner and `add (all)`. Their hardware and operating-system command backends are not implemented yet; the examples above describe the intended syntax, not commands that are ready to run. The existing PowerShell helper is currently a whitelist-checking placeholder and does not launch PowerShell.


### Time and conversion

`add (time)` reads the current local date/time or Unix timestamp. `add (convert)` converts simple values between number, text, and boolean types.

```text
add (time)
time year current_year
time month current_month
time day current_day
time hour current_hour
time unix timestamp

add (convert)
convert number "42.5" amount
convert text amount label
convert bool "yes" enabled
```

### Statistics

`add (stats)` calculates common statistics from a numeric list. The list must contain only values that can be read as numbers.

```text
list scores = 10, 20, 30, 40
add (stats)
stats sum scores total
stats average scores average_score
stats median scores middle_score
stats min scores lowest
stats max scores highest
stats range scores spread
```

### Unit conversions

`add (units)` converts common temperature, distance, and mass units. The result is stored as a number.

```text
add (units)
units c_to_f 20 fahrenheit
units km_to_mi 10 miles
units m_to_ft 5 feet
units kg_to_lb 2 pounds
```

Supported conversions: `c_to_f`, `f_to_c`, `km_to_mi`, `mi_to_km`, `m_to_ft`, `ft_to_m`, `kg_to_lb`, and `lb_to_kg`.
### Math

`add (math)` enables beginner-friendly math commands. Angles for `sin`, `cos`, and `tan` are in radians.

```text
add (math)
math sin 0 result
math sqrt 81 root
math min 5 10 smaller
math max 5 10 larger
math clamp 15 0 10 limited
math distance 0 0 3 4 distance_value
math round 2.6 nearest
```

Supported operations: `sin`, `cos`, `tan`, `abs`, `floor`, `ceil`, `round`, `sqrt`, `min`, `max`, `clamp`, and `distance`.
### Plain-English commands

You do not need to know terms like *regular expression* to do common text jobs. These commands work directly:

```text
find "Hello there" "there" found
find_position "Hello there" "there" place
replace_text "Hello there" "there" "TRG" updated
count_text "one fish two fish" "fish" how_many
starts_with "TRG language" "TRG" begins
ends_with "TRG language" "language" ends

add_numbers 5 3 total
subtract_numbers 5 3 difference
multiply_numbers 5 3 product
divide_numbers 10 2 answer
square_root 81 root
round_number 2.6 rounded
absolute_number -12 positive
minimum 5 3 smaller
maximum 5 3 bigger
distance_between 0 0 3 4 distance
```

- `find` gives `true` if the text is there, otherwise `false`.
- `find_position` gives the starting character number, or `-1` if it is missing.
- `replace_text` changes every matching part of the text.
- `count_text` counts matching parts that do not overlap.
- `starts_with` and `ends_with` check the beginning and end.
- The number commands do the operation named by their command. Division by zero is reported as an error.

These are built-in commands; you do not need to add a library first. Advanced `regex` and `math` commands remain available too.

### Binary and bitwise tools

TRG includes byte conversion, encoding, bitwise operations, and checksums without extra packages.

```text
add (binary)

binary to_bin 42 binary_text
binary from_bin 101010 number_again
binary to_hex "TRG" hex_text
binary from_hex "545247" decoded_text
binary base64_encode "Hello TRG" encoded
binary base64_decode "SGVsbG8gVFJH" decoded

bit and 12 10 bit_result
bit xor 12 10 xor_result
bit shift_left 3 2 shifted
bit not 0 inverted

checksum crc32 "123456789" crc
checksum fnv1a "TRG" fingerprint
```

- `binary to_bin VALUE RESULT` and `from_bin` convert unsigned whole numbers (up to 53 bits) to/from binary text.
- `to_hex` and `from_hex` convert text bytes to/from hexadecimal.
- `base64_encode` and `base64_decode` handle Base64 text.
- `bit and|or|xor|not|shift_left|shift_right` perform unsigned 32-bit operations.
- `checksum crc32|fnv1a TEXT RESULT` calculates a non-cryptographic checksum/hash. These are **not encryption or password-security functions**.

### Regular expressions

`add (regex)` enables pattern matching, search, and replacement using C++ ECMAScript regular expressions.

```text
add (regex)
regex match "TRG language" "^TRG" starts_with_trg
regex search "version 4.2" "[0-9]+\\.[0-9]+" version_position
regex replace "TRG language" "language" "runtime" updated
```

### Compression

`add (compression)` enables a built-in run-length encoding (RLE) format. It is useful for learning and repeated data; it is not a general-purpose replacement for ZIP, zlib, or LZMA.

```text
add (compression)
compress encode "aaabbbcccc" packed
compress decode packed restored
```

RLE output is text-safe and stores each run as a count plus a hexadecimal byte. Decoding is limited to 16 MiB per result.

### AI

`add (ai)` enables TRG's small, beginner-friendly machine-learning library. It uses a trainable logistic model for simple yes/no classification. It is designed for learning and small experiments, not as a replacement for full PyTorch or large language models.

```text
add (ai)

ai create gate 2
ai teach gate 0;0 0
ai teach gate 0;1 1
ai teach gate 1;0 1
ai teach gate 1;1 1
ai train gate 3000 0.1

ai classify gate 1;0 answer
say "Should turn on:" answer

ai predict gate 1;1 confidence
say "Confidence:" confidence
```

- `ai create MODEL INPUT_COUNT` creates a model with 1-64 numeric inputs.
- `ai teach MODEL INPUTS EXPECTED` adds an example. Separate input values with semicolons; expected output must be `0` or `1`.
- `ai train MODEL EPOCHS [LEARNING_RATE]` trains from the examples.
- `ai predict MODEL INPUTS RESULT` stores a probability from 0 to 1.
- `ai classify MODEL INPUTS RESULT` stores `true` or `false` using a 0.5 threshold.
### Files

File commands stay inside the project folder. Windows uses the folder containing the running `.trg` file; Android Studio uses the app's private project folder. Import assets in Android Studio before using them.

```text
add (files)
read_file "notes.txt" notes
append_file "notes.txt" "New note"
add (plugin.trg)
```

`add (plugin.trg)` executes that project-local file at that point and makes its functions available to the rest of the program. File commands are `read_file PATH VARIABLE`, `write_file PATH TEXT...`, `append_file PATH TEXT...`, `file_exists PATH VARIABLE`, and `delete_file PATH`.

### JSON

`add (json)` provides JSON creation and reading without any external dependency.

```text
add (json)

group MyGroup (
    num score = 10
    word name = "Alex"
    toggle ready = true
    list tags = "a", "b"
)

json MyGroup clean MyGroup_json
json MyGroup compressed MyGroup_compact

create "MyGroup.json" (saves) MyGroup_json
json read (saves/MyGroup.json)
```

- `json GROUP clean RESULT` stores pretty JSON in `RESULT`: one key per line, keys sorted longest-first (top longer, bottom shorter).
- `json GROUP compressed RESULT` stores compact JSON with no spaces.
- `GROUP` is the name used by `group GROUP ( ... )`. Variables from nested groups are included in the outer group's JSON.
- `create FILE (LOCATION) RESULT` writes the JSON text held in `RESULT` to `LOCATION/FILE`. The location is created if needed, and the path must stay inside the project folder.
- `json read (LOCATION/FILE)` reads a JSON file and sets variables. Numbers become `num`, `true`/`false` become `toggle`, strings become `word`, arrays become `list`, and nested objects become dotted names such as `player.name`.

Example `MyGroup_json`:

```json
{
  "long_variable_name": 42,
  "nested_flag": true,
  "short": "hi",
  "tags": ["a","b"],
  "z": 7
}
```

### Project-wide globals

`add global ( ... )` runs its block once before the project's programs and makes its variables and libraries available to every `.trg` file in the project folder and in subfolders.

```text
add global (
    num ProjectVersion = 1
    word ProjectName = "My Game"
    add (json)
)
```

Project-wide discovery is performed for saved files inside a project folder. An untitled editor tab is treated as a standalone program, so save it into the project folder to load globals declared by other files. The current tab's own `add global` block still works before it is saved.

### Connected files and run

`add connect (file.trg)` treats another project file as part of the same program. Connected files run once, share variables and data, and follow their own connections, so connections are transitive and undirected: if one file connects A and B, then A and B are connected too. A file can also run another file explicitly with `run (file.trg)`; if that file already ran as part of the connected program, `run` skips it instead of running it twice.

```text
add connect (player.trg)
add connect (levels/level1.trg)

run (tools/helpers.trg)
```

Both `add connect` and `run` only accept files inside the project folder. Absolute paths, `..` paths, and paths that resolve outside the project (including through symlinks) are rejected.

## Web, media, and low-level libraries

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

register counter num
move counter 5
add counter 2
subtract counter 1
say counter
```

Windows UI blocks use percentages of the TRG window's canvas for position and size. The output panel remains available below the canvas.

```text
add (ui)

block z 1 x 0 y 0 h 100 w 100 color gray edges round name style
sprite portrait "portrait.png" x 40 y 20 w 20 h 30

add (animation)

animate style x +10 y +10 fps 60 time 2s
```

The name declared on a block, button, or sprite is a handle you can use on later lines. For example, `style y +10` moves the named UI item down by 10 percentage points; `style x 25` sets its x position. Use `edges round` or `edges square` on blocks and buttons. A sprite file must be in the project folder; Android users can import the image into TRG Studio first.

### Physics

`add (physics)` adds reusable 2D physics bodies to named TRG UI blocks, sprites, and buttons. Physics coordinates use the same percentage-based canvas units as the UI. Gravity, drag, and bounce are configurable per body.

```text
add (ui)
add (physics)

block x 10 y 10 w 5 h 5 color #44AAFF shape circle name ball
block x 0 y 90 w 100 h 5 color #888888 name floor
block x 0 y 0 w 5 h 100 color #888888 name left_wall
block x 95 y 0 w 5 h 100 color #888888 name right_wall

physics body ball
physics solid floor
physics solid left_wall
physics solid right_wall
physics velocity ball 35 -10
physics gravity ball 0
physics bounce ball 1

every 16ms (
    physics step 0.016
)
```

Commands:
- `physics body OBJECT` registers a moving body.
- `physics solid OBJECT` registers a stationary collider that reflects moving bodies.
- `physics trigger OBJECT` registers a non-solid overlap target. Triggers never bounce bodies; use `watch_collision` to react when a body overlaps one.
- `physics velocity BODY VX VY` sets velocity in canvas units per second.
- `physics gravity BODY VALUE` sets vertical acceleration; zero means no gravity.
- `physics drag BODY VALUE` applies exponential velocity damping; zero means no drag.
- `physics bounce BODY VALUE` sets the restitution multiplier. `1` keeps speed, values below `1` reduce bounce speed, and values above `1` increase it.
- `physics step SECONDS` advances all dynamic bodies by up to 0.1 seconds. Put it in an `every` block for continuous simulation.
- `physics position BODY X Y` teleports a registered body to a canvas position.
- `physics freeze BODY` stops a dynamic body and suspends its movement until unfrozen.
- `physics unfreeze BODY` resumes a frozen body; set its velocity separately to launch it again.
- `physics drag_launch BODY MAX_SPEED` waits for a pointer drag that starts inside the body, then launches it in the direction of the drag when released. The speed is scaled from drag distance and capped by `MAX_SPEED`.
- Add `(controls)` to expose `pointer_x` and `pointer_y` as 0–100 canvas coordinates and `pointer_down` as `press` or `released`. Pointer input is supported by the desktop mouse and Android touch screen.
- `physics reset BODY` stops the body and resets its one-shot drag-launch state for another attempt.
- `physics remove OBJECT` unregisters an object.

The first version resolves axis-aligned rectangle collisions against registered solid bodies. It does not yet include rotated-shape collision, frictional contact, joints, or continuous collision detection for very fast objects. For accurate results, use small fixed steps (for example, 0.016 seconds).

### Reusable UI styles

`add (style)` enables named, reusable visual styles for UI blocks, buttons, and text. Define a style with `style name NAME ( ... )` and apply it with `style apply OBJECT NAME`. The property commands are deliberately simpler than CSS:

```text
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

block name panel x 10 y 10 w 40 h 20 color #FFFFFF
style apply panel glow
```

Supported style properties are `background`, `text`, `glow COLOR [INTENSITY]`, `radius`, `opacity`, `font`, `size`, `bold`, and `align`. Blocks use background, glow, radius, and opacity. Buttons also support text colour, font, text size, and bold. Text labels support text colour, font, size, bold, alignment, and opacity. Glow is currently rendered on blocks.

### Editing 2D UI objects

After `add (ui)`, TRG can create and edit objects at runtime. Coordinates and sizes use the same 0–100 canvas units as the UI library.

```text
add (ui)

ui create block panel x 10 y 15 w 30 h 20 color #8B5CF6 shape square z 5
ui duplicate panel panel_copy
ui move panel_copy 5 0
ui resize panel_copy 2 2
ui color panel_copy #22C55E
ui shape panel_copy circle
ui rename panel_copy green_circle
ui delete panel
```

Supported operations are `ui create block`, `ui duplicate` (or `ui copy`), `ui rename`, `ui move`, `ui resize`, `ui color`, `ui shape`, and `ui delete`. A new object name can be `auto`; the generated name is returned in `last_ui_object`. The current shape choices are square, circle, triangle, pentagon, hexagon, and star.

### 3D primitives

The general-purpose `add (3d)` library creates reusable scene objects in world coordinates. Rotation values are degrees; move and rotate values are increments, while scale is a positive multiplier.

```text
add (ui)
add (3d)

3d box platform x 0 y -2 z 0 w 8 h 0.5 d 5 color #8B5CF6
3d sphere ball x 0 y 1 z 0 size 1.5 color #22C55E
3d duplicate platform platform_copy
3d move ball 0 0.5 0
3d rotate platform 0 15 0
3d scale platform_copy 1.2
3d color ball #3B82F6
3d rename ball player_ball
3d delete platform_copy
```

The initial primitive set is box and sphere. Use `3d scale NAME FACTOR` for uniform scaling or `3d scale NAME X Y Z` for independent axis factors. The mobile Studio editor uses a perspective-style grid and axis gizmo, supports model selection and dragging, one-finger camera orbit, two-finger orbit and pinch zoom, inspector transforms and colour changes, and project-relative scene save/load. It remains a lightweight primitive renderer: arbitrary imported meshes, a shader pipeline, and full 3D physics are not implemented.

For simple games, `add (collision)` enables automatic rectangle overlap checks and movement helpers. `watch_collision A B hit` keeps `hit` updated every scheduled tick, `collides A B hit` checks immediately, and `move_ui player x +1 y 0` moves a named item:

```text
add (ui)

block x 0 y 40 w 10 h 10 color #44AAFF name player
block x 30 y 40 w 10 h 10 color #FF4444 name wall

add (collision)

watch_collision player wall hit
every 100ms (
    move_ui player x +1 y 0
    if hit = true (
        move_ui player x -1 y 0
    )
)
```

UI buttons run their code when clicked:

```text
add (ui)

button greet "Say hello" x 10 y 10 w 30 h 12 color #7043BE opacity 0.8 (
    say "Hello from a button"
)
```

Buttons accept an optional `color COLOR` (`#RRGGBB` or `0xRRGGBB`) and `opacity VALUE` from `0` (transparent) to `1` (fully opaque). Both desktop and Android previews use these settings.

`add (controls)` declares `key_*` variables for letters, digits, function keys, modifiers, navigation, and numpad keys. On Windows, conditions refresh those values as `press` or `released`. It also declares Xbox/XInput button states such as `controller_a`, `controller_lt`, and `controller_dpad_up`, plus normalized stick axes such as `controller_left_stick_x`. Controller state is read from the first connected XInput controller. Mobile variables (`mobile_tap`, `mobile_hold`, swipes, and buttons) are declared as `unavailable` on Windows.

```text
add (controls)

toggle was_pressed = false
loop true (
    if key_x = press (
        if was_pressed = false (
            say "X was pressed"
            was_pressed = true
        )
    )
    else (
        was_pressed = false
    )
    if key_escape = press (
        break
    )
)
```


## Repository hygiene

Generated Windows executables, DLLs, object/resource files, Android Gradle caches, CMake intermediates, APKs, and machine-specific `local.properties` are ignored by Git. Build them locally with the scripts above or the Gradle wrapper; do not commit generated outputs.


## TRG test runner

Run the language regression suite from the repository root:

```sh
python3 tests/run-tests.py
```

The runner builds a separate console-mode test executable from `TRG.cpp`, runs each marked regression script, checks for its expected pass marker, and fails the process if a script reports an interpreter error. It is also run automatically by the **TRG language regression tests** GitHub Actions workflow on changes to the interpreter or tests. The test executable is kept in `.test-build/` and is not the desktop application.
