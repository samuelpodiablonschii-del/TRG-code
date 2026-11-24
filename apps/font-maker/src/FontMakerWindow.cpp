#include "FontMakerWindow.h"
#include "TrueTypeWriter.h"

#include <QApplication>
#include <QFileDialog>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QSaveFile>
#include <QScrollArea>
#include <QSpinBox>
#include <QSplitter>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QWidget>

#include <algorithm>

using namespace trg::font;

namespace {

QPushButton* toolButton(const QString& label, QWidget* parent) {
    auto* button = new QPushButton(label, parent);
    button->setMinimumHeight(42);
    button->setCursor(Qt::PointingHandCursor);
    return button;
}

QString glyphLabel(std::uint32_t codepoint) {
    const char32_t character = static_cast<char32_t>(codepoint);
    const QString glyph = QString::fromUcs4(&character, 1);
    return QStringLiteral("%1   U+%2").arg(glyph.isEmpty() ? QStringLiteral("□") : glyph)
        .arg(codepoint, 4, 16, QLatin1Char('0')).toUpper();
}

}

FontMakerWindow::FontMakerWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle(tr("TRG Font Maker"));
    resize(1280, 820);
    setMinimumSize(360, 540);
    setStyleSheet(QStringLiteral(
        "QMainWindow,QWidget{background:#10121d;color:#eceaf5;font-family:'Segoe UI',sans-serif;}"
        "QMenuBar,QMenu,QToolBar{background:#171a29;color:#e9e5f5;border:0;}"
        "QMenu::item{padding:7px 25px;} QMenu::item:selected{background:#393052;}"
        "QPushButton{background:#202438;border:1px solid #343951;border-radius:8px;padding:8px 12px;color:#e9e5f5;}"
        "QPushButton:hover{background:#2b2d47;border-color:#8d75d8;}"
        "QLineEdit,QSpinBox,QListWidget{background:#0b0d16;border:1px solid #30344a;border-radius:7px;padding:8px;color:#f4f3fb;}"
        "QListWidget::item{padding:8px;border-radius:5px;} QListWidget::item:selected{background:#393052;}"
        "QStatusBar{background:#171a29;color:#a7a4b8;}"
    ));

    FontGlyph notdef;
    notdef.codepoint = 0;
    notdef.advanceWidth = 600;
    notdef.contours = {{{{80, 0, true}, {520, 0, true}, {520, 700, true}, {80, 700, true}}}};
    project_.glyphs.push_back(notdef);
    FontGlyph space;
    space.codepoint = 32;
    space.advanceWidth = 300;
    project_.glyphs.push_back(space);

    buildInterface();
    refreshGlyphList();
    selectGlyph(0);
}

