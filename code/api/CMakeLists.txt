#-------------------------------------------------------------------------------
# anyfx api
#-------------------------------------------------------------------------------


fips_begin_lib(anyfx_api)
fips_deps(anyfx_util)

fips_include_directories(
	${ANYFX_ROOT}
	${ANYFX_ROOT}/code/util
	${ANYFX_ROOT}/exts/glslang
	${ANYFX_ROOT}/api
	${ANYFX_ROOT}/api/lowlevel
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
