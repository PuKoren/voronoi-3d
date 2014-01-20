cmake_minimum_required (VERSION 2.6)
project (voronoi-3d) 
list( APPEND CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS} -g -ftest-coverage -fprofile-arcs")
#########################################################
# FIND GLEW
#########################################################
find_package (GLEW REQUIRED)
include_directories (${GLEW_INCLUDE_DIRS})
link_directories (${GLEW_LIBRARY_DIRS})
if (NOT GLEW_FOUND)
    message (ERROR "GLEW not found!")
endif (NOT GLEW_FOUND)

#########################################################
# FIND GLUT
#########################################################
find_package (GLUT REQUIRED)
include_directories (${GLUT_INCLUDE_DIRS})
link_directories (${GLUT_LIBRARY_DIRS})
if (NOT GLUT_FOUND)
    message (ERROR "GLUT not found!")
endif (NOT GLUT_FOUND)
#########################################################
# FIND OPENGL
#########################################################
find_package (OpenGL REQUIRED)
include_directories (${OpenGL_INCLUDE_DIRS})
link_directories (${OpenGL_LIBRARY_DIRS})
if (NOT OPENGL_FOUND)
    message (ERROR "OPENGL not found!")
endif (NOT OPENGL_FOUND)

file(GLOB SRCS src/*.cpp)
file(GLOB HDRS include/*.h)

include_directories("include")

add_executable (${PROJECT_NAME} ${SRCS} ${HDRS} ${ESGIGL_HDRS} ${ESGIGL_SRCS} ${ESGIGL_COMMON})

########################################################
# Linking & stuff
#########################################################
target_link_libraries (${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLEW_LIBRARY} ${GLUT_LIBRARY})
