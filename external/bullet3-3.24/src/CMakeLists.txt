
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