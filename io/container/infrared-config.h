#pragma once

#include <cstdlib>

class InfraredConfigContainer {
public:
	bool accEnabled() const;
	double accMultiX() const;
	double accMultiY() const;
	double accPowX() const;
	double accPowY() const;
	double deadzoneX() const;
	double deadzoneY() const;
	int32_t accTimeout() const;

	void setAccEnabled(bool accEnabled);
	void setAccMultiX(double accMultiX);
	void setAccMultiY(double accMultiY);
	void setAccPowX(double accPowX);
	void setAccPowY(double accPowY);
	void setAccTimeout(const int32_t &accTimeout);
	void setDeadzoneX(double deadzoneX);
	void setDeadzoneY(double deadzoneY);

private:
	bool m_accEnabled;
	double m_accMultiX;
	double m_accMultiY;
	double m_accPowX;
	double m_accPowY;
	double m_deadzoneX;
	double m_deadzoneY;
	int32_t m_accTimeout;
};
