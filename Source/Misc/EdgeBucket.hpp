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
	double m_ZOfYMin;

	EdgeBucket();

	void NextXAndZ(double A, double B, double C);
	bool IsAlive(int yPos);
};

#endif
