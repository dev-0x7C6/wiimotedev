#pragma once

#include <functional>
#include <list>
#include <memory>

#include <QObject>

#include "interfaces/igamepad.h"
#include "interfaces/imouse.h"
#include "interfaces/iprofile.h"
#include "interfaces/iwiimotedev-event.h"

namespace io {
namespace functional {

class Profile final : public QObject, public interface::IProfile {
	Q_OBJECT
public:
	explicit Profile(const std::string &configurationFilePath);
	~Profile() override;

	void connected(common::enums::Device, uint id) override;
	void disconnected(common::enums::Device, uint id) override;
	void found(common::enums::Device, uint id) override;
	void lost(common::enums::Device, uint id) override;
	void virtualCursorDataChanged(uint id, double x, double y, double l, double a) override;
	void buttonDataChanged(common::enums::Device, uint id, qulonglong mask) override;
	void stickDataChanged(common::enums::Device, uint id, int lx, int ly, int rx, int ry) override;
	void accelerometerDataChanged(common::enums::Device, uint id, int x, int y, int z) override;
	void gyroscopeDataChanged(uint id, int x, int y, int z) override;
	void infraredDataChanged(uint id, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) override;
	void pressureDataChanged(uint id, int tl, int tr, int bl, int br) override;

protected:
	void gamepad_iterator(common::enums::Device type, quint32 id, std::function<void(const std::unique_ptr<interface::IGamepad> &)> &&function);
	bool setup(common::enums::Device type, std::string &&name, quint32 id, const QJsonObject &json);

private:
	std::list<std::unique_ptr<io::interface::IGamepad>> m_gamepads;
	std::list<std::unique_ptr<io::interface::IMouse>> m_mouses;
};
}
}
