#pragma once
//------------------------------------------------------------------------------
/**
    @class ShaderCompilerApp

    Application class for the anyfxcompiler tool.
        
    (C) 2019 Individual contributors, see AUTHORS file
*/

#include "singleshadercompiler.h"

//------------------------------------------------------------------------------
class ShaderCompilerApp
{
public:
    /// run the application
    bool Run();

    /// parse command line arguments
    bool ParseCmdLineArgs(const char ** argv);    

    void PrintHelp();

    SingleShaderCompiler shaderCompiler;
    std::string src;
	bool mode;
};

//------------------------------------------------------------------------------

    