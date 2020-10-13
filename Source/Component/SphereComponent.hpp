#ifndef __COMPONENT_SPHERECOMPONENT_HPP
#define __COMPONENT_SPHERECOMPONENT_HPP

class SphereComponent
{
public:
	float r;

	SphereComponent(float r);
	~SphereComponent() = default;
	SphereComponent& operator=(const SphereComponent&) noexcept;
};

#endif
