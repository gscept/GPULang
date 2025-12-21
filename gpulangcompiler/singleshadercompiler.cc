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
	flags(0x0)
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
void
SingleShaderCompiler::SetDefaultGroup(const uint8_t g)
{
	this->defaultGroup = g;
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
        try
        {
            fs::create_directories(sp.parent_path());
            if (this->flags & Flags::Debug)
            {
                sp.replace_extension("");
                sp.replace_filename(sp.filename().string() + "_debug");
                fs::create_directories(sp);
            }

        } catch (const fs::filesystem_error&)
        {
            fprintf(stderr, "[gpulangc] error: could not create output directory '%s'\n", sp.parent_path().string().c_str());
            return false;
        }

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
			fprintf(stdout, "[gpulangc] \n Validating:\n   %s\n", src.c_str());
		}
	}
	else
	{
		// compile
		if (!(this->flags & Flags::Quiet))
		{
			fprintf(stdout, "[gpulangc] \n %s -> %s\n", src.c_str(), this->dstBinary.c_str());
            if (!this->dstHeader.empty())
			    fprintf(stdout,"          \n %s -> %s\n", src.c_str(), this->dstHeader.c_str());
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
    options.debugSymbols = this->flags & Flags::Symbols ? 1 : 0;
	options.errorFormat = GPULang::Compiler::ErrorFormat::MSVC;
    options.debugInfo = this->flags & Flags::Debug ? 1 : 0;
	options.defaultGroupBinding = this->defaultGroup;

    GPULangErrorBlob* errors = NULL;

    // this will get the highest possible value for the GL version, now clamp the minor and major to the one supported by glew
    int major = 1;
    int minor = 0;
	
	fs::path escapedDst(this->dstBinary);
	fs::path escapedHeader(this->dstHeader);

    GPULang::Allocator GlobalStringAllocator = GPULang::CreateAllocator();
    GPULang::InitAllocator(&GlobalStringAllocator);

    GPULang::StringAllocator = &GlobalStringAllocator;    
    GPULangFile* f = GPULangLoadFile(sp.string());

    bool res = GPULangCompile(
          f
        , GPULang::Compiler::Language::VULKAN_SPIRV
        , escapedDst.string()
        , escapedHeader.string()
        , defines
        , options
        , errors);

    GPULang::ResetAllocator(&GlobalStringAllocator);
    
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

	// compile
	if (!(this->flags & Flags::Quiet))
	{
        fprintf(stdout, "[gpulangc] \n Analyzing:\n   %s\n", src.c_str());
	}

	std::vector<std::string> defines;
	std::vector<std::string> flags;
	std::string define = "";

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

    GPULang::Allocator GlobalStringAllocator = GPULang::CreateAllocator();
    GPULang::InitAllocator(&GlobalStringAllocator);

    GPULang::StringAllocator = &GlobalStringAllocator;
    GPULangFile* sourceFile = GPULangLoadFile(src);
	if (sourceFile != nullptr && output != nullptr)
	{
        GPULang::Allocator alloc = GPULang::CreateAllocator();
        GPULang::InitAllocator(&alloc);
        GPULang::MakeAllocatorCurrent(&alloc);
        
        GPULang::PinnedArray<GPULangDiagnostic> diagnostics(0xFFF);
        GPULang::FixedArray<GPULang::FixedString> deps = GPULangGenerateDependencies(sourceFile, defines, diagnostics);
        bool hasError = false;
        for (const auto& diag : diagnostics)
        {
            if (diag.severity == GPULangDiagnostic::Severity::Error)
            {
                printf("Generation dependency failed with: %s", diag.error.c_str());
                hasError = true;
            }
        }
        if (hasError)
            return false;
        
        for(auto str : deps)
        {
			fprintf(output, "%s;", str.c_str());
        }
        fclose(output);
        GPULang::ResetAllocator(&alloc);
	}
    
    return true;
}
