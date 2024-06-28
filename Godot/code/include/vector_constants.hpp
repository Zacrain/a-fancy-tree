#ifndef VECTOR_CONSTANTS_HPP
#define VECTOR_CONSTANTS_HPP

#include <godot_cpp/variant/vector3.hpp>

/** Defines commonly used vector constants like zero vector or unit vectors for the different axes etc.
 *  Will be filled as required.
 */

namespace godot {

    /// 3D zero float (c++ double) vector: (0, 0, 0).
    const Vector3 Vec3_ZERO(0, 0, 0);
    /// 3D up float (c++ double) vector: (0, 1, 0).
    const Vector3 Vec3_UP(0, 1, 0);
    /// 3D forward float (c++ double) vector: (1, 0, 0).
    const Vector3 Vec3_LEFT(1, 0, 0);
    const Vector3 Vec3_X_POS = Vec3_LEFT;
    const Vector3 Vec3_RIGHT = -Vec3_LEFT;
    const Vector3 Vec3_X_NEG = Vec3_RIGHT;

} // namespace godot

#endif /* VECTOR_CONSTANTS_HPP */
