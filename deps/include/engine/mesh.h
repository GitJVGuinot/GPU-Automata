#include "engine.h"

#ifndef __MESH_H__
#define __MESH_H__ 1

/**
 * @struct Vertex
 *
 * @brief Represents a 3D vertex with position, normal, and texture coordinates.
 */
struct Vertex
{
  Math::Vec3 position_;  ///< 3D position of the vertex.
  Math::Vec3 normal_;    ///< Normal vector of the vertex.
  Math::Vec2 texCoords_; ///< Texture coordinates of the vertex.
};

/**
 * @struct CustomMesh
 *
 * @brief Structure representing a custom mesh.
 */
struct CustomMesh
{
  u32 *indices_;      ///< Pointer to an array of indices
  u32 indices_size_;  ///< Size of the indices array
  Vertex *vertices_;  ///< Pointer to an array of vertices.
  u32 vertices_size_; ///< Size of the vertices array.
  boolean has_mesh_;  ///< Flag indicating whether the mesh has been successfully created.
};

enum class DrawMode : s16
{
  Points = 0,
  Lines,
  Triangles,
};

enum class CullMode : s16
{
  Front = 0,
  Back,
  FrontAndBack,
};

enum class CullFront : s16
{
  Clockwise = 0,
  CounterClockwise,
};

struct DrawConfig
{
  DrawMode mode_ = DrawMode::Triangles;

  boolean active_culling_ = true;
  CullMode cll_mode_ = CullMode::Back;
  CullFront cll_face_ = CullFront::Clockwise;
};

/**
 * @class Mesh
 *
 * @brief Class for handling 3D meshes.
 */
class Mesh
{
public:
  /**
   * @enum Platonic
   *
   * @brief Enumeration representing Platonic shapes.
   */
  enum class Platonic : s16
  {
    // 2D - Forms
    k_Triangle = 0,  ///< Triangle shape.
    k_Quad,          ///< Quad shape.
    k_Circunference, ///< Circumference shape.

    // 3D - Forms
    k_Pyramid, ///< Pyramid shape.
    k_Cube,    ///< Cube shape.
    k_Sphere,  ///< Sphere shape.

    k_MaxForms, ///< Maximum number of Platonic shapes.
  };

  /**
   * @brief Typedefinition for the Id type used in the Mesh class.
   */
  typedef uint32_t Id;

  DrawConfig dr_config_;

  /**
   * @brief Constructor for creating an empty Mesh.
   *
   * Constructor to load a mesh from an OBJ file.
   */
  Mesh();

  /**
   * @brief Destructor
   */
  ~Mesh();

  /**
   * @brief for releasing resources associated with the Mesh.
   */
  void free();

  /**
   * @brief Renders the mesh.
   */
  void render() const;

  /**
   * @brief Checks if the mesh has been loaded.
   *
   * @return True if the mesh has been loaded, false otherwise.
   */
  boolean hasMesh() const;

  /**
   * @brief Loads a mesh from an OBJ file.
   *
   * Function to load a mesh from an OBJ file.
   *
   * @param filePath The path to the OBJ file.
   * @param normalize Flag to center and normalize de mesh
   */
  void loadMesh(const std::string &filePath, boolean normalize);

  /**
   * @brief Loads a custom mesh using a user-defined callback.
   *
   * @param load_mesh_callback A callback function to load the mesh.
   */
  void loadCustomMesh(CustomMesh *(*load_mesh_callback)());

private:
  mutable u32 VAO, VBO, EBO; ///< Vertex Array Object (VAO), Vertex Buffer Object (VBO) and Elements Buffer Object (EBO) for rendering.
  boolean has_mesh_;         ///< Flag indicating whether the mesh has been loaded.
  u32 *indices_;             ///< Array of indices
  u32 indices_size_;         ///< Sizeo of array indices
  u32 vertices_size_;        ///< Size of the vertices array.
  Vertex *vertices_;         ///< Array of vertices representing the mesh.

  /**
   * @brief Loads the buffers for rendering the mesh.
   */
  void loadBuffers() const;

  /**
   * @brief Centers and normalize the mesh within the given bounding box.
   *
   * @param min Minimum corner of the bounding box.
   * @param max Maximum corner of the bounding box.
   */
  void centerAndNormalize(Math::Vec3 min, Math::Vec3 max);
};

#endif /* __MESH_H__ */