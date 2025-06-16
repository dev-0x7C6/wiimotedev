#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "ir.hpp"

int main(int argc, char *argv[]) {
	QGuiApplication app(argc, argv);
	VirtualCursor vc;
	return app.exec();
}
