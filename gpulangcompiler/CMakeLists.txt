#-------------------------------------------------------------------------------
# gpulangc
#-------------------------------------------------------------------------------

fips_begin_app(gpulangc cmdline)
    target_compile_options(gpulangc PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/W3>)
    target_include_directories(gpulangc PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
    fips_deps(gpulang_compiler glslang)
    if(FIPS_LINUX OR FIPS_APPLE)
        fips_deps(stdc++fs)
    endif()
    fips_files(
        shadercompilerapp.cc
        shadercompilerapp.h
        singleshadercompiler.cc
        singleshadercompiler.h   
        argh.h
    )

    set_target_properties(gpulangc PROPERTIES
        OUTPUT_NAME "gpulangc${PLATFORM}"
    )
fips_end_app()
