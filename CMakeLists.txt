cmake_minimum_required(VERSION 3.0.0)

project(tower_sim VERSION 0.1.0)
set(CMAKE_VERBOSE_MAKEFILE ON)

add_executable(tower
	src/GL/displayable.hpp
	src/GL/dynamic_object.hpp
	src/GL/opengl_interface.cpp
	src/GL/opengl_interface.hpp
	src/GL/texture.hpp
	src/img/image.cpp
	src/img/image.hpp
	src/img/media_path.hpp
	src/img/stb_image.h
    src/aircraft_types.hpp
    src/aircraft.cpp
    src/aircraft.hpp
    src/airport_type.hpp
	src/airport.hpp
	src/config.hpp
	src/geometry.hpp
	src/runway.hpp
	src/terminal.hpp
	src/tower_sim.cpp
	src/tower_sim.hpp
	src/tower.cpp
	src/tower.hpp
	src/waypoint.hpp
	src/main.cpp
)

###################
# Compile options #
###################

target_compile_features(tower PRIVATE cxx_std_17)

if(MSVC)
  target_compile_options(tower PRIVATE /W4 /WX)
else()
  target_compile_options(tower PRIVATE -Wall -Wextra -Werror -Wshadow -Wno-deprecated)
endif()


################
# Dependencies #
################

## GLUT / FreeGLUT
find_package(FreeGLUT CONFIG)

if(FreeGLUT_FOUND)
	message("Library FreeGLUT found.")
	target_include_directories(tower SYSTEM PRIVATE FreeGLUT::freeglut)
	target_link_libraries(tower PRIVATE FreeGLUT::freeglut)
else()
	find_package(GLUT REQUIRED)
	message("Library GLUT found.")
	target_include_directories(tower SYSTEM PRIVATE ${GLUT_INCLUDE_DIR})
	target_link_libraries(tower PRIVATE ${GLUT_LIBRARIES})
endif()

target_compile_definitions(tower PRIVATE GLUT_DISABLE_ATEXIT_HACK)


## OpenGL
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
target_include_directories(tower SYSTEM PRIVATE ${OPENGL_INCLUDE_DIR})
target_link_libraries(tower	PRIVATE ${OPENGL_LIBRARIES})


## FileSystem
if((${CMAKE_CXX_COMPILER_ID} MATCHES "GNU") AND (${CMAKE_CXX_COMPILER_VERSION} LESS 9))
	target_link_libraries(tower	PRIVATE stdc++fs)
endif()


##########
# Assets #
##########

add_custom_command(TARGET tower POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/media"
        "$<TARGET_FILE_DIR:tower>/media")


########
# DLLs #
########

if (WIN32)
    add_custom_command(TARGET tower POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:FreeGLUT::freeglut>
            "$<TARGET_FILE_DIR:tower>/")
endif()
