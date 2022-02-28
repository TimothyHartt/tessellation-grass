file(GLOB SOURCES "*.cpp" "*.c" "*.hpp" "*.h")

add_executable(final 
    ${SOURCES}
)
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)



target_link_libraries(final PRIVATE 
    ${SDL2_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${CMAKE_DL_LIBS}
)

target_include_directories(final PUBLIC
    ${SDL2_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIR}
    glad
    KHR
    glm)
