#include "eioremotekeyboard.h"

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
	compareType = HashCompare<uint32_t, uint64_t>::BitCompare;
}

EIORemoteKeyboard::~EIORemoteKeyboard() {
	disconnect(this, 0, 0, 0);
	for (const auto &action : keyboardActions)
		releaseKeyboardButtons(action->keys);

	keyboardActions.clear();
}

void EIORemoteKeyboard::setCompareType(QString type) {
	compareType = HashCompare<uint32_t, uint64_t>::BitCompare;

	if (type.toLower() == QString("bitCompare").toLower())
		compareType = HashCompare<uint32_t, uint64_t>::BitCompare;
	else if (type.toLower() == QString("equal").toLower())
		compareType = HashCompare<uint32_t, uint64_t>::EqualCompare;
	else if (type.toLower() == QString("notEqual").toLower())
		compareType = HashCompare<uint32_t, uint64_t>::NotEqualCompare;
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

void EIORemoteKeyboard::dbusWiimoteGeneralButtons(uint32_t id, uint64_t value) {
	if (value == buttons.value(id, -1))
		return;

	buttons[id] = value;
	HashCompare<uint32_t, uint64_t> compare;
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

void EIORemoteKeyboard::pressKeyboardExtendedButton(uint32_t key) {
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

void EIORemoteKeyboard::releaseKeyboardExtendedButton(uint32_t key) {
	Q_UNUSED(key);
}

void EIORemoteKeyboard::pressKeyboardButtons(QList<uint> &list) {
	if (list.isEmpty())
		return;

	foreach (const uint32_t key, list) {
		if (key <= keyboardExt)
			m_device.pressKey(key);
		else
			pressKeyboardExtendedButton(key);
	}
}

void EIORemoteKeyboard::releaseKeyboardButtons(QList<uint> &list) {
	if (list.isEmpty())
		return;

	foreach (const uint32_t key, list)

		if (key <= keyboardExt)
			m_device.releaseKey(key);
		else
			releaseKeyboardExtendedButton(key);
}
