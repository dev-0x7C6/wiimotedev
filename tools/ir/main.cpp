#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <memory>

#include "ir.hpp"

int main(int argc, char *argv[]) {
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QGuiApplication app(argc, argv);

	auto cursor = std::make_unique<VirtualCursor>();

	QQmlApplicationEngine engine;
	engine.rootContext()->setContextProperty("cursor", cursor.get());
	engine.load(QUrl(QLatin1String("qrc:/main.qml")));

	return app.exec();
}
