package com.openscansa.app.ui

import android.content.Context
import android.graphics.Canvas
import android.graphics.Color
import android.graphics.Paint
import android.graphics.RectF
import android.util.AttributeSet
import android.view.View

class BarcodeOverlayView @JvmOverloads constructor(
    context: Context,
    attrs: AttributeSet? = null
) : View(context, attrs) {
    private val borderPaint = Paint(Paint.ANTI_ALIAS_FLAG).apply {
        color = Color.RED
        style = Paint.Style.STROKE
        strokeWidth = resources.displayMetrics.density * 3
    }
    private var barcodeBounds: List<RectF> = emptyList()

    fun showBarcodes(bounds: List<RectF>) {
        barcodeBounds = bounds
        invalidate()
    }

    fun clear() {
        barcodeBounds = emptyList()
        invalidate()
    }

    override fun onDraw(canvas: Canvas) {
        super.onDraw(canvas)
        barcodeBounds.forEach { canvas.drawRect(it, borderPaint) }
    }
}
