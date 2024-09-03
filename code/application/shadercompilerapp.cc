//------------------------------------------------------------------------------
//  shadercompilerapp.cc
//  (C) 2019 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------

#include "shadercompilerapp.h"
#include "cmdlineargs.h"
#include "argh.h"
#include <filesystem>


//------------------------------------------------------------------------------
/**
*/
bool
ShaderCompilerApp::ParseCmdLineArgs(int argc, const char ** argv)
{
    AnyFX::CmdLineArgs args(argc, argv);
    
	if (args.HasArgument("--help"))
	{
		this->PrintHelp();
		return false;
	}

	this->mode = args.GetBool("M");

	this->shaderCompiler.SetDebugFlag(args.GetBool("debug"));
	this->shaderCompiler.SetQuietFlag(args.GetBool("q"));
	std::string buffer;
	if (args.HasArgument("o"))
	{
		this->shaderCompiler.SetDstDir(args.GetArgument("o"));
	}
	else if (this->mode)
	{
		fprintf(stderr, "anyfxcompiler error: no output file specified while trying to create dependecies\n");
		this->PrintHelp();
		return false;
	}
	

	if (!args.HasArgument("i"))
	{
		fprintf(stderr, "anyfxcompiler error: no input file specified\n");
		this->PrintHelp();
		return false;
	}

	this->src = args.GetArgument("i");
	if (args.HasArgument("h"))
	{
		this->shaderCompiler.SetHeaderDir(args.GetArgument("h"));
	}

    // find include dir args
	const std::vector<std::string> &allargs = args.GetArguments();

    for (int i = 0; i < allargs.size(); i++)
    {
         if (allargs[i] == "-I" && i+1 < allargs.size())
         {
			 this->shaderCompiler.AddIncludeDir(allargs[++i]);
         }
    }
            
    return true;
}

//------------------------------------------------------------------------------
/**
*/
void
ShaderCompilerApp::Run()
{   
	bool success = false;
            
    if(this->mode)
    {
        success = this->shaderCompiler.CreateDependencies(this->src);
    }
    else
    {
        success = this->shaderCompiler.CompileShader(this->src);
    }       
}

//------------------------------------------------------------------------------
/**
*/
void
ShaderCompilerApp::PrintHelp()
{
	const char* help = "\
usage: anyfxcompiler [-M] [--help] [-i <file>] [-I <path>]\n\
                     [-o <path>] [-h <path>] [-q] [-debug]\n\
\n\
-M       Create dependencies\n\
--help   Print this message\n\
-i       Path to input file\n\
-I       Where to search for include headers. This can be repeated multiple times.\n\
-o       Where to output the binaries\n\
-h       Where to place generated C headers\n\
-q     	 Suppress standard output\n\
-debug   Generate debugging information\n\
";

	fprintf(stdout, help);
}



//------------------------------------------------------------------------------
/**
*/
int __cdecl
main(int argc, const char** argv)
{
	ShaderCompilerApp app;
	
	if (app.ParseCmdLineArgs(argc, argv))
	{
		app.Run();
	}
}
