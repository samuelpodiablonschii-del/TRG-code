package com.trg.fontmaker;

import android.app.Activity;
import android.content.Intent;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.Typeface;
import android.graphics.drawable.GradientDrawable;
import android.net.Uri;
import android.os.Bundle;
import android.view.Gravity;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.ScrollView;
import android.widget.TextView;
import android.widget.Toast;

import org.json.JSONArray;
import org.json.JSONObject;

import java.io.File;
import java.io.FileInputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;

public final class MainActivity extends Activity {
    private static final int REQUEST_EXPORT = 201;
    private static final int REQUEST_PROJECT = 202;

    static { System.loadLibrary("trg_font_maker"); }

    private native boolean nativeExportFont(String outputPath, String familyName, String styleName,
        int[] codepoints, int[] advances, int[] offsets, short[] xs, short[] ys);

    private static final class GlyphPoint {
        final short x;
        final short y;
        GlyphPoint(int x, int y) { this.x = (short) x; this.y = (short) y; }
    }

    private static final class Glyph {
        final int codepoint;
        int advance = 600;
        final List<GlyphPoint> points = new ArrayList<>();
        Glyph(int codepoint) { this.codepoint = codepoint; }
    }

    private final List<Glyph> glyphs = new ArrayList<>();
    private int currentIndex = 0;
    private EditText familyField;
    private EditText styleField;
    private EditText characterField;
    private EditText advanceField;
    private GlyphCanvasView canvas;
    private TextView selectedGlyph;
    private Uri pendingUri;
    private String pendingProjectJson;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getWindow().setStatusBarColor(Color.rgb(12, 14, 23));
        getWindow().setNavigationBarColor(Color.rgb(12, 14, 23));
        Glyph notdef = new Glyph(0);
        notdef.advance = 600;
        notdef.points.add(new GlyphPoint(80, 0));
        notdef.points.add(new GlyphPoint(520, 0));
        notdef.points.add(new GlyphPoint(520, 700));
        notdef.points.add(new GlyphPoint(80, 700));
        glyphs.add(notdef);
        Glyph space = new Glyph(32);
        space.advance = 300;
        glyphs.add(space);
        buildUi();
        selectGlyph(0);
    }

    private int dp(int value) {
        return (int) (value * getResources().getDisplayMetrics().density);
    }

    private GradientDrawable rounded(int color) {
        GradientDrawable drawable = new GradientDrawable();
        drawable.setColor(color);
        drawable.setCornerRadius(dp(11));
        drawable.setStroke(dp(1), Color.rgb(57, 62, 88));
        return drawable;
    }

    private Button button(String label, Runnable action) {
        Button result = new Button(this);
        result.setText(label);
        result.setAllCaps(false);
        result.setTextSize(12);
        result.setTypeface(Typeface.create("sans-serif-medium", Typeface.NORMAL));
        result.setTextColor(Color.rgb(235, 231, 248));
        result.setBackground(rounded(Color.rgb(31, 35, 53)));
        result.setMinHeight(dp(44));
        result.setOnClickListener(view -> action.run());
        return result;
    }

    private void buildUi() {
        LinearLayout root = new LinearLayout(this);
        root.setOrientation(LinearLayout.VERTICAL);
        root.setPadding(dp(14), dp(12), dp(14), dp(12));
        root.setBackgroundColor(Color.rgb(12, 14, 23));

        TextView title = new TextView(this);
        title.setText("TRG  /  FONT MAKER");
        title.setTextSize(20);
        title.setTypeface(Typeface.create("sans-serif", Typeface.BOLD));
        title.setTextColor(Color.WHITE);
        root.addView(title);

        TextView subtitle = new TextView(this);
        subtitle.setText("Design glyphs and export a real TrueType font.");
        subtitle.setTextSize(11);
        subtitle.setTextColor(Color.rgb(145, 151, 174));
        subtitle.setPadding(0, dp(4), 0, dp(10));
        root.addView(subtitle);

        ScrollView formScroll = new ScrollView(this);
        LinearLayout form = new LinearLayout(this);
        form.setOrientation(LinearLayout.VERTICAL);
        familyField = field("Family name", "TRG Custom");
        styleField = field("Style", "Regular");
        characterField = field("Character", "A");
        advanceField = field("Advance width", "600");
        form.addView(familyField);
        form.addView(styleField);
        form.addView(characterField);
        form.addView(advanceField);
        Button selectButton = button("Create / select glyph", this::addGlyph);
        form.addView(selectButton);
        formScroll.addView(form);
        root.addView(formScroll, new LinearLayout.LayoutParams(
            ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT));

        selectedGlyph = new TextView(this);
        selectedGlyph.setTextColor(Color.rgb(203, 185, 255));
        selectedGlyph.setTextSize(15);
        selectedGlyph.setTypeface(Typeface.DEFAULT_BOLD);
        selectedGlyph.setPadding(0, dp(8), 0, dp(6));
        root.addView(selectedGlyph);

        canvas = new GlyphCanvasView();
        LinearLayout.LayoutParams canvasParams = new LinearLayout.LayoutParams(
            ViewGroup.LayoutParams.MATCH_PARENT, 0, 1f);
        canvasParams.topMargin = dp(7);
        canvasParams.bottomMargin = dp(8);
        root.addView(canvas, canvasParams);

        LinearLayout actions = new LinearLayout(this);
        actions.setOrientation(LinearLayout.HORIZONTAL);
        actions.addView(button("Clear outline", this::clearOutline), new LinearLayout.LayoutParams(0, dp(48), 1f));
        actions.addView(button("Save project", this::saveProject), new LinearLayout.LayoutParams(0, dp(48), 1f));
        actions.addView(button("Export TTF", this::exportFont), new LinearLayout.LayoutParams(0, dp(48), 1f));
        root.addView(actions);

        TextView installNote = new TextView(this);
        installNote.setText("Device-wide font changes depend on your phone manufacturer. Export the TTF and use supported font settings where available.");
        installNote.setTextColor(Color.rgb(145, 151, 174));
        installNote.setTextSize(10);
        installNote.setPadding(0, dp(8), 0, 0);
        root.addView(installNote);

        setContentView(root);
    }

    private EditText field(String hint, String value) {
        EditText edit = new EditText(this);
        edit.setSingleLine(true);
        edit.setText(value);
        edit.setHint(hint);
        edit.setTextSize(13);
        edit.setTextColor(Color.WHITE);
        edit.setHintTextColor(Color.rgb(120, 126, 148));
        edit.setPadding(dp(12), dp(5), dp(12), dp(5));
        edit.setBackground(rounded(Color.rgb(8, 10, 17)));
        LinearLayout.LayoutParams params = new LinearLayout.LayoutParams(
            ViewGroup.LayoutParams.MATCH_PARENT, dp(47));
        params.bottomMargin = dp(6);
        edit.setLayoutParams(params);
        return edit;
    }

    private void addGlyph() {
        String text = characterField.getText().toString();
        if (text.isEmpty()) {
            Toast.makeText(this, "Enter a character first.", Toast.LENGTH_SHORT).show();
            return;
        }
        int codepoint = text.codePointAt(0);
        if (codepoint > 0xffff || (codepoint >= 0xd800 && codepoint <= 0xdfff)) {
            Toast.makeText(this, "This first exporter supports BMP characters.", Toast.LENGTH_LONG).show();
            return;
        }
        int found = -1;
        for (int i = 0; i < glyphs.size(); i++) {
            if (glyphs.get(i).codepoint == codepoint) { found = i; break; }
        }
        if (found < 0) {
            glyphs.add(new Glyph(codepoint));
            Collections.sort(glyphs, Comparator.comparingInt(glyph -> glyph.codepoint));
            for (int i = 0; i < glyphs.size(); i++) {
                if (glyphs.get(i).codepoint == codepoint) { found = i; break; }
            }
        }
        selectGlyph(found);
    }

    private void selectGlyph(int index) {
        if (index < 0 || index >= glyphs.size()) return;
        currentIndex = index;
        Glyph glyph = glyphs.get(index);
        selectedGlyph.setText(new String(Character.toChars(glyph.codepoint)) + "   U+" +
            String.format("%04X", glyph.codepoint));
        characterField.setText(new String(Character.toChars(glyph.codepoint)));
        advanceField.setText(Integer.toString(glyph.advance));
        canvas.setPoints(glyph.points);
    }

    private void clearOutline() {
        if (currentIndex < 0 || currentIndex >= glyphs.size()) return;
        glyphs.get(currentIndex).points.clear();
        canvas.invalidate();
    }

    private int[] codepoints() {
        int[] values = new int[glyphs.size()];
        for (int i = 0; i < glyphs.size(); i++) values[i] = glyphs.get(i).codepoint;
        return values;
    }

    private int[] advances() {
        int[] values = new int[glyphs.size()];
        for (int i = 0; i < glyphs.size(); i++) {
            if (i == currentIndex) {
                try { glyphs.get(i).advance = Integer.parseInt(advanceField.getText().toString()); }
                catch (Exception ignored) { }
            }
            values[i] = Math.max(0, Math.min(2000, glyphs.get(i).advance));
        }
        return values;
    }

    private int[] offsets() {
        int[] values = new int[glyphs.size() + 1];
        int total = 0;
        for (int i = 0; i < glyphs.size(); i++) {
            values[i] = total;
            total += glyphs.get(i).points.size();
        }
        values[glyphs.size()] = total;
        return values;
    }

    private short[] pointCoordinates(boolean xAxis) {
        int total = offsets()[glyphs.size()];
        short[] values = new short[total];
        int index = 0;
        for (Glyph glyph : glyphs) {
            for (GlyphPoint point : glyph.points) values[index++] = xAxis ? point.x : point.y;
        }
        return values;
    }

    private void exportFont() {
        Intent intent = new Intent(Intent.ACTION_CREATE_DOCUMENT);
        intent.addCategory(Intent.CATEGORY_OPENABLE);
        intent.setType("application/x-font-ttf");
        intent.putExtra(Intent.EXTRA_TITLE, safeName(familyField.getText().toString()) + ".ttf");
        startActivityForResult(intent, REQUEST_EXPORT);
    }

    private void saveProject() {
        try {
            JSONObject root = new JSONObject();
            root.put("format", "TRGFontProject");
            root.put("version", 1);
            root.put("familyName", familyField.getText().toString());
            root.put("styleName", styleField.getText().toString());
            JSONArray array = new JSONArray();
            for (Glyph glyph : glyphs) {
                JSONObject item = new JSONObject();
                item.put("codepoint", glyph.codepoint);
                item.put("advanceWidth", glyph.advance);
                JSONArray points = new JSONArray();
                for (GlyphPoint point : glyph.points) {
                    JSONArray p = new JSONArray();
                    p.put(point.x); p.put(point.y);
                    points.put(p);
                }
                item.put("points", points);
                array.put(item);
            }
            root.put("glyphs", array);
            pendingProjectJson = root.toString(2);
            Intent intent = new Intent(Intent.ACTION_CREATE_DOCUMENT);
            intent.addCategory(Intent.CATEGORY_OPENABLE);
            intent.setType("application/json");
            intent.putExtra(Intent.EXTRA_TITLE, safeName(familyField.getText().toString()) + ".trgfont");
            startActivityForResult(intent, REQUEST_PROJECT);
        } catch (Exception error) {
            Toast.makeText(this, "Could not create project data.", Toast.LENGTH_LONG).show();
        }
    }

    private String safeName(String input) {
        String value = input.replaceAll("[^A-Za-z0-9_-]", "-");
        return value.isEmpty() ? "TRG-Custom" : value;
    }

    private void writeFont(Uri uri) {
        File temp = new File(getCacheDir(), "trg-font-output.ttf");
        boolean ok = nativeExportFont(temp.getAbsolutePath(), familyField.getText().toString(),
            styleField.getText().toString(), codepoints(), advances(), offsets(),
            pointCoordinates(true), pointCoordinates(false));
        if (!ok || !temp.exists()) {
            Toast.makeText(this, "Font export failed. Check that each outline has at least three points.", Toast.LENGTH_LONG).show();
            return;
        }
        try (FileInputStream input = new FileInputStream(temp);
             OutputStream output = getContentResolver().openOutputStream(uri)) {
            if (output == null) throw new IllegalStateException("Could not open the selected destination.");
            byte[] buffer = new byte[8192];
            int read;
            while ((read = input.read(buffer)) != -1) output.write(buffer, 0, read);
            output.flush();
            Toast.makeText(this, "TTF exported. System-wide installation depends on device support.", Toast.LENGTH_LONG).show();
        } catch (Exception error) {
            Toast.makeText(this, error.getMessage() == null ? "Could not save the font." : error.getMessage(), Toast.LENGTH_LONG).show();
        }
    }

    @Override
    @Deprecated
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (resultCode != RESULT_OK || data == null || data.getData() == null) return;
        Uri uri = data.getData();
        if (requestCode == REQUEST_EXPORT) {
            writeFont(uri);
        } else if (requestCode == REQUEST_PROJECT) {
            try (OutputStream output = getContentResolver().openOutputStream(uri)) {
                if (output == null) throw new IllegalStateException("Could not open the selected destination.");
                output.write(pendingProjectJson.getBytes(java.nio.charset.StandardCharsets.UTF_8));
                output.flush();
                Toast.makeText(this, "Font project saved.", Toast.LENGTH_SHORT).show();
            } catch (Exception error) {
                Toast.makeText(this, "Could not save the project.", Toast.LENGTH_LONG).show();
            }
        }
    }

    private final class GlyphCanvasView extends View {
        private final Paint paint = new Paint(Paint.ANTI_ALIAS_FLAG);
        private List<GlyphPoint> points = new ArrayList<>();

        GlyphCanvasView() {
            super(MainActivity.this);
            setBackground(rounded(Color.rgb(7, 9, 15)));
            setMinimumHeight(dp(250));
        }

        void setPoints(List<GlyphPoint> source) {
            points = source;
            invalidate();
        }

        @Override
        protected void onDraw(Canvas canvas) {
            super.onDraw(canvas);
            float left = dp(25);
            float top = dp(20);
            float areaWidth = Math.max(dp(100), getWidth() - dp(50));
            float areaHeight = Math.max(dp(100), getHeight() - dp(40));
            float scale = areaWidth / 1000f;
            float baseline = top + areaHeight * 0.8f;
            paint.setColor(Color.rgb(35, 39, 57));
            paint.setStrokeWidth(dp(1));
            paint.setStyle(Paint.Style.STROKE);
            for (int i = 0; i <= 10; i++) {
                float x = left + i * areaWidth / 10f;
                canvas.drawLine(x, top, x, top + areaHeight, paint);
                float y = top + i * areaHeight / 10f;
                canvas.drawLine(left, y, left + areaWidth, y, paint);
            }
            paint.setColor(Color.rgb(139, 121, 201));
            paint.setStrokeWidth(dp(2));
            canvas.drawLine(left, baseline, left + areaWidth, baseline, paint);
            if (!points.isEmpty()) {
                Path path = new Path();
                for (int i = 0; i < points.size(); i++) {
                    GlyphPoint p = points.get(i);
                    float x = left + p.x * scale;
                    float y = baseline - p.y * scale;
                    if (i == 0) path.moveTo(x, y); else path.lineTo(x, y);
                }
                if (points.size() >= 3) path.close();
                paint.setColor(Color.rgb(197, 178, 255));
                paint.setStrokeWidth(dp(2));
                paint.setStyle(Paint.Style.STROKE);
                canvas.drawPath(path, paint);
                paint.setColor(Color.rgb(156, 233, 197));
                paint.setStyle(Paint.Style.FILL);
                for (GlyphPoint p : points) {
                    canvas.drawCircle(left + p.x * scale, baseline - p.y * scale, dp(4), paint);
                }
            }
        }

        @Override
        public boolean onTouchEvent(MotionEvent event) {
            if (event.getAction() != MotionEvent.ACTION_DOWN) return true;
            float left = dp(25);
            float top = dp(20);
            float areaWidth = Math.max(dp(100), getWidth() - dp(50));
            float areaHeight = Math.max(dp(100), getHeight() - dp(40));
            float scale = areaWidth / 1000f;
            float baseline = top + areaHeight * 0.8f;
            int x = Math.max(0, Math.min(1000, Math.round((event.getX() - left) / scale)));
            int y = Math.max(-200, Math.min(800, Math.round((baseline - event.getY()) / scale)));
            points.add(new GlyphPoint(x, y));
            invalidate();
            return true;
        }
    }
}
