#-------------------------------------------------------------------------------
# GPULang reflection
#-------------------------------------------------------------------------------


fips_begin_lib(gpulang_reflection)
fips_deps(gpulang_util)

target_include_directories(gpulang_reflection PUBLIC
	${GPULANG_ROOT}
	${GPULANG_ROOT}/code
	${GPULANG_ROOT}/code
	${VULKAN_INCLUDE_DIR}
)

fips_files(
	loader.cc
	loader.h
)

fips_dir(reflection)
	fips_files(
		program.h
		structure.h
		variable.h
	)

fips_dir(reflection/vk)
    fips_files(
        pipelineassembly.cc
        pipelineassembly.h
    )

    set_target_properties(gpulang_reflection PROPERTIES
        OUTPUT_NAME "gpulang_reflection${PLATFORM}"
    )

if(DEPLOY)
    install(TARGETS gpulang_reflection
        ARCHIVE DESTINATION ${CMAKE_SOURCE_DIR}/runtime/lib/
        LIBRARY DESTINATION ${CMAKE_SOURCE_DIR}/runtime/lib/
    )

    install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/"
        DESTINATION ${CMAKE_SOURCE_DIR}/runtime/include
        FILES_MATCHING PATTERN "*.h"
    )
endif()
fips_end_lib()
