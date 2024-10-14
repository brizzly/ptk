# Specify minimum CMake version and project name
cmake_minimum_required(VERSION 3.4.1)
project("android_game_project")



# If GLM is directly downloaded in your project (libs/glm folder)
# Add this line to set it up as an imported target
add_library(glm INTERFACE)
target_include_directories(glm INTERFACE ${CMAKE_SOURCE_DIR}/../../../libs/glm)


# Use ${ANDROID_ABI} to select the correct architecture version
#set(OpenAL_DIR "${CMAKE_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI}")
set(OpenAL_DIR ${CMAKE_SOURCE_DIR}/../jniLibs/${ANDROID_ABI})
#find_library(openal-lib openal PATHS ${OpenAL_DIR})
find_library(openal-lib openal PATHS ${OpenAL_DIR} NO_CMAKE_FIND_ROOT_PATH)

# Example for linking with OpenSL ES
find_library(opensl_es OpenSLES REQUIRED)






# Include the OpenAL directory for the headers
include_directories(${CMAKE_SOURCE_DIR}/../../../../../../OpenAL/include)
#include_directories(${CMAKE_SOURCE_DIR}/../../../libs/openal/include)
#include_directories(${CMAKE_SOURCE_DIR}/../../../libs/openal/common)
#include_directories(${CMAKE_SOURCE_DIR}/../../../libs/openal/core)
#include_directories(${CMAKE_SOURCE_DIR}/../../../libs/openal)


# Collect OpenAL source files
#file(GLOB_RECURSE OPENAL_SOURCES
#        ${CMAKE_SOURCE_DIR}/../../../libs/openal/*
#)

# Set OpenAL Library Path
#set(OPENAL_LIB_DIR ${CMAKE_SOURCE_DIR}/../../../../../../OpenAL)

# Include the OpenAL library path
#link_directories(${OPENAL_LIB_DIR})




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



message(STATUS "CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")

# Output the list of files collected (for debug purposes)
message(STATUS "OpenAL source files:")
foreach(file ${OPENAL_SOURCES})
    message(STATUS "${file}")
endforeach()

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

# Add OpenAL include directories
include_directories(${CMAKE_SOURCE_DIR}/openal_include)


# Find OpenAL library
#add_library(openal SHARED IMPORTED)
#set_target_properties(openal PROPERTIES IMPORTED_LOCATION ${libs_DIR}/libopenal.so)



# Create shared library
add_library(
        android_game_lib
        SHARED
        ${SOURCE_FILES}
        ${OPENAL_SOURCES}
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
        c++_shared
        ${openal-lib}
        ${opensl_es}
        ${log-lib}
        ${android-lib}
        ${GLESv2-lib}
        ${EGL-lib}
)

# Enable debugging information
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -O0 -g")

