//------------------------------------------------------------------------------
//  shadercompilerapp.cc
//  (C) 2019 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------

#include "shadercompilerapp.h"
#include "argh.h"
#include <filesystem>


//------------------------------------------------------------------------------
/**
*/
bool
ShaderCompilerApp::ParseCmdLineArgs(const char ** argv)
{
	argh::parser args;
	args.add_params({ "-i", "-o", "-r", "-h" });
	args.parse(argv);

	if (args["--help"])
	{
		this->PrintHelp();
		return false;
	}

	this->mode = args["M"];

	this->shaderCompiler.SetDebugFlag(args["debug"]);
	this->shaderCompiler.SetQuietFlag(args["q"]);
	std::string buffer;
	if (args("o") >> buffer)
	{
		this->shaderCompiler.SetDstBinary(buffer);
	}
	else if (this->mode)
	{
		fprintf(stderr, "anyfxcompiler error: no output file specified while trying to create dependecies\n");
		this->PrintHelp();
		return false;
	}

    if (args("r") >> buffer)
    {
        this->shaderCompiler.SetRootDir(buffer);
    }	

	if (!(args("i") >> buffer))
	{
		fprintf(stderr, "anyfxcompiler error: no input file specified\n");
		this->PrintHelp();
		return false;
	}

	this->src = buffer;
	if (args("h") >> buffer)
	{
		this->shaderCompiler.SetDstHeader(buffer);
	}

    // find include dir args
	const std::vector<std::string> &allargs = args.args();

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
bool
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

	return success;
}

//------------------------------------------------------------------------------
/**
*/
void
ShaderCompilerApp::PrintHelp()
{
	const char* help = "\
usage: gpulangcompiler [-M] [--help] [-i <file>] [-I <path>]\n\
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
	
	if (app.ParseCmdLineArgs(argv))
	{
		return app.Run() ? 0 : 1;
	}
	return -1;
}
