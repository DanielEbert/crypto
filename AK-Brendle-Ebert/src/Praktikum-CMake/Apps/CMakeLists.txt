# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# + CMakeLists.txt
# + Folder: src/Praktikum-CMake/Apps
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

add_executable(helloworld helloworld-main )
add_executable(hellostudent hellostudent-main Greetings)

include_directories($(CMAKE_CURRENT_SOURCE_DIR)/../Lib)
add_executable(calculate calculate)
target_link_libraries(calculate functions)