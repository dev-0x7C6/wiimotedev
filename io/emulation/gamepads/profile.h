#pragma once

#include <string>

namespace io {
namespace functional {

class Profile {
public:
	explicit Profile(const std::string &confFilePath);
	virtual ~Profile() = default;

private:
	const std::string m_configurationFilePath;
};
}
}
