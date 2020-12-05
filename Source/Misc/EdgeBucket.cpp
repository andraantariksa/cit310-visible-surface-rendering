#include "EdgeBucket.hpp"

#include <cmath>

EdgeBucket::EdgeBucket() :
	m_Carry(0)
{
}

void EdgeBucket::NextX(double A, double B, double C)
{
	m_Carry += std::abs(m_DX);
	while (2 * m_Carry >= m_DY)
	{
		m_Carry -= m_DY;
		if (m_DX > 0)
		{
			m_XOfYMin++;
		}
		else
		{
			m_XOfYMin--;
		}
	}
	
	m_ZOfYMin -= (A / ((double)m_DY / (double)m_DX) + B) / C;
}

bool EdgeBucket::IsAlive(int yPos)
{
	return yPos < m_YMax;
}
