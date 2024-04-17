/**
 * @author Javier Guinot Almenar <guinotal@esat-alumni.com>
 *
 * @file matrix_4.h
 *
 * @brief Matrix 4 Class Definition.
 * Order:
 *  m0  m1  m2  m3
 *  m4  m5  m6  m7
 *  m8  m9  m10 m11
 *  m12 m13 m14 m15
 *
 */

#include "mathlib.h"

#ifndef __MATRIX4_H__
#define __MATRIX4_H__ 1

namespace Math
{
  class Mat4
  {
  public:
    // Console
    ///////////////////////////////////////////////////////////////////////////
    inline void print(unsigned d = 5, const char *name = "Mat4") const { fprintf(stdout, "%s:\n %.*f, %.*f, %.*f, %.*f\n %.*f, %.*f, %.*f, %.*f\n %.*f, %.*f, %.*f, %.*f\n %.*f, %.*f, %.*f, %.*f\n", name,
                                                                            d, this->m[0], d, this->m[1], d, this->m[2], d, this->m[3],
                                                                            d, this->m[4], d, this->m[5], d, this->m[6], d, this->m[7],
                                                                            d, this->m[8], d, this->m[9], d, this->m[10], d, this->m[11],
                                                                            d, this->m[12], d, this->m[13], d, this->m[14], d, this->m[15]); }
    // This it to use with std strings and the operator <<
    friend std::ostream &operator<<(std::ostream &os, Mat4 mat) { return (os << std::endl
                                                                             << mat.m[0] << ", " << mat.m[1] << ", " << mat.m[2] << ", " << mat.m[3] << std::endl
                                                                             << mat.m[4] << ", " << mat.m[5] << ", " << mat.m[6] << ", " << mat.m[7] << std::endl
                                                                             << mat.m[8] << ", " << mat.m[9] << ", " << mat.m[10] << ", " << mat.m[11] << std::endl
                                                                             << mat.m[12] << ", " << mat.m[13] << ", " << mat.m[14] << ", " << mat.m[15]); }
    ///////////////////////////////////////////////////////////////////////////

    // Constructors
    ///////////////////////////////////////////////////////////////////////////
    inline Mat4(float value = 0);
    inline Mat4(const float *a); // Need 16 values
    inline Mat4(const Mat4 &copy);
    inline ~Mat4();
    ///////////////////////////////////////////////////////////////////////////

    // Operators
    ///////////////////////////////////////////////////////////////////////////
    inline void operator=(float value);
    inline void operator=(const float *a); // Need 16 values
    inline void operator=(Mat4 copy);

    inline Mat4 operator+(Mat4 other) const;
    inline Mat4 &operator+=(Mat4 other);

    inline Mat4 operator+(float value) const;
    inline Mat4 &operator+=(float value);

    inline Mat4 operator-() const;
    inline Mat4 operator-(Mat4 other) const;
    inline Mat4 &operator-=(Mat4 other);

    inline Mat4 operator-(float value) const;
    inline Mat4 &operator-=(float value);

    inline Mat4 operator*(Mat4 other) const;
    inline Mat4 &operator*=(Mat4 other);

    inline Mat4 operator*(float value) const;
    inline Mat4 &operator*=(float value);

    inline Mat4 operator/(float value) const;
    inline Mat4 &operator/=(float value);

    inline bool operator==(float value) const;
    inline bool operator==(const float *a) const; // Need 16 values
    inline bool operator==(Mat4 other) const;

    inline bool operator!=(float value) const;
    inline bool operator!=(const float *a) const; // Need 16 values
    inline bool operator!=(Mat4 other) const;

    /*
      In use with array make sure the first one is for the array number
      and the second one is for the vector number
    */
    inline float operator[](size_t index) const;
    inline float &operator[](size_t index);
    ///////////////////////////////////////////////////////////////////////////

    // Static Methods
    ///////////////////////////////////////////////////////////////////////////
    inline static Mat4 Identity();
    inline static Mat4 Projection();
    inline static float Determinant(const float *value);

    inline static Mat4 Translate(Vec3 distance);
    inline static Mat4 Translate(float x, float y, float z);

    inline static Mat4 Scale(Vec3 scale);
    inline static Mat4 Scale(float x, float y, float z);

