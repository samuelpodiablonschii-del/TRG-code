#pragma once

#include <QImage>
#include <QMainWindow>
#include <QVector>

class QLabel;
class QScrollArea;
class QListWidget;
class QSlider;

class PhotoEditorWindow final : public QMainWindow {
public:
    explicit PhotoEditorWindow(QWidget* parent = nullptr);

private:
    QLabel* imageView_{nullptr};
    QLabel* imageInfo_{nullptr};
    QScrollArea* canvasScroll_{nullptr};
    QListWidget* layerList_{nullptr};
    QSlider* zoomSlider_{nullptr};
    QSlider* brightnessSlider_{nullptr};
    QSlider* contrastSlider_{nullptr};
    QVector<QImage> history_;
    int historyIndex_{-1};
    QImage image_;

    void buildInterface();
    void openImage();
    void saveImage();
    void saveImageAs();
    void setImage(const QImage& image, bool recordHistory = true);
    void refreshCanvas();
    void refreshHistoryActions();
    void applyImageOperation(const char* label, const QImage& (*operation)(const QImage&));
    void applyAdjustments();
    void cropImage();
    void undo();
    void redo();
};
