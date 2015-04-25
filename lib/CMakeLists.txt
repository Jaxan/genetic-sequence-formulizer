
file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")
add_library(common ${headers} ${sources})
target_link_libraries(common ${libs})
target_include_directories(common INTERFACE ".")
