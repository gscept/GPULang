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
	args.add_params({ "-i", "-o", "-r", "-h", "-g" });
	args.parse(argv);

	if (args["--help"])
	{
		this->PrintHelp();
		return false;
	}

	this->mode = args["M"];

	if (!this->shaderCompiler.SetFlag(args["optimize"] || args["Ox"] ? SingleShaderCompiler::Optimize : 0))
	{
		this->PrintHelp();
		return false;
	}
	if (!this->shaderCompiler.SetFlag(args["symbols"] || args["s"] ? SingleShaderCompiler::Symbols : 0))
	{
		this->PrintHelp();
		return false;
	}
	this->shaderCompiler.SetFlag(args["quiet"] || args["q"] ? SingleShaderCompiler::Quiet : 0);
	this->shaderCompiler.SetFlag(args["validate"] || args["v"] ? SingleShaderCompiler::Validate : 0);
	this->shaderCompiler.SetFlag(args["profile"] || args["p"] ? SingleShaderCompiler::Profile : 0);
	uint32_t defaultGroup;
	if (args("g") >> defaultGroup)
		this->shaderCompiler.SetDefaultGroup(defaultGroup);
	std::string buffer;
	if (args("o") >> buffer)
	{
		this->shaderCompiler.SetDstBinary(buffer);
	}
	else if (this->mode)
	{
		fprintf(stderr, "gpulangc error: no output file specified while trying to create dependencies\n");
		this->PrintHelp();
		return false;
	}

    if (args("r") >> buffer)
    {
        this->shaderCompiler.SetRootDir(buffer);
    }	

	if (!(args("i") >> buffer))
	{
		fprintf(stderr, "gpulangc error: no input file specified\n");
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
usage: gpulangc [-M] [--help] [-i <file>] [-I <path>]\n\
                     [-o <path>] [-h <path>] [-q] [-debug] [-profile] [-validate]\n\
\n\
-M       		Create dependencies\n\
--help   		Print this message\n\
-i       		Path to input file\n\
-I       		Where to search for include headers. This can be repeated multiple times.\n\
-o       		Where to output the binaries. If folder, outputs both binaries and headers to this folder unless -h is provided.\n\
-h       		Where to output generated C headers.\n\
-g <value>      Default binding group if none is provided.\n\
-quiet/-q  		Suppress standard output.\n\
-optimize/-Ox  	Optimize output (not allowed with -symbols/-s).\n\
-symbols/-s		Generate debug symbols (not allowed with -optimize/-Ox).\n\
-profile/-p		Log compilation times.\n\
-validate/-v	Validate compilation output.\n\
";

	fprintf(stdout, help);
}


SystemSetup dummy;

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
