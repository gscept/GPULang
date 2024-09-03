#pragma once
//------------------------------------------------------------------------------
/**
    @class ToolkitUtil::ShaderC
    
	Compiles a single shader.
    
    (C) 2018 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------

#include <string>
#include <vector>

class SingleShaderCompiler
{
public:

	/// constructor
	SingleShaderCompiler();
	/// destructor
	~SingleShaderCompiler();

	/// set target platform
	void SetPlatform(const std::string& platform);
	/// sets the source language
	void SetLanguage(const std::string& lang);
	/// set source directory for base shaders
	void AddIncludeDir(const std::string& incDir);    
	/// set destination directory
	void SetDstDir(const std::string& dstDir);
	/// set output header directory
	void SetHeaderDir(const std::string& headerDir);
			
	/// set debugging flag
	void SetDebugFlag(bool b);
	/// set additional command line params
	void SetAdditionalParams(const std::string& params);
	/// set quiet flag
	void SetQuietFlag(bool b);

	/// compile shader
	bool CompileShader(const std::string& src);

	/// calculate include dependencies
	bool CreateDependencies(const std::string& src);
	
	///
	void SetDefaultSet(int size);
private:
	
	/// compiles shaders for SPIRV
	bool CompileSPIRV(const std::string& src);
	int defaultSet;
	std::string platform;	
	std::string dstDir;
	std::string headerDir;
	std::string language;
	bool quiet;
	bool debug;
	std::string additionalParams;
	std::vector<std::string> includeDirs;
}; 

//------------------------------------------------------------------------------
/**
*/
inline void
SingleShaderCompiler::SetPlatform(std::string const & p)
{
	this->platform = p;
}

//------------------------------------------------------------------------------
/**
*/
inline void
SingleShaderCompiler::SetDefaultSet(int size)
{
	this->defaultSet = size;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
SingleShaderCompiler::SetLanguage( const std::string& lang )
{
	this->language = lang;
}

///------------------------------------------------------------------------------
/**
*/
inline void
SingleShaderCompiler::SetDstDir(const std::string& d)
{
	this->dstDir = d;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
SingleShaderCompiler::SetHeaderDir(const std::string& headerDir)
{
	this->headerDir = headerDir;
}

//------------------------------------------------------------------------------
/**
*/
inline void
SingleShaderCompiler::AddIncludeDir(const std::string& d)
{
	this->includeDirs.push_back(d);
}


//------------------------------------------------------------------------------
/**
*/
inline void
SingleShaderCompiler::SetDebugFlag(bool b)
{
	this->debug = b;
}

//------------------------------------------------------------------------------
/**
*/
inline void
SingleShaderCompiler::SetAdditionalParams(const std::string& p)
{
	this->additionalParams = p;
}

//------------------------------------------------------------------------------
/**
*/
inline void
SingleShaderCompiler::SetQuietFlag(bool b)
{
	this->quiet = b;
}

//------------------------------------------------------------------------------