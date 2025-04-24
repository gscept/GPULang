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

fips_end_lib()
