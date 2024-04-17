#include "engine.h"

#ifndef __SHADOWS_H__
#define __SHADOWS_H__ 1

class ShadowsManager
{
public:
  enum class Resolution : s16
  {
    Low = 1,
    Medium,
    High,
  };

  /**
   * @brief Constructor for shadow manager
   *
   * @param point_res resolution for point light
   * @param spot_res resolution for spot light
   * @param directional_res resolution for directional light
   */
  ShadowsManager(Resolution point_res = Resolution::Low, Resolution spot_res = Resolution::Low, Resolution directional_res = Resolution::Medium);
  ~ShadowsManager();
  void free();

  /**
   * @brief Activates the shadow map for rendering.
   *
   * @return true if the light is active, false otherwise
   */
  boolean use(u32 light_id, LightType light_type);

  /**
   * @brief Return the sampler array id of specific light
   *
   * @param light_type
   * @return the sampler id
   */
  u32 getMapId(LightType light_type);

  /**
   * @brief Active a new light with light_id
   *
   * @param light_id
   * @param light_type
   */
  void addShadow(size_t light_id, LightType light_type);

  /**
   * @brief Set a matrix 4x4 like uniform to cast shadows
   *
   * @param uniform_name
   * @param matrix
   */
  void setMat4(const byte *uniform_name, Math::Mat4 matrix);

private:
  u32 point_res_, spot_res_, directional_res_; ///< Lights resolution

  u32 program_id_; ///< Shadow program to render it

  static const u32 max_point_lights = 16;
  u32 point_map_id_;                              ///< Sampler id
  u32 *point_fbos_;                               ///< Buffers for each light
  boolean active_point_lights_[max_point_lights]; ///< Flag to know if active

  static const u32 max_spot_lights = 16;
  u32 spot_map_id_;                             ///< Sampler id
  u32 *spot_fbos_;                              ///< Buffers for each light
  boolean active_spot_lights_[max_spot_lights]; ///< Flag to know if active

  static const u32 max_directional_lights = 16;
  u32 directional_map_id_;                                    ///< Sampler id
  u32 *directional_fbos_;                                     ///< Buffers for each light
  boolean active_directional_lights_[max_directional_lights]; ///< Flag to know if active
};

#endif /* __SHADOWS_H__ */
