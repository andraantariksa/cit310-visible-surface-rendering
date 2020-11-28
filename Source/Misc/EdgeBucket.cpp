#include "EdgeBucket.hpp"

#include <cmath>

EdgeBucket::EdgeBucket() :
	m_Carry(0)
{
}

void EdgeBucket::NextX(float A, float B, float C)
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
	
	m_ZOfYMin -= (A / ((float)m_DY / (float)m_DX) + B) / C;
}

bool EdgeBucket::IsAlive(int yPos)
{
	return yPos < m_YMax;
}

EdgeBucket2::EdgeBucket2():
	m_Carry(0)
{
}

void EdgeBucket2::NextX()
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
}

bool EdgeBucket2::IsAlive(int yPos)
{
	return yPos < m_YMax;
}
