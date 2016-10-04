#define DAEMON_NAME "wiimotedev[io]"
#define PID_FILE "/var/run/wiimotedev-io.pid"
#define PID_MODE 0644

#include "../config.h"
#include <fcntl.h>
#include <iostream>
#include <signal.h>
#include <stdlib.h>

#include "eiomanager/manager.h"
#include "linux/usr/include/wiimotedev/wiimotedev"

#include <QCoreApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QFile>
#include <QFileInfo>
#include <QMap>
#include <QSettings>
#include <QStringList>
#include <QTextStream>

const QString scancodeFile("/etc/wiimotedev/scancode.conf");

QMap<QString, uint64_t> devicebuttons;
QMap<QString, uint> scancodes;

void signal_handler(int sig) {
	switch (sig) {
		case SIGHUP:
		case SIGTERM:
		case SIGINT:
		case SIGQUIT:
			qApp->quit();
			break;

		case SIGPIPE:
			signal(SIGPIPE, signal_handler);
			break;
	}
}

int main(int argc, char *argv[]) {
	QCoreApplication application(argc, argv);
	application.setApplicationName(DAEMON_NAME);
	application.setApplicationVersion(
		QString::number(WIIMOTEDEV_VERSION_MAJOR) + '.' +
		QString::number(WIIMOTEDEV_VERSION_MINOR) + '.' +
		QString::number(WIIMOTEDEV_VERSION_PATCH));
	QCommandLineParser parser;
	parser.setApplicationDescription("Wiimotedev general service");
	parser.addHelpOption();
	parser.addVersionOption();
	QCommandLineOption optionNoQuiet({"q", "no-quiet"}, "do not block stdout messages");
	QCommandLineOption optionNoDaemon({"d", "no-daemon"}, "do not run in background");
	parser.addOption(optionNoDaemon);
	parser.addOption(optionNoQuiet);
	parser.process(application);

	if (getuid()) {
		std::cout << "root privilages needed." << std::endl;
		exit(EXIT_FAILURE);
	}

	pid_t pid;

	if (!parser.isSet(optionNoDaemon)) {
		pid = fork();

		if (pid < 0) exit(EXIT_FAILURE);

		if (pid > 0) exit(EXIT_SUCCESS);

		pid_t sid = setsid();

		if (sid < 0) exit(EXIT_FAILURE);

		if (chdir("/") < 0) exit(EXIT_FAILURE);

		int fd = open(PID_FILE, O_CREAT | O_WRONLY | O_SYNC, PID_MODE);

		if (!fd) exit(EXIT_FAILURE);

		write(fd, QString::number(sid).toLocal8Bit().constData(), QString::number(sid).length());
		close(fd);
	}

	if (parser.isSet(optionNoQuiet)) {
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		close(STDERR_FILENO);
	}

	signal(SIGHUP, signal_handler);
	signal(SIGTERM, signal_handler);
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, signal_handler);
	signal(SIGPIPE, signal_handler);
	QSettings settings(scancodeFile, QSettings::IniFormat);
	settings.beginGroup("scancode");

	for (int32_t i = 0; i < settings.allKeys().count(); ++i)
		scancodes[QString(settings.allKeys().at(i)).toLower().remove(QChar(' '))] = settings.value(settings.allKeys().at(i), 0).toInt();

	settings.endGroup();
	devicebuttons.insert("wiimote.1", WIIMOTE_BTN_1);
	devicebuttons.insert("wiimote.2", WIIMOTE_BTN_2);
	devicebuttons.insert("wiimote.a", WIIMOTE_BTN_A);
	devicebuttons.insert("wiimote.b", WIIMOTE_BTN_B);
	devicebuttons.insert("wiimote.plus", WIIMOTE_BTN_PLUS);
	devicebuttons.insert("wiimote.minus", WIIMOTE_BTN_MINUS);
	devicebuttons.insert("wiimote.home", WIIMOTE_BTN_HOME);
	devicebuttons.insert("wiimote.up", WIIMOTE_BTN_UP);
	devicebuttons.insert("wiimote.down", WIIMOTE_BTN_DOWN);
	devicebuttons.insert("wiimote.left", WIIMOTE_BTN_LEFT);
	devicebuttons.insert("wiimote.right", WIIMOTE_BTN_RIGHT);
	devicebuttons.insert("wiimote.shift.up", WIIMOTE_BTN_SHIFT_UP);
	devicebuttons.insert("wiimote.shift.down", WIIMOTE_BTN_SHIFT_DOWN);
	devicebuttons.insert("wiimote.shift.left", WIIMOTE_BTN_SHIFT_LEFT);
	devicebuttons.insert("wiimote.shift.right", WIIMOTE_BTN_SHIFT_RIGHT);
	devicebuttons.insert("wiimote.shift.forward", WIIMOTE_BTN_SHIFT_FORWARD);
	devicebuttons.insert("wiimote.shift.backward", WIIMOTE_BTN_SHIFT_BACKWARD);
	devicebuttons.insert("wiimote.tilt.front", WIIMOTE_BTN_TILT_FRONT);
	devicebuttons.insert("wiimote.tilt.back", WIIMOTE_BTN_TILT_BACK);
	devicebuttons.insert("wiimote.tilt.left", WIIMOTE_BTN_TILT_LEFT);
	devicebuttons.insert("wiimote.tilt.right", WIIMOTE_BTN_TILT_RIGHT);
	devicebuttons.insert("nunchuk.z", NUNCHUK_BTN_Z);
	devicebuttons.insert("nunchuk.c", NUNCHUK_BTN_C);
	devicebuttons.insert("nunchuk.stick.up", NUNCHUK_BTN_STICK_UP);
	devicebuttons.insert("nunchuk.stick.down", NUNCHUK_BTN_STICK_DOWN);
	devicebuttons.insert("nunchuk.stick.left", NUNCHUK_BTN_STICK_LEFT);
	devicebuttons.insert("nunchuk.stick.right", NUNCHUK_BTN_STICK_RIGHT);
	devicebuttons.insert("nunchuk.shift.up", NUNCHUK_BTN_SHIFT_UP);
	devicebuttons.insert("nunchuk.shift.down", NUNCHUK_BTN_SHIFT_DOWN);
	devicebuttons.insert("nunchuk.shift.left", NUNCHUK_BTN_SHIFT_LEFT);
	devicebuttons.insert("nunchuk.shift.right", NUNCHUK_BTN_SHIFT_RIGHT);
	devicebuttons.insert("nunchuk.shift.forward", NUNCHUK_BTN_SHIFT_FORWARD);
	devicebuttons.insert("nunchuk.shift.backward", NUNCHUK_BTN_SHIFT_BACKWARD);
	devicebuttons.insert("nunchuk.tilt.front", NUNCHUK_BTN_TILT_FRONT);
	devicebuttons.insert("nunchuk.tilt.back", NUNCHUK_BTN_TILT_BACK);
	devicebuttons.insert("nunchuk.tilt.left", NUNCHUK_BTN_TILT_LEFT);
	devicebuttons.insert("nunchuk.tilt.right", NUNCHUK_BTN_TILT_RIGHT);
	devicebuttons.insert("classic.x", CLASSIC_BTN_X);
	devicebuttons.insert("classic.y", CLASSIC_BTN_Y);
	devicebuttons.insert("classic.a", CLASSIC_BTN_A);
	devicebuttons.insert("classic.b", CLASSIC_BTN_B);
	devicebuttons.insert("classic.l", CLASSIC_BTN_L);
	devicebuttons.insert("classic.r", CLASSIC_BTN_R);
	devicebuttons.insert("classic.zl", CLASSIC_BTN_ZL);
	devicebuttons.insert("classic.zr", CLASSIC_BTN_ZR);
	devicebuttons.insert("classic.minus", CLASSIC_BTN_MINUS);
	devicebuttons.insert("classic.plus", CLASSIC_BTN_PLUS);
	devicebuttons.insert("classic.home", CLASSIC_BTN_HOME);
	devicebuttons.insert("classic.up", CLASSIC_BTN_UP);
	devicebuttons.insert("classic.down", CLASSIC_BTN_DOWN);
	devicebuttons.insert("classic.left", CLASSIC_BTN_LEFT);
	devicebuttons.insert("classic.right", CLASSIC_BTN_RIGHT);
	devicebuttons.insert("classic.lstick.up", CLASSIC_BTN_LSTICK_UP);
	devicebuttons.insert("classic.lstick.down", CLASSIC_BTN_LSTICK_DOWN);
	devicebuttons.insert("classic.lstick.left", CLASSIC_BTN_LSTICK_LEFT);
	devicebuttons.insert("classic.lstick.right", CLASSIC_BTN_LSTICK_RIGHT);
	devicebuttons.insert("classic.rstick.up", CLASSIC_BTN_RSTICK_UP);
	devicebuttons.insert("classic.rstick.down", CLASSIC_BTN_RSTICK_DOWN);
	devicebuttons.insert("classic.rstick.left", CLASSIC_BTN_RSTICK_LEFT);
	devicebuttons.insert("classic.rstick.right", CLASSIC_BTN_RSTICK_RIGHT);
	devicebuttons.insert("wiimote.shift.shake", WIIMOTE_BTN_SHIFT_SHAKE);
	devicebuttons.insert("nunchuk.shift.shake", NUNCHUK_BTN_SHIFT_SHAKE);
	UInputProfileManager profileManager;
	application.exec();
	exit(EXIT_SUCCESS);
}
