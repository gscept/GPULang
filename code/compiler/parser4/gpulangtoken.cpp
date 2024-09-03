#include "gpulangtoken.h"

const Ref<TokenFactory<CommonToken>> GPULangTokenFactory::DEFAULT = std::make_shared<GPULangTokenFactory>();

GPULangTokenFactory::GPULangTokenFactory(){
}

std::unique_ptr<CommonToken> GPULangTokenFactory::create(std::pair<TokenSource*, CharStream*> source, size_t type, const std::string& text, size_t channel, size_t start, size_t stop, size_t line, size_t charPositionInLine)
{
	std::unique_ptr<GPULangToken> t(new GPULangToken(source, type, channel, start, stop));
	t->setLine(line);
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
