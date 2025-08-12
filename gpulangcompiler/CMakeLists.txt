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
fips_end_app()

if (APPLE)
    add_custom_command(TARGET gpulangc POST_BUILD
        COMMAND codesign --sign - $<TARGET_FILE:gpulangc>
        COMMENT "Signing the binary with codesign"
    )
endif()

if (DEPLOY)
    add_custom_command(TARGET gpulangc POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/bin
        COMMAND ${CMAKE_COMMAND} -E copy
            $<TARGET_FILE:gpulangc>
            ${CMAKE_SOURCE_DIR}/bin/
        COMMENT "Copying the GPULang compiler binary to deploy folder"
    )
endif()