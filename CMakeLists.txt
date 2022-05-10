# CMake entry point
cmake_minimum_required (VERSION 3.0)
set(GLFW_BUILD_EXAMPLES 0)
project (Moteur)
find_package(OpenGL REQUIRED)

# Compile external dependencies 
add_subdirectory(external)

# On Visual 2005 and above, this module can set the debug working directory
cmake_policy(SET CMP0026 OLD)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/external/rpavlik-cmake-modules-fe2273")
include(CreateLaunchers)
include(MSVCMultipleProcessCompile) # /MP

if(INCLUDE_DISTRIB)
	add_subdirectory(distrib)
endif(INCLUDE_DISTRIB)



include_directories(
	external/glfw-3.3.7/include/GLFW/
	external/glew-2.1.0/include/
	external/glm-0.9.8.5/
	external/bullet3-3.24/include/
	external/stb/
	common/
	moteur/metier/
)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	glfw
	GLEW_210
	BULLET3_324
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
		 common/objloader.hpp
		common/objloader.cpp
        moteur/Moteur.cpp "moteur/metier/Physique/DebugDrawer.hpp"  "moteur/metier/Physique/GestionContraintes.hpp" "moteur/metier/Physique/BulletRigidbody.hpp"   "moteur/metier/Transformation/ITransformation.hpp" "moteur/metier/Light/Lightning.hpp" "moteur/metier/Light/ILight.hpp" "moteur/metier/Light/DirectionnalLight.hpp" "moteur/metier/Light/ShadowMap.hpp"  "moteur/metier/World/CameraData.hpp")


target_link_libraries(moteur
	${ALL_LIBS} 
)
# Xcode and Visual working directories
set_target_properties(moteur PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/moteur/")
create_target_launcher(moteur WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/moteur/")

SOURCE_GROUP(common REGULAR_EXPRESSION ".*/common/.*" )
SOURCE_GROUP(shaders REGULAR_EXPRESSION ".*Shaders/.*glsl$" )

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/moteur/Shaders/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Shaders/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/moteur/Textures/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Textures/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/moteur/Model/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Model/)

if (NOT ${CMAKE_GENERATOR} MATCHES "Xcode" )
add_custom_command(
   TARGET moteur POST_BUILD


   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/moteur${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/moteur/"
)
elseif (${CMAKE_GENERATOR} MATCHES "Xcode" )

endif (NOT ${CMAKE_GENERATOR} MATCHES "Xcode" )

