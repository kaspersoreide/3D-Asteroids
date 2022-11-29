project(application)

find_package(glm REQUIRED)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

include_directories(
    ${PROJECT_SOURCE_DIR}/src
)

file(GLOB_RECURSE all_srcs
    "${PROJECT_SOURCE_DIR}/include/*.h"
	"${PROJECT_SOURCE_DIR}/include/*.hpp"
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
	"${PROJECT_SOURCE_DIR}/src/*.c"
)

add_compile_definitions(GLEW_NO_GLU)

add_executable(application ${all_srcs})
target_link_libraries(application glm::glm OpenGL::GL GLEW::glew glfw)
