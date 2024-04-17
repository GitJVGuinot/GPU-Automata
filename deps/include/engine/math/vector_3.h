/**
 * @author Javier Guinot Almenar <guinotal@esat-alumni.com>
 *
 * @file vector_3.h
 *
 * @brief vector3 Class Definition.
 *
 */

#include "mathlib.h"

#ifndef __VEC3_H__
#define __VEC3_H__ 1

namespace Math
{
  class Vec3
  {
  public:
    // Console
    ///////////////////////////////////////////////////////////////////////////
    inline void print(unsigned d = 5, const char *name = "Vec3") const { fprintf(stdout, "%s: %.*f, %.*f, %.*f\n", name, d, this->x, d, this->y, d, this->z); }
    // This it to use with std strings and the operator <<
    inline friend std::ostream &operator<<(std::ostream &os, Vec3 vec) { return (os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")"); }
    ///////////////////////////////////////////////////////////////////////////

    // Conversors
    ///////////////////////////////////////////////////////////////////////////
    // The array must be initialized by size of 3
    inline void ToFloat(float *arr) const;
    ///////////////////////////////////////////////////////////////////////////

    // Constructors
    ///////////////////////////////////////////////////////////////////////////
    inline Vec3(float value = 0);
    inline Vec3(float x, float y, float z);
    inline Vec3(const float *values_array); // Need 3 values
    inline Vec3(const Vec3 &other);
    inline ~Vec3();
    ///////////////////////////////////////////////////////////////////////////

    // Operators
    ///////////////////////////////////////////////////////////////////////////
    inline Vec3 operator+(Vec3 other) const;
    inline Vec3 &operator+=(Vec3 other);

    inline Vec3 operator+(float value) const;
    inline Vec3 &operator+=(float value);

    inline Vec3 operator-() const;
    inline Vec3 operator-(Vec3 other) const;
    inline Vec3 &operator-=(Vec3 other);

    inline Vec3 operator-(float value) const;
    inline Vec3 &operator-=(float value);

    inline bool operator==(Vec3 value) const;
    inline bool operator!=(Vec3 value) const;

    inline void operator=(Vec3 other);
    inline void operator=(float value);

    inline Vec3 operator*(float value) const;
    inline Vec3 &operator*=(float value);

    inline Vec3 operator/(float value) const;
    inline Vec3 &operator/=(float value);

    /*
      In use with array make sure the first one is for the array number
      and the second one is for the vector number
    */
    inline float operator[](size_t index) const;
    inline float &operator[](size_t index);
    ///////////////////////////////////////////////////////////////////////////

    // Static Methods
    ///////////////////////////////////////////////////////////////////////////
    inline static float Distance(Vec3 a, Vec3 b);

    inline static float DotProduct(Vec3 a, Vec3 b);
    inline static Vec3 CrossProduct(Vec3 a, Vec3 b);

    inline static Vec3 Lerp(Vec3 a, Vec3 b, float value);
    inline static Vec3 LerpUnclamped(Vec3 a, Vec3 b, float t);

    inline static float Angle(Vec3 a, Vec3 b);
    inline static Vec3 Reflect(Vec3 direction, Vec3 normal);
    ///////////////////////////////////////////////////////////////////////////

    // Methods
    ///////////////////////////////////////////////////////////////////////////
    inline float Magnitude() const;

    inline bool IsNormalized() const;
    inline Vec3 Normalized() const;

    inline float SqrMagnitude() const;
    ///////////////////////////////////////////////////////////////////////////

    // Static Attributes
    ///////////////////////////////////////////////////////////////////////////
    static const Vec3 up, down, right, left, forward, back, zero, one;
    ///////////////////////////////////////////////////////////////////////////

    // Attributes
    ///////////////////////////////////////////////////////////////////////////
    float x, y, z;
    ///////////////////////////////////////////////////////////////////////////
  };

  // Implementation
  ///////////////////////////////////////////////////////////////////////////////

  void Vec3::ToFloat(float *arr) const
  {
    arr[0] = this->x;
    arr[1] = this->y;
    arr[2] = this->z;
  }

  // Constructors
  Vec3::Vec3(float value) : x(value), y(value), z(value) {}
  Vec3::Vec3(float a, float b, float c) : x(a), y(b), z(c) {}
  Vec3::Vec3(const float *values_array) : x(values_array[0]), y(values_array[1]), z(values_array[2]) {}
  Vec3::Vec3(const Vec3 &other) : x(other.x), y(other.y), z(other.z) {}

  // Destructor
  Vec3::~Vec3() { x = y = z = 0; }

  // Operators
  Vec3 Vec3::operator+(Vec3 other) const
  {
    return Vec3(this->x + other.x, this->y + other.y, this->z + other.z);
  }

  Vec3 &Vec3::operator+=(Vec3 other)
  {
    this->x += other.x;
    this->y += other.y;
    this->z += other.z;

    return (*this);
  }

