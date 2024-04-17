/**
 * @author Javier Guinot Almenar <guinotal@esat-alumni.com>
 *
 * @file vector_4.h
 *
 * @brief vector4 Class Definition.
 *
 */

#include "mathlib.h"

#ifndef __VEC4_H__
#define __VEC4_H__ 1

namespace Math
{
  class Vec4
  {
  public:
    // Console
    ///////////////////////////////////////////////////////////////////////////
    inline void print(unsigned d = 5, const char *name = "Vec4") const { fprintf(stdout, "%s: %.*f, %.*f, %.*f, %.*f\n", name, d, this->x, d, this->y, d, this->z, d, this->w); }
    // This it to use with std strings and the operator <<
    inline friend std::ostream &operator<<(std::ostream &os, Vec4 vec) { return (os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")"); }
    ///////////////////////////////////////////////////////////////////////////

    // Conversors
    ///////////////////////////////////////////////////////////////////////////
    // The array must be initialized by size of 4
    inline void ToFloat(float *arr) const;
    ///////////////////////////////////////////////////////////////////////////

    // Constructors
    ///////////////////////////////////////////////////////////////////////////
    inline Vec4(float value = 0);
    inline Vec4(float x, float y, float z, float w);
    inline Vec4(const float *values_array); // Need 4 values
    inline Vec4(const Vec4 &other);
    inline ~Vec4();
    ///////////////////////////////////////////////////////////////////////////

    // Operators
    ///////////////////////////////////////////////////////////////////////////
    inline Vec4 operator+(Vec4 other) const;
    inline void operator+=(Vec4 other);

    inline Vec4 operator+(float value) const;
    inline void operator+=(float value);

    inline Vec4 operator-() const;
    inline Vec4 operator-(Vec4 other) const;
    inline void operator-=(Vec4 other);

    inline Vec4 operator-(float value) const;
    inline void operator-=(float value);

    inline bool operator==(Vec4 value) const;
    inline bool operator!=(Vec4 value) const;

    inline void operator=(Vec4 other);
    inline void operator=(float value);

    inline Vec4 operator*(float value) const;
    inline void operator*=(float value);

    inline Vec4 operator/(float value) const;
    inline void operator/=(float value);

    /*
      In use with array make sure the first one is for the array number
      and the second one is for the vector number
    */
    inline float operator[](size_t index) const;
    inline float &operator[](size_t index);
    ///////////////////////////////////////////////////////////////////////////

    // Static Methods
    ///////////////////////////////////////////////////////////////////////////
    inline static float Distance(Vec4 a, Vec4 b);

    inline static float DotProduct(Vec4 a, Vec4 b);

    inline static Vec4 Lerp(Vec4 a, Vec4 b, float value);
    inline static Vec4 LerpUnclamped(Vec4 a, Vec4 b, float value);
    ///////////////////////////////////////////////////////////////////////////

    // Methods
    ///////////////////////////////////////////////////////////////////////////
    inline float Magnitude() const;

    inline bool IsNormalized() const;
    inline Vec4 Normalized() const;

    inline float SqrMagnitude() const;
    ///////////////////////////////////////////////////////////////////////////

    // Static Attributes
    ///////////////////////////////////////////////////////////////////////////
    static const Vec4 one, zero;
    ///////////////////////////////////////////////////////////////////////////

    // Attributes
    ///////////////////////////////////////////////////////////////////////////
    float x, y, z, w;
    ///////////////////////////////////////////////////////////////////////////
  };

  // Implementation
  ///////////////////////////////////////////////////////////////////////////////

  void Vec4::ToFloat(float *arr) const
  {
    arr[0] = this->x;
    arr[1] = this->y;
    arr[2] = this->z;
    arr[3] = this->w;
  }

  // Constructors
  Vec4::Vec4(float value) : x(value), y(value), z(value), w(value) {}
  Vec4::Vec4(float a, float b, float c, float d) : x(a), y(b), z(c), w(d) {}
  Vec4::Vec4(const float *values_array) : x(values_array[0]), y(values_array[1]), z(values_array[2]), w(values_array[3]) {}
  Vec4::Vec4(const Vec4 &other) : x(other.x), y(other.y), z(other.z), w(other.w) {}

  // Destructor
  Vec4::~Vec4() { x = y = z = w = 0; }

  // Operators
  Vec4 Vec4::operator+(Vec4 other) const
  {
    return Vec4(this->x + other.x, this->y + other.y, this->z + other.z, this->w + other.w);
  }

  void Vec4::operator+=(Vec4 other)
  {
    this->x += other.x;
    this->y += other.y;
    this->z += other.z;
    this->w += other.w;
  }

