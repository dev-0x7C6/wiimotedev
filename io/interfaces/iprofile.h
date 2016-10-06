#pragma once

#include <string>

#include "interfaces/iwiimotedev-event.h"

namespace io {
namespace interface {

class IProfile : public IWiimotedevEvent {
public:
	explicit IProfile(const std::string &configurationFilePath);
	virtual ~IProfile() = default;

	std::string configurationFilePath() const;

private:
	const std::string m_configurationFilePath;
};
}
}
