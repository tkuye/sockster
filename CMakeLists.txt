cmake_minimum_required(VERSION 3.10)

project(sockster VERSION 1.0)


add_executable(sockster 
"src/sockster.c")

set(LOCAL_INCLUDE_DIR /usr/local/include)
set(INCLUDE_DIR /usr/include)

set(CMAKE_C_FLAGS " ${CMAKE_C_FLAGS} -pthread -cjson")


set(SOCK_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(LIB_EVENT_2 event2)

target_include_directories(sockster PUBLIC 
${SOCK_INCLUDE_DIR}
${LOCAL_INCLUDE_DIR}
${INCLUDE_DIR}
)


list(APPEND CMAKE_PREFIX_PATH "/usr/local/include/")

find_library(LIB_EVENT NAMES event2 event libevent2 libevent PATHS ${LOCAL_INCLUDE_DIR})

target_link_libraries(sockster ${LIB_EVENT})

if(${LIB_EVENT}_NOT_FOUND)
message(FATAL_ERROR "Could not find libevent2")
endif()

list(APPEND REQ_LIB ${LIB_EVENT})
