#pragma once

#include <string>

namespace io {
namespace interface {

class IProfile {
public:
	explicit IProfile(const std::string &configurationFilePath);
	virtual ~IProfile() = default;

	std::string configurationFilePath() const;

private:
	const std::string m_configurationFilePath;
};
}
}
