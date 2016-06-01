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
#include <string.h>
#include "genVar.h"
#include "symbol.h"
#include "exception.h"
#include "var_storage.h"
#include "global_var.h"
#include "genInstr.h"
#include "token.h"
#include "operand.h"


void genGlobalVar(Symbol *sym)
{

	/*for const char array*/
	if ((sym->storage_type & JC_CONST) && (sym->type.data_type & T_ARRAY)) {
		sym->relation = const_STR_index;
		const_STR_index++;
		asmPrintf("\n    .section    .rodata\n");
		asmPrintf("    .align 8\n");
		asmPrintf(".LC%d:\n", const_STR_index);
		asmPrintf("    .string  %s\n\n", sourceSTR.data);
		return;
	}

	uint32_t align, size, data_type;
	char *var_name = get_tkSTR(sym->tk_code);;
	char *data_length;

	data_type = sym->type.data_type & JC_ValMASK;
	switch (data_type) {
	case T_INT:
		align = 4;
		size = 4;
		data_length = dSIZE_32bits;
		break;
	case T_PTR:
		align = 8;
		size = 8;
		data_length = dSIZE_64bits;
		break;
	case T_CHAR:
		align = 1;
		size = 1;
		data_length = dSIZE_8bits;
		break;
	case T_STRUCT:
		align = (sym->type.ref->storage_type & 0xF);
		size = (sym->type.ref->relation & 0xFFFF);
		break;
	default:
		error("UNDEFIND variable allocation!");
		break;
	}

	/*Has init value(always),even non-init value also give 0*/
	if (1) {
		asmPrintf("    .globl   %s\n", var_name);
		asmPrintf("    .data\n");
		asmPrintf("    .align   %d\n", align);
		asmPrintf("    .type    %s, @object\n", var_name);
		asmPrintf("    .size    %s, %d\n", var_name, size);
		asmPrintf("%s:\n", var_name);
		if (data_type != T_STRUCT) {
			asmPrintf("    .%s    %d\n\n", data_length, sym->relation);
		} else {
			error("JCC does not support initialization with struct declaration");
		}


	} else { /*Without init value*/
		//asmPrintf(".comm   %s,%d,%d\n\n", var_name, size, align);
	}
}

/*when leaving the current local scope*/
void clearFP_offset(void)
{
	FP_offset = 0;
}


static void updateSYM_FPoffset(Symbol *sym, uint32_t size)
{
	FP_offset -= size;
	sym->fp_offset = FP_offset;
}


void genLocalVar(Symbol *sym)
{
	uint32_t size, data_type;

	data_type = sym->type.data_type & JC_ValMASK;
	switch (data_type) {
	case T_INT:
	case T_PTR:
	case T_CHAR:
		size = 8;
		break;
	case T_STRUCT:
		size = (sym->type.ref->relation & 0xFFFF);
		break;
	default:
		error("UNDEFIND variable allocation!");
		break;
	}

	asmPrintf("    subq    $%d, %%rsp\n", size);
	updateSYM_FPoffset(sym, size);
	instrMOV_VAL_OFFSET(size, sym->relation, "rbp", sym->fp_offset);
	asmPrintf("\n");
}

void genVar(void)
{
	Symbol *pS;
	pS = opTop->sym;
	/*Avoiding gen code for "mother/symbol type" */
	if (!(pS->tk_code & JC_SymTypeMASK)) {
		if ((pS->storage_type & JC_GLOBAL) && (pS->storage_type & JC_LVAL)) {
			genGlobalVar(pS);
		} else if ((pS->storage_type & JC_LOCAL) && (pS->storage_type & JC_LVAL)) {
			genLocalVar(pS);
		}
		operand_pop();
	}
}

void genAssign(void)
{
	Symbol *Lsym = opTop[-1].sym;
	Symbol *ret_sym = opTop->sym;
	uint32_t temp_reg;
	char dest_reg[4];

	temp_reg = FindFreeReg();
	assignReg(temp_reg);

	/*if there is return operand, get it*/
	if (ret_sym->storage_type & JC_RET_REG) {
		instrMOV_REG_REG(8, reg_pool[REG_RAX].reg_name, reg_pool[temp_reg].reg_name);
		FreeReg(REG_RAX);
	}



	if (Lsym->storage_type & JC_GLOBAL) {
		strcpy(dest_reg, "rip");
		instrMOV_REG_symOFFSET(8, get_tkSTR(Lsym->tk_code & ~JC_SymTypeMASK), reg_pool[temp_reg].reg_name, dest_reg);
	} else if (Lsym->storage_type & JC_LOCAL) {
		strcpy(dest_reg, "rbp");
		instrMOV_REG_OFFSET(8, reg_pool[temp_reg].reg_name, dest_reg, Lsym->fp_offset);
	} else {
		interERROR("genAssign() error!");
	}

	FreeReg(temp_reg);

}


