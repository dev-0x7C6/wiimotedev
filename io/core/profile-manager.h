#pragma once

#include <QObject>
#include <memory>
#include <list>

#include "interfaces/iprofile.h"

namespace io {
namespace core {

class ProfileManager final : public QObject {
	Q_OBJECT
public:
	explicit ProfileManager(QObject *parent = nullptr);
	virtual ~ProfileManager() = default;

	QStringList list() const;
	bool load(const QString &filePath);
	bool unload(const QString &filePath);

signals:
	void loaded(const QString &filePath);
	void unloaded(const QString &filePath);

private:
	std::list<std::unique_ptr<interface::IProfile>> m_profiles;
};
}
}
