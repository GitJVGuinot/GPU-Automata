#include "engine.h"

#ifndef __ENTITY_H__
#define __ENTITY_H__ 1

namespace Entity
{
  typedef uint32_t Id;
}

/**
 * @class Treenode
 *
 * @brief Class that represents a node in a tree structure.
 *
 * The Treenode class is used to represent nodes in a tree structure,
 * where each node can have up to a maximum number of children.
 */
///////////////////////////////////////////////////////////////////////////////
class Treenode
{
public:
  const static u32 k_max_childs = 4; ///< Maximum number of children that each node can have.

  /**
   * @brief Treenode class constructor.
   *
   * @param index Unique identifier associated with the node.
   */
  Treenode(Entity::Id index);

  /**
   * @brief Treenode class destructor.
   */
  ~Treenode();

  /**
   * @brief Gets the index associated with the node.
   *
   * @return Node index.
   */
  Entity::Id index();

  /**
   * @brief Sets a child at a specific position of the node.
   *
   * @param index Child index to set.
   *
   * @return 0 if successful, otherwise an error code.
   */
  u32 setChild(u32 index);

  /**
   * @brief Gets the index of the child at a specific position in the node.
   *
   * @param n_child Position of the son to get.
   *
   * @return Child index at the specified position.
   */
  u32 getChild(u32 n_child);

private:
  Entity::Id index_;                     ///< Unique identifier associated with the node.
  Entity::Id child_index_[k_max_childs]; ///< Array that stores the indexes of the node's children.
};
///////////////////////////////////////////////////////////////////////////////

namespace Entity
{
  /**
   * @struct ComponentBase
   *
   * @brief Base interface for components of an entity system.
   *
   * Used to make an undefined class array.
   */
  /////////////////////////////////////////////////////////////////////////////
  struct ComponentBase
  {
    /**
     * @brief Grows the size of components.
     *
     * @param adds size component to add
     */
    virtual void grow(size_t adds) = 0;

    /**
     * @brief Decreasethe size of the component.
     *
     * @param erase size component to decrease
     */
    virtual void decrease(size_t erase) = 0;

    /**
     * @brief Gets the current size of the component.
     *
     * @return Current size of the component.
     */
    virtual size_t size() = 0;

    /**
     * @brief Clear the content of the component.
     *
     * This function clears the content of the component, leaving it in an
     * initial state.
     */
    virtual void clear() = 0;

    /**
     * @brief Erase the component associated with a specific entity.
     *
     * @param e Identifier of the entity for which the component is reset.
     */
    virtual void erase(size_t e) = 0;
  };
  /////////////////////////////////////////////////////////////////////////////

  /**
   * @struct Components
   *
   * @brief Implementation of the base interface for a list of components.
   *
   * This template structure implements the base interface 'ComponentBase'
   * and provides a list of components of type T associated with entities
   * in an entity system.
   *
   * @tparam T Type of data that this component stores.
   */
  /////////////////////////////////////////////////////////////////////////////
  template <typename T>
  struct Components : ComponentBase
  {
    boolean *has_value_ = nullptr; ///< Flag to know if components has a value
    T *components_ = nullptr;      ///< Save a specific type components
    size_t size_ = 0;

    /**
     * @brief Grows the size of components.
     *
     * @param adds size component to add
     */
    void grow(size_t adds) override
    {
      size_t prev_size = size_;
      size_ += adds;

      components_ = reinterpret_cast<T *>(std::realloc(reinterpret_cast<void *>(components_), (size_ * sizeof(T))));
      if (!components_)
        throw std::bad_alloc();

      has_value_ = reinterpret_cast<boolean *>(std::realloc(has_value_, (size_ * sizeof(boolean))));
      if (!has_value_)
        throw std::bad_alloc();

      for (size_t i = prev_size; i < size_; i++)
        *(has_value_ + i) = false;
    }

    /**
     * @brief Decrease the size of component errasing the latest one
     *
     * @param erase size component to decrease
     */
    void decrease(size_t erase) override
    {
      size_ -= erase;
      if (size_ <= 0)
      {
        size_ = 0;
        DESTROY(components_);
        DESTROY(has_value_);
        return;
      }

      T *new_components = reinterpret_cast<T *>(std::realloc(reinterpret_cast<void *>(components_), (size_ * sizeof(T))));

      if (!components_)
        throw std::bad_alloc();
      components_ = new_components;

      has_value_ = reinterpret_cast<boolean *>(std::realloc(has_value_, (size_ * sizeof(boolean))));
      if (!has_value_)
        throw std::bad_alloc();
    }

    /**
     * @brief Gets the current size of the component.
     *
     * @return Current size of the component.
     */
    size_t size() override { return size_; }

    /**
     * @brief Clears the contents of the component list.
     *
     * This function clears the contents of the component list, leaving it
     * in an initial state.
     */
    void clear() override
    {
      size_ = 0;
      DESTROY(components_);
      DESTROY(has_value_);
    }

