cmake_minimum_required(VERSION 3.10)
project(communicate CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find libcurl
find_package(CURL REQUIRED)

# Find nlohmann_json (header-only JSON library)
find_package(nlohmann_json REQUIRED)

# Create executable
add_executable(communicate hello.cpp)

# Link libcurl
target_link_libraries(communicate PRIVATE CURL::libcurl nlohmann_json::nlohmann_json)

# Install target
install(TARGETS communicate DESTINATION bin)
