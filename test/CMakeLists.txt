cmake_minimum_required(VERSION 3.10)
project(3DViewerTests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

enable_testing()

set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/../src)
include_directories(${SOURCE_DIR})

file(GLOB PROJECT_SOURCES 
    "${SOURCE_DIR}/model/*.cpp"
    "${SOURCE_DIR}/controller/*.cpp"
)

file(GLOB TEST_SOURCES "*.cpp")

add_executable(3DViewerTests ${TEST_SOURCES} ${PROJECT_SOURCES})

target_link_libraries(3DViewerTests
  gtest_main
  gmock_main
)

include(GoogleTest)
gtest_discover_tests(3DViewerTests) 