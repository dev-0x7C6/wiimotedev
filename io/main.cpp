#include "../config.h"
#include <iostream>
#include <signal.h>

#include "eiomanager/manager.h"
#include "include/wiimotedev/wiimotedev"
#include "core/profile-manager.h"

#include <QCoreApplication>
#include <QMap>

//const QString scancodeFile("/etc/wiimotedev/scancode.conf");

QMap<QString, u64> devicebuttons;
QMap<QString, uint> scancodes;

using namespace io::core;

int main(int argc, char *argv[]) {
	QCoreApplication application(argc, argv);
	application.setApplicationName("wiimotedev-io");
	application.setApplicationVersion(
		QString::number(WIIMOTEDEV_VERSION_MAJOR) + '.' +
		QString::number(WIIMOTEDEV_VERSION_MINOR) + '.' +
		QString::number(WIIMOTEDEV_VERSION_PATCH));

	if (getuid()) {
		std::cerr << "root privilages needed." << std::endl;
		return 1;
	}

	signal(SIGTERM, [](int) { qApp->quit(); });

	//	QSettings settings(scancodeFile, QSettings::IniFormat);
	//	settings.beginGroup("scancode");
	//	for (i32 i = 0; i < settings.allKeys().count(); ++i)
	//		scancodes[QString(settings.allKeys().at(i)).toLower().remove(QChar(' '))] = settings.value(settings.allKeys().at(i), 0).toInt();
	//	settings.endGroup();

	ProfileManager profileManager;
	application.exec();
	return 0;
}
