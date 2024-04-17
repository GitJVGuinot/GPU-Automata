/**
 * @author Javier Guinot Almenar <guinotal@esat-alumni.com>
 *
 * @file matrix_3.h
 *
 * @brief Matrix 3 Class Definition.
 * Order:
 *  C0 C1 C2
 *
 *  m0 m1 m2    L0
 *  m3 m4 m5    L1
 *  m6 m7 m8    L2
 *
 */

#include "mathlib.h"

#ifndef __MATRIX3_H__
#define __MATRIX3_H__ 1

namespace Math
{
  class Mat3
  {
  public:
    // Console
    ///////////////////////////////////////////////////////////////////////////
    inline void print(unsigned d = 5, const char *name = "Mat3") const { fprintf(stdout, "%s:\n %.*f, %.*f, %.*f\n %.*f, %.*f, %.*f\n %.*f, %.*f, %.*f\n",
                                                                            name, d, this->m[0], d, this->m[1], d, this->m[2], d, this->m[3],
                                                                            d, this->m[4], d, this->m[5], d, this->m[6], d, this->m[7], d, this->m[8]); }
    // This it to use with std strings and the operator <<
    friend std::ostream &operator<<(std::ostream &os, Mat3 mat) { return (os << std::endl
                                                                             << mat.m[0] << ", " << mat.m[1] << ", " << mat.m[2] << std::endl
                                                                             << mat.m[3] << ", " << mat.m[4] << ", " << mat.m[5] << std::endl
                                                                             << mat.m[6] << ", " << mat.m[7] << ", " << mat.m[8]); }
    ///////////////////////////////////////////////////////////////////////////

    // Constructors
    ///////////////////////////////////////////////////////////////////////////
    inline Mat3(float value = 0);
    inline Mat3(const float *a); // Need 9 values
    inline Mat3(const Mat3 &copy);
    inline ~Mat3();
    ///////////////////////////////////////////////////////////////////////////

    // Operators
    ///////////////////////////////////////////////////////////////////////////
    inline void operator=(float value);
    inline void operator=(const float *a); // Need 9 values
    inline void operator=(Mat3 copy);

    inline Mat3 operator+(Mat3 other) const;
    inline Mat3 &operator+=(Mat3 other);

    inline Mat3 operator+(float value) const;
    inline Mat3 &operator+=(float value);

    inline Mat3 operator-() const;
    inline Mat3 operator-(Mat3 other) const;
    inline Mat3 &operator-=(Mat3 other);

    inline Mat3 operator-(float value) const;
    inline Mat3 &operator-=(float value);

    inline Mat3 operator*(Mat3 other) const;
    inline Mat3 &operator*=(Mat3 other);

    inline Mat3 operator*(float value) const;
    inline Mat3 &operator*=(float value);

    inline Mat3 operator/(float value) const;
    inline Mat3 &operator/=(float value);

    inline bool operator==(float value) const;
    inline bool operator==(const float *a) const; // Need 9 values
    inline bool operator==(Mat3 other) const;

    inline bool operator!=(float value) const;
    inline bool operator!=(const float *a) const; // Need 9 values
    inline bool operator!=(Mat3 other) const;

    /*
      In use with array make sure the first one is for the array number
      and the second one is for the vector number
    */
    inline float operator[](size_t index) const;
    inline float &operator[](size_t index);
    ///////////////////////////////////////////////////////////////////////////

    // Static Methods
    ///////////////////////////////////////////////////////////////////////////
    inline static Mat3 Identity();
    inline static float Determinant(const float *value);

    inline static Mat3 Translate(Vec2 position);
    inline static Mat3 Translate(float x, float y);

    inline static Mat3 Scale(Vec2 scale);
    inline static Mat3 Scale(float x, float y);

    inline static Mat3 Rotate(float rotation);

    inline static Mat3 Transform(float rotation = 0, Vec2 scale = Vec2(1, 1), Vec2 translate = Vec2(0, 0));
    inline static Mat3 Transform(float rotation = 0, float scaleX = 1, float scaleY = 1, float transX = 0, float transY = 0);
    ///////////////////////////////////////////////////////////////////////////

    // Methods
    ///////////////////////////////////////////////////////////////////////////
    inline float Determinant() const;

