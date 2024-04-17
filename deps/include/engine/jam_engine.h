#include "engine.h"

#ifndef __JAM_ENGINE_H__
#define __JAM_ENGINE_H__ 1

/**
 * @class JAM_Engine
 *
 * @brief Represents a game engine providing initialization, update, rendering, and input handling functionalities.
 */
class JAM_Engine
{
public:
  /**
   * @struct Config
   *
   * @brief Configuration structure for initializing the engine.
   */
  struct Config
  {
    s32 argc_ = 0;          ///< Number of command line arguments.
    byte **argv_ = nullptr; ///< Array of command line arguments.

    s32 width_ = 0, height_ = 0;                                                      ///< Width and height of the window.
    boolean fullscreen_ = false;                                                      ///< Flag for fullscreen mode.
    boolean make_resizable_ = false;                                                  ///< Flag for resizable window.
    boolean end_escape_key_ = true;                                                   ///< Flag for ending the engine with the Escape key.
    ShadowsManager::Resolution point_res_ = ShadowsManager::Resolution::Low;          ///< Point light shadow resolution
    ShadowsManager::Resolution spot_res_ = ShadowsManager::Resolution::Low;           ///< Spot light shadow resolution
    ShadowsManager::Resolution directional_res_ = ShadowsManager::Resolution::Medium; ///< Directional light shadow resolution
  };

  /**
   * @brief Initializes the engine.
   *
   * @param user_init Function pointer for user-provided initialization.
   * @param config Configuration structure.
   * @param user_struct Optional user-provided structure.
   */
  static void Init(void (*user_init)(s32 argc, byte *argv[], void *user_struct), Config config, void *user_struct = nullptr);

  /**
   * @brief Updates the engine.
   *
   * @param user_update Function pointer for user-provided update.
   * @param user_struct Optional user-provided structure.
   */
  static void Update(void (*user_update)(void *user_struct), void *user_struct = nullptr);

  /**
   * @brief Remove all allocated resources from engine
   */
  static void Clean(void (*user_update)(void *user_struct), void *user_struct = nullptr);

  /**
   * @brief Prepers the render shadow.
   */
  static void BeginRenderShadow(u32 light_id, LightType light_type);

  /**
   * @brief Renders the scene with shadows.
   *
   * @param root_node Root node data.
   * @param camera Camera to use for rendering.
   * @param father_mat Transformation matrix.
   */
  static void RenderShadow(const byte *root_node, Math::Mat4 father_mat = Math::Mat4::Identity());

  /**
   * @brief Renders the scene with shadows.
   *
   * @param root_node Root node identifier.
   * @param camera Camera to use for rendering.
   * @param father_mat Transformation matrix.
   */
  static void RenderShadow(Entity::Id root_node, Math::Mat4 father_mat = Math::Mat4::Identity());

  /**
   * @brief Ends the render shadow.
   */
  static void EndRenderShadow();

  /**
   * @brief Prepers the render.
   *
   * @param camera Pointer to the camera object to use for rendering.
   */
  static void BeginRender(Camera *camera);

  /**
   * @brief Renders the scene with a specified root node, camera, and transformation matrix.
   *
   * @param root_node Root node data.
   * @param camera Camera to use for rendering.
   * @param father_mat Transformation matrix.
   */
  static void Render(const byte *root_node, Math::Mat4 father_mat = Math::Mat4::Identity());

  /**
   * @brief Renders the scene with a specified root node, camera, and transformation matrix.
   *
   * @param root_node Root node identifier.
   * @param camera Camera to use for rendering.
   * @param father_mat Transformation matrix.
   */
  static void Render(Entity::Id root_node, Math::Mat4 father_mat = Math::Mat4::Identity());

  /**
   * @brief Ends the render shadow.
   */
  static void EndRender();

  /**
   * @brief Gets the time elapsed since the last frame in seconds.
   *
   * @return Delta time in miliseconds.
   */
  static f32 DeltaTime();

  /**
   * @brief Finishes the engine when needed
   */
  static void Finish();

  /**
   * @brief Load a mesh model from a file.
   *
   * @param path Path to the mesh file.
   * @param center Flag to center and normalize the mesh
   *
   * @return Unique identifier of the loaded mesh.
   */
  static Mesh::Id UploadMesh(const byte *path, boolean center);

