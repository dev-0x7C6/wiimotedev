#include "../config.h"
#include <iostream>
#include <csignal>
#include <unistd.h>

#include <QCoreApplication>
#include <QTimer>

#include "wiimotedevcore.h"

#include <spdlog/spdlog.h>

using namespace dae::core;

int main(int argc, char *argv[]) {
	QCoreApplication application(argc, argv);
	QCoreApplication::setApplicationName("wiimotedev-daemon");
	QCoreApplication::setApplicationVersion(
		QString::number(WIIMOTEDEV_VERSION_MAJOR) + '.' +
		QString::number(WIIMOTEDEV_VERSION_MINOR) + '.' +
		QString::number(WIIMOTEDEV_VERSION_PATCH));

	spdlog::set_level(spdlog::level::debug);
	spdlog::set_pattern("[%^%l%$] %v");

	if (getuid()) {
		spdlog::warn("core: dbus session will be user wise (non-root)");
	}

	signal(SIGTERM, [](int) { qApp->quit(); });

	WiimotedevCore core;
	QTimer processTimer;
	QCoreApplication::connect(&processTimer, &QTimer::timeout, [&core]() { core.process(); });
	processTimer.start(1);

	return QCoreApplication::exec();
}
