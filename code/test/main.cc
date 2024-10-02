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
    bool res = GPULangCompile(input, GPULang::Compiler::Language::SPIRV, binaryOut, headerOut, {}, {}, errors);

    if (!res)
        printf("%s Compilation failed\n", input.c_str());

    if (errors != nullptr)
    {
        printf("%s\n", errors->buffer);
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
    TEST(nestedtest);
    TEST(explicitreturntest);
    TEST(switchtest);
    
    TEST(whiletest);
    TEST(fortest);
    TEST(breaktest);
    TEST(iftest);
}