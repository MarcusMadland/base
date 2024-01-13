#ifndef BASE_SPRING_H_HEADER_GUARD
#	error "Must be included from base/spring.h!"
#endif // BASE_CPU_H_HEADER_GUARD

namespace base
{
    void criticalSpringDamper(
        F32& _position,
        F32& _velocity,
        F32& _acceleration,
        F32 _velocityGoal,
        F32 _halfLife,
        F32 _dt)
    {
        F32 _decayFactor = ((4.0f * 0.69314718056f) / (_halfLife + 1e-5f)) / 2.0f;
        F32 _jerk0 = _velocity - _velocityGoal;
        F32 _jerk1 = _acceleration + _jerk0 * _decayFactor;
        F32 _x = _decayFactor * _dt;
        F32 _exponential = 1.0f / (1.0f + _x + 0.48f * _x * _x + 0.235f * _x * _x * _x);

        _position = _exponential * (((-_jerk1) / (_decayFactor * _decayFactor)) + ((-_jerk0 - _jerk1 * _dt) / _decayFactor)) +
            (_jerk1 / (_decayFactor * _decayFactor)) + _jerk0 / _decayFactor + _velocityGoal * _dt + _position;
        _velocity = _exponential * (_jerk0 + _jerk1 * _dt) + _velocityGoal;
        _acceleration = _exponential * (_acceleration - _jerk1 * _decayFactor * _dt);
    }

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
        F32 _dt)
    {
        for (U32 i = 0; i < _count; i++)
        {
            _positions[i] = _initialPosition;
            _velocities[i] = _initialVelocity;
            _accelerations[i] = _initialAcceleration;
        }

        for (U32 i = 0; i < _count; i++)
        {
            criticalSpringDamper(_positions[i], _velocities[i], _accelerations[i], _velocityGoal, _halfLife, i * _dt);
        }
    }

} // namespace base