  /**
   * @brief Load a mesh model from a custom function.
   *
   * @param path Path to the mesh file.
   *
   * @return Unique identifier of the loaded mesh.
   */
  static Mesh::Id UploadCustomMesh(CustomMesh *(*load_mesh_callback)());

  /**
   * @brief Gets a pointer to the mesh using its unique identifier.
   *
   * @param index Unique identifier of the mesh.
   *
   * @return Pointer to the corresponding mesh or nullptr if not found.
   */
  static Mesh *GetMesh(Mesh::Id index);

  /**
   * @brief Gets a pointer to the mesh using an enum form.
   *
   * @param form Unique identifier of the Platonic enum.
   *
   * @return Pointer to the corresponding mesh or nullptr if not found.
   */
  static Mesh *GetMesh(Mesh::Platonic form);

  /**
   * @brief Loads a texture from a file with wrapper and filter options specified.
   *
   * @param path Path to the texture file.
   * @param wrapS Wrapping mode in S direction.
   * @param wrapT Wrapping mode in T direction.
   * @param minF Magnification filter (default: Linear).
   * @param magF Minimization filter (default: Linear).
   *
   * @return Unique identifier of the loaded texture.
   */
  static Texture::Id UploadTexture(const byte *path,
                                   Texture::Wrap wrapS = Texture::Wrap::Repeat, Texture::Wrap wrapT = Texture::Wrap::Repeat,
                                   Texture::Filter minF = Texture::Filter::Linear, Texture::Filter magF = Texture::Filter::Linear);

  /**
   * @brief Gets a pointer to the texture using its unique identifier.
   *
   * @param index Unique texture identifier.
   *
   * @return Pointer to the corresponding texture or nullptr if not found.
   */
  static Texture *GetTexture(u32 index);

  /**
   * @brief Reload all shading programs.
   */
  static void RechargeShaders();

  /**
   * @brief Sets the color of the basic material.
   *
   * If any component is not provided, it is assumed to be 1.0.
   *
   * @param color The color of the basic material as an RGBA color vector.
   */
  static void SetBasicMaterialColor(Math::Vec4 color = 1.0f);

  /**
   * @brief Sets the color of the basic material.
   *
   * If no argument is provided, the default color is white (1.0, 1.0, 1.0, 1.0).
   *
   * @param r The red component of the basic material color.
   * @param g The green component of the basic material color.
   * @param b The blue component of the basic material color.
   * @param a The alpha component of the basic material color.
   */
  static void SetBasicMaterialColor(f32 r = 1.0f, f32 g = 1.0f, f32 b = 1.0f, f32 a = 1.0f);

  /**
   * @brief Load a material from fragment and vertex files.
   *
   * @param fragment Path to the fragment shader file.
   * @param vertex Path to the vertex shader file.
   *
   * @return Unique identifier of the loaded material.
   */
  static Material::Id UploadMaterial(const byte *fragment, const byte *vertex);

  /**
   * @brief Gets a pointer to the material using its unique identifier.
   *
   * @param index Unique identifier of the material.
   *
   * @return Pointer to the corresponding material or nullptr if not found.
   */
  static Material *GetMaterial(Material::Id index);

  /**
   * @brief Checks if a key is pressed at the current time.
   *
   * @param key Key to verify.
   *
   * @return True if the key is pressed, False otherwise.
   */
  static boolean InputDown(Inputs::Key key);

  /**
   * @brief Checks if a key was pressed in the last frame.
   *
   * @param key Key to verify.
   *
   * @return True if the key was pressed, False otherwise.
   */
  static boolean InputPress(Inputs::Key key);

  /**
   * @brief Gets the size of the window in screen coordinates.
   *
   * @return Vector with the dimensions of the window (width and height).
   */
  static Math::Vec2 WindowSize();

  /**
   * @brief Gets the current mouse position in screen coordinates.
   *
   * @return Vector with the X and Y coordinates of the mouse position.
   */
  static Math::Vec2 MousePosition();

  /**
   * @brief Gets the amount of mouse wheel scrolling.
   *
   * @return Vector with the horizontal and vertical scroll values ​​of the mouse wheel.
   */
  static Math::Vec2 WheelScroll();

