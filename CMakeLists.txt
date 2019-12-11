cmake_minimum_required(VERSION 3.10)
project(unique)

#creating 3 variables, storing the names of a few files that will be looped over later
set(SOURCE_FILES)
	
set(HEADER_FILES
	unique_ptr.hpp)

set(UNIT_TEST_FILES
    TestMemory.cpp)

#Adds to CMake the excecutable files to be run	
add_executable(unique ${HEADER_FILES} ${SOURCE_FILES} main.cpp)
add_executable(UnitTestRunner ${HEADER_FILES} ${SOURCE_FILES} ${UNIT_TEST_FILES})

#not really sure what this does
set_property(TARGET unique PROPERTY CXX_STANDARD 17)
set_property(TARGET UnitTestRunner PROPERTY CXX_STANDARD 17)

#Setting error settings for the program
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    target_compile_options(unique PRIVATE /W4 /permissive-)
    target_compile_options(UnitTestRunner PRIVATE /W4 /permissive-)
    set_target_properties(unique PROPERTIES LINK_FLAGS "/STACK:10000000")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_compile_options(unique PRIVATE -Wall -Wextra -pedantic)
    target_compile_options(UnitTestRunner PRIVATE -Wall -Wextra -pedantic)
endif()

#trying to find a clang format file
find_program(CLANG_FORMAT "clang-format")

#if we find a clang format file we unset the current paths to our files and
#loop over our previously created variables to find new paths to them.
if (CLANG_FORMAT)
	unset(SOURCE_FILES_PATHS)
    foreach(SOURCE_FILE ${HEADER_FILES} ${SOURCE_FILES} ${UNIT_TEST_FILES} main.cpp)
        get_source_file_property(WHERE ${SOURCE_FILE} LOCATION)
        set(SOURCE_FILES_PATHS ${SOURCE_FILES_PATHS} ${WHERE})
    endforeach()

#I'm not quite sure what this does either but it makes it work
	add_custom_target(
		ClangFormat
		COMMAND ${CLANG_FORMAT}
	    -i
		-style=file
		${SOURCE_FILES_PATHS})

	add_dependencies(unique ClangFormat)
else()
    message("Unable to find clang-format")
endif()

#This whole thing makes gtest work with our code. It does alot of stuff I don't understand completely
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

target_link_libraries(UnitTestRunner gtest_main)
set_target_properties(unique PROPERTIES LINK_FLAGS "/STACK:10000000")