cmake_minimum_required(VERSION 2.8)

project(assigment)


add_executable(assignment1.out assignment1.cpp)


find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )


target_link_libraries( assignment1.out ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )
