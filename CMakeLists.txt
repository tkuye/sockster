cmake_minimum_required(VERSION 3.10)

project(sockster VERSION 1.0)

set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")



list(APPEND SOURCE "src/cJSON.c" 
"src/server.c"  
"src/sockster.c"
)



add_executable(sockster 
${SOURCE}
)



set(SOCK_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(LOCAL_INCLUDE_DIR /usr/local/include)
set(INCLUDE_DIR /usr/include)

set(GCC_COVERAGE_COMPILE_FLAGS "-lcjson -pthread ")


SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

add_definitions(${GCC_COVERAGE_COMPILE_FLAGS})


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
message(WARNING "Could not find libevent2")
endif()

list(APPEND REQ_LIB ${LIB_EVENT})
