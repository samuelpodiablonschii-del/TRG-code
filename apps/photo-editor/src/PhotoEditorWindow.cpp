#include "PhotoEditorWindow.h"
#include "ImageBuffer.h"

#include <QAction>
#include <QApplication>
#include <QFileDialog>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QLabel>
#include <QListWidget>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollArea>
#include <QSlider>
#include <QSplitter>
#include <QStatusBar>
#include <QToolBar>
#include <QVBoxLayout>
#include <QWidget>

#include <algorithm>
#include <functional>

using trg::photo::ImageBuffer;
using trg::photo::Pixel;

namespace {

ImageBuffer toBuffer(const QImage& source) {
    const QImage rgba = source.convertToFormat(QImage::Format_RGBA8888);
    ImageBuffer result(rgba.width(), rgba.height());
    for (int y = 0; y < rgba.height(); ++y) {
        const auto* row = rgba.constScanLine(y);
        for (int x = 0; x < rgba.width(); ++x) {
            const int offset = x * 4;
            result.at(x, y) = {
                row[offset], row[offset + 1], row[offset + 2], row[offset + 3]
            };
        }
    }
    return result;
}

QImage toQImage(const ImageBuffer& source) {
    if (source.empty()) return {};
    QImage result(source.width(), source.height(), QImage::Format_RGBA8888);
    for (int y = 0; y < source.height(); ++y) {
        auto* row = result.scanLine(y);
        for (int x = 0; x < source.width(); ++x) {
            const Pixel p = source.at(x, y);
            const int offset = x * 4;
            row[offset] = p.r;
            row[offset + 1] = p.g;
            row[offset + 2] = p.b;
            row[offset + 3] = p.a;
        }
    }
    return result;
}

QImage expand2x(const QImage& image) {
    return toQImage(toBuffer(image).expanded2x());
}
QImage rotateImage(const QImage& image) {
    return toQImage(toBuffer(image).rotatedClockwise());
}
QImage flipHorizontal(const QImage& image) {
    return toQImage(toBuffer(image).flippedHorizontal());
}
QImage flipVertical(const QImage& image) {
    return toQImage(toBuffer(image).flippedVertical());
}
QImage grayscaleImage(const QImage& image) {
    return toQImage(toBuffer(image).grayscale());
}
QImage invertImage(const QImage& image) {
    return toQImage(toBuffer(image).inverted());
}

QPushButton* makeToolButton(const QString& text, QWidget* parent) {
    auto* button = new QPushButton(text, parent);
    button->setMinimumHeight(42);
    button->setCursor(Qt::PointingHandCursor);
    return button;
}

}

PhotoEditorWindow::PhotoEditorWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle(QStringLiteral("TRG Photo Editor"));
    resize(1280, 820);
    setMinimumSize(360, 520);
    setStyleSheet(QStringLiteral(
        "QMainWindow,QWidget{background:#10121d;color:#eceaf5;font-family:'Segoe UI',sans-serif;}"
        "QMenuBar,QMenu,QToolBar{background:#171a29;color:#e9e5f5;border:0;}"
        "QMenu::item{padding:7px 25px;} QMenu::item:selected{background:#393052;}"
        "QPushButton{background:#202438;border:1px solid #343951;border-radius:8px;padding:8px 12px;color:#e9e5f5;}"
        "QPushButton:hover{background:#2b2d47;border-color:#8d75d8;}"
        "QPushButton:pressed{background:#393052;}"
        "QListWidget,QScrollArea{background:#0b0d16;border:1px solid #292d40;border-radius:8px;}"
        "QSlider::groove:horizontal{height:5px;background:#30344a;border-radius:2px;}"
        "QSlider::handle:horizontal{width:15px;margin:-5px 0;border-radius:7px;background:#b49aff;}"
        "QStatusBar{background:#171a29;color:#a7a4b8;}"
    ));
    buildInterface();
    statusBar()->showMessage(tr("Open an image to begin editing."));
}

