#-------------------------------------------------------------------------------
# GPULang util
#-------------------------------------------------------------------------------

fips_begin_lib(gpulang_util)

target_include_directories(gpulang_util PUBLIC
	${CMAKE_CURRENT_BINARY_DIR}
	${GPULANG_ROOT}/util/
	)

fips_files(
	binreader.cc
	binreader.h
	binwriter.cc
	binwriter.h
	cmdlineargs.cc
	cmdlineargs.h
    containers.cc
    containers.h
	memory.cc
	memory.h
	serialize.h
    shaderusage.h
    string_utils.h
	textwriter.cc
	textwriter.h
    thread.h
	types.h
	util.h
)

fips_end_lib()
