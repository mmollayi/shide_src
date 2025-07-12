
cmake_minimum_required(VERSION 3.8)

project(shide_src)
set(CMAKE_CXX_STANDARD 17)

add_library(shide INTERFACE)

target_include_directories( shide INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> )

add_subdirectory(external/date)
#target_compile_definitions(date INTERFACE ONLY_C_LOCALE=1)
#target_compile_definitions(date-tz PRIVATE ONLY_C_LOCALE=1)

#set(TZDB_PATH "${CMAKE_SOURCE_DIR}/external/date/tzdata")
#add_compile_definitions(TZDB_PATH=\"${TZDB_PATH}\")

#add_custom_target(build_tz_data ALL
#    COMMAND ${CMAKE_COMMAND} -E echo "Downloading and installing tzdata..."
#    COMMAND ${CMAKE_COMMAND} -S ${CMAKE_SOURCE_DIR}/external/date -B ${CMAKE_SOURCE_DIR}/external/date/build
#    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_SOURCE_DIR}/external/date/build --target install_tz_db
 #   BYPRODUCTS ${CMAKE_SOURCE_DIR}/external/date/tzdata
#    COMMENT "Installing timezone database"
#    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/external/date
#)

target_link_libraries(shide INTERFACE date::date date::date-tz)

add_executable(main src/main.cpp)
target_link_libraries(main PRIVATE shide)
#add_dependencies(main build_tz_data)

