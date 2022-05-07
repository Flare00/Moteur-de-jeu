INCLUDE_DIRECTORIES(${BULLET_PHYSICS_SOURCE_DIR}/include/)

SET(Bullet3Common_SRCS
	b3AlignedAllocator.cpp
	b3Vector3.cpp
	b3Logging.cpp
)

SET(Bullet3Common_HDRS
	b3AlignedAllocator.h
	b3AlignedObjectArray.h
	b3CommandLineArgs.h
	b3HashMap.h
	b3Logging.h
	b3Matrix3x3.h
	b3MinMax.h
	b3PoolAllocator.h
	b3QuadWord.h
	b3Quaternion.h
	b3Random.h
	b3Scalar.h
	b3StackAlloc.h
	b3Transform.h
	b3TransformUtil.h
	b3Vector3.h
	shared/b3Float4.h
	shared/b3Int2.h
	shared/b3Int4.h
	shared/b3Mat3x3.h
	shared/b3PlatformDefinitions.h
	shared/b3Quat.h
)

ADD_LIBRARY(Bullet3Common STATIC ${Bullet3Common_SRCS} ${Bullet3Common_HDRS})