/*
 * Copyright (c) 2016, Chang Jia-Rung, All Rights Reserved                                                 
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

#ifndef __STATEMENT_H_
#define __STATEMENT_H_
#include "token.h"

void statement(void);
void compound_statement(void);
uint32_t is_type_specifier(TOKEN tk_code);
void expression_statement(void);
void if_statement(void);
void for_statement(void);
void continue_statement(void);
void break_statement(void);
void return_statement(void);

#endif
