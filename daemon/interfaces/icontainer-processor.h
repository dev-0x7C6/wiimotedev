#pragma once

#include <functional>
#include <memory>

#include <QObject>

#include "common/enums/command-type.hpp"
#include "common/enums/device.h"
#include "enums/adaptor.h"
#include "containers/structs.hpp"

#include <QSet>
#include <QList>

template <class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

namespace dae::interface {

namespace id {
class manager {
public:
	void set(const u32 id, const bool state) {
		if (state)
			ids.insert(id);
		else
			ids.remove(id);
	}

	auto values() const {
		auto ret = ids.values();
		std::sort(ret.begin(), ret.end());
		return ret;
	}

private:
	QSet<u32> ids;
};
}

class IContainerProcessor : public QObject {
public:
	explicit IContainerProcessor(EventCallback &&eventCallback, QObject *parent = nullptr);
	~IContainerProcessor() override = default;

	virtual enums::Adaptor type() const = 0;
	virtual void process(u32 id, const dae::container::event &ev) = 0;

protected:
	CommandResult generateEvent(CommandEvent &&event) const noexcept;
	id::manager ids;

private:
	EventCallback m_eventCallback;
};
}
