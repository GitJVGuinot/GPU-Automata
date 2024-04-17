#include "engine.h"

#ifndef __CAMERA_H__
#define __CAMERA_H__ 1

/**
 * @class Camera
 *
 * @brief Class that represents a camera.
 *
 * This class provides functionality for working with cameras,
 * including configuration and related operations.
 */
class Camera
{
public:
  /**
   * @enum TextureData
   *
   * @brief Enumeration for texture types
   */
  enum TextureDataType : u32
  {
    Colour = 0,
    Location,
    Normals,
    Picker,

    MaxTextures,
  };

  /**
   * @enum Type
   *
   * @brief Enumerator that represents the type of camera.
   */
  enum class RenderType : u32
  {
    Invalid = 0,
    /**
     * @brief Orthographic camera type.
     *
     * Create a cubic perspective for the camera view.
     */
    Orthographic,

    /**
     * @brief Tipo de cámara perspectiva.
     *
     * Create a frustum perspective for the camera view.
     */
    Perspective
  };

  /**
   * @enum Type
   *
   * @brief Enumerator that represents the type of camera.
   */
  enum class LightRenderType : u32
  {
    Invalid = 0,
    /**
     * @brief Use deffer rendering
     */
    Deferred,

    /**
     * @brief Use forward rendering
     */
    Forward
  };

  /**
   * @struct CamConfig
   *
   * @brief Structure that defines the camera configuration.
   *
   * This structure contains parameters that define the configuration of a
   * camera.
   */
  struct CamConfig
  {
    RenderType camera_render_type_ = RenderType::Perspective;       ///< Camera type.
    LightRenderType light_render_type_ = LightRenderType::Deferred; ///< Lights Type

    Math::Vec2 cam_win_; ///< Camera window size.
    Math::Vec3 pos_;     ///< Camera position in three-dimensional space.
    Math::Vec3 target_;  ///< Point the camera is pointing at.

    f32 near_ = 1.0f;   ///< Close distance for the camera.
    f32 far_ = 1000.0f; ///< Far distance for the camera.

    f32 right_ = 10.0f;   ///< Parameter used for the orthographic view.
    f32 left_ = -10.0f;   ///< Parameter used for the orthographic view.
    f32 top_ = 10.0f;     ///< Parameter used for the orthographic view.
    f32 bottom_ = -10.0f; ///< Parameter used for the orthographic view.

    f32 fovy_ = Math::MathUtils::AngleToRads(60.0f); ///< Angle of view inradians for perspective view.

    Mesh *(*GetMesh)(Mesh::Id);
    Mesh::Id (*UploadMesh)(CustomMesh *(*load_mesh_callback)());

    Math::Vec2 (*WheelScroll)();
    Math::Vec2 (*MousePosition)();
    boolean (*KeyInputPress)(Inputs::Key);
    boolean (*MouseInputPress)(Inputs::MouseButton);
  };

  // To change to desired key: camera_object.front_move_key = static_cast<u32>(Inputs::Key::...);
  Inputs::Key front_move_key_, back_move_key_;
  Inputs::Key right_move_key_, left_move_key_;
  Inputs::Key up_move_key_, down_move_key_;

  /**
   * @brief Constructor of the Camera class.
   */
  Camera();

  /**
   * @brief Initialize the camera configuration.
   *
   * Need to call after window init.
   *
   * @param config Configuration of the camera to use.
   */
  void init(CamConfig config);

  /**
   * @brief Destructor of the Camera class.
   *
   * Release the created camera.
   */
  ~Camera();

  /**
   * @brief Updates the camera.
   *
   * @param dt Delta time for constant movement
   */
  void control(const f32 dt);

  /**
   * @brief Gets the type of the camera.
   *
   * @return Camera type.
   */
  RenderType getRenderType() const;

  /**
   * @brief Gets the type of the light render.
   *
   * @return Light render type.
   */
  LightRenderType getLightRenderType() const;

  /**
   * @brief Sets the initial position of the camera window.
   *
   * This function assigns the initial position of the camera window using
   * the specified coordinates.
   *
   * @param win_pos Initial position of the camera window (x, y coordinates).
   */
  void setWinPos(Math::Vec2 win_pos);

  /**
   * @brief Gets the initial position of the camera window.
   *
   * This function returns the starting position of the camera window as
   * a Math::Vec2 object.
   *
   * @return Initial position of the camera window (x, y coordinates).
   */
  Math::Vec2 winPos() const;

  /**
   * @brief Gets the size of the camera window.
   *
   * This function returns the camera window size as
   * a Math::Vec2 object.
   *
   * @return Size of the camera window (x, y coordinates).
   */
  Math::Vec2 winSize() const;

