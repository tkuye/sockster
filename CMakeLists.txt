cmake_minimum_required(VERSION 3.10)

project(sockster VERSION 1.0)

set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

include(ExternalProject)

list(APPEND SOURCE "src/cJSON.c" 
"src/server.c"  
"src/sockster.c"
)


add_executable(sockster 
${SOURCE}
)

set(libevent_INSTALL_DIR "${CMAKE_CURRENT_BINARY_DIR}/libevent")
set(libevent_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}/libevent/include")
set(libevent_SRC_DIR "${CMAKE_CURRENT_BINARY_DIR}/libevent/src")
set(libevent_CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${libevent_INSTALL_DIR})


set(SOCK_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(LOCAL_INCLUDE_DIR /usr/local/include)
set(INCLUDE_DIR /usr/include)

set(GCC_COVERAGE_COMPILE_FLAGS "-lcjson -pthread")


SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

add_definitions(${GCC_COVERAGE_COMPILE_FLAGS})




include_directories(
${SOCK_INCLUDE_DIR}
${LOCAL_INCLUDE_DIR}
${INCLUDE_DIR}
${libevent_INSTALL_DIR}
)

list(APPEND CMAKE_PREFIX_PATH "/usr/local/include/")


ExternalProject_Add(libevent
PREFIX ${libevent_INSTALL_DIR}
GIT_REPOSITORY  "https://github.com/libevent/libevent.git"
SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/deps" 
TIMEOUT 5
CMAKE_ARGS ${libevent_CMAKE_ARGS}
INSTALL_DIR ${libevent_INSTALL_DIR}
INSTALL_COMMAND make install
)

target_link_libraries(sockster ${libevent_INSTALL_DIR}/lib/libevent.so)

add_dependencies(sockster libevent)

# remove deps folder as we no longer need it
add_custom_command(TARGET sockster
    POST_BUILD
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND bash del.sh
    COMMENT "Deleting deps folder..."
)


