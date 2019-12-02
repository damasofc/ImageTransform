cmake_minimum_required(VERSION 3.5)
project(hello LANGUAGES CXX)

add_executable(hello main.cpp)
target_compile_features(hello PRIVATE cxx_std_11)
find_package(ImageMagick REQUIRED COMPONENTS Magick++)
target_compile_definitions(hello PRIVATE
  MAGICKCORE_QUANTUM_DEPTH=16
  MAGICKCORE_HDRI_ENABLE=0
)
target_include_directories(hello PRIVATE ${ImageMagick_INCLUDE_DIRS})

execute_process(COMMAND Magick++-config --ldflags
  OUTPUT_VARIABLE ImageMagick_LINK_FLAGS
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
target_link_libraries(hello PRIVATE ${ImageMagick_LINK_FLAGS})