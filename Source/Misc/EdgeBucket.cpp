#include "EdgeBucket.hpp"

#include <cmath>

size_t EdgeBucket::NextX()
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
	return m_XOfYMin;
}

bool EdgeBucket::IsAlive(size_t y_pos)
{
	return y_pos < m_YMax;
}
