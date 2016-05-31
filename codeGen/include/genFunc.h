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
#ifndef __genFunc_H_
#define __genFunc_H_

#include <stdio.h>
#include "symbol.h"

#define FuncIsPassingConstVal     1
#define FuncNotPassingConstVal    0

void setFuncConstValFlag(void);
void clearFuncConstValFlag(void);
uint32_t isFuncPassConstVal(void);

void genFileTitle(FILE * file, char *file_name);
void genFuncProlog(Symbol *sym);
void genFuncEpilog(Symbol *sym);
void genFuncCall(Symbol *sym);

#endif
