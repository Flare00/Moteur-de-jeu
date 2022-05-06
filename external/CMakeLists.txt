
add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)

### GLFW ###
add_subdirectory (glfw-3.3.7)

include_directories(
	glfw-3.3.7/include/GLFW/
	glew-2.1.0/include/
)

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
set(OPENGL_LIBRARY
	${OPENGL_LIBRARY}
	-lGL -lGLU -lXrandr -lXext -lX11 -lrt
	${CMAKE_DL_LIBS}
	${GLFW_LIBRARIES}
)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
set(OPENGL_LIBRARY
	${OPENGL_LIBRARY}
	${CMAKE_DL_LIBS}
	${GLFW_LIBRARIES}
)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

### GLEW ###

set(GLEW_SOURCE
	glew-2.1.0/src/glew.c
)

set(GLEW_HEADERS
)

add_library( GLEW_210 STATIC
	${GLEW_SOURCE}
	${GLEW_INCLUDE}
)

target_link_libraries(GLEW_210
	${OPENGL_LIBRARY}
	${EXTRA_LIBS}
)

### BULLET ###

add_subdirectory(bullet3-3.24/src/LinearMath)
add_subdirectory(bullet3-3.24/src/Bullet3Common)
add_subdirectory(bullet3-3.24/src/BulletCollision)
add_subdirectory(bullet3-3.24/src/BulletDynamics)
add_subdirectory(bullet3-3.24/src/BulletSoftBody)
add_subdirectory(bullet3-3.24/src/BulletInverseDynamics)

set(BULLET_SRC
	bullet3-3.24/src/btBulletCollisionAll.cpp
	bullet3-3.24/src/btBulletCollisionCommon.h
	bullet3-3.24/src/btBulletDynamicsAll.cpp
	bullet3-3.24/src/btBulletDynamicsCommon.h
	bullet3-3.24/src/btLinearMathAll.cpp
)


add_library( BULLET3_324 STATIC 
	${BULLET_SRC}
)
target_link_libraries(BULLET3_324 LinearMath Bullet3Common BulletCollision BulletDynamics BulletSoftBody BulletInverseDynamics)