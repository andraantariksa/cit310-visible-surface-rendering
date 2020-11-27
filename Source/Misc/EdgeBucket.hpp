#ifndef __MISC_EDGEBUCKET_HPP
#define __MISC_EDGEBUCKET_HPP

class EdgeBucket
{
public:
	int m_YMax;
	int m_XOfYMin;
	int m_DX;
	int m_DY;
	int m_Carry;
	float m_ZOfYMin;

	void NextX(float A, float B, float C);
	bool IsAlive(int y_pos);
};

class EdgeBucket2
{
public:
	int m_YMax;
	int m_XOfYMin;
	int m_DX;
	int m_DY;
	int m_Carry;

	void NextX();
	bool IsAlive(int y_pos);
};

#endif
