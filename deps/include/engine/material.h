#include "engine.h"

#ifndef __MATERIAL_H__
#define __MATERIAL_H__ 1

/**
 * @class Material
 *
 * @brief Class that represents and manages graphic material.
 */
class Material
{
public:
  /**
   * @brief Type for material identifier.
   */
  typedef uint32_t Id;

  /**
   * @brief Material constructor.
   */
  Material();

  /**
   * @brief Material destructor.
   */
  ~Material();

  /**
   * @brief Free all resources.
  */
  void free();

  /**
   * @brief Check if the material has a shader associated with it.
   *
   * @return True if the material has a shader, false otherwise.
   */
  boolean hasShader();

  /**
   * @brief Load and assign the shaders to the material.
   *
   * @param fragmentPath Path to the file containing the fragment shader source code.
   * @param vertexPath Path to the file containing the vertex shader source code.
   * @param is_path check of the source code origin.
   */
  void loadShader(const byte *fragment, const byte *vertex, boolean is_path = true);

  /**
   * @brief Reloads the shader associated with the material.
   */
  void rechargeShader();

  /**
   * @brief Assigns a texture to the material.
   *
   * @param uniform_name Uniform name in the shader.
   * @param texture_id Texture identifier.
   * @param texture_unit Texture shader order. Max 32 per shader
   */
  void setTexture(const byte *uniform_name, u32 texture_id, u32 texture_unit);

  /**
   * @brief Assigns a texture to the material.
   *
   * @param uniform_name Uniform name in the shader.
   * @param texture_id Texture identifier.
   * @param texture_unit Texture shader order. Max 32 per shader
   */
  void setTexture2DArray(const byte *uniform_name, u32 texture_id, u32 texture_unit);

  /**
   * @brief Assigns a int value to a uniform in the shader.
   *
   * @param uniform_name Uniform name in the shader.
   * @param value int value to assign.
   */
  void setU32(const byte *uniform_name, u32 value);

  /**
   * @brief Assigns a float value to a uniform in the shader.
   *
   * @param uniform_name Uniform name in the shader.
   * @param value Float value to assign.
   */
  void setF32(const byte *uniform_name, f32 value);

  /**
   * @brief Assigns a Vector2 to a uniform in the shader.
   *
   * @param uniform_name Uniform name in the shader.
   * @param value Vector2 components to be assigned.
   */
  void setVec2(const byte *uniform_name, Math::Vec2 value);

  /**
   * @brief Assigns a Vector3 to a uniform in the shader.
   *
   * @param uniform_name Uniform name in the shader.
   * @param value Vector3 components to be assigned.
   */
  void setVec3(const byte *uniform_name, Math::Vec3 value);

  /**
   * @brief Assigns a Vector4 to a uniform in the shader.
   *
   * @param uniform_name Uniform name in the shader.
   * @param value Vector4 components to be assigned.
   */
  void setVec4(const byte *uniform_name, Math::Vec4 value);

  /**
   * @brief Assigns a 2x2 matrix to a uniform in the shader.
   *
   * @param uniform_name Uniform name in the shader.
   * @param matrix 2x2 matrix to assign.
   */
  void setMat2(const byte *uniform_name, Math::Mat2 matrix);

  /**
   * @brief Assigns a 3x3 matrix to a uniform in the shader.
   *
   * @param uniform_name Uniform name in the shader.
   * @param matrix 3x3 matrix to assign.
   */
  void setMat3(const byte *uniform_name, Math::Mat3 matrix);

  /**
   * @brief Assigns a 4x4 matrix to a uniform in the shader.
   *
   * @param uniform_name Uniform name in the shader.
   * @param matrix 4x4 matrix to assign.
   */
  void setMat4(const byte *uniform_name, Math::Mat4 matrix);

  /**
   * @brief Activate the use of the material, configuring the shader and the uniforms.
   */
  void use() const;

private:
  u32 program_id_;                   ///< Shader program identifier.
  boolean has_shader_;               ///< Indicates if the material has a shader.
  byte *fragmentPath_, *vertexPath_; ///< Shader file paths.

  std::unordered_map<std::string, s32> uniforms_; ///< Map uniform names to their locations.

  /**
   * @brief Gets the location of a uniform in the shader.
   *
   * @param uniform_name Uniform name.
   *
   * @return Location of the uniform in the shader.
   */
  s32 getUniformLocation(const byte *uniform_name);
};

#endif /* __MATERIAL_H__ */
