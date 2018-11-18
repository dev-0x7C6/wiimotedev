#pragma once

#include <cstdlib>

#include <externals/common/types.hpp>

class InfraredConfigContainer {
public:
	bool accEnabled() const;
	double accMultiX() const;
	double accMultiY() const;
	double accPowX() const;
	double accPowY() const;
	double deadzoneX() const;
	double deadzoneY() const;
	i32 accTimeout() const;

	void setAccEnabled(bool accEnabled);
	void setAccMultiX(double accMultiX);
	void setAccMultiY(double accMultiY);
	void setAccPowX(double accPowX);
	void setAccPowY(double accPowY);
	void setAccTimeout(const i32 &accTimeout);
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
	i32 m_accTimeout;
};
