#include "../config.h"

#include <QCoreApplication>
#include <QTimer>

#include <csignal>
#include <atomic>
#include <spdlog/spdlog.h>

#include "wiimotedevcore.h"

static std::atomic_bool interrupted{false};

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

	if (getuid())
		spdlog::warn("core: dbus session will be user wise (non-root)");

	signal(SIGTERM, [](int) { interrupted = true; });

	WiimotedevCore core;
	QTimer processTimer;
	QCoreApplication::connect(&processTimer, &QTimer::timeout, [&core, &application]() {
		if (interrupted)
			application.quit();

		core.process();
	});
	processTimer.start(1);

	return QCoreApplication::exec();
}
