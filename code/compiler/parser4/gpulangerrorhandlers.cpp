#include "gpulangerrorhandlers.h"

#include "GPULangParser.h"
#include "gpulangtoken.h"
#include "util.h"

void GPULangLexerErrorHandler::syntaxError(antlr4::Recognizer* recognizer, antlr4::Token* offendingSymbol, size_t line, size_t charPositionInLine, const std::string& msg, std::exception_ptr e)
{
    // find the next parsed row which comes after the token
    GPULangToken* gpuLangToken = static_cast<GPULangToken*>(offendingSymbol);

    std::string file = std::get<2>(GPULangParser::LineStack.back());
    size_t correctLine = std::get<1>(GPULangParser::LineStack.back()) + line - 1 - std::get<0>(GPULangParser::LineStack.back());

    this->errorBuffer += GPULang::Format("%s(%d): syntax error: %s\n", file.c_str(), correctLine, msg.c_str());
    this->diagnostics.Append(GPULang::Diagnostic{ .error = msg.c_str(), .file = file.c_str(), .line = (int)correctLine, .column = (int)charPositionInLine });
    this->hasError = true;
}

void GPULangParserErrorHandler::syntaxError(antlr4::Recognizer* recognizer, antlr4::Token* offendingSymbol, size_t line, size_t charPositionInLine, const std::string& msg, std::exception_ptr e)
{
	// find the next parsed row which comes after the token
	GPULangToken* gpuLangToken = static_cast<GPULangToken*>(offendingSymbol);

	std::string file = std::get<2>(GPULangParser::LineStack.back());
	size_t correctLine = std::get<1>(GPULangParser::LineStack.back()) + line - 1 - std::get<0>(GPULangParser::LineStack.back());

	this->errorBuffer += GPULang::Format("%s(%d): syntax error: %s\n", file.c_str(), correctLine, msg.c_str());
    this->diagnostics.Append(GPULang::Diagnostic{ .error = msg.c_str(), .file = file.c_str(), .line = (int)correctLine, .column = (int)charPositionInLine });
	this->hasError = true;
}
