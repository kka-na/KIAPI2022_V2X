cmake_minimum_required(VERSION 3.0.2)
project(v2x_parser LANGUAGES CXX)

find_package(catkin REQUIRED COMPONENTS roscpp std_msgs geometry_msgs genmsg)
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

add_executable(
    v2x_parser ./src/sample.c ./src/j2735.c  ./src/bsm.c ./src/pvd.c
)

set_source_files_properties( ./src/sample.c PROPERTIES LANGUAGE CXX )
set_source_files_properties( ./src/pvd.c PROPERTIES LANGUAGE CXX )
set_source_files_properties( ./src/bsm.c PROPERTIES LANGUAGE CXX )
set_source_files_properties( ./src/j2735.c PROPERTIES LANGUAGE CXX )

set_target_properties(v2x_parser PROPERTIES OUTPUT_NAME v2x_parser PREFIX "")

target_link_libraries(
    v2x_parser ${catkin_LIBRARIES} asncodec json-c
)