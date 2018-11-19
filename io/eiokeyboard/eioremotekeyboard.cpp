#include "eioremotekeyboard.h"

using namespace io::emulation;

extern QMap<QString, uint> scancodes;

enum KeyboardExtension {
	keyboardExt = 0xFFFF,
	keyboardExtMouseWheelLeft,
	keyboardExtMouseWheelRight,
	keyboardExtMouseWheelUp,
	keyboardExtMouseWheelDown
};

EIORemoteKeyboard::EIORemoteKeyboard(EventDevice &device)
		: m_device(device) {
	compareType = HashCompare<u32, u64>::BitCompare;
}

EIORemoteKeyboard::~EIORemoteKeyboard() {
	disconnect(this, 0, 0, 0);
	for (const auto &action : keyboardActions)
		releaseKeyboardButtons(action->keys);

	keyboardActions.clear();
}

void EIORemoteKeyboard::setCompareType(QString type) {
	compareType = HashCompare<u32, u64>::BitCompare;

	if (type.toLower() == QString("bitCompare").toLower())
		compareType = HashCompare<u32, u64>::BitCompare;
	else if (type.toLower() == QString("equal").toLower())
		compareType = HashCompare<u32, u64>::EqualCompare;
	else if (type.toLower() == QString("notEqual").toLower())
		compareType = HashCompare<u32, u64>::NotEqualCompare;
}

void EIORemoteKeyboard::addKeyboardAction(KeyboardAction &action) {
	auto kbdAction = std::make_unique<KeyboardAction>();
	kbdAction->event = action.event;
	kbdAction->keys = action.keys;
	kbdAction->pushed = false;
	keyboardActions.emplace_back(std::move(kbdAction));
}

void EIORemoteKeyboard::clearKeyboardActions() {
	keyboardActions.clear();
}

void EIORemoteKeyboard::dbusWiimoteGeneralButtons(u32 id, u64 value) {
	if (value == buttons.value(id, -1))
		return;

	buttons[id] = value;
	HashCompare<u32, u64> compare;
	for (const auto &action : keyboardActions) {
		if (action->event.isEmpty())
			continue;

		bool matched = compare.compare(&action->event, &buttons, compareType);

		if (matched && !action->pushed) {
			action->pushed = !action->pushed;
			pressKeyboardButtons(action->keys);
		} else if (!matched && action->pushed) {
			action->pushed = !action->pushed;
			releaseKeyboardButtons(action->keys);
		}
	}
}

void EIORemoteKeyboard::pressKeyboardExtendedButton(u32 key) {
	switch (key) {
		case keyboardExtMouseWheelLeft:
			m_device.moveMouseHWheel(-1);
			break;

		case keyboardExtMouseWheelRight:
			m_device.moveMouseHWheel(1);
			break;

		case keyboardExtMouseWheelUp:
			m_device.moveMouseVWheel(1);
			break;

		case keyboardExtMouseWheelDown:
			m_device.moveMouseVWheel(-1);
			break;
	}
}

void EIORemoteKeyboard::releaseKeyboardExtendedButton(u32 key) {
	Q_UNUSED(key);
}

void EIORemoteKeyboard::pressKeyboardButtons(QList<uint> &list) {
	if (list.isEmpty())
		return;

	for (auto &&key : list) {
		if (key <= keyboardExt)
			m_device.pressKey(key);
		else
			pressKeyboardExtendedButton(key);
	}
}

void EIORemoteKeyboard::releaseKeyboardButtons(QList<uint> &list) {
	if (list.isEmpty())
		return;

	for (auto &&key : list)
		if (key <= keyboardExt)
			m_device.releaseKey(key);
		else
			releaseKeyboardExtendedButton(key);
}
