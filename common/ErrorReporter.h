#ifndef ERROR_REPORTER_H
#define ERROR_REPORTER_H

#include <string>

// ---------------------------------------------------------
// TODO: Centralized Error Handling (Crucial for high-level)
// A professional compiler needs beautiful, informative error
// messages indicating exactly where the user made a mistake 
// (Syntax Errors, Runtime Errors, etc.).
// ---------------------------------------------------------

class ErrorReporter {
public:
    static bool hadError;
    static bool hadRuntimeError;

    // Report a generic error at a specific line
    static void error(int line, const std::string& message);
    
    // Report exactly where the error occurred
    static void report(int line, const std::string& where, const std::string& message);
    
    // VM Execution errors (like dividing by zero)
    static void runtimeError(int line, const std::string& message);
};

#endif // ERROR_REPORTER_H
