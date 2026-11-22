package com.trg.studio;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.content.Intent;
import android.content.ClipData;
import android.content.ClipboardManager;
import android.database.Cursor;
import android.graphics.Canvas;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.RectF;
import android.media.MediaPlayer;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.SystemClock;
import android.text.InputType;
import android.text.Editable;
import android.text.InputFilter;
import android.text.Spannable;
import android.text.Spanned;
import android.text.TextWatcher;
import android.text.style.ForegroundColorSpan;
import android.text.style.UnderlineSpan;
import android.view.Gravity;
import android.view.MotionEvent;
import android.view.View;
import android.view.WindowManager;
import android.view.inputmethod.InputMethodManager;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.PopupMenu;
import android.widget.HorizontalScrollView;
import android.widget.SeekBar;
import android.widget.ScrollView;
import android.widget.Spinner;
import android.widget.ArrayAdapter;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.MediaController;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.Switch;
import android.widget.VideoView;
import android.webkit.WebSettings;
import android.webkit.WebView;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.File;
import java.io.OutputStream;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashSet;
import java.util.HashMap;
import android.util.Base64;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Locale;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;
import java.util.zip.ZipOutputStream;

public final class MainActivity extends Activity {
    private static final int REQUEST_IMPORT = 1;
    private static final int REQUEST_EXPORT = 2;
    private static final int REQUEST_PACKAGE = 3;
    private static final int MENU_RUN = 1;
    private static final int MENU_CODE = 2;
    private static final int MENU_EXAMPLE = 3;
    private static final int MENU_IMPORT = 4;
    private static final int MENU_EXPORT = 5;
    private static final int MENU_PACKAGE = 6;
    private static final int MENU_PREVIEW = 7;
    private static final int MENU_DICTIONARY = 8;
    private static final int MENU_SETTINGS = 9;
    private static final int MENU_DEBUG = 10;
    private static final int MENU_2D = 11;
    private static final int MENU_3D = 12;
    private static final String[] MOBILE_SWIPES = {
            "mobile_swipe_left", "mobile_swipe_right", "mobile_swipe_up", "mobile_swipe_down"
    };
    static {
        System.loadLibrary("trg");
    }

    private static native void nativeInitialize(MainActivity activity);
    private static native String nativeRun(String source);
    private static native String nativeGetEditorSource(boolean threeD);
    private static native String nativeRunEvery(int index);
    private static native String nativeClickButton(int index);
    private static native void nativeSetMobileInput(String name, String state);
    private static native void nativeSetPointer(float x, float y, boolean down);
    private static native void nativeStop();
    private static native void nativeSetEditorSelection(String name);
    private static native String nativeTransformEditorObject(String kind, String name, float dx, float dy, float dz);
    private static native String nativeScaleEditorObject(String kind, String name, float factor);

    private final ExecutorService executor = Executors.newSingleThreadExecutor();
    private EditText editor;
    private TextView lineNumbers;
    private TextView output;
    private TextView status;
    private PreviewView preview;
    private final android.graphics.Typeface fixedUiFont = android.graphics.Typeface.create("monospace", android.graphics.Typeface.NORMAL);
    private final android.graphics.Typeface fixedUiFontBold = android.graphics.Typeface.create("monospace", android.graphics.Typeface.BOLD);
    private LinearLayout editorScreen;
    private LinearLayout outputScreen;
    private String sourceText = "";
    private String exampleSource;
    private String studioSource;
    private String studioMenuSource;
    private String twoDEditorSource;
    private String threeDEditorSource;
    private JSONObject lastRunState = new JSONObject();
    private boolean toolbarInitialized;
    private boolean showingOutput;
    private String lastRunError = "";
    private float editorFontSize = 14;
    private boolean formattingEditor;
    private final LinkedHashMap<String, String> openDocuments = new LinkedHashMap<>();
    private String currentDocumentName = "program.trg";
    private LinearLayout fileTabs;
    private MediaPlayer activeAudio;
    private Dialog programPreviewDialog;
    private PreviewView activeProgramPreview;
    private Dialog studioMenuDialog;
    private PreviewView studioMenuPreview;
    private boolean studioMenuOpening;
    private boolean menuReturnShowingOutput;
    private final Handler everyHandler = new Handler(Looper.getMainLooper());
    private final HashMap<Integer, Runnable> scheduledEveryTasks = new HashMap<>();
    private String everyScheduleSignature = "";

    private final class LineNumberEditText extends EditText {
        LineNumberEditText() { super(MainActivity.this); }
        @Override
        protected void onScrollChanged(int x, int y, int oldX, int oldY) {
            super.onScrollChanged(x, y, oldX, oldY);
            if (lineNumbers != null) lineNumbers.scrollTo(0, y);
        }
    }

    private final class TrgIndentFilter implements InputFilter {
        @Override
        public CharSequence filter(CharSequence source, int start, int end, Spanned dest, int dstart, int dend) {
            if (!"\n".contentEquals(source.subSequence(start, end))) return null;
            String before = dest.subSequence(0, dstart).toString();
            int lineStart = before.lastIndexOf('\n') + 1;
            String currentLine = before.substring(lineStart);
            int spaces = 0;
            while (spaces < currentLine.length() && currentLine.charAt(spaces) == ' ') spaces++;
            String trimmed = currentLine.trim();
            int indent = spaces;
            if (trimmed.endsWith("(")) indent += 4;
            if (dend < dest.length()) {
                char next = dest.charAt(dend);
                if (next == ')' && indent >= 4) indent -= 4;
            }
            return "\n" + repeatSpaces(indent);
        }
    }

