#include "cwiid-controller.h"

#include "static/libcwiid/cwiid.h"

using namespace daemon::api;

bool CwiidController::isRumbleSupported() {
	return true;
}

bool CwiidController::isLedSupported() {
	return true;
}

bool CwiidController::isInfraredSupported() {
	return true;
}

uint8_t CwiidController::batteryStatus() {
	return 0;
}

bool CwiidController::ledStatus(const uint32_t id) {
	static_cast<void>(id);
	return 0;
}

bool CwiidController::rumbleStatus() {
	return false;
}

bool CwiidController::setLedStatus(const uint32_t id, const bool status) {
	static_cast<void>(id);
	static_cast<void>(status);
	return false;
}

bool CwiidController::setRumbleStatus(const bool rumble) {
	static_cast<void>(rumble);
	return false;
}

bool CwiidController::hasClassicExtension() {
	return false;
}

bool CwiidController::hasMotionPlusExtension() {
	return false;
}

bool CwiidController::hasNunchukExtension() {
	return false;
}
