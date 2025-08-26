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
    )
    fips_dir(argh)
    fips_files(
        argh.h
    )

    set_target_properties(gpulangc PROPERTIES
        OUTPUT_NAME "gpulangc${PLATFORM}"
    )
    target_compile_features(gpulangc PRIVATE cxx_std_20)
fips_end_app()

if (APPLE)
    add_custom_command(TARGET gpulangc POST_BUILD
        COMMAND codesign --force --sign - $<TARGET_FILE:gpulangc>
        COMMENT "Signing the binary with codesign"
    )
endif()

if (DEPLOY)
    install(TARGETS gpulangc
        RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/bin/
    )
endif()