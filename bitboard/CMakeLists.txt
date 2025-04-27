cmake_minimum_required (VERSION 3.8)

file(GLOB bitboard_SRC
     "*.h"
     "*.cpp"
)

add_library(bitboard STATIC ${bitboard_SRC})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET bitboard PROPERTY CXX_STANDARD 20)
endif()

target_link_libraries(bitboard PUBLIC common)
target_include_directories(bitboard PUBLIC "..")
