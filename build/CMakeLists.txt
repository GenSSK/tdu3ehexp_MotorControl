cmake_minimum_required(VERSION 3.10)
project(MotorControl)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(GLUT REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})

if(GLUT_FOUND)
    message(STATUS "GLUT_INCLUDE_DIR: ${GLUT_INCLUDE_DIR}")
    message(STATUS "GLUT_LIBRARIES: ${GLUT_LIBRARIES}")
    message(STATUS "GLUT_VERSION: ${GLUT_VERSION}")
endif()

INCLUDE_DIRECTORIES(MotorControl
        ../UDP
        ../Plot
)

add_executable(MotorControl
        ../main.cpp
        ../myconfig.h
        ../myfunc.h
        ../UDP/udpReceive.h
        ../UDP/udpReceive.cpp
        ../UDP/udpSend.h
        ../UDP/udpSend.cpp
        ../UDP/dataStruct.h
        ../Plot/paint_class.cpp
        ../Plot/paint_class.h
        ../Plot/paint_class_bc.cpp
        ../Plot/paint_class_event.cpp
        ../Plot/paint_conf.cpp
        ../Plot/paint_conf.h
        )

target_link_libraries(MotorControl
        pthread
        OpenGL::GL
        GLUT::GLUT
        ${OPENGL_LIBRARIES}
        ${GLUT_LIBRARY}
)