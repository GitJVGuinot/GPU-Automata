/**
 * @author Javier Guinot Almenar <guinotal@esat-alumni.com>
 *
 * @file math_utils.h
 *
 * @brief Utils function for math lib
 *
 */

#include "mathlib.h"

#ifndef __MATH_UTILS_H__
#define __MATH_UTILS_H__ 1

namespace Math
{
#define Between(x, y, z) (x > y && y >= z)

  class MathUtils
  {
  public:
    inline static bool MoreOrLessEqual(float n, float n1, float error = 1e-5f);
    inline static float Sqrt(float n);

    inline static float Clamp(float value, float minVal, float maxVal);

    inline static Vec4 HomogeinizaVec(Vec4 v);

    inline static Vec3 Mat4TransformVec3(Mat4 m, Vec3 v);

    inline static Vec2 Vec3ToVec2(Vec3 v);

    inline static Vec3 Rotate3dPoint(Vec3 rot, Vec3 point);

    inline static Vec3 OrbitPoint(Vec3 center, Vec3 orbit, Vec3 point);

    inline static Vec3 Vec2ToVec3(Vec2 v2, bool homogeneizado);

    inline static Vec3 Mat3TransformVec3(Mat3 m, Vec3 v);

    inline static void NormalizeVector(float v[3]);

    inline static Vec3 CalculateTriangleNormal(const float v0[3], const float v1[3], const float v2[3]);

    inline static Vec3 CalculateVertexNormal(const float n0[3], const float n1[3], const float n2[3], const float n3[3]);

    inline static float AngleToRads(float angle);
    inline static float RadsToAngle(float rads);

    // This function alloc the vec2 array internally
    inline static Vec2 *InitBasicCircle(size_t vertex, float diam, float rot, Vec2 desp);

    inline static bool CircularCollision(Vec2 center, float diam, Vec2 center1, float diam1);
    inline static bool SphericCollision(Vec3 center, float diam, Vec3 center1, float diam1);

  private:
    MathUtils();
    ~MathUtils();
  };

  inline MathUtils::MathUtils() {}
  inline MathUtils::~MathUtils() {}

  // Implementation
  ///////////////////////////////////////////////////////////////////////////////

  bool MathUtils::MoreOrLessEqual(float n, float n1, float error)
  {
    return std::abs(n - n1) < error;
  }

  float MathUtils::Sqrt(float number)
  {
    if (number < 0)
      return -1.0f;

    float estimation = number;
    float error = 1e-7f;

    while (std::abs(estimation * estimation - number) > error)
      estimation = 0.5f * (estimation + number / estimation);

    return estimation;
  }

  float MathUtils::Clamp(float value, float minVal, float maxVal)
  {
    return ((value < minVal) ? minVal : (value > maxVal ? maxVal : value));
  }

  float MathUtils::AngleToRads(float angle)
  {
    return angle * static_cast<float>(M_PI * REC_180);
  }

  float MathUtils::RadsToAngle(float rads)
  {
    return rads * 180.0f * static_cast<float>(M_1_PI);
  }

  Vec4 MathUtils::HomogeinizaVec(Vec4 v)
  {
    Vec4 v1;
    float rec_w = 1.0f / v.w;
    if (v.w != 1.0f)
    {
      v1.x = v.x * rec_w;
      v1.y = v.y * rec_w;
      v1.z = v.z * rec_w;
      v1.w = v.w * rec_w;
    }
    else
      v1 = v;

    return v1;
  }

  Vec3 MathUtils::Mat4TransformVec3(Mat4 m, Vec3 v)
  {

    Vec4 v1(
        (m.m[0] * v.x) + (m.m[4] * v.y) + (m.m[8] * v.z) + (m.m[12] * 1.0f),
        (m.m[1] * v.x) + (m.m[5] * v.y) + (m.m[9] * v.z) + (m.m[13] * 1.0f),
        (m.m[2] * v.x) + (m.m[6] * v.y) + (m.m[10] * v.z) + (m.m[14] * 1.0f),
        (m.m[3] * v.x) + (m.m[7] * v.y) + (m.m[11] * v.z) + (m.m[15] * 1.0f));

    Vec4 v2 = HomogeinizaVec(v1);
    Vec3 homo = {v2.x, v2.y, v2.z};

    return homo;
  }

  Vec2 MathUtils::Vec3ToVec2(Vec3 v)
  {
    return Vec2(v.x, v.y);
  }

  Vec3 MathUtils::Rotate3dPoint(Vec3 rot, Vec3 point)
  {
    Mat4 model = Mat4::Identity();
    if (rot.x != 0.0f)
      model *= Mat4::RotateX((rot.x * static_cast<float>(M_PI * REC_180)));
    if (rot.y != 0.0f)
      model *= Mat4::RotateY((rot.y * static_cast<float>(M_PI * REC_180)));
    if (rot.z != 0.0f)
      model *= Mat4::RotateZ((rot.z * static_cast<float>(M_PI * REC_180)));

    return Mat4TransformVec3(model, point);
  }

  Vec3 MathUtils::OrbitPoint(Vec3 centro_orbita, Vec3 p_orbita, Vec3 punto)
  {

    Mat4 model = Mat4::Identity();
    if (p_orbita.x != 0)
      model *= Mat4::RotateX((p_orbita.x * static_cast<float>(M_PI * REC_180)));
    if (p_orbita.y != 0)
      model *= Mat4::RotateY((p_orbita.y * static_cast<float>(M_PI * REC_180)));
    if (p_orbita.z != 0)
      model *= Mat4::RotateZ((p_orbita.z * static_cast<float>(M_PI * REC_180)));

    Vec3 ret(punto);
    ret -= centro_orbita;
    ret = Mat4TransformVec3(model, ret);
    ret += centro_orbita;

    return ret;
  }

  Vec3 MathUtils::Vec2ToVec3(Vec2 v2, bool homogeneizado)
  {
    return Vec3(v2.x, v2.y, (float)homogeneizado);
  }

  Vec3 MathUtils::Mat3TransformVec3(Mat3 m, Vec3 v)
  {

    return Vec3((m.m[0] * v.x) + (m.m[3] * v.y) + (m.m[6] * v.z),
                (m.m[1] * v.x) + (m.m[4] * v.y) + (m.m[7] * v.z),
                (m.m[2] * v.x) + (m.m[5] * v.y) + (m.m[8] * v.z));
  }

  void MathUtils::NormalizeVector(float v[3])
  {
    float length = sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    float rec_length = 1.0f / length;
    if (length != 0.0f && length != 1.0f)
    {
      v[0] = v[0] * rec_length;
      v[1] = v[1] * rec_length;
      v[2] = v[2] * rec_length;
    }
  }

  Vec3 MathUtils::CalculateTriangleNormal(const float v0[3], const float v1[3], const float v2[3])
  {
    Vec3 outNormal;

    // Triangle center vectors
    float v10[3] = {v1[0] - v0[0], v1[1] - v0[1], v1[2] - v0[2]};
    float v20[3] = {v2[0] - v0[0], v2[1] - v0[1], v2[2] - v0[2]};

    // Cross product between v10 & v20
    outNormal.x = v10[1] * v20[2] - v10[2] * v20[1];
    outNormal.y = v10[2] * v20[0] - v10[0] * v20[2];
    outNormal.z = v10[0] * v20[1] - v10[1] * v20[0];

    // Normaliza el vector resultante
    outNormal = outNormal.Normalized();

    return outNormal;
  }

  Vec3 MathUtils::CalculateVertexNormal(const float n0[3], const float n1[3], const float n2[3], const float n3[3])
  {
    Vec3 outNormal;

    // Add of the normals
    outNormal.x = (n0[0] + n1[0] + n2[0] + n3[0]);
    outNormal.y = (n0[1] + n1[1] + n2[1] + n3[1]);
    outNormal.z = (n0[2] + n1[2] + n2[2] + n3[2]);

    outNormal = outNormal.Normalized();

    return outNormal;
  }

  Vec2 *MathUtils::InitBasicCircle(size_t vertices, float diam, float rotacion, Vec2 desp)
  {
    size_t size = vertices + 2;
    Vec2 *circle = reinterpret_cast<Vec2 *>(std::calloc(size, sizeof(Vec2)));
    if (!circle)
      return nullptr;

    for (size_t i = 0; i < vertices + 1; i++)
      circle[i] = {0, 0};

    float radianes = 0;
    int vertex = 0;

    while (radianes < (2 * M_PI))
    {
      circle[vertex] = Vec2(
          diam * cosf(radianes - rotacion) + desp.x, // vertice en x
          diam * sinf(radianes - rotacion) + desp.y  // vertice en y
      );

      vertex++;
      radianes += static_cast<float>((2.0f * M_PIf) / static_cast<float>(vertices));
    }

    circle[vertex] = Vec2(
        diam * cosf(radianes - rotacion) + desp.x, // vertice en x
        diam * sinf(radianes - rotacion) + desp.y  // vertice en y
    );

    return circle;
  }

  bool MathUtils::CircularCollision(Vec2 centro, float diam, Vec2 centro1, float diam1)
  {
    return (std::abs((centro - centro1).Magnitude()) - ((diam + diam1) * 0.5f)) <= 0;
  }

  bool MathUtils::SphericCollision(Vec3 centro, float diam, Vec3 centro1, float diam1)
  {
    return (std::abs((centro - centro1).Magnitude()) - ((diam + diam1) * 0.5f)) <= 0;
  }

}

#endif /* __MATH_UTILS_H__ */
