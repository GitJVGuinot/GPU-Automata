/**
 * @author Javier Guinot Almenar <guinotal@esat-alumni.com>
 *
 * @file vector_2.h
 *
 * @brief vector2 Class Definition.
 *
 */

#include "mathlib.h"

#ifndef __VEC2_H__
#define __VEC2_H__ 1

namespace Math
{
  class Vec2
  {
  public:
    // Console
    ///////////////////////////////////////////////////////////////////////////
    inline void print(unsigned d = 5, const char *name = "Vec2") const { fprintf(stdout, "%s: %.*f, %.*f\n", name, d, this->x, d, this->y); }
    // This it to use with std strings and the operator <<
    inline friend std::ostream &operator<<(std::ostream &os, Vec2 vec) { return (os << "(" << vec.x << ", " << vec.y << ")"); }
    ///////////////////////////////////////////////////////////////////////////

    // Conversors
    ///////////////////////////////////////////////////////////////////////////
    // The array must be initialized by size of 2
    inline void ToFloat(float *arr) const;
    ///////////////////////////////////////////////////////////////////////////

    // Constructors
    ///////////////////////////////////////////////////////////////////////////
    inline Vec2(float value = 0);
    inline Vec2(float x, float y);
    inline Vec2(const float *values_array); // Need 2 values
    inline Vec2(const Vec2 &copy);
    inline ~Vec2();
    ///////////////////////////////////////////////////////////////////////////

    // Operators
    ///////////////////////////////////////////////////////////////////////////

    // Value
    inline Vec2 operator+(float value) const;
    inline Vec2 &operator+=(float value);

    inline Vec2 operator-(float value) const;
    inline Vec2 &operator-=(float value);

    inline Vec2 operator*(float value) const;
    inline Vec2 &operator*=(float value);

    inline Vec2 operator/(float value) const;
    inline Vec2 &operator/=(float value);

    inline Vec2 operator-() const;

    inline void operator=(float value);

    inline bool operator==(float value) const;
    inline bool operator!=(float value) const;

    // Other
    inline Vec2 operator+(Vec2 other) const;
    inline Vec2 &operator+=(Vec2 other);

    inline Vec2 operator-(Vec2 other) const;
    inline Vec2 &operator-=(Vec2 other);

    inline void operator=(Vec2 other);

    inline bool operator==(Vec2 other) const;
    inline bool operator!=(Vec2 other) const;

    /*
      In use with array make sure the first one is for the array number
      and the second one is for the vector number
    */
    inline float operator[](size_t index) const;
    inline float &operator[](size_t index);
    ///////////////////////////////////////////////////////////////////////////

    // Static Methods
    ///////////////////////////////////////////////////////////////////////////
    inline static float Distance(Vec2 a, Vec2 b);

    inline static float DotProduct(Vec2 a, Vec2 b);

    inline static Vec2 Lerp(Vec2 a, Vec2 b, float t);
    inline static Vec2 LerpUnclamped(Vec2 a, Vec2 b, float t);

    inline static float Angle(Vec2 a, Vec2 b);
    inline static Vec2 Reflect(Vec2 direction, Vec2 normal);
    ///////////////////////////////////////////////////////////////////////////

    // Methods
    ///////////////////////////////////////////////////////////////////////////
    inline float Magnitude() const;

    inline bool IsNormalized() const;
    inline Vec2 Normalized() const;

    inline float SqrMagnitude() const;
    ///////////////////////////////////////////////////////////////////////////

    // Static Attributes
    ///////////////////////////////////////////////////////////////////////////
    static const Vec2 up, down, right, left, zero, one;
    ///////////////////////////////////////////////////////////////////////////

    // Attributes
    ///////////////////////////////////////////////////////////////////////////
    float x, y;
    ///////////////////////////////////////////////////////////////////////////
  };

  // Implementation
  ///////////////////////////////////////////////////////////////////////////////
  void Vec2::ToFloat(float *arr) const
  {
    arr[0] = this->x;
    arr[1] = this->y;
  }

  // Constructors
  Vec2::Vec2(float value) : x(value), y(value) {}
  Vec2::Vec2(float a, float b) : x(a), y(b) {}
  Vec2::Vec2(const float *values_array) : x(values_array[0]), y(values_array[1]) {}
  Vec2::Vec2(const Vec2 &other) : x(other.x), y(other.y) {}

  // Destructor
  Vec2::~Vec2() { x = y = 0; }

  // Operators
  Vec2 Vec2::operator+(Vec2 other) const
  {
    return Vec2(this->x + other.x, this->y + other.y);
  }