  Vec4 Vec4::operator+(float value) const
  {
    return Vec4(this->x + value, this->y + value, this->z + value, this->w + value);
  }

  void Vec4::operator+=(float value)
  {
    this->x += value;
    this->y += value;
    this->z += value;
    this->w += value;
  }

  Vec4 Vec4::operator-() const
  {
    return Vec4(-this->x, -this->y, -this->z, -this->w);
  }

  Vec4 Vec4::operator-(Vec4 other) const
  {
    return Vec4(this->x - other.x, this->y - other.y, this->z - other.z, this->w - other.w);
  }

  void Vec4::operator-=(Vec4 other)
  {
    this->x -= other.x;
    this->y -= other.y;
    this->z -= other.z;
    this->w -= other.w;
  }

  Vec4 Vec4::operator-(float value) const
  {
    return Vec4(this->x - value, this->y - value, this->z - value, this->w - value);
  }

  void Vec4::operator-=(float value)
  {
    this->x -= value;
    this->y -= value;
    this->z -= value;
    this->w -= value;
  }

  bool Vec4::operator==(Vec4 value) const
  {
    if (this->x != value.x)
      return false;
    if (this->y != value.y)
      return false;
    if (this->z != value.z)
      return false;
    if (this->w != value.w)
      return false;
    return true;
  }

  bool Vec4::operator!=(Vec4 value) const
  {
    if (this->x == value.x)
      return false;
    if (this->y == value.y)
      return false;
    if (this->z == value.z)
      return false;
    if (this->w == value.w)
      return false;
    return true;
  }

  void Vec4::operator=(Vec4 other)
  {
    this->x = other.x;
    this->y = other.y;
    this->z = other.z;
    this->w = other.w;
  }

  void Vec4::operator=(float value)
  {
    this->x = value;
    this->y = value;
    this->z = value;
    this->w = value;
  }

  Vec4 Vec4::operator*(float value) const
  {
    return Vec4(this->x * value, this->y * value, this->z * value, this->w * value);
  }

  void Vec4::operator*=(float value)
  {
    this->x *= value;
    this->y *= value;
    this->z *= value;
    this->w *= value;
  }

  Vec4 Vec4::operator/(float value) const
  {
    float rec_value = 1.0f / value;
    return Vec4(this->x * rec_value, this->y * rec_value, this->z * rec_value, this->w * rec_value);
  }

  void Vec4::operator/=(float value)
  {
    float rec_value = 1.0f / value;
    this->x *= rec_value;
    this->y *= rec_value;
    this->z *= rec_value;
    this->w *= rec_value;
  }

  float Vec4::operator[](size_t index) const
  {
    if (index == 0)
      return x;
    else if (index == 1)
      return y;
    else if (index == 2)
      return z;
    else if (index == 3)
      return w;
    else
      throw std::out_of_range("Ínvalid index reading Vec4");
  }

  float &Vec4::operator[](size_t index)
  {
    if (index == 0)
      return x;
    else if (index == 1)
      return y;
    else if (index == 2)
      return z;
    else if (index == 3)
      return w;
    else
      throw std::out_of_range("Invalid index writting Vec4");
  }

  // Methods
  float Vec4::Magnitude() const
  {
    return sqrtf((this->x * this->x) + (this->y * this->y) + (this->z * this->z) + (this->w * this->w));
  }

  bool Vec4::IsNormalized() const
  {
    float magn = Magnitude();

    if (magn < 1.001 && magn > 0.999)
      return true;

    return false;
  }

  Vec4 Vec4::Normalized() const
  {
    if (IsNormalized())
      return (*this);
    else
      return (*this) / Magnitude();
  }

  float Vec4::SqrMagnitude() const
  {
    return (this->x * this->x) + (this->y * this->y) + (this->z * this->z) + (this->w * this->w);
  }

  float Vec4::Distance(Vec4 a, Vec4 b)
  {
    Vec4 newVec = a - b;

    return sqrtf((newVec.x * newVec.x) + (newVec.y * newVec.y) + (newVec.z * newVec.z) + (newVec.w * newVec.w));
  }

  Vec4 Vec4::Lerp(Vec4 a, Vec4 b, float value)
  {
    return Vec4(((b - a) * ((value < 0) ? 0 : (value > 1 ? 1 : value))) + a);
  }

  Vec4 Vec4::LerpUnclamped(Vec4 a, Vec4 b, float value)
  {
    return Vec4(((b - a) * value) + a);
  }

  float Vec4::DotProduct(Vec4 a, Vec4 b)
  {
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w);
  }

}

#endif /* __VEC4_H__ */
