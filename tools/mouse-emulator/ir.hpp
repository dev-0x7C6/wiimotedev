#pragma once

#include <QObject>

class VirtualCursor final : public QObject {
	Q_OBJECT
public:
	explicit VirtualCursor(QObject *parent = nullptr);

private:
	void dataChanged(uint id, double x, double y, double yaw, double roll, double pitch, double distance_cm, bool visible);
};