  Vec2 &Vec2::operator+=(Vec2 other)
  {
    this->x += other.x;
    this->y += other.y;

    return (*this);
  }

  Vec2 Vec2::operator+(float value) const
  {
    return Vec2(this->x + value, this->y + value);
  }

  Vec2 &Vec2::operator+=(float value)
  {
    this->x += value;
    this->y += value;

    return (*this);
  }

  Vec2 Vec2::operator-() const
  {
    return Vec2(-this->x, -this->y);
  }

  Vec2 Vec2::operator-(Vec2 other) const
  {
    return Vec2(this->x - other.x, this->y - other.y);
  }

  Vec2 &Vec2::operator-=(Vec2 other)
  {
    this->x -= other.x;
    this->y -= other.y;

    return (*this);
  }

  Vec2 Vec2::operator-(float value) const
  {
    return Vec2(this->x - value, this->y - value);
  }

  Vec2 &Vec2::operator-=(float value)
  {
    this->x -= value;
    this->y -= value;

    return (*this);
  }

  bool Vec2::operator==(Vec2 other) const
  {
    if (this->x != other.x)
      return false;
    if (this->y != other.y)
      return false;

    return true;
  }

  bool Vec2::operator==(float value) const
  {
    if (this->x != value)
      return false;
    if (this->y != value)
      return false;

    return true;
  }

  bool Vec2::operator!=(Vec2 other) const
  {
    if (this->x == other.x)
      return false;
    if (this->y == other.y)
      return false;

    return true;
  }

  bool Vec2::operator!=(float value) const
  {
    if (this->x == value)
      return false;
    if (this->y == value)
      return false;

    return true;
  }

  void Vec2::operator=(Vec2 other)
  {
    this->x = other.x;
    this->y = other.y;
  }

  void Vec2::operator=(float value)
  {
    this->x = this->y = value;
  }

  Vec2 Vec2::operator*(float value) const
  {
    return Vec2(this->x * value, this->y * value);
  }

  Vec2 &Vec2::operator*=(float value)
  {
    this->x *= value;
    this->y *= value;

    return (*this);
  }

  Vec2 Vec2::operator/(float value) const
  {
    float rec_value = 1.0f / value;
    return Vec2(this->x * rec_value, this->y * rec_value);
  }

  Vec2 &Vec2::operator/=(float value)
  {
    float rec_value = 1.0f / value;

    this->x *= rec_value;
    this->y *= rec_value;

    return (*this);
  }

  float Vec2::operator[](size_t index) const
  {
    if (index == 0)
      return x;
    else if (index == 1)
      return y;
    else
      throw std::out_of_range("Ínvalid index reading Vec2");
  }

  float &Vec2::operator[](size_t index)
  {
    if (index == 0)
      return x;
    else if (index == 1)
      return y;
    else
      throw std::out_of_range("Invalid index writting Vec2");
  }

  // Methods
  float Vec2::Magnitude() const
  {
    return sqrtf((this->x * this->x) + (this->y * this->y));
  }

  bool Vec2::IsNormalized() const
  {
    float magn = Magnitude();

    if (magn < 1.001 && magn > 0.999)
      return true;

    return false;
  }

  Vec2 Vec2::Normalized() const
  {
    if (IsNormalized())
      return (*this);
    else
      return (*this) / Magnitude();
  }

  float Vec2::SqrMagnitude() const
  {
    return (this->x * this->x) + (this->y * this->y);
  }

  float Vec2::Distance(Vec2 a, Vec2 b)
  {
    Vec2 newVec = a - b;

    return sqrtf((newVec.x * newVec.x) + (newVec.y * newVec.y));
  }

  Vec2 Vec2::Lerp(Vec2 a, Vec2 b, float value)
  {
    return Vec2(((b - a) * ((value < 0) ? 0 : (value > 1 ? 1 : value))) + a);
  }

  Vec2 Vec2::LerpUnclamped(Vec2 a, Vec2 b, float value)
  {
    return Vec2(((b - a) * value) + a);
  }

  float Vec2::DotProduct(Vec2 a, Vec2 b)
  {
    return (a.x * b.x) + (a.y * b.y);
  }

  float Vec2::Angle(Vec2 a, Vec2 b)
  {
    return acosf(DotProduct(a, b) / (a.Magnitude() * b.Magnitude()));
  }

  Vec2 Vec2::Reflect(Vec2 direction, Vec2 normal)
  {
    return direction - normal * (2.0f * DotProduct(direction, normal));
  }

}

#endif /* __VEC2_H__ */
