#pragma once
#include "antlr4-runtime.h"
#include "antlr4-common.h"
#include "BaseErrorListener.h"

class AnyFXLexerHandler : public antlr4::BaseErrorListener
{
private:
	virtual void syntaxError(antlr4::Recognizer* recognizer, antlr4::Token* offendingSymbol, size_t line, size_t charPositionInLine,
		const std::string& msg, std::exception_ptr e) override;
public:
	bool hasError = false;
	std::string errorBuffer;
	std::string warningBuffer;
	std::vector<std::tuple<int, size_t, size_t, size_t, std::string>> lines;
};

class AnyFXParserHandler : public antlr4::BaseErrorListener
{
private:
	virtual void syntaxError(antlr4::Recognizer* recognizer, antlr4::Token* offendingSymbol, size_t line, size_t charPositionInLine,
		const std::string& msg, std::exception_ptr e) override;
public:
	bool hasError = false;
	std::string errorBuffer;
	std::string warningBuffer;
	std::vector<std::tuple<int,size_t, size_t, size_t, std::string>> lines;
};