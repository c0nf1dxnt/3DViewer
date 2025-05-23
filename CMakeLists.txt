cmake_minimum_required(VERSION 3.10)
project(3DViewer VERSION 2.0 LANGUAGES CXX)

if(POLICY CMP0072)
  cmake_policy(SET CMP0072 NEW)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(APPLE)
  execute_process(
    COMMAND brew --prefix qt@5
    OUTPUT_VARIABLE QTDIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
  if(QTDIR)
    list(APPEND CMAKE_PREFIX_PATH ${QTDIR})
  endif()
endif()

if(APPLE)
  add_definitions(-DGL_SILENCE_DEPRECATION)
endif()

find_package(Qt5 COMPONENTS Widgets Core Gui OpenGL REQUIRED)

find_package(OpenGL REQUIRED)
if(UNIX AND NOT APPLE)
    find_package(GLUT REQUIRED)
endif()

file(GLOB_RECURSE SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

file(GLOB_RECURSE HEADERS 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
)

add_executable(viewer ${SOURCES} ${HEADERS})

target_include_directories(viewer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(viewer PRIVATE
    Qt5::Widgets
    Qt5::Core
    Qt5::Gui
    Qt5::OpenGL
    ${OPENGL_LIBRARIES}
)

install(TARGETS viewer
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

if(UNIX AND NOT APPLE)
    target_link_libraries(viewer PRIVATE ${GLUT_LIBRARIES})
endif()
