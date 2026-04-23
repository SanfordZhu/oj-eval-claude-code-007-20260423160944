/*
 * File: Basic.cpp
 * ---------------
 * This file is the starter project for the BASIC interpreter.
 */

#include <cctype>
#include <iostream>
#include <string>
#include <cstdlib>
#include "exp.hpp"
#include "parser.hpp"
#include "program.hpp"
#include "statement.hpp"
#include "Utils/error.hpp"
#include "Utils/tokenScanner.hpp"
#include "Utils/strlib.hpp"


/* Function prototypes */

void processLine(std::string line, Program &program, EvalState &state);

/* Main program */

int main() {
    EvalState state;
    Program program;
    //cout << "Stub implementation of BASIC" << endl;
    while (true) {
        try {
            std::string input;
            getline(std::cin, input);
            if (!input.empty() && input != "\r") {
                processLine(input, program, state);
            }
        } catch (ErrorException &ex) {
            std::cout << ex.getMessage() << std::endl;
        }
    }
    return 0;
}

/*
 * Function: processLine
 * Usage: processLine(line, program, state);
 * -----------------------------------------
 * Processes a single line entered by the user.  In this version of
 * implementation, the program reads a line, parses it as an expression,
 * and then prints the result.  In your implementation, you will
 * need to replace this method with one that can respond correctly
 * when the user enters a program line (which begins with a number)
 * or one of the BASIC commands, such as LIST or RUN.
 */

Statement* parseStatement(TokenScanner& scanner);

void processLine(std::string line, Program &program, EvalState &state) {
    TokenScanner scanner;
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    scanner.setInput(line);

    // Check if this is a numbered line
    if (!scanner.hasMoreTokens()) {
        return;
    }

    std::string firstToken = scanner.nextToken();
    TokenType firstTokenType = scanner.getTokenType(firstToken);

    if (firstTokenType == NUMBER) {
        // This is a numbered line
        int lineNumber = stringToInteger(firstToken);

        // Check if there's more content after the line number
        if (scanner.hasMoreTokens()) {
            // Add or replace the line
            program.addSourceLine(lineNumber, line);

            // Parse the statement part
            Statement* stmt = parseStatement(scanner);
            if (stmt != nullptr) {
                program.setParsedStatement(lineNumber, stmt);
            }
        } else {
            // Empty line number - remove the line
            program.removeSourceLine(lineNumber);
        }
    } else {
        // This is a direct command
        scanner.saveToken(firstToken);
        Statement* stmt = parseStatement(scanner);
        if (stmt != nullptr) {
            stmt->execute(state, program);
            delete stmt;
        }
    }
}

Statement* parseStatement(TokenScanner& scanner) {
    if (!scanner.hasMoreTokens()) {
        return nullptr;
    }

    std::string cmd = scanner.nextToken();
    std::string cmdUpper = toUpperCase(cmd);

    if (cmdUpper == "REM") {
        // REM statement - rest of line is comment
        std::string comment;
        while (scanner.hasMoreTokens()) {
            comment += scanner.nextToken() + " ";
        }
        return new RemStatement(comment);
    } else if (cmdUpper == "LET") {
        // LET var = expression
        if (!scanner.hasMoreTokens()) error("SYNTAX ERROR");
        std::string var = scanner.nextToken();

        if (!scanner.hasMoreTokens() || scanner.nextToken() != "=") error("SYNTAX ERROR");

        Expression* exp = parseExp(scanner);
        return new LetStatement(var, exp);
    } else if (cmdUpper == "PRINT") {
        // PRINT expression
        Expression* exp = parseExp(scanner);
        return new PrintStatement(exp);
    } else if (cmdUpper == "INPUT") {
        // INPUT var
        if (!scanner.hasMoreTokens()) error("SYNTAX ERROR");
        std::string var = scanner.nextToken();
        return new InputStatement(var);
    } else if (cmdUpper == "END") {
        return new EndStatement();
    } else if (cmdUpper == "GOTO") {
        // GOTO lineNumber
        if (!scanner.hasMoreTokens()) error("SYNTAX ERROR");
        std::string lineStr = scanner.nextToken();
        int lineNum = stringToInteger(lineStr);
        return new GotoStatement(lineNum);
    } else if (cmdUpper == "IF") {
        // IF exp1 op exp2 THEN line
        // Parse first expression
        Expression* exp1 = readE(scanner);

        if (!scanner.hasMoreTokens()) error("SYNTAX ERROR");
        std::string op = scanner.nextToken();

        // Check for two-character operators
        if (op == "<" || op == ">") {
            if (scanner.hasMoreTokens()) {
                std::string next = scanner.nextToken();
                if (next == "=" || (op == "<" && next == ">")) {
                    op += next;
                } else {
                    scanner.saveToken(next);
                }
            }
        }

        // Validate operator
        if (op != "=" && op != "<>" && op != "<" && op != ">" && op != "<=" && op != ">=") {
            error("SYNTAX ERROR");
        }

        // Parse second expression
        Expression* exp2 = readE(scanner);

        if (!scanner.hasMoreTokens() || toUpperCase(scanner.nextToken()) != "THEN") error("SYNTAX ERROR");

        if (!scanner.hasMoreTokens()) error("SYNTAX ERROR");
        int targetLine = stringToInteger(scanner.nextToken());

        return new IfStatement(exp1, op, exp2, targetLine);
    } else if (cmdUpper == "RUN") {
        return new RunStatement();
    } else if (cmdUpper == "LIST") {
        return new ListStatement();
    } else if (cmdUpper == "CLEAR") {
        return new ClearStatement();
    } else if (cmdUpper == "QUIT") {
        return new QuitStatement();
    } else if (cmdUpper == "HELP") {
        // HELP is optional, just ignore it
        return nullptr;
    } else {
        error("SYNTAX ERROR");
    }

    return nullptr;
}

