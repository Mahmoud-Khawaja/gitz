cmake_minimum_required(VERSION 3.10)
project(gitz)

# doing it with c++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# find OpenSSL for SHA-1 hashing
find_package(OpenSSL REQUIRED)

# creating executable
add_executable(gitz main.cpp repository.cpp object.cpp index.cpp commands.cpp)

# openSSL libraries
target_link_libraries(gitz OpenSSL::SSL OpenSSL::Crypto)

# set up installation in this location /usr/local/bin
install(TARGETS gitz DESTINATION bin)