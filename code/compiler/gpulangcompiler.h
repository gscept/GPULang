//------------------------------------------------------------------------------
/**
    AnyFX compiler header
    
    (C) 2013 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include <string>
#include <vector>
#include "compiler.h"

struct GPULangErrorBlob
{
	char* buffer;
	size_t size;

	// constructor
	GPULangErrorBlob() :
		buffer(0),
		size(0)
	{
		// empty
	}

	// destructor
	~GPULangErrorBlob()
	{
		if (buffer) delete [] buffer;
	}
};

extern std::vector<std::string> GPULangGenerateDependencies(const std::string& file, const std::vector<std::string>& defines);
extern bool GPULangCompile(const std::string& file, GPULang::Compiler::Language target, const std::string& output, const std::string& header_output, const std::string& vendor, const std::vector<std::string>& defines, const std::vector<std::string>& flags, GPULangErrorBlob*& errorBuffer);
