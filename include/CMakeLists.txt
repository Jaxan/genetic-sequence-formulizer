
# Hack to show headers in Qt Creator
file(GLOB headers "*.hpp")
add_library(dummy ${headers} dummy.cpp)
