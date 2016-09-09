#pragma once

#include <QMap>
#include <QTime>
#include <QTimer>

#include <memory>

#include "io/emulation/event-device.h"
#include "io/functionals/hash-compare.h"
#include "linux/usr/include/wiimotedev/consts.h"

struct KeyboardAction {
	QHash<uint32_t, uint64_t> event;
	QList<uint> keys;
	bool pushed;
};

class EIORemoteKeyboard : public QObject {
	Q_OBJECT
public:
	EIORemoteKeyboard(EventDevice &device);
	~EIORemoteKeyboard();

	void addKeyboardAction(KeyboardAction &);
	void clearKeyboardActions();
	void setCompareType(QString);

public slots:
	void dbusWiimoteGeneralButtons(uint32_t, uint64_t);

private:
	void pressKeyboardButtons(QList<uint> &);
	void releaseKeyboardButtons(QList<uint> &);
	void pressKeyboardExtendedButton(uint32_t);
	void releaseKeyboardExtendedButton(uint32_t);

private:
	EventDevice &m_device;
	uint32_t id;
	uint32_t compareType;

	QHash<uint32_t, uint64_t> buttons;

	std::list<std::unique_ptr<KeyboardAction>> keyboardActions;
};