    inline static Mat4 RotateX(float radians);
    inline static Mat4 RotateY(float radians);
    inline static Mat4 RotateZ(float radians);
    inline static Mat4 Rotate(Vec3 radians);
    inline static Mat4 Rotate(float radX, float radY, float radZ);

    inline static Mat4 Transform(Vec3 rotation = Vec3::zero, Vec3 scale = Vec3::one, Vec3 translate = Vec3::zero);
    inline static Mat4 Transform(float rotX = 0, float rotY = 0, float rotZ = 0,
                                 float scaleX = 1, float scaleY = 1, float scaleZ = 1,
                                 float transX = 0, float transY = 0, float transZ = 0);

    inline static Mat4 ViewMatrix(Vec3 camara, Vec3 mira, Vec3 up);

    inline static Mat4 PerspectiveMatrix(float fov, float aspect,
                                         float near, float far);

    inline static Mat4 OrthoMatrix(float right, float left,
                                   float top, float bottom,
                                   float near, float far);
    ///////////////////////////////////////////////////////////////////////////

    // Methods
    ///////////////////////////////////////////////////////////////////////////
    inline float Determinant() const;

    inline Vec4 GetLine(size_t line) const;
    inline Vec4 GetColumn(size_t column) const;

    inline Mat4 Adjoint() const;

    inline Mat4 Inverse() const;

    inline Mat4 Transpose() const;
    ///////////////////////////////////////////////////////////////////////////

    // Static Attributes
    ///////////////////////////////////////////////////////////////////////////
    static const Mat4 zero, one;
    ///////////////////////////////////////////////////////////////////////////

    // Attributes
    ///////////////////////////////////////////////////////////////////////////
    float m[16];
    ///////////////////////////////////////////////////////////////////////////
  };

  // Implementation
  ///////////////////////////////////////////////////////////////////////////////
  // Constructors
  Mat4::Mat4(float value) { std::fill(std::begin(this->m), std::end(this->m), value); }
  Mat4::Mat4(const float *a) { std::memcpy(this->m, a, sizeof(this->m)); }
  Mat4::Mat4(const Mat4 &copy) { std::memcpy(this->m, copy.m, sizeof(this->m)); }

  // Destructor
  Mat4::~Mat4() { std::memset(this->m, 0, sizeof(this->m)); }

  // Operators
  void Mat4::operator=(float value) { std::fill(std::begin(this->m), std::end(this->m), value); }
  void Mat4::operator=(const float *a) { std::memcpy(this->m, a, sizeof(this->m)); }
  void Mat4::operator=(Mat4 copy) { std::memcpy(this->m, copy.m, sizeof(this->m)); }

  Mat4 Mat4::operator+(Mat4 other) const
  {
    Mat4 ret;
    for (size_t i = 0; i < 16; i++)
      ret.m[i] = this->m[i] + other.m[i];

    return ret;
  }

  Mat4 &Mat4::operator+=(Mat4 other)
  {
    for (size_t i = 0; i < 16; i++)
      this->m[i] += other.m[i];

    return (*this);
  }

  Mat4 Mat4::operator+(float value) const
  {
    Mat4 ret;
    for (size_t i = 0; i < 16; i++)
      ret.m[i] = this->m[i] + value;

    return ret;
  }

  Mat4 &Mat4::operator+=(float value)
  {
    for (size_t i = 0; i < 16; i++)
      this->m[i] += value;

    return (*this);
  }

  Mat4 Mat4::operator-() const
  {
    Mat4 ret;
    for (size_t i = 0; i < 16; i++)
      ret.m[i] = -this->m[i];

    return ret;
  }

  Mat4 Mat4::operator-(Mat4 other) const
  {
    Mat4 ret;
    for (size_t i = 0; i < 16; i++)
      ret.m[i] = this->m[i] - other.m[i];

    return ret;
  }

  Mat4 &Mat4::operator-=(Mat4 other)
  {
    for (size_t i = 0; i < 16; i++)
      this->m[i] -= other.m[i];

    return (*this);
  }

  Mat4 Mat4::operator-(float value) const
  {
    Mat4 ret;
    for (size_t i = 0; i < 16; i++)
      ret.m[i] = this->m[i] - value;

    return ret;
  }

