/*
 * Cppcheck - A tool for static C/C++ code analysis
 * Copyright (C) 2007-2020 Cppcheck team.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

 //---------------------------------------------------------------------------
#ifndef checkselfassignmentH
#define checkselfassignmentH
//---------------------------------------------------------------------------

#include "check.h"

class CPPCHECKLIB CheckSelfAssignment : public Check {
public:

    /** This constructor is used when registering the CheckClass */
    CheckSelfAssignment() : Check(myName()) {
    }

    /** This constructor is used when running checks. */
    CheckSelfAssignment(const Tokenizer* tokenizer, const Settings* settings, ErrorLogger* errorLogger)
        : Check(myName(), tokenizer, settings, errorLogger) {
    }

    void check();

    /** @brief Run checks against the normal token list */
    void runChecks(const Tokenizer* tokenizer, const Settings* settings, ErrorLogger* errorLogger) OVERRIDE {
        CheckSelfAssignment checkSelfAssignment(tokenizer, settings, errorLogger);
        // Checks
        checkSelfAssignment.check();
    }    

    std::string classInfo() const OVERRIDE {
        return "Assignment operator should check for self assignment";
    }
    

private:
    void getErrorMessages(ErrorLogger* errorLogger, const Settings* settings) const OVERRIDE {
        ErrorPath errorPath;
        CheckSelfAssignment checkSelfAssignment(nullptr, settings, errorLogger);
    }

    void selfAssignmentError(const Token* tok);
    static std::string myName() {
        return "CheckSelfAssignment";
    }    
};

#endif // checkselfassignmentH