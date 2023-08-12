cmake_minimum_required(VERSION 3.5)
project(Loz VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# include(CTest)
# enable_testing()

# set(SOURCE main.cpp)
include_directories(${PROJECT_SOURCE_DIR}/Game)
file(GLOB ALL_SOURCES "${PROJECT_SOURCE_DIR}/Game/Loz/*.cpp")
message("ALL_SOURCES = ${ALL_SOURCES}")
add_executable(Loz ${ALL_SOURCES})

INCLUDE_DIRECTORIES(/opt/homebrew/Cellar/allegro/5.2.8.0_2/include)
LINK_DIRECTORIES(/opt/homebrew/Cellar/allegro/5.2.8.0_2/lib)

file(GLOB LIBRARIES "/opt/homebrew/Cellar/allegro/5.2.8.0_2/lib/*.dylib")
# message("LIBRARIES = ${LIBRARIES}")

TARGET_LINK_LIBRARIES(Loz  ${LIBRARIES})
# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)
