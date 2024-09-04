//------------------------------------------------------------------------------
//  @file completeshadertest.cc
//  @copyright (C) 2021 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "gpulangcompiler.h"
#include "loader.h"
#include <stdlib.h>
#include <string>
//------------------------------------------------------------------------------
/**
*/
void 
CompleteShaderTest()
{
    GPULangErrorBlob* errors;
    GPULangCompile(std::string(TEST_FOLDER) + "/completeshader.fx", GPULang::Compiler::Language::SPIRV, std::string(TEST_OUTPUT_FOLDER) + "/completeshader.fxb", std::string(TEST_OUTPUT_FOLDER) + "/completeshader.h", "khronos", {}, {}, errors);

    if (errors != nullptr)
    {
        printf(errors->buffer);
        return;
    }

    // try to load
    FILE* file = fopen((std::string(TEST_OUTPUT_FOLDER) + "/completeshader.fxb").c_str(), "r");
    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    rewind(file);

    char* buf = new char[size];
    fread(buf, 1, size, file);
    GPULang::Loader loader;
    loader.Load(buf, size);
}
