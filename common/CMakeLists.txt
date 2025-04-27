cmake_minimum_required (VERSION 3.8)

file(GLOB common_SRC
     "*.h"
     "*.cpp"
)

add_library(common STATIC ${common_SRC})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET common PROPERTY CXX_STANDARD 20)
endif()

# TODO: Fügen Sie bei Bedarf Tests hinzu, und installieren Sie Ziele.
target_include_directories(common PUBLIC "..")
