#pragma once

#include <QMap>
#include <QTime>
#include <QTimer>

#include <memory>

#include "io/emulation/event-device.h"
#include "io/functionals/hash-compare.h"
#include "include/wiimotedev/wiimotedev"

struct KeyboardAction {
	QHash<u32, u64> event;
	QList<uint> keys;
	bool pushed;
};

class EIORemoteKeyboard : public QObject {
	Q_OBJECT
public:
	EIORemoteKeyboard(io::emulation::EventDevice &device);
	~EIORemoteKeyboard();

	void addKeyboardAction(KeyboardAction &);
	void clearKeyboardActions();
	void setCompareType(QString);

public slots:
	void dbusWiimoteGeneralButtons(u32, u64);

private:
	void pressKeyboardButtons(QList<uint> &);
	void releaseKeyboardButtons(QList<uint> &);
	void pressKeyboardExtendedButton(u32);
	void releaseKeyboardExtendedButton(u32);

private:
	io::emulation::EventDevice &m_device;
	u32 id;
	u32 compareType;

	QHash<u32, u64> buttons;

	std::list<std::unique_ptr<KeyboardAction>> keyboardActions;
};