void FontMakerWindow::buildInterface() {
    auto* toolbar = addToolBar(tr("Font tools"));
    toolbar->setMovable(false);
    auto* addAction = toolbar->addAction(tr("Add glyph"));
    auto* saveAction = toolbar->addAction(tr("Save project"));
    auto* exportAction = toolbar->addAction(tr("Export TTF"));
    connect(addAction, &QAction::triggered, this, [this] { addGlyph(); });
    connect(saveAction, &QAction::triggered, this, [this] { saveProject(); });
    connect(exportAction, &QAction::triggered, this, [this] { exportFont(); });

    auto* splitter = new QSplitter(this);
    const bool narrow = QApplication::primaryScreen() &&
                        QApplication::primaryScreen()->availableGeometry().width() < 720;
    splitter->setOrientation(narrow ? Qt::Vertical : Qt::Horizontal);

    glyphList_ = new QListWidget(splitter);
    glyphList_->setMinimumWidth(130);
    connect(glyphList_, &QListWidget::currentRowChanged, this, [this](int row) { selectGlyph(row); });
    splitter->addWidget(glyphList_);

    canvas_ = new FontCanvas(splitter);
    canvas_->setContourChanged([this](const GlyphContour& contour) {
        if (currentGlyphIndex_ < 0 || currentGlyphIndex_ >= static_cast<int>(project_.glyphs.size())) return;
        auto& glyph = project_.glyphs[static_cast<std::size_t>(currentGlyphIndex_)];
        glyph.contours.clear();
        if (!contour.points.empty()) glyph.contours.push_back(contour);
        statusBar()->showMessage(tr("Outline updated · %1 points").arg(contour.points.size()), 2500);
    });
    splitter->addWidget(canvas_);

    auto* controls = new QWidget(splitter);
    controls->setMinimumWidth(210);
    auto* layout = new QVBoxLayout(controls);
    layout->setContentsMargins(13, 13, 13, 13);
    layout->setSpacing(10);

    auto* familyLabel = new QLabel(tr("FAMILY NAME"), controls);
    familyLabel->setStyleSheet(QStringLiteral("color:#b7a4f2;font-size:10px;font-weight:700;"));
    layout->addWidget(familyLabel);
    familyEdit_ = new QLineEdit(QString::fromStdString(project_.familyName), controls);
    layout->addWidget(familyEdit_);
    connect(familyEdit_, &QLineEdit::textChanged, this, [this](const QString& text) {
        project_.familyName = text.toStdString();
    });

    auto* styleLabel = new QLabel(tr("STYLE"), controls);
    styleLabel->setStyleSheet(QStringLiteral("color:#b7a4f2;font-size:10px;font-weight:700;"));
    layout->addWidget(styleLabel);
    styleEdit_ = new QLineEdit(QString::fromStdString(project_.styleName), controls);
    layout->addWidget(styleEdit_);
    connect(styleEdit_, &QLineEdit::textChanged, this, [this](const QString& text) {
        project_.styleName = text.toStdString();
    });

    auto* glyphLabelWidget = new QLabel(tr("SELECTED GLYPH"), controls);
    glyphLabelWidget->setStyleSheet(QStringLiteral("color:#b7a4f2;font-size:10px;font-weight:700;"));
    layout->addWidget(glyphLabelWidget);
    selectedGlyphLabel_ = new QLabel(tr("No glyph selected"), controls);
    selectedGlyphLabel_->setStyleSheet(QStringLiteral("font-size:18px;font-weight:700;color:#e7ddff;"));
    layout->addWidget(selectedGlyphLabel_);

    characterEdit_ = new QLineEdit(QStringLiteral("A"), controls);
    characterEdit_->setMaxLength(2);
    characterEdit_->setPlaceholderText(tr("Character"));
    layout->addWidget(characterEdit_);
    auto* addGlyphButton = toolButton(tr("Create / select glyph"), controls);
    layout->addWidget(addGlyphButton);
    connect(addGlyphButton, &QPushButton::clicked, this, [this] { addGlyph(); });

    auto* advanceLabel = new QLabel(tr("ADVANCE WIDTH"), controls);
    advanceLabel->setStyleSheet(QStringLiteral("color:#b7a4f2;font-size:10px;font-weight:700;"));
    layout->addWidget(advanceLabel);
    advanceSpin_ = new QSpinBox(controls);
    advanceSpin_->setRange(0, 2000);
    advanceSpin_->setValue(600);
    layout->addWidget(advanceSpin_);
    connect(advanceSpin_, &QSpinBox::valueChanged, this, [this](int value) {
        if (currentGlyphIndex_ >= 0 && currentGlyphIndex_ < static_cast<int>(project_.glyphs.size())) {
            project_.glyphs[static_cast<std::size_t>(currentGlyphIndex_)].advanceWidth = static_cast<std::uint16_t>(value);
        }
    });

    auto* closeContourButton = toolButton(tr("Close current contour"), controls);
    auto* clearButton = toolButton(tr("Clear outline"), controls);
    auto* saveButton = toolButton(tr("Save .trgfont"), controls);
    auto* exportButton = toolButton(tr("Export .ttf"), controls);
    layout->addWidget(closeContourButton);
    layout->addWidget(clearButton);
    layout->addWidget(saveButton);
    layout->addWidget(exportButton);
    connect(closeContourButton, &QPushButton::clicked, this, [this] {
        if (canvas_->contour().points.size() < 3) {
            QMessageBox::information(this, tr("Outline needs more points"), tr("Add at least three points to make a closed glyph contour."));
        } else {
            statusBar()->showMessage(tr("Contour closed automatically when exported."), 3000);
        }
    });
    connect(clearButton, &QPushButton::clicked, this, [this] { clearOutline(); });
    connect(saveButton, &QPushButton::clicked, this, [this] { saveProject(); });
    connect(exportButton, &QPushButton::clicked, this, [this] { exportFont(); });

    layout->addStretch(1);
    auto* installNote = new QLabel(
        tr("Android note: export the TTF and use your device's supported font settings or font manager if available. System-wide font changes are device-dependent."),
        controls);
    installNote->setWordWrap(true);
    installNote->setStyleSheet(QStringLiteral("color:#8e94a8;font-size:10px;line-height:1.5;"));
    layout->addWidget(installNote);

    splitter->addWidget(controls);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 4);
    splitter->setStretchFactor(2, 1);
    setCentralWidget(splitter);
}

void FontMakerWindow::refreshGlyphList() {
    const int selected = currentGlyphIndex_;
    glyphList_->clear();
    for (const auto& glyph : project_.glyphs) glyphList_->addItem(glyphLabel(glyph.codepoint));
    if (selected >= 0 && selected < glyphList_->count()) glyphList_->setCurrentRow(selected);
}

void FontMakerWindow::selectGlyph(int index) {
    if (index < 0 || index >= static_cast<int>(project_.glyphs.size())) return;
    currentGlyphIndex_ = index;
    const auto& glyph = project_.glyphs[static_cast<std::size_t>(index)];
    selectedGlyphLabel_->setText(glyphLabel(glyph.codepoint));
    advanceSpin_->setValue(glyph.advanceWidth);
    if (glyph.contours.empty()) canvas_->setContour({});
    else canvas_->setContour(glyph.contours.front());
    const char32_t character = static_cast<char32_t>(glyph.codepoint);
    characterEdit_->setText(QString::fromUcs4(&character, 1));
}

