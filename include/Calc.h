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
// OVERVIEW: Calc.h
// ========
// Class definition for matrix calculator.
//
// Author: Paulo Pagliosa
// Last revision: 17/05/2023

#ifndef __Calc_h
#define __Calc_h

#include "parser/Parser.h"
#include "parser/StringBuffer.h"
#include "symbol/GlobalScope.h"
#include "Writer.h"

namespace calc
{ // begin namespace calc

using namespace parser;
using namespace symbol;


/////////////////////////////////////////////////////////////////////
//
// Calc: calc class
// ====
class Calc: public Parser
{
public:
  Calc():
    _input{""}
  {
    StringBuffer::makeUse(&_input);

    addEye();
    addZeros();
    addOnes();
    addDiag();
  }

  auto execute(const char* line) try
  {
    _input.set(line);
    setInput(&_input);
    return command();
  }
  catch (const std::exception& e)
  {
    puts(e.what());
    return true;
  }

private:
  enum
  {
    T_CLEAR = Parser::lastToken,
    T_LIST,
    T_LOAD,
    T_EXIT
  };

  void addEye()
  {
      auto* func_decl = new FunctionDeclaration{};
      func_decl->setName("eye");
      func_decl->parameters().add(new Parameter{ "m" });
      func_decl->parameters().add(new Parameter{ "n" });
      func_decl->output().add(new Parameter{ "o" });
      _globals.buildFunction(func_decl);
  }

  void addZeros()
  {
      auto* funcDecl = new FunctionDeclaration{};
      funcDecl->setName("zeros");
      funcDecl->parameters().add(new Parameter{ "m" });
      funcDecl->parameters().add(new Parameter{ "n" });
      funcDecl->output().add(new Parameter{ "o" });
      _globals.buildFunction(funcDecl);
  }

  void addOnes()
  {
      auto* funcDecl = new FunctionDeclaration{};
      funcDecl->setName("ones");
      funcDecl->parameters().add(new Parameter{ "m" });
      funcDecl->parameters().add(new Parameter{ "n" });
      funcDecl->output().add(new Parameter{ "o" });
      _globals.buildFunction(funcDecl);
  }

  void addDiag()
  {
      auto* funcDecl = new FunctionDeclaration{};
      funcDecl->setName("diag");
      funcDecl->parameters().add(new Parameter{ "m" });
      funcDecl->output().add(new Parameter{ "o" });
      _globals.buildFunction(funcDecl);
  }
  StringBuffer _input;
  GlobalScope _globals;
  Writer _writer;

  DECLARE_KEYWORD_TABLE(Calc);

  void matchEof()
  {
    if (_token->type != T_EOF)
      error(SYNTAX);
  }

  bool command()
  {
    if (_token->type == T_LIST)
    {
      String name;

      advance();
      if (_token->type == T_IDENTIFIER)
      {
        name = _token->lexeme;
        advance();
      }
      matchEof();
      if (name.length() == 0)
        _globals.Frame::write(_writer);
      else if (auto r = _globals.findRecord(name))
        r->write(_writer);
    }
    else if (_token->type == T_CLEAR)
    {
      String name;

      advance();
      if (_token->type == T_IDENTIFIER)
      {
        name = _token->lexeme;
        advance();
      }
      matchEof();
      if (name.length() == 0)
        _globals.clear();
      else if (!_globals.removeVariable(name))
        printf("No variable '%s'\n", name.c_str());
    }
    else if (_token->type == T_EXIT)
    {
      advance();
      matchEof();
      return false;
    }
    else if (_token->type != T_EOF)
      if (isAssignment())
      {
        AssignmentPtr a = assignment();

        matchEof();
        a->resolve(&_globals);
        a->execute(&_globals);
        for (auto r : a->lhs())
        {
          auto& name = r->name();

          _writer.write(name.c_str());
          _writer.write("=\n");
          _globals.findRecord(name)->value.write(_writer);
          _writer.endLine();
        }
      }
      else
        parseExpressionStatement();
    return true;
  }

  void parseExpressionStatement()
  {
    auto s = expressionStatement();

    matchEof();
    s->resolve(&_globals);
    s->expression()->eval(&_globals).write(_writer);
    _writer.endLine();
  }

  String errorMessageFormat(const char* msg) const override
  {
    return ErrorHandler::errorMessageFormat(msg);
  }

}; // Calc

DEFINE_KEYWORD_TABLE2(Calc, parser::Parser)
  KEYWORD("clear", T_CLEAR, 0),
  KEYWORD("list", T_LIST, 0),
  KEYWORD("exit", T_EXIT, 0),
END_KEYWORD_TABLE;

} // end namespace calc

#endif // __Calc_h
