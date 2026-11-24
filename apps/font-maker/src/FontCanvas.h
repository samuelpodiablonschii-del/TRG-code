#pragma once

#include "FontProject.h"

#include <QWidget>
#include <QRectF>

#include <functional>

class FontCanvas final : public QWidget {
public:
    explicit FontCanvas(QWidget* parent = nullptr);

    void setContour(const GlyphContour& contour);
    const GlyphContour& contour() const noexcept { return contour_; }
    void clearContour();
    void setContourChanged(std::function<void(const GlyphContour&)> callback);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

private:
    GlyphContour contour_;
    std::function<void(const GlyphContour&)> changed_;
    QRectF drawingArea() const;
    QPointF toCanvasPoint(const GlyphPoint& point) const;
};
