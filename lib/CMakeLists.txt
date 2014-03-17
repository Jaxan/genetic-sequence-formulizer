
file(GLOB sources "*.cpp")
add_library(common STATIC ${sources})
target_link_libraries(common ${libs})