  /**
   * @brief Sets the size of the camera window.
   *
   * This function assigns the camera window size as
   * a Math::Vec2 object.
   *
   * @param size Size of the camera window (x, y coordinates).
   */
  void setWinSize(Math::Vec2 size);

  /**
   * @brief Gets the perspective projection matrix associated with the current camera settings.
   *
   * This function returns the perspective projection matrix calculated from
   * the current camera settings, which includes parameters such as position,
   * orientation, and window aspect.
   *
   * @return Perspective projection matrix.
   */
  Math::Mat4 getPerspectiveMatrix() const;

  /**
   * @brief Gets the orthographic projection matrix associated with the current camera settings.
   *
   * This function returns the orthographic projection matrix calculated from
   * the current camera settings, which includes parameters such as position,
   * orientation, and window dimensions.
   *
   * @return Orthographic projection matrix.
   */
  Math::Mat4 getOrtoMatrix() const;

  /**
   * @brief Gets the view array associated with the current camera settings.
   *
   * This function returns the view matrix calculated from the current camera
   * settings, which includes parameters such as position, orientation, and
   * focus point.
   *
   * @return View matrix.
   */
  Math::Mat4 getViewMatrix() const;

  /**
   * @brief Gets the position of the camera.
   *
   * @return Camera Position.
   */
  Math::Vec3 getPosition() const;

  /**
   * @brief Set the position of the camera.
   *
   * @param pos Camera new position.
   */
  void setPosition(Math::Vec3 pos);

  /**
   * @brief Gets the direction of the camera.
   *
   * @return Camera Direction.
   */
  Math::Vec3 getViewDir() const;

  /**
   * @brief Set the target of the camera.
   *
   * @param target Camera new target.
   */
  void setTarget(Math::Vec3 target);

  /**
   * @brief Bind the camera buffers
   * Used internally
   */
  void beginRender();

  /**
   * @brief Activate the camera buffer
   * Used internally
   */
  Material *getMaterial();

  /**
   * @brief Render the camera buffer
   * Used internally
   */
  void render();

  void setMatPostProcess(Material *mat);

  Entity::Id getSelectedEntityId();

private:
  /**
   * @brief Generates textures to save screen data
   */
  boolean initTextures();

  /**
   * @brief Move the camera.
   *
   * Adjust the camera's position according to keyboard commands.
   *
   * @param dt Delta time for constant movement
   */
  void move(const f32 dt);

  /**
   * @brief Rotate the camera (Keyboard).
   *
   * Adjust the camera's rotation according to keyboard commands.
   *
   * @param dt Delta time for constant movement
   *
   */
  void rotate(const f32 dt);

  /**
   * @brief Rotate the camera (Mouse).
   *
   * Adjust the camera's rotation according to mouse movement.
   *
   * @param dt Delta time for constant movement
   *
   */
  void mouseRotate(const f32 dt);

  RenderType render_type_;                  ///< Camera render type.
  LightRenderType light_type_;              ///< Light render type.
  Math::Vec3 camera_, target_;              ///< Camera position and focus point.
  Math::Vec3 view_dir_, side_dir_, up_dir_; ///< Camera directions.

  Math::Vec2 prev_mouse_;   ///< Mouse coordinates in the previous frame.
  f32 speed_, sensitivity_; ///< Camera speed and sensitivity.

  f32 near_, far_;                  ///< Near and far distances for the camera.
  f32 right_, left_, top_, bottom_; ///< Parameters for the orthographic view.
  f32 fov_, aspect_;                ///< Viewing angle and aspect ratio.

  Math::Vec2 window_pos_;  ///< Window position
  Math::Vec2 window_size_; ///< Size of the camera window.
  Math::Vec2 textures_size_;

  // Light Material
  u32 fbo_;
  u32 depth_buffer_;
  u32 textures_[TextureDataType::MaxTextures];
  u32 attachments_[TextureDataType::MaxTextures];
  u32 active_textures_[TextureDataType::MaxTextures];

  boolean is_initialized_; ///< Camera initialization status.

  Material light_mat_;
  Mesh *quad_;

  // Post Process Material
  Material *post_process_mat_;
  u32 post_process_fbo_;
  u32 post_process_texture_;
  u32 post_process_attachment_;
  u32 post_process_active_texture_;

  Math::Vec2 (*WheelScroll)();
  Math::Vec2 (*MousePosition)();
  boolean (*KeyInputPress)(Inputs::Key);
  boolean (*MouseInputPress)(Inputs::MouseButton);
};

#endif /* __CAMERA_H__ */