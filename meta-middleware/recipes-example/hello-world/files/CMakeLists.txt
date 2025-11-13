cmake_minimum_required(VERSION 3.10)
project(hello-world C)

# Set C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Create executable
add_executable(hello-world hello.c)

# Install target
install(TARGETS hello-world DESTINATION bin)