    /**
     * @brief Erase the component associated with a specific entity.
     *
     * @param entity_id Identifier of the entity for which the component is erased.
     */
    void erase(size_t entity_id) override
    {
      if (entity_id >= size_)
        throw std::out_of_range("Entity ID is out of range");

      if (size_ == 1)
      {
        size_ = 0;
        DESTROY(components_);
        DESTROY(has_value_);
        return;
      }

      T *c_it = (components_ + entity_id);
      T *c_next = (c_it + 1);
      T *c_end = (components_ + size_);

      boolean *v_it = (has_value_ + entity_id);
      boolean *v_next = (v_it + 1);
      boolean *v_end = (has_value_ + size_);
      for (; c_next != c_end && v_next != v_end; c_it++, c_next++, v_it++, v_next++)
      {
        *c_it = std::move(*c_next);
        *v_it = std::move(*v_next);
      }

      this->decrease(1);
    }

    /**
     * @brief Assigns a component to a specific entity.
     *
     * @param entity_id Identifier of the entity to which the component is assigned.
     * @param component Value of the component to be assigned.
     */
    void emplace(size_t entity_id, const T &component)
    {
      if (entity_id >= size_)
        this->grow(entity_id - size_ + 1);

      assert(*(has_value_ + entity_id) == false);
      *(components_ + entity_id) = component;
      *(has_value_ + entity_id) = true;
    }
  };
  /////////////////////////////////////////////////////////////////////////////

  /**
   * @struct ComponentManager
   *
   * @brief Centralized manager for the administration of components associated with entities.
   *
   * The ComponentManager manages and coordinates different types of components
   * associated with entities in an entity system. Provides functions for
   * adding component types, assigning components to entities, getting
   * components from entities, and managing the creation and deletion
   * of entities.
   */
  /////////////////////////////////////////////////////////////////////////////
  struct ComponentsManager
  {
    /// Map that associates component types with component instances.
    std::unordered_map<size_t, std::unique_ptr<ComponentBase>> component_type_list_;
    size_t entities_count_;

    /**
     * @brief ComponentManager constructor.
     *
     * Initializes a new ComponentManager.
     */
    ComponentsManager();

    /**
     * @brief ComponentManager destructor.
     */
    ~ComponentsManager() {}

    /**
     * @brief Releases the resources associated with component instances.
     */
    void clear();

    /**
     * @brief Adds a component type to the list of types managed by the ComponentManager.
     *
     * @tparam T Type of component to add.
     */
    template <typename T>
    void addComponent()
    {
      component_type_list_.insert(std::make_pair(typeid(T).hash_code(), std::move(std::make_unique<Components<T>>())));
    }

    /**
     * @brief Set a component to a specific entity.
     *
     * @tparam T Type of component to set.
     *
     * @param entity_id Identifier of the entity to which the component is assigned.
     * @param component Value of the component to be assigned.
     */
    template <typename T>
    void setComponent(size_t entity_id, const T &component)
    {
      assert(entity_id < entities_count_);

      size_t hc = typeid(T).hash_code();

      assert(component_type_list_.contains(hc));
      auto it = component_type_list_.find(hc);

      Components<T> *list_it = reinterpret_cast<Components<T> *>(it->second.get());
      list_it->emplace(entity_id, component);
    }

    /**
     * @brief Gets a pointer to the component associated with a specific entity.
     *
     * @tparam T Type of component to get.
     *
     * @param entity_id Identifier of the entity for which the component is claimed.
     *
     * @return Pointer to the component associated with the entity, or nullptr if it does not exist.
     */
    template <typename T>
    T *getComponent(size_t entity_id)
    {
      if (entity_id >= entities_count_)
        return nullptr;

      auto hc = typeid(T).hash_code();

      assert(component_type_list_.contains(hc));
      auto it = component_type_list_.find(hc);

      Components<T> *list_it = reinterpret_cast<Components<T> *>(it->second.get());

      auto *ret = (list_it->components_ + entity_id);
      if (*(list_it->has_value_ + entity_id) == false)
        return nullptr;

      return ret;
    }

    /**
     * @brief Gets a pointer to the components list of an entity by its identifier.
     *
     * @tparam T Type of component to get.
     *
     * @return a std::pair markin the initial ptr to components & flag has value
     */
    template <typename T>
    std::pair<T *, boolean *> getComponentsList()
    {
      auto hc = typeid(T).hash_code();

      assert(component_type_list_.contains(hc));
      auto it = component_type_list_.find(hc);

      Components<T> *list_it = reinterpret_cast<Components<T> *>(it->second.get());

      return std::make_pair(list_it->components_, list_it->has_values_);
    }

    /**
     * @brief Creates a new entity and returns its identifier.
     *
     * @return Identifier of the new entity created.
     */
    size_t newEntity();

