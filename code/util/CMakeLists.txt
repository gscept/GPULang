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
	serialize.h
	textwriter.cc
	textwriter.h
	types.h
	util.h
)

fips_end_lib()