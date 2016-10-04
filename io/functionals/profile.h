#pragma once

#include <functional>
#include <list>
#include <memory>

#include <QObject>

#include "interfaces/igamepad.h"
#include "interfaces/iprofile.h"
#include "interfaces/iwiimotedev-event.h"

namespace io {
namespace functional {

class Profile : public QObject, public interface::IProfile, public interface::IWiimotedevEvent {
	Q_OBJECT
public:
	explicit Profile(const std::string &configurationFilePath);
	virtual ~Profile();

	virtual void classicControllerButtons(quint32 id, quint64 buttons) override;
//	virtual void classicControllerLStick(quint32 id, stickdata stick) override;
//	virtual void classicControllerRStick(quint32 id, stickdata stick) override;
	virtual void nunchukButtons(quint32 id, quint64 buttons) override;
//	virtual void nunchukStick(quint32 id, stickdata stick) override;
	virtual void wiimoteButtons(quint32 id, quint64 buttons) override;
	virtual void wiimoteGeneralButtons(quint32 id, quint64 buttons) override;

protected:
	void gamepad_iterator(const interface::IGamepad::Type type, const quint32 id, std::function<void(const std::unique_ptr<interface::IGamepad> &)> &&function);
	bool setup(const interface::IGamepad::Type type, const std::string &name, quint32 id);

private:
	std::list<std::unique_ptr<io::interface::IGamepad>> m_gamepads;
};
}
}
