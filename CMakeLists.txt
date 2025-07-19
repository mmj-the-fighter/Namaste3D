cmake_minimum_required(VERSION 3.14)
project(Namaste3DSolution)

# Set project variables
set(POINTS_RENDERER points_rendering)
set(WIREFRAME_RENDERER wireframe_rendering)
set(SOLIDCOLOR_RENDERER solid_color_rendering)
set(COLOR_INTERPOLATION color_interpolation)
set(LINEAR_TEXTURING linear_texturing)
set(PERSPECTIVE_CORRECT_TEXTURING perspective_correct_texturing)
set(DEPTH_BUFFERING depth_buffering)
set(LIGHTING_AND_SHADING lighting_and_shading)
set(SCENE_PARSING scene_parsing)

# Set standards
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Platform-specific settings
if(APPLE)
    add_compile_options(-Wno-deprecated-declarations)
    # Additional macOS flags
    add_compile_options(-O3 -Wall -Wextra)
elseif(UNIX AND NOT APPLE)
    # Linux flags
    add_compile_options(-O3 -Wall -Wextra -pthread)
elseif(WIN32)
    # Windows flags
    add_compile_options(/O2 /W3)
endif()

# SDL3 - Try to find system SDL3 first, then download if not found
find_package(SDL3 QUIET)
if(NOT SDL3_FOUND)
    message(STATUS "SDL3 not found in system, downloading from source...")
    include(FetchContent)
    
    FetchContent_Declare(
        SDL3
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG        release-3.2.0
        GIT_SHALLOW    TRUE
    )
    FetchContent_MakeAvailable(SDL3)
    
    # Set flags for built SDL3
    add_compile_definitions(SDL3_BUILT_FROM_SOURCE)
else()
    message(STATUS "Found system SDL3 installation")
endif()

# Spinach sources
file(GLOB_RECURSE SPN_CPP_SRC CONFIGURE_DEPENDS
    spinach/common/*.cpp
    spinach/core/*.cpp
)

file(GLOB_RECURSE SPN_C_SRC CONFIGURE_DEPENDS
    spinach/external/stb/*.c
)

file(GLOB_RECURSE SPN_H CONFIGURE_DEPENDS
    spinach/common/*.h
    spinach/common/*.hpp
    spinach/core/*.h
    spinach/core/*.hpp
    spinach/external/stb/*.h
)

# Helper function to add renderers
function(add_renderer_target name)
    string(TOLOWER "${name}" lname)
    
    # Verify main.cpp exists
    if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${lname}/main.cpp")
        message(FATAL_ERROR "main.cpp not found in ${lname} directory")
    endif()

    # Collect sources
    file(GLOB SRC CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/${lname}/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/${lname}/demo/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/${lname}/n3d/*.cpp"
    )

    # Collect headers
    file(GLOB HDR CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/${lname}/demo/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/${lname}/demo/*.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/${lname}/n3d/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/${lname}/n3d/*.hpp"
    )

    # Create executable
    add_executable(${name}
        ${SRC}
        ${SPN_CPP_SRC}
        ${SPN_C_SRC}
        ${HDR}
        ${SPN_H}
    )

    # Set include directories
    target_include_directories(${name} PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/spinach/common"
        "${CMAKE_CURRENT_SOURCE_DIR}/spinach/core"
        "${CMAKE_CURRENT_SOURCE_DIR}/spinach/external/stb"
        "${CMAKE_CURRENT_SOURCE_DIR}/${lname}/demo"
        "${CMAKE_CURRENT_SOURCE_DIR}/${lname}/n3d"
    )

    # Link libraries
    target_link_libraries(${name} PRIVATE SDL3::SDL3)

    # Set additional properties
    set_target_properties(${name} PROPERTIES
        CXX_STANDARD 17
        C_STANDARD 99
    )
endfunction()

# Add all renderers
add_renderer_target(${POINTS_RENDERER})
add_renderer_target(${WIREFRAME_RENDERER})
add_renderer_target(${SOLIDCOLOR_RENDERER})
add_renderer_target(${COLOR_INTERPOLATION})
add_renderer_target(${LINEAR_TEXTURING})
add_renderer_target(${PERSPECTIVE_CORRECT_TEXTURING})
add_renderer_target(${DEPTH_BUFFERING})
add_renderer_target(${LIGHTING_AND_SHADING})
add_renderer_target(${SCENE_PARSING})

# Installation (optional)
if(UNIX AND NOT APPLE)
    install(TARGETS ${POINTS_RENDERER} ${WIREFRAME_RENDERER} ${SOLIDCOLOR_RENDERER}
            DESTINATION bin)
endif()
