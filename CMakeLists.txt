PROJECT(GPULang)
cmake_minimum_required(VERSION 3.22)
get_filename_component(FIPS_ROOT_DIR "../fips" ABSOLUTE)
include("${FIPS_ROOT_DIR}/cmake/fips.cmake")

ADD_DEFINITIONS(-DANTLR4CPP_STATIC)

if(NOT FIPS_IMPORT)
	fips_project(gpulang)
	SET(BUILD_GPULANGCOMPILER 1)
	fips_setup()
endif()

	SET(GPULANG_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
	get_filename_component(GPULANG_SOURCE_DIR "gpulang" ABSOLUTE)

	fips_add_subdirectory(code)


if(NOT FIPS_IMPORT)
    fips_finish()
endif()
