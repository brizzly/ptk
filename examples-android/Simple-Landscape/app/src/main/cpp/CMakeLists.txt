# Specify minimum CMake version and project name
cmake_minimum_required(VERSION 3.4.1)
project("android_game_project")



# If GLM is directly downloaded in your project (libs/glm folder)
# Add this line to set it up as an imported target
add_library(glm INTERFACE)
#target_include_directories(glm INTERFACE ${CMAKE_SOURCE_DIR}/../../../libs/glm)
target_include_directories(glm INTERFACE ${CMAKE_SOURCE_DIR}/../../../../../../../ptk/ExternalSDKs/glm)

# Use ${ANDROID_ABI} to select the correct architecture version
#set(OpenAL_DIR "${CMAKE_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI}")
set(OpenAL_DIR ${CMAKE_SOURCE_DIR}/../jniLibs/${ANDROID_ABI})
#find_library(openal-lib openal PATHS ${OpenAL_DIR})
find_library(openal-lib openal PATHS ${OpenAL_DIR} NO_CMAKE_FIND_ROOT_PATH)

# Example for linking with OpenSL ES
#find_library(opensl_es OpenSLES REQUIRED)



# Include directories for libmpg123 headers
#include_directories(${CMAKE_SOURCE_DIR}/../../../../../../mpg123/include)
include_directories(${CMAKE_SOURCE_DIR}/../../../../../../../ptk/ExternalSDKs/mpg123/include)


# Include the OpenAL directory for the headers
#include_directories(${CMAKE_SOURCE_DIR}/../../../../../../OpenAL/include)
include_directories(${CMAKE_SOURCE_DIR}/../../../../../../../ptk/ExternalSDKs/OpenAL/include)

#include_directories(${CMAKE_SOURCE_DIR}/../../../libs/openal/include)
#include_directories(${CMAKE_SOURCE_DIR}/../../../libs/openal/common)
#include_directories(${CMAKE_SOURCE_DIR}/../../../libs/openal/core)
#include_directories(${CMAKE_SOURCE_DIR}/../../../libs/openal)



# Chemin vers le répertoire partagé
set(PTK_SOURCE_DIR ${CMAKE_SOURCE_DIR}/../../../../../../iPTKSources)


# Récupérer tous les fichiers .cpp et .h du répertoire partagé
file(GLOB_RECURSE SHARED_SOURCE_FILES
        ${PTK_SOURCE_DIR}/*.cpp
        ${PTK_SOURCE_DIR}/*.h
)

# Exclure des fichiers spécifiques (GLTextureHelper.mm et GLTextureHelper.h)
list(REMOVE_ITEM SHARED_SOURCE_FILES
        ${PTK_SOURCE_DIR}/*.mm
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



# =============================================================================
#               Chemin vers le répertoire Box2D
# =============================================================================
set(Box2D_SOURCE2_DIR "${CMAKE_SOURCE_DIR}/../../../../../../../ptk/ExternalSDKs")
include_directories("${Box2D_SOURCE2_DIR}")
set(Box2D_SOURCE_DIR "${CMAKE_SOURCE_DIR}/../../../../../../../ptk/ExternalSDKs/Box2D")
include_directories("${Box2D_SOURCE_DIR}")

# Debug output
message(STATUS "Box2D_SOURCE_DIR: ${Box2D_SOURCE_DIR}")

# Add Box2D headers
file(GLOB_RECURSE Box2D_HEADERS "${Box2D_SOURCE_DIR}/*.h")

# Optional: Add Box2D sources if you're building Box2D from source
file(GLOB_RECURSE Box2D_SOURCES "${Box2D_SOURCE_DIR}/*.cpp")

# Add Box2D as a static library
add_library(Box2D STATIC ${Box2D_SOURCES})
target_include_directories(Box2D PUBLIC "${Box2D_SOURCE_DIR}")

# =============================================================================
#               Chemin vers le répertoire Freetype2
# =============================================================================
#set(FREETYPE_LIBRARY_PATH "${CMAKE_SOURCE_DIR}/../../../../../../../ptk/ExternalSDKs/ft2/libfreetype.a")
set(FREETYPE_LIBRARY_PATH "${CMAKE_SOURCE_DIR}/../../../../../../../ptk/ExternalSDKs/ft2/libfreetype-${ANDROID_ABI}.a")

set(Ft2_SOURCE_DIR "${CMAKE_SOURCE_DIR}/../../../../../../../ptk/ExternalSDKs/ft2")
include_directories(${Ft2_SOURCE_DIR} ${Ft2_SOURCE_DIR}/freetype)
message(STATUS "Box2D_SOURCE_DIR: ${Ft2_SOURCE_DIR}")
#file(GLOB_RECURSE Ft2_SOURCE_DIR
#        ${Ft2_SOURCE_DIR}/*.cpp
#        ${Ft2_SOURCE_DIR}/*.h
#)
#include_directories(${Ft2_SOURCE_DIR})



# Specify source files
set(SOURCE_FILES
        ${SHARED_SOURCE_FILES}
        android_integration.cpp
        MainController.cpp
        MainController.h
        game.cpp
        game.h
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
        android_landscape_game_lib
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


# Add libmpg123 as a prebuilt library
add_library(mpg123-lib SHARED IMPORTED)
set_target_properties(mpg123-lib PROPERTIES IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/../jniLibs/${ANDROID_ABI}/libmpg123.so)


# Link libraries to the native library
target_link_libraries(
        android_landscape_game_lib PRIVATE glm
        c++_shared
        mpg123-lib
        z
        log
        ${FREETYPE_LIBRARY_PATH}
        ${openal-lib}
        ${OPENSLES_LIB}
        ${log-lib}
        ${android-lib}
        ${GLESv2-lib}
        ${EGL-lib}
)

# Enable debugging information
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -O0 -g")

