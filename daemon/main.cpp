#include "../config.h"
#include <iostream>
#include <csignal>
#include <unistd.h>

#include <QCoreApplication>
#include <QTimer>

#include "wiimotedevcore.h"

using namespace dae::core;

int main(int argc, char *argv[]) {
	QCoreApplication application(argc, argv);
	QCoreApplication::setApplicationName("wiimotedev-daemon");
	QCoreApplication::setApplicationVersion(
		QString::number(WIIMOTEDEV_VERSION_MAJOR) + '.' +
		QString::number(WIIMOTEDEV_VERSION_MINOR) + '.' +
		QString::number(WIIMOTEDEV_VERSION_PATCH));

	if (getuid()) {
		std::cerr << "root privilages needed." << std::endl;
		return 1;
	}

	signal(SIGTERM, [](int) { qApp->quit(); });

	WiimotedevCore core;
	QTimer processTimer;
	QCoreApplication::connect(&processTimer, &QTimer::timeout, [&core]() { core.process(); });
	processTimer.start(1);

	return QCoreApplication::exec();
}