    /**
     * @brief Deletes an entity and releases its associated components.
     *
     * @param entity_id Identifier of the entity to remove.
     */
    void removeEntity(size_t entity_id);
  };
  /////////////////////////////////////////////////////////////////////////////
} // namespace Entity

/**
 * @class EntityManager
 *
 * @brief Class that manages the creation, destruction and manipulation of entities and their components.
 */
///////////////////////////////////////////////////////////////////////////////
class EntityManager : private Entity::ComponentsManager
{
public:
  /**
   * @brief Gets the single instance of the EntityManager.
   *
   * @return Pointer to the single instance of the EntityManager.
   */
  static EntityManager *Instance();

  /**
   * @brief Free all resources.
   */
  void clear();

  /**
   * @brief Create a new entity and assign an optional name.
   *
   * @param name Optional name for the entity.
   *
   * @return Unique identifier of the new created entity.
   */
  Entity::Id newEntity(const char *name = nullptr);

  /**
   * @brief Removes an entity by its identifier.
   *
   * @param entity_id Identifier of the entity to delete.
   */
  void removeEntity(Entity::Id entity_id);

  /**
   * @brief Establishes an entity as a child of another entity.
   *
   * @param father Identifier of the parent entity.
   * @param child Identifier of the child entity.
   */
  void setChild(Entity::Id father, Entity::Id child);

  /**
   * @brief Adds a component type to the list of types managed by the EntityManager.
   *
   * @tparam T Type of component to add.
   */
  template <typename T>
  void addComponent() { ComponentsManager::addComponent<T>(); }

  /**
   * @brief Set a component to an entity by its identifier.
   *
   * @tparam T Type of component to set.
   *
   * @param entity_id Identifier of the entity to which the component is assigned.
   * @param component Value of the component to set.
   */
  template <typename T>
  void setComponent(Entity::Id entity_id, const T &component)
  {
    size_t internal_id = static_cast<size_t>(-1);

    auto i_it = ids_map_.begin();
    auto i_end = ids_map_.end();

    for (; i_it != i_end; i_it++)
      if (i_it->first == entity_id)
      {
        internal_id = i_it->second;
        break;
      }

    if (internal_id == static_cast<size_t>(-1))
      return;

    ComponentsManager::setComponent(internal_id, component);
  }

  /**
   * @brief Gets a pointer to the component of an entity by its identifier.
   *
   * @tparam T Type of component to get.
   *
   * @param entity_id Identifier of the entity for which the component is obtained.
   *
   * @return Pointer to the component associated with the entity, or nullptr if it does not exist.
   */
  template <typename T>
  T *getComponent(Entity::Id entity_id)
  {
    size_t internal_id = static_cast<size_t>(-1);

    auto i_it = ids_map_.begin();
    auto i_end = ids_map_.end();

    for (; i_it != i_end; i_it++)
      if (i_it->first == entity_id)
      {
        internal_id = i_it->second;
        break;
      }

    if (internal_id == static_cast<size_t>(-1))
      return nullptr;

    return ComponentsManager::getComponent<T>(internal_id);
  }

  /**
   * @brief Gets a pointer to the components list of an entity by its identifier.
   *
   * @tparam T Type of component to get.
   *
   * @return Pointer to the component list associated with the entity, or nullptr if it does not exist.
   */
  template <typename T>
  T *getComponentsList() { return ComponentsManager::getComponentsList<T>(); }

  /**
   * @brief Gets the name associated with an entity by its identifier.
   *
   * @param entity_id Identifier of the entity for which the name is obtained.
   *
   * @return Name associated with the entity, or "Invalid" if the entity does not exist.
   */
  std::string getName(Entity::Id entity_id) const;

  /**
   * @brief Gets the identifier associated with an entity by name.
   *
   * @param name Name of the entity for which the identifier is obtained.
   *
   * @return Identifier associated with the entity, or the invalid entity identifier if the entity does not exist.
   */
  Entity::Id getId(const std::string name) const;

private:
  /**
   * @brief Constructor of the EntityManager class.
   */
  EntityManager();

  /**
   * @brief Constructor of the EntityManager class.
   */
  ~EntityManager();

  boolean cleared_;    ///< Flag to know if is necessary to clean or not.
  Entity::Id invalid_; ///< Invalid entity identifier.

  Entity::Id external_counter_; ///< External counter to assign unique identifiers to entities.

  // First External ID / Second Internal ID
  std::vector<std::pair<Entity::Id, size_t>> ids_map_; ///< Mapping between external and internal identifiers.

  // First Name / Second External ID
  std::vector<std::pair<std::string, Entity::Id>> names_map_; ///< Mapping between entity names and external identifiers.

  size_t maps_size_;
};

/**
 * @brief Accesses the EntityManager instance.
 */
#define EM (EntityManager::Instance())
/////////////////////////////////////////////////////////////////////////////

#endif /* __ENTITY_H__ */