#-------------------------------------------------------------------------------
# GPULang tests
#-------------------------------------------------------------------------------


fips_begin_app(gpulang_test cmdline)

    fips_include_directories(
        ${GPULANG_ROOT}/code/compiler
        ${GPULANG_ROOT}/code/api/

        )

    ADD_DEFINITIONS(-DTEST_FOLDER="${ANYFX_ROOT}/code/test/shaders")
    ADD_DEFINITIONS(-DTEST_OUTPUT_FOLDER="${ANYFX_ROOT}/code/test/shaders/output")

    fips_files(
        main.cc
        completeshadertest.cc
        constantbuffertest.cc
        readwritebuffertest.cc
        apitest.cc
    )

    fips_deps(gpulang_compiler gpulang_reflection)

fips_end_app()
