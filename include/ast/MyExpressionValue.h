#ifndef __ExpressionValue_h
#define __ExpressionValue_h

#include "ast/Expression.h"
#include "util/Exception.h"
#include "Matrix.h"
#include "Writer.h"
#include <variant>
#include <type_traits>

using namespace math;

namespace calc
{ // begin namespace calc

//
// Forward definition
//
    class Writer;

    namespace ast
    { // begin namespace ast

    /////////////////////////////////////////////////////////////////////
    //
    // Expression::Value: expression value class
    // =================
        class Expression::Value
        {
        public:
            Value();
            Value(int);
            Value(float);

            Type* type() const
            {
                return _type;
            }

            Value castTo(const Type*) const;

            Value operator +(const Value&) const;       
            Value operator -(const Value&) const;
            Value operator *(const Value&) const;
            Value operator /(const Value&) const;

            Value operator -() const;

            Value size() const;
            Value transpose() const;

            Value horzcat(const Value&) const;
            Value vertcat(const Value&) const;

            Value operator ()(const Value&) const;
            Value operator ()(const Value&, const Value&) const;
            Value rows(const Value&) const;
            Value cols(const Value&) const;
            Value vector() const;

            void set(const Value&, const Value&);
            void set(const Value&, const Value&, const Value&);
            void setRows(const Value&, const Value&);
            void setCols(const Value&, const Value&);
            void setVector(const Value&);

            void write(Writer&) const;

            static Value colon(const Value&, const Value&, const Value&);

        private:
            Type* _type;
            std::variant<FloatMatrix, IntMatrix> _value;

        }; // Expression::Value

    } // end namespace ast

} // end namespace calc

#endif // __ExpressionValue_h
