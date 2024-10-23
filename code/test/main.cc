//------------------------------------------------------------------------------
//  @file main.cc
//  @copyright (C) 2021 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "gpulangcompiler.h"
#include <stdlib.h>


#define TEST(path)\
    ShaderTest(std::string(TEST_FOLDER) + "/" + #path + ".gpul", std::string(TEST_OUTPUT_FOLDER) + "/" + #path + ".gplb", std::string(TEST_OUTPUT_FOLDER) + "/" + #path + ".h");

//------------------------------------------------------------------------------
/**
*/
void
ShaderTest(std::string input, std::string binaryOut, std::string headerOut)
{
    GPULangErrorBlob* errors;
    GPULang::Compiler::Options options;
    options.emitTimings = true;
    options.disallowImplicitConversion = true;
    options.validate = true;
    bool res = GPULangCompile(input, GPULang::Compiler::Language::VULKAN_SPIRV, binaryOut, headerOut, {}, options, errors);

    if (!res)
        printf("%s Compilation failed\n", input.c_str());
    else
        printf("%s Compilation successful!\n", input.c_str());

    if (errors != nullptr)
    {
        printf("%s\n", errors->buffer);
        return;
    }
}


//------------------------------------------------------------------------------
/**
*/
int 
main(const char** argv, int argc)
{
    //CompleteShaderTest();
    TEST(computewithstore);
    TEST(basicgraphics);
    TEST(computewithimagestore);
    /*
    TEST(for);
    TEST(explicitreturn);
    TEST(nested);
    TEST(typeinference);
    TEST(switch);
    
    TEST(while);
    TEST(break);
    TEST(if);
    */
    return 0;
}