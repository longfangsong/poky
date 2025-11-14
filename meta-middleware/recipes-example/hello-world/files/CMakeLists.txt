cmake_minimum_required(VERSION 3.10)
project(hello-world CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Create executable
add_executable(hello-world hello.cpp)

# Install target
install(TARGETS hello-world DESTINATION bin)
