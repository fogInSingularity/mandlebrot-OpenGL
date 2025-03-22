include(cmake/CPM.cmake)

CPMAddPackage(
    NAME glm
    GITHUB_REPOSITORY g-truc/glm
    GIT_TAG master
)

CPMAddPackage(
    NAME glad
    GITHUB_REPOSITORY Dav1dde/glad
    GIT_TAG v2.0.8
)

set(GLAD_SOURCES_DIR "${glad_SOURCE_DIR}")
set(GLAD_GEN_DIR "${CMAKE_BINARY_DIR}/gladgen")
add_subdirectory("${GLAD_SOURCES_DIR}/cmake" glad_cmake)
glad_add_library(glad STATIC REPRODUCIBLE EXCLUDE_FROM_ALL QUIET LOCATION ${GLAD_GEN_DIR} LOADER API gl:core=3.3)
include_directories("${GLAD_GEN_DIR}/include")
