#include "../config.h"
#include <iostream>
#include <memory>
#include <csignal>
#include <unistd.h>

#include <QCoreApplication>
#include "wiimotedevcore.h"

using namespace dae::core;

int main(int argc, char *argv[]) {
	QCoreApplication application(argc, argv);
	application.setApplicationName("wiimotedev-daemon");
	application.setApplicationVersion(
		QString::number(WIIMOTEDEV_VERSION_MAJOR) + '.' +
		QString::number(WIIMOTEDEV_VERSION_MINOR) + '.' +
		QString::number(WIIMOTEDEV_VERSION_PATCH));

	if (getuid()) {
		std::cerr << "root privilages needed." << std::endl;
		return 1;
	}

	signal(SIGTERM, [](int) { qApp->quit(); });

	std::unique_ptr<WiimotedevCore> core = std::make_unique<WiimotedevCore>();
	application.exec();

	return 0;
}
