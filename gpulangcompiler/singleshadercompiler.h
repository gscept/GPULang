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
#include <cstdint>


class SingleShaderCompiler
{
public:

	enum Flags : uint32_t
	{
		Optimize = 0x1,
		Quiet = 0x2,
		Validate = 0x4,
		Profile = 0x8,
		Symbols = 0x10,
        Debug = 0x20
	};

	/// constructor
	SingleShaderCompiler();
	/// destructor
	~SingleShaderCompiler();

	/// set source directory for base shaders
	void AddIncludeDir(const std::string& incDir);    
	/// set destination directory
	void SetDstBinary(const std::string& dstBinary);
	/// set output header directory
	void SetDstHeader(const std::string& dstHeader);
    /// set the root dir
    void SetRootDir(const std::string& rootDir);
    /// Set the targets
    void SetTargets(const std::string& targets);
			
	/// set additional command line params
	void SetAdditionalParams(const std::string& params);
	/// Append flag bit
	bool SetFlag(const uint32_t f);
	/// Set default group binding
	void SetDefaultGroup(const uint8_t g);

	/// compile shader
	bool CompileShader(const std::string& src);

	/// calculate include dependencies
	bool CreateDependencies(const std::string& src);
	
private:
	
	/// compiles shaders for SPIRV
	bool CompileSPIRV(const std::string& src);
	std::string dstBinary;
	std::string dstHeader;
    std::string rootDir;
    std::string targets;
	uint32_t flags;
	uint8_t defaultGroup = 0;
	std::string additionalParams;
	std::vector<std::string> includeDirs;
}; 

///------------------------------------------------------------------------------
/**
*/
inline void
SingleShaderCompiler::SetDstBinary(const std::string& d)
{
	this->dstBinary = d;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
SingleShaderCompiler::SetDstHeader(const std::string& dstHeader)
{
	this->dstHeader = dstHeader;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
SingleShaderCompiler::SetRootDir(const std::string& rootDir)
{
	this->rootDir = rootDir;
}

//------------------------------------------------------------------------------
/**
*/
inline void
SingleShaderCompiler::SetTargets(const std::string& targets)
{
    this->targets = targets;
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
SingleShaderCompiler::SetAdditionalParams(const std::string& p)
{
	this->additionalParams = p;
}

//------------------------------------------------------------------------------
