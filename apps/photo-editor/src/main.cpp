#include "PhotoEditorWindow.h"

#include <QApplication>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    QApplication::setApplicationName(QStringLiteral("TRG Photo Editor"));
    QApplication::setOrganizationName(QStringLiteral("TechRiverGames"));

    PhotoEditorWindow window;
    window.show();
    return app.exec();
}
