cmake_minimum_required(VERSION 2.8)

project(assigment)


add_executable(Skybox.out Skybox.cpp)
add_executable(Cannon.out Cannon.cpp)


find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )


target_link_libraries( Skybox.out ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )
target_link_libraries( Cannon.out ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )

