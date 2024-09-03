#include "anyfxtoken.h"

const Ref<TokenFactory<CommonToken>> AnyFXTokenFactory::DEFAULT = std::make_shared<AnyFXTokenFactory>();

AnyFXTokenFactory::AnyFXTokenFactory(){
}

std::unique_ptr<CommonToken> AnyFXTokenFactory::create(std::pair<TokenSource*, CharStream*> source, size_t type, const std::string& text, size_t channel, size_t start, size_t stop, size_t line, size_t charPositionInLine)
{
	std::unique_ptr<AnyFXToken> t(new AnyFXToken(source, type, channel, start, stop));
	t->setLine(line);
	t->setCharPositionInLine(charPositionInLine);
	if (text != "")
	{
		t->setText(text);
	}

	return t;
}

std::unique_ptr<CommonToken> AnyFXTokenFactory::create(size_t type, const std::string & text)
{
	return std::unique_ptr<AnyFXToken>(new AnyFXToken(type, text));
}
