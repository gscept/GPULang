#include "effect.h"

namespace GPULang
{

void Effect::Destroy()
{
    for (Symbol* sym : this->symbols)
        sym->~Symbol();
}

}