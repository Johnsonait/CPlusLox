#include "../include/resolver.hpp"

namespace Lox {

Resolver::Resolver(Interpreter* interpreter) : interpreter{interpreter}, scopes{}, currentFunction{FunctionType::NONE}
{}

void Resolver::resolve(std::vector<std::unique_ptr<Stmt>>& statements) {
    for (auto& stmt : statements) {
        resolve(stmt);
    }
}

void Resolver::resolve(std::list<std::unique_ptr<Stmt>>& statements) {
    for (auto& stmt : statements) {
        resolve(stmt);
    }
}


void Resolver::resolve(std::unique_ptr<Stmt>& stmt) {
    stmt->accept(this);
}

void Resolver::resolveLocal(Expr* expr, const Token& name) {
    for (int i = scopes.size()-1; i >= 0; i--) {
        if (scopes[i].find(name.lexeme)!= scopes[i].end()) {
            interpreter->resolve(expr,scopes.size()-1-i);
            return;
        }
    }
}

void Resolver::resolveFunction(Function* function, const FunctionType& type) {
    auto enclosingFunction = currentFunction;
    currentFunction = type;

    beginScope();
    for (auto& param : function->params) {
        declare(param);
        define(param);
    }
    resolve(function->body);
    endScope();

    currentFunction = enclosingFunction;
}

void Resolver::resolve(std::unique_ptr<Expr>& expr) {
    expr->accept(this);
}

void Resolver::declare(const Token& name) {
    if (scopes.empty()) return;
    auto& scope = scopes.back();
    if (scope.find(name.lexeme) != scope.end()) {
        Lox::error(name, "Already a variable with this name in this scope");
    }
    scope[name.lexeme] = false;
}

void Resolver::define(const Token& name) {
    if (scopes.empty()) return;
    auto& scope = scopes.back();
    scope[name.lexeme] = true;
}

void Resolver::beginScope() {
    scopes.push_back(std::unordered_map<std::string,bool>{});
}

void Resolver::endScope() {
    scopes.pop_back();
}

//==============================================================================
// ExprVisitor<void>
//==============================================================================
void Resolver::visitBinaryExpr(Binary* expr) {
    resolve(expr->left);
    resolve(expr->right);
}

void Resolver::visitGroupingExpr(Grouping* expr) {
    resolve(expr->expression);
}

void Resolver::visitUnaryExpr(Unary* expr) {
    resolve(expr->expression);
}

void Resolver::visitLiteralExpr(Literal* expr) {
    // Do nothing
    return;
}

void Resolver::visitVariableExpr(Variable* expr) {
    if (!scopes.empty() && scopes.back().find(expr->name.lexeme) == scopes.back().end()) {
        Lox::error(expr->name, "Local variable not found.");
    } else if (!scopes.empty() && scopes.back().at(expr->name.lexeme) == false) {
        Lox::error(expr->name, "Can't read local variable in it's own initializer.");
    }
    resolveLocal(expr, expr->name);
}

void Resolver::visitAssignExpr(Assign* expr) {
    resolve(expr->value);   
    resolveLocal(expr,expr->name);
}

void Resolver::visitLogicalExpr(Logical* expr) {
    resolve(expr->left);
    resolve(expr->right);
}

void Resolver::visitCallExpr(Call* expr) {
    resolve(expr->callee);

    for (auto& arg : expr->arguments) {
        resolve(arg);
    }
}

//==============================================================================
// StmtVisitor<void>
//==============================================================================
void Resolver::visitExpressionStmt(Expression* stmt) {
    resolve(stmt->expr);
}

void Resolver::visitFunctionStmt(Function* stmt) {
    declare(stmt->name);
    define(stmt->name);
    resolveFunction(stmt,FunctionType::FUNCTION);
}

void Resolver::visitReturnStmt(Return* stmt) {
    if (currentFunction == FunctionType::NONE) {
        Lox::error(stmt->keyword,"Can't return from top-level code.");
    }
    if (stmt->value != nullptr) resolve(stmt->value);
}

void Resolver::visitVarStmt(Var* stmt) {
    declare(stmt->name);
    if (stmt->initializer != nullptr) {
        resolve(stmt->initializer);
    }
    define(stmt->name);
}

void Resolver::visitPrintStmt(Print* stmt) {
    resolve(stmt->value);
}

void Resolver::visitBlockStmt(Block* stmt) {
    beginScope();
    resolve(stmt->statements);
    endScope();
}

void Resolver::visitIfStmt(If* stmt) {
    resolve(stmt->condition);
    resolve(stmt->thenBranch);
    if (stmt->elseBranch != nullptr) resolve(stmt->elseBranch);
}

void Resolver::visitWhileStmt(While* stmt) {
    resolve(stmt->expr);
    resolve(stmt->body);
}


} // Lox namespace