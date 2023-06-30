//[]---------------------------------------------------------------[]
//|                                                                 |
//| Copyright (C) 2010, 2023 Paulo Pagliosa.                        |
//|                                                                 |
//| This software is provided 'as-is', without any express or       |
//| implied warranty. In no event will the authors be held liable   |
//| for any damages arising from the use of this software.          |
//|                                                                 |
//| Permission is granted to anyone to use this software for any    |
//| purpose, including commercial applications, and to alter it and |
//| redistribute it freely, subject to the following restrictions:  |
//|                                                                 |
//| 1. The origin of this software must not be misrepresented; you  |
//| must not claim that you wrote the original software. If you use |
//| this software in a product, an acknowledgment in the product    |
//| documentation would be appreciated but is not required.         |
//|                                                                 |
//| 2. Altered source versions must be plainly marked as such, and  |
//| must not be misrepresented as being the original software.      |
//|                                                                 |
//| 3. This notice may not be removed or altered from any source    |
//| distribution.                                                   |
//|                                                                 |
//[]---------------------------------------------------------------[]
//
// OVERVIEW: Parser.h
// ========
// Class definition for parser.
//
// Author: Paulo Pagliosa
// Last revision: 18/05/2023

#ifndef __Parser_h
#define __Parser_h

#include "ast/AST.h"
#include "parser/ErrorHandler.h"
#include "parser/InputBuffer.h"
#include "parser/Token.h"
#include "symbol/Scope.h"

namespace calc::parser
{ // begin namespace calc::parser

using namespace ast;
using namespace symbol;


/////////////////////////////////////////////////////////////////////
//
// KeywordTableEntry: keyword table entry
// =================
struct KeywordTableEntry
{
  const char* name;
  int token;
  void* value;

}; // KeywordTableEntry

//
// Auxiliary function
//
KeywordTableEntry* searchKeyword(KeywordTableEntry*, const char*);

//
// Macros to declare a keyword table
//
#define DECLARE_KEYWORD_TABLE(cls) \
private: \
  static parser::KeywordTableEntry _keywords[]; \
protected: \
  virtual parser::KeywordTableEntry* findKeyword(const char*)

#define DEFINE_KEYWORD_TABLE(cls) \
parser::KeywordTableEntry* \
cls::findKeyword(const char* name) \
{ \
  return searchKeyword(_keywords, name); \
} \
DEFINE_KEYWORD_TABLE_ENTRIES(cls)

#define DEFINE_KEYWORD_TABLE2(cls, base) \
parser::KeywordTableEntry* \
cls::findKeyword(const char* name) \
{ \
  auto k = searchKeyword(_keywords, name); \
  return k != nullptr ? k : base::findKeyword(name); \
} \
DEFINE_KEYWORD_TABLE_ENTRIES(cls)

#define DEFINE_KEYWORD_TABLE_ENTRIES(cls) \
parser::KeywordTableEntry cls::_keywords[] = {

#define KEYWORD(name, token, code) \
{##name, token, code}

#define END_KEYWORD_TABLE \
{0, -1}}


/////////////////////////////////////////////////////////////////////
//
// Parser: abstract parser class
// ======
class Parser: public ErrorHandler
{
public:
  virtual ~Parser();

protected:
  enum TokenType
  {
    T_EOF = 0,
    T_IDENTIFIER = 256,
    T_TYPE_NAME,
    T_INTEGER,
    T_REAL,
    T_SIZE,
    lastToken
  };

  class Integer;
  class Real;
  class Keyword;

  enum ErrorCode
  {
    SYNTAX,
    UNEXPECTED_END_OF_FILE_IN_COMMENT_STARTED_ON_LINE,
    UNEXPECTED_CHARACTER,
    NUMBER_OUT_OF_RANGE,
    EXPECTED,
    UNEXPECTED,
    NAME_EXPECTED,
    lastErrorCode
  };

  int _lineNumber;
  util::ObjectPtr<Token> _token;

  String errorMessageFormat(const char*) const override;

  auto& buffer()
  {
    return *_input;
  }

  void setInput(InputBuffer* input)
  {
    deleteTokens();
    _input = input;
    _lineNumber = 1;
    _token = nextToken();
  }

  void deleteTokens();
  Token* lookahead(Token*, int = 1);

  virtual Token* nextToken();

  void advance()
  {
    _token = _token->_next ? _token->_next : nextToken();
  }

  void match(int);
  String matchIdentifier();

  DECLARE_KEYWORD_TABLE(Parser);
  DECLARE_ERROR_MESSAGE_TABLE(Parser);

  ExpressionStatementPtr expressionStatement();
  AssignmentPtr assignment();
  void referenceList(ReferenceList&);
  ReferencePtr reference(Reference::Place = Reference::LHS);
  void argumentList(Reference*);
  void expressionList(ExpressionList*);
  ExpressionPtr expression();
  ExpressionPtr colonExpression();
  ExpressionPtr additiveExpression();
  ExpressionPtr multiplicativeExpression();
  ExpressionPtr unaryExpression();
  ExpressionPtr primaryExpression();
  MatrixExpressionPtr matrix();

  bool isAssignment();

private:
  util::ObjectPtr<InputBuffer> _input;

}; // Parser

} // end namespace calc::parser

#endif // __Parser_h
