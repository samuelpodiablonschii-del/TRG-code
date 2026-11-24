#include "FontCanvas.h"

#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPaintEvent>

#include <algorithm>
#include <cmath>

FontCanvas::FontCanvas(QWidget* parent) : QWidget(parent) {
    setMinimumSize(300, 300);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setCursor(Qt::CrossCursor);
    setToolTip(tr("Tap or click to add points to the current glyph outline."));
}

QRectF FontCanvas::drawingArea() const {
    return QRectF(38, 30, std::max(100, width() - 76), std::max(100, height() - 60));
}

QPointF FontCanvas::toCanvasPoint(const GlyphPoint& point) const {
    const QRectF area = drawingArea();
    const qreal scale = area.width() / 1000.0;
    const qreal baseline = area.top() + area.height() * 0.8;
    return QPointF(area.left() + point.x * scale, baseline - point.y * scale);
}

void FontCanvas::setContour(const GlyphContour& contour) {
    contour_ = contour;
    update();
}

void FontCanvas::clearContour() {
    contour_.points.clear();
    if (changed_) changed_(contour_);
    update();
}

void FontCanvas::setContourChanged(std::function<void(const GlyphContour&)> callback) {
    changed_ = std::move(callback);
}

void FontCanvas::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillRect(rect(), QColor("#0b0d16"));

    const QRectF area = drawingArea();
    const qreal scale = area.width() / 1000.0;
    const qreal baseline = area.top() + area.height() * 0.8;

    painter.setPen(QPen(QColor("#24283a"), 1));
    for (int unit = 0; unit <= 1000; unit += 100) {
        const qreal x = area.left() + unit * scale;
        painter.drawLine(QPointF(x, area.top()), QPointF(x, area.bottom()));
    }
    for (int unit = -200; unit <= 800; unit += 100) {
        const qreal y = baseline - unit * scale;
        painter.drawLine(QPointF(area.left(), y), QPointF(area.right(), y));
    }

    painter.setPen(QPen(QColor("#8b79c9"), 1.4, Qt::DashLine));
    painter.drawLine(QPointF(area.left(), baseline), QPointF(area.right(), baseline));
    painter.setPen(QPen(QColor("#4d9db5"), 1, Qt::DashLine));
    painter.drawLine(QPointF(area.left(), baseline - 700 * scale), QPointF(area.right(), baseline - 700 * scale));
    painter.setPen(QPen(QColor("#a66d8c"), 1, Qt::DashLine));
    painter.drawLine(QPointF(area.left(), baseline - 800 * scale), QPointF(area.right(), baseline - 800 * scale));

    if (!contour_.points.empty()) {
        QPainterPath path;
        path.moveTo(toCanvasPoint(contour_.points.front()));
        for (std::size_t i = 1; i < contour_.points.size(); ++i) path.lineTo(toCanvasPoint(contour_.points[i]));
        if (contour_.points.size() >= 3) path.closeSubpath();
        painter.setPen(QPen(QColor("#c5b2ff"), 2));
        painter.setBrush(QColor(153, 124, 232, 55));
        painter.drawPath(path);

        painter.setBrush(QColor("#9ce9c5"));
        painter.setPen(QPen(QColor("#10121d"), 1));
        for (const auto& point : contour_.points) {
            painter.drawEllipse(toCanvasPoint(point), 4.5, 4.5);
        }
    }

    painter.setPen(QColor("#7e849b"));
    painter.drawText(QPointF(area.left(), 18), tr("GLYPH OUTLINE · 1000 UNITS PER EM"));
    painter.drawText(QPointF(area.left(), height() - 12), tr("Baseline"));
    painter.drawText(QPointF(area.right() - 75, height() - 12), tr("%1 points").arg(contour_.points.size()));
}

void FontCanvas::mousePressEvent(QMouseEvent* event) {
    if (event->button() != Qt::LeftButton) return;
    const QRectF area = drawingArea();
    if (!area.contains(event->position())) return;
    const qreal scale = area.width() / 1000.0;
    const qreal baseline = area.top() + area.height() * 0.8;
    const int x = std::clamp(static_cast<int>(std::lround((event->position().x() - area.left()) / scale)), 0, 1000);
    const int y = std::clamp(static_cast<int>(std::lround((baseline - event->position().y()) / scale)), -200, 800);
    contour_.points.push_back({static_cast<std::int16_t>(x), static_cast<std::int16_t>(y), true});
    if (changed_) changed_(contour_);
    update();
}
