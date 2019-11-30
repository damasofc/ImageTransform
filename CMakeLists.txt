cmake_minimum_required (VERSION 2.8)
project (filterImages)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_executable(${PROJECT_NAME}  main.cpp )

add_definitions( -DMAGICKCORE_QUANTUM_DEPTH=64 )
add_definitions( -DMAGICKCORE_HDRI_ENABLE=0 )
find_package(ImageMagick COMPONENTS Magick++ MagickCore)
include_directories(${ImageMagick_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${ImageMagick_LIBRARIES})

include_directories(${PROJECT_SOURCE_DIR})
