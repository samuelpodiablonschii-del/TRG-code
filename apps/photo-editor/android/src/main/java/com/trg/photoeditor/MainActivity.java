package com.trg.photoeditor;

import android.app.Activity;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.graphics.Typeface;
import android.graphics.drawable.GradientDrawable;
import android.net.Uri;
import android.os.Bundle;
import android.view.Gravity;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.HorizontalScrollView;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;

public final class MainActivity extends Activity {
    private static final int REQUEST_OPEN = 101;
    private static final int REQUEST_SAVE = 102;

    static { System.loadLibrary("trg_photo_editor"); }

    private native Bitmap nativeTransform(Bitmap bitmap, int operation);

    private Bitmap currentBitmap;
    private ImageView imageView;
    private TextView emptyHint;
    private TextView statusView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getWindow().setStatusBarColor(Color.rgb(12, 14, 23));
        getWindow().setNavigationBarColor(Color.rgb(12, 14, 23));
        buildUi();
    }

    private int dp(int value) {
        return (int) (value * getResources().getDisplayMetrics().density);
    }

    private GradientDrawable rounded(int color, int stroke) {
        GradientDrawable drawable = new GradientDrawable();
        drawable.setShape(GradientDrawable.RECTANGLE);
        drawable.setCornerRadius(dp(12));
        drawable.setColor(color);
        if (stroke != Color.TRANSPARENT) drawable.setStroke(dp(1), stroke);
        return drawable;
    }

    private Button actionButton(String label, Runnable action) {
        Button button = new Button(this);
        button.setText(label);
        button.setAllCaps(false);
        button.setTextSize(12);
        button.setTypeface(Typeface.create("sans-serif-medium", Typeface.NORMAL));
        button.setTextColor(Color.rgb(235, 231, 248));
        button.setBackground(rounded(Color.rgb(31, 35, 53), Color.rgb(57, 62, 88)));
        button.setMinHeight(dp(44));
        button.setPadding(dp(12), dp(5), dp(12), dp(5));
        button.setOnClickListener(view -> action.run());
        return button;
    }

    private LinearLayout.LayoutParams toolbarParams() {
        LinearLayout.LayoutParams params = new LinearLayout.LayoutParams(
            ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT);
        params.rightMargin = dp(7);
        params.bottomMargin = dp(7);
        return params;
    }

    private void buildUi() {
        LinearLayout root = new LinearLayout(this);
        root.setOrientation(LinearLayout.VERTICAL);
        root.setPadding(dp(14), dp(12), dp(14), dp(12));
        root.setBackgroundColor(Color.rgb(12, 14, 23));

        TextView heading = new TextView(this);
        heading.setText("TRG  /  PHOTO EDITOR");
        heading.setTextSize(20);
        heading.setTypeface(Typeface.create("sans-serif", Typeface.BOLD));
        heading.setTextColor(Color.WHITE);
        root.addView(heading);

        TextView subtitle = new TextView(this);
        subtitle.setText("Edit images with a precise C++ processing core.");
        subtitle.setTextSize(11);
        subtitle.setTextColor(Color.rgb(145, 151, 174));
        subtitle.setPadding(0, dp(4), 0, dp(12));
        root.addView(subtitle);

        HorizontalScrollView firstScroll = new HorizontalScrollView(this);
        firstScroll.setHorizontalScrollBarEnabled(false);
        LinearLayout firstRow = new LinearLayout(this);
        firstRow.setOrientation(LinearLayout.HORIZONTAL);
        firstRow.addView(actionButton("Open", this::openImage), toolbarParams());
        firstRow.addView(actionButton("Save", this::saveImage), toolbarParams());
        firstRow.addView(actionButton("2× pixels", () -> applyOperation(0, "Expanded every pixel into a 2×2 block.")), toolbarParams());
        firstRow.addView(actionButton("Rotate", () -> applyOperation(1, "Rotated 90 degrees.")), toolbarParams());
        firstScroll.addView(firstRow);
        root.addView(firstScroll);

        HorizontalScrollView secondScroll = new HorizontalScrollView(this);
        secondScroll.setHorizontalScrollBarEnabled(false);
        LinearLayout secondRow = new LinearLayout(this);
        secondRow.setOrientation(LinearLayout.HORIZONTAL);
        secondRow.addView(actionButton("Flip H", () -> applyOperation(2, "Flipped horizontally.")), toolbarParams());
        secondRow.addView(actionButton("Flip V", () -> applyOperation(3, "Flipped vertically.")), toolbarParams());
        secondRow.addView(actionButton("Grayscale", () -> applyOperation(4, "Converted to grayscale.")), toolbarParams());
        secondRow.addView(actionButton("Invert", () -> applyOperation(5, "Inverted colours.")), toolbarParams());
        secondRow.addView(actionButton("Brighten", () -> applyOperation(6, "Brightness increased.")), toolbarParams());
        secondRow.addView(actionButton("Darken", () -> applyOperation(7, "Brightness decreased.")), toolbarParams());
        secondRow.addView(actionButton("Contrast +", () -> applyOperation(8, "Contrast increased.")), toolbarParams());
        secondRow.addView(actionButton("Contrast −", () -> applyOperation(9, "Contrast decreased.")), toolbarParams());
        secondScroll.addView(secondRow);
        root.addView(secondScroll);

        FrameLayout canvas = new FrameLayout(this);
        canvas.setBackground(rounded(Color.rgb(7, 9, 15), Color.rgb(40, 44, 62)));
        imageView = new ImageView(this);
        imageView.setScaleType(ImageView.ScaleType.FIT_CENTER);
        imageView.setPadding(dp(8), dp(8), dp(8), dp(8));
        emptyHint = new TextView(this);
        emptyHint.setText("Open an image to begin");
        emptyHint.setTextSize(15);
        emptyHint.setTextColor(Color.rgb(112, 120, 145));
        emptyHint.setGravity(Gravity.CENTER);
        canvas.addView(imageView, new FrameLayout.LayoutParams(
            ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT));
        canvas.addView(emptyHint, new FrameLayout.LayoutParams(
            ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT));

        LinearLayout.LayoutParams canvasParams = new LinearLayout.LayoutParams(
            ViewGroup.LayoutParams.MATCH_PARENT, 0, 1f);
        canvasParams.topMargin = dp(12);
        canvasParams.bottomMargin = dp(10);
        root.addView(canvas, canvasParams);

        statusView = new TextView(this);
        statusView.setText("Ready · PNG / JPEG / WebP");
        statusView.setTextSize(10);
        statusView.setTextColor(Color.rgb(145, 151, 174));
        statusView.setPadding(dp(2), dp(4), dp(2), dp(4));
        root.addView(statusView);

        setContentView(root);
    }

    private void openImage() {
        Intent intent = new Intent(Intent.ACTION_GET_CONTENT);
        intent.setType("image/*");
        startActivityForResult(Intent.createChooser(intent, "Choose an image"), REQUEST_OPEN);
    }

    private void saveImage() {
        if (currentBitmap == null) {
            Toast.makeText(this, "Open an image first.", Toast.LENGTH_SHORT).show();
            return;
        }
        Intent intent = new Intent(Intent.ACTION_CREATE_DOCUMENT);
        intent.addCategory(Intent.CATEGORY_OPENABLE);
        intent.setType("image/png");
        intent.putExtra(Intent.EXTRA_TITLE, "TRG-Edited.png");
        startActivityForResult(intent, REQUEST_SAVE);
    }

    private void applyOperation(int operation, String message) {
        if (currentBitmap == null) {
            Toast.makeText(this, "Open an image first.", Toast.LENGTH_SHORT).show();
            return;
        }
        try {
            Bitmap mutable = currentBitmap.copy(Bitmap.Config.ARGB_8888, true);
            Bitmap result = nativeTransform(mutable, operation);
            if (result == null) {
                Toast.makeText(this, "The operation could not be completed. The image may be too large.", Toast.LENGTH_LONG).show();
                return;
            }
            currentBitmap = result;
            imageView.setImageBitmap(result);
            emptyHint.setVisibility(View.GONE);
            statusView.setText(result.getWidth() + " × " + result.getHeight() + " px · " + message);
        } catch (Exception error) {
            Toast.makeText(this, error.getMessage() == null ? "Image operation failed." : error.getMessage(), Toast.LENGTH_LONG).show();
        }
    }

    @Override
    @Deprecated
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (resultCode != RESULT_OK || data == null || data.getData() == null) return;
        Uri uri = data.getData();
        if (requestCode == REQUEST_OPEN) {
            try (java.io.InputStream stream = getContentResolver().openInputStream(uri)) {
                Bitmap decoded = BitmapFactory.decodeStream(stream);
                if (decoded == null) {
                    Toast.makeText(this, "This image could not be opened.", Toast.LENGTH_LONG).show();
                    return;
                }
                currentBitmap = decoded.copy(Bitmap.Config.ARGB_8888, true);
                imageView.setImageBitmap(currentBitmap);
                emptyHint.setVisibility(View.GONE);
                statusView.setText(decoded.getWidth() + " × " + decoded.getHeight() + " px · Opened");
            } catch (Exception error) {
                Toast.makeText(this, error.getMessage() == null ? "Could not open image." : error.getMessage(), Toast.LENGTH_LONG).show();
            }
        } else if (requestCode == REQUEST_SAVE) {
            try (java.io.OutputStream stream = getContentResolver().openOutputStream(uri)) {
                if (stream == null || !currentBitmap.compress(Bitmap.CompressFormat.PNG, 100, stream)) {
                    throw new IllegalStateException("PNG export failed.");
                }
                Toast.makeText(this, "Image exported.", Toast.LENGTH_SHORT).show();
            } catch (Exception error) {
                Toast.makeText(this, error.getMessage() == null ? "Could not save image." : error.getMessage(), Toast.LENGTH_LONG).show();
            }
        }
    }
}
