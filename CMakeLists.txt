cmake_minimum_required(VERSION 3.10)
project(gitz)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenSSL REQUIRED)

# Source files
set(SOURCES
    /src/repository.cpp
    /src/index.cpp
    /src/object_utils.cpp
    /src/main.cpp
    /src/tree.cpp
    /src/commit.cpp
    /src/blob.cpp
    /src/commands.cpp
)

add_executable(gitz ${SOURCES})

target_include_directories(gitz PRIVATE src)

target_link_libraries(gitz OpenSSL::SSL OpenSSL::Crypto)

install(TARGETS gitz DESTINATION bin)