void PhotoEditorWindow::buildInterface() {
    auto* fileMenu = menuBar()->addMenu(tr("&File"));
    auto* openAction = fileMenu->addAction(tr("&Open image…"));
    auto* saveAction = fileMenu->addAction(tr("&Save"));
    auto* saveAsAction = fileMenu->addAction(tr("Save &as…"));
    fileMenu->addSeparator();
    auto* exitAction = fileMenu->addAction(tr("E&xit"));
    connect(openAction, &QAction::triggered, this, [this] { openImage(); });
    connect(saveAction, &QAction::triggered, this, [this] { saveImage(); });
    connect(saveAsAction, &QAction::triggered, this, [this] { saveImageAs(); });
    connect(exitAction, &QAction::triggered, this, [this] { close(); });

    auto* editMenu = menuBar()->addMenu(tr("&Edit"));
    auto* undoAction = editMenu->addAction(tr("&Undo"));
    auto* redoAction = editMenu->addAction(tr("&Redo"));
    connect(undoAction, &QAction::triggered, this, [this] { undo(); });
    connect(redoAction, &QAction::triggered, this, [this] { redo(); });

    auto* toolbar = addToolBar(tr("Main tools"));
    toolbar->setMovable(false);
    toolbar->setIconSize(QSize(20, 20));
    auto* openTool = toolbar->addAction(tr("Open"));
    auto* saveTool = toolbar->addAction(tr("Save"));
    toolbar->addSeparator();
    auto* undoTool = toolbar->addAction(tr("Undo"));
    auto* redoTool = toolbar->addAction(tr("Redo"));
    connect(openTool, &QAction::triggered, this, [this] { openImage(); });
    connect(saveTool, &QAction::triggered, this, [this] { saveImage(); });
    connect(undoTool, &QAction::triggered, this, [this] { undo(); });
    connect(redoTool, &QAction::triggered, this, [this] { redo(); });

    auto* splitter = new QSplitter(this);
    const bool narrow = QApplication::primaryScreen() &&
                        QApplication::primaryScreen()->availableGeometry().width() < 720;
    splitter->setOrientation(narrow ? Qt::Vertical : Qt::Horizontal);

    canvasScroll_ = new QScrollArea(splitter);
    canvasScroll_->setWidgetResizable(false);
    canvasScroll_->setAlignment(Qt::AlignCenter);
    imageView_ = new QLabel(tr("Open an image to start"));
    imageView_->setAlignment(Qt::AlignCenter);
    imageView_->setMinimumSize(320, 280);
    imageView_->setStyleSheet(QStringLiteral("QLabel{background:#090b12;color:#737b94;border:0;font-size:16px;}"));
    canvasScroll_->setWidget(imageView_);
    splitter->addWidget(canvasScroll_);

    auto* sidePanel = new QWidget(splitter);
    auto* sideLayout = new QVBoxLayout(sidePanel);
    sideLayout->setContentsMargins(12, 12, 12, 12);
    sideLayout->setSpacing(10);

    auto* toolsTitle = new QLabel(tr("EDITING TOOLS"), sidePanel);
    toolsTitle->setStyleSheet(QStringLiteral("color:#b7a4f2;font-size:10px;font-weight:700;letter-spacing:1px;"));
    sideLayout->addWidget(toolsTitle);

    auto* expandButton = makeToolButton(tr("Pixel expand · 2×"), sidePanel);
    auto* rotateButton = makeToolButton(tr("Rotate 90°"), sidePanel);
    auto* flipHButton = makeToolButton(tr("Flip horizontal"), sidePanel);
    auto* flipVButton = makeToolButton(tr("Flip vertical"), sidePanel);
    auto* grayButton = makeToolButton(tr("Grayscale"), sidePanel);
    auto* invertButton = makeToolButton(tr("Invert colours"), sidePanel);
    auto* cropButton = makeToolButton(tr("Crop…"), sidePanel);
    sideLayout->addWidget(expandButton);
    sideLayout->addWidget(rotateButton);
    sideLayout->addWidget(flipHButton);
    sideLayout->addWidget(flipVButton);
    sideLayout->addWidget(grayButton);
    sideLayout->addWidget(invertButton);
    sideLayout->addWidget(cropButton);

    connect(expandButton, &QPushButton::clicked, this, [this] {
        if (image_.isNull()) return;
        if (image_.width() > 15000 || image_.height() > 15000) {
            QMessageBox::warning(this, tr("Image too large"), tr("The 2× result would exceed the editor's 30,000 pixel dimension limit."));
            return;
        }
        applyImageOperation("Pixel expand 2×", expand2x);
    });
    connect(rotateButton, &QPushButton::clicked, this, [this] { applyImageOperation("Rotate 90°", rotateImage); });
    connect(flipHButton, &QPushButton::clicked, this, [this] { applyImageOperation("Flip horizontal", flipHorizontal); });
    connect(flipVButton, &QPushButton::clicked, this, [this] { applyImageOperation("Flip vertical", flipVertical); });
    connect(grayButton, &QPushButton::clicked, this, [this] { applyImageOperation("Grayscale", grayscaleImage); });
    connect(invertButton, &QPushButton::clicked, this, [this] { applyImageOperation("Invert colours", invertImage); });
    connect(cropButton, &QPushButton::clicked, this, [this] { cropImage(); });

    sideLayout->addSpacing(8);
    auto* adjustmentTitle = new QLabel(tr("LIGHT & CONTRAST"), sidePanel);
    adjustmentTitle->setStyleSheet(QStringLiteral("color:#b7a4f2;font-size:10px;font-weight:700;letter-spacing:1px;"));
    sideLayout->addWidget(adjustmentTitle);

    sideLayout->addWidget(new QLabel(tr("Brightness"), sidePanel));
    brightnessSlider_ = new QSlider(Qt::Horizontal, sidePanel);
    brightnessSlider_->setRange(-100, 100);
    brightnessSlider_->setValue(0);
    sideLayout->addWidget(brightnessSlider_);
    sideLayout->addWidget(new QLabel(tr("Contrast"), sidePanel));
    contrastSlider_ = new QSlider(Qt::Horizontal, sidePanel);
    contrastSlider_->setRange(-100, 100);
    contrastSlider_->setValue(0);
    sideLayout->addWidget(contrastSlider_);
    auto* applyAdjustmentsButton = makeToolButton(tr("Apply adjustments"), sidePanel);
    sideLayout->addWidget(applyAdjustmentsButton);
    connect(applyAdjustmentsButton, &QPushButton::clicked, this, [this] { applyAdjustments(); });

    sideLayout->addSpacing(8);
    auto* zoomTitle = new QLabel(tr("CANVAS ZOOM"), sidePanel);
    zoomTitle->setStyleSheet(QStringLiteral("color:#b7a4f2;font-size:10px;font-weight:700;letter-spacing:1px;"));
    sideLayout->addWidget(zoomTitle);
    zoomSlider_ = new QSlider(Qt::Horizontal, sidePanel);
    zoomSlider_->setRange(25, 400);
    zoomSlider_->setValue(100);
    sideLayout->addWidget(zoomSlider_);
    connect(zoomSlider_, &QSlider::valueChanged, this, [this] { refreshCanvas(); });

    sideLayout->addSpacing(8);
    auto* layersTitle = new QLabel(tr("LAYERS"), sidePanel);
    layersTitle->setStyleSheet(QStringLiteral("color:#b7a4f2;font-size:10px;font-weight:700;letter-spacing:1px;"));
    sideLayout->addWidget(layersTitle);
    layerList_ = new QListWidget(sidePanel);
    layerList_->setMinimumHeight(90);
    layerList_->addItem(tr("Background · base image"));
    sideLayout->addWidget(layerList_, 1);
    sideLayout->addWidget(new QLabel(tr("Layer controls are being expanded in the next editor milestone."), sidePanel));

    splitter->addWidget(sidePanel);
    splitter->setStretchFactor(0, 4);
    splitter->setStretchFactor(1, 1);
    setCentralWidget(splitter);
}

