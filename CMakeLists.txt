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
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${SODIUM_INCLUDE_DIRS}
)

target_link_directories(passgen PRIVATE ${SODIUM_LIBRARY_DIRS})
target_link_libraries(passgen PRIVATE ${SODIUM_LIBRARIES})
target_compile_options(passgen PRIVATE ${SODIUM_CFLAGS_OTHER})

# Определение пути к алфавитам при установке
set(PASSGEN_ALPHABETS_DIR ${CMAKE_INSTALL_PREFIX}/share/passgen/alphabets)
target_compile_definitions(passgen PRIVATE
    PASSGEN_ALPHABETS_DIR="${PASSGEN_ALPHABETS_DIR}"
)

# Установка исполняемого файла
install(TARGETS passgen
    RUNTIME DESTINATION bin
)

# Установка файлов алфавитов
install(DIRECTORY alphabet/
    DESTINATION share/passgen/alphabets
    FILES_MATCHING PATTERN "*.txt"
)
