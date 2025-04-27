cmake_minimum_required (VERSION 3.8)

file(GLOB search_SRC
     "*.h"
     "*.cpp"
)

add_library(search STATIC ${search_SRC})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET search PROPERTY CXX_STANDARD 20)
endif()

target_include_directories(search PUBLIC "..")
target_link_libraries(search PUBLIC common bitboard movegen)

