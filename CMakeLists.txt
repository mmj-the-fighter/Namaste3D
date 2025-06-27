# set project
cmake_minimum_required(VERSION 3.14)
project(Namaste3DSolution)
set(POINTS_RENDERER points_renderer)
set(WIREFRAME_RENDERER wireframe_renderer)
set(SOLIDCOLOR_RENDERER solid_color_renderer)
set(COLOR_INTERPOLATION color_interpolation)
set(LINEAR_TEXTURING linear_texturing)
set(PERSPECTIVE_CORRECT_TEXTURING perspective_correct_texturing)
set(DEPTH_BUFFERING depth_buffering)
set(LIGHTING_AND_SHADING lighting_and_shading)
set(SCENE_PARSING scene_parsing)
#---------------------------------------------
# set standards
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD_REQUIRED ON)
#---------------------------------------------
# SDL3 
# fetch SDL3 from internet
include(FetchContent)

FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG        release-3.2.0
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(SDL3)
#---------------------------------------------
# Spinach
# Collect C++ source files for spinach
file(GLOB_RECURSE SPN_CPP_SRC CONFIGURE_DEPENDS
    spinach/common/*.cpp
    spinach/core/*.cpp
)


# Collect C sources for spinach
file(GLOB_RECURSE SPN_C_SRC CONFIGURE_DEPENDS
    spinach/external/stb/*.c
)

# Collect headers 
file(GLOB_RECURSE SPN_H CONFIGURE_DEPENDS
    spinach/common/*.h
    spinach/common/*.hpp
    spinach/core/*.h
    spinach/core/*.hpp
    spinach/external/stb/*.h
)
# ------------------------------------
# 1. POINTS_RENDERER
# Collect C++ source files
file(GLOB_RECURSE PTR_CPP_SRC CONFIGURE_DEPENDS
    points_rendering/*.cpp
	points_rendering/demo/*.cpp
    points_rendering/n3d/*.cpp
)

# Collect headers 
file(GLOB_RECURSE PTR_CPP_H CONFIGURE_DEPENDS
	points_rendering/demo/*.h
    points_rendering/n3d/*.h
	points_rendering/n3d/*.hpp
)

# Create the executable
add_executable(	${POINTS_RENDERER}
	${PTR_CPP_SRC}
	${SPN_CPP_SRC}
    ${SPN_C_SRC}
    ${PTR_CPP_H} 
	${SPN_H}
)

target_include_directories(${POINTS_RENDERER} PRIVATE
    spinach/common
    spinach/core
    spinach/external/stb
	points_rendering/demo/
	points_rendering/n3d/
)


target_link_libraries(${POINTS_RENDERER} PRIVATE SDL3::SDL3)

# ------------------------------------------

# ------------------------------------
# 2. WIREFRAME_RENDERER
# Collect C++ source files
file(GLOB_RECURSE WIRE_CPP_SRC CONFIGURE_DEPENDS
    wireframe_rendering/*.cpp
	wireframe_rendering/demo/*.cpp
    wireframe_rendering/n3d/*.cpp
)

# Collect headers 
file(GLOB_RECURSE WIRE_CPP_H CONFIGURE_DEPENDS
	wireframe_rendering/demo/*.h
    wireframe_rendering/n3d/*.h
	wireframe_rendering/n3d/*.hpp
)

# Create the executable
add_executable(	${WIREFRAME_RENDERER}
	${WIRE_CPP_SRC}
	${SPN_CPP_SRC}
    ${SPN_C_SRC}
    ${WIRE_CPP_H} 
	${SPN_H}
)

target_include_directories(${WIREFRAME_RENDERER} PRIVATE
    spinach/common
    spinach/core
    spinach/external/stb
	wireframe_rendering/demo/
	wireframe_rendering/n3d/
)


target_link_libraries(${WIREFRAME_RENDERER} PRIVATE SDL3::SDL3)

# ------------------------------------
# 3. SOLIDCOLOR_RENDERER
# Collect C++ source files
file(GLOB_RECURSE SLD_CPP_SRC CONFIGURE_DEPENDS
    solid_color_rendering/*.cpp
	solid_color_rendering/demo/*.cpp
    solid_color_rendering/n3d/*.cpp
)

# Collect headers 
file(GLOB_RECURSE SLD_CPP_H CONFIGURE_DEPENDS
	solid_color_rendering/demo/*.h
    solid_color_rendering/n3d/*.h
	solid_color_rendering/n3d/*.hpp
)

# Create the executable
add_executable(	${SOLIDCOLOR_RENDERER}
	${SLD_CPP_SRC}
	${SPN_CPP_SRC}
    ${SPN_C_SRC}
    ${SLD_CPP_H} 
	${SPN_H}
)

target_include_directories(${SOLIDCOLOR_RENDERER} PRIVATE
    spinach/common
    spinach/core
    spinach/external/stb
	solid_color_rendering/demo/
	solid_color_rendering/n3d/
)


target_link_libraries(${SOLIDCOLOR_RENDERER} PRIVATE SDL3::SDL3)

# ------------------------------------
# 4. COLOR_INTERPOLATION
# Collect C++ source files
file(GLOB_RECURSE INTERP_CPP_SRC CONFIGURE_DEPENDS
    color_interpolation/*.cpp
	color_interpolation/demo/*.cpp
    color_interpolation/n3d/*.cpp
)

# Collect headers 
file(GLOB_RECURSE INTERP_CPP_H CONFIGURE_DEPENDS
	color_interpolation/demo/*.h
    color_interpolation/n3d/*.h
	color_interpolation/n3d/*.hpp
)

# Create the executable
add_executable(	${COLOR_INTERPOLATION}
	${INTERP_CPP_SRC}
	${SPN_CPP_SRC}
    ${SPN_C_SRC}
    ${INTERP_CPP_H} 
	${SPN_H}
)

target_include_directories(${COLOR_INTERPOLATION} PRIVATE
    spinach/common
    spinach/core
    spinach/external/stb
	color_interpolation/demo/
	color_interpolation/n3d/
)


target_link_libraries(${COLOR_INTERPOLATION} PRIVATE SDL3::SDL3)

# ------------------------------------
# 5. LINEAR_TEXTURING
# Collect C++ source files
file(GLOB_RECURSE LIN_CPP_SRC CONFIGURE_DEPENDS
    linear_texturing/*.cpp
	linear_texturing/demo/*.cpp
    linear_texturing/n3d/*.cpp
)

# Collect headers 
file(GLOB_RECURSE LIN_CPP_H CONFIGURE_DEPENDS
	linear_texturing/demo/*.h
    linear_texturing/n3d/*.h
	linear_texturing/n3d/*.hpp
)

# Create the executable
add_executable(	${LINEAR_TEXTURING}
	${LIN_CPP_SRC}
	${SPN_CPP_SRC}
    ${SPN_C_SRC}
    ${LIN_CPP_H} 
	${SPN_H}
)

target_include_directories(${LINEAR_TEXTURING} PRIVATE
    spinach/common
    spinach/core
    spinach/external/stb
	linear_texturing/demo/
	linear_texturing/n3d/
)


target_link_libraries(${LINEAR_TEXTURING} PRIVATE SDL3::SDL3)

# ------------------------------------------
# ------------------------------------
# 6. PERSPECTIVE_CORRECT_TEXTURING
# Collect C++ source files
file(GLOB_RECURSE PERSPECT_CPP_SRC CONFIGURE_DEPENDS
    perspective_correct_texturing/*.cpp
	perspective_correct_texturing/demo/*.cpp
    perspective_correct_texturing/n3d/*.cpp
)

# Collect headers 
file(GLOB_RECURSE PERSPECT_CPP_H CONFIGURE_DEPENDS
	perspective_correct_texturing/demo/*.h
    perspective_correct_texturing/n3d/*.h
	perspective_correct_texturing/n3d/*.hpp
)

# Create the executable
add_executable(	${PERSPECTIVE_CORRECT_TEXTURING}
	${PERSPECT_CPP_SRC}
	${SPN_CPP_SRC}
    ${SPN_C_SRC}
    ${PERSPECT_CPP_H} 
	${SPN_H}
)

target_include_directories(${PERSPECTIVE_CORRECT_TEXTURING} PRIVATE
    spinach/common
    spinach/core
    spinach/external/stb
	perspective_correct_texturing/demo/
	perspective_correct_texturing/n3d/
)


target_link_libraries(${PERSPECTIVE_CORRECT_TEXTURING} PRIVATE SDL3::SDL3)

# ------------------------------------------

# ------------------------------------
# 7. DEPTH_BUFFERING
# Collect C++ source files
file(GLOB_RECURSE DEPTH_CPP_SRC CONFIGURE_DEPENDS
    depth_buffering/*.cpp
	depth_buffering/demo/*.cpp
    depth_buffering/n3d/*.cpp
)

# Collect headers 
file(GLOB_RECURSE DEPTH_CPP_H CONFIGURE_DEPENDS
	depth_buffering/demo/*.h
    depth_buffering/n3d/*.h
	depth_buffering/n3d/*.hpp
)

# Create the executable
add_executable(	${DEPTH_BUFFERING}
	${DEPTH_CPP_SRC}
	${SPN_CPP_SRC}
    ${SPN_C_SRC}
    ${DEPTH_CPP_H} 
	${SPN_H}
)

target_include_directories(${DEPTH_BUFFERING} PRIVATE
    spinach/common
    spinach/core
    spinach/external/stb
	depth_buffering/demo/
	depth_buffering/n3d/
)


target_link_libraries(${DEPTH_BUFFERING} PRIVATE SDL3::SDL3)

# ------------------------------------------

# ------------------------------------

# 8. LIGHTING_AND_SHADING
# Collect C++ source files
file(GLOB_RECURSE LIGHTSHADE_CPP_SRC CONFIGURE_DEPENDS
    lighting_and_shading/*.cpp
	lighting_and_shading/demo/*.cpp
    lighting_and_shading/n3d/*.cpp
)

# Collect headers 
file(GLOB_RECURSE LIGHTSHADE_CPP_H CONFIGURE_DEPENDS
	lighting_and_shading/demo/*.h
    lighting_and_shading/n3d/*.h
	lighting_and_shading/n3d/*.hpp
)

# Create the executable
add_executable(	${LIGHTING_AND_SHADING}
	${LIGHTSHADE_CPP_SRC}
	${SPN_CPP_SRC}
    ${SPN_C_SRC}
    ${LIGHTSHADE_CPP_H} 
	${SPN_H}
)

target_include_directories(${LIGHTING_AND_SHADING} PRIVATE
    spinach/common
    spinach/core
    spinach/external/stb
	lighting_and_shading/demo/
	lighting_and_shading/n3d/
)


target_link_libraries(${LIGHTING_AND_SHADING} PRIVATE SDL3::SDL3)
# ------------------------------------------


# ------------------------------------

# 8. SCENE_PARSING
# Collect C++ source files
file(GLOB_RECURSE SCENE_PARSING_CPP_SRC CONFIGURE_DEPENDS
    scene_parsing/*.cpp
	scene_parsing/demo/*.cpp
    scene_parsing/n3d/*.cpp
)

# Collect headers 
file(GLOB_RECURSE SCENE_PARSING_CPP_H CONFIGURE_DEPENDS
	scene_parsing/demo/*.h
    scene_parsing/n3d/*.h
	scene_parsing/n3d/*.hpp
)

# Create the executable
add_executable(	${SCENE_PARSING}
	${SCENE_PARSING_CPP_SRC}
	${SPN_CPP_SRC}
    ${SPN_C_SRC}
    ${SCENE_PARSING_CPP_H} 
	${SPN_H}
)

target_include_directories(${SCENE_PARSING} PRIVATE
    spinach/common
    spinach/core
    spinach/external/stb
	scene_parsing/demo/
	scene_parsing/n3d/
)


target_link_libraries(${SCENE_PARSING} PRIVATE SDL3::SDL3)
# ------------------------------------------