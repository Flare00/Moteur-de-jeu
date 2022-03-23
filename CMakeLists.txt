# CMake entry point
cmake_minimum_required (VERSION 3.0)
project (Tutorials)

find_package(OpenGL REQUIRED)


if( CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR )
    message( FATAL_ERROR "Please select another Build Directory ! (and give it a clever name, like bin_Visual2012_64bits/)" )
endif()
if( CMAKE_SOURCE_DIR MATCHES " " )
	message( "Your Source Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()
if( CMAKE_BINARY_DIR MATCHES " " )
	message( "Your Build Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()



# Compile external dependencies 
add_subdirectory (external)


# On Visual 2005 and above, this module can set the debug working directory
cmake_policy(SET CMP0026 OLD)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/external/rpavlik-cmake-modules-fe2273")
include(CreateLaunchers)
include(MSVCMultipleProcessCompile) # /MP

if(INCLUDE_DISTRIB)
	add_subdirectory(distrib)
endif(INCLUDE_DISTRIB)



include_directories(
	external/glfw-3.1.2/include/
	external/glm-0.9.7.1/
	external/glew-1.13.0/include/
	external/stb
	moteur/metier
	.
)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	glfw
	GLEW_1130
)

add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)

# Moteur
add_executable(moteur
        moteur/Moteur.cpp
	common/shader.cpp
	common/shader.hpp
	common/controls.cpp
	common/controls.hpp
	common/texture.cpp
	common/texture.hpp
	common/objloader.cpp
	common/objloader.hpp
	common/vboindexer.cpp
	common/vboindexer.hpp
        moteur/Shaders/vertex_shader.glsl
		moteur/Shaders/fragment_shader.glsl
		moteur/Shaders/terrain_vertex_shader.glsl
		moteur/Shaders/terrain_fragment_shader.glsl
  "moteur/metier/Game/Scene.hpp" "moteur/metier/Game/Game.hpp"  "moteur/metier/Game/SceneSpace.hpp" "moteur/metier/Tools/PrimitiveMesh.hpp" "moteur/metier/Light/Material.hpp" "moteur/metier/Game/SceneCollision.hpp" "moteur/metier/Collision/Collision.hpp"  "moteur/metier/GameObject/ModeleLOD.hpp" "moteur/metier/Component.hpp" "moteur/metier/Collision/Physique.hpp" "moteur/metier/Collision/Gravity.hpp")
target_link_libraries(moteur
	${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(moteur PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/moteur/")
create_target_launcher(moteur WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/moteur/")

SOURCE_GROUP(common REGULAR_EXPRESSION ".*/common/.*" )
SOURCE_GROUP(shaders REGULAR_EXPRESSION ".*Shaders/.*shader$" )

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/moteur/Shaders/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Shaders/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/moteur/Textures/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Textures/)

if (NOT ${CMAKE_GENERATOR} MATCHES "Xcode" )
add_custom_command(
   TARGET moteur POST_BUILD


   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/moteur${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/moteur/"
)
elseif (${CMAKE_GENERATOR} MATCHES "Xcode" )

endif (NOT ${CMAKE_GENERATOR} MATCHES "Xcode" )

