#-------------------------------------------------------------------------------
# anyfx tests
#-------------------------------------------------------------------------------


fips_begin_app(anyfx_test cmdline)

    fips_include_directories(
        ${ANYFX_ROOT}/code/compiler
        ${ANYFX_ROOT}/code/api/

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

    set(LLVM_LINK_COMPONENTS
          LLVMBitWriter
          LLVMCore
          LLVMExecutionEngine
          LLVMMC
          LLVMMCJIT
          LLVMSupport
          )

    fips_deps(anyfx_compiler anyfx_api ${LLVM_LINK_COMPONENTS})

fips_end_app()
