INCLUDE_DIRECTORIES(${BULLET_PHYSICS_SOURCE_DIR}/include/)

SET(Bullet3Geometry_SRCS
	b3ConvexHullComputer.cpp
	b3GeometryUtil.cpp
)

SET(Bullet3Geometry_HDRS
	b3AabbUtil.h
	b3ConvexHullComputer.h
	b3GeometryUtil.h
	b3GrahamScan2dConvexHull.h
)

ADD_LIBRARY(Bullet3Geometry STATIC ${Bullet3Geometry_SRCS} ${Bullet3Geometry_HDRS})
target_link_libraries(Bullet3Geometry Bullet3Common)