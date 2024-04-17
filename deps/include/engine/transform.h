#include "engine.h"

#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__ 1

/**
 * @class Transform
 *
 * @brief Represents a 3D transformation with rotation, scaling, translation, and orbiting capabilities.
 */
class Transform
{
public:
  /**
   * @enum MatrixTr
   *
   * @brief Enumeration for transformation matrix types.
   */
  enum MatrixTr
  {
    Rotate = 0, ///< Rotation transformation matrix.
    Scale,      ///< Scaling transformation matrix.
    Translate,  ///< Translation transformation matrix.
    Orbit,      ///< Orbiting transformation matrix.
  };

  /**
   * @brief Constructor for the Transform class.
   */
  Transform();

  /**
   * @brief Destructor for the Transform class.
   */
  ~Transform();

  /**
   * @brief Gets the combined transformation matrix incorporating rotation, scaling, translation, and orbiting.
   *
   * @return 4x4 transformation matrix.
   */
  Math::Mat4 getTrMatrix();

  /**
   * @brief Rotates the object by the specified angles in radians.
   *
   * @param radians Angles in radians for rotation around the x, y, and z axes.
   */
  void rotate(Math::Vec3 radians);

  /**
   * @brief Gets the current rotation angles.
   *
   * @return Rotation angles in radians around the x, y, and z axes.
   */
  Math::Vec3 getRotate() const;

  /**
   * @brief Gets the rotation transformation matrix.
   *
   * @return 4x4 rotation transformation matrix.
   */
  Math::Mat4 getRotateMat() const;

  /**
   * @brief Scales the object by the specified factors along the x, y, and z axes.
   *
   * @param size Scaling factors along the x, y, and z axes.
   */
  void scale(Math::Vec3 size);

  /**
   * @brief Gets the current scaling factors.
   *
   * @return Scaling factors along the x, y, and z axes.
   */
  Math::Vec3 getScale() const;

  /**
   * @brief Gets the scaling transformation matrix.
   *
   * @return 4x4 scaling transformation matrix.
   */
  Math::Mat4 getScaleMat() const;

  /**
   * @brief Translates the object by the specified amounts along the x, y, and z axes.
   *
   * @param move Translation amounts along the x, y, and z axes.
   */
  void translate(Math::Vec3 move);

  /**
   * @brief Gets the current translation amounts.
   *
   * @return Translation amounts along the x, y, and z axes.
   */
  Math::Vec3 getTranslate() const;

  /**
   * @brief Gets the translation transformation matrix.
   *
   * @return 4x4 translation transformation matrix.
   */
  Math::Mat4 getTranslateMat() const;

  /**
   * @brief Orbits the object by the specified angles in radians.
   *
   * @param orbit Angles in radians for orbiting around the x, y, and z axes.
   */
  void orbit(Math::Vec3 orbit);

  /**
   * @brief Gets the current orbiting angles.
   *
   * @return Orbiting angles in radians around the x, y, and z axes.
   */
  Math::Vec3 getOrbit() const;

  /**
   * @brief Gets the orbiting transformation matrix.
   *
   * @return 4x4 orbiting transformation matrix.
   */
  Math::Mat4 getOrbitMat() const;

  /**
   * @brief Orbits the object around a specified center point.
   *
   * @param orbit_center Center point for orbiting.
   */
  void orbitCenter(Math::Vec3 orbit_center);

  /**
   * @brief Gets the center point for orbiting.
   *
   * @return Center point for orbiting.
   */
  Math::Vec3 getOrbitCenter() const;

private:
  Math::Mat4 matrix_[4]; ///< Array of 4x4 transformation matrices for rotation, scaling, translation, and orbiting.

  Math::Vec3 translate_;    ///< Translation amounts along the x, y, and z axes.
  Math::Vec3 rotate_;       ///< Rotation angles in radians around the x, y, and z axes.
  Math::Vec3 scalate_;      ///< Scaling factors along the x, y, and z axes.
  Math::Vec3 orbit_;        ///< Orbiting angles in radians around the x, y, and z axes.
  Math::Vec3 orbit_center_; ///< Center point for orbiting.
};

#endif /* __TRANSFORM_H__ */