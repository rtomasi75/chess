cmake_minimum_required (VERSION 3.8)

file(GLOB engine_SRC
     "*.h"
     "*.cpp"
     "commands/*.h"
     "commands/*.cpp"
)

add_library(engine STATIC ${engine_SRC})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET engine PROPERTY CXX_STANDARD 20)
endif()

target_include_directories(engine PUBLIC "..")
target_link_libraries(engine PUBLIC common movegen search)