    inline Vec3 GetLine(size_t line) const;
    inline Vec3 GetColumn(size_t column) const;

    inline Mat3 Adjoint() const;

    inline Mat3 Inverse() const;

    inline Mat3 Transpose() const;
    ///////////////////////////////////////////////////////////////////////////

    // Static Attributes
    ///////////////////////////////////////////////////////////////////////////
    static const Mat3 zero, one;
    ///////////////////////////////////////////////////////////////////////////

    // Attributes
    ///////////////////////////////////////////////////////////////////////////
    float m[9];
    ///////////////////////////////////////////////////////////////////////////
  };

  // Implementation
  ///////////////////////////////////////////////////////////////////////////////

  // Constructors
  Mat3::Mat3(float value) { std::fill(std::begin(this->m), std::end(this->m), value); }
  Mat3::Mat3(const float *a) { std::memcpy(this->m, a, sizeof(this->m)); }
  Mat3::Mat3(const Mat3 &copy) { std::memcpy(this->m, copy.m, sizeof(this->m)); }

  // Destructor
  Mat3::~Mat3() { std::memset(this->m, 0, sizeof(this->m)); }

  // Operators
  void Mat3::operator=(float value) { std::fill(std::begin(this->m), std::end(this->m), value); }
  void Mat3::operator=(const float *a) { std::memcpy(this->m, a, sizeof(this->m)); }
  void Mat3::operator=(Mat3 copy) { std::memcpy(this->m, copy.m, sizeof(this->m)); }

  Mat3 Mat3::operator+(Mat3 other) const
  {
    Mat3 ret;
    for (size_t i = 0; i < 9; i++)
      ret.m[i] = this->m[i] + other.m[i];

    return ret;
  }

  Mat3 &Mat3::operator+=(Mat3 other)
  {
    for (size_t i = 0; i < 9; i++)
      this->m[i] += other.m[i];

    return (*this);
  }

  Mat3 Mat3::operator+(float value) const
  {
    Mat3 ret;
    for (size_t i = 0; i < 9; i++)
      ret.m[i] = this->m[i] + value;

    return ret;
  }

  Mat3 &Mat3::operator+=(float value)
  {
    for (size_t i = 0; i < 9; i++)
      this->m[i] += value;

    return (*this);
  }

  Mat3 Mat3::operator-() const
  {
    Mat3 ret;
    for (size_t i = 0; i < 9; i++)
      ret.m[i] = -this->m[i];

    return ret;
  }

  Mat3 Mat3::operator-(Mat3 other) const
  {
    Mat3 ret;
    for (size_t i = 0; i < 9; i++)
      ret.m[i] = this->m[i] - other.m[i];

    return ret;
  }

  Mat3 &Mat3::operator-=(Mat3 other)
  {
    for (size_t i = 0; i < 9; i++)
      this->m[i] -= other.m[i];

    return (*this);
  }

  Mat3 Mat3::operator-(float value) const
  {
    Mat3 ret;
    for (size_t i = 0; i < 9; i++)
      ret.m[i] = this->m[i] - value;

    return ret;
  }

  Mat3 &Mat3::operator-=(float value)
  {
    for (size_t i = 0; i < 9; i++)
      this->m[i] -= value;

    return (*this);
  }

  Mat3 Mat3::operator*(Mat3 other) const
  {
    float ret[9] = {
        (this->m[0] * other.m[0] + this->m[1] * other.m[3] + this->m[2] * other.m[6]),
        (this->m[0] * other.m[1] + this->m[1] * other.m[4] + this->m[2] * other.m[7]),
        (this->m[0] * other.m[2] + this->m[1] * other.m[5] + this->m[2] * other.m[8]),
        (this->m[3] * other.m[0] + this->m[4] * other.m[3] + this->m[5] * other.m[6]),
        (this->m[3] * other.m[1] + this->m[4] * other.m[4] + this->m[5] * other.m[7]),
        (this->m[3] * other.m[2] + this->m[4] * other.m[5] + this->m[5] * other.m[8]),
        (this->m[6] * other.m[0] + this->m[7] * other.m[3] + this->m[8] * other.m[6]),
        (this->m[6] * other.m[1] + this->m[7] * other.m[4] + this->m[8] * other.m[7]),
        (this->m[6] * other.m[2] + this->m[7] * other.m[5] + this->m[8] * other.m[8]),
    };

    return Mat3(ret);
  }

