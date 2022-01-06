cmake_minimum_required(VERSION 3.21.3)
project(ronix)

set(CMAKE_CXX_STANDARD 11)
find_package(SDL2 REQUIRED)

set(HL2SDK_DIR ${PROJECT_SOURCE_DIR}/hl2sdk)
execute_process(OUTPUT_VARIABLE HL2SDK_INC COMMAND find ${HL2SDK_DIR} -type d -exec printf "{};" \;)

set(IMGUI_DIR ${PROJECT_SOURCE_DIR}/imgui)
file(GLOB_RECURSE IMGUI_SRC ${IMGUI_DIR}/*.cpp)

set(RONIX_DIR ${PROJECT_SOURCE_DIR}/ronix)
set(RONIX_PCH ${RONIX_DIR}/pch.hpp)
file(GLOB_RECURSE RONIX_SRC ${RONIX_DIR}/*.cpp)

add_library(imgui STATIC ${IMGUI_SRC})
target_include_directories(imgui PUBLIC ${IMGUI_DIR} ${SDL2_INCLUDE_DIRS})
target_compile_options(imgui PUBLIC -g -m32 -fPIC)
target_link_options(imgui PUBLIC -m32)

add_library(ronix SHARED ${RONIX_SRC})
target_include_directories(ronix PUBLIC ${PROJECT_SOURCE_DIR} ${RONIX_DIR} ${IMGUI_DIR} ${HL2SDK_INC} ${SDL2_INCLUDE_DIRS})
target_precompile_headers(ronix PUBLIC ${RONIX_PCH})
target_compile_options(ronix PUBLIC -g -ggdb -m32 -shared -fPIC -fpermissive -pthread)
target_link_options(ronix PUBLIC -m32 -static-libstdc++ -pthread LINKER:--no-undefined,--no-allow-shlib-undefined)
target_link_libraries(ronix PUBLIC stdc++ imgui m dl GL SDL2)