void PhotoEditorWindow::openImage() {
    const QString path = QFileDialog::getOpenFileName(this, tr("Open image"), QString(),
        tr("Images (*.png *.jpg *.jpeg *.bmp *.webp *.gif *.tif *.tiff);;All files (*)"));
    if (path.isEmpty()) return;
    QImage loaded(path);
    if (loaded.isNull()) {
        QMessageBox::warning(this, tr("Cannot open image"), tr("The selected file could not be decoded."));
        return;
    }
    setImage(loaded);
    statusBar()->showMessage(tr("Opened %1").arg(QFileInfo(path).fileName()), 5000);
}

void PhotoEditorWindow::saveImage() {
    if (image_.isNull()) return;
    if (image_.text(QStringLiteral("trg.filePath")).isEmpty()) {
        saveImageAs();
        return;
    }
    const QString path = image_.text(QStringLiteral("trg.filePath"));
    if (!image_.save(path)) QMessageBox::warning(this, tr("Save failed"), tr("The image could not be saved."));
    else statusBar()->showMessage(tr("Saved %1").arg(QFileInfo(path).fileName()), 4000);
}

void PhotoEditorWindow::saveImageAs() {
    if (image_.isNull()) return;
    const QString path = QFileDialog::getSaveFileName(this, tr("Save image"), QString(),
        tr("PNG image (*.png);;JPEG image (*.jpg *.jpeg);;WebP image (*.webp);;BMP image (*.bmp)"));
    if (path.isEmpty()) return;
    if (!image_.save(path)) {
        QMessageBox::warning(this, tr("Save failed"), tr("The image could not be saved in that format."));
        return;
    }
    image_.setText(QStringLiteral("trg.filePath"), path);
    statusBar()->showMessage(tr("Saved %1").arg(QFileInfo(path).fileName()), 4000);
}