  Mat3 &Mat3::operator*=(Mat3 other)
  {
    float ret[9] = {
        (this->m[0] * other.m[0] + this->m[1] * other.m[3] + this->m[2] * other.m[6]),
        (this->m[0] * other.m[1] + this->m[1] * other.m[4] + this->m[2] * other.m[7]),
        (this->m[0] * other.m[2] + this->m[1] * other.m[5] + this->m[2] * other.m[8]),
        (this->m[3] * other.m[0] + this->m[4] * other.m[3] + this->m[5] * other.m[6]),
        (this->m[3] * other.m[1] + this->m[4] * other.m[4] + this->m[5] * other.m[7]),
        (this->m[3] * other.m[2] + this->m[4] * other.m[5] + this->m[5] * other.m[8]),
        (this->m[6] * other.m[0] + this->m[7] * other.m[3] + this->m[8] * other.m[6]),
        (this->m[6] * other.m[1] + this->m[7] * other.m[4] + this->m[8] * other.m[7]),
        (this->m[6] * other.m[2] + this->m[7] * other.m[5] + this->m[8] * other.m[8]),
    };

    for (size_t i = 0; i < 9; i++)
      this->m[i] = ret[i];

    return (*this);
  }

  Mat3 Mat3::operator*(float value) const
  {
    Mat3 ret;
    for (size_t i = 0; i < 9; i++)
      ret.m[i] = this->m[i] * value;

    return ret;
  }

  Mat3 &Mat3::operator*=(float value)
  {
    for (size_t i = 0; i < 9; i++)
      this->m[i] *= value;

    return (*this);
  }

  Mat3 Mat3::operator/(float value) const
  {
    Mat3 ret;
    float rec_value = 1.0f / value;
    for (size_t i = 0; i < 9; i++)
      ret.m[i] = this->m[i] * rec_value;

    return ret;
  }

  Mat3 &Mat3::operator/=(float value)
  {
    float rec_value = 1.0f / value;
    for (size_t i = 0; i < 9; i++)
      this->m[i] *= rec_value;

    return (*this);
  }

  bool Mat3::operator==(float value) const
  {
    for (size_t i = 0; i < 9; ++i)
      if (this->m[i] != value)
        return false;

    return true;
  }

  bool Mat3::operator==(const float *a) const
  {
    for (size_t i = 0; i < 9; ++i)
      if (this->m[i] != a[i])
        return false;

    return true;
  }

  bool Mat3::operator==(Mat3 other) const
  {
    for (size_t i = 0; i < 9; ++i)
      if (this->m[i] != other.m[i])
        return false;

    return true;
  }

  bool Mat3::operator!=(float value) const
  {
    for (size_t i = 0; i < 9; ++i)
      if (this->m[i] != value)
        return true;

    return false;
  }

  bool Mat3::operator!=(const float *a) const
  {
    for (size_t i = 0; i < 9; ++i)
      if (this->m[i] != a[i])
        return true;

    return false;
  }

  bool Mat3::operator!=(Mat3 other) const
  {
    for (size_t i = 0; i < 9; ++i)
      if (this->m[i] != other.m[i])
        return true;

    return false;
  }

  float Mat3::operator[](size_t index) const
  {
    if (index < 9)
      return m[index];
    else
      throw std::out_of_range("Ínvalid index reading Mat3");
  }

  float &Mat3::operator[](size_t index)
  {
    if (index < 9)
      return m[index];
    else
      throw std::out_of_range("Ínvalid index reading Mat3");
  }

  // Methods
  Mat3 Mat3::Identity()
  {
    float ret[9] = {
        1, 0, 0,
        0, 1, 0,
        0, 0, 1};

    return Mat3(ret);
  }

  float Mat3::Determinant() const
  {
    return (
        (
            (this->m[0] * this->m[4] * this->m[8]) +
            (this->m[3] * this->m[7] * this->m[2]) +
            (this->m[6] * this->m[1] * this->m[5])) -
        ((this->m[2] * this->m[4] * this->m[6]) +
         (this->m[5] * this->m[7] * this->m[0]) +
         (this->m[8] * this->m[1] * this->m[3])));
  }

