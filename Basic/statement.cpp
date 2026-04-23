/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include "statement.hpp"
#include <iostream>
#include <cstdlib>

/* Implementation of the Statement class */

int stringToInt(std::string str);

Statement::Statement() = default;

Statement::~Statement() = default;

// RemStatement implementation
RemStatement::RemStatement(const std::string& comment) : comment(comment) {}

void RemStatement::execute(EvalState &state, Program &program) {
    // REM statements do nothing
}

// LetStatement implementation
LetStatement::LetStatement(const std::string& var, Expression* exp)
    : variable(var), expression(exp) {}

LetStatement::~LetStatement() {
    delete expression;
}

void LetStatement::execute(EvalState &state, Program &program) {
    int value = expression->eval(state);
    state.setValue(variable, value);
}

// PrintStatement implementation
PrintStatement::PrintStatement(Expression* exp) : expression(exp) {}

PrintStatement::~PrintStatement() {
    delete expression;
}

void PrintStatement::execute(EvalState &state, Program &program) {
    int value = expression->eval(state);
    std::cout << value << std::endl;
}

// InputStatement implementation
InputStatement::InputStatement(const std::string& var) : variable(var) {}

void InputStatement::execute(EvalState &state, Program &program) {
    int value;
    std::cin >> value;
    state.setValue(variable, value);
}

// EndStatement implementation
EndStatement::EndStatement() {}

void EndStatement::execute(EvalState &state, Program &program) {
    exit(0);
}

// GotoStatement implementation
GotoStatement::GotoStatement(int line) : lineNumber(line) {}

void GotoStatement::execute(EvalState &state, Program &program) {
    // This will be handled by the program execution logic
    // For now, we'll just set the next line in the EvalState
    state.setCurrentLine(lineNumber);
}

// IfStatement implementation
IfStatement::IfStatement(Expression* exp1, const std::string& op, Expression* exp2, int line)
    : lhs(exp1), rhs(exp2), op(op), targetLine(line) {}

IfStatement::~IfStatement() {
    delete lhs;
    delete rhs;
}

void IfStatement::execute(EvalState &state, Program &program) {
    int left = lhs->eval(state);
    int right = rhs->eval(state);
    bool condition = false;

    if (op == "=") {
        condition = (left == right);
    } else if (op == "<") {
        condition = (left < right);
    } else if (op == ">") {
        condition = (left > right);
    } else if (op == "<>") {
        condition = (left != right);
    } else if (op == "<=") {
        condition = (left <= right);
    } else if (op == ">=") {
        condition = (left >= right);
    }

    if (condition) {
        state.setCurrentLine(targetLine);
    }
}

// RunStatement implementation
RunStatement::RunStatement() {}

void RunStatement::execute(EvalState &state, Program &program) {
    int currentLine = program.getFirstLineNumber();
    if (currentLine == -1) {
        return; // Empty program
    }

    while (currentLine != -1) {
        state.setCurrentLine(-1); // Reset current line
        Statement* stmt = program.getParsedStatement(currentLine);
        if (stmt == nullptr) {
            error("SYNTAX ERROR");
        }

        stmt->execute(state, program);

        int nextLine = state.getCurrentLine();
        if (nextLine != -1) {
            // GOTO or IF-THEN was executed
            currentLine = nextLine;
        } else {
            // Normal flow - go to next line
            currentLine = program.getNextLineNumber(currentLine);
        }
    }
}

// ListStatement implementation
ListStatement::ListStatement() {}

void ListStatement::execute(EvalState &state, Program &program) {
    int currentLine = program.getFirstLineNumber();
    while (currentLine != -1) {
        std::cout << program.getSourceLine(currentLine) << std::endl;
        currentLine = program.getNextLineNumber(currentLine);
    }
}

// ClearStatement implementation
ClearStatement::ClearStatement() {}

void ClearStatement::execute(EvalState &state, Program &program) {
    program.clear();
    state.clear();
}

// QuitStatement implementation
QuitStatement::QuitStatement() {}

void QuitStatement::execute(EvalState &state, Program &program) {
    // Clean up before exit
    program.clear();
    state.clear();
    std::exit(0);
}
