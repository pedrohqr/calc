cmake_minimum_required(VERSION 3.18)

project(calc)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

add_executable(calc
  include/Calc.h
  include/Frame.h
  include/ProblemReporter.h
  include/Writer.h
  include/Matrix.h
  include/ast/Assignment.h
  include/ast/AST.h
  include/ast/ASTNode.h
  include/ast/BinaryExpression.h
  include/ast/CastExpression.h
  include/ast/ColonAtom.h
  include/ast/ColonExpression.h
  include/ast/Declaration.h
  include/ast/Expression.h
  include/ast/ExpressionList.h
  include/ast/ExpressionStatement.h
  include/ast/FloatLiteral.h
  include/ast/FunctionDeclaration.h
  include/ast/IntLiteral.h
  include/ast/MatrixExpression.h
  include/ast/Reference.h
  include/ast/SizeExpression.h
  include/ast/Statement.h
  include/ast/Transpose.h
  include/ast/TypeReference.h
  include/ast/UnaryExpression.h
  include/parser/ErrorHandler.h
  include/parser/InputBuffer.h
  include/parser/Parser.h
  include/parser/StringBuffer.h
  include/parser/Token.h
  include/symbol/Function.h
  include/symbol/GlobalScope.h
  include/symbol/Scope.h
  include/symbol/Symbol.h
  include/symbol/SymbolMap.h
  include/symbol/Type.h
  include/symbol/Variable.h
  include/util/Allocator.h
  include/util/Exception.h
  include/util/LinkedList.h
  include/util/ObjectPtrList.h
  include/util/SharedObject.h
  include/ast/MyExpressionValue.h
  include/symbol/FunctionFrame.h 
  include/symbol/MiscFunctions.h
  src/Frame.cpp
  src/Main.cpp
  src/ProblemReporter.cpp
  src/Writer.cpp
  src/util/Exception.cpp
  src/ast/Assignment.cpp
  src/ast/BinaryExpression.cpp
  src/ast/CastExpression.cpp
  src/ast/ColonAtom.cpp
  src/ast/ColonExpression.cpp
  src/ast/Expression.cpp
  src/ast/ExpressionStatement.cpp
  src/ast/FloatLiteral.cpp
  src/ast/FunctionDeclaration.cpp
  src/ast/IntLiteral.cpp
  src/ast/MatrixExpression.cpp
  src/ast/Reference.cpp
  src/ast/SizeExpression.cpp
  src/ast/Transpose.cpp
  src/ast/UnaryExpression.cpp
  src/parser/ErrorHandler.cpp
  src/parser/Parser.cpp
  src/symbol/GlobalScope.cpp
  src/symbol/Scope.cpp
  src/symbol/Symbol.cpp 
  src/ast/MyExpressionValue.cpp  
  src/symbol/MiscFunctions.cpp)