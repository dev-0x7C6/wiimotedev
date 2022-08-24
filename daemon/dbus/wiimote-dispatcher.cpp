#include "wiimote-dispatcher.h"

#include <QDBusConnection>

#include "wiimoteadaptor.h"

using namespace common::enums;
using namespace dae::container;
using namespace dae::enums;
using namespace dae::dbus;
using namespace dae::interface;

WiimoteDispatcher::WiimoteDispatcher(EventCallback &&eventCallback)
		: IContainerProcessor(std::move(eventCallback)) {

	new WiimoteAdaptor(this);
}

Adaptor WiimoteDispatcher::type() const { return Adaptor::Wiimote; }
QList<uint> WiimoteDispatcher::list() { return m_ids.values(); }

bool WiimoteDispatcher::ledStatus(uint id, uint led_id) { return std::get<bool>(generateEvent({CommandType::GetLedState, id, GetLedStateEvent{led_id}}).value_or(false)); }
bool WiimoteDispatcher::rumbleStatus(uint id) { return std::get<bool>(generateEvent({CommandType::GetRumbleState, id, {}}).value_or(false)); }
bool WiimoteDispatcher::setLedStatus(uint id, uint led_id, bool status) { return std::get<bool>(generateEvent({CommandType::SetLedState, id, SetLedStateEvent{led_id, status}}).value_or(false)); }
bool WiimoteDispatcher::setRumbleStatus(uint id, bool status) { return std::get<bool>(generateEvent({CommandType::SetRumbleState, id, SetRumbleStateEvent{status}}).value_or(false)); }

template <class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

void WiimoteDispatcher::process(const u32 id, const dae::container::structs::event &ev) {
	if (Device::Wiimote != ev.first)
		return;

	std::visit(overloaded{
				   [&](auto) {},
				   [&](dae::container::structs::ir_points v) {
					   emit infraredDataChanged(id, v[0].x, v[0].y, v[1].x, v[1].y, v[2].x, v[2].y, v[3].x, v[3].y);
				   },
				   [&](dae::container::structs::accdata v) {
					   emit accelerometerDataChanged(id, v.x, v.y, v.z, v.pitch, v.roll);
				   },
				   [&](dae::container::structs::button v) {
					   emit buttonDataChanged(id, v.states);
				   },
				   [&](dae::container::structs::gyro v) {
					   emit gyroscopeDataChanged(id, v.x, v.y, v.z);
				   },
				   [&](dae::container::structs::status v) {
					   if (v.is_connected) {
						   m_ids.insert(id);
						   emit connected(id);
					   } else {
						   m_ids.remove(id);
						   emit disconnected(id);
					   }
				   }},

		ev.second);
}
