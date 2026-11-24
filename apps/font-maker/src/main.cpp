#include "FontMakerWindow.h"

#include <QApplication>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    QApplication::setApplicationName(QStringLiteral("TRG Font Maker"));
    QApplication::setOrganizationName(QStringLiteral("TechRiverGames"));

    FontMakerWindow window;
    window.show();
    return app.exec();
}
