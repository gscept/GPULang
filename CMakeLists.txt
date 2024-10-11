cmake_minimum_required(VERSION 3.21)

if(NOT FIPS_IMPORT)
	project(gpulang)
    get_filename_component(FIPS_ROOT_DIR "../fips" ABSOLUTE)
    include("${FIPS_ROOT_DIR}/cmake/fips.cmake")
	SET(BUILD_GPULANGCOMPILER 1)
	fips_setup()
endif()

	set(CMAKE_CXX_STANDARD 20)
	set(CMAKE_CXX_STANDARD_REQUIRED on)
	set(CMAKE_CXX_EXTENSIONS OFF)
	#set(FIPS_WINDOWS_LTCG OFF)

	ADD_DEFINITIONS(-DANTLR4CPP_STATIC)


	SET(GPULANG_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
	get_filename_component(GPULANG_SOURCE_DIR "gpulang" ABSOLUTE)

	add_subdirectory(code)
