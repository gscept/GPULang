cmake_minimum_required(VERSION 3.21)

option(BUILD_GPULANG_STANDALONE_COMPILER "Build standalone compiler" ON)
option(BUILD_GPULANG_APP "Build GPULang with the example app" OFF)
option(BUILD_GPULANG_TEST "Build GPULang with unit tests" OFF)
option(BUILD_GPULANG_COMPILER "Build GPULang compiler" ON)
option(BUILD_GPULANG_LANGUAGE_SERVER "Build GPULang language server" ON)
    
if(NOT FIPS_IMPORT)
	project(gpulang)
    get_filename_component(FIPS_ROOT_DIR "../fips" ABSOLUTE)
    include("${FIPS_ROOT_DIR}/cmake/fips.cmake")	
	fips_setup()
else()
    set(BUILD_GPULANG_COMPILER OFF)
    set(BUILD_GPULANG_TEST OFF)
    set(BUILD_GPULANG_STANDALONE_COMPILER OFF)
    set(BUILD_GPULANG_APP OFF)
endif()


IF(WIN32)
    add_definitions(-D__WIN32__)
ELSEIF(UNIX AND NOT APPLE)
    add_definitions(-D__linux__)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-variable -Wno-unused-parameter -fbracket-depth=4096")
ELSEIF(APPLE)
    add_definitions(-D__APPLE__)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-variable -Wno-unused-parameter -fbracket-depth=4096")
ENDIF()

	set(CMAKE_CXX_STANDARD 20)
	set(CMAKE_CXX_STANDARD_REQUIRED on)
	set(CMAKE_CXX_EXTENSIONS OFF)
	#set(FIPS_WINDOWS_LTCG OFF)

	ADD_DEFINITIONS(-DANTLR4CPP_STATIC)


	SET(GPULANG_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
	get_filename_component(GPULANG_SOURCE_DIR "gpulang" ABSOLUTE)

	add_subdirectory(code)
	
IF(BUILD_GPULANG_STANDALONE_COMPILER)
    add_subdirectory(gpulangcompiler)
ENDIF()

IF(BUILD_GPULANG_LANGUAGE_SERVER)
    add_subdirectory(gpulangserver)
ENDIF()
