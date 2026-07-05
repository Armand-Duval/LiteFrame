#include "editor_controller.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickWindow>
#include <QSurfaceFormat>
#include <QUrl>

int main(int argc, char* argv[]) {
    QSurfaceFormat format;
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    QSurfaceFormat::setDefaultFormat(format);

    QGuiApplication app(argc, argv);
    QGuiApplication::setApplicationName("LiteFrame Editor");
    QGuiApplication::setOrganizationName("LiteFrame");

    QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);

    EditorController controller;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("controller", &controller);
    engine.loadFromModule("LiteFrame", "Main");

    if (engine.rootObjects().isEmpty()) {
        engine.load(QUrl(QStringLiteral("qrc:/qt/qml/LiteFrame/qml/Main.qml")));
    }

    if (engine.rootObjects().isEmpty()) {
        engine.load(QUrl(QStringLiteral("qrc:/LiteFrame/qml/Main.qml")));
    }

    if (engine.rootObjects().isEmpty()) {
        return 1;
    }

    return app.exec();
}
