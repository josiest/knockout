cmake_minimum_required(VERSION 3.10)

project(knockout VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# project structure
set(PROGRAM_NAME punch)

set(INCLUDE_DIR include)
set(INCLUDE_EXT hpp)

set(SOURCES_DIR src)
set(SOURCE_EXT cpp)

# project dependencies
set(LIBS stdc++fs)

# configure the main header
configure_file(
    ${INCLUDE_DIR}/${PROGRAM_NAME}.${INCLUDE_EXT}.in
    ${INCDIR}/${PROGRAM_NAME}.${INCLUDE_EXT})

# compile the program
file(GLOB SOURCES ${SOURCES_DIR}/*.${SOURCE_EXT})
add_executable(${PROGRAM_NAME} ${SOURCES})

# link dependencies and include directories
target_link_libraries(${PROGRAM_NAME} ${LIBS})
target_include_directories(${PROGRAM_NAME} PRIVATE
    ${INCLUDE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}/${INCLUDE_DIR})
