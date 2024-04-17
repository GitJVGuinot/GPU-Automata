#ifndef __ENGINE_H__
#define __ENGINE_H__ 1

// System Includes
///////////////////////////////////////////////////////////////////////////////
#include <memory_resource>
#include <unordered_map>
#include <functional>
#include <stdexcept>
#include <iostream>
#include <optional>
#include <typeinfo>
#include <numeric>
#include <fstream>
#include <sstream>
#include <ostream>
#include <cstring>
#include <cassert>
#include <thread>
#include <random>
#include <future>
#include <vector>
#include <memory>
#include <mutex>
#include <queue>
#include <tuple>
///////////////////////////////////////////////////////////////////////////////

// Deps Libs
///////////////////////////////////////////////////////////////////////////////
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <AL/al.h>
#include <AL/alc.h>
#ifdef _WIN32
#include <tiny_obj_loader.h>
#elif __linux__
#include <tiny_obj/tiny_obj_loader.h>
#endif
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <stb/stb_image.h>

#ifdef _WIN32
#elif __linux__
#endif
///////////////////////////////////////////////////////////////////////////////

// Defines
///////////////////////////////////////////////////////////////////////////////
#include <engine/defines.h>
///////////////////////////////////////////////////////////////////////////////

// Engine non-dependent includes
///////////////////////////////////////////////////////////////////////////////
#include <engine/types.h>
#include <engine/inputs.h>
#include <engine/timecont.h>
#include <engine/math/mathlib.h>
#include <engine/filemanager.h>
#include <engine/gpumanager.h>
///////////////////////////////////////////////////////////////////////////////

// Engine dependents includes
///////////////////////////////////////////////////////////////////////////////
#include <engine/taskmanager.h>
#include <engine/soundcore.h>
#include <engine/errorlog.h>
#include <engine/transform.h>
#include <engine/material.h>
#include <engine/texture.h>
#include <engine/mesh.h>
#include <engine/entity.h>
#include <engine/light.h>
#include <engine/shadows.h>
#include <engine/ogl_window.h>
#include <engine/camera.h>
///////////////////////////////////////////////////////////////////////////////

#endif /* __ENGINE_H__ */