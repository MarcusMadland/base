#ifndef BASE_SPRING_H_HEADER_GUARD
#define BASE_SPRING_H_HEADER_GUARD

#include "base.h"

namespace base
{
    void criticalSpringDamper(
        F32& _position,
        F32& _velocity,
        F32& _acceleration,
        F32 _velocityGoal,
        F32 _halfLife,
        F32 _dt);

    void criticalSpringDamperPredict(
        F32* _positions,
        F32* _velocities,
        F32* _accelerations,
        U32 _count,
        F32 _initialPosition,
        F32 _initialVelocity,
        F32 _initialAcceleration,
        F32 _velocityGoal,
        F32 _halfLife,
        F32 _dt);

} // namespace base

#include "inline/spring.inl"

#endif // BASE_SPRING_H_HEADER_GUARD
