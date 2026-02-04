add_subdirectory(api)
add_subdirectory(util)

IF(BUILD_GPULANG_COMPILER)

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

add_subdirectory(compiler)
ENDIF()

IF(BUILD_GPULANG_APP)
add_subdirectory(app)
ENDIF()

IF(BUILD_GPULANG_TEST)
add_subdirectory(test)
ENDIF()