void FontMakerWindow::addGlyph() {
    const QVector<uint> codepoints = characterEdit_->text().toUcs4();
    if (codepoints.isEmpty()) {
        QMessageBox::information(this, tr("Enter a character"), tr("Enter one character to create a glyph."));
        return;
    }
    if (codepoints.front() > 0xffff || (codepoints.front() >= 0xd800 && codepoints.front() <= 0xdfff)) {
        QMessageBox::information(this, tr("Character not supported yet"), tr("This first exporter supports characters in the Unicode Basic Multilingual Plane."));
        return;
    }
    const std::uint32_t codepoint = codepoints.front();
    auto found = std::find_if(project_.glyphs.begin(), project_.glyphs.end(),
        [codepoint](const FontGlyph& glyph) { return glyph.codepoint == codepoint; });
    if (found == project_.glyphs.end()) {
        FontGlyph glyph;
        glyph.codepoint = codepoint;
        glyph.advanceWidth = 600;
        project_.glyphs.push_back(glyph);
        std::sort(project_.glyphs.begin(), project_.glyphs.end(),
            [](const FontGlyph& a, const FontGlyph& b) { return a.codepoint < b.codepoint; });
    }
    const auto index = std::find_if(project_.glyphs.begin(), project_.glyphs.end(),
        [codepoint](const FontGlyph& glyph) { return glyph.codepoint == codepoint; });
    currentGlyphIndex_ = static_cast<int>(std::distance(project_.glyphs.begin(), index));
    refreshGlyphList();
    glyphList_->setCurrentRow(currentGlyphIndex_);
}

void FontMakerWindow::clearOutline() {
    if (currentGlyphIndex_ < 0 || currentGlyphIndex_ >= static_cast<int>(project_.glyphs.size())) return;
    project_.glyphs[static_cast<std::size_t>(currentGlyphIndex_)].contours.clear();
    canvas_->clearContour();
}

void FontMakerWindow::saveProject() {
    const QString path = QFileDialog::getSaveFileName(this, tr("Save font project"), QString(),
        tr("TRG Font Project (*.trgfont);;JSON (*.json)"));
    if (path.isEmpty()) return;

    QJsonObject root;
    root.insert(QStringLiteral("format"), QStringLiteral("TRGFontProject"));
    root.insert(QStringLiteral("version"), 1);
    root.insert(QStringLiteral("familyName"), QString::fromStdString(project_.familyName));
    root.insert(QStringLiteral("styleName"), QString::fromStdString(project_.styleName));
    root.insert(QStringLiteral("unitsPerEm"), project_.unitsPerEm);
    root.insert(QStringLiteral("ascender"), project_.ascender);
    root.insert(QStringLiteral("descender"), project_.descender);

    QJsonArray glyphs;
    for (const auto& glyph : project_.glyphs) {
        QJsonObject glyphObject;
        glyphObject.insert(QStringLiteral("codepoint"), static_cast<int>(glyph.codepoint));
        glyphObject.insert(QStringLiteral("advanceWidth"), glyph.advanceWidth);
        glyphObject.insert(QStringLiteral("leftSideBearing"), glyph.leftSideBearing);
        QJsonArray contours;
        for (const auto& contour : glyph.contours) {
            QJsonArray points;
            for (const auto& point : contour.points) {
                QJsonArray value;
                value.append(point.x); value.append(point.y); value.append(point.onCurve);
                points.append(value);
            }
            contours.append(points);
        }
        glyphObject.insert(QStringLiteral("contours"), contours);
        glyphs.append(glyphObject);
    }
    root.insert(QStringLiteral("glyphs"), glyphs);

    QSaveFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, tr("Save failed"), tr("Could not create the project file."));
        return;
    }
    file.write(QJsonDocument(root).toJson(QJsonDocument::Indented));
    if (!file.commit()) {
        QMessageBox::warning(this, tr("Save failed"), tr("Could not finish writing the project file."));
        return;
    }
    statusBar()->showMessage(tr("Saved %1").arg(QFileInfo(path).fileName()), 4000);
}

void FontMakerWindow::exportFont() {
    const QString path = QFileDialog::getSaveFileName(this, tr("Export TrueType font"), project_.familyName.empty()
        ? QStringLiteral("TRG-Custom.ttf") : QString::fromStdString(project_.familyName) + QStringLiteral(".ttf"),
        tr("TrueType font (*.ttf)"));
    if (path.isEmpty()) return;
    std::string error;
    if (!exportTrueType(project_, path.toStdString(), &error)) {
        QMessageBox::warning(this, tr("Font export failed"), QString::fromStdString(error));
        return;
    }
    QMessageBox::information(this, tr("Font exported"),
        tr("The TTF file was exported successfully. Android system-wide font installation depends on your device manufacturer and supported font settings."));
    statusBar()->showMessage(tr("Exported %1").arg(QFileInfo(path).fileName()), 5000);
}
