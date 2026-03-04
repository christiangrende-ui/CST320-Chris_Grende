#pragma once
//**************************************
// cDeclNode
//
// Base class for all declarations.
// Lab 5 adds type predicates and type accessors.
//**************************************

#include <string>
#include "cAstNode.h"

class cDeclNode : public cAstNode
{
public:
    cDeclNode() : cAstNode() {}

    // Predicates (override only those that should return true)
    virtual bool IsArray()  { return false; }
    virtual bool IsStruct() { return false; }
    virtual bool IsType()   { return false; }
    virtual bool IsFunc()   { return false; }
    virtual bool IsVar()    { return false; }
    virtual bool IsFloat()  { return false; }
    virtual bool IsInt()    { return false; }
    virtual bool IsChar()   { return false; }

    // Size in bytes (type size for type decls, allocation size for var/func decls)
    virtual int  GetSize()  { return m_size; }

    // Lab 6: size/offset storage (getters/setters do no computation)
    int  GetOffset() const { return m_offset; }
    void SetOffset(int off) { m_offset = off; }

    void SetSize(int sz) { m_size = sz; }

    // Returns the type of this declaration (base types return themselves)
    virtual cDeclNode *GetType() = 0;

    // Human-readable name for error messages (e.g., "int", "foo")
    virtual std::string GetName() = 0;

    // Assignment/type compatibility (Lab 5b)
    virtual bool IsCompatibleWith(cDeclNode *rhsType)
    {
        if (rhsType == nullptr) return false;

        // Same exact type
        if (this == rhsType) return true;

        // Float destination: allow int/char -> float promotion, and smaller float -> larger float.
        if (IsFloat())
        {
            if (rhsType->IsFloat())
            {
                return rhsType->GetSize() <= GetSize();
            }
            // allow integer category to promote to float
            if (rhsType->IsInt() || rhsType->IsChar()) return true;
            return false;
        }

        // Integer destination: allow smaller/equal integers (including char), disallow floats.
        if (IsInt() || IsChar())
        {
            if (rhsType->IsFloat()) return false;
            if (rhsType->IsInt() || rhsType->IsChar())
            {
                return rhsType->GetSize() <= GetSize();
            }
        }

        return false;
    }
protected:
    int m_size{0};
    int m_offset{0};
};
