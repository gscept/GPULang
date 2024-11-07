#-------------------------------------------------------------------------------
# gpulangcompiler
#-------------------------------------------------------------------------------

fips_begin_app(gpulangcompiler cmdline)
     fips_vs_warning_level(3)
     fips_include_directories(${CMAKE_CURRENT_SOURCE_DIR})
     fips_deps(gpulang_compiler antlr4 mcpp glslang)
     if(FIPS_LINUX OR FIPS_APPLE)
          fips_deps(gpulang stdc++fs)
     endif()
     fips_files(
          shadercompilerapp.cc
          shadercompilerapp.h
          singleshadercompiler.cc
          singleshadercompiler.h   
          argh.h
     )
fips_end_app()
