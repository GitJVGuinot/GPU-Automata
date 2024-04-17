/**
 * @author Javier Guinot Almenar <guinotal@esat-alumni.com>
 *
 * @file matrix_2.h
 *
 * @brief Matrix 2 Class Definition.
 * Order:
 *  C0 C1
 *
 *  m0 m1   L0
 *  m2 m3   L1
 *
 */

#include "mathlib.h"

#ifndef __MATRIX2_H__
#define __MATRIX2_H__ 1

namespace Math
{
  class Mat2
  {
  public:
    // Console
    ///////////////////////////////////////////////////////////////////////////
    inline void print(unsigned d = 5, const char *name = "Mat2") const { fprintf(stdout, "%s:\n %.*f, %.*f\n %.*f, %.*f\n", name, d, this->m[0], d, this->m[1], d, this->m[2], d, this->m[3]); }
    // This it to use with std strings and the operator <<
    friend std::ostream &operator<<(std::ostream &os, Mat2 mat) { return (os << std::endl
                                                                             << mat.m[0] << ", " << mat.m[1] << std::endl
                                                                             << mat.m[2] << ", " << mat.m[3]); }
    ///////////////////////////////////////////////////////////////////////////

    // Constructors
    ///////////////////////////////////////////////////////////////////////////
    inline Mat2(float value = 0);
    inline Mat2(const float *a); // Need 4 values
    inline Mat2(const Mat2 &copy);
    inline ~Mat2();
    ///////////////////////////////////////////////////////////////////////////

    // Operators
    ///////////////////////////////////////////////////////////////////////////
    inline void operator=(float value);
    inline void operator=(const float *a); // Need 4 values
    inline void operator=(Mat2 copy);

    inline Mat2 operator+(Mat2 other) const;
    inline Mat2 &operator+=(Mat2 other);

    inline Mat2 operator+(float value) const;
    inline Mat2 &operator+=(float value);

    inline Mat2 operator-() const;
    inline Mat2 operator-(Mat2 other) const;
    inline Mat2 &operator-=(Mat2 other);

    inline Mat2 operator-(float value) const;
    inline Mat2 &operator-=(float value);

    inline Mat2 operator*(Mat2 other) const;
    inline Mat2 &operator*=(Mat2 other);

    inline Mat2 operator*(float value) const;
    inline Mat2 &operator*=(float value);

    inline Mat2 operator/(float value) const;
    inline Mat2 &operator/=(float value);

    inline bool operator==(float value) const;
    inline bool operator==(const float *a) const; // Need 4 values
    inline bool operator==(Mat2 other) const;

    inline bool operator!=(float value) const;
    inline bool operator!=(const float *a) const; // Need 4 values
    inline bool operator!=(Mat2 other) const;

    /*
      In use with array make sure the first one is for the array number
      and the second one is for the vector number
    */
    inline float operator[](size_t index) const;
    inline float &operator[](size_t index);
    ///////////////////////////////////////////////////////////////////////////

    // Static methods
    ///////////////////////////////////////////////////////////////////////////
    inline static Mat2 Identity();
    inline static float Determinant(const float *value); // Need 4 values
    ///////////////////////////////////////////////////////////////////////////

    // Methods
    ///////////////////////////////////////////////////////////////////////////
    inline float Determinant() const;

    inline Vec2 GetLine(size_t line) const;
    inline Vec2 GetColumn(size_t column) const;

    inline Mat2 Adjoint() const;

    inline Mat2 Inverse() const;

    inline Mat2 Transpose() const;
    ///////////////////////////////////////////////////////////////////////////

    // Static Attributes
    ///////////////////////////////////////////////////////////////////////////
    static const Mat2 zero, one;
    ///////////////////////////////////////////////////////////////////////////

    // Attributes
    ///////////////////////////////////////////////////////////////////////////
    float m[4];
    ///////////////////////////////////////////////////////////////////////////
  };

  // Implementation
  ///////////////////////////////////////////////////////////////////////////////

  // Constructors
  Mat2::Mat2(float value) { std::fill(std::begin(this->m), std::end(this->m), value); }
  Mat2::Mat2(const float *a) { std::memcpy(this->m, a, sizeof(this->m)); }
  Mat2::Mat2(const Mat2 &copy) { std::memcpy(this->m, copy.m, sizeof(this->m)); }

  // Destructor
  Mat2::~Mat2() { std::memset(this->m, 0, sizeof(this->m)); }

  // Operators
  void Mat2::operator=(float value) { std::fill(std::begin(this->m), std::end(this->m), value); }
  void Mat2::operator=(const float *a) { std::memcpy(this->m, a, sizeof(this->m)); }
  void Mat2::operator=(Mat2 copy) { std::memcpy(this->m, copy.m, sizeof(this->m)); }

  Mat2 Mat2::operator+(Mat2 other) const
  {
    Mat2 ret;
    for (size_t i = 0; i < 4; i++)
      ret.m[i] = this->m[i] + other.m[i];

    return ret;
  }

  Mat2 &Mat2::operator+=(Mat2 other)
  {
    for (size_t i = 0; i < 4; i++)
      this->m[i] += other.m[i];

    return (*this);
  }

  Mat2 Mat2::operator+(float value) const
  {
    Mat2 ret;
    for (size_t i = 0; i < 4; i++)
      ret.m[i] = this->m[i] + value;

    return ret;
  }

  Mat2 &Mat2::operator+=(float value)
  {
    for (size_t i = 0; i < 4; i++)
      this->m[i] += value;

    return (*this);
  }

