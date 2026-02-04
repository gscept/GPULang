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

if(BUILD_GPULANG_COMPILER)

find_package(Vulkan REQUIRED COMPONENTS SPIRV-Tools)

find_library(SPIRV-Tools-opt_LIBRARY
NAMES SPIRV-Tools-opt
HINTS
    $ENV{VULKAN_SDK}/lib)
mark_as_advanced(SPIRV-Tools-opt_LIBRARY)

find_library(SPIRV-Tools-opt_DEBUG_LIBRARY
NAMES SPIRV-Tools-optd
HINTS
    $ENV{VULKAN_SDK}/lib)
mark_as_advanced(SPIRV-Tools-opt_DEBUG_LIBRARY)


if((SPIRV-Tools-opt_LIBRARY OR SPIRV-Tools-opt_DEBUG_LIBRARY) AND NOT TARGET SPIRV-Tools-opt)
    add_library(SPIRV-Tools-opt STATIC IMPORTED)
	if(SPIRV-Tools-opt_LIBRARY)
		set_property(TARGET SPIRV-Tools-opt APPEND
		PROPERTY
			IMPORTED_CONFIGURATIONS Release)
		set_property(TARGET SPIRV-Tools-opt
		PROPERTY
			IMPORTED_LOCATION_RELEASE "${SPIRV-Tools-opt_LIBRARY}")
	endif()
	if(SPIRV-Tools-opt_DEBUG_LIBRARY)
		set_property(TARGET SPIRV-Tools-opt APPEND
		PROPERTY
			IMPORTED_CONFIGURATIONS Debug)
		set_property(TARGET SPIRV-Tools-opt
		PROPERTY
			IMPORTED_LOCATION_DEBUG "${SPIRV-Tools-opt_DEBUG_LIBRARY}")
	endif()
endif()

endif()


if(DEPLOY)
    string(TOLOWER "${CMAKE_SYSTEM_PROCESSOR}" ARCH)
    if(ARCH STREQUAL "amd64" OR ARCH STREQUAL "x86_64")
        set(ARCH "x86_64")
    elseif(ARCH STREQUAL "arm64" OR ARCH STREQUAL "aarch64")
        set(ARCH "aarch64")
    else()
        message(WARNING "Unsupported Architecture ${ARCH}")
    endif()

    string(TOLOWER "${CMAKE_SYSTEM_NAME}" OS)
    if(OS STREQUAL "darwin")
        set(PLATFORM "-macos-${ARCH}")
    elseif(OS STREQUAL "windows")
        set(PLATFORM "-windows-${ARCH}")
    elseif(OS STREQUAL "linux")
        set(PLATFORM "-linux-${ARCH}")
    else()
        message(WARNING "Unsupported OS ${OS}")
    endif()
    message(STATUS "Platform: ${PLATFORM}")
else()
    set(PLATFORM "")
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

	SET(GPULANG_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
	get_filename_component(GPULANG_SOURCE_DIR "gpulang" ABSOLUTE)

	add_subdirectory(code)
	
IF(BUILD_GPULANG_STANDALONE_COMPILER)
    add_subdirectory(gpulangcompiler)
ENDIF()

IF(BUILD_GPULANG_LANGUAGE_SERVER)
    add_subdirectory(gpulangserver)
ENDIF()
