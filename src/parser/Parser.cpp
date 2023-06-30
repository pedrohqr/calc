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
// OVERVIEW: Parser.cpp
// ========
// Source file for parser.
//
// Author: Paulo Pagliosa
// Last revision: 18/05/2023

#include "parser/Parser.h"
#include <ctype.h>

namespace calc::parser
{ // begin namespace calc::parser

//
// Auxiliary function
//
KeywordTableEntry*
searchKeyword(KeywordTableEntry* k, const char* name)
//[]----------------------------------------------------[]
//|  Search keyword                                      |
//[]----------------------------------------------------[]
{
  for (; k->name != 0; k++)
    if (strcmp(k->name, name) == 0)
      return k;
  return nullptr;
}


/////////////////////////////////////////////////////////////////////
//
// Parser::Integer: integer token class
// ===============
class Parser::Integer: public Token
{
public:
  static int valueOf(const String& s, int radix)
  {
    return stoi(s, nullptr, radix);
  }

  Integer(const String& lexeme):
    Token{T_INTEGER, lexeme}
  {
    integer = valueOf(lexeme, 10);
  }

}; // Parser::Integer


/////////////////////////////////////////////////////////////////////
//
// Parser::Real: real _token class
// ============
class Parser::Real: public Token
{
public:
  static float valueOf(const String& s)
  {
    return stof(s);
  }

