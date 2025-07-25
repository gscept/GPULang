//------------------------------------------------------------------------------
/**
    GPULang compiler include header
    
    (C) 2024 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#pragma once
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


static auto GPULangValidIdentifierStart = [](const char c) -> bool
{
    if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c == '_'))
        return true;
    return false;
};

static auto GPULangValidIdentifierChar = [](const char c) -> bool
{
    if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || (c == '_'))
        return true;
    return false;
};


struct GPULangServerResult
{
    GPULang::Effect* root = nullptr;
	GPULang::PinnedArray<GPULang::Symbol*> symbols;
	GPULang::Scope* intrinsicScope = nullptr, *mainScope = nullptr;
    GPULang::PinnedMap<std::string, GPULang::Symbol*> lookup;
    GPULang::PinnedArray<GPULang::Diagnostic> diagnostics = 0xFFF;
	GPULang::PinnedArray<GPULang::FixedString> messages = 0xFF;
};

struct GPULangFile
{
    GPULang::FixedString path;
    char* contents = nullptr;
    size_t contentSize = 0;
    bool consumed = false;
    int lines = 0;

    GPULangFile() {};

    ~GPULangFile()
    {
        if (this->contents != nullptr)
            free(this->contents);
        this->contentSize = 0;
    }

    GPULangFile(GPULangFile&& rhs) noexcept
    {
        this->path = std::move(rhs.path);
        this->contents = rhs.contents;
        this->contentSize = rhs.contentSize;
        rhs.contents = nullptr;
        rhs.contentSize = 0;
    }

    void operator=(GPULangFile&& rhs) noexcept
    {
        this->path = std::move(rhs.path);
        this->contents = rhs.contents;
        this->contentSize = rhs.contentSize;
        
        rhs.contents = nullptr;
        rhs.contentSize = 0;
    }

};

extern std::vector<std::string> GPULangGenerateDependencies(const std::string& file, const std::vector<std::string>& defines);
extern bool GPULangCompile(const std::string& file, GPULang::Compiler::Language target, const std::string& output, const std::string& header_output, const std::vector<std::string>& defines, GPULang::Compiler::Options options, GPULangErrorBlob*& errorBuffer);
extern bool GPULangValidate(GPULangFile*, GPULang::Compiler::Language , const std::vector<std::string>&, GPULang::Compiler::Options, GPULangServerResult&);
extern GPULangFile* GPULangLoadFile(const std::string_view& path);
