#include "cwiid-api.h"

#include "static/libcwiid/cwiid.h"

using namespace daemon::api;

bool CwiidApi::isRumbleSupported() {
	return true;
}

bool CwiidApi::isLedSupported() {
	return true;
}

bool CwiidApi::isInfraredSupported() {
	return true;
}

uint8_t CwiidApi::batteryStatus() {
	return 0;
}

bool CwiidApi::ledStatus(const uint32_t id) {
	static_cast<void>(id);
	return 0;
}

bool CwiidApi::rumbleStatus() {
	return false;
}

bool CwiidApi::setLedStatus(const uint32_t id, const bool status) {
	static_cast<void>(id);
	static_cast<void>(status);
	return false;
}

bool CwiidApi::setRumbleStatus(const bool rumble) {
	static_cast<void>(rumble);
	return false;
}

bool CwiidApi::hasClassicExtension() {
	return false;
}

bool CwiidApi::hasMotionPlusExtension() {
	return false;
}

bool CwiidApi::hasNunchukExtension() {
	return false;
}
