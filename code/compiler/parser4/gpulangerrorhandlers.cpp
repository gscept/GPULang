#include "gpulangerrorhandlers.h"

#include "GPULangParser.h"
#include "gpulangtoken.h"
#include "util.h"

void GPULangLexerErrorHandler::syntaxError(antlr4::Recognizer* recognizer, antlr4::Token* offendingSymbol, size_t line, size_t charPositionInLine, const std::string& msg, std::exception_ptr e)
{
	// find the next parsed row which comes after the token
	int loop = 0;
	int tokenLine = line;
	while (loop < this->lines.size() - 1)
	{
		auto temp = this->lines[loop];
		if (std::get<1>(temp) > tokenLine)
		{
			loop--;
			break;
		}
		loop++;
	}

	auto tu2 = this->lines[loop];
	std::string file = std::get<4>(tu2);
	file = file.substr(1, file.length() - 2); // remove trailing "
	int correctedLine = std::get<0>(tu2);

	this->errorBuffer += GPULang::Format("%s(%d): error: %s\n", file.c_str(), correctedLine + line, msg.c_str());
    this->diagnostics.push_back(GPULang::Diagnostic{ .error = msg, .file = file, .line = correctedLine, .column = (int)charPositionInLine });
	this->hasError = true;
}

void GPULangParserErrorHandler::syntaxError(antlr4::Recognizer* recognizer, antlr4::Token* offendingSymbol, size_t line, size_t charPositionInLine, const std::string& msg, std::exception_ptr e)
{
	// find the next parsed row which comes after the token
	GPULangToken* gpuLangToken = static_cast<GPULangToken*>(offendingSymbol);

	std::string file = std::get<2>(GPULangParser::LineStack.back());
	size_t correctLine = std::get<1>(GPULangParser::LineStack.back()) + line - 1 - std::get<0>(GPULangParser::LineStack.back());

	this->errorBuffer += GPULang::Format("%s(%d): syntax error: %s\n", file.c_str(), correctLine, msg.c_str());
    this->diagnostics.push_back(GPULang::Diagnostic{ .error = msg, .file = file, .line = (int)correctLine, .column = (int)charPositionInLine });
	this->hasError = true;
}
