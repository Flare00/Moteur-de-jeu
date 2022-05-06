
add_subdirectory(Bullet3Common)

IF(BUILD_BULLET3)
	add_subdirectory(Bullet3Geometry)
	add_subdirectory(Bullet3Collision)
	add_subdirectory(Bullet3Dynamics)
	add_subdirectory(Bullet3Serialize/Bullet2FileLoader)
	add_subdirectory(Bullet3OpenCL)
ENDIF(BUILD_BULLET3)



add_subdirectory(LinearMath)
add_subdirectory(BulletCollision)
add_subdirectory(BulletDynamics)
add_subdirectory(BulletSoftBody)
add_subdirectory(BulletInverseDynamics)


IF(INSTALL_LIBS)
	#INSTALL of other files requires CMake 2.6
	IF (${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} GREATER 2.5)
		IF (APPLE AND BUILD_SHARED_LIBS AND FRAMEWORK)
			# Don't actually need to install any common files, the frameworks include everything
		ELSE (APPLE AND BUILD_SHARED_LIBS AND FRAMEWORK)
			INSTALL(FILES btBulletCollisionCommon.h btBulletDynamicsCommon.h  DESTINATION ${INCLUDE_INSTALL_DIR})
		ENDIF (APPLE AND BUILD_SHARED_LIBS AND FRAMEWORK)
	ENDIF (${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} GREATER 2.5)
ENDIF(INSTALL_LIBS)
