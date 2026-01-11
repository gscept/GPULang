#pragma once
//------------------------------------------------------------------------------
/**
    @class GPULang::Expression
    
    Base class for expressions. 
    In AnyFX, expressions are purely aritmetic, meaning they can only return a value.
    
    (C) 2021 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "ast/symbol.h"
#include "ast/types/type.h"

namespace GPULang
{

enum AccessFlags
{
    Const = 0x1,
    LinkTime = 0x2
};

union AccessBits
{
    AccessBits()
        : bits(0x0)
    {}

    AccessBits(uint32_t mask)
        : bits(mask)
    {}

    struct
    {
        uint32_t volatileAccess : 1;
        uint32_t nonTemporalAccess : 1;
    } flags;
    uint32_t bits;
};

/// Convert storage to string
ConstantString StorageToString(Storage storage);
/// Check if storage is compatible
bool IsStorageCompatible(Storage lhs, Storage rhs);
/// Returns true if storage type needs explicit call signature
bool StorageRequiresSignature(Storage storage);



#define VALUE_UNION_SWITCH()\
case TypeCode::Bool:\
{\
X(Bool, b, bool)\
break;\
}\
case TypeCode::Float:\
case TypeCode::Float16:\
{\
X(Float, f, float)\
break;\
}\
case TypeCode::Int:\
case TypeCode::Int16:\
{\
X(Int, i, int)\
break;\
}\
case TypeCode::UInt:\
case TypeCode::UInt16:\
{\
X(UInt, ui, unsigned int)\
break;\
default:\
assert(false);\
break;\
}

struct ValueUnion
{
    union
    {
        int i[16];
        unsigned int ui[16];
        float f[16];
        bool b[16];
    };

#define CONVERT(from, to)\
    this->to[0] = this->from[0];\
    this->to[1] = this->from[1];\
    this->to[2] = this->from[2];\
    this->to[3] = this->from[3];\
    this->to[4] = this->from[4];\
    this->to[5] = this->from[5];\
    this->to[6] = this->from[6];\
    this->to[7] = this->from[7];\
    this->to[8] = this->from[8];\
    this->to[9] = this->from[9];\
    this->to[10] = this->from[10];\
    this->to[11] = this->from[11];\
    this->to[12] = this->from[12];\
    this->to[13] = this->from[13];\
    this->to[14] = this->from[14];\
    this->to[15] = this->from[15];
    
    void Convert(TypeCode newCode)
    {
        if (newCode == this->code)
            return;
        switch (newCode)
        {
            case TypeCode::Bool:
                switch (this->code)
                {
                    case TypeCode::Int:
                    case TypeCode::Int16:
                        CONVERT(i, b);
                        break;
                    case TypeCode::UInt:
                    case TypeCode::UInt16:
                        CONVERT(ui, b);
                        break;
                    case TypeCode::Float:
                    case TypeCode::Float16:
                        CONVERT(f, b);
                        break;
                    default:
                        assert(false);
                }
                break;
            case TypeCode::Int:
            case TypeCode::Int16:
                switch (this->code)
                {
                    case TypeCode::Bool:
                        CONVERT(b, i);
                        break;
                    case TypeCode::UInt:
                    case TypeCode::UInt16:
                        CONVERT(ui, i);
                        break;
                    case TypeCode::Float:
                    case TypeCode::Float16:
                        CONVERT(f, i);
                        break;
                    default:
                        assert(false);
                }
                break;
            case TypeCode::UInt:
            case TypeCode::UInt16:
                switch (this->code)
                {
                    case TypeCode::Bool:
                        CONVERT(b, ui);
                        break;
                    case TypeCode::Int:
                    case TypeCode::Int16:
                        CONVERT(i, ui);
                        break;
                    case TypeCode::Float:
                    case TypeCode::Float16:
                        CONVERT(f, ui);
                        break;
                    default:
                        assert(false);
                }
                break;
            case TypeCode::Float:
            case TypeCode::Float16:
                switch (this->code)
                {
                    case TypeCode::Int:
                    case TypeCode::Int16:
                        CONVERT(i, f);
                        break;
                    case TypeCode::UInt:
                    case TypeCode::UInt16:
                        CONVERT(ui, f);
                        break;
                    case TypeCode::Bool:
                        CONVERT(b, f);
                        break;
                    default:
                        assert(false);
                }
                break;
            default:
                assert(false);
        }
        this->code = newCode;
    }

    void Assign(const ValueUnion& rhs, int fromIndex, int toIndex)
    {
        assert(this->code == rhs.code);
        switch (this->code)
        {
            case TypeCode::Bool:
                this->b[toIndex] = rhs.b[fromIndex];
                break;
            case TypeCode::Int:
            case TypeCode::Int16:
                this->i[toIndex] = rhs.i[fromIndex];
                break;
            case TypeCode::UInt:
            case TypeCode::UInt16:
                this->ui[toIndex] = rhs.ui[fromIndex];
                break;
            case TypeCode::Float:
            case TypeCode::Float16:
                this->f[toIndex] = rhs.f[fromIndex];
                break;
            default:
                assert(false);
        }
    }

    void Expand(int newColumnSize, int newRowSize)
    {
        if (this->columnSize < newColumnSize || this->rowSize < newRowSize)
        {
            int newSize = (newColumnSize * newRowSize);
            int oldSize = (this->columnSize * this->rowSize);
            for (int i = oldSize; i < newSize; i++)
            {
                Assign(*this, oldSize - 1, i);
            }
            this->columnSize = newColumnSize;
            this->rowSize = newRowSize;    
        }
    }

    bool Store(uint32_t& ui)
    {
        this->Convert(TypeCode::UInt);
        ui = this->ui[0];
        return this->columnSize == 1 && this->rowSize == 1;
    }
    bool Store(int& i)
    {
        this->Convert(TypeCode::Int);
        i = this->i[0];
        return this->columnSize == 1 && this->rowSize == 1;
    }
    bool Store(float& f)
    {
        this->Convert(TypeCode::Float);
        f = this->f[0];
        return this->columnSize == 1 && this->rowSize == 1;
    }
    bool Store(bool& b)
    {
        this->Convert(TypeCode::Bool);
        b = this->b[0];
        return this->columnSize == 1 && this->rowSize == 1;
    }
    bool Store(uint8_t& ui8)
    {
        this->Convert(TypeCode::UInt);
        ui8 = this->ui[0] & 0xFF;
        return this->columnSize == 1 && this->rowSize == 1;
    }

    bool SetType(const Type* type)
    {
        this->columnSize = type->columnSize;
        this->rowSize = type->rowSize;
        this->code = type->baseType;
        switch (this->code)
        {
            case TypeCode::Bool:
            case TypeCode::Int:
            case TypeCode::Int16:
            case TypeCode::UInt:
            case TypeCode::UInt16:
            case TypeCode::Float:
            case TypeCode::Float16:
                return true;
                break;
            default:
                return false;
        }
    }

    TypeCode code = TypeCode::InvalidType;
    int columnSize = 1, rowSize = 1;
};

struct Compiler;
struct Expression : public Symbol
{
    /// constructor
    Expression();
    /// destructor
    virtual ~Expression();

    /// Evaluate expression and store data in resolved
    virtual bool Resolve(Compiler* compiler);

    /// eval type
    virtual bool EvalType(Type::FullType& out) const;
    /// eval type symbol
    virtual bool EvalTypeSymbol(Type*& out) const;
    /// eval unswizzle type symbol
    virtual bool EvalUnswizzledTypeSymbol(Type*& out) const;
    /// eval symbol
    virtual bool EvalSymbol(FixedString& out) const;
    /// Evaluate value
    virtual bool EvalValue(ValueUnion& out) const;
    ///// evaluates expression as an integer
    //virtual bool EvalIntVec(std::vector<int>& out) const;
    ///// evaluates expression as an integer
    //virtual bool EvalUIntVec(std::vector<unsigned>& out) const;
    ///// evaulates expression as a float
    //virtual bool EvalFloatVec(std::vector<float>& out) const;
    ///// evaluates expression as a boolean
    //virtual bool EvalBoolVec(std::vector<bool>& out) const;
    /// evaluates access flags
    virtual bool EvalAccessFlags(unsigned& out) const;
    /// evaluates storage
    virtual bool EvalStorage(Storage& out) const;
    
    /// evaluates expression as a string
    virtual TransientString EvalString() const;

    bool isLhsValue;

    struct __Resolved : public Symbol::__Resolved
    {
        virtual ~__Resolved() {};
        union
        {
            int32_t i;
            uint32_t ui;
            float f;
            bool b;
        } value;
        Storage storage = Storage::Default;
        //std::string text;
    };
}; 
} // namespace GPULang
//------------------------------------------------------------------------------
