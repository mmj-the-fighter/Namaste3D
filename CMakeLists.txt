cmake_minimum_required(VERSION 3.14)
project(Namaste3DSolution)

# C/C++ Standards
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Renderer Targets
set(RENDERERS
    points_rendering
    wireframe_rendering
    solid_color_rendering
    color_interpolation
    linear_texturing
    perspective_correct_texturing
    depth_buffering
    lighting_and_shading
    scene_parsing
)

# Platform-specific flags
if(APPLE)
    add_compile_options(-Wno-deprecated-declarations -O3 -Wall -Wextra)
elseif(UNIX AND NOT APPLE)
    add_compile_options(-O3 -Wall -Wextra -pthread)
endif()

# SDL3 - Try system, else fetch from source
find_package(SDL3 QUIET)
if(NOT SDL3_FOUND)
    message(STATUS "SDL3 not found, downloading...")
    include(FetchContent)
    FetchContent_Declare(
        SDL3
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-3.2.0
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(SDL3)
    add_compile_definitions(SDL3_BUILT_FROM_SOURCE)
else()
    message(STATUS "Found system SDL3")
endif()

# Spinach sources
file(GLOB_RECURSE SPN_CPP_SRC CONFIGURE_DEPENDS spinach/common/*.cpp spinach/core/*.cpp)
file(GLOB_RECURSE SPN_C_SRC CONFIGURE_DEPENDS spinach/external/stb/*.c)
file(GLOB_RECURSE SPN_H CONFIGURE_DEPENDS 
    spinach/common/*.[h|hpp] 
    spinach/core/*.[h|hpp] 
    spinach/external/stb/*.h
)

# Function to define renderer targets
function(add_renderer_target name)
    string(TOLOWER "${name}" lname)

    if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${lname}/main.cpp")
        message(FATAL_ERROR "main.cpp not found in ${lname}")
    endif()

    file(GLOB SRC CONFIGURE_DEPENDS
        "${lname}/*.cpp"
        "${lname}/demo/*.cpp"
        "${lname}/n3d/*.cpp"
    )

    file(GLOB HDR CONFIGURE_DEPENDS
        "${lname}/demo/*.[h|hpp]"
        "${lname}/n3d/*.[h|hpp]"
    )

    add_executable(${name}
        ${SRC}
        ${HDR}
        ${SPN_CPP_SRC}
        ${SPN_C_SRC}
        ${SPN_H}
    )

    target_include_directories(${name} PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/spinach/common"
        "${CMAKE_CURRENT_SOURCE_DIR}/spinach/core"
        "${CMAKE_CURRENT_SOURCE_DIR}/spinach/external/stb"
        "${CMAKE_CURRENT_SOURCE_DIR}/${lname}/demo"
        "${CMAKE_CURRENT_SOURCE_DIR}/${lname}/n3d"
    )

    target_link_libraries(${name} PRIVATE SDL3::SDL3)

    set_target_properties(${name} PROPERTIES
        CXX_STANDARD 17
        C_STANDARD 99
    )

    # MSVC-specific compile flags
    if(MSVC)
        target_compile_options(${name} PRIVATE
            $<$<CONFIG:Release>:/O2 /W3>
            $<$<CONFIG:Debug>:/W3>  # Avoid /O2 in Debug to prevent conflict with /RTC1
        )
    endif()
endfunction()

# Add all renderers
foreach(renderer ${RENDERERS})
    add_renderer_target(${renderer})
endforeach()
