//------------------------------------------------------------------------------
//  shaderc.cc
//  (C) 2018 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "singleshadercompiler.h"
#include <filesystem>
#include "gpulangcompiler.h"

#ifdef __APPLE__
namespace fs = std::__fs::filesystem;
#else
namespace fs = std::filesystem;
#endif

//------------------------------------------------------------------------------
/**
*/
SingleShaderCompiler::SingleShaderCompiler() :
	language("SPIRV"),        	
	platform("win32"),
	flags(0x0),
	defaultSet(3)
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
SingleShaderCompiler::~SingleShaderCompiler()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
bool
SingleShaderCompiler::SetFlag(const uint32_t f)
{
	this->flags |= f;
	if (this->flags & Optimize && this->flags & Symbols)
	{
		fprintf(stderr, "[gpulangc] error: -optimize/-Ox not allowed with -symbols/-s\n");
		return false;
	}
	return true;
}

//------------------------------------------------------------------------------
/**
*/
bool 
SingleShaderCompiler::CompileShader(const std::string& src)
{
	// check if source
	if (!fs::exists(src))
	{
		fprintf(stderr, "[gpulangc] error: shader source '%s' not found!\n", src.c_str());
		return false;
	}

	// make sure the target directory exists
	if (!this->dstBinary.empty())
    {
    	fs::path sp(this->dstBinary);
		fs::create_directories(sp.parent_path());

		// If output is just a folder, then assume binary output is .gplb for the binary and .h for the header to that folder 
		if (is_directory(sp))
		{
			fs::path source(src);
			
			this->dstBinary = sp.parent_path().string() + "/" + source.stem().string() + ".gplb";
			if (this->dstHeader.empty())
			{
				this->dstHeader = sp.parent_path().string() + "/" + source.stem().string() + ".h";
			}
		}
    }
	if (!this->dstHeader.empty())
    {
        fs::path sp(this->dstHeader);
		fs::create_directories(sp.parent_path());
    }
	return this->CompileSPIRV(src);	
}

//------------------------------------------------------------------------------
/**
*/
bool
SingleShaderCompiler::CompileSPIRV(const std::string& src)
{
	fs::path sp(src);
    
	std::string file = sp.stem().string();
    std::string folder = sp.parent_path().string();
	
	if (this->dstBinary.empty())
	{
		if (!(this->flags & Flags::Quiet))
		{
			fprintf(stderr, "[gpulangc] \n Checking for problems:\n   %s\n", src.c_str());
		}
	}
	else
	{
		// compile
		if (!(this->flags & Flags::Quiet))
		{
			fprintf(stderr, "[gpulangc] \n Compiling:\n   %s -> %s", src.c_str(), this->dstBinary.c_str());
            if (!this->dstHeader.empty())
			    fprintf(stderr,"          \n Generating:\n   %s -> %s\n", src.c_str(), this->dstHeader.c_str());
		}
	}
    
    std::vector<std::string> defines;
    std::vector<std::string> flags;
	GPULang::Compiler::Options options;

    // first include this folder
	std::string define = "-I" + folder + "/";
    defines.push_back(define);

    for (auto inc : this->includeDirs)
    {
		define = "-I" + inc + "/";
        defines.push_back(define);
    }
	
    // if using debug, output raw shader code
	options.quiet = this->flags & Flags::Quiet ? 1 : 0; 
	options.optimize = this->flags & Flags::Optimize ? 1 : 0;
	options.emitTimings = this->flags & Flags::Profile ? 1 : 0;
	options.validate = this->flags & Flags::Validate ? 1 : 0;
	options.symbols = this->flags & Flags::Symbols ? 1 : 0;
	options.errorFormat = GPULang::Compiler::ErrorFormat::MSVC;

    GPULangErrorBlob* errors = NULL;

    // this will get the highest possible value for the GL version, now clamp the minor and major to the one supported by glew
    int major = 1;
    int minor = 0;
	
	fs::path escapedDst(this->dstBinary);
	fs::path escapedHeader(this->dstHeader);

    bool res = GPULangCompile(
        sp.string()
        , GPULang::Compiler::Language::VULKAN_SPIRV
        , escapedDst.string()
        , escapedHeader.string()
        , defines
        , options
        , errors);
    if (!res)
    {
        if (errors)
        {
            fprintf(stderr,"%s\n", errors->buffer);
            delete errors;
            errors = 0;
        }
        return false;
    }
    else if (errors)
    {
        fprintf(stderr, "%s\n", errors->buffer);
        delete errors;
        errors = 0;
    }
    return true;
}


//------------------------------------------------------------------------------
/**
*/
bool
SingleShaderCompiler::CreateDependencies(const std::string& src)
{

    if (!this->dstBinary.empty())
    {
    	fs::path sp(this->dstBinary);
		fs::create_directories(sp.parent_path());
    }

	fs::path sp(src);
	std::string file = sp.stem().string();
	std::string folder = sp.parent_path().string();

	// format destination
	std::string destFile = fs::absolute(folder + "/" + file + ".dep").string();

	// compile
	if (!(this->flags & Flags::Quiet))
	{
		fprintf(stderr, "[gpulangc] \n Analyzing:\n   %s -> %s\n", src.c_str(), destFile.c_str());	
	}

	std::vector<std::string> defines;
	std::vector<std::string> flags;
	std::string define = "-D GLSL";
	defines.push_back(define);

	// first include this folder
	define = "-I" + folder + "/";
	defines.push_back(define);

	for (auto inc : this->includeDirs)
	{
		define = "-I" + inc + "/";
		defines.push_back(define);
	}

#pragma warning (disable:4996)
	FILE * output = fopen(this->dstBinary.c_str(), "w");
	if(output)
	{
		std::vector<std::string> deps = GPULangGenerateDependencies(sp.string(), defines);
        for(auto str : deps)
        {
			fprintf(output, "%s;", str.c_str());
        }
		fclose(output);
	}
    
    return true;
}
