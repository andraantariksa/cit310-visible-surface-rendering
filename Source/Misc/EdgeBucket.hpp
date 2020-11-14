#ifndef __MISC_EDGEBUCKET_HPP
#define __MISC_EDGEBUCKET_HPP

class EdgeBucket
{
public:
	size_t m_YMax;
	size_t m_XOfYMin;
	int m_DX;
	int m_DY;
	int m_Carry;
	float m_ZOfYMin;

	void NextX(float A, float B, float C);
	bool IsAlive(size_t y_pos);
};

#endif
