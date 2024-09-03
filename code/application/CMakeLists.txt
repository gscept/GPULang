#-------------------------------------------------------------------------------
# anyfx_application
#-------------------------------------------------------------------------------

set(CMAKE_CXX_STANDARD 17)

fips_begin_app(anyfx_application cmdline)
    fips_vs_warning_level(3)
    fips_include_directories(
         ${CMAKE_CURRENT_SOURCE_DIR}
         ${ANYFX_ROOT}/code/util
         )
    fips_deps(anyfx_compiler anyfx_util antlr4 mcpp glslang)
    if(FIPS_LINUX)
	    fips_deps(anyfxc stdc++fs)
    endif()
    fips_files(
         shadercompilerapp.cc
         shadercompilerapp.h
         singleshadercompiler.cc
         singleshadercompiler.h
		 argh.h
     )
fips_end_app()
