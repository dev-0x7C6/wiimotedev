#define DAEMON_NAME "wiimotedev-daemon"
#define PID_FILE "/var/run/wiimotedev-daemon.pid"
#define PID_MODE 0644

#include "../config.h"
#include <fcntl.h>
#include <iostream>
#include <memory>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QCoreApplication>
#include <QFileInfo>

#include "wiimotedevcore.h"

using namespace service::core;

void signal_handler(int sig) {
	switch (sig) {
		case SIGHUP:
		case SIGTERM:
		case SIGINT:
		case SIGALRM:
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

		write(fd, QString::number(sid).toLocal8Bit().data(), QString::number(sid).length());
		close(fd);
		pid = sid;
	}

	if (parser.isSet(optionNoQuiet)) {
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		close(STDERR_FILENO);
	}

	signal(SIGHUP, signal_handler);
	signal(SIGALRM, signal_handler);
	signal(SIGTERM, signal_handler);
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, signal_handler);
	signal(SIGPIPE, signal_handler);
	std::unique_ptr<WiimotedevCore> core = std::make_unique<WiimotedevCore>();
	application.exec();

	if (getpid() == pid)
		unlink(PID_FILE);

	return 0;
}
