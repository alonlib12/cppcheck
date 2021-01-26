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

#include "checkselfassignment.h"

#include "astutils.h"
#include "errorlogger.h"
#include "library.h"
#include "settings.h"
#include "symboldatabase.h"
#include "token.h"
#include "tokenize.h"
#include "utils.h"

#include <algorithm>
#include <cstddef>
#include <set>

 //---------------------------------------------------------------------------
using namespace std;

// Register this check class into cppcheck by creating a static instance of it..
namespace {
	CheckSelfAssignment instance;
}

// CWE ID used:
static const CWE CWE398(398U);  // Indicator of Poor Code Quality
static const CWE CWE401(401U);  // Improper Release of Memory Before Removing Last Reference ('Memory Leak')
static const CWE CWE771(771U);  // Missing Reference to Active Allocated Resource
static const CWE CWE772(772U);  // Missing Release of Resource after Effective Lifetimes

static const std::set<std::string> deallocations_func_list = {
    "delete", "free"    
};


#include <iostream>
void CheckSelfAssignment::check()
{
	const SymbolDatabase* symbolDatabase = mTokenizer->getSymbolDatabase();    
    for (const Scope* scope : symbolDatabase->classAndStructScopes)
    {
        for (const Token* tok = scope->bodyStart; tok && tok != scope->bodyEnd; tok = tok->next())
        {
            if (tok->str() == "operator=")
            {
                cout << "isassignmentOp() " << tok->isAssignmentOp() << endl;
                auto func = tok->function();
                if (func)
                {
                    const Token* argTok;
                    if (Token::Match(func->argDef, "( const Bug & %name%"))
                    {
                        argTok = func->argDef->tokAt(4);
                    }
                    else
                    {
                        continue;
                    }
                    auto fieldList = func->functionScope->functionOf->varlist;
                    bool insideSelfCheckScope = false;
                    string selfCheckPattern = "if ( this != & " + argTok->str();
                    const Token* endOfSelfCheckScope = NULL;
                    for (const Token* itr = func->functionScope->bodyStart; itr && itr != func->functionScope->bodyEnd; itr = itr->next())
                    {

                        if (Token::Match(itr, selfCheckPattern.c_str()))
                        {
                            insideSelfCheckScope = true;
                            const Token* parenthesisTok = itr->tokAt(7);
                            if (parenthesisTok && parenthesisTok->str() == "{")
                            {
                                endOfSelfCheckScope = parenthesisTok->link();
                            }
                            else
                            {
                                const Token* iter;
                                for (iter = parenthesisTok; iter && iter->str() != ";"; iter = iter->next())
                                {}
                                if (iter)
                                {
                                    endOfSelfCheckScope = iter;
                                }
                            }
                        }
                        else if (itr == endOfSelfCheckScope)
                        {
                            insideSelfCheckScope = false;
                        }
                        else if (!insideSelfCheckScope && deallocations_func_list.find(itr->str()) != deallocations_func_list.end())
                        {
                            if (itr->next() && fieldList.find(itr->next())) //ALON CONTINUE HERE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                            selfAssignmentError(itr);
                        }                        
                    }
                }
            }
        }
    }
}



void CheckSelfAssignment::selfAssignmentError(const Token* tok)
{
	reportError(tok, Severity::error, "selfAssignmentError",
		"Assignment operator self assignment issue "
		, CWE398, false);
}