void PhotoEditorWindow::setImage(const QImage& image, bool recordHistory) {
    image_ = image.convertToFormat(QImage::Format_RGBA8888);
    if (recordHistory) {
        while (history_.size() > historyIndex_ + 1) history_.removeLast();
        history_.append(image_);
        historyIndex_ = history_.size() - 1;
    }
    refreshCanvas();
    refreshHistoryActions();
}

void PhotoEditorWindow::refreshCanvas() {
    if (image_.isNull()) return;
    const int zoom = zoomSlider_ ? zoomSlider_->value() : 100;
    const QSize displaySize(
        std::max(1, image_.width() * zoom / 100),
        std::max(1, image_.height() * zoom / 100)
    );
    imageView_->setPixmap(QPixmap::fromImage(image_).scaled(displaySize, Qt::KeepAspectRatio, Qt::FastTransformation));
    imageView_->resize(displaySize);
    imageInfo_ = imageView_;
    imageView_->setToolTip(tr("%1 × %2 pixels · %3% zoom").arg(image_.width()).arg(image_.height()).arg(zoom));
    statusBar()->showMessage(tr("%1 × %2 px · %3% zoom").arg(image_.width()).arg(image_.height()).arg(zoom));
    if (layerList_ && layerList_->count() > 0) {
        layerList_->item(0)->setText(tr("Background · %1 × %2").arg(image_.width()).arg(image_.height()));
    }
}

void PhotoEditorWindow::refreshHistoryActions() {
    const bool hasImage = !image_.isNull();
    if (menuBar()->actions().size() > 1) {
        auto* editMenu = menuBar()->actions().at(1)->menu();
        if (editMenu && editMenu->actions().size() >= 2) {
            editMenu->actions().at(0)->setEnabled(hasImage && historyIndex_ > 0);
            editMenu->actions().at(1)->setEnabled(hasImage && historyIndex_ + 1 < history_.size());
        }
    }
}

void PhotoEditorWindow::applyImageOperation(const char* label, const QImage& (*operation)(const QImage&)) {
    if (image_.isNull()) return;
    try {
        setImage(operation(image_));
        statusBar()->showMessage(tr("%1 · %2 × %3 px").arg(QString::fromUtf8(label)).arg(image_.width()).arg(image_.height()), 5000);
    } catch (const std::exception& error) {
        QMessageBox::warning(this, tr("Operation failed"), QString::fromUtf8(error.what()));
    }
}

void PhotoEditorWindow::applyAdjustments() {
    if (image_.isNull()) return;
    try {
        setImage(toQImage(toBuffer(image_).adjusted(brightnessSlider_->value(), contrastSlider_->value())));
        brightnessSlider_->setValue(0);
        contrastSlider_->setValue(0);
    } catch (const std::exception& error) {
        QMessageBox::warning(this, tr("Adjustment failed"), QString::fromUtf8(error.what()));
    }
}

void PhotoEditorWindow::cropImage() {
    if (image_.isNull()) return;
    bool ok = false;
    const int x = QInputDialog::getInt(this, tr("Crop image"), tr("Left position (x)"), 0, 0, image_.width() - 1, 1, &ok);
    if (!ok) return;
    const int y = QInputDialog::getInt(this, tr("Crop image"), tr("Top position (y)"), 0, 0, image_.height() - 1, 1, &ok);
    if (!ok) return;
    const int width = QInputDialog::getInt(this, tr("Crop image"), tr("Crop width"), image_.width() - x, 1, image_.width() - x, 1, &ok);
    if (!ok) return;
    const int height = QInputDialog::getInt(this, tr("Crop image"), tr("Crop height"), image_.height() - y, 1, image_.height() - y, 1, &ok);
    if (!ok) return;
    try {
        setImage(toQImage(toBuffer(image_).cropped(x, y, width, height)));
    } catch (const std::exception& error) {
        QMessageBox::warning(this, tr("Crop failed"), QString::fromUtf8(error.what()));
    }
}

void PhotoEditorWindow::undo() {
    if (historyIndex_ <= 0) return;
    --historyIndex_;
    setImage(history_.at(historyIndex_), false);
    refreshHistoryActions();
}

void PhotoEditorWindow::redo() {
    if (historyIndex_ + 1 >= history_.size()) return;
    ++historyIndex_;
    setImage(history_.at(historyIndex_), false);
    refreshHistoryActions();
}
