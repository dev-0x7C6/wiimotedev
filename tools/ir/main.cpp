#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "infrared-model.hpp"
#include "virtual-cursor-model.hpp"

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    qmlRegisterType<VirtualCursorModel>("Wiimotedev", 2, 9, "VirtualCursorModel");
    qmlRegisterType<InfraredModel>("Wiimotedev", 2, 9, "InfraredModel");
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));

    return app.exec();
}
