cmake_minimum_required(VERSION 3.10)
project(gitz)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenSSL REQUIRED)

# Source files
set(SOURCES
    src/main.cpp
    src/repository.cpp
    src/object.cpp
    src/index.cpp
    src/commands.cpp
)

add_executable(gitz ${SOURCES})

target_include_directories(gitz PRIVATE src)

target_link_libraries(gitz OpenSSL::SSL OpenSSL::Crypto)

install(TARGETS gitz DESTINATION bin)
