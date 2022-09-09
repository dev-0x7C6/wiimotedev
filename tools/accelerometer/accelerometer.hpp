#pragma once

#include <QObject>
#include <QPoint>
#include <QJsonObject>

class Accelerometer final : public QObject {
	Q_OBJECT
	Q_PROPERTY(QJsonObject data READ data NOTIFY dataChanged)
public:
	explicit Accelerometer(QObject *parent = nullptr);

	auto data() const -> QJsonObject;

private:
	void accelerometerDataChanged(uint id, int x, int y, int z, int pitch, int roll);

private:
	QJsonObject m_data;

signals:
	void dataChanged(QJsonObject);
};
