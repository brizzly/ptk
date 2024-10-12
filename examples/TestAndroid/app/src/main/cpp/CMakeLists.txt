# Specify minimum CMake version and project name
cmake_minimum_required(VERSION 3.4.1)
project("android_game_project")



# If GLM is directly downloaded in your project (libs/glm folder)
# Add this line to set it up as an imported target
add_library(glm INTERFACE)
target_include_directories(glm INTERFACE ${CMAKE_SOURCE_DIR}/../../../libs/glm)

# Add GLM include path
#include_directories(${CMAKE_SOURCE_DIR}/../../../libs/glm)

# Chemin vers le répertoire partagé
set(PTK_SOURCE_DIR ${CMAKE_SOURCE_DIR}/../../../../../../iPTKSources)

# Récupérer tous les fichiers .cpp et .h du répertoire partagé
file(GLOB_RECURSE SHARED_SOURCE_FILES
        ${PTK_SOURCE_DIR}/*.cpp
        ${PTK_SOURCE_DIR}/*.h
)

# Exclure des fichiers spécifiques (GLTextureHelper.mm et GLTextureHelper.h)
list(REMOVE_ITEM SHARED_SOURCE_FILES
        ${PTK_SOURCE_DIR}/GLTextureHelper.mm
        ${PTK_SOURCE_DIR}/GLTextureHelper.h
)

# Output the list of files collected
message(STATUS "Shared source files:")
foreach(file ${SHARED_SOURCE_FILES})
    message(STATUS "${file}")
endforeach()


# Specify source files
set(SOURCE_FILES
        ${SHARED_SOURCE_FILES}
        android_integration.cpp
        MainController.cpp
        MainController.h
        glfm_android.c
        glfm.h
        glfm_internal.h
        stb_image.h
)

# Inclure le répertoire partagé
include_directories(${PTK_SOURCE_DIR})



# Create shared library
add_library(
        android_game_lib
        SHARED
        ${SOURCE_FILES}
)

# Include directories for current source and header files
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Find Android system libraries
find_library(log-lib log)
find_library(android-lib android)
find_library(GLESv2-lib GLESv2)
find_library(EGL-lib EGL)

# Link libraries to the native library
target_link_libraries(
        android_game_lib PRIVATE glm
        ${log-lib}
        ${android-lib}
        ${GLESv2-lib}
        ${EGL-lib}
)

# Enable debugging information
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -O0 -g")
