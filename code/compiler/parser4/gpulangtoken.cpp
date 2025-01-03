#include "gpulangtoken.h"
#include "GPULangParser.h"

TokenFactory<CommonToken>* GPULangTokenFactory::DEFAULT = new GPULangTokenFactory;

GPULangTokenFactory::GPULangTokenFactory(){
}

std::unique_ptr<CommonToken> GPULangTokenFactory::create(std::pair<TokenSource*, CharStream*> source, size_t type, const std::string& text, size_t channel, size_t start, size_t stop, size_t line, size_t charPositionInLine)
{
	std::unique_ptr<GPULangToken> t(new GPULangToken(source, type, channel, start, stop));
	if (!GPULangParser::LineStack.empty())
	{
		t->file = std::get<2>(GPULangParser::LineStack.back());
		t->line = std::get<1>(GPULangParser::LineStack.back()) + line - 1 - std::get<0>(GPULangParser::LineStack.back());
	}
	t->setCharPositionInLine(charPositionInLine);
	if (text != "")
	{
		t->setText(text);
	}

	return t;
}

std::unique_ptr<CommonToken> GPULangTokenFactory::create(size_t type, const std::string & text)
{
	return std::unique_ptr<GPULangToken>(new GPULangToken(type, text));
}