  /**
   * @brief Checks if a mouse button is pressed at the current time.
   *
   * @param button Mouse button to verify.
   *
   * @return True if the button is pressed, False otherwise.
   */
  static boolean InputDown(Inputs::MouseButton button);

  /**
   * @brief Checks if a mouse button was pressed in the last frame.
   *
   * @param button Mouse button to verify.
   *
   * @return True if the button was pressed, False otherwise.
   */
  static boolean InputPress(Inputs::MouseButton button);

  /**
   * @brief Sets the desired number of frames per second (FPS).
   *
   * @param fps Desired number of frames per second.
   */
  static void SetDesiredFPS(f32 fps);

  /**
   * @brief Enables or disables FPS control by the engine.
   *
   * @param fps_control True to enable FPS control, False to disable it.
   */
  static void SetFPSControl(boolean fps_control);

  /**
   * @brief Gets the last file path dropped into the window.
   *
   * @return Path of the last dropped file.
   */
  static const byte *LastDroppedFilePath();

  /**
   * @brief Adds a complex light based on the given point light.
   *
   * The shadow sharpness parameter controls the sharpness of shadows cast by the light.
   *
   * @param light The PointLight to use as the basis for the complex light.
   * @param shadow_sharpness The sharpness of shadows cast by the light.
   *
   * @return The identifier (Id) assigned to the added PointLight.
   */
  static PointLight::Id AddLight(PointLight light);

  /**
   * @brief Retrieves a pointer to the PointLight with the specified index.
   *
   * @param index The identifier of the PointLight.
   *
   * @return A pointer to the PointLight with the specified index, or nullptr if not found.
   */
  static PointLight *GetPointLight(PointLight::Id index);

  /**
   * @brief Adds a complex light based on the provided spot light.
   *
   * The shadow sharpness parameter controls the sharpness of shadows cast by the light.
   *
   * @param light The SpotLight to use as the basis for the complex light.
   * @param shadow_sharpness The sharpness of shadows cast by the light.
   *
   * @return The identifier (Id) assigned to the added SpotLight.
   */
  static SpotLight::Id AddLight(SpotLight light);

  /**
   * @brief Retrieves a pointer to the SpotLight with the specified index.
   *
   * @param index The identifier of the SpotLight.
   *
   * @return A pointer to the SpotLight with the specified index, or nullptr if not found.
   */
  static SpotLight *GetSpotLight(SpotLight::Id index);

  /**
   * @brief Adds a complex light based on the provided directional light.
   *
   * @param light The DirectionalLight to be used as the basis for the complex light.
   * @param shadow_sharpness The sharpness of shadows cast by the light.
   *
   * @return The identifier (Id) assigned to the added DirectionalLight.
   */
  static DirectionalLight::Id AddLight(DirectionalLight light);

  /**
   * @brief Retrieves a pointer to the DirectionalLight with the specified index.
   *
   * @param index The identifier of the DirectionalLight.
   *
   * @return A pointer to the DirectionalLight with the specified index, or nullptr if not found.
   */
  static DirectionalLight *GetDirectionalLight(DirectionalLight::Id index);

  /**
   * @brief Adds an error to the ErrorLog.
   *
   * @param error The error message.
   * @param function The function where the error occurred.
   * @param line The line number where the error occurred.
   */
  static void AddError(std::string error = "\0", std::string function = "\0", std::string line = "\0");

  /**
   * @brief Retrieves the last error from the ErrorLog.
   *
   * @return Error structure containing information about the last error.
   */
  static ErrorLog::Error GetError();

private:
  /**
   * @brief Private constructor.
   *
   * Not intended to be instantiated.
   */
  JAM_Engine();

  /**
   * @brief Private destructor.
   *
   * Not intended to be instantiated.
   */
  ~JAM_Engine();
};

/**
 * @brief Access the sound core instance (SoundCore).
 */
#define SC (SoundCore::Instance())

/**
 * @brief Access the entity manager instance (EntityManager).
 */
#define EM (EntityManager::Instance())

/**
 * @brief Access the task manager instance (TaskManager).
 */
#define TM (TaskManager::Instance())

#endif /* __JAM_ENGINE_H__ */