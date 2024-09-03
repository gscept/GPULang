//------------------------------------------------------------------------------
//	cmdlineargs.cc
//  2012 Gustav Sterbrant
//------------------------------------------------------------------------------

#include "cmdlineargs.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
CmdLineArgs::CmdLineArgs(int argc, const char** argv)
{
    int i;
    for (i = 0; i < argc; i++)
    {
        std::string arg(argv[i]);
        this->arguments.push_back(arg);
    }
}

//------------------------------------------------------------------------------
/**
*/
bool
CmdLineArgs::HasArgument(const std::string& key)
{
    return (this->FindIndex(key) != NOT_FOUND);
}

//------------------------------------------------------------------------------
/**
*/
const std::string&
CmdLineArgs::GetArgument(const std::string& key)
{
    int index = this->FindIndex(key);
    assert(index != NOT_FOUND);
    return this->arguments[index+1];
}

//------------------------------------------------------------------------------
/**
*/
std::vector<std::string>
CmdLineArgs::GetArguments(const std::string& key)
{
    std::vector<std::string> args;
    unsigned i;
    for (i = 0; i < this->arguments.size(); i++)
    {
        if (this->arguments[i].substr(0, key.size()) == key)
        {
            args.push_back(this->arguments[i]);
        }
    }
    return args;
}

//------------------------------------------------------------------------------
/**
*/
const std::vector<std::string>& 
CmdLineArgs::GetArguments() const
{
    return this->arguments;
}

//------------------------------------------------------------------------------
/**
*/
bool
CmdLineArgs::GetBool(const std::string& key)
{
    int index = this->FindIndex(key);
    if (index == NOT_FOUND)
        return false;

    const std::string& value = this->arguments[index+1];
    if (value.compare("true") == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

//------------------------------------------------------------------------------
/**
*/
int
CmdLineArgs::GetInt(const std::string& key)
{
    int index = this->FindIndex(key);
    assert(index != NOT_FOUND);
    return atoi(this->arguments[index+1].c_str());
}

//------------------------------------------------------------------------------
/**
*/
int
CmdLineArgs::FindIndex(const std::string& key)
{
    unsigned i;
    for (i = 0; i < this->arguments.size(); i++)
    {
        if (this->arguments[i].compare(key) == 0)
        {
            return i;
        }
    }
    return NOT_FOUND;
}

} // namespace GPULang