  // Constructor
  Real(const String& lexeme):
    Token{T_REAL, lexeme}
  {
    real = valueOf(lexeme);
  }

}; // Parser::Real


/////////////////////////////////////////////////////////////////////
//
// Parser::Keyword: keyword token class
// ===============
class Parser::Keyword: public Token
{
public:
  // Constructor
  Keyword(const KeywordTableEntry* k):
    Token{k->token, k->name}
  {
    value = k->value;
  }

}; // Parser::Keyword


/////////////////////////////////////////////////////////////////////
//
// Parser implementation
// ======
DEFINE_KEYWORD_TABLE(Parser)
  KEYWORD("int", T_TYPE_NAME, TypeReference::Int()),
  KEYWORD("float", T_TYPE_NAME, TypeReference::Float()),
  KEYWORD("size", T_SIZE, 0),
END_KEYWORD_TABLE;

DEFINE_ERROR_MESSAGE_TABLE(Parser, ErrorHandler)
  ERROR_MESSAGE(SYNTAX,
    "Syntax error"),
  ERROR_MESSAGE(UNEXPECTED_END_OF_FILE_IN_COMMENT_STARTED_ON_LINE,
    "Unexpected end of file in comment started on line %d"),
  ERROR_MESSAGE(UNEXPECTED_CHARACTER,
    "Unexpected character '%c'"),
  ERROR_MESSAGE(NUMBER_OUT_OF_RANGE,
    "Number out of range"),
  ERROR_MESSAGE(EXPECTED,
    "Expected '%c'"),
  ERROR_MESSAGE(UNEXPECTED,
    "Unexpected '%s'"),
  ERROR_MESSAGE(NAME_EXPECTED,
    "Name expected"),
END_ERROR_MESSAGE_TABLE;

Parser::~Parser()
//[]----------------------------------------------------[]
//|  Destructor                                          |
//[]----------------------------------------------------[]
{
  deleteTokens();
}

void
Parser::deleteTokens()
//[]----------------------------------------------------[]
//|  Delete tokens                                       |
//[]----------------------------------------------------[]
{
  for (Token* t; _token; t = _token->_next, _token = t)
    ;
}

String
Parser::errorMessageFormat(const char* msg) const
//[]----------------------------------------------------[]
//|  Error message format                                |
//[]----------------------------------------------------[]
{
  constexpr auto maxLen = 1024;
  char buffer[maxLen];

  snprintf(buffer,
    maxLen,
    "Error %s %d: %s\n", _input->name(),
    _lineNumber,
    msg);
  return buffer;
}

Token*
Parser::nextToken()
//[]----------------------------------------------------[]
//|  Next token                                          |
//[]----------------------------------------------------[]
{
_space:
  while (isspace(*buffer()))
    if (buffer()++ == '\n')
      _lineNumber++;
  if (*buffer() == 0)
    return new Token{T_EOF};
  if (*buffer() == '/')
  {
    if (++buffer() == '/')
    {
      while (*buffer() != '\n')
        if (buffer()++ == 0)
          return new Token{T_EOF};
      ++buffer();
      _lineNumber++;
      goto _space;
    }
    else if (*buffer() == '*')
    {
      auto line = _lineNumber;

_comment:
      while (++buffer() != '*')
        if (*buffer() == 0)
          error(UNEXPECTED_END_OF_FILE_IN_COMMENT_STARTED_ON_LINE, line);
        else if (*buffer() == '\n')
          _lineNumber++;
      if (++buffer() != '/')
        goto _comment;
      ++buffer();
      goto _space;
    }
    return new Token{'/', "/"};
  }
  buffer().beginLexeme();
  if (isalpha(*buffer()) || *buffer() == '_')
  {
    while (isalnum(++buffer()) || *buffer() == '_')
      ;

    auto lexeme = buffer().lexeme();
    auto k = findKeyword(lexeme.c_str());

    return k ? new Keyword{k} : new Token{T_IDENTIFIER, lexeme};
  }

  auto isReal = false;

  if (isdigit(*buffer()))
  {
    while (isdigit(++buffer()))
      ;
    if (*buffer() == '.')
    {
_real:
      isReal = true;
      while (isdigit(++buffer()))
        ;
    }
    if (toupper(*buffer()) == 'E')
    {
      isReal = true;
      if (++buffer() == '+' || *buffer() == '-')
        buffer()++;
      if (!isdigit(*buffer()))
        error(UNEXPECTED_CHARACTER, *buffer());
      while (isdigit(++buffer()))
        ;
    }

    auto lexeme = buffer().lexeme();

    try
    {
      return isReal ? (Token*)new Real{lexeme} : new Integer{lexeme};
    }
    catch (std::out_of_range&)
    {
      error(NUMBER_OUT_OF_RANGE);
    }
  }

  int c = buffer()++;

  switch (c)
  {
    case '=':
    case '(':
    case ')':
    case ',':
    case ';':
    case ':':
    case '[':
    case ']':
    case '+':
    case '-':
    case '*':
    case '^':
    case '\'':
    //case '\\': // TODO
      break;
    case '.':
      if (isdigit(*buffer()))
        goto _real;
    default:
      error(UNEXPECTED_CHARACTER, c);
  }
  return new Token{c, buffer().lexeme()};
}

Token*
Parser::lookahead(Token* token, int n)
//[]----------------------------------------------------[]
//|  Lookahead                                           |
//[]----------------------------------------------------[]
{
  auto t = token;

  for (int i{}; i < n; i++)
    t = t->_next != nullptr ? t->_next : (t->_next = nextToken());
  return t;
}

void
Parser::match(int c)
//[]----------------------------------------------------[]
//|  Match                                               |
//[]----------------------------------------------------[]
{
  if (_token->type != c)
  {
    if (_token->type == T_EOF)
      error(EXPECTED, c);
    error(UNEXPECTED, _token->lexeme.c_str());
  }
  advance();
}

String
Parser::matchIdentifier()
//[]----------------------------------------------------[]
//|  Match identifier                                    |
//[]----------------------------------------------------[]
{
  if (_token->type != T_IDENTIFIER)
    error(NAME_EXPECTED);

  auto identifier = _token->lexeme;

  advance();
  return identifier;
}

ExpressionStatementPtr
Parser::expressionStatement()
//[]----------------------------------------------------[]
//|  Expression statement                                |
//[]----------------------------------------------------[]
{
  return new ExpressionStatement{expression()};
}

AssignmentPtr
Parser::assignment()
//[]----------------------------------------------------[]
//|  Assignment                                          |
//[]----------------------------------------------------[]
{
  AssignmentPtr s = new Assignment;

  if (_token->type == '[')
  {
    advance();
    referenceList(s->lhs());
    match(']');
  }
  else
    s->lhs().add(reference());
  match('=');
  s->setExpression(expression());
  return s;
}

void
Parser::expressionList(ExpressionList* expressions)
//[]----------------------------------------------------[]
//|  Expression list                                     |
//[]----------------------------------------------------[]
{
  for (;; advance())
  {
    expressions->add(expression());
    if (_token->type != ',')
      break;
  }
}

void
Parser::referenceList(ReferenceList& references)
//[]----------------------------------------------------[]
//|  Reference list                                      |
//[]----------------------------------------------------[]
{
  for (;; advance())
  {
    references.add(reference());
    if (_token->type != ',')
      break;
  }
}

ReferencePtr
Parser::reference(Reference::Place place)
//[]----------------------------------------------------[]
//|  Reference                                           |
//[]----------------------------------------------------[]
{
  int line = _lineNumber;
  ReferencePtr reference = new Reference{matchIdentifier(), place};

  reference->beginLine = line;
  if (_token->type == '(')
  {
    advance();
    if (_token->type == ')')
      advance();
    else
    {
      argumentList(reference);
      match(')');
    }
  }
  return reference;
}

void
Parser::argumentList(Reference* reference)
//[]----------------------------------------------------[]
//|  Argument list                                       |
//[]----------------------------------------------------[]
{
  for (int index = 0;; advance(), index++)
  {
    if (_token->type == ':')
    {
      new ColonAtom(reference, index);
      advance();
    }
    else
      reference->arguments().add(expression());
    if (_token->type != ',')
      break;
  }
}

ExpressionPtr
Parser::expression()
//[]----------------------------------------------------[]
//|  Expression                                          |
//[]----------------------------------------------------[]
{
  return colonExpression();
}

ExpressionPtr
Parser::colonExpression()
//[]----------------------------------------------------[]
//|  Colon expression                                    |
//[]----------------------------------------------------[]
{
  auto e = additiveExpression();

  if (_token->type != ':')
    return e;

  ExpressionPtr stride{};
  ExpressionPtr end{};

  do
  {
    advance(); // :
    if (stride != nullptr)
    {
      e = new ColonExpression{e, stride, end};
      stride = nullptr;
    }
    else if (end != nullptr)
      stride = end;
    end = additiveExpression();
  }
  while (_token->type == ':');
  if (stride == nullptr)
    stride = new IntLiteral{1};
  return new ColonExpression{e, stride, end};
}

ExpressionPtr
Parser::additiveExpression()
//[]----------------------------------------------------[]
//|  Additive expression                                 |
//[]----------------------------------------------------[]
{
  auto e = multiplicativeExpression();

  for (int op;;)
  {
    switch (op = _token->type)
    {
      case '+':
      case '-':
        break;
      default:
        return e;
    }
    advance();
    e = new BinaryExpression{e, multiplicativeExpression(), op};
  }
}

ExpressionPtr
Parser::multiplicativeExpression()
//[]----------------------------------------------------[]
//|  Multiplicative expression                           |
//[]----------------------------------------------------[]
{
  auto e = unaryExpression();

  for (int op;;)
  {
    switch (op = _token->type)
    {
      case '*':
      case '/':
        break;
      default:
        return e;
    }
    advance();
    e = new BinaryExpression{e, unaryExpression(), op};
  }
}

ExpressionPtr
Parser::unaryExpression()
//[]----------------------------------------------------[]
//|  Unary expression                                    |
//[]----------------------------------------------------[]
{
  auto beginLine = _lineNumber;
  auto minus = false;

  for (;; advance())
    if (_token->type == '-')
      minus ^= true;
    else if (_token->type != '+')
      break;

  auto transpose = false;
  auto e = primaryExpression();

  while (_token->type == '\'')
  {
    transpose ^= true;
    advance();
  }
  if (transpose)
    e = new Transpose{e};
  if (minus)
    e = new UnaryExpression{e, '-'};
  e->beginLine = beginLine;
  return e;
}

ExpressionPtr
Parser::primaryExpression()
//[]----------------------------------------------------[]
//|  Primary expression                                  |
//[]----------------------------------------------------[]
{
  ExpressionPtr e;
  int beginLine = _lineNumber;

  switch (_token->type)
  {
    case '(':
      advance();
      e = expression();
      match(')');
      break;
    case T_SIZE:
      advance();
      match('(');
      e = new SizeExpression{expression()};
      match(')');
      break;
    case T_TYPE_NAME:
    {
      auto type = (TypeReference*)_token->value;

      advance();
      match('(');
      e = new CastExpression{type, expression()};
      match(')');
      break;
    }
    case T_IDENTIFIER:
      e = reference(Reference::RHS);
      break;
    case T_INTEGER:
      e = new IntLiteral{_token->integer};
      advance();
      break;
    case T_REAL:
      e = new FloatLiteral{_token->real};
      advance();
      break;
    case '[':
      e = matrix();
      break;
    default:
      error(SYNTAX);
  }
  e->beginLine = beginLine;
  return e;
}

MatrixExpressionPtr
Parser::matrix()
//[]----------------------------------------------------[]
//|  Matrix                                              |
//[]----------------------------------------------------[]
{
  MatrixExpressionPtr matrix = new MatrixExpression;

  advance(); // [
  if (_token->type == ']')
    advance();
  else
  {
    for (;; advance())
    {
      MatrixRowPtr row = new MatrixRow;

      expressionList(row);
      matrix->rows().add(row);
      if (_token->type != ';')
        break;
    }
    match(']');
  }
  return matrix;
}

bool
Parser::isAssignment()
//[]----------------------------------------------------[]
//|  Is assignment                                       |
//[]----------------------------------------------------[]
{
  Token* t{_token};

  if (t->type == '[')
    for (int level = 1;;)
      switch ((t = lookahead(t))->type)
      {
        case T_EOF:
          return false;
        case '[':
          level++;
          break;
        case ']':
          level--;
          break;
        case '=':
          if (level > 0)
            error(UNEXPECTED_CHARACTER, '=');
          return true;
      }
  if (t->type != T_IDENTIFIER)
    return false;
  if ((t = lookahead(t))->type == '(')
    for (int level = 1;;)
      switch ((t = lookahead(t))->type)
      {
        case T_EOF:
          return false;
        case '(':
          level++;
          break;
        case ')':
          level--;
          break;
        case '=':
          if (level > 1)
            error(UNEXPECTED_CHARACTER, '=');
          return true;
    }
  return t->type == '=';
}

} // end namespace calc::parser