  Mat4 &Mat4::operator-=(float value)
  {
    for (size_t i = 0; i < 16; i++)
      this->m[i] -= value;

    return (*this);
  }

  Mat4 Mat4::operator*(Mat4 other) const
  {
    float ret[16] = {
        (this->m[0] * other.m[0] + this->m[1] * other.m[4] + this->m[2] * other.m[8] + this->m[3] * other.m[12]),
        (this->m[0] * other.m[1] + this->m[1] * other.m[5] + this->m[2] * other.m[9] + this->m[3] * other.m[13]),
        (this->m[0] * other.m[2] + this->m[1] * other.m[6] + this->m[2] * other.m[10] + this->m[3] * other.m[14]),
        (this->m[0] * other.m[3] + this->m[1] * other.m[7] + this->m[2] * other.m[11] + this->m[3] * other.m[15]),
        (this->m[4] * other.m[0] + this->m[5] * other.m[4] + this->m[6] * other.m[8] + this->m[7] * other.m[12]),
        (this->m[4] * other.m[1] + this->m[5] * other.m[5] + this->m[6] * other.m[9] + this->m[7] * other.m[13]),
        (this->m[4] * other.m[2] + this->m[5] * other.m[6] + this->m[6] * other.m[10] + this->m[7] * other.m[14]),
        (this->m[4] * other.m[3] + this->m[5] * other.m[7] + this->m[6] * other.m[11] + this->m[7] * other.m[15]),
        (this->m[8] * other.m[0] + this->m[9] * other.m[4] + this->m[10] * other.m[8] + this->m[11] * other.m[12]),
        (this->m[8] * other.m[1] + this->m[9] * other.m[5] + this->m[10] * other.m[9] + this->m[11] * other.m[13]),
        (this->m[8] * other.m[2] + this->m[9] * other.m[6] + this->m[10] * other.m[10] + this->m[11] * other.m[14]),
        (this->m[8] * other.m[3] + this->m[9] * other.m[7] + this->m[10] * other.m[11] + this->m[11] * other.m[15]),
        (this->m[12] * other.m[0] + this->m[13] * other.m[4] + this->m[14] * other.m[8] + this->m[15] * other.m[12]),
        (this->m[12] * other.m[1] + this->m[13] * other.m[5] + this->m[14] * other.m[9] + this->m[15] * other.m[13]),
        (this->m[12] * other.m[2] + this->m[13] * other.m[6] + this->m[14] * other.m[10] + this->m[15] * other.m[14]),
        (this->m[12] * other.m[3] + this->m[13] * other.m[7] + this->m[14] * other.m[11] + this->m[15] * other.m[15]),
    };

    return Mat4(ret);
  }

  Mat4 &Mat4::operator*=(Mat4 other)
  {

    float ret[16] = {
        (this->m[0] * other.m[0] + this->m[1] * other.m[4] + this->m[2] * other.m[8] + this->m[3] * other.m[12]),
        (this->m[0] * other.m[1] + this->m[1] * other.m[5] + this->m[2] * other.m[9] + this->m[3] * other.m[13]),
        (this->m[0] * other.m[2] + this->m[1] * other.m[6] + this->m[2] * other.m[10] + this->m[3] * other.m[14]),
        (this->m[0] * other.m[3] + this->m[1] * other.m[7] + this->m[2] * other.m[11] + this->m[3] * other.m[15]),
        (this->m[4] * other.m[0] + this->m[5] * other.m[4] + this->m[6] * other.m[8] + this->m[7] * other.m[12]),
        (this->m[4] * other.m[1] + this->m[5] * other.m[5] + this->m[6] * other.m[9] + this->m[7] * other.m[13]),
        (this->m[4] * other.m[2] + this->m[5] * other.m[6] + this->m[6] * other.m[10] + this->m[7] * other.m[14]),
        (this->m[4] * other.m[3] + this->m[5] * other.m[7] + this->m[6] * other.m[11] + this->m[7] * other.m[15]),
        (this->m[8] * other.m[0] + this->m[9] * other.m[4] + this->m[10] * other.m[8] + this->m[11] * other.m[12]),
        (this->m[8] * other.m[1] + this->m[9] * other.m[5] + this->m[10] * other.m[9] + this->m[11] * other.m[13]),
        (this->m[8] * other.m[2] + this->m[9] * other.m[6] + this->m[10] * other.m[10] + this->m[11] * other.m[14]),
        (this->m[8] * other.m[3] + this->m[9] * other.m[7] + this->m[10] * other.m[11] + this->m[11] * other.m[15]),
        (this->m[12] * other.m[0] + this->m[13] * other.m[4] + this->m[14] * other.m[8] + this->m[15] * other.m[12]),
        (this->m[12] * other.m[1] + this->m[13] * other.m[5] + this->m[14] * other.m[9] + this->m[15] * other.m[13]),
        (this->m[12] * other.m[2] + this->m[13] * other.m[6] + this->m[14] * other.m[10] + this->m[15] * other.m[14]),
        (this->m[12] * other.m[3] + this->m[13] * other.m[7] + this->m[14] * other.m[11] + this->m[15] * other.m[15]),
    };

    for (size_t i = 0; i < 16; i++)
      this->m[i] = ret[i];

    return (*this);
  }

