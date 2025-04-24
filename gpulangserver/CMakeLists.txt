#-------------------------------------------------------------------------------
# gpulangserver
#-------------------------------------------------------------------------------

fips_begin_app(gpulang_server cmdline)
    fips_vs_warning_level(3)
     fips_include_directories(${CMAKE_CURRENT_SOURCE_DIR})
     fips_deps(gpulang_compiler antlr4 mcpp glslang lsp)
     if (WIN32)
         find_library(WSOCK32_LIBRARY wsock32)
         find_library(WS2_32_LIBRARY ws2_32)
         fips_deps(ws2_32)
     endif()
     if(FIPS_LINUX OR FIPS_APPLE)
          fips_deps(gpulang stdc++fs)
     endif()
     fips_files(
          languageserverapp.cc
          languageserverapp.h
     )
fips_end_app()