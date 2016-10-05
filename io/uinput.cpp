#include "../config.h"
#include <iostream>
#include <signal.h>

#include "eiomanager/manager.h"
#include "include/wiimotedev/wiimotedev"

#include <QCoreApplication>
#include <QFile>
#include <QFileInfo>
#include <QMap>
#include <QSettings>
#include <QStringList>
#include <QTextStream>

const QString scancodeFile("/etc/wiimotedev/scancode.conf");

QMap<QString, uint64_t> devicebuttons;
QMap<QString, uint> scancodes;

int main(int argc, char *argv[]) {
	QCoreApplication application(argc, argv);
	application.setApplicationName("wiimotedev-io");
	application.setApplicationVersion(
		QString::number(WIIMOTEDEV_VERSION_MAJOR) + '.' +
		QString::number(WIIMOTEDEV_VERSION_MINOR) + '.' +
		QString::number(WIIMOTEDEV_VERSION_PATCH));

	if (getuid()) {
		std::cout << "root privilages needed." << std::endl;
		return 1;
	}

	signal(SIGTERM, [](int) { qApp->quit(); });

	QSettings settings(scancodeFile, QSettings::IniFormat);
	settings.beginGroup("scancode");
	for (int32_t i = 0; i < settings.allKeys().count(); ++i)
		scancodes[QString(settings.allKeys().at(i)).toLower().remove(QChar(' '))] = settings.value(settings.allKeys().at(i), 0).toInt();
	settings.endGroup();

	UInputProfileManager profileManager;
	application.exec();
	return 0;
}
