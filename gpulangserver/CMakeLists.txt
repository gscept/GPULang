#-------------------------------------------------------------------------------
# gpulangserver
#-------------------------------------------------------------------------------

fips_begin_app(gpulang_server cmdline)
    target_compile_options(gpulang_server PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/W3>)
    target_include_directories(gpulang_server PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
    fips_deps(gpulang_compiler glslang lsp)
    if (WIN32)
        find_library(WSOCK32_LIBRARY wsock32)
        find_library(WS2_32_LIBRARY ws2_32)
        fips_deps(ws2_32)
    endif()
    if(FIPS_LINUX OR FIPS_APPLE)
        fips_deps(stdc++fs)
    endif()
    fips_files(
        languageserverapp.cc
        languageserverapp.h
    )

    set_target_properties(gpulang_server PROPERTIES
        OUTPUT_NAME "gpulang_server${PLATFORM}"
    )
fips_end_app()
