cmake_minimum_required(VERSION 3.20)

# Crosscompiling
include(cmake/crosscompile.cmake)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

project(TupoyeMenu CXX ASM_MASM)

set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")

# Git commit embed
include(cmake/git.cmake)

# Fetch modules
message("\nFetching modules")
include(cmake/asmjit.cmake)
include(cmake/minhook.cmake)
include(cmake/async-logger.cmake)
include(cmake/pugixml.cmake)
include(cmake/json.cmake)
include(cmake/lua.cmake)

message("\nFetching custom modules")
include(cmake/imgui.cmake)
include(cmake/gtav-classes.cmake)

# Build options
include(cmake/build_options.cmake)

# TupoyeMenu
message(STATUS "TupoyeMenu")
file(GLOB_RECURSE SRC_MAIN
    "${SRC_DIR}/**.hpp"
    "${SRC_DIR}/**.h"
    "${SRC_DIR}/**.cpp"
    "${SRC_DIR}/**.cc"
    "${SRC_DIR}/**.cxx"
    "${SRC_DIR}/**.asm"
)

if(NOT ENABLE_LUA)
    list(FILTER SRC_MAIN EXCLUDE REGEX "lua/")
endif()

if (MSVC)
    add_compile_options(/bigobj)
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
else ()
    add_compile_options(-Wa,-mbig-obj)
endif ()

add_library(TupoyeMenu MODULE "${SRC_MAIN}")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(TARGET TupoyeMenu PROPERTY CXX_STANDARD 23)

source_group(TREE ${SRC_DIR} PREFIX "src" FILES ${SRC_MAIN} )

target_include_directories(TupoyeMenu PRIVATE 
    "${SRC_DIR}"
    "${json_SOURCE_DIR}/single_include"
    "${gtav_classes_SOURCE_DIR}"
    "${imgui_SOURCE_DIR}"
    "${minhook_SOURCE_DIR}/src/hde"
    "${asmjit_SOURCE_DIR}/src"
)


target_precompile_headers(TupoyeMenu PRIVATE "${SRC_DIR}/common.hpp")
target_link_libraries(TupoyeMenu PRIVATE pugixml minhook AsyncLogger dbghelp imgui lua_static asmjit::asmjit d3dcompiler dwmapi winmm psapi)

# Actually use build options
include(cmake/build_options_impl.cmake)

# Warnings as errors
if(NOT CROSSCOMPILE)
    set_property(TARGET TupoyeMenu PROPERTY COMPILE_WARNING_AS_ERROR ON)
endif()


add_compile_definitions(TupoyeMenu
    "_CRT_SECURE_NO_WARNINGS"
    "NOMINMAX"
    "WIN32_LEAN_AND_MEAN"
    "YimMenu"
)
