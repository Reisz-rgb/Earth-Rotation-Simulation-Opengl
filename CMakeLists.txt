cmake_minimum_required(VERSION 3.5)
project(SolarSystemSimulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

message(STATUS "Fetching GLFW...")
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

message(STATUS "Fetching GLM...")
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 0.9.9.8
)
FetchContent_MakeAvailable(glm)

message(STATUS "Fetching GLAD...")
FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG v0.1.36
)
FetchContent_MakeAvailable(glad)

find_package(OpenGL REQUIRED)

add_executable(main 
    main.cpp
    shader.cpp
    geometry.cpp
    input.cpp
)

target_link_libraries(main 
    PRIVATE 
    glfw
    glad
    glm::glm
    OpenGL::GL
)

target_include_directories(main 
    PRIVATE 
    ${glad_SOURCE_DIR}/include # For GLAD headers
    ${CMAKE_CURRENT_SOURCE_DIR} # For local headers 
)

# Set output directory
set_target_properties(main PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Platform-specific settings
if(APPLE)
    target_compile_definitions(main PRIVATE GL_SILENCE_DEPRECATION)
endif()

# Print build information
message(STATUS "=================================")
message(STATUS "Solar System Simulator - Build Configuration")
message(STATUS "=================================")
message(STATUS "Platform: ${CMAKE_SYSTEM_NAME}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Using: GLFW + GLAD + GLM (modern OpenGL stack)")
message(STATUS "=================================")

# Installation rules
install(TARGETS main DESTINATION bin)

# Print instructions after build
add_custom_command(TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo ""
    COMMAND ${CMAKE_COMMAND} -E echo "=== Build Complete ==="
    COMMAND ${CMAKE_COMMAND} -E echo "Executable: ${CMAKE_BINARY_DIR}/bin/main"
    COMMAND ${CMAKE_COMMAND} -E echo ""
    COMMAND ${CMAKE_COMMAND} -E echo "Controls:"
    COMMAND ${CMAKE_COMMAND} -E echo "  Mouse: Rotate camera"
    COMMAND ${CMAKE_COMMAND} -E echo "  Scroll: Zoom in/out"
    COMMAND ${CMAKE_COMMAND} -E echo "  ESC: Exit"
    COMMAND ${CMAKE_COMMAND} -E echo ""
)