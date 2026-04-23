/*
 * File: program.cpp
 * -----------------
 * This file is a stub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.  Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */

#include "program.hpp"



Program::Program() = default;

Program::~Program() {
    clear();
}

void Program::clear() {
    for (auto& pair : parsedStatements) {
        delete pair.second;
    }
    sourceLines.clear();
    parsedStatements.clear();
}

void Program::addSourceLine(int lineNumber, const std::string &line) {
    // If line already exists, delete its parsed statement
    if (sourceLines.find(lineNumber) != sourceLines.end()) {
        auto it = parsedStatements.find(lineNumber);
        if (it != parsedStatements.end()) {
            delete it->second;
            parsedStatements.erase(it);
        }
    }

    sourceLines[lineNumber] = line;
}

void Program::removeSourceLine(int lineNumber) {
    auto sourceIt = sourceLines.find(lineNumber);
    if (sourceIt != sourceLines.end()) {
        sourceLines.erase(sourceIt);
    }

    auto parsedIt = parsedStatements.find(lineNumber);
    if (parsedIt != parsedStatements.end()) {
        delete parsedIt->second;
        parsedStatements.erase(parsedIt);
    }
}

std::string Program::getSourceLine(int lineNumber) {
    auto it = sourceLines.find(lineNumber);
    if (it != sourceLines.end()) {
        return it->second;
    }
    return "";
}

void Program::setParsedStatement(int lineNumber, Statement *stmt) {
    if (sourceLines.find(lineNumber) == sourceLines.end()) {
        error("Line number does not exist in program");
    }

    auto it = parsedStatements.find(lineNumber);
    if (it != parsedStatements.end()) {
        delete it->second;
    }

    parsedStatements[lineNumber] = stmt;
}

//void Program::removeSourceLine(int lineNumber) {

Statement *Program::getParsedStatement(int lineNumber) {
    auto it = parsedStatements.find(lineNumber);
    if (it != parsedStatements.end()) {
        return it->second;
    }
    return nullptr;
}

int Program::getFirstLineNumber() {
    if (sourceLines.empty()) {
        return -1;
    }
    return sourceLines.begin()->first;
}

int Program::getNextLineNumber(int lineNumber) {
    auto it = sourceLines.find(lineNumber);
    if (it == sourceLines.end()) {
        error("Line number does not exist in program");
    }

    ++it;
    if (it != sourceLines.end()) {
        return it->first;
    }
    return -1;
}

//more func to add
//todo


