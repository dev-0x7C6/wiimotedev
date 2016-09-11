#include "iprofile.h"

using namespace io::interface;

IProfile::IProfile(const std::string &configurationFilePath)
		: m_configurationFilePath(configurationFilePath) {
}

std::string IProfile::configurationFilePath() const {
	return m_configurationFilePath;
}
