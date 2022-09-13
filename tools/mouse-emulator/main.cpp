#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "ir.hpp"

int main(int argc, char *argv[]) {
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QGuiApplication app(argc, argv);

	VirtualCursor vc;
	return app.exec();
}
