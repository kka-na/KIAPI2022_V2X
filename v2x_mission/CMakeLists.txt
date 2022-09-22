cmake_minimum_required(VERSION 3.0.2)
project(v2x_mission LANGUAGES CXX)

find_package(catkin REQUIRED COMPONENTS roscpp std_msgs geometry_msgs genmsg)

generate_messages(DEPENDENCIES std_msgs geometry_msgs)

catkin_package(
    INCLUDE_DIRS include header
)

include_directories(
    include
    header
    ${catkin_INCLUDE_DIRS}
)

set(
    SOURCES ./src/main.cpp ./src/setSocket.cpp ./src/setStage1.cpp ./src/setStage2.cpp
)
add_executable(
    v2x_mission ${SOURCES}
)

set_source_files_properties( ${SOURCES} PROPERTIES LANGUAGE CXX )
set_target_properties(v2x_mission PROPERTIES OUTPUT_NAME v2x_mission PREFIX "")

target_link_libraries(
    v2x_mission ${catkin_LIBRARIES}
)

