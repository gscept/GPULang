#-------------------------------------------------------------------------------
# GPULang reflection
#-------------------------------------------------------------------------------


fips_begin_lib(gpulang_reflection)
fips_deps(gpulang_util)

target_include_directories(gpulang_reflection PUBLIC
	${GPULANG_ROOT}
	${GPULANG_ROOT}/code/util
	${GPULANG_ROOT}/api
	${GPULANG_ROOT}/api/lowlevel
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

fips_end_lib()
