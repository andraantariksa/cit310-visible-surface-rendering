#ifndef __COMPONENT_POSITIONCOMPONENT_HPP
#define __COMPONENT_POSITIONCOMPONENT_HPP

class PositionComponent final
{
public:
	float x;
	float y;
	float z;

	PositionComponent(float x, float y, float z) noexcept;
	~PositionComponent() = default;
	PositionComponent& operator=(const PositionComponent&) noexcept;
};

#endif