  Mat2 Mat2::operator-() const
  {
    Mat2 ret;
    for (size_t i = 0; i < 4; i++)
      ret.m[i] = -this->m[i];

    return ret;
  }

  Mat2 Mat2::operator-(Mat2 other) const
  {
    Mat2 ret;
    for (size_t i = 0; i < 4; i++)
      ret.m[i] = this->m[i] - other.m[i];

    return ret;
  }

  Mat2 &Mat2::operator-=(Mat2 other)
  {
    for (size_t i = 0; i < 4; i++)
      this->m[i] -= other.m[i];

    return (*this);
  }

  Mat2 Mat2::operator-(float value) const
  {
    Mat2 ret;
    for (size_t i = 0; i < 4; i++)
      ret.m[i] = this->m[i] - value;

    return Mat2(ret);
  }

  Mat2 &Mat2::operator-=(float value)
  {
    for (size_t i = 0; i < 4; i++)
      this->m[i] -= value;

    return (*this);
  }

  Mat2 Mat2::operator*(Mat2 other) const
  {
    float ret[4] = {
        (this->m[0] * other.m[0] + this->m[1] * other.m[2]),
        (this->m[0] * other.m[1] + this->m[1] * other.m[3]),
        (this->m[2] * other.m[0] + this->m[3] * other.m[2]),
        (this->m[2] * other.m[1] + this->m[3] * other.m[3])};

    return Mat2(ret);
  }

  Mat2 &Mat2::operator*=(Mat2 other)
  {
    float ret[4] = {
        (this->m[0] * other.m[0] + this->m[1] * other.m[2]),
        (this->m[0] * other.m[1] + this->m[1] * other.m[3]),
        (this->m[2] * other.m[0] + this->m[3] * other.m[2]),
        (this->m[2] * other.m[1] + this->m[3] * other.m[3])};

    for (size_t i = 0; i < 4; i++)
      this->m[i] = ret[i];

    return (*this);
  }

  Mat2 Mat2::operator*(float value) const
  {
    Mat2 ret;
    for (size_t i = 0; i < 4; i++)
      ret.m[i] = this->m[i] * value;

    return ret;
  }

  Mat2 &Mat2::operator*=(float value)
  {
    for (size_t i = 0; i < 4; i++)
      this->m[i] *= value;

    return (*this);
  }

  Mat2 Mat2::operator/(float value) const
  {
    Mat2 ret;
    float rec_value = 1.0f / value;
    for (size_t i = 0; i < 4; i++)
      ret.m[i] = this->m[i] * rec_value;

    return ret;
  }

  Mat2 &Mat2::operator/=(float value)
  {
    float rec_value = 1.0f / value;

    for (size_t i = 0; i < 4; i++)
      this->m[i] *= rec_value;

    return (*this);
  }

  bool Mat2::operator==(float value) const
  {
    for (size_t i = 0; i < 4; i++)
      if (this->m[i] != value)
        return false;

    return true;
  }

  bool Mat2::operator==(const float *a) const
  {
    for (size_t i = 0; i < 4; i++)
      if (this->m[i] != a[i])
        return false;

    return true;
  }

  bool Mat2::operator==(Mat2 other) const
  {
    for (size_t i = 0; i < 4; i++)
      if (this->m[i] != other.m[i])
        return false;

    return true;
  }

  bool Mat2::operator!=(float value) const
  {
    for (size_t i = 0; i < 4; i++)
      if (this->m[i] == value)
        return false;

    return true;
  }

  bool Mat2::operator!=(const float *a) const
  {
    for (size_t i = 0; i < 4; i++)
      if (this->m[i] == a[i])
        return false;

    return true;
  }

  bool Mat2::operator!=(Mat2 other) const
  {
    for (size_t i = 0; i < 4; i++)
      if (this->m[i] == other.m[i])
        return false;

    return true;
  }

  float Mat2::operator[](size_t index) const
  {
    if (index < 4)
      return m[index];
    else
      throw std::out_of_range("Ínvalid index reading Mat2");
  }

  float &Mat2::operator[](size_t index)
  {
    if (index < 4)
      return m[index];
    else
      throw std::out_of_range("Ínvalid index reading Mat2");
  }

  // Methods
  Mat2 Mat2::Identity()
  {
    float ret[4] = {
        1, 0,
        0, 1};

    return Mat2(ret);
  }

  float Mat2::Determinant() const
  {
    return ((this->m[0] * this->m[3]) - (this->m[2] * this->m[1]));
  }

  float Mat2::Determinant(const float *value)
  {
    return ((value[0] * value[3]) - (value[2] * value[1]));
  }

  Mat2 Mat2::Inverse() const
  {
    Mat2 ret = this->Adjoint();
    float det = this->Determinant();

    if (det == 0)
      return Mat2::zero;

    ret /= det;

    return ret;
  }

  Mat2 Mat2::Adjoint() const
  {
    float ret[4] = {
        this->m[3], -this->m[1],
        -this->m[2], this->m[0]};

    return Mat2(ret);
  }

  Mat2 Mat2::Transpose() const
  {
    float ret[4] = {this->m[0], this->m[2], this->m[1], this->m[3]};

    return Mat2(ret);
  }

  Vec2 Mat2::GetLine(size_t line) const
  {
    if (line < 2)
      return Vec2(this->m[0 + line * 2], this->m[1 + line * 2]);
    else
      return Vec2::zero;
  }

  Vec2 Mat2::GetColumn(size_t column) const
  {
    if (column < 2)
      return Vec2(this->m[0 + column], this->m[2 + column]);
    else
      return Vec2::zero;
  }

}

#endif /* __MATRIX2_H__ */