    @Override
    protected void onCreate(Bundle state) {
        super.onCreate(state);
        enableImmersiveMode();
        getWindow().setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_ADJUST_RESIZE);
        editorFontSize = getPreferences(MODE_PRIVATE).getFloat("editor_font_size", 14f);
        nativeInitialize(this);
        try (InputStream input = getAssets().open("Studio.trg")) {
            studioSource = readText(input);
            try (InputStream example = getAssets().open("Example.trg")) {
                exampleSource = readText(example);
            }
            try (InputStream studioMenu = getAssets().open("StudioMenu.trg")) {
                studioMenuSource = readText(studioMenu);
            }
sourceText = exampleSource;
            openDocuments.put("program.trg", sourceText);
        } catch (Exception exception) {
            showError("Could not load the TRG Studio app", exception);
            return;
        }
        buildInterface();
        executor.execute(() -> {
            String result = nativeRun(studioSource);
            runOnUiThread(() -> applyState(result));
        });
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);
        if (hasFocus) {
            enableImmersiveMode();
        }
    }

    private void enableImmersiveMode() {
        getWindow().getDecorView().setSystemUiVisibility(
                View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
                        | View.SYSTEM_UI_FLAG_FULLSCREEN
                        | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                        | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                        | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                        | View.SYSTEM_UI_FLAG_LAYOUT_STABLE);
    }

    @Override
    protected void onDestroy() {
        cancelEverySchedules();
        nativeStop();
        if (activeAudio != null) { activeAudio.release(); activeAudio = null; }
        executor.shutdownNow();
        super.onDestroy();
    }

    private void buildInterface() {
        LinearLayout root = new LinearLayout(this);
        root.setOrientation(LinearLayout.VERTICAL);
        root.setBackgroundColor(Color.rgb(10, 16, 25));

        preview = new PreviewView(true);
        root.addView(preview, new LinearLayout.LayoutParams(-1, dp(72)));

        editorScreen = new LinearLayout(this);
        editorScreen.setOrientation(LinearLayout.VERTICAL);
        editorScreen.setBackgroundColor(Color.rgb(10, 16, 25));

        // OneCompiler-style compact project/action toolbar.
        LinearLayout editorToolbar = new LinearLayout(this);
        editorToolbar.setOrientation(LinearLayout.HORIZONTAL);
        editorToolbar.setGravity(Gravity.CENTER_VERTICAL);
        editorToolbar.setPadding(dp(8), dp(5), dp(8), dp(5));
        editorToolbar.setBackgroundColor(Color.rgb(18, 31, 45));
        addEditorToolbarAction(editorToolbar, "▶ RUN", Color.rgb(20, 119, 111), this::runSource, 1.25f);
        addEditorToolbarAction(editorToolbar, "＋ NEW", Color.rgb(37, 56, 75), this::createNewDocument, 1f);
        addEditorToolbarAction(editorToolbar, "OPEN", Color.rgb(37, 56, 75), this::openDocument, 1f);
        addEditorToolbarAction(editorToolbar, "SAVE", Color.rgb(37, 56, 75),
                () -> createDocument(currentDocumentName, REQUEST_EXPORT), 1f);
        addEditorToolbarAction(editorToolbar, "MENU", Color.rgb(37, 56, 75), this::showStudioMenu, 1f);
        editorScreen.addView(editorToolbar, new LinearLayout.LayoutParams(-1, dp(48)));

        HorizontalScrollView tabScroller = new HorizontalScrollView(this);
        tabScroller.setHorizontalScrollBarEnabled(false);
        tabScroller.setBackgroundColor(Color.rgb(16, 27, 40));
        fileTabs = new LinearLayout(this);
        fileTabs.setOrientation(LinearLayout.HORIZONTAL);
        tabScroller.addView(fileTabs);
        editorScreen.addView(tabScroller, new LinearLayout.LayoutParams(-1, dp(38)));

        LinearLayout codePane = new LinearLayout(this);
        codePane.setOrientation(LinearLayout.HORIZONTAL);
        codePane.setBackgroundColor(Color.rgb(11, 19, 29));
        lineNumbers = new TextView(this);
        lineNumbers.setText("1");
        lineNumbers.setTextColor(Color.rgb(112, 139, 160));
        lineNumbers.setTextSize(editorFontSize);
        lineNumbers.setTypeface(fixedUiFontBold);
        lineNumbers.setMaxLines(Integer.MAX_VALUE);
        lineNumbers.setIncludeFontPadding(false);
        lineNumbers.setGravity(Gravity.TOP | Gravity.RIGHT);
        lineNumbers.setPadding(dp(10), dp(12), dp(9), dp(10));
        lineNumbers.setBackgroundColor(Color.rgb(15, 25, 37));
        codePane.addView(lineNumbers, new LinearLayout.LayoutParams(dp(42), -1));

        editor = new LineNumberEditText();
        editor.setGravity(Gravity.TOP | Gravity.START);
        editor.setTextColor(Color.rgb(226, 237, 246));
        editor.setHintTextColor(Color.rgb(105, 126, 145));
        editor.setHint("Write TRG source here");
        editor.setTextSize(editorFontSize);
        editor.setTypeface(fixedUiFont);
        editor.setMaxLines(Integer.MAX_VALUE);
        editor.setIncludeFontPadding(false);
        editor.setInputType(InputType.TYPE_CLASS_TEXT | InputType.TYPE_TEXT_FLAG_MULTI_LINE
                | InputType.TYPE_TEXT_FLAG_NO_SUGGESTIONS | InputType.TYPE_TEXT_VARIATION_VISIBLE_PASSWORD);
        editor.setImeOptions(android.view.inputmethod.EditorInfo.IME_FLAG_NO_EXTRACT_UI
                | android.view.inputmethod.EditorInfo.IME_FLAG_NO_PERSONALIZED_LEARNING);
        editor.setPadding(dp(12), dp(12), dp(12), dp(10));
        editor.setBackgroundColor(Color.rgb(11, 19, 29));
        editor.setVerticalScrollBarEnabled(true);
        editor.setFilters(new InputFilter[]{new TrgIndentFilter()});
        codePane.addView(editor, new LinearLayout.LayoutParams(0, -1, 1));
        editorScreen.addView(codePane, new LinearLayout.LayoutParams(-1, 0, 1));
        root.addView(editorScreen, new LinearLayout.LayoutParams(-1, 0, 1));

        outputScreen = new LinearLayout(this);
        outputScreen.setOrientation(LinearLayout.VERTICAL);
        outputScreen.setBackgroundColor(Color.rgb(8, 14, 22));
        TextView outputHeader = new TextView(this);
        outputHeader.setText("  >   PROGRAM OUTPUT");
        outputHeader.setTypeface(fixedUiFontBold);
        outputHeader.setTextColor(Color.rgb(113, 222, 207));
        outputHeader.setTextSize(11);
        outputHeader.setGravity(Gravity.CENTER_VERTICAL);
        outputHeader.setBackgroundColor(Color.rgb(17, 29, 42));
        outputScreen.addView(outputHeader, new LinearLayout.LayoutParams(-1, dp(36)));

        ScrollView outputScroll = new ScrollView(this);
        outputScroll.setFillViewport(true);
        outputScroll.setBackgroundColor(Color.rgb(8, 14, 22));
        output = new TextView(this);
        output.setTextColor(Color.rgb(213, 226, 237));
        output.setTextSize(14);
        output.setTypeface(fixedUiFont);
        output.setPadding(dp(14), dp(12), dp(12), dp(12));
        output.setText("Run a TRG program to see its output here.\n");
        outputScroll.addView(output);
        outputScreen.addView(outputScroll, new LinearLayout.LayoutParams(-1, 0, 1));
        root.addView(outputScreen, new LinearLayout.LayoutParams(-1, 0, 1));
        outputScreen.setVisibility(View.GONE);

        status = new TextView(this);
        status.setTypeface(fixedUiFontBold);
        status.setTextColor(Color.rgb(151, 177, 194));
        status.setTextSize(11);
        status.setPadding(dp(12), dp(4), dp(10), dp(4));
        status.setBackgroundColor(Color.rgb(16, 27, 40));
        status.setText("Ready  |  TRG C++ interpreter");
        root.addView(status, new LinearLayout.LayoutParams(-1, dp(24)));

        editor.setText(sourceText);
        editor.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence value, int start, int count, int after) {}

            @Override
            public void onTextChanged(CharSequence value, int start, int before, int count) {
                sourceText = value.toString();
                openDocuments.put(currentDocumentName, sourceText);
                updateLineNumbers(value);
                if (value instanceof Editable) applyEditorHighlighting((Editable) value);
            }

            @Override
            public void afterTextChanged(Editable value) {}
        });
        setContentView(root);
        updateLineNumbers(editor.getText());
        applyEditorHighlighting(editor.getText());
        refreshFileTabs();
    }

    private void runSource() {
        cancelEverySchedules();
        if (editor != null) sourceText = editor.getText().toString();
        String source = sourceText;
        InputMethodManager keyboard = (InputMethodManager) getSystemService(INPUT_METHOD_SERVICE);
        if (keyboard != null && editor != null) {
            keyboard.hideSoftInputFromWindow(editor.getWindowToken(), 0);
        }
        showingOutput = true;
        editorScreen.setVisibility(View.GONE);
        outputScreen.setVisibility(View.VISIBLE);
        output.setText("Running...\n");
        status.setText("Running  |  " + currentDocumentName);
        executor.execute(() -> {
            String result = nativeRun(source);
            runOnUiThread(() -> applyState(result));
        });
    }

    public void onTrgStudioAction(String action) {
        runOnUiThread(() -> {
            String normalized = action == null ? "" : action.toLowerCase(Locale.ROOT);
            if (studioMenuDialog != null && !normalized.equals("menu")) {
                closeStudioMenu(() -> dispatchStudioAction(normalized));
            } else {
                dispatchStudioAction(normalized);
            }
        });
    }

    private void dispatchStudioAction(String action) {
        switch (action) {
            case "menu":
                showStudioMenu();
                break;
            case "run":
                runSource();
                break;
            case "code":
                showCodeScreen();
                break;
            case "example":
                currentDocumentName = "Example.trg";
                sourceText = exampleSource;
                openDocuments.put(currentDocumentName, sourceText);
                if (editor != null) {
                    editor.setText(sourceText);
                    editor.setSelection(0);
                    refreshFileTabs();
                }
                showCodeScreen();
                status.setText("Example loaded  |  Example.trg");
                break;
            case "open_2d":
                launchTrgTool("2D Editor", nativeGetEditorSource(false));
                break;
            case "open_3d":
                launchTrgTool("3D Editor", nativeGetEditorSource(true));
                break;
            case "import":
                openDocument();
                break;
            case "export":
                createDocument(currentDocumentName, REQUEST_EXPORT);
                break;
            case "package":
                createDocument("program.trgpack", REQUEST_PACKAGE);
                break;
            case "preview":
                if (programPreviewDialog == null) showProgramPreview();
                break;
            case "dictionary":
                showCommandDictionary();
                break;
            case "settings":
                showEditorSettings();
                break;
            case "debug":
                showErrorExplanation();
                break;
            case "closemenu":
                break;
            default:
                if (action.regionMatches(true, 0, "media:", 0, 6)) displayTrgMedia(action);
                else if (action.regionMatches(true, 0, "wasm:", 0, 5)) displayWebAssembly(action.substring(5));
                else if (action.regionMatches(true, 0, "webpage:", 0, 8)) displayWebPage(action.substring(8));
                else showError("Unknown TRG Studio action", new IllegalArgumentException(action));
                break;
        }
    }

    private void showStudioMenu() {
        showStudioMenu(preview);
    }

    private void launchTrgTool(String name, String source) {
        if (source == null || source.trim().isEmpty()) {
            showError("TRG editor unavailable",
                    new IllegalStateException(name + " could not be loaded."));
            return;
        }
        cancelEverySchedules();
        nativeStop();
        if (editor != null && !showingOutput) {
            sourceText = editor.getText().toString();
            openDocuments.put(currentDocumentName, sourceText);
        }
        currentDocumentName = name;
        sourceText = source;
        openDocuments.put(currentDocumentName, sourceText);
        if (editor != null) {
            editor.setText(sourceText);
            editor.setSelection(0);
            refreshFileTabs();
        }
        InputMethodManager keyboard =
                (InputMethodManager) getSystemService(INPUT_METHOD_SERVICE);
        if (keyboard != null && editor != null) {
            keyboard.hideSoftInputFromWindow(editor.getWindowToken(), 0);
        }
        showingOutput = true;
        editorScreen.setVisibility(View.GONE);
        outputScreen.setVisibility(View.VISIBLE);
        output.setText("Opening " + name + "...\\n");
        status.setText("Opening TRG editor  |  " + name);
        executor.execute(() -> {
            String result = nativeRun(source);
            runOnUiThread(() -> {
                applyState(result);
                showProgramPreview();
            });
        });
    }

    private void showStudioMenu(View anchor) {
        if (studioMenuDialog != null || studioMenuOpening) return;
        if (studioMenuSource == null || studioMenuSource.trim().isEmpty()) {
            showError("TRG menu is unavailable", new IllegalStateException("StudioMenu.trg was not loaded."));
            return;
        }
        if (editor != null && !showingOutput) {
            sourceText = editor.getText().toString();
            openDocuments.put(currentDocumentName, sourceText);
        }
        menuReturnShowingOutput = showingOutput;
        studioMenuOpening = true;
        cancelEverySchedules();
        nativeStop();
        status.setText("Opening TRG Studio menu...");
        executor.execute(() -> {
            String rawState = nativeRun(studioMenuSource);
            runOnUiThread(() -> {
                studioMenuOpening = false;
                try {
                    JSONObject menuState = new JSONObject(rawState);
                    PreviewView menuView = new PreviewView();
                    menuView.setState(menuState);
                    Dialog dialog = new Dialog(this);
                    studioMenuDialog = dialog;
                    studioMenuPreview = menuView;
                    FrameLayout surface = new FrameLayout(this);
                    surface.setBackgroundColor(Color.rgb(10, 16, 25));
                    surface.addView(menuView, new FrameLayout.LayoutParams(-1, -1));
                    dialog.setCancelable(true);
                    dialog.setOnCancelListener(ignored -> {
                        if (studioMenuDialog == dialog) closeStudioMenu(null);
                    });
                    dialog.setOnDismissListener(ignored -> {
                        if (studioMenuDialog == dialog) {
                            studioMenuDialog = null;
                            studioMenuPreview = null;
                        }
                    });
                    dialog.setContentView(surface);
                    dialog.show();
                    makeImmersive(dialog);
                } catch (JSONException exception) {
                    showError("Could not open TRG Studio menu", exception);
                }
            });
        });
    }

    private void closeStudioMenu(Runnable afterClose) {
        Dialog dialog = studioMenuDialog;
        studioMenuDialog = null;
        studioMenuPreview = null;
        if (dialog != null && dialog.isShowing()) dialog.dismiss();
        studioMenuOpening = false;
        final boolean restoreOutput = menuReturnShowingOutput;
        final String restoreSource = restoreOutput ? sourceText : studioSource;
        executor.execute(() -> {
            String restored = nativeRun(restoreSource);
            runOnUiThread(() -> {
                try {
                    JSONObject restoredState = new JSONObject(restored);
                    lastRunState = restoredState;
                    updateEverySchedules(restoredState);
                    if (restoreOutput) {
                        applyState(restored);
                    } else {
                        if (preview != null) {
                            preview.setState(restoredState);
                            toolbarInitialized = true;
                        }
                        status.setText("Editor ready  |  " + currentDocumentName);
                    }
                } catch (JSONException exception) {
                    status.setText("Menu closed; the previous view could not be restored.");
                    output.setText("Interpreter response error: " + exception.getMessage());
                }
                if (afterClose != null) afterClose.run();
            });
        });
    }

    private void showCommandDictionary() {
        String guide = "TRG COMMAND DICTIONARY\n\n"
                + "OUTPUT AND INPUT\n"
                + "say VALUE...  — print text and values\n"
                + "ask PROMPT VARIABLE  — ask for input\n\n"
                + "VARIABLES\n"
                + "word NAME [= VALUE]  — text\nnum NAME [= EXPRESSION]  — number\n"
                + "toggle NAME [= true|false]  — boolean\ncolor NAME [= COLOR]  — color value\n"
                + "set NAME VALUE  — update a typed variable in UI callbacks or timers\n"
                + "increase NUMBER [AMOUNT] | decrease NUMBER [AMOUNT]  — change numeric variables\n"
                + "list NAME [= VALUE, VALUE]  — list of text values\n"
                + "push LIST VALUE  — add to end\nget LIST INDEX VARIABLE  — read item (index starts at 0)\n"
                + "remove LIST INDEX  — remove item\n\n"
                + "BEGINNER DATA TYPES\n"
                + "record TYPE (\n    word name\n    num score = 0\n)  — describe a record\n"
                + "struct TYPE ( ... ) | class TYPE ( ... )  — aliases for record\n"
                + "new TYPE variable  — create a record; use variable.field to read or set fields\n"
                + "wait 10s | wait 100ms  — pause for a time\n\n"
                + "MATH AND TEXT\n"
                + "Numbers support + - * / % ^ and parentheses; sqrt(VALUE) calculates square root.\n"
                + "upper VALUE RESULT | lower VALUE RESULT | length VALUE RESULT\n"
                + "contains VALUE SEARCH RESULT | replace VALUE SEARCH WITH RESULT\n\n"
                + "GAMES AND TIMING\n"
                + "random MIN MAX RESULT  — choose an integer when bounds are integers; otherwise choose a decimal\n"
                + "every 1s ( ... ) | every 100ms ( ... )  — repeat a block on a timer\n"
                + "add (collision)  — enable game collision and movement helpers\n"
                + "collides A B RESULT  — check if two named UI items overlap\n"
                + "watch_collision A B RESULT  — keep a true/false collision value updated each tick\n"
                + "move_ui OBJECT x DX y DY  — move a block, button, or sprite by x/y amounts\n\n"
                + "FLOW\n"
                + "Blocks use ( ) only; curly braces are not supported.\n"
                + "if CONDITION ( ... ) | else ( ... ) | loop CONDITION ( ... )\n"
                + "every 1s ( ... ) | every 100ms ( ... )  — scheduled repeating code\n"
                + "group NAME [visible true|false] ( ... ) | show NAME | hide NAME\n"
                + "define NAME PARAMS ( ... )\n"
                + "break | continue | stop\n\n"
                + "LIBRARIES AND FILES\n"
                + "Libraries are file-wide; do not put a block after a library line.\n"
                + "add (files) | add (json) | add (web) | add (collision)\n"
                + "add (manual) | add (ui) | add (3d) | add (animation) | add (controls)\n"
                + "add (all)  — enable all built-in libraries (including json and 3d)\n"
                + "add (plugin.trg)  — load another TRG file from the project folder\n"
                + "add global ( ... )  — variables and libraries for every .trg in the project\n"
                + "add connect (file.trg)  — share variables with another project file (transitive)\n"
                + "run (file.trg)  — run another project file in the current scope\n"
                + "JSON\n"
                + "json GROUP clean RESULT | json GROUP compressed RESULT\n"
                + "create FILE (LOCATION) RESULT  — write JSON text held in RESULT\n"
                + "json read (LOCATION/FILE)  — load JSON into variables\n"
                + "read_file PATH VARIABLE | write_file PATH TEXT... | append_file PATH TEXT...\n"
                + "file_exists PATH VARIABLE | delete_file PATH\n\n"
                + "MEDIA AND WEB\n"
                + "play_audio FILE | show_image FILE | play_video FILE  — use files imported into the project folder\n"
                + "add (web)  — enable the TRG page builder and WebAssembly viewer\n"
                + "web_page TITLE | web_heading TEXT | web_text TEXT | web_color COLOR\n"
                + "web_button LABEL [MESSAGE] | web_image FILE | web_show\n"
                + "run_wasm FILE  — load a self-contained .wasm module in Android WebView\n\n"
                + "UI (after add (ui))\n"
                + "text NAME VALUE x X y Y size SIZE color COLOR [font sans|serif|mono] [align left|center|right]\n"
                + "button NAME LABEL x X y Y w WIDTH h HEIGHT [color COLOR] [opacity 0..1] [edges round|square] [font FONT] ( ... )\n"
                + "block z Z x X y Y h HEIGHT w WIDTH color COLOR edges round|square name NAME\n"
                + "sprite NAME FILE x X y Y w WIDTH h HEIGHT  — draw an imported image (image also works)\n"
                + "NAME x|y|w|h VALUE  — change a named block, button, or sprite; +VALUE moves it\n"
                + "Example: style y +10 moves the UI block named style down by 10 percent\n"
                + "add (controls) | add (animation)\n\n"
                + "2D OBJECT EDITING (after add (ui))\n"
                + "ui create block NAME x X y Y w W h H color COLOR [shape SHAPE] [z LAYER]\n"
                + "ui duplicate SOURCE NAME | ui rename OLD NEW | ui delete NAME\n"
                + "ui move NAME DX DY | ui resize NAME DW DH | ui color NAME COLOR | ui shape NAME SHAPE\n\n"
                + "3D SCENE OBJECTS (after add (3d))\n"
                + "3d box NAME x X y Y z Z w W h H d D color COLOR\n"
                + "3d sphere NAME x X y Y z Z size SIZE color COLOR\n"
                + "3d duplicate SOURCE NAME | 3d move NAME DX DY DZ | 3d rotate NAME RX RY RZ\n"
                + "3d scale NAME FACTOR | 3d color NAME COLOR | 3d rename OLD NEW | 3d delete NAME\n"
                + "The current 3D renderer is an isometric primitive renderer, not a full mesh/camera engine.\n\n"
                + "Commands ignore letter case. Colors accept #RRGGBB or 0xRRGGBB.";
        TextView text = new TextView(this);
        text.setText(guide);
        text.setTextSize(14);
        text.setTypeface(android.graphics.Typeface.create("monospace", android.graphics.Typeface.BOLD));
        text.setTextColor(Color.WHITE);
        text.setPadding(dp(16), dp(12), dp(16), dp(12));
        android.graphics.drawable.GradientDrawable dictionaryPanel = new android.graphics.drawable.GradientDrawable();
        dictionaryPanel.setColor(Color.rgb(7, 7, 12));
        dictionaryPanel.setCornerRadius(dp(12));
        dictionaryPanel.setStroke(dp(2), Color.rgb(139, 92, 246));
        text.setBackground(dictionaryPanel);
        ScrollView scroll = new ScrollView(this);
        scroll.setBackgroundColor(Color.rgb(7, 7, 12));
        scroll.addView(text);
        AlertDialog dictionaryDialog = new AlertDialog.Builder(this).setTitle("TRG language guide")
                .setView(scroll).setPositiveButton("Close", null).create();
        dictionaryDialog.setOnShowListener(ignored -> {
            if (dictionaryDialog.getWindow() != null)
                dictionaryDialog.getWindow().setBackgroundDrawable(new android.graphics.drawable.ColorDrawable(Color.rgb(7, 7, 12)));
            dictionaryDialog.getButton(AlertDialog.BUTTON_POSITIVE).setTextColor(Color.rgb(190, 160, 255));
        });
        dictionaryDialog.show();
    }

    private void showEditorSettings() {
        String[] options = {"Change editor font size", "Color wheel and format converter"};
        new AlertDialog.Builder(this).setTitle("Editor settings")
                .setItems(options, (dialog, which) -> {
                    if (which == 0) showFontSizePicker();
                    else showColorWheel();
                }).show();
    }

    private void showErrorExplanation() {
        if (lastRunError.isEmpty()) {
            new AlertDialog.Builder(this).setTitle("TRG debug helper")
                    .setMessage("Run a program with an error first. TRG also suggests a command when it spots a likely typo.")
                    .setPositiveButton("Close", null).show();
            return;
        }
        String explanation = "Check the command name and its arguments in the TRG command dictionary.";
        String error = lastRunError.toLowerCase(Locale.ROOT);
        if (error.contains("did you mean")) explanation = "TRG found a command name that is close to a built-in command. Try the suggested spelling shown in the error.";
        else if (error.contains("invalid number") || error.contains("must be a number")) explanation = "A numeric value needs a valid number or expression. TRG supports +, -, *, /, %, ^, parentheses, and sqrt(...).";
        else if (error.contains("missing block") || error.contains("needs a code block")) explanation = "This command starts a block. Add a matching pair of ( ) around the lines that belong inside it.";
        else if (error.contains("opacity")) explanation = "Button opacity must be between 0 (transparent) and 1 (fully opaque).";
        else if (error.contains("file") || error.contains("project folder")) explanation = "TRG file and media commands use files inside the project folder. On Android, import the asset first; file commands also need add (files).";
        else if (error.contains("list index")) explanation = "List indexes start at 0. For a list with three items, the valid indexes are 0, 1, and 2.";
        new AlertDialog.Builder(this).setTitle("TRG debug helper")
                .setMessage(lastRunError + "\n\n" + explanation)
                .setPositiveButton("Open command dictionary", (dialog, which) -> showCommandDictionary())
                .setNegativeButton("Close", null).show();
    }

    private void showFontSizePicker() {
        LinearLayout content = new LinearLayout(this);
        content.setOrientation(LinearLayout.VERTICAL);
        content.setPadding(dp(20), dp(12), dp(20), dp(4));
        TextView value = new TextView(this);
        value.setText("Editor font: " + Math.round(editorFontSize) + " sp");
        SeekBar size = new SeekBar(this);
        size.setMax(16);
        size.setProgress(Math.max(0, Math.min(16, Math.round(editorFontSize) - 8)));
        size.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override public void onProgressChanged(SeekBar bar, int progress, boolean fromUser) {
                value.setText("Editor font: " + (progress + 8) + " sp");
            }
            @Override public void onStartTrackingTouch(SeekBar bar) {}
            @Override public void onStopTrackingTouch(SeekBar bar) {}
        });
        content.addView(value);
        content.addView(size);
        new AlertDialog.Builder(this).setTitle("Editor font size").setView(content)
                .setNegativeButton("Cancel", null)
                .setPositiveButton("Apply", (dialog, which) -> {
                    editorFontSize = size.getProgress() + 8;
                    getPreferences(MODE_PRIVATE).edit().putFloat("editor_font_size", editorFontSize).apply();
                    editor.setTextSize(editorFontSize);
                    lineNumbers.setTextSize(editorFontSize);
                }).show();
    }

    private void showColorWheel() {
        LinearLayout content = new LinearLayout(this);
        content.setOrientation(LinearLayout.VERTICAL);
        content.setPadding(dp(20), dp(12), dp(20), dp(8));
        TextView swatch = new TextView(this);
        swatch.setText("Selected color   #FF0000");
        swatch.setTextColor(Color.WHITE);
        swatch.setGravity(Gravity.CENTER);
        swatch.setPadding(0, dp(12), 0, dp(12));
        SeekBar hue = new SeekBar(this); hue.setMax(360);
        SeekBar saturation = new SeekBar(this); saturation.setMax(100); saturation.setProgress(100);
        SeekBar brightness = new SeekBar(this); brightness.setMax(100); brightness.setProgress(100);
        TextView hueLabel = new TextView(this); hueLabel.setText("Hue");
        TextView saturationLabel = new TextView(this); saturationLabel.setText("Saturation");
        TextView brightnessLabel = new TextView(this); brightnessLabel.setText("Brightness");
        Spinner format = new Spinner(this);
        String[] formats = {"#RRGGBB", "#GGBBRR", "#BBGGRR", "##BBRRGG", "#GGRRBB", "#RRBBGG"};
        format.setAdapter(new ArrayAdapter<>(this, android.R.layout.simple_spinner_dropdown_item, formats));
        Switch prefix = new Switch(this); prefix.setText("Use 0x prefix instead of #");
        content.addView(swatch);
        content.addView(hueLabel); content.addView(hue);
        content.addView(saturationLabel); content.addView(saturation);
        content.addView(brightnessLabel); content.addView(brightness);
        content.addView(format); content.addView(prefix);
        int[] selected = {Color.RED};
        SeekBar.OnSeekBarChangeListener listener = new SeekBar.OnSeekBarChangeListener() {
            @Override public void onProgressChanged(SeekBar bar, int progress, boolean fromUser) {
                float[] hsv = {hue.getProgress(), saturation.getProgress() / 100f, brightness.getProgress() / 100f};
                selected[0] = Color.HSVToColor(hsv);
                swatch.setBackgroundColor(selected[0]);
                swatch.setText("Selected color   " + formattedColor(selected[0], formats[format.getSelectedItemPosition()], prefix.isChecked()));
            }
            @Override public void onStartTrackingTouch(SeekBar bar) {}
            @Override public void onStopTrackingTouch(SeekBar bar) {}
        };
        hue.setOnSeekBarChangeListener(listener);
        saturation.setOnSeekBarChangeListener(listener);
        brightness.setOnSeekBarChangeListener(listener);
        format.setOnItemSelectedListener(new android.widget.AdapterView.OnItemSelectedListener() {
            @Override public void onItemSelected(android.widget.AdapterView<?> parent, View view, int position, long id) { listener.onProgressChanged(hue, hue.getProgress(), false); }
            @Override public void onNothingSelected(android.widget.AdapterView<?> parent) {}
        });
        prefix.setOnCheckedChangeListener((button, checked) -> listener.onProgressChanged(hue, hue.getProgress(), false));
        listener.onProgressChanged(hue, 0, false);
        new AlertDialog.Builder(this).setTitle("Color wheel")
                .setView(content).setNegativeButton("Close", null)
                .setPositiveButton("Copy", (dialog, which) -> {
                    String value = formattedColor(selected[0], formats[format.getSelectedItemPosition()], prefix.isChecked());
                    ClipboardManager clipboard = (ClipboardManager) getSystemService(CLIPBOARD_SERVICE);
                    if (clipboard != null) clipboard.setPrimaryClip(ClipData.newPlainText("TRG color", value));
                    status.setText("Copied " + value);
                }).show();
    }

    private String formattedColor(int color, String format, boolean use0x) {
        String red = String.format(Locale.ROOT, "%02X", Color.red(color));
        String green = String.format(Locale.ROOT, "%02X", Color.green(color));
        String blue = String.format(Locale.ROOT, "%02X", Color.blue(color));
        String body;
        if (format.contains("RRGGBB")) body = red + green + blue;
        else if (format.contains("GGBBRR")) body = green + blue + red;
        else if (format.contains("BBGGRR")) body = blue + green + red;
        else if (format.contains("BBRRGG")) body = blue + red + green;
        else if (format.contains("GGRRBB")) body = green + red + blue;
        else body = red + blue + green;
        if (use0x) return "0x" + body;
        return (format.startsWith("##") ? "##" : "#") + body;
    }

    private void showCodeScreen() {
        cancelEverySchedules();
        nativeStop();
        showingOutput = false;
        outputScreen.setVisibility(View.GONE);
        editorScreen.setVisibility(View.VISIBLE);
        status.setText("Editor ready  |  " + currentDocumentName);
    }

    private void addEditorToolbarAction(LinearLayout parent, String label, int background,
                                        Runnable action, float weight) {
        TextView button = new TextView(this);
        button.setText(label);
        button.setTextColor(Color.rgb(238, 246, 250));
        button.setTextSize(11);
        button.setTypeface(fixedUiFontBold);
        button.setGravity(Gravity.CENTER);
        button.setPadding(dp(10), 0, dp(10), 0);
        button.setBackgroundColor(background);
        button.setOnClickListener(view -> action.run());
        LinearLayout.LayoutParams params = new LinearLayout.LayoutParams(0, dp(36), weight);
        params.setMargins(dp(3), 0, dp(3), 0);
        parent.addView(button, params);
    }

    private void createNewDocument() {
        final EditText nameInput = new EditText(this);
        nameInput.setSingleLine(true);
        nameInput.setText("program" + (openDocuments.size() + 1) + ".trg");
        nameInput.setHint("filename.trg");
        new AlertDialog.Builder(this)
                .setTitle("New TRG file")
                .setView(nameInput)
                .setNegativeButton("CANCEL", null)
                .setPositiveButton("CREATE", (dialog, which) -> {
                    String name = nameInput.getText().toString().trim();
                    if (name.isEmpty()) name = "program.trg";
                    if (!name.toLowerCase(Locale.ROOT).endsWith(".trg")) name += ".trg";
                    openDocuments.put(currentDocumentName, editor.getText().toString());
                    currentDocumentName = name;
                    sourceText = "";
                    openDocuments.put(name, sourceText);
                    editor.setText(sourceText);
                    editor.setSelection(0);
                    refreshFileTabs();
                    showCodeScreen();
                    status.setText("New file  |  " + currentDocumentName);
                })
                .show();
    }

    private void refreshFileTabs() {
        if (fileTabs == null) return;
        fileTabs.removeAllViews();
        for (String name : openDocuments.keySet()) {
            TextView tab = new TextView(this);
            tab.setTypeface(fixedUiFontBold);
            tab.setText("  " + name + "  ");
            tab.setTextSize(12);
            tab.setGravity(Gravity.CENTER_VERTICAL);
            tab.setPadding(dp(12), 0, dp(12), 0);
            tab.setTextColor(name.equals(currentDocumentName) ? Color.rgb(224, 250, 246) : Color.rgb(139, 160, 178));
            tab.setBackgroundColor(name.equals(currentDocumentName) ? Color.rgb(24, 75, 78) : Color.TRANSPARENT);
            tab.setOnClickListener(view -> {
                openDocuments.put(currentDocumentName, editor.getText().toString());
                currentDocumentName = name;
                sourceText = openDocuments.get(name);
                editor.setText(sourceText);
                editor.setSelection(0);
                refreshFileTabs();
            });
            fileTabs.addView(tab, new LinearLayout.LayoutParams(-2, -1));
        }
    }

    private void cancelEverySchedules() {
        for (Runnable task : scheduledEveryTasks.values()) everyHandler.removeCallbacks(task);
        scheduledEveryTasks.clear();
        everyScheduleSignature = "";
    }

    private void updateEverySchedules(JSONObject state) {
        JSONArray tasks = state.optJSONArray("every");
        String signature = tasks == null ? "[]" : tasks.toString();
        if (signature.equals(everyScheduleSignature)) return;
        cancelEverySchedules();
        everyScheduleSignature = signature;
        if (!showingOutput || tasks == null) return;
        for (int i = 0; i < tasks.length(); i++) {
            JSONObject task = tasks.optJSONObject(i);
            if (task == null) continue;
            int index = task.optInt("index", i);
            long interval = Math.max(1, task.optLong("ms", 1000));
            Runnable ticker = new Runnable() {
                @Override public void run() {
                    if (!showingOutput || !signature.equals(everyScheduleSignature)) return;
                    executor.execute(() -> {
                        String result = nativeRunEvery(index);
                        runOnUiThread(() -> {
                            applyState(result);
                            if (showingOutput && signature.equals(everyScheduleSignature)
                                    && scheduledEveryTasks.get(index) == this) {
                                everyHandler.postDelayed(this, interval);
                            }
                        });
                    });
                }
            };
            scheduledEveryTasks.put(index, ticker);
            everyHandler.postDelayed(ticker, interval);
        }
    }

    private void showProgramPreview() {
        boolean editor2D = "2DEditor.trg".equalsIgnoreCase(currentDocumentName);
        boolean editor3D = "3DEditor.trg".equalsIgnoreCase(currentDocumentName);
        PreviewView programPreview = new PreviewView(false, editor2D, editor3D);
        JSONObject previewSnapshot;
        try { previewSnapshot = new JSONObject(lastRunState.toString()); }
        catch (JSONException exception) { previewSnapshot = new JSONObject(); }
        programPreview.setState(previewSnapshot);
        Dialog dialog = new Dialog(this);
        programPreviewDialog = dialog;
        activeProgramPreview = programPreview;
        dialog.setOnDismissListener(ignored -> {
            if (programPreviewDialog == dialog) programPreviewDialog = null;
            if (activeProgramPreview == programPreview) activeProgramPreview = null;
        });
        FrameLayout fullScreen = new FrameLayout(this);
        fullScreen.addView(programPreview, new FrameLayout.LayoutParams(-1, -1));
        TextView menuButton = new TextView(this);
        menuButton.setTypeface(fixedUiFontBold);
        menuButton.setText("MENU");
        menuButton.setTextColor(Color.WHITE);
        menuButton.setTextSize(12);
        menuButton.setGravity(Gravity.CENTER);
        menuButton.setPadding(dp(14), dp(8), dp(14), dp(8));
        menuButton.setBackgroundColor(Color.rgb(20, 105, 99));
        FrameLayout.LayoutParams menuLayout = new FrameLayout.LayoutParams(-2, -2, Gravity.TOP | Gravity.END);
        menuLayout.setMargins(0, dp(16), dp(16), 0);
        fullScreen.addView(menuButton, menuLayout);
        menuButton.setOnClickListener(view -> showStudioMenu(menuButton));
        TextView closeButton = new TextView(this);
        closeButton.setTypeface(fixedUiFontBold);
        closeButton.setText("×");
        closeButton.setTextColor(Color.WHITE);
        closeButton.setTextSize(25);
        closeButton.setGravity(Gravity.CENTER);
        closeButton.setBackgroundColor(Color.rgb(22, 43, 58));
        FrameLayout.LayoutParams closeLayout = new FrameLayout.LayoutParams(dp(44), dp(44), Gravity.TOP | Gravity.START);
        closeLayout.setMargins(dp(14), dp(14), 0, 0);
        fullScreen.addView(closeButton, closeLayout);
        closeButton.setOnClickListener(view -> { dialog.dismiss(); showCodeScreen(); });
        dialog.setContentView(fullScreen);
        dialog.show();
        makeImmersive(dialog);
    }

    private void displayTrgMedia(String action) {
        String[] parts = action.split(":", 3);
        if (parts.length != 3) { showError("Media command failed", new IllegalArgumentException("Expected media type and file path.")); return; }
        String type = parts[1].toLowerCase(Locale.ROOT);
        File file = new File(parts[2]);
        if (!file.isFile()) { showError("Media file not found", new IllegalArgumentException(file.getPath())); return; }
        Uri uri = Uri.fromFile(file);
        if (type.equals("show_image")) {
            ImageView image = new ImageView(this);
            image.setImageURI(uri);
            image.setAdjustViewBounds(true);
            image.setScaleType(ImageView.ScaleType.FIT_CENTER);
            showFullScreenContent(image, () -> {});
        } else if (type.equals("play_video")) {
            VideoView video = new VideoView(this);
            video.setVideoURI(uri);
            MediaController controls = new MediaController(this);
            controls.setAnchorView(video);
            video.setMediaController(controls);
            video.setOnPreparedListener(player -> video.start());
            showFullScreenContent(video, video::stopPlayback);
        } else if (type.equals("play_audio")) {
            if (activeAudio != null) { activeAudio.release(); activeAudio = null; }
            activeAudio = MediaPlayer.create(this, uri);
            if (activeAudio == null) { showError("Audio could not be opened", new IllegalArgumentException(file.getName())); return; }
            MediaPlayer player = activeAudio;
            AlertDialog dialog = new AlertDialog.Builder(this).setTitle(file.getName())
                    .setMessage("Playing audio from the TRG project folder.")
                    .setPositiveButton("Close", null).create();
            dialog.setOnDismissListener(ignored -> {
                if (activeAudio == player) { player.release(); activeAudio = null; }
            });
            dialog.show();
            player.start();
        }
    }

    private void displayWebAssembly(String path) {
        File module = new File(path);
        if (!module.isFile() || !module.getName().toLowerCase(Locale.ROOT).endsWith(".wasm")) {
            showError("WebAssembly module not found", new IllegalArgumentException(path));
            return;
        }
        try {
            byte[] bytes = java.nio.file.Files.readAllBytes(module.toPath());
            String encoded = Base64.encodeToString(bytes, Base64.NO_WRAP);
            String html = "<!doctype html><meta name='viewport' content='width=device-width,initial-scale=1'>"
                    + "<body style='margin:0;background:#111;color:#eee;font:16px sans-serif;padding:20px'>"
                    + "<h2>TRG WebAssembly</h2><p>" + module.getName() + "</p><pre id='status'>Loading module…</pre>"
                    + "<script>try{const b=Uint8Array.from(atob('" + encoded + "'),c=>c.charCodeAt(0));"
                    + "WebAssembly.instantiate(b,{env:{}}).then(r=>{window.trgWasm=r.instance;"
                    + "document.getElementById('status').textContent='Module loaded and its start function ran.';})"
                    + ".catch(e=>document.getElementById('status').textContent='Module error: '+e); }"
                    + "catch(e){document.getElementById('status').textContent='Module error: '+e}</script></body>";
            WebView web = new WebView(this);
            WebSettings settings = web.getSettings();
            settings.setJavaScriptEnabled(true);
            settings.setAllowFileAccess(false);
            web.loadDataWithBaseURL("https://localhost/", html, "text/html", "UTF-8", null);
            showFullScreenWebContent(web);
        } catch (Exception exception) {
            showError("Could not load WebAssembly module", exception);
        }
    }

    private void displayWebPage(String html) {
        WebView web = new WebView(this);
        web.getSettings().setJavaScriptEnabled(true);
        web.getSettings().setAllowFileAccess(false);
        web.loadDataWithBaseURL("https://localhost/", html, "text/html", "UTF-8", null);
        showFullScreenWebContent(web);
    }

    private void showFullScreenWebContent(WebView web) {
        showFullScreenContent(web, web::destroy);
    }

    private void showFullScreenContent(View mainContent, Runnable onDismiss) {
        FrameLayout content = new FrameLayout(this);
        content.addView(mainContent, new FrameLayout.LayoutParams(-1, -1));
        TextView menuButton = new TextView(this);
        menuButton.setText("MENU");
        menuButton.setTextColor(Color.WHITE);
        menuButton.setTextSize(12);
        menuButton.setGravity(Gravity.CENTER);
        menuButton.setPadding(dp(14), dp(8), dp(14), dp(8));
        menuButton.setBackgroundColor(Color.rgb(91, 52, 170));
        FrameLayout.LayoutParams params = new FrameLayout.LayoutParams(-2, -2, Gravity.TOP | Gravity.END);
        params.setMargins(0, dp(16), dp(16), 0);
        content.addView(menuButton, params);
        Dialog dialog = new Dialog(this);
        programPreviewDialog = dialog;
        dialog.setOnDismissListener(ignored -> {
            onDismiss.run();
            if (programPreviewDialog == dialog) programPreviewDialog = null;
        });
        menuButton.setOnClickListener(view -> showStudioMenu(menuButton));
        dialog.setContentView(content);
        dialog.show();
        makeImmersive(dialog);
    }

    private void makeImmersive(Dialog dialog) {
        if (dialog.getWindow() == null) return;
        android.view.Window window = dialog.getWindow();
        window.setLayout(-1, -1);
        window.setBackgroundDrawable(new android.graphics.drawable.ColorDrawable(
                Color.rgb(10, 16, 25)));
        window.addFlags(WindowManager.LayoutParams.FLAG_DRAWS_SYSTEM_BAR_BACKGROUNDS);
        window.setStatusBarColor(Color.TRANSPARENT);
        window.setNavigationBarColor(Color.TRANSPARENT);
        if (android.os.Build.VERSION.SDK_INT >= 30) {
            android.view.WindowInsetsController controller = window.getInsetsController();
            if (controller != null) {
                controller.hide(android.view.WindowInsets.Type.statusBars()
                        | android.view.WindowInsets.Type.navigationBars());
                controller.setSystemBarsBehavior(
                        android.view.WindowInsetsController.BEHAVIOR_SHOW_TRANSIENT_BARS_BY_SWIPE);
            }
        } else {
            window.getDecorView().setSystemUiVisibility(
                    View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
                            | View.SYSTEM_UI_FLAG_FULLSCREEN
                            | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                            | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                            | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                            | View.SYSTEM_UI_FLAG_LAYOUT_STABLE);
        }
    }

    private void updateLineNumbers(CharSequence source) {
        if (lineNumbers == null) return;
        int lines = 1;
        for (int i = 0; i < source.length(); i++) {
            if (source.charAt(i) == '\n') lines++;
        }
        StringBuilder numbers = new StringBuilder(lines * 3);
        for (int i = 1; i <= lines; i++) {
            if (i > 1) numbers.append('\n');
            numbers.append(i);
        }
        lineNumbers.setText(numbers);
    }

    private void applyEditorHighlighting(Editable source) {
        if (source == null || formattingEditor) return;
        formattingEditor = true;
        try {
            for (ForegroundColorSpan span : source.getSpans(0, source.length(), ForegroundColorSpan.class)) {
                source.removeSpan(span);
            }
            for (UnderlineSpan span : source.getSpans(0, source.length(), UnderlineSpan.class)) {
                source.removeSpan(span);
            }
            String text = source.toString();
            String[] commands = {"say", "ask", "word", "num", "toggle", "color", "if", "else", "loop", "group",
                    "wait", "every", "random", "collides", "touching", "watch_collision", "move_ui",
                    "record", "class", "struct", "new", "image", "sprite", "json", "create", "run",
                    "define", "stop", "exit", "break", "continue", "add", "text", "button", "block", "animate",
                    "ui", "3d", "studio", "set_text", "set_color",
                    "register", "registry", "move", "subtract", "multiply", "divide", "list", "push", "get", "remove",
                    "upper", "lower", "length", "contains", "replace", "read_file", "write_file", "append_file", "delete_file", "file_exists",
                    "play_audio", "show_image", "play_video", "run_wasm", "web_page", "web_heading", "web_text",
                    "web_color", "web_button", "web_image", "web_show"};
            HashSet<String> knownCommands = new HashSet<>(Arrays.asList(commands));
            for (String sourceLine : text.split("\\n")) {
                String[] declaration = sourceLine.trim().split("\\s+");
                if (declaration.length >= 2 && declaration[0].equalsIgnoreCase("define")) knownCommands.add(declaration[1].toLowerCase(Locale.ROOT));
                if (declaration.length >= 2 && (declaration[0].equalsIgnoreCase("block")
                        || declaration[0].equalsIgnoreCase("sprite") || declaration[0].equalsIgnoreCase("image")
                        || declaration[0].equalsIgnoreCase("button") || declaration[0].equalsIgnoreCase("new"))) {
                    if (declaration[0].equalsIgnoreCase("block")) {
                        for (int i = 1; i + 1 < declaration.length; i++) {
                            if (declaration[i].equalsIgnoreCase("name")) knownCommands.add(declaration[i + 1].toLowerCase(Locale.ROOT));
                        }
                    } else if (declaration[0].equalsIgnoreCase("new")) knownCommands.add(declaration[2 < declaration.length ? 2 : 1].toLowerCase(Locale.ROOT));
                    else knownCommands.add(declaration[1].toLowerCase(Locale.ROOT));
                }
            }
            int lineStart = 0;
            while (lineStart < text.length()) {
                int lineEnd = text.indexOf('\n', lineStart);
                if (lineEnd < 0) lineEnd = text.length();
                String line = text.substring(lineStart, lineEnd);
                String trimmed = line.trim();
            int leading = 0;
            while (leading < line.length() && Character.isWhitespace(line.charAt(leading))) leading++;
                int comment = line.indexOf("//");
                if (comment >= 0) source.setSpan(new ForegroundColorSpan(Color.rgb(123, 148, 126)),
                        lineStart + comment, lineEnd, Spannable.SPAN_EXCLUSIVE_EXCLUSIVE);
                int quote = -1;
                boolean escaped = false;
                for (int i = 0; i < line.length(); i++) {
                    char c = line.charAt(i);
                    if (quote >= 0) {
                        if (!escaped && c == '"') {
                            source.setSpan(new ForegroundColorSpan(Color.rgb(214, 173, 121)),
                                    lineStart + quote, lineStart + i + 1, Spannable.SPAN_EXCLUSIVE_EXCLUSIVE);
                            quote = -1;
                        }
                        escaped = !escaped && c == '\\';
                    } else if (c == '"') { quote = i; escaped = false; }
                }
                if (quote >= 0) source.setSpan(new ForegroundColorSpan(Color.rgb(214, 173, 121)),
                        lineStart + quote, lineEnd, Spannable.SPAN_EXCLUSIVE_EXCLUSIVE);
                int wordEnd = leading;
                while (wordEnd < line.length() && (Character.isLetterOrDigit(line.charAt(wordEnd)) || line.charAt(wordEnd) == '_')) wordEnd++;
                if (wordEnd > leading && !trimmed.isEmpty() && !trimmed.startsWith("//") &&
                        trimmed.indexOf('=') < 0 && !knownCommands.contains(line.substring(leading, wordEnd).toLowerCase(Locale.ROOT))) {
                    source.setSpan(new ForegroundColorSpan(Color.rgb(255, 105, 115)), lineStart + leading, lineStart + wordEnd,
                            Spannable.SPAN_EXCLUSIVE_EXCLUSIVE);
                    source.setSpan(new UnderlineSpan(), lineStart + leading, lineStart + wordEnd,
                            Spannable.SPAN_EXCLUSIVE_EXCLUSIVE);
                } else if (wordEnd > leading && knownCommands.contains(line.substring(leading, wordEnd).toLowerCase(Locale.ROOT))) {
                    source.setSpan(new ForegroundColorSpan(Color.rgb(190, 155, 255)), lineStart + leading, lineStart + wordEnd,
                            Spannable.SPAN_EXCLUSIVE_EXCLUSIVE);
                }
                lineStart = lineEnd + 1;
            }
            java.util.regex.Matcher numberMatcher = java.util.regex.Pattern.compile("(?<![A-Za-z_])\\d+(?:\\.\\d+)?").matcher(text);
            while (numberMatcher.find()) source.setSpan(new ForegroundColorSpan(Color.rgb(141, 190, 235)),
                    numberMatcher.start(), numberMatcher.end(), Spannable.SPAN_EXCLUSIVE_EXCLUSIVE);
        } finally {
            formattingEditor = false;
        }
    }

    private static String repeatSpaces(int count) {
        StringBuilder result = new StringBuilder(Math.max(0, count));
        for (int i = 0; i < count; i++) result.append(' ');
        return result.toString();
    }

    @Override
    public void onBackPressed() {
        if (studioMenuDialog != null) {
            closeStudioMenu(null);
            return;
        }
        if (showingOutput) {
            showCodeScreen();
            return;
        }
        super.onBackPressed();
    }

    private void runButton(int index) {
        nativeSetMobileInput("mobile_tap", "press");
        executor.execute(() -> {
            String result = nativeClickButton(index);
            nativeSetMobileInput("mobile_tap", "released");
            runOnUiThread(() -> applyState(result));
        });
    }

    private void applyState(String rawState) {
        try {
            JSONObject state = new JSONObject(rawState);
            lastRunState = state;
            updateEverySchedules(state);
            if (activeProgramPreview != null) activeProgramPreview.setState(state);
            output.setText(state.optString("output"));
            lastRunError = "";
            if (state.optInt("errors") > 0) {
                String[] lines = state.optString("output").split("\\R");
                lastRunError = "";
                for (String line : lines) if (line.startsWith("ERROR")) { lastRunError = line; break; }
                status.setText("Finished with " + state.optInt("errors") + " error(s).");
            } else if (!showingOutput) {
                status.setText("Ready  |  TRG C++ interpreter");
            } else if (state.optJSONArray("every") != null && state.optJSONArray("every").length() > 0) {
                status.setText("Running  |  repeating game/update code");
            } else {
                status.setText("Finished  |  use MENU to return to code");
            }
            if (!toolbarInitialized) {
                preview.setState(state);
                toolbarInitialized = true;
            }
        } catch (JSONException exception) {
            status.setText("Could not read the interpreter result.");
            output.setText("Interpreter response error: " + exception.getMessage());
        }
    }

    public String requestTrgInput(String prompt) {
        CountDownLatch completed = new CountDownLatch(1);
        String[] answer = new String[1];
        runOnUiThread(() -> presentInputDialog(prompt, answer, completed));
        try {
            if (!completed.await(10, TimeUnit.MINUTES)) return null;
        } catch (InterruptedException exception) {
            Thread.currentThread().interrupt();
            return null;
        }
        return answer[0];
    }

    private void presentInputDialog(String prompt, String[] answer, CountDownLatch completed) {
        EditText field = new EditText(this);
        field.setSingleLine(true);
        AlertDialog dialog = new AlertDialog.Builder(this)
                .setTitle("TRG input")
                .setMessage(prompt)
                .setView(field)
                .setPositiveButton("Submit", null)
                .setNegativeButton("Cancel", (popup, which) -> completed.countDown())
                .create();
        dialog.setOnCancelListener(popup -> completed.countDown());
        dialog.setOnShowListener(ignored -> {
            dialog.getButton(AlertDialog.BUTTON_POSITIVE).setOnClickListener(view -> {
                answer[0] = field.getText().toString();
                completed.countDown();
                dialog.dismiss();
            });
        });
        dialog.show();
        dialog.getWindow().setSoftInputMode(android.view.WindowManager.LayoutParams.SOFT_INPUT_STATE_ALWAYS_VISIBLE);
    }

    private void openDocument() {
        Intent intent = new Intent(Intent.ACTION_OPEN_DOCUMENT);
        intent.addCategory(Intent.CATEGORY_OPENABLE);
        intent.setType("*/*");
        startActivityForResult(intent, REQUEST_IMPORT);
    }

    private void createDocument(String name, int request) {
        Intent intent = new Intent(Intent.ACTION_CREATE_DOCUMENT);
        intent.addCategory(Intent.CATEGORY_OPENABLE);
        intent.setType(request == REQUEST_PACKAGE ? "application/zip" : "text/plain");
        intent.putExtra(Intent.EXTRA_TITLE, name);
        startActivityForResult(intent, request);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (resultCode != RESULT_OK || data == null || data.getData() == null) return;
        Uri uri = data.getData();
        if (requestCode == REQUEST_IMPORT) importDocument(uri);
        else if (requestCode == REQUEST_EXPORT) saveSource(uri);
        else if (requestCode == REQUEST_PACKAGE) savePackage(uri);
    }

    private void importDocument(Uri uri) {
        try (InputStream input = getContentResolver().openInputStream(uri)) {
            if (input == null) throw new IllegalStateException("The selected document could not be opened.");
            String displayName = getDocumentName(uri);
            if (isProjectAsset(displayName)) {
                File projectCopy = new File(getFilesDir(), new File(displayName).getName());
                try (OutputStream copy = new java.io.FileOutputStream(projectCopy)) {
                    byte[] buffer = new byte[8192];
                    int count;
                    while ((count = input.read(buffer)) != -1) copy.write(buffer, 0, count);
                }
                status.setText("Imported project asset: " + projectCopy.getName());
                return;
            }
            boolean packaged = displayName.toLowerCase(Locale.ROOT).endsWith(".trgpack");
            sourceText = packaged ? readPackagedSource(input) : readText(input);
            currentDocumentName = packaged ? "program.trg" : displayName;
            openDocuments.put(currentDocumentName, sourceText);
            File projectCopy = new File(getFilesDir(), new File(currentDocumentName).getName());
            try (OutputStream copy = new java.io.FileOutputStream(projectCopy)) {
                copy.write(sourceText.getBytes(StandardCharsets.UTF_8));
            }
            editor.setText(sourceText);
            editor.setSelection(0);
            refreshFileTabs();
            showCodeScreen();
            status.setText("Source imported.");
        } catch (Exception exception) {
            showError("Import failed", exception);
        }
    }

    private String getDocumentName(Uri uri) {
        try (Cursor cursor = getContentResolver().query(uri, null, null, null, null)) {
            if (cursor != null && cursor.moveToFirst()) {
                int index = cursor.getColumnIndex(android.provider.OpenableColumns.DISPLAY_NAME);
                if (index >= 0) {
                    String name = cursor.getString(index);
                    if (name != null && !name.trim().isEmpty()) return name;
                }
            }
        } catch (Exception ignored) {}
        String segment = uri.getLastPathSegment();
        return segment == null || segment.isEmpty() ? "program.trg" : segment;
    }

    private boolean isProjectAsset(String name) {
        String lowerName = name.toLowerCase(Locale.ROOT);
        return lowerName.endsWith(".wasm") || lowerName.endsWith(".png") || lowerName.endsWith(".jpg")
                || lowerName.endsWith(".jpeg") || lowerName.endsWith(".gif") || lowerName.endsWith(".webp")
                || lowerName.endsWith(".mp3") || lowerName.endsWith(".wav") || lowerName.endsWith(".ogg")
                || lowerName.endsWith(".m4a") || lowerName.endsWith(".mp4") || lowerName.endsWith(".webm")
                || lowerName.endsWith(".3gp");
    }

    private String readPackagedSource(InputStream input) throws Exception {
        try (ZipInputStream zip = new ZipInputStream(input)) {
            ZipEntry entry;
            while ((entry = zip.getNextEntry()) != null) {
                if ("program.trg".equals(entry.getName())) return readText(zip);
            }
        }
        throw new IllegalArgumentException("The package does not contain program.trg.");
    }

    private String readText(InputStream input) throws Exception {
        BufferedReader reader = new BufferedReader(new InputStreamReader(input, StandardCharsets.UTF_8));
        StringBuilder text = new StringBuilder();
        char[] buffer = new char[4096];
        int count;
        while ((count = reader.read(buffer)) != -1) text.append(buffer, 0, count);
        return text.toString();
    }

    private void saveSource(Uri uri) {
        try (OutputStream outputStream = getContentResolver().openOutputStream(uri)) {
            if (outputStream == null) throw new IllegalStateException("The selected document could not be written.");
            if (editor != null) sourceText = editor.getText().toString();
            openDocuments.put(currentDocumentName, sourceText);
            outputStream.write(sourceText.getBytes(StandardCharsets.UTF_8));
            status.setText("TRG source exported.");
        } catch (Exception exception) {
            showError("Export failed", exception);
        }
    }

    private void savePackage(Uri uri) {
        try (OutputStream outputStream = getContentResolver().openOutputStream(uri)) {
            if (outputStream == null) throw new IllegalStateException("The selected document could not be written.");
            if (editor != null) sourceText = editor.getText().toString();
            openDocuments.put(currentDocumentName, sourceText);
            try (ZipOutputStream zip = new ZipOutputStream(outputStream)) {
                byte[] source = sourceText.getBytes(StandardCharsets.UTF_8);
                zip.putNextEntry(new ZipEntry("program.trg"));
                zip.write(source);
                zip.closeEntry();
                zip.putNextEntry(new ZipEntry("README.txt"));
                zip.write(("TRG source package. Import this .trgpack in TRG Studio to edit or run it.\n"
                        + "This is not a standalone native executable or APK.\n").getBytes(StandardCharsets.UTF_8));
                zip.closeEntry();
            }
            status.setText("Importable .trgpack created. It is not a standalone native executable.");
        } catch (Exception exception) {
            showError("Packaging failed", exception);
        }
    }

    private void showError(String title, Exception exception) {
        new AlertDialog.Builder(this).setTitle(title)
                .setMessage(exception.getMessage() == null ? exception.toString() : exception.getMessage())
                .setPositiveButton("OK", null).show();
    }

    private int dp(float value) {
        return (int) (value * getResources().getDisplayMetrics().density + 0.5f);
    }

    private final class PreviewView extends View {
        private final Paint paint = new Paint(Paint.ANTI_ALIAS_FLAG);
        private final RectF buttonRect = new RectF();
        private final List<JSONObject> orderedBlocks = new ArrayList<>();
        private final List<JSONObject> textItems = new ArrayList<>();
        private final List<JSONObject> sprites = new ArrayList<>();
        private final List<JSONObject> models = new ArrayList<>();
        private final HashMap<String, Bitmap> spriteBitmaps = new HashMap<>();
        private final HashMap<String, JSONObject> animationByName = new HashMap<>();
        private final float[] animatedPositionScratch = new float[2];
        private JSONObject state = new JSONObject();
        private long updatedAt;
        private float touchStartX;
        private float touchStartY;
        private boolean swipeDetected;
        private int touchSequence;
        private final boolean menuToolbar;

        private final boolean editor2D;
        private final boolean editor3D;
        private String selectedEditorName = "";
        private String selectedEditorKind = "";
        private boolean editorObjectDragging;
        private boolean editorMoved;
        private int editorGestureMode;
        private float lastEditorX;
        private float lastEditorY;
        private float lastPinchSpan;
        private float lastPinchCenterX;
        private float lastPinchCenterY;
        private float cameraYaw = 0.48f;
        private float cameraPitch = 0.32f;
        private float cameraZoom = 1.0f;

        private final class EditorHit {
            final String kind;
            final String name;
            EditorHit(String kind, String name) { this.kind = kind; this.name = name; }
        }

        PreviewView() { this(false, false, false); }
        PreviewView(boolean menuToolbar) { this(menuToolbar, false, false); }

        PreviewView(boolean menuToolbar, boolean editor2D, boolean editor3D) {
            super(MainActivity.this);
            this.menuToolbar = menuToolbar;
            this.editor2D = editor2D;
            this.editor3D = editor3D;
            setBackgroundColor(Color.rgb(13, 11, 18));
            setFocusable(true);
        }

        void setState(JSONObject next) {
            state = next;
            updatedAt = SystemClock.uptimeMillis();
            animationByName.clear();
            JSONArray nextAnimations = state.optJSONArray("animations");
            if (nextAnimations != null) {
                for (int i = 0; i < nextAnimations.length(); i++) {
                    JSONObject animation = nextAnimations.optJSONObject(i);
                    if (animation != null) animationByName.put(animation.optString("name"), animation);
                }
            }
            orderedBlocks.clear();
            textItems.clear();
            sprites.clear();
            models.clear();
            spriteBitmaps.clear();
            JSONArray modelArray = state.optJSONArray("models");
            if (modelArray != null) {
                for (int i = 0; i < modelArray.length(); i++) {
                    JSONObject model = modelArray.optJSONObject(i);
                    if (model != null) models.add(model);
                }
            }
            JSONArray spriteArray = state.optJSONArray("sprites");
            if (spriteArray != null) {
                for (int i = 0; i < spriteArray.length(); i++) {
                    JSONObject sprite = spriteArray.optJSONObject(i);
                    if (sprite != null) sprites.add(sprite);
                }
            }
            JSONArray blocks = state.optJSONArray("blocks");
            if (blocks != null) {
                for (int i = 0; i < blocks.length(); i++) {
                    JSONObject block = blocks.optJSONObject(i);
                    if (block != null) orderedBlocks.add(block);
                }
                orderedBlocks.sort((first, second) -> Integer.compare(first.optInt("z"), second.optInt("z")));
            }
            JSONArray texts = state.optJSONArray("texts");
            if (texts != null) {
                for (int i = 0; i < texts.length(); i++) {
                    JSONObject text = texts.optJSONObject(i);
                    if (text != null) textItems.add(text);
                }
            }
            if (editor2D || editor3D) {
                String selectionLabel = editor3D ? "model_label" : "object_label";
                JSONArray labels = state.optJSONArray("texts");
                if (labels != null) for (int i = 0; i < labels.length(); i++) {
                    JSONObject label = labels.optJSONObject(i);
                    if (label == null || !selectionLabel.equals(label.optString("name"))) continue;
                    String candidate = label.optString("value");
                    if (!candidate.startsWith("No ")) {
                        selectedEditorName = candidate;
                        selectedEditorKind = editor3D ? "3d" :
                                findJsonObject(state.optJSONArray("blocks"), candidate) != null ? "block" :
                                findJsonObject(state.optJSONArray("buttons"), candidate) != null ? "button" : "text";
                    } else {
                        selectedEditorName = "";
                        selectedEditorKind = "";
                    }
                    break;
                }
            }
            invalidate();
        }

        private void drawBlockShape(Canvas canvas, JSONObject block, RectF rect) {
            String shape = block.optString("shape", "square").toLowerCase(Locale.ROOT);
            if ("circle".equals(shape)) {
                canvas.drawOval(rect, paint);
                return;
            }
            if ("triangle".equals(shape) || "pentagon".equals(shape)
                    || "hexagon".equals(shape) || "star".equals(shape)) {
                int points = "triangle".equals(shape) ? 3 : "pentagon".equals(shape) ? 5
                        : "hexagon".equals(shape) ? 6 : 10;
                Path path = new Path();
                float cx = rect.centerX(), cy = rect.centerY();
                float rx = rect.width() * 0.5f, ry = rect.height() * 0.5f;
                for (int i = 0; i < points; i++) {
                    double angle = -Math.PI / 2 + i * Math.PI * 2 / points;
                    float factor = "star".equals(shape) && i % 2 == 1 ? 0.42f : 1f;
                    float x = cx + (float) Math.cos(angle) * rx * factor;
                    float y = cy + (float) Math.sin(angle) * ry * factor;
                    if (i == 0) path.moveTo(x, y); else path.lineTo(x, y);
                }
                path.close();
                canvas.drawPath(path, paint);
                return;
            }
            float radius = (float) (Math.max(0, block.optDouble("radius")) * Math.min(getWidth(), getHeight()) / 100);
            radius = Math.min(radius, Math.min(rect.width(), rect.height()) / 2f);
            if (radius > 0) canvas.drawRoundRect(rect, radius, radius, paint);
            else canvas.drawRect(rect, paint);
        }

        private float[] project3DPoint(double x, double y, double z, float width, float height) {
            double cy = Math.cos(cameraYaw), sy = Math.sin(cameraYaw);
            double cp = Math.cos(cameraPitch), sp = Math.sin(cameraPitch);
            double viewX = x * cy - z * sy;
            double depthAxis = x * sy + z * cy;
            double viewY = y * cp - depthAxis * sp;
            double depth = y * sp + depthAxis * cp;
            double perspective = Math.max(0.42, Math.min(1.85, 28.0 / Math.max(8.0, 28.0 + depth)));
            float scale = Math.min(width, height) * 0.115f * cameraZoom;
            return new float[] { width * 0.5f + (float) (viewX * scale * perspective),
                    height * 0.53f - (float) (viewY * scale * perspective), (float) perspective };
        }

        private void draw3DModels(Canvas canvas, float width, float height) {
            if (models.isEmpty() || width <= 0 || height <= 0) return;
            final int[][] faces = {
                    {0, 1, 2, 3}, {4, 5, 6, 7}, {0, 3, 7, 4},
                    {1, 2, 6, 5}, {0, 1, 5, 4}, {3, 2, 6, 7}
            };
            final float[] shades = {0.58f, 1.0f, 0.68f, 0.78f, 0.52f, 0.90f};
            for (JSONObject model : models) {
                if (!model.optBoolean("visible", true)) continue;
                int base = parseColor(model.optString("color", "#8B5CF6"));
                float opacity = Math.max(0f, Math.min(1f, (float) model.optDouble("opacity", 1.0)));
                double rx = Math.toRadians(model.optDouble("rx"));
                double ry = Math.toRadians(model.optDouble("ry"));
                double rz = Math.toRadians(model.optDouble("rz"));
                double cx = Math.cos(rx), sx = Math.sin(rx);
                double cy = Math.cos(ry), sy = Math.sin(ry);
                double cz = Math.cos(rz), sz = Math.sin(rz);
                float halfW = (float) model.optDouble("w", 2) * 0.5f;
                float halfH = (float) model.optDouble("h", 2) * 0.5f;
                float halfD = (float) model.optDouble("d", 2) * 0.5f;
                double[][] local = {
                        {-halfW,-halfH,-halfD}, {halfW,-halfH,-halfD},
                        {halfW,halfH,-halfD}, {-halfW,halfH,-halfD},
                        {-halfW,-halfH,halfD}, {halfW,-halfH,halfD},
                        {halfW,halfH,halfD}, {-halfW,halfH,halfD}
                };
                float[][] projected = new float[8][2];
                for (int i = 0; i < 8; i++) {
                    double x1 = local[i][0];
                    double y1 = local[i][1] * cx - local[i][2] * sx;
                    double z1 = local[i][1] * sx + local[i][2] * cx;
                    double x2 = x1 * cy + z1 * sy;
                    double y2 = y1;
                    double z2 = -x1 * sy + z1 * cy;
                    double x3 = x2 * cz - y2 * sz + model.optDouble("x");
                    double y3 = x2 * sz + y2 * cz + model.optDouble("y");
                    double z3 = z2 + model.optDouble("z");
                    float[] point = project3DPoint(x3, y3, z3, width, height);
                    projected[i][0] = point[0];
                    projected[i][1] = point[1];
                }
                float[] center = project3DPoint(model.optDouble("x"), model.optDouble("y"),
                        model.optDouble("z"), width, height);
                float[] ground = project3DPoint(model.optDouble("x"),
                        model.optDouble("y") - halfH - 0.08, model.optDouble("z"), width, height);
                float shadowWidth = Math.max(dp(10), halfW * Math.min(width, height) * 0.115f * cameraZoom * ground[2]);
                float shadowHeight = Math.max(dp(4), halfD * Math.min(width, height) * 0.038f * cameraZoom * ground[2]);
                paint.setStyle(Paint.Style.FILL);
                paint.setColor(Color.argb(46, 18, 14, 30));
                canvas.drawOval(new RectF(ground[0] - shadowWidth, ground[1] - shadowHeight * 0.5f,
                        ground[0] + shadowWidth, ground[1] + shadowHeight * 0.5f), paint);
                if ("sphere".equals(model.optString("shape"))) {
                    float radiusX = halfW * Math.min(width, height) * 0.115f * cameraZoom * center[2];
                    float radiusY = halfH * Math.min(width, height) * 0.115f * cameraZoom * center[2];
                    RectF sphere = new RectF(center[0] - radiusX, center[1] - radiusY,
                            center[0] + radiusX, center[1] + radiusY);
                    int red = Color.red(base), green = Color.green(base), blue = Color.blue(base);
                    int highlight = Color.rgb(Math.min(255, red + 82), Math.min(255, green + 82),
                            Math.min(255, blue + 82));
                    int shadowColor = Color.rgb(Math.round(red * 0.38f), Math.round(green * 0.38f),
                            Math.round(blue * 0.42f));
                    paint.setStyle(Paint.Style.FILL);
                    paint.setShader(new android.graphics.RadialGradient(
                            sphere.left + radiusX * 0.34f, sphere.top + radiusY * 0.28f,
                            Math.max(radiusX, radiusY) * 1.35f,
                            new int[] { highlight, base, shadowColor },
                            new float[] { 0f, 0.58f, 1f }, android.graphics.Shader.TileMode.CLAMP));
                    paint.setAlpha(Math.round(255 * opacity));
                    canvas.drawOval(sphere, paint);
                    paint.setShader(null);
                    paint.setAlpha(255);
                } else {
                    Integer[] order = {0, 1, 2, 3, 4, 5};
                    java.util.Arrays.sort(order, (a, b) -> {
                        double da = 0, db = 0;
                        for (int vertex : faces[a]) da += projected[vertex][1];
                        for (int vertex : faces[b]) db += projected[vertex][1];
                        return Double.compare(da, db);
                    });
                    for (int face : order) {
                        Path path = new Path();
                        path.moveTo(projected[faces[face][0]][0], projected[faces[face][0]][1]);
                        for (int corner = 1; corner < 4; corner++)
                            path.lineTo(projected[faces[face][corner]][0], projected[faces[face][corner]][1]);
                        path.close();
                        float shade = shades[face] * opacity;
                        paint.setStyle(Paint.Style.FILL);
                        paint.setColor(Color.rgb(Math.round(Color.red(base) * shade),
                                Math.round(Color.green(base) * shade), Math.round(Color.blue(base) * shade)));
                        canvas.drawPath(path, paint);
                        paint.setStyle(Paint.Style.STROKE);
                        paint.setStrokeWidth(dp(1));
                        paint.setColor(Color.rgb(37, 29, 53));
                        canvas.drawPath(path, paint);
                    }
                    paint.setStyle(Paint.Style.FILL);
                }
                if (model.optString("name").equals(selectedEditorName)) {
                    float radius = Math.max(dp(18), Math.max(halfW, Math.max(halfH, halfD))
                            * Math.min(width, height) * 0.115f * cameraZoom * center[2] * 1.35f);
                    paint.setStyle(Paint.Style.STROKE);
                    paint.setStrokeWidth(dp(2));
                    paint.setColor(Color.rgb(224, 204, 255));
                    canvas.drawCircle(center[0], center[1], radius, paint);
                    paint.setStyle(Paint.Style.FILL);
                }
            }
            paint.setColor(Color.rgb(178, 165, 204));
            paint.setTypeface(fixedUiFontBold);
            paint.setTextSize(dp(10));
            paint.setTextAlign(Paint.Align.LEFT);
            canvas.drawText("DRAG OBJECT  •  TWO FINGERS: ORBIT + ZOOM", dp(12), height - dp(14), paint);
        }

        private void drawSelected2DOutline(Canvas canvas, float width, float height) {
            if (selectedEditorName.isEmpty() || editor3D) return;
            JSONObject match = findJsonObject(state.optJSONArray("blocks"), selectedEditorName);
            if (match == null) match = findJsonObject(state.optJSONArray("sprites"), selectedEditorName);
            if (match == null) match = findJsonObject(state.optJSONArray("buttons"), selectedEditorName);
            if (match == null) match = findJsonObject(state.optJSONArray("texts"), selectedEditorName);
            if (match == null) return;
            double x = match.optDouble("x"), y = match.optDouble("y");
            double w = match.optDouble("w", match.optDouble("size") * 3);
            double h = match.optDouble("h", match.optDouble("size") * 1.4);
            buttonRect.set((float) (x * width / 100 - dp(3)), (float) (y * height / 100 - dp(3)),
                    (float) ((x + w) * width / 100 + dp(3)), (float) ((y + h) * height / 100 + dp(3)));
            paint.setStyle(Paint.Style.STROKE);
            paint.setStrokeWidth(dp(2));
            paint.setColor(Color.rgb(224, 204, 255));
            canvas.drawRoundRect(buttonRect, dp(4), dp(4), paint);
            paint.setStyle(Paint.Style.FILL);
        }

        private JSONObject findJsonObject(JSONArray array, String name) {
            if (array == null) return null;
            for (int i = 0; i < array.length(); i++) {
                JSONObject item = array.optJSONObject(i);
                if (item != null && name.equals(item.optString("name"))) return item;
            }
            return null;
        }

        private float[] animatedPosition(String name, float x, float y, double elapsed, JSONArray animations) {
            JSONObject animation = animationByName.get(name);
            if (animation != null) {
                double start = animation.optDouble("start");
                double duration = Math.max(0.001, animation.optDouble("duration", 1));
                double progress = Math.max(0, Math.min(1, (elapsed - start) / duration));
                x = (float) (animation.optDouble("x") + animation.optDouble("dx") * progress);
                y = (float) (animation.optDouble("y") + animation.optDouble("dy") * progress);
            }
            animatedPositionScratch[0] = x;
            animatedPositionScratch[1] = y;
            return animatedPositionScratch;
        }

        private String rotationSource(JSONObject model, String name) {
            double rx = model.optDouble("rx"), ry = model.optDouble("ry"), rz = model.optDouble("rz");
            if (Math.abs(rx) < 0.001 && Math.abs(ry) < 0.001 && Math.abs(rz) < 0.001) return "";
            return "\n3d rotate " + name + " " + number(rx) + " " + number(ry) + " " + number(rz);
        }

        @Override
        protected void onDraw(Canvas canvas) {
            super.onDraw(canvas);
            canvas.drawColor(Color.rgb(13, 11, 18));
            float width = getWidth();
            float height = getHeight();
            JSONArray blocks = state.optJSONArray("blocks");
            boolean animating = false;
            double elapsed = state.optDouble("elapsed")
                    + (SystemClock.uptimeMillis() - updatedAt) / 1000.0;
            JSONArray animations = state.optJSONArray("animations");
            if (animations != null) {
                for (int i = 0; i < animations.length(); i++) {
                    JSONObject animation = animations.optJSONObject(i);
                    if (animation != null && elapsed < animation.optDouble("start") + animation.optDouble("duration")
                            && elapsed >= animation.optDouble("start")) animating = true;
                }
            }
            if (blocks != null) {
                for (JSONObject block : orderedBlocks) {
                    if (!block.optBoolean("visible", true)) continue;
                    double x = block.optDouble("x");
                    double y = block.optDouble("y");
                    double bx = x;
                    double by = y;
                    JSONObject animation = animationByName.get(block.optString("name"));
                    if (animation != null) {
                        double start = animation.optDouble("start");
                        double duration = Math.max(0.001, animation.optDouble("duration", 1));
                        double progress = Math.max(0, Math.min(1, (elapsed - start) / duration));
                        if (elapsed < start + duration) animating = true;
                        bx = animation.optDouble("x") + animation.optDouble("dx") * progress;
                        by = animation.optDouble("y") + animation.optDouble("dy") * progress;
                    }
                    int blockColor = parseColor(block.optString("color"));
                    paint.setColor(Color.argb(Math.round(Color.alpha(blockColor) * Math.max(0f,
                            Math.min(1f, (float) block.optDouble("opacity", 1.0)))), Color.red(blockColor),
                            Color.green(blockColor), Color.blue(blockColor)));
                    paint.setStyle(Paint.Style.FILL);
                    buttonRect.set((float) (bx * width / 100), (float) (by * height / 100),
                            (float) ((bx + block.optDouble("w")) * width / 100),
                            (float) ((by + block.optDouble("h")) * height / 100));
                    drawBlockShape(canvas, block, buttonRect);
                }
            }
            draw3DModels(canvas, width, height);
            for (JSONObject sprite : sprites) {
                if (!sprite.optBoolean("visible", true)) continue;
                String path = sprite.optString("path");
                Bitmap bitmap = spriteBitmaps.get(path);
                if (bitmap == null) {
                    bitmap = BitmapFactory.decodeFile(path);
                    if (bitmap != null) spriteBitmaps.put(path, bitmap);
                }
                if (bitmap != null) {
                    float[] spritePosition = animatedPosition(sprite.optString("name"),
                            (float) sprite.optDouble("x"), (float) sprite.optDouble("y"), elapsed, animations);
                    buttonRect.set(spritePosition[0] * width / 100,
                            spritePosition[1] * height / 100,
                            (spritePosition[0] + (float) sprite.optDouble("w")) * width / 100,
                            (spritePosition[1] + (float) sprite.optDouble("h")) * height / 100);
                    paint.setAlpha(Math.round(255 * Math.max(0f, Math.min(1f,
                            (float) sprite.optDouble("opacity", 1.0)))));
                    canvas.drawBitmap(bitmap, null, buttonRect, paint);
                }
            }
            for (JSONObject text : textItems) {
                int textColor = parseColor(text.optString("color"));
                paint.setColor(Color.argb(Math.round(Color.alpha(textColor) * Math.max(0f,
                        Math.min(1f, (float) text.optDouble("opacity", 1.0)))), Color.red(textColor),
                        Color.green(textColor), Color.blue(textColor)));
                paint.setTextAlign("center".equals(text.optString("align")) ? Paint.Align.CENTER :
                        "right".equals(text.optString("align")) ? Paint.Align.RIGHT : Paint.Align.LEFT);
                paint.setTypeface(text.optBoolean("bold", true) ? fixedUiFontBold : fixedUiFont);
                paint.setTextSize((float) (height * text.optDouble("size") / 100));
                float[] textPosition = animatedPosition(text.optString("name"),
                        (float) text.optDouble("x"), (float) text.optDouble("y"), elapsed, animations);
                canvas.drawText(text.optString("value"),
                        textPosition[0] * width / 100, textPosition[1] * height / 100, paint);
            }
            drawSelected2DOutline(canvas, width, height);
            JSONArray buttons = state.optJSONArray("buttons");
            if (buttons != null) {
                paint.setTextAlign(Paint.Align.CENTER);
                paint.setTextSize(dp(12));
                for (int i = 0; i < buttons.length(); i++) {
                    JSONObject button = buttons.optJSONObject(i);
                    if (button == null || !button.optBoolean("visible", true)) continue;
                    float[] buttonPosition = animatedPosition(button.optString("name"),
                            (float) button.optDouble("x"), (float) button.optDouble("y"), elapsed, animations);
                    float left = buttonPosition[0] * width / 100;
                    float top = buttonPosition[1] * height / 100;
                    float right = (buttonPosition[0] + (float) button.optDouble("w")) * width / 100;
                    float bottom = (buttonPosition[1] + (float) button.optDouble("h")) * height / 100;
                    int buttonColor = parseColor(button.optString("color", "#7043BE"));
                    paint.setColor(Color.argb(Math.round(255 * Math.max(0f, Math.min(1f,
                            (float) button.optDouble("opacity", 1.0)))), Color.red(buttonColor),
                            Color.green(buttonColor), Color.blue(buttonColor)));
                    buttonRect.set(left, top, right, bottom);
                    float radius = (float) Math.max(0, button.optDouble("radius", 7));
                    radius = Math.min(dp(radius), Math.min(buttonRect.width(), buttonRect.height()) / 2f);
                    if (radius > 0) canvas.drawRoundRect(buttonRect, radius, radius, paint);
                    else canvas.drawRect(buttonRect, paint);
                    int labelColor = parseColor(button.optString("text_color", "#FFFFFF"));
                    paint.setColor(Color.argb(Math.round(Color.alpha(labelColor) * Math.max(0f,
                            Math.min(1f, (float) button.optDouble("opacity", 1.0)))), Color.red(labelColor),
                            Color.green(labelColor), Color.blue(labelColor)));
                    paint.setTypeface(button.optBoolean("bold", true) ? fixedUiFontBold : fixedUiFont);
                    paint.setTextSize((float) (button.optDouble("text_size", 0.34) * buttonRect.height()));
                    canvas.drawText(button.optString("label"), (left + right) / 2,
                            (top + bottom) / 2 - (paint.ascent() + paint.descent()) / 2, paint);
                }
            }
            if (animating) postInvalidateDelayed(16);
            if ((blocks == null || blocks.length() == 0) && sprites.isEmpty() && models.isEmpty()
                    && (buttons == null || buttons.length() == 0)) {
                paint.setColor(Color.LTGRAY);
                paint.setTextAlign(Paint.Align.CENTER);
                paint.setTextSize(dp(14));
                canvas.drawText("Run a program containing add (ui) to preview it", width / 2, height / 2, paint);
            }
        }

        @Override
        public boolean onTouchEvent(MotionEvent event) {
            if (editor2D || editor3D) return handleEditorTouch(event);
            if (event.getAction() == MotionEvent.ACTION_DOWN) {
                if (menuToolbar) return true;
                touchSequence++;
                touchStartX = event.getX();
                touchStartY = event.getY();
                swipeDetected = false;
                releaseSwipes();
                nativeSetMobileInput("mobile_tap", "press");
                nativeSetMobileInput("mobile_hold", "press");
                nativeSetPointer(getWidth() == 0 ? 0 : event.getX() * 100f / getWidth(),
                        getHeight() == 0 ? 0 : event.getY() * 100f / getHeight(), true);
                return true;
            }
            if (event.getAction() == MotionEvent.ACTION_MOVE) {
                nativeSetPointer(getWidth() == 0 ? 0 : event.getX() * 100f / getWidth(),
                        getHeight() == 0 ? 0 : event.getY() * 100f / getHeight(), true);
                if (swipeDetected) return true;
                float deltaX = event.getX() - touchStartX;
                float deltaY = event.getY() - touchStartY;
                if (Math.max(Math.abs(deltaX), Math.abs(deltaY)) >= dp(32)) {
                    if (Math.abs(deltaX) > Math.abs(deltaY)) {
                        nativeSetMobileInput(deltaX < 0 ? MOBILE_SWIPES[0] : MOBILE_SWIPES[1], "press");
                    } else {
                        nativeSetMobileInput(deltaY < 0 ? MOBILE_SWIPES[2] : MOBILE_SWIPES[3], "press");
                    }
                    swipeDetected = true;
                }
                return true;
            }
            if (event.getAction() == MotionEvent.ACTION_UP) {
                nativeSetPointer(getWidth() == 0 ? 0 : event.getX() * 100f / getWidth(),
                        getHeight() == 0 ? 0 : event.getY() * 100f / getHeight(), false);
                if (menuToolbar) {
                    int tappedButton = hitButton(event.getX(), event.getY());
                    JSONArray toolbarButtons = state.optJSONArray("buttons");
                    if (tappedButton >= 0 && toolbarButtons != null) {
                        for (int i = 0; i < toolbarButtons.length(); i++) {
                            JSONObject item = toolbarButtons.optJSONObject(i);
                            if (item != null && item.optInt("index", i) == tappedButton) {
                                onTrgStudioAction(item.optString("name", "menu"));
                                break;
                            }
                        }
                    }
                    performClick();
                    return true;
                }
                nativeSetMobileInput("mobile_hold", "released");
                int buttonIndex = hitButton(event.getX(), event.getY());
                int sequence = touchSequence;
                if (buttonIndex >= 0 && studioMenuPreview == this) {
                    String action = buttonName(buttonIndex);
                    nativeSetMobileInput("mobile_tap", "released");
                    if (!action.isEmpty()) onTrgStudioAction(action);
                    postDelayed(() -> {
                        if (sequence == touchSequence) releaseSwipes();
                    }, 150);
                } else if (buttonIndex >= 0) {
                    runButton(buttonIndex);
                    postDelayed(() -> {
                        if (sequence == touchSequence) releaseSwipes();
                    }, 150);
                } else {
                    postDelayed(() -> {
                        if (sequence == touchSequence) {
                            nativeSetMobileInput("mobile_tap", "released");
                            releaseSwipes();
                        }
                    }, 150);
                }
                performClick();
                return true;
            }
            if (event.getAction() == MotionEvent.ACTION_CANCEL) {
                nativeSetPointer(getWidth() == 0 ? 0 : event.getX() * 100f / getWidth(),
                        getHeight() == 0 ? 0 : event.getY() * 100f / getHeight(), false);
                touchSequence++;
                nativeSetMobileInput("mobile_hold", "released");
                nativeSetMobileInput("mobile_tap", "released");
                releaseSwipes();
                return true;
            }
            return true;
        }

        private EditorHit hitEditorObject(float px, float py) {
            if (getWidth() <= 0 || getHeight() <= 0) return null;
            float x = px * 100f / getWidth();
            float y = py * 100f / getHeight();
            if (y < 14 || y > 64) return null;
            if (editor3D) {
                for (int i = models.size() - 1; i >= 0; i--) {
                    JSONObject model = models.get(i);
                    if (!model.optBoolean("visible", true)) continue;
                    float[] center = project3DPoint(model.optDouble("x"), model.optDouble("y"),
                            model.optDouble("z"), getWidth(), getHeight());
                    float radius = Math.max(dp(22), (float) Math.max(model.optDouble("w", 2),
                            Math.max(model.optDouble("h", 2), model.optDouble("d", 2)))
                            * Math.min(getWidth(), getHeight()) * 0.115f * cameraZoom * center[2] * 0.85f);
                    if (Math.hypot(px - center[0], py - center[1]) <= radius)
                        return new EditorHit("3d", model.optString("name"));
                }
                return null;
            }
            for (int i = orderedBlocks.size() - 1; i >= 0; i--) {
                JSONObject item = orderedBlocks.get(i);
                String name = item.optString("name");
                if (!item.optBoolean("visible", true) || name.startsWith("EDITOR_")
                        || name.startsWith("GRID_") || name.equals("CANVAS")
                        || name.equals("CANVAS_FRAME") || name.equals("VIEWPORT")
                        || name.equals("VIEWPORT_FRAME") || name.startsWith("FLOOR_")) continue;
                if (inside(x, y, item.optDouble("x"), item.optDouble("y"),
                        item.optDouble("w"), item.optDouble("h"))) return new EditorHit("block", name);
            }
            for (int i = sprites.size() - 1; i >= 0; i--) {
                JSONObject item = sprites.get(i);
                if (item.optBoolean("visible", true) && inside(x, y, item.optDouble("x"),
                        item.optDouble("y"), item.optDouble("w"), item.optDouble("h")))
                    return new EditorHit("sprite", item.optString("name"));
            }
            JSONArray buttonArray = state.optJSONArray("buttons");
            if (buttonArray != null) for (int i = buttonArray.length() - 1; i >= 0; i--) {
                JSONObject item = buttonArray.optJSONObject(i);
                if (item != null && item.optBoolean("visible", true) && inside(x, y,
                        item.optDouble("x"), item.optDouble("y"), item.optDouble("w"), item.optDouble("h")))
                    return new EditorHit("button", item.optString("name"));
            }
            JSONArray textArray = state.optJSONArray("texts");
            if (textArray != null) for (int i = textArray.length() - 1; i >= 0; i--) {
                JSONObject item = textArray.optJSONObject(i);
                if (item == null || !item.optBoolean("visible", true)) continue;
                double size = item.optDouble("size", 4);
                double textWidth = Math.max(size, item.optString("value").length() * size * 0.58);
                if (inside(x, y, item.optDouble("x"), item.optDouble("y") - size,
                        textWidth, size * 1.4)) return new EditorHit("text", item.optString("name"));
            }
            return null;
        }

        private boolean inside(double x, double y, double left, double top, double width, double height) {
            return x >= left && x <= left + width && y >= top && y <= top + height;
        }

        private float span(MotionEvent event) {
            if (event.getPointerCount() < 2) return 0;
            return (float) Math.hypot(event.getX(0) - event.getX(1), event.getY(0) - event.getY(1));
        }

        private boolean handleEditorTouch(MotionEvent event) {
            int action = event.getActionMasked();
            if (action == MotionEvent.ACTION_DOWN) {
                touchSequence++;
                lastEditorX = event.getX();
                lastEditorY = event.getY();
                editorMoved = false;
                editorGestureMode = 0;
                EditorHit hit = hitEditorObject(lastEditorX, lastEditorY);
                if (hit != null) {
                    selectedEditorKind = hit.kind;
                    selectedEditorName = hit.name;
                    nativeSetEditorSelection(selectedEditorName);
                    status.setText("Selected: " + selectedEditorName);
                    editorObjectDragging = true;
                    editorGestureMode = 1;
                } else {
                    editorObjectDragging = false;
                    if (editor3D) editorGestureMode = 2;
                }
                invalidate();
                return true;
            }
            if (action == MotionEvent.ACTION_POINTER_DOWN && editor3D && event.getPointerCount() >= 2) {
                editorGestureMode = 3;
                editorObjectDragging = false;
                lastPinchSpan = Math.max(1, span(event));
                lastPinchCenterX = (event.getX(0) + event.getX(1)) * 0.5f;
                lastPinchCenterY = (event.getY(0) + event.getY(1)) * 0.5f;
                return true;
            }
            if (action == MotionEvent.ACTION_MOVE) {
                if (editor3D && event.getPointerCount() >= 2) {
                    float currentSpan = Math.max(1, span(event));
                    float centerX = (event.getX(0) + event.getX(1)) * 0.5f;
                    float centerY = (event.getY(0) + event.getY(1)) * 0.5f;
                    if (lastPinchSpan > 0) cameraZoom = Math.max(0.3f, Math.min(4.0f,
                            cameraZoom * currentSpan / lastPinchSpan));
                    cameraYaw += (centerX - lastPinchCenterX) * 0.006f;
                    cameraPitch = Math.max(-1.25f, Math.min(1.25f,
                            cameraPitch + (centerY - lastPinchCenterY) * 0.006f));
                    lastPinchSpan = currentSpan;
                    lastPinchCenterX = centerX;
                    lastPinchCenterY = centerY;
                    editorMoved = true;
                    invalidate();
                    return true;
                }
                float x = event.getX();
                float y = event.getY();
                float dx = x - lastEditorX;
                float dy = y - lastEditorY;
                if (Math.abs(dx) + Math.abs(dy) > dp(2)) editorMoved = true;
                if (editorGestureMode == 1 && !selectedEditorName.isEmpty()) {
                    if (editor2D) {
                        String result = nativeTransformEditorObject("2d", selectedEditorName,
                                dx * 100f / Math.max(1, getWidth()),
                                dy * 100f / Math.max(1, getHeight()), 0);
                        applyEditorTransform(result);
                    } else if (editor3D) {
                        float unit = 1f / Math.max(1f, Math.min(getWidth(), getHeight()) * 0.115f * cameraZoom);
                        float worldX = dx * unit * (float) Math.cos(cameraYaw);
                        float worldZ = -dx * unit * (float) Math.sin(cameraYaw);
                        float worldY = -dy * unit;
                        String result = nativeTransformEditorObject("3d", selectedEditorName, worldX, worldY, worldZ);
                        applyEditorTransform(result);
                    }
                } else if (editor3D && editorGestureMode == 2) {
                    cameraYaw += dx * 0.008f;
                    cameraPitch = Math.max(-1.25f, Math.min(1.25f, cameraPitch + dy * 0.008f));
                    invalidate();
                }
                lastEditorX = x;
                lastEditorY = y;
                return true;
            }
            if (action == MotionEvent.ACTION_POINTER_UP) {
                if (editor3D) {
                    editorGestureMode = 2;
                    if (event.getPointerCount() > 1) {
                        int remaining = event.getActionIndex() == 0 ? 1 : 0;
                        lastEditorX = event.getX(remaining);
                        lastEditorY = event.getY(remaining);
                    }
                }
                return true;
            }
            if (action == MotionEvent.ACTION_UP) {
                int tappedButton = hitButton(event.getX(), event.getY());
                if (!editorMoved && tappedButton >= 0) {
                    String actionName = buttonName(tappedButton);
                    if ("copy_code".equalsIgnoreCase(actionName)) copySelectedObjectCode();
                    else if ("reset_view".equalsIgnoreCase(actionName)) {
                        cameraYaw = 0.48f;
                        cameraPitch = 0.32f;
                        cameraZoom = 1.0f;
                        invalidate();
                    } else runButton(tappedButton);
                } else if (!editorMoved && selectedEditorName.isEmpty()) {
                    EditorHit hit = hitEditorObject(event.getX(), event.getY());
                    if (hit != null) {
                        selectedEditorKind = hit.kind;
                        selectedEditorName = hit.name;
                        invalidate();
                    }
                }
                editorObjectDragging = false;
                editorGestureMode = 0;
                performClick();
                return true;
            }
            if (action == MotionEvent.ACTION_CANCEL) {
                editorObjectDragging = false;
                editorGestureMode = 0;
                return true;
            }
            return true;
        }

        private void applyEditorTransform(String rawState) {
            try {
                JSONObject next = new JSONObject(rawState);
                lastRunState = next;
                state = next;
                updatedAt = SystemClock.uptimeMillis();
                orderedBlocks.clear();
                textItems.clear();
                sprites.clear();
                models.clear();
                JSONArray modelArray = state.optJSONArray("models");
                if (modelArray != null) for (int i = 0; i < modelArray.length(); i++) {
                    JSONObject item = modelArray.optJSONObject(i);
                    if (item != null) models.add(item);
                }
                JSONArray spriteArray = state.optJSONArray("sprites");
                if (spriteArray != null) for (int i = 0; i < spriteArray.length(); i++) {
                    JSONObject item = spriteArray.optJSONObject(i);
                    if (item != null) sprites.add(item);
                }
                JSONArray blockArray = state.optJSONArray("blocks");
                if (blockArray != null) for (int i = 0; i < blockArray.length(); i++) {
                    JSONObject item = blockArray.optJSONObject(i);
                    if (item != null) orderedBlocks.add(item);
                }
                orderedBlocks.sort((first, second) -> Integer.compare(first.optInt("z"), second.optInt("z")));
                JSONArray textArray = state.optJSONArray("texts");
                if (textArray != null) for (int i = 0; i < textArray.length(); i++) {
                    JSONObject item = textArray.optJSONObject(i);
                    if (item != null) textItems.add(item);
                }
                invalidate();
            } catch (JSONException exception) {
                status.setText("Editor update failed: " + exception.getMessage());
            }
        }

        private void copySelectedObjectCode() {
            String name = selectedEditorName;
            if (name.isEmpty() || (editor3D
                    ? findJsonObject(state.optJSONArray("models"), name) == null
                    : findJsonObject(state.optJSONArray("blocks"), name) == null
                        && findJsonObject(state.optJSONArray("buttons"), name) == null
                        && findJsonObject(state.optJSONArray("texts"), name) == null)) {
                String labelName = editor3D ? "model_label" : "object_label";
                JSONArray labels = state.optJSONArray("texts");
                if (labels != null) for (int i = 0; i < labels.length(); i++) {
                    JSONObject label = labels.optJSONObject(i);
                    if (label != null && labelName.equals(label.optString("name"))) {
                        String value = label.optString("value");
                        if (!value.startsWith("No ")) name = value;
                        break;
                    }
                }
            }
            if (name.isEmpty()) {
                Toast.makeText(MainActivity.this, "Select an object first", Toast.LENGTH_SHORT).show();
                return;
            }
            String code = null;
            if (editor3D) {
                JSONObject model = findJsonObject(state.optJSONArray("models"), name);
                if (model != null) {
                    if ("sphere".equals(model.optString("shape"))) {
                        code = "3d sphere " + name + " x " + number(model.optDouble("x"))
                                + " y " + number(model.optDouble("y")) + " z " + number(model.optDouble("z"))
                                + " size " + number(model.optDouble("w")) + " color " + model.optString("color")
                                + rotationSource(model, name);
                    } else {
                        code = "3d box " + name + " x " + number(model.optDouble("x"))
                                + " y " + number(model.optDouble("y")) + " z " + number(model.optDouble("z"))
                                + " w " + number(model.optDouble("w")) + " h " + number(model.optDouble("h"))
                                + " d " + number(model.optDouble("d")) + " color " + model.optString("color")
                                + rotationSource(model, name);
                    }
                }
            } else {
                JSONObject block = findJsonObject(state.optJSONArray("blocks"), name);
                if (block != null) {
                    code = "block z " + block.optInt("z") + " x " + number(block.optDouble("x"))
                            + " y " + number(block.optDouble("y")) + " h " + number(block.optDouble("h"))
                            + " w " + number(block.optDouble("w")) + " color " + block.optString("color")
                            + " shape " + block.optString("shape", "square") + " name " + name;
                } else {
                    JSONObject button = findJsonObject(state.optJSONArray("buttons"), name);
                    if (button != null) {
                        code = "button " + name + " \"" + escapeCode(button.optString("label")) + "\" x "
                                + number(button.optDouble("x")) + " y " + number(button.optDouble("y"))
                                + " w " + number(button.optDouble("w")) + " h " + number(button.optDouble("h"))
                                + " color " + button.optString("color") + " ( say \"Clicked " + name + "\" )";
                    } else {
                        JSONObject text = findJsonObject(state.optJSONArray("texts"), name);
                        if (text != null) {
                            code = "text " + name + " \"" + escapeCode(text.optString("value")) + "\" x "
                                    + number(text.optDouble("x")) + " y " + number(text.optDouble("y"))
                                    + " size " + number(text.optDouble("size")) + " color " + text.optString("color");
                        }
                    }
                }
            }
            if (code == null) {
                Toast.makeText(MainActivity.this, "Could not find the selected object", Toast.LENGTH_SHORT).show();
                return;
            }
            code = (editor3D ? "add (3d)\n" : "add (ui)\n") + code;
            ClipboardManager clipboard = (ClipboardManager) getSystemService(CLIPBOARD_SERVICE);
            if (clipboard != null) {
                clipboard.setPrimaryClip(ClipData.newPlainText("TRG object source", code));
                Toast.makeText(MainActivity.this, "TRG creation code copied", Toast.LENGTH_SHORT).show();
            }
        }

        private String escapeCode(String value) {
            return value.replace("\\", "\\\\").replace("\"", "\\\"");
        }

        private String number(double value) {
            return String.format(Locale.US, "%.3f", value).replaceAll("0+$", "").replaceAll("\\.$", "");
        }

        private void releaseSwipes() {
            for (String swipe : MOBILE_SWIPES) nativeSetMobileInput(swipe, "released");
        }

        private String buttonName(int index) {
            JSONArray buttons = state.optJSONArray("buttons");
            if (buttons == null) return "";
            for (int i = 0; i < buttons.length(); i++) {
                JSONObject button = buttons.optJSONObject(i);
                if (button != null && button.optInt("index", i) == index)
                    return button.optString("name", "");
            }
            return "";
        }

        private int hitButton(float x, float y) {
            JSONArray buttons = state.optJSONArray("buttons");
            if (buttons == null || getWidth() == 0 || getHeight() == 0) return -1;
            for (int i = buttons.length() - 1; i >= 0; i--) {
                JSONObject button = buttons.optJSONObject(i);
                if (button == null || !button.optBoolean("visible", true)) continue;
                float left = (float) (button.optDouble("x") * getWidth() / 100);
                float top = (float) (button.optDouble("y") * getHeight() / 100);
                float right = (float) ((button.optDouble("x") + button.optDouble("w")) * getWidth() / 100);
                float bottom = (float) ((button.optDouble("y") + button.optDouble("h")) * getHeight() / 100);
                if (x >= left && x <= right && y >= top && y <= bottom) return button.optInt("index", i);
            }
            return -1;
        }

        @Override
        public boolean performClick() {
            super.performClick();
            return true;
        }

        private int parseColor(String source) {
            try {
                if (source.startsWith("##") && source.length() == 8) {
                    long channels = Long.parseLong(source.substring(2), 16);
                    int blue = (int) ((channels >> 16) & 255);
                    int red = (int) ((channels >> 8) & 255);
                    int green = (int) (channels & 255);
                    return Color.rgb(red, green, blue);
                }
                if (source.startsWith("#")) return Color.parseColor(source);
                if (source.startsWith("0x") || source.startsWith("0X")) {
                    long value = Long.parseLong(source.substring(2), 16);
                    if (source.length() > 8) return (int) value;
                    return Color.rgb((int) ((value >> 16) & 255), (int) ((value >> 8) & 255), (int) (value & 255));
                }
            } catch (IllegalArgumentException ignored) {
                return Color.MAGENTA;
            }
            return Color.MAGENTA;
        }
    }
}
