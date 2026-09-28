cmake_minimum_required(VERSION 3.16)
project(OpenGLSolarSystem LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenGL REQUIRED)

# GLFW is vendored so the project can be cloned and built without manually
# downloading the library.
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
# GLFW 3.3.2 predates CMake 4; opt into the supported policy baseline.
set(CMAKE_POLICY_VERSION_MINIMUM 3.10)
add_subdirectory(third_party/glfw EXCLUDE_FROM_ALL)

# Build the vendored GLEW source as a static library.
add_library(glew_static STATIC third_party/glew/src/glew.c)
target_include_directories(glew_static PUBLIC third_party/glew/include)
target_compile_definitions(glew_static PUBLIC GLEW_STATIC GLEW_NO_GLU)
target_link_libraries(glew_static PUBLIC OpenGL::GL)
if(UNIX AND NOT APPLE)
    target_link_libraries(glew_static PUBLIC ${CMAKE_DL_LIBS})
endif()

set(SHADER_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/shaders/")
set(TEXTURE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/assets/textures/")
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/include/ProjectPaths.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/generated/ProjectPaths.h"
    @ONLY
)

add_executable(solar_system
    src/main.cpp
    src/stb_image.cpp
)

target_include_directories(solar_system PRIVATE
    include
    "${CMAKE_CURRENT_BINARY_DIR}/generated"
    third_party/glm
    third_party/stb
)

target_compile_definitions(solar_system PRIVATE GLEW_STATIC GLEW_NO_GLU GLFW_INCLUDE_NONE NOMINMAX)
if(MSVC)
    target_compile_definitions(solar_system PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

target_link_libraries(solar_system PRIVATE
    OpenGL::GL
    glfw
    glew_static
)

# Package resources beside the executable for relocatable builds.
add_custom_command(TARGET solar_system POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/assets" "$<TARGET_FILE_DIR:solar_system>/assets"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/shaders" "$<TARGET_FILE_DIR:solar_system>/shaders"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/THIRD_PARTY_NOTICES.md"
        "$<TARGET_FILE_DIR:solar_system>/THIRD_PARTY_NOTICES.md"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/licenses" "$<TARGET_FILE_DIR:solar_system>/licenses"
)
install(TARGETS solar_system RUNTIME DESTINATION .)
install(DIRECTORY assets shaders licenses DESTINATION .)
install(FILES README.md THIRD_PARTY_NOTICES.md DESTINATION .)
