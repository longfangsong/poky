cmake_minimum_required(VERSION 3.10)
project(hello-world CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find libcurl
find_package(CURL REQUIRED)

# Find nlohmann_json (header-only JSON library)
find_package(nlohmann_json REQUIRED)

# Create executable
add_executable(hello-world hello.cpp)

# Link libcurl
target_link_libraries(hello-world PRIVATE CURL::libcurl nlohmann_json::nlohmann_json)

# Install target
install(TARGETS hello-world DESTINATION bin)