  Mat4 Mat4::operator*(float value) const
  {
    Mat4 ret;
    for (size_t i = 0; i < 16; i++)
      ret.m[i] = this->m[i] * value;

    return ret;
  }

  Mat4 &Mat4::operator*=(float value)
  {
    for (size_t i = 0; i < 16; i++)
      this->m[i] *= value;

    return (*this);
  }

  Mat4 Mat4::operator/(float value) const
  {
    Mat4 ret;
    float rec_value = 1.0f / value;
    for (size_t i = 0; i < 16; i++)
      ret.m[i] = this->m[i] * rec_value;

    return ret;
  }

  Mat4 &Mat4::operator/=(float value)
  {
    float rec_value = 1.0f / value;
    for (size_t i = 0; i < 16; i++)
      this->m[i] *= rec_value;

    return (*this);
  }

  bool Mat4::operator==(float value) const
  {
    for (size_t i = 0; i < 16; ++i)
      if (this->m[i] != value)
        return false;

    return true;
  }

  bool Mat4::operator==(const float *a) const
  {
    for (size_t i = 0; i < 16; ++i)
      if (this->m[i] != a[i])
        return false;

    return true;
  }

  bool Mat4::operator==(Mat4 other) const
  {
    for (size_t i = 0; i < 16; ++i)
      if (this->m[i] != other.m[i])
        return false;

    return true;
  }

  bool Mat4::operator!=(float value) const
  {
    for (size_t i = 0; i < 16; ++i)
      if (this->m[i] != value)
        return true;

    return false;
  }

  bool Mat4::operator!=(const float *a) const
  {
    for (size_t i = 0; i < 16; ++i)
      if (this->m[i] != a[i])
        return true;

    return false;
  }

  bool Mat4::operator!=(Mat4 other) const
  {
    for (size_t i = 0; i < 16; ++i)
      if (this->m[i] != other.m[i])
        return true;

    return false;
  }

  float Mat4::operator[](size_t index) const
  {
    if (index < 16)
      return m[index];
    else
      throw std::out_of_range("Ínvalid index reading Mat4");
  }

  float &Mat4::operator[](size_t index)
  {
    if (index < 16)
      return m[index];
    else
      throw std::out_of_range("Ínvalid index reading Mat4");
  }

  // Methods
  Mat4 Mat4::Identity()
  {
    float ret[16] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1};