  float Mat3::Determinant(const float *value)
  {
    return (
        (
            (value[0] * value[4] * value[8]) +
            (value[3] * value[7] * value[2]) +
            (value[6] * value[1] * value[5])) -
        ((value[2] * value[4] * value[6]) +
         (value[5] * value[7] * value[0]) +
         (value[8] * value[1] * value[3])));
  }

  Mat3 Mat3::Inverse() const
  {
    Mat3 ret = this->Adjoint();
    float det = this->Determinant();

    if (det == 0)
      return Mat3::zero;

    ret /= det;

    return ret;
  }

  Mat3 Mat3::Translate(Vec2 mov)
  {
    float ret[9] = {
        1, 0, 0,
        0, 1, 0,
        mov.x, mov.y, 1};

    return Mat3(ret);
  }

  Mat3 Mat3::Translate(float a, float b)
  {
    float ret[9] = {
        1, 0, 0,
        0, 1, 0,
        a, b, 1};

    return Mat3(ret);
  }

  Mat3 Mat3::Scale(Vec2 scale)
  {
    float f[9] = {
        scale.x, 0, 0,
        0, scale.y, 0,
        0, 0, 1};

    return Mat3(f);
  }

  Mat3 Mat3::Scale(float x, float y)
  {
    float f[9] = {
        x, 0, 0,
        0, y, 0,
        0, 0, 1};

    return Mat3(f);
  }

  Mat3 Mat3::Rotate(float rotation)
  {
    float cos_ = std::cos(rotation);
    float sin_ = std::sin(rotation);

    float rot[9] = {
        cos_, sin_, 0,
        -sin_, cos_, 0,
        0, 0, 1};

    return Mat3(rot);
  }

  Mat3 Mat3::Transform(float rotation, Vec2 scale, Vec2 translate)
  {
    return Translate(translate) * Rotate(rotation) * Scale(scale);
  }

  Mat3 Mat3::Transform(float rotation, float scaleX, float scaleY, float transX, float transY)
  {
    return Translate(transX, transY) * Rotate(rotation) * Scale(scaleX, scaleY);
  }

  Mat3 Mat3::Adjoint() const
  {

    float det_0[4] = {this->m[4], this->m[5], this->m[7], this->m[8]};
    float det_1[4] = {this->m[3], this->m[5], this->m[6], this->m[8]};
    float det_2[4] = {this->m[3], this->m[4], this->m[6], this->m[7]};
    float det_3[4] = {this->m[1], this->m[2], this->m[7], this->m[8]};
    float det_4[4] = {this->m[0], this->m[2], this->m[6], this->m[8]};
    float det_5[4] = {this->m[0], this->m[1], this->m[6], this->m[7]};
    float det_6[4] = {this->m[1], this->m[2], this->m[4], this->m[5]};
    float det_7[4] = {this->m[0], this->m[2], this->m[3], this->m[5]};
    float det_8[4] = {this->m[0], this->m[1], this->m[3], this->m[4]};

    float ret[9] = {
        Mat2::Determinant(det_0),
        -Mat2::Determinant(det_1),
        Mat2::Determinant(det_2),
        -Mat2::Determinant(det_3),
        Mat2::Determinant(det_4),
        -Mat2::Determinant(det_5),
        Mat2::Determinant(det_6),
        -Mat2::Determinant(det_7),
        Mat2::Determinant(det_8)};

    return Mat3(ret).Transpose();
  }

  Mat3 Mat3::Transpose() const
  {
    float ret[9] = {
        this->m[0],
        this->m[3],
        this->m[6],
        this->m[1],
        this->m[4],
        this->m[7],
        this->m[2],
        this->m[5],
        this->m[8]};

    return Mat3(ret);
  }

  Vec3 Mat3::GetLine(size_t line) const
  {
    if (line < 3)
      return Vec3(this->m[0 + line * 3], this->m[1 + line * 3], this->m[2 + line * 3]);
    else
      return Vec3();
  }

  Vec3 Mat3::GetColumn(size_t column) const
  {
    if (column < 3)
      return Vec3(this->m[0 + column], this->m[3 + column], this->m[6 + column]);
    else
      return Vec3();
  }

}

#endif /* __MATRIX3_H__ */
