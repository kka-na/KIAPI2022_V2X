cmake_minimum_required(VERSION 3.0.2)
project(v2x_parser LANGUAGES CXX)

find_package(catkin REQUIRED COMPONENTS roscpp std_msgs sbg_driver genmsg)
find_package(json-c CONFIG)

catkin_package(
    INCLUDE_DIRS include include/j2735
    LIBRARIES include/j2735 
)

include_directories(
    include
    ${catkin_INCLUDE_DIRS}
    include/j2735
)

link_directories(
    include/j2735  
) 

set(
    SOURCES ./src/sample.c ./src/j2735.c  ./src/bsm.c ./src/pvd.c
)

add_executable(
    v2x_parser ${SOURCES}
)

set_source_files_properties( ${SOURCES} PROPERTIES LANGUAGE CXX )

target_link_libraries(
    v2x_parser ${catkin_LIBRARIES} asncodec json-c
)


# set_target_properties(v2x_parser PROPERTIES OUTPUT_NAME v2x_parser PREFIX "")
