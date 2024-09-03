#pragma once
//------------------------------------------------------------------------------
/**
    @class AnyFX::TextWriter
    
    Outputs raw text
    
    (C) 2019 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include <iostream>
#include <fstream>
namespace AnyFX
{
class TextWriter
{
public:
	/// constructor
	TextWriter();
	/// destructor
	virtual ~TextWriter();

	/// set output stream
	void SetPath(const std::string& path);
	/// get output stream
	const std::string& GetPath() const;
	/// opens writer
	bool Open();
	/// closes writer
	void Close();
	/// return true if writer is open
	bool IsOpen() const;

	void WriteString(const std::string& s);
	/// write character
	void WriteChar(char c);

private:
	std::string path;
	std::ofstream output;
	bool isOpen;
}; 

//------------------------------------------------------------------------------
/**
*/
inline void
TextWriter::SetPath(const std::string& path)
{
	this->path = path;
}

//------------------------------------------------------------------------------
/**
*/
inline const std::string&
TextWriter::GetPath() const
{
	return this->path;
}

//------------------------------------------------------------------------------
/**
*/
inline bool 
TextWriter::IsOpen() const
{
	return this->isOpen;
}

} // namespace AnyFX
//------------------------------------------------------------------------------