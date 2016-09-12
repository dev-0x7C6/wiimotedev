#pragma once

#include <cstdint>
#include <qnamespace.h>
#include "linux/usr/include/wiimotedev/consts.h"

namespace io {
namespace interface {

class IWiimotedevEvent {
public:
	explicit IWiimotedevEvent() = default;
	virtual ~IWiimotedevEvent() = default;

	virtual void classicControllerButtons(quint32 id, quint64 buttons) = 0;
	virtual void classicControllerLStick(quint32 id, stickdata stick) = 0;
	virtual void classicControllerRStick(quint32 id, stickdata stick) = 0;
	virtual void nunchukButtons(quint32 id, quint64 buttons) = 0;
	virtual void nunchukStick(quint32 id, stickdata stick) = 0;
	virtual void wiimoteButtons(quint32 id, quint64 buttons) = 0;
	virtual void wiimoteGeneralButtons(quint32 id, quint64 buttons) = 0;
};
}
}
