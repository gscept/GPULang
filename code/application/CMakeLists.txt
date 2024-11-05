#-------------------------------------------------------------------------------
# gpulang_application
#-------------------------------------------------------------------------------

set(CMAKE_CXX_STANDARD 20)

fips_begin_app(gpulang_application cmdline)
    fips_vs_warning_level(3)
    target_include_directories(gpulang_application PUBLIC
         ${CMAKE_CURRENT_SOURCE_DIR}
         ${ANYFX_ROOT}/code/util
         )
    fips_deps(gpulang_compiler gpulang_util antlr4 mcpp glslang)
    if(FIPS_LINUX)
	    fips_deps(gpulang_compiler stdc++fs)
    endif()
    fips_files(
         shadercompilerapp.cc
         shadercompilerapp.h
         singleshadercompiler.cc
         singleshadercompiler.h
		 argh.h
     )
fips_end_app()
