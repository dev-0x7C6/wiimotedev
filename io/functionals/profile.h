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

class Profile final : public QObject, public interface::IProfile, public interface::IWiimotedevEvent {
	Q_OBJECT
public:
	explicit Profile(const std::string &configurationFilePath);
	virtual ~Profile();

protected:
	virtual void connected(enums::Source, uint id) override;
	virtual void disconnected(enums::Source, uint id) override;
	virtual void buttonDataChanged(enums::Source, uint id, qulonglong mask) override;
	virtual void stickDataChanged(enums::Source, uint id, int lx, int ly, int rx, int ry) override;
	virtual void accelerometerDataChanged(enums::Source, uint id, int x, int y, int z) override;
	virtual void gyroscopeDataChanged(uint id, int x, int y, int z, int lowX, int lowY, int lowZ) override;
	virtual void infraredDataChanged(uint id, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) override;

protected:
	void gamepad_iterator(const interface::IGamepad::Type type, const quint32 id, std::function<void(const std::unique_ptr<interface::IGamepad> &)> &&function);
	bool setup(const interface::IGamepad::Type type, const std::string &name, quint32 id);

private:
	std::list<std::unique_ptr<io::interface::IGamepad>> m_gamepads;
};
}
}