  Vec3 Vec3::operator+(float value) const
  {
    return Vec3(this->x + value, this->y + value, this->z + value);
  }

  Vec3 &Vec3::operator+=(float value)
  {
    this->x += value;
    this->y += value;
    this->z += value;

    return (*this);
  }

  Vec3 Vec3::operator-() const
  {
    return Vec3(-this->x, -this->y, -this->z);
  }

  Vec3 Vec3::operator-(Vec3 other) const
  {
    return Vec3(this->x - other.x, this->y - other.y, this->z - other.z);
  }

  Vec3 &Vec3::operator-=(Vec3 other)
  {
    this->x -= other.x;
    this->y -= other.y;
    this->z -= other.z;

    return (*this);
  }

  Vec3 Vec3::operator-(float value) const
  {
    return Vec3(this->x - value, this->y - value, this->z - value);
  }

  Vec3 &Vec3::operator-=(float value)
  {
    this->x -= value;
    this->y -= value;
    this->z -= value;

    return (*this);
  }

  bool Vec3::operator==(Vec3 value) const
  {
    if (this->x != value.x)
      return false;
    if (this->y != value.y)
      return false;
    if (this->z != value.z)
      return false;

    return true;
  }

  bool Vec3::operator!=(Vec3 value) const
  {
    if (this->x == value.x)
      return false;
    if (this->y == value.y)
      return false;
    if (this->z == value.z)
      return false;

    return true;
  }

  void Vec3::operator=(Vec3 other)
  {
    this->x = other.x;
    this->y = other.y;
    this->z = other.z;
  }

  void Vec3::operator=(float value)
  {
    this->x = value;
    this->y = value;
    this->z = value;
  }

  Vec3 Vec3::operator*(float value) const
  {
    return Vec3(this->x * value, this->y * value, this->z * value);
  }

  Vec3 &Vec3::operator*=(float value)
  {
    this->x *= value;
    this->y *= value;
    this->z *= value;

    return (*this);
  }

  Vec3 Vec3::operator/(float value) const
  {
    float rec_value = 1.0f / value;
    return Vec3(this->x * rec_value, this->y * rec_value, this->z * rec_value);
  }

  Vec3 &Vec3::operator/=(float value)
  {
    float rec_value = 1.0f / value;

    this->x *= rec_value;
    this->y *= rec_value;
    this->z *= rec_value;

    return (*this);
  }

  float Vec3::operator[](size_t index) const
  {
    if (index == 0)
      return x;
    else if (index == 1)
      return y;
    else if (index == 2)
      return z;
    else
      throw std::out_of_range("Ínvalid index reading Vec3");
  }

  float &Vec3::operator[](size_t index)
  {
    if (index == 0)
      return x;
    else if (index == 1)
      return y;
    else if (index == 2)
      return z;
    else
      throw std::out_of_range("Invalid index writting Vec3");
  }

  // Methods
  float Vec3::Magnitude() const
  {
    return sqrtf((this->x * this->x) + (this->y * this->y) + (this->z * this->z));
  }

  bool Vec3::IsNormalized() const
  {
    float magn = Magnitude();
    bool turn = false;

    if (magn < 1.001 && magn > 0.999)
      turn = true;

    return turn;
  }

  Vec3 Vec3::Normalized() const
  {
    if (IsNormalized())
      return (*this);
    else
      return (*this) / Magnitude();
  }

  float Vec3::SqrMagnitude() const
  {
    return (this->x * this->x) + (this->y * this->y) + (this->z * this->z);
  }

  float Vec3::Distance(Vec3 a, Vec3 b)
  {
    Vec3 newVec = a - b;

    return sqrtf((newVec.x * newVec.x) + (newVec.y * newVec.y) + (newVec.z * newVec.z));
  }

  Vec3 Vec3::Lerp(Vec3 a, Vec3 b, float value)
  {
    return Vec3(((b - a) * ((value < 0) ? 0 : (value > 1 ? 1 : value))) + a);
  }

  Vec3 Vec3::LerpUnclamped(Vec3 a, Vec3 b, float value)
  {
    return Vec3(((b - a) * value) + a);
  }

  float Vec3::DotProduct(Vec3 a, Vec3 b)
  {
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
  }

  Vec3 Vec3::CrossProduct(Vec3 a, Vec3 b)
  {
    Vec3 ret;
    ret.x = a.y * b.z - a.z * b.y;
    ret.y = a.z * b.x - a.x * b.z;
    ret.z = a.x * b.y - a.y * b.x;

    return ret;
  }

  float Vec3::Angle(Vec3 a, Vec3 b)
  {
    return acosf(DotProduct(a, b) / (a.Magnitude() * b.Magnitude()));
  }

  Vec3 Vec3::Reflect(Vec3 direction, Vec3 normal)
  {
    return direction - normal * (2 * DotProduct(direction, normal));
  }

}

#endif /* __VEC3_H__ */
