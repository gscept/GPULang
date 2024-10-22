#-------------------------------------------------------------------------------
# GPULang tests
#-------------------------------------------------------------------------------


fips_begin_app(gpulang_test cmdline)

    target_include_directories(gpulang_test PUBLIC
        ${GPULANG_ROOT}/code/compiler
        ${GPULANG_ROOT}/code/api/

        )

    ADD_DEFINITIONS(-DTEST_FOLDER="${GPULANG_ROOT}/code/test/shaders")
    ADD_DEFINITIONS(-DTEST_OUTPUT_FOLDER="${GPULANG_ROOT}/code/test/shaders/output")

    fips_files(
        main.cc
    )

    fips_files_ex(shaders
        *.gpul
    )

    fips_deps(gpulang_compiler gpulang_reflection)

fips_end_app()
