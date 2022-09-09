#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <memory>

#include "accelerometer.hpp"

int main(int argc, char *argv[]) {
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QGuiApplication app(argc, argv);

	auto accel = std::make_unique<Accelerometer>();

	QQmlApplicationEngine engine;
	engine.rootContext()->setContextProperty("accel", accel.get());
	engine.load(QUrl(QLatin1String("qrc:/main.qml")));

	return app.exec();
}
