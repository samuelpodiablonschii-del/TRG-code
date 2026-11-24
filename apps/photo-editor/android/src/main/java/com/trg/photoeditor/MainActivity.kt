package com.trg.photoeditor

import android.app.Activity
import android.content.Intent
import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.graphics.Color
import android.graphics.Typeface
import android.graphics.drawable.GradientDrawable
import android.net.Uri
import android.os.Bundle
import android.view.Gravity
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.FrameLayout
import android.widget.HorizontalScrollView
import android.widget.ImageView
import android.widget.LinearLayout
import android.widget.TextView
import android.widget.Toast

class MainActivity : Activity() {
    companion object {
        private const val REQUEST_OPEN = 101
        private const val REQUEST_SAVE = 102
        init { System.loadLibrary("trg_photo_editor") }
    }

    private external fun nativeTransform(bitmap: Bitmap, operation: Int): Bitmap?
    private var currentBitmap: Bitmap? = null
    private lateinit var imageView: ImageView
    private lateinit var emptyHint: TextView
    private lateinit var statusView: TextView

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        window.statusBarColor = Color.rgb(12, 14, 23)
        window.navigationBarColor = Color.rgb(12, 14, 23)
        buildUi()
    }

    private fun dp(value: Int): Int = (value * resources.displayMetrics.density).toInt()

    private fun rounded(color: Int, stroke: Int = Color.TRANSPARENT): GradientDrawable =
        GradientDrawable().apply {
            shape = GradientDrawable.RECTANGLE
            cornerRadius = dp(12).toFloat()
            setColor(color)
            if (stroke != Color.TRANSPARENT) setStroke(dp(1), stroke)
        }

    private fun actionButton(label: String, onClick: () -> Unit): Button =
        Button(this).apply {
            text = label
            isAllCaps = false
            textSize = 12f
            typeface = Typeface.create("sans-serif-medium", Typeface.NORMAL)
            setTextColor(Color.rgb(235, 231, 248))
            background = rounded(Color.rgb(31, 35, 53), Color.rgb(57, 62, 88))
            minHeight = dp(44)
            setPadding(dp(12), dp(5), dp(12), dp(5))
            setOnClickListener { onClick() }
        }

    private fun buildUi() {
        val root = LinearLayout(this).apply {
            orientation = LinearLayout.VERTICAL
            setPadding(dp(14), dp(12), dp(14), dp(12))
            setBackgroundColor(Color.rgb(12, 14, 23))
        }

        val heading = TextView(this).apply {
            text = "TRG  /  PHOTO EDITOR"
            textSize = 20f
            typeface = Typeface.create("sans-serif", Typeface.BOLD)
            setTextColor(Color.WHITE)
        }
        root.addView(heading)

        val subtitle = TextView(this).apply {
            text = "Edit images with a precise C++ processing core."
            textSize = 11f
            setTextColor(Color.rgb(145, 151, 174))
            setPadding(0, dp(4), 0, dp(12))
        }
        root.addView(subtitle)

        val toolbar = HorizontalScrollView(this).apply { isHorizontalScrollBarEnabled = false }
        val toolbarRow = LinearLayout(this).apply { orientation = LinearLayout.HORIZONTAL }
        toolbar.addView(toolbarRow)
        toolbarRow.addView(actionButton("Open") { openImage() }, marginParams())
        toolbarRow.addView(actionButton("Save") { saveImage() }, marginParams())
        toolbarRow.addView(actionButton("2× pixels") { applyOperation(0, "Expanded every pixel into a 2×2 block.") }, marginParams())
        toolbarRow.addView(actionButton("Rotate") { applyOperation(1, "Rotated 90 degrees.") }, marginParams())
        root.addView(toolbar)

        val secondRow = HorizontalScrollView(this).apply { isHorizontalScrollBarEnabled = false }
        val secondButtons = LinearLayout(this).apply { orientation = LinearLayout.HORIZONTAL }
        secondRow.addView(secondButtons)
        secondButtons.addView(actionButton("Flip H") { applyOperation(2, "Flipped horizontally.") }, marginParams())
        secondButtons.addView(actionButton("Flip V") { applyOperation(3, "Flipped vertically.") }, marginParams())
        secondButtons.addView(actionButton("Grayscale") { applyOperation(4, "Converted to grayscale.") }, marginParams())
        secondButtons.addView(actionButton("Invert") { applyOperation(5, "Inverted colours.") }, marginParams())
        secondButtons.addView(actionButton("Brighten") { applyOperation(6, "Brightness increased.") }, marginParams())
        secondButtons.addView(actionButton("Darken") { applyOperation(7, "Brightness decreased.") }, marginParams())
        secondButtons.addView(actionButton("Contrast +") { applyOperation(8, "Contrast increased.") }, marginParams())
        secondButtons.addView(actionButton("Contrast −") { applyOperation(9, "Contrast decreased.") }, marginParams())
        root.addView(secondRow)

        val canvas = FrameLayout(this).apply {
            background = rounded(Color.rgb(7, 9, 15), Color.rgb(40, 44, 62))
        }
        imageView = ImageView(this).apply {
            scaleType = ImageView.ScaleType.FIT_CENTER
            adjustViewBounds = true
            setPadding(dp(8), dp(8), dp(8), dp(8))
        }
        emptyHint = TextView(this).apply {
            text = "Open an image to begin"
            textSize = 15f
            setTextColor(Color.rgb(112, 120, 145))
            gravity = Gravity.CENTER
        }
        canvas.addView(imageView, FrameLayout.LayoutParams(
            ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT
        ))
        canvas.addView(emptyHint, FrameLayout.LayoutParams(
            ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT
        ))
        root.addView(canvas, LinearLayout.LayoutParams(
            ViewGroup.LayoutParams.MATCH_PARENT, 0, 1f
        ).apply { topMargin = dp(12); bottomMargin = dp(10) })

        statusView = TextView(this).apply {
            text = "Ready · PNG / JPEG / WebP"
            textSize = 10f
            setTextColor(Color.rgb(145, 151, 174))
            setPadding(dp(2), dp(4), dp(2), dp(4))
        }
        root.addView(statusView)
        setContentView(root)
    }

    private fun marginParams() = LinearLayout.LayoutParams(
        ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT
    ).apply { rightMargin = dp(7); bottomMargin = dp(7) }

    private fun openImage() {
        val intent = Intent(Intent.ACTION_GET_CONTENT).apply { type = "image/*" }
        startActivityForResult(Intent.createChooser(intent, "Choose an image"), REQUEST_OPEN)
    }

    private fun saveImage() {
        if (currentBitmap == null) {
            Toast.makeText(this, "Open an image first.", Toast.LENGTH_SHORT).show()
            return
        }
        val intent = Intent(Intent.ACTION_CREATE_DOCUMENT).apply {
            addCategory(Intent.CATEGORY_OPENABLE)
            type = "image/png"
            putExtra(Intent.EXTRA_TITLE, "TRG-Edited.png")
        }
        startActivityForResult(intent, REQUEST_SAVE)
    }

    private fun applyOperation(operation: Int, message: String) {
        val original = currentBitmap ?: run {
            Toast.makeText(this, "Open an image first.", Toast.LENGTH_SHORT).show()
            return
        }
        try {
            val mutable = original.copy(Bitmap.Config.ARGB_8888, true)
            val result = nativeTransform(mutable, operation)
            if (result == null) {
                Toast.makeText(this, "The operation could not be completed. The image may be too large.", Toast.LENGTH_LONG).show()
                return
            }
            currentBitmap = result
            imageView.setImageBitmap(result)
            emptyHint.visibility = View.GONE
            statusView.text = "${result.width} × ${result.height} px · $message"
        } catch (error: Exception) {
            Toast.makeText(this, error.message ?: "Image operation failed.", Toast.LENGTH_LONG).show()
        }
    }

    @Deprecated("Deprecated in Android API; retained for broad device compatibility.")
    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        super.onActivityResult(requestCode, resultCode, data)
        if (resultCode != RESULT_OK || data?.data == null) return
        val uri: Uri = data.data!!
        when (requestCode) {
            REQUEST_OPEN -> {
                try {
                    contentResolver.openInputStream(uri)?.use { stream ->
                        val decoded = BitmapFactory.decodeStream(stream)
                        if (decoded == null) {
                            Toast.makeText(this, "This image could not be opened.", Toast.LENGTH_LONG).show()
                            return
                        }
                        currentBitmap = decoded.copy(Bitmap.Config.ARGB_8888, true)
                        imageView.setImageBitmap(currentBitmap)
                        emptyHint.visibility = View.GONE
                        statusView.text = "${decoded.width} × ${decoded.height} px · Opened"
                    }
                } catch (error: Exception) {
                    Toast.makeText(this, error.message ?: "Could not open image.", Toast.LENGTH_LONG).show()
                }
            }
            REQUEST_SAVE -> {
                try {
                    val bitmap = currentBitmap ?: return
                    contentResolver.openOutputStream(uri)?.use { stream ->
                        if (!bitmap.compress(Bitmap.CompressFormat.PNG, 100, stream)) {
                            throw IllegalStateException("PNG export failed.")
                        }
                    }
                    Toast.makeText(this, "Image exported.", Toast.LENGTH_SHORT).show()
                } catch (error: Exception) {
                    Toast.makeText(this, error.message ?: "Could not save image.", Toast.LENGTH_LONG).show()
                }
            }
        }
    }
}
