cmake_minimum_required (VERSION 3.8)

file(GLOB movegen_SRC
     "*.h"
     "*.cpp"
)

add_library(movegen STATIC ${movegen_SRC})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET movegen PROPERTY CXX_STANDARD 20)
endif()

target_include_directories(movegen PUBLIC "..")
target_link_libraries(movegen PUBLIC common bitboard)

