#include "engine.h"

#ifndef __TEXTURE_H__
#define __TEXTURE_H__ 1

/**
 * @class Texture
 *
 * @brief Represents a 2D texture with various options for wrapping and filtering.
 */
class Texture
{
public:
  typedef uint32_t Id; ///< Type definition for the texture identifier.

  /**
   * @enum Wrap
   *
   * @brief Enumeration for texture wrap modes.
   */
  enum class Wrap : s16
  {
    Repeat = 0,      ///< Repeat texture coordinates.
    Mirrored_Repeat, ///< Repeat with mirroring.
    Clamp_To_Edge,   ///< Clamp to the edge.
    Clamp_To_Border  ///< Clamp to the border.
  };

  /**
   * @enum Filter
   *
   * @brief Enumeration for texture filtering modes.
   */
  enum class Filter
  {
    Linear = 0,            ///< Linear interpolation.
    Nearest,               ///< Nearest-neighbor interpolation.
    Linear_Mipmap_Linear,  ///< Linear interpolation with mipmapping.
    Linear_Mipmap_Nearest, ///< Linear interpolation with nearest-neighbor mipmapping.
    Nearest_Mipmap_Nearest ///< Nearest-neighbor interpolation with nearest-neighbor mipmapping.
  };

  /**
   * @brief Constructor for the Texture class.
   *
   * @param wrapS Wrap mode for the S-axis.
   * @param wrapT Wrap mode for the T-axis.
   * @param minFilter Minification filter.
   * @param magFilters Magnification filter.
   */
  Texture(Wrap wrapS = Wrap::Repeat, Wrap wrapT = Wrap::Repeat, Filter minFilter = Filter::Linear, Filter magFilters = Filter::Linear);

  /**
   * @brief Destructor for the Texture class.
   */
  ~Texture();

  /**
   * @brief Free all the resources
   */
  void free();

  /**
   * @brief Checks if the Texture has a valid texture loaded.
   *
   * @return True if a texture is loaded, false otherwise.
   */
  boolean hasTexture() const;

  /**
   * @brief Loads a texture from the specified file path.
   *
   * @param filePath Path to the image file.
   */
  void loadTexture(const std::string &filePath);

  /**
   * @brief Gets the identifier of the texture.
   *
   * @return Texture identifier.
   */
  u32 id();

  /**
   * @brief Gets the raw data of the texture.
   *
   * @return Pointer to the texture data.
   */
  u_byte *data() const;

  /**
   * @brief Gets the width of the texture.
   *
   * @return Width of the texture.
   */
  s32 width() const;

  /**
   * @brief Gets the height of the texture.
   *
   * @return Height of the texture.
   */
  s32 height() const;

  /**
   * @brief Gets the number of channels in the texture.
   *
   * @return Number of channels.
   */
  s32 channels() const;

private:
  u32 id_;              ///< Texture identifier.
  u_byte *data_;        ///< Raw texture data.
  s32 width_, height_;  ///< Dimensions of the texture
  s32 channels_;        ///< Number of color channels in the texture.
  boolean has_texture_; ///< Flag indicating whether a texture is loaded.
  Wrap wrapS_, wrapT_;  ///< Wrap modes for the S and T axes.
  Filter minF_, magF_;  ///< Minification and magnification filter modes.

  /**
   * @brief Loads texture data into OpenGL buffers.
   */
  void loadBuffers();
};

#endif /* __TEXTURE_H__ */