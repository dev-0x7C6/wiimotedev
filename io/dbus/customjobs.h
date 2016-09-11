#pragma once

#include <wiimotedev/consts.h>

#include <QDBusAbstractAdaptor>
#include <QDBusConnection>
#include <QDBusReply>
#include <QDBusMetaType>

#ifndef WIIMOTEDEV_MARSHALL
#include <QDBusArgument>
#endif

#ifndef WIIMOTEDEV_META_TYPES
#define WIIMOTEDEV_META_TYPES

Q_DECLARE_METATYPE(QList<irpoint>)
Q_DECLARE_METATYPE(QList<accdata>)
Q_DECLARE_METATYPE(QList<stickdata>)
Q_DECLARE_METATYPE(QList<uint>)

Q_DECLARE_METATYPE(irpoint)
Q_DECLARE_METATYPE(accdata)
Q_DECLARE_METATYPE(stickdata)

#endif

#ifndef WIIMOTEDEV_MARSHALL
#define WIIMOTEDEV_MARSHALL

QDBusArgument &operator<<(QDBusArgument &argument, const irpoint &point);
const QDBusArgument &operator>>(const QDBusArgument &argument, irpoint &point);
QDBusArgument &operator<<(QDBusArgument &argument, const accdata &acc);
const QDBusArgument &operator>>(const QDBusArgument &argument, accdata &acc);
QDBusArgument &operator<<(QDBusArgument &argument, const stickdata &stick);
const QDBusArgument &operator>>(const QDBusArgument &argument, stickdata &stick);

inline QDBusArgument &operator<<(QDBusArgument &argument, const irpoint &point) {
	argument.beginStructure();
	argument << point.size << point.x << point.y;
	argument.endStructure();
	return argument;
}

inline const QDBusArgument &operator>>(const QDBusArgument &argument, irpoint &point) {
	argument.beginStructure();
	argument >> point.size >> point.x >> point.y;
	argument.endStructure();
	return argument;
}

inline QDBusArgument &operator<<(QDBusArgument &argument, const accdata &acc) {
	argument.beginStructure();
	argument << acc.x << acc.y << acc.z << acc.pitch << acc.roll;
	argument.endStructure();
	return argument;
}

inline const QDBusArgument &operator>>(const QDBusArgument &argument, accdata &acc) {
	argument.beginStructure();
	argument >> acc.x >> acc.y >> acc.z >> acc.pitch >> acc.roll;
	argument.endStructure();
	return argument;
}

inline QDBusArgument &operator<<(QDBusArgument &argument, const stickdata &stick) {
	argument.beginStructure();
	argument << stick.x << stick.y;
	argument.endStructure();
	return argument;
}

inline const QDBusArgument &operator>>(const QDBusArgument &argument, stickdata &stick) {
	argument.beginStructure();
	argument >> stick.x >> stick.y;
	argument.endStructure();
	return argument;
}

#endif

class DBusCustomJobsAdaptor : public QDBusAbstractAdaptor {
	Q_OBJECT
	Q_CLASSINFO("D-Bus Interface", "org.wiimotedev.customJobs")
	Q_CLASSINFO("D-Bus Introspection", ""
									   "  <interface name=\"org.wiimotedev.customJobs\">\n"
									   "    <signal name=\"executeRequest\">\n"
									   "      <arg type=\"as\" direction=\"out\"/>\n"
									   "    </signal>"
									   "  </interface>\n"
									   "")
public:
	DBusCustomJobsAdaptor(QObject *parent);

Q_SIGNALS:
	void executeRequest(QStringList);
};

class DBusCustomJobsAdaptorWrapper : public QObject {
	Q_OBJECT
private:
	bool registred;

public:
	DBusCustomJobsAdaptorWrapper(QObject *parent, QDBusConnection connection);

	inline bool isRegistred() {
		return registred;
	}

	inline void slotExecuteRequest(QStringList params) {
		emit executeRequest(params);
	}

Q_SIGNALS:
	void executeRequest(QStringList);
};

inline DBusCustomJobsAdaptorWrapper::DBusCustomJobsAdaptorWrapper(QObject *parent, QDBusConnection connection)
		: QObject(parent) {
	new DBusCustomJobsAdaptor(this);
	registred = connection.registerObject("/customJobs", this);
}

inline DBusCustomJobsAdaptor::DBusCustomJobsAdaptor(QObject *parent)
		: QDBusAbstractAdaptor(parent) {
	setAutoRelaySignals(true);
}
