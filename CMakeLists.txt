project(Sm2-HW1)

cmake_minimum_required(VERSION 3.16)

if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
    add_definitions(-DLINUX)
    
endif()
  

#Won't need this 
if(WIN32)
    add_definitions(-DWINDOWS)
    configure_file(${CMAKE_SOURCE_DIR}/SDL2.dll ${CMAKE_BINARY_DIR}/bin/SDL2.dll COPYONLY)


endif()



add_compile_options(-Wall)

include_directories(${CMAKE_SOURCE_DIR}
    common/
    ext/
    )


set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/..)
else()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif()

add_subdirectory(src)
