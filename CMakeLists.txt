cmake_minimum_required(VERSION 3.1)
project(renoir)
set(CMAKE_CXX_STANDARD 11)


#####################################################
# SDL2 LOADING
#####################################################

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)



#####################################################
# DIRECTORES
#####################################################

set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(EXTERNAL_DIR ${CMAKE_SOURCE_DIR}/external)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(BUILD_DIR ${CMAKE_SOURCE_DIR}/bin)
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/imgui)
set(ASSETS_DIR ${CMAKE_SOURCE_DIR}/assets)
set(GLM_DIR ${CMAKE_SOURCE_DIR}/glm)

if (UNIX)
  set(PLATFORM_DIR ${CMAKE_SOURCE_DIR}/platforms/linux)
else()
  set(PLATFORM_DIR ${CMAKE_SOURCE_DIR}/platforms/win32)
endif()

set(WINDOWS_PLATFORM_DIR ${CMAKE_SOURCE_DIR}/platforms/win32)
set(LINUX_PLATFORM_DIR ${CMAKE_SOURCE_DIR}/platforms/linux)

#####################################################
# SOURCE GLOBBING
#####################################################

file(GLOB_RECURSE INTERNAL_SRCS ${SOURCE_DIR}/*.cc
                                ${SOURCE_DIR}/*.h
)

file(GLOB_RECURSE EXTERNAL_SRCS ${EXTERNAL_DIR}/*.cpp
                                ${EXTERNAL_DIR}/*.cc
                                ${EXTERNAL_DIR}/*.h
                                ${EXTERNAL_DIR}/*.c
)

file(GLOB_RECURSE WINDOWS_SRCS ${WINDOWS_PLATFORM_DIR}/*.cc
                               ${WINDOWS_PLATFORM_DIR}/*.h)
file(GLOB_RECURSE LINUX_SRCS ${LINUX_PLATFORM_DIR}/*.cc
                             ${LINUX_PLATFORM_DIR}/*.h)

file(GLOB IMGUI_SRCS ${IMGUI_DIR}/*.cpp
                     ${IMGUI_DIR}/*.h
)

set(SOURCES ${INTERNAL_SRCS} ${EXTERNAL_SRCS} ${IMGUI_SRCS})

# Group of files that will send warnings
# TODO(cristiandonosoc): Remove warnings for these files
#                        Instead of just removing -Werror/-WX
file(GLOB_RECURSE NO_WARN_AS_ERROR ${EXTERNAL_DIR}/*
                                   ${INCLUDE_DIR}/*
                                   ${IMGUI_DIR}/*
                                   src/main.cc
)

#####################################################
# INCLUDES
#####################################################

# For includes like <external/bla.h> or <glm/glm.h>
include_directories(SYSTEM ${CMAKE_SOURCE_DIR})
include_directories(${SOURCE_DIR})
include_directories(SYSTEM ${INCLUDE_DIR}
                           ${IMGUI_DIR}
                           ${SDL2_INCLUDE_DIR}
                           ${GLM_DIR}
                           ${EXTERNAL_DIR})

#####################################################
# COMPILER FLAGS
#####################################################

if (UNIX)
  message(STATUS "Setting linux C++ flags")
  # Linux CXX flags
  set(LINUX_CXX_FLAGS "-Wall -Wextra -Werror")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${LINUX_CXX_FLAGS}")
  # Adding linux linker flags
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -ldl")
else()
  message(STATUS "Setting MSVC flags")
  # Ignoring the following warnings
  # warning C4267: 'argument': conversion from 'size_t' to 'int', possible loss of data
  set(IGNORES "/wd4267")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${IGNORES} /MP /WX /EHsc")
endif()

#####################################################
# ERROR WARNINGS FIX
#####################################################

# -Werror Handling
if(CMAKE_CXX_FLAGS MATCHES "-Werror")
  string(REPLACE "-Werror" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
  set(ERROR_FLAGS "${ERROR_FLAGS} -Werror")
endif()
if(CMAKE_CXX_FLAGS MATCHES "/WX")
  string(REPLACE "/WX" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
  set(ERROR_FLAGS "${ERROR_FLAGS} /WX")
endif()

# We add it warning-as-error only for files that need it
if (ERROR_FLAGS)
  foreach (f ${SOURCES})
    list(FIND NO_WARN_AS_ERROR ${f} _index)
    if (${_index} GREATER -1)
      set_source_files_properties(f COMPILE_FLAGS ${ERROR_FLAGS})
    endif()
  endforeach()
endif()

#####################################################
# TARGET DEFINITIONS
#####################################################

# EXECUTABLE
add_executable(renoir ${SOURCES})

target_link_libraries(renoir ${SDL2_LIBRARY})
target_link_libraries(renoir ${OPENGL_LIBRARIES})

#####################################################
# PLATFORM
#####################################################

if (EXISTS ${PLATFORM_DIR})
  message(FATAL "test")
  file(GLOB_RECURSE PLATFORM_SRCS ${PLATFORM_DIR}/*.cc
                                  ${PLATFORM_DIR}/*.h)
  add_library(platform STATIC ${PLATFORM_SRCS})
  target_link_libraries(renoir platform)
endif()

#####################################################
# SHADER COPY
#####################################################

add_custom_command(TARGET renoir POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${ASSETS_DIR} $<TARGET_FILE_DIR:renoir>)

#####################################################
# OUTPUT
#####################################################

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${BUILD_DIR}")
set(CMAKE_ARCHIVE_LIBRARY_DIRECTORY "${BUILD_DIR}")
set(CMAKE_ARCHIVE_RUNTIME_DIRECTORY "${BUILD_DIR}")
