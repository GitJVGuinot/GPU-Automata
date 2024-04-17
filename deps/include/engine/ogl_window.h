/// @author F.c.o Javier guinot Almenar <guinotal@esat-alumni.com>
/// @file opengl_window.h

#include "engine.h"

///////////////////////////
#ifndef __OGL_WINDOW_H__
#define __OGL_WINDOW_H__
///////////////////////////

/**
 * @class OGLWindow
 *
 * @brief Class representing an OpenGL window with configuration settings.
 */
class OGLWindow
{
public:
  /**
   * @struct Config
   *
   * @brief Structure containing configuration options for the OpenGL window.
   */
  struct Config
  {
    byte name_[16] = "\0";         ///< Name/title of the window.
    boolean is_exec_name_ = false; ///< Flag indicating if the executable name is used as the window name.

    s32 width_ = 0, height_ = 0;     ///< Width and Height of the window.
    boolean fullscreen_ = false;     ///< Flag indicating if the window should be in fullscreen mode.
    boolean make_resizable_ = false; ///< Flag indicating if the window should be resizable.
  };

  ErrorLog logger;

  /**
   * @brief Constructor to initialize the window, GLFW, and Freetype font.
   *
   * @param config Configuration options for the OpenGL window.
   */
  OGLWindow(Config config);

  /**
   * @brief Destructor
   */
  ~OGLWindow();

  /**
   * @brief to destroy the window and terminate GLFW.
   */
  void free();

  /**
   * @brief Set the desired frames per second (FPS) for the window.
   *
   * @param desiredFPS The desired frames per second.
   */
  void setDesiredFPS(f32 desiredFPS);

  /**
   * @brief Enable or disable frame rate control.
   *
   * @param frameControl Flag indicating whether to control the frame rate.
   */
  void setFPSControl(boolean frameControl);

  /**
   * @brief Get the time elapsed between frames.
   *
   * @return The time elapsed between frames in seconds.
   */
  f32 deltaTime();

  /**
   * @brief Get the path of the last dropped file.
   *
   * @return The path of the last dropped file.
   */
  const byte *getDroppedFilePath() const;

  /**
   * @brief Get the last mouse scroll movement.
   *
   * @return A 2D vector representing the mouse scroll movement.
   */
  const Math::Vec2 getMouseScroll() const;

  /**
   * @brief Callback function for dropped files.
   *
   * @param count The number of dropped files.
   * @param paths An array containing the paths of the dropped files.
   */
  void dropCallback(s32 count, const byte *paths[]);

  /**
   * @brief Callback function for mouse wheel scroll.
   *
   * @param xoffset The horizontal scroll offset.
   * @param yoffset The vertical scroll offset.
   */
  void scrollCallback(f64 xoffset, f64 yoffset);

  /**
   * @brief Callback function for resizing the window.
   *
   * @param width The new width of the window.
   * @param height The new height of the window.
   */
  void frameCallback(s32 width, s32 height);

  /**
   * @brief Check if the window should be closed.
   *
   * @return True if the window should be closed, false otherwise.
   */
  boolean shouldClose() const;

  /**
   * @brief Close the window.
   */
  void close();

  /**
   * @brief Check if a key is currently being pressed.
   *
   * @param key The key code.
   *
   * @return True if the key is currently being pressed, false otherwise.
   */
  boolean isKeyPressed(s32 key) const;

  /**
   * @brief Check if a key was pressed once.
   *
   * @param key The key code.
   *
   * @return True if the key was pressed once, false otherwise.
   */
  boolean isKeyDown(s32 key) const;

  /**
   * @brief Check if a mouse button is currently being pressed.
   *
   * @param button The mouse button code.
   *
   * @return True if the mouse button is currently being pressed, false otherwise.
   */
  boolean isMouseButtonPressed(s32 button) const;

  /**
   * @brief Check if a mouse button was pressed once.
   *
   * @param button The mouse button code.
   *
   * @return True if the mouse button was pressed once, false otherwise.
   */
  boolean isMouseButtonDown(s32 button) const;

  /**
   * @brief Get the current cursor position.
   *
   * @param xpos Reference to store the X-coordinate of the cursor.
   * @param ypos Reference to store the Y-coordinate of the cursor.
   */
  void getCursorPos(f64 &xpos, f64 &ypos) const;

  /**
   * @brief Get the size of the window.
   *
   * @param width Reference to store the width of the window.
   * @param height Reference to store the height of the window.
   */
  void getSize(s32 &width, s32 &height) const;

  /**
   * @brief Start a new frame for rendering.
   */
  void frameStart();

  /**
   * @brief Swap the front and back buffers, poll for and process events. Use at the end of the rendering loop.
   */
  void refresh();

private:
  void *window_;                                                          ///< Pointer to the GLFW window.
  mutable s32 last_key_state_[GLFW_KEY_LAST] = {GLFW_RELEASE};            ///< Array to store the last state of each key.
  mutable s32 last_mouse_state_[GLFW_MOUSE_BUTTON_LAST] = {GLFW_RELEASE}; ///< Array to store the last state of each mouse button.
  s32 width_, height_;                                                    ///< Width and height of the window.
  byte last_dropped_file_path_[4096] = "\0";                              ///< Path of the last dropped file.
  Math::Vec2 mouse_scroll_;                                               ///< Last mouse scroll movement.

  f32 delta_time_;      ///< Time elapsed between frames
  f32 fps_;             ///< Frames per second
  boolean fps_control_; ///< Flag to control frame rate
  TimeCont timer_;      ///< Timer for frame rate control
};

/**
 * @brief Alias for OGLWindow as Window.
 */
typedef OGLWindow Window;

////////////////////////////////
#endif /* __OGL_WINDOW_H__ */
       ////////////////////////////////