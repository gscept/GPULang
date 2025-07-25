#-------------------------------------------------------------------------------
# GPULang compiler
#-------------------------------------------------------------------------------

ADD_SUBDIRECTORY(ext/antlr4)

fips_begin_lib(gpulang_compiler)

target_include_directories(gpulang_compiler PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}
    ${GPULANG_ROOT}/code/util/
    ${GPULANG_ROOT}/code/compiler
    ${GPULANG_ROOT}/code/compiler/ext/antlr4/src
    )

    fips_files(
        compiler.cc
        compiler.h
        gpulangcompiler.cc
        gpulangcompiler.h
    )

fips_dir(parser4)
    fips_files(
        GPULangParser.cpp
        GPULangParser.h
        GPULangLexer.cpp
        GPULangLexer.h
        gpulangerrorhandlers.cpp
        gpulangerrorhandlers.h
        gpulangtoken.cpp
        gpulangtoken.h
    )
    
fips_dir(ast)
    fips_files(
        alias.h
        alias.cc
        annotation.h
        attribute.h
        effect.cc
        effect.h
        enumeration.cc
        enumeration.h
        function.cc
        function.h
	generate.cc
	generate.h
        intrinsics.cc
        intrinsics.h
        preprocessor.cc
        preprocessor.h
        program.cc
        program.h
        renderstate.cc
        renderstate.h
        samplerstate.cc
        samplerstate.h
        state.cc
        state.h
        structure.cc
        structure.h
        symbol.cc
        symbol.h
        variable.cc
        variable.h
    )
fips_dir(ast/expressions)
    fips_files(
        accessexpression.cc
        accessexpression.h
        arrayindexexpression.cc
        arrayindexexpression.h
        arrayinitializerexpression.cc
        arrayinitializerexpression.h
        binaryexpression.cc
        binaryexpression.h
        boolexpression.cc
        boolexpression.h
        boolvecexpression.cc
        boolvecexpression.h
        callexpression.cc
        callexpression.h
        commaexpression.cc
        commaexpression.h
        declaredexpression.cc
        declaredexpression.h
        enumexpression.cc
        enumexpression.h
        expression.cc
        expression.h
        floatexpression.cc
        floatexpression.h
        floatvecexpression.cc
        floatvecexpression.h
        initializerexpression.cc
        initializerexpression.h
        intexpression.cc
        intexpression.h
        intvecexpression.cc
        intvecexpression.h
        stringexpression.cc
        stringexpression.h
        symbolexpression.cc
        symbolexpression.h
        ternaryexpression.cc
        ternaryexpression.h
        uintexpression.cc
        uintexpression.h
        uintvecexpression.cc
        uintvecexpression.h
        unaryexpression.cc
        unaryexpression.h
    )
fips_dir(ast/statements)
    fips_files(
        breakstatement.cc
        breakstatement.h
        continuestatement.cc
        continuestatement.h
        expressionstatement.cc
        expressionstatement.h
        forstatement.cc
        forstatement.h
        ifstatement.cc
        ifstatement.h
        scopestatement.cc
        scopestatement.h
        statement.cc
        statement.h
        switchstatement.cc
        switchstatement.h
        terminatestatement.cc
        terminatestatement.h
        whilestatement.cc
        whilestatement.h
    )
fips_dir(ast/types)
    fips_files(
        type.cc
        type.h
    )

fips_dir(generators)
    fips_files(
        generator.cc
        generator.h
        glslgenerator.cc
        glslgenerator.h
        hgenerator.cc
        hgenerator.h
        hlslgenerator.cc
        hlslgenerator.h
        spirvgenerator.cc
        spirvgenerator.h
    )
fips_dir(validators)
    fips_files(
        validator.cc
        validator.h
    )
fips_dir(generated)
    fips_files(
        intrinsics.cc
        intrinsics.h
        swizzle.h
        spirv.h
        types.cc
        types.h
    )

fips_deps(gpulang_util antlr4 glslang)
SET_DEFAULT_OUTPUT(gpulang_compiler)

IF (MSVC11)
    ADD_DEFINITIONS(-D_XKEYCHECK_H)
ENDIF()

fips_end_lib()
	
