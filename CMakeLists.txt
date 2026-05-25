cmake_minimum_required(VERSION 3.16)

project(PassGen LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SODIUM REQUIRED libsodium)

add_executable(passgen
    src/main.cpp
)

target_include_directories(passgen PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/incluse
    ${SODIUM_INCLUDE_DIRS}
)

target_link_directories(passgen PRIVATE ${SODIUM_LIBRARY_DIRS})
target_link_libraries(passgen PRIVATE ${SODIUM_LIBRARIES})
target_compile_options(passgen PRIVATE ${SODIUM_CFLAGS_OTHER})
