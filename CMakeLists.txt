cmake_minimum_required (VERSION 3.8)

# Aktivieren Sie Hot Reload für MSVC-Compiler, sofern unterstützt.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("pygmalion")

file(GLOB pygmalion_SRC
     "*.h"
     "*.cpp"
)
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    add_compile_options(-march=native)
endif()

# Schließen Sie Unterprojekte ein.
add_subdirectory ("common")
add_subdirectory ("bitboard")
add_subdirectory ("movegen")
add_subdirectory ("search")
add_subdirectory ("engine")

add_executable(pygmalion ${pygmalion_SRC})

target_link_libraries(pygmalion engine)


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET pygmalion PROPERTY CXX_STANDARD 20)
endif()
