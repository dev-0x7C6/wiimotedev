#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <memory>

#include "balanceboard.hpp"

int main(int argc, char *argv[]) {
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QGuiApplication app(argc, argv);

	auto balanceboard = std::make_unique<BalanceBoard>();

	QQmlApplicationEngine engine;
	engine.rootContext()->setContextProperty("balanceboard", balanceboard.get());
	engine.load(QUrl(QLatin1String("qrc:/main.qml")));

	return app.exec();
}
