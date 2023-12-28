#include "../include/interpreter.hpp"

namespace Lox {

void Interpreter::interpret(std::unique_ptr<Expr>& expression) {
    try {
        Value value = evaluate(expression);
        std::cout<<stringify(value) << std::endl;
    } catch (RuntimeError& error){
        Lox::runtimeError(error);
    }
}

void Interpreter::interpret(std::vector<std::unique_ptr<Stmt>>& statments) {
    try {
        for (auto& statement : statments) {
            execute(statement);
        }
    } catch(RuntimeError& error) {
        Lox::runtimeError(error);
    }
    
}

//==============================================================================
// ExprVisitor<Value> implementation
//==============================================================================
Value Interpreter::visitBinaryExpr(Binary* b) {
    const Value left = evaluate(b->left);
    const Value right = evaluate(b->right);

    switch (b->op.type)
    {
    case TokenType::MINUS: {
        checkNumberOperands(b->op,left,right);
        return left - right;
    }
    case TokenType::SLASH: {
        checkNumberOperands(b->op,left,right);   
        return left / right;
    }
    case TokenType::STAR: {
        checkNumberOperands(b->op,left,right);
        return left * right;
    }
    case TokenType::PLUS: {
        checkAdditionOperation(b->op,left,right);
        return left + right;
    }
    case TokenType::GREATER: {
        checkNumberOperands(b->op,left,right);
        return Value{left > right};
    }
    case TokenType::GREATER_EQUAL: {
        checkNumberOperands(b->op,left,right);
        return Value{left >= right};
    }
    case TokenType::LESS: {
        checkNumberOperands(b->op,left,right);
        return Value{left < right};
    }
    case TokenType::LESS_EQUAL: {
        checkNumberOperands(b->op,left,right);
        return Value{left <= right};
    }
    case TokenType::EQUAL_EQUAL: {
        return Value{left == right};
    }
    case TokenType::BANG_EQUAL: {
        return Value{left != right};
    }
    default:
        break;
    }

    return Value{std::monostate{}};
}

Value Interpreter::visitGroupingExpr(Grouping* g) {
    return evaluate(g->expression);
}

Value Interpreter::visitUnaryExpr(Unary* u) {
    Value right = evaluate(u->expression);

    switch (u->op.type)
    {
    case TokenType::BANG: {
        return !isTruthy(right);
    }
    case TokenType::MINUS: {
        checkNumberOperand(u->op,right);
        if (std::holds_alternative<double>(right.item)) {
            double v = -std::get<double>(right.item);
            right = Value{v};
            return right;
        }
    }
    default:
        break;
    }

    return Value{std::monostate{}};
}

Value Interpreter::visitLiteralExpr(Literal* l) {
    return l->value;
}

//==============================================================================
// StmtVisitor<void> implementation
//==============================================================================
void Interpreter::visitExpressionStmt(Expression* stmt) {
    evaluate(stmt->expr);
}

void Interpreter::visitPrintStmt(Print* stmt) {
    auto val = evaluate(stmt->value);
    std::cout<<stringify(val)<<std::endl;
}


//==============================================================================
// Utility methods
//==============================================================================
Value Interpreter::evaluate(Expr* expr) {
    return expr->accept(this);
}

Value Interpreter::evaluate(std::unique_ptr<Expr>& expr) {
    return expr->accept(this);
}

void Interpreter::execute(std::unique_ptr<Stmt>& stmt) {
    stmt->accept(this);
}

bool Interpreter::isTruthy(const Value& v) {
    if (std::holds_alternative<std::monostate>(v.item)) return false;
    if (std::holds_alternative<bool>(v.item)) return !std::get<bool>(v.item);
    return true;
}

std::string Interpreter::stringify(const Value& v) {
    if (std::holds_alternative<std::monostate>(v.item)) return "nil";
    if (std::holds_alternative<double>(v.item)) {
        std::stringstream text{std::to_string(std::get<double>(v.item))};
        return text.str();
    }
    if (std::holds_alternative<std::string>(v.item)) return std::get<std::string>(v.item);
    if (std::holds_alternative<bool>(v.item)) return std::get<bool>(v.item) ? "true" : "false";
    

    return "";

}

void Interpreter::checkNumberOperand(const Token& op, const Value& v) {
    if (std::holds_alternative<double>(v.item)) return;
    throw RuntimeError{op, "Operand must be a number."};
}

void Interpreter::checkNumberOperands(const Token& op, const Value& l, const Value& r) {
    if (std::holds_alternative<double>(l.item) && std::holds_alternative<double>(r.item)) return;
    throw RuntimeError{op,"Operands must be double."};
}

void Interpreter::checkAdditionOperation(const Token& op, const Value& l, const Value& r) {
    if (std::holds_alternative<double>(l.item) && std::holds_alternative<double>(r.item)) return;
    if (std::holds_alternative<std::string>(l.item) && std::holds_alternative<std::string>(r.item)) return;
    throw RuntimeError{op,"Operands must be double or string."};
}

} // Lox namespace