//------------------------------------------------------------------------------
//  @file main.cc
//  @copyright (C) 2021 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "gpulangcompiler.h"
#include <stdlib.h>

void CompleteShaderTest();
void ApiTest();
void ConstantBufferTest();
void ReadWriteBufferTest();


#define TEST(path)\
    ShaderTest(std::string(TEST_FOLDER) + "/" + #path + ".gpul", std::string(TEST_OUTPUT_FOLDER) + "/" + #path + ".gplb", std::string(TEST_OUTPUT_FOLDER) + "/" + #path + ".h");

//------------------------------------------------------------------------------
/**
*/
void
ShaderTest(std::string input, std::string binaryOut, std::string headerOut)
{
    GPULangErrorBlob* errors;
    GPULangCompile(input, GPULang::Compiler::Language::SPIRV, binaryOut, headerOut, "GPULang", {}, {}, errors);

    if (errors != nullptr)
    {
        printf(errors->buffer);
        return;
    }
}


//------------------------------------------------------------------------------
/**
*/
void 
main(const char** argv, int argc)
{
    //CompleteShaderTest();

    TEST(fortest);
    TEST(iftest);
    TEST(whiletest);
}