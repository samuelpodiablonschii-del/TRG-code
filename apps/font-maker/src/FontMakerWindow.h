#pragma once

#include "FontCanvas.h"
#include "FontProject.h"

#include <QMainWindow>

using trg::font::FontProject;

class QLabel;
class QLineEdit;
class QListWidget;
class QSpinBox;

class FontMakerWindow final : public QMainWindow {
public:
    explicit FontMakerWindow(QWidget* parent = nullptr);

private:
    FontProject project_;
    FontCanvas* canvas_{nullptr};
    QListWidget* glyphList_{nullptr};
    QLineEdit* familyEdit_{nullptr};
    QLineEdit* styleEdit_{nullptr};
    QLineEdit* characterEdit_{nullptr};
    QSpinBox* advanceSpin_{nullptr};
    QLabel* selectedGlyphLabel_{nullptr};
    int currentGlyphIndex_{-1};

    void buildInterface();
    void refreshGlyphList();
    void selectGlyph(int index);
    void addGlyph();
    void clearOutline();
    void saveProject();
    void exportFont();
};