    return Mat4(ret);
  }

  Mat4 Mat4::Projection()
  {
    float ret[16] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 1,
        0, 0, 0, 0};

    return Mat4(ret);
  }

  float Mat4::Determinant(const float *value)
  {
    return (
        (value[0] * value[5] * value[10] * value[15]) +
        (value[0] * value[6] * value[11] * value[13]) +
        (value[0] * value[7] * value[9] * value[14]) -
        (value[0] * value[7] * value[10] * value[13]) -
        (value[0] * value[6] * value[9] * value[15]) -
        (value[0] * value[5] * value[11] * value[14]) -
        (value[1] * value[4] * value[10] * value[15]) -
        (value[2] * value[4] * value[11] * value[13]) -
        (value[3] * value[4] * value[9] * value[14]) +
        (value[3] * value[4] * value[10] * value[13]) +
        (value[2] * value[4] * value[9] * value[15]) +
        (value[1] * value[4] * value[11] * value[14]) +
        (value[1] * value[6] * value[8] * value[15]) +
        (value[2] * value[7] * value[8] * value[13]) +
        (value[3] * value[5] * value[8] * value[14]) -
        (value[3] * value[6] * value[8] * value[13]) -
        (value[2] * value[5] * value[8] * value[15]) -
        (value[1] * value[7] * value[8] * value[14]) -
        (value[1] * value[6] * value[11] * value[12]) -
        (value[2] * value[7] * value[9] * value[12]) -
        (value[3] * value[5] * value[10] * value[12]) +
        (value[3] * value[6] * value[9] * value[12]) +
        (value[2] * value[5] * value[11] * value[12]) +
        (value[1] * value[7] * value[10] * value[12]));
  }

  float Mat4::Determinant() const
  {
    return (
        (this->m[0] * this->m[5] * this->m[10] * this->m[15]) +
        (this->m[0] * this->m[6] * this->m[11] * this->m[13]) +
        (this->m[0] * this->m[7] * this->m[9] * this->m[14]) -
        (this->m[0] * this->m[7] * this->m[10] * this->m[13]) -
        (this->m[0] * this->m[6] * this->m[9] * this->m[15]) -
        (this->m[0] * this->m[5] * this->m[11] * this->m[14]) -
        (this->m[1] * this->m[4] * this->m[10] * this->m[15]) -
        (this->m[2] * this->m[4] * this->m[11] * this->m[13]) -
        (this->m[3] * this->m[4] * this->m[9] * this->m[14]) +
        (this->m[3] * this->m[4] * this->m[10] * this->m[13]) +
        (this->m[2] * this->m[4] * this->m[9] * this->m[15]) +
        (this->m[1] * this->m[4] * this->m[11] * this->m[14]) +
        (this->m[1] * this->m[6] * this->m[8] * this->m[15]) +
        (this->m[2] * this->m[7] * this->m[8] * this->m[13]) +
        (this->m[3] * this->m[5] * this->m[8] * this->m[14]) -
        (this->m[3] * this->m[6] * this->m[8] * this->m[13]) -
        (this->m[2] * this->m[5] * this->m[8] * this->m[15]) -
        (this->m[1] * this->m[7] * this->m[8] * this->m[14]) -
        (this->m[1] * this->m[6] * this->m[11] * this->m[12]) -
        (this->m[2] * this->m[7] * this->m[9] * this->m[12]) -
        (this->m[3] * this->m[5] * this->m[10] * this->m[12]) +
        (this->m[3] * this->m[6] * this->m[9] * this->m[12]) +
        (this->m[2] * this->m[5] * this->m[11] * this->m[12]) +
        (this->m[1] * this->m[7] * this->m[10] * this->m[12]));
  }

  Mat4 Mat4::Adjoint() const
  {

    float det_0[9] = {this->m[5], this->m[6], this->m[7], this->m[9], this->m[10], this->m[11], this->m[13], this->m[14], this->m[15]};
    float det_1[9] = {this->m[4], this->m[6], this->m[7], this->m[8], this->m[10], this->m[11], this->m[12], this->m[14], this->m[15]};
    float det_2[9] = {this->m[4], this->m[5], this->m[7], this->m[8], this->m[9], this->m[11], this->m[12], this->m[13], this->m[15]};
    float det_3[9] = {this->m[4], this->m[5], this->m[6], this->m[8], this->m[9], this->m[10], this->m[12], this->m[13], this->m[14]};
    float det_4[9] = {this->m[1], this->m[2], this->m[3], this->m[9], this->m[10], this->m[11], this->m[13], this->m[14], this->m[15]};
    float det_5[9] = {this->m[0], this->m[2], this->m[3], this->m[8], this->m[10], this->m[11], this->m[12], this->m[14], this->m[15]};
    float det_6[9] = {this->m[0], this->m[1], this->m[3], this->m[8], this->m[9], this->m[11], this->m[12], this->m[13], this->m[15]};
    float det_7[9] = {this->m[0], this->m[1], this->m[2], this->m[8], this->m[9], this->m[10], this->m[12], this->m[13], this->m[14]};
    float det_8[9] = {this->m[1], this->m[2], this->m[3], this->m[5], this->m[6], this->m[7], this->m[13], this->m[14], this->m[15]};
    float det_9[9] = {this->m[0], this->m[2], this->m[3], this->m[4], this->m[6], this->m[7], this->m[12], this->m[14], this->m[15]};
    float det_10[9] = {this->m[0], this->m[1], this->m[3], this->m[4], this->m[5], this->m[7], this->m[12], this->m[13], this->m[15]};
    float det_11[9] = {this->m[0], this->m[1], this->m[2], this->m[4], this->m[5], this->m[6], this->m[12], this->m[13], this->m[14]};
    float det_12[9] = {this->m[1], this->m[2], this->m[3], this->m[5], this->m[6], this->m[7], this->m[9], this->m[10], this->m[11]};
    float det_13[9] = {this->m[0], this->m[2], this->m[3], this->m[4], this->m[6], this->m[7], this->m[8], this->m[10], this->m[11]};
    float det_14[9] = {this->m[0], this->m[1], this->m[3], this->m[4], this->m[5], this->m[7], this->m[8], this->m[9], this->m[11]};
    float det_15[9] = {this->m[0], this->m[1], this->m[2], this->m[4], this->m[5], this->m[6], this->m[8], this->m[9], this->m[10]};

    float ret[16] = {
        Mat3::Determinant(det_0),
        -Mat3::Determinant(det_1),
        Mat3::Determinant(det_2),
        -Mat3::Determinant(det_3),
        -Mat3::Determinant(det_4),
        Mat3::Determinant(det_5),
        -Mat3::Determinant(det_6),
        Mat3::Determinant(det_7),
        Mat3::Determinant(det_8),
        -Mat3::Determinant(det_9),
        Mat3::Determinant(det_10),
        -Mat3::Determinant(det_11),
        -Mat3::Determinant(det_12),
        Mat3::Determinant(det_13),
        -Mat3::Determinant(det_14),
        Mat3::Determinant(det_15)};

    return Mat4(ret).Transpose();
  }

  Mat4 Mat4::Inverse() const
  {

    Mat4 ret = this->Adjoint();
    float det = this->Determinant();

    if (det == 0)
      return Mat4::zero;

    ret /= det;

    return ret;
  }

  Mat4 Mat4::Transpose() const
  {
    float f[16] = {
        this->m[0],
        this->m[4],
        this->m[8],
        this->m[12],
        this->m[1],
        this->m[5],
        this->m[9],
        this->m[13],
        this->m[2],
        this->m[6],
        this->m[10],
        this->m[14],
        this->m[3],
        this->m[7],
        this->m[11],
        this->m[15]};

    return Mat4(f);
  }

  Mat4 Mat4::Translate(Vec3 mov)
  {
    float tras[16] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        mov.x, mov.y, mov.z, 1};

    return Mat4(tras);
  }

  Mat4 Mat4::Translate(float x, float y, float z)
  {
    float tras[16] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        x, y, z, 1};

    return Mat4(tras);
  }

  Mat4 Mat4::Scale(Vec3 scale)
  {
    float f[16] = {
        scale.x, 0, 0, 0,
        0, scale.y, 0, 0,
        0, 0, scale.z, 0,
        0, 0, 0, 1};

    return Mat4(f);
  }

  Mat4 Mat4::Scale(float x, float y, float z)
  {
    float f[16] = {
        x, 0, 0, 0,
        0, y, 0, 0,
        0, 0, z, 0,
        0, 0, 0, 1};

    return Mat4(f);
  }

  Mat4 Mat4::RotateX(float radians)
  {
    float cos_ = std::cos(radians);
    float sin_ = std::sin(radians);
    float rot[16] = {
        1, 0, 0, 0,
        0, cos_, sin_, 0,
        0, -sin_, cos_, 0,
        0, 0, 0, 1};

    return Mat4(rot);
  }

  Mat4 Mat4::RotateY(float radians)
  {
    float cos_ = std::cos(radians);
    float sin_ = std::sin(radians);
    float rot[16] = {
        cos_, 0, -sin_, 0,
        0, 1, 0, 0,
        sin_, 0, cos_, 0,
        0, 0, 0, 1};

    return Mat4(rot);
  }

  Mat4 Mat4::RotateZ(float radians)
  {
    float cos_ = std::cos(radians);
    float sin_ = std::sin(radians);
    float rot[16] = {
        cos_, sin_, 0, 0,
        -sin_, cos_, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1};

    return Mat4(rot);
  }

  Mat4 Mat4::Rotate(Vec3 radians)
  {
    return RotateX(radians.x) * RotateY(radians.y) * RotateZ(radians.z);
  }

  Mat4 Mat4::Rotate(float radX, float radY, const float radZ)
  {
    return RotateX(radX) * RotateY(radY) * RotateZ(radZ);
  }

  Mat4 Mat4::Transform(Vec3 rotate,
                       Vec3 scale,
                       Vec3 translate)
  {
    return Scale(scale) * Translate(translate) * Rotate(rotate);
  }

  Mat4 Mat4::Transform(float rotateX, float rotateY, float rotateZ,
                       float scaleX, float scaleY, float scaleZ,
                       float transX, float transY, float transZ)
  {
    return Scale(scaleX, scaleY, scaleZ) * Translate(transX, transY, transZ) * Rotate(rotateX, rotateY, rotateZ);
  }

  Vec4 Mat4::GetColumn(size_t column) const
  {
    if (column < 4)
      return Vec4(this->m[0 + column], this->m[4 + column], this->m[8 + column], this->m[12 + column]);
    else
      return Vec4::zero;
  }

  Vec4 Mat4::GetLine(size_t line) const
  {
    if (line < 4)
      return Vec4(this->m[0 + line * 4], this->m[1 + line * 4], this->m[2 + line * 4], this->m[3 + line * 4]);
    else
      return Vec4::zero;
  }

  Mat4 Mat4::ViewMatrix(Vec3 camara, Vec3 mira, Vec3 up)
  {
    Vec3 zAxis = camara - mira; // Vector Z
    if (!zAxis.IsNormalized())
      zAxis = zAxis.Normalized();

    Vec3 xAxis = Vec3::CrossProduct(up, zAxis); // Vector X
    if (!xAxis.IsNormalized())
      xAxis = xAxis.Normalized();

    Vec3 yAxis = Vec3::CrossProduct(zAxis, xAxis); // Vector Y
    if (!yAxis.IsNormalized())
      yAxis = yAxis.Normalized();

    // Crea una matriz de vista a partir de los vectores de la camara
    float vMat[16] = {
        xAxis.x, yAxis.x, zAxis.x, 0.0f,
        xAxis.y, yAxis.y, zAxis.y, 0.0f,
        xAxis.z, yAxis.z, zAxis.z, 0.0f,
        -Vec3::DotProduct(xAxis, camara), -Vec3::DotProduct(yAxis, camara), -Vec3::DotProduct(zAxis, camara), 1.0f};

    return Mat4(vMat);
  }

  Mat4 Mat4::PerspectiveMatrix(float fov, float aspect,
                               float near, float far)
  {
    Mat4 result;

    float tanHalfFov = tanf(fov / 2.0f);
    float range = far - near;

    result.m[0] = 1.0f / (aspect * tanHalfFov);
    result.m[5] = 1.0f / tanHalfFov;
    result.m[10] = -(far + near) / range;
    result.m[11] = -2.0f * far * near / range;
    result.m[14] = -1.0f;

    return result;
  }

  Mat4 Mat4::OrthoMatrix(float right, float left,
                         float top, float bottom,
                         float near, float far)
  {
    Mat4 result;

    float rec_width = 1.0f / (right - left);
    float rec_height = 1.0f / (top - bottom);
    float rec_depth = 1.0f / (far - near);

    result.m[0] = 2.0f * rec_width;
    result.m[5] = 2.0f * rec_height;
    result.m[10] = -2.0f * rec_depth;

    result.m[12] = -(right + left) * rec_width;
    result.m[13] = -(top + bottom) * rec_height;
    result.m[14] = -(far + near) * rec_depth;
    result.m[15] = 1.0f;

    return result;
  }
}

#endif /* __MATRIX4_H */
