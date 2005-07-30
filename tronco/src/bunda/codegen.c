#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stringqueue.h"

FILE * fpSaida;
int m_currentFunctionParams, m_left_side_memPos;
char m_operator;
char m_currentFunction[MAX_STRING_SIZE];
char m_functionToCall[MAX_STRING_SIZE];
StringQueue * stringsInSourceFile;
StringQueue * globals;

int c_if, i_if, c_while, i_while;

int h(int g) {
	return g;
}

void gen_if_prepare(void) {
	c_if++;
	i_if = c_if;
}

void gen_init(const char * fileName) {
	stringsInSourceFile = (StringQueue *)malloc(sizeof(StringQueue));
	stringsInSourceFile->queueEnd = -1;

	globals = (StringQueue *)malloc(sizeof(StringQueue));
	globals->queueEnd = -1;

	m_operator = 0;

	fpSaida = fopen("gerado.asm", "w");
	fprintf(fpSaida, "_$init:\n");
	fprintf(fpSaida, "\n");
}

void gen_function_prepare(const char * funcName) {
	strcpy(m_currentFunction, funcName);
	fprintf(fpSaida, "\t.type _%s, function\n", funcName);
	fprintf(fpSaida, "_%s:\n", funcName);
	fprintf(fpSaida, "\tpushl	%%ebp\n");
	fprintf(fpSaida, "\tmovl	%%esp,%%ebp\n");
	fprintf(fpSaida, "\tpushl	%%edi\n");
	fprintf(fpSaida, "\n");
}

void gen_prepare_call(const char * funcName) {
	m_currentFunctionParams = 0;
	strcpy(m_functionToCall, funcName);
}

void gen_add_param_var(const char varName[MAX_STRING_SIZE]) {
	m_currentFunctionParams += 4;
	fprintf(fpSaida, "\tpushl _%s\n", varName);
}

void gen_add_param_mem(const int memPos) {
	m_currentFunctionParams += 4;
	fprintf(fpSaida, "\tpushl %d(%%ebp)\n", memPos);
}

void gen_add_param_int(const int p_value) {
	m_currentFunctionParams += 4;
	fprintf(fpSaida, "\tpushl $ %d\n", p_value);
}

void gen_add_param_generic(const char paramChar[MAX_STRING_SIZE]) {
	m_currentFunctionParams += 4;
	fprintf(fpSaida, "\tpushl %s\n",paramChar);
}


void gen_add_param_char(const char paramChar[MAX_STRING_SIZE]) {
	int pos = StringQueue_insert(stringsInSourceFile, paramChar);
	m_currentFunctionParams += 4;
	fprintf(fpSaida, "\tpushl $_$str_%d\n", pos);
}

void gen_execute_call(void) {
	fprintf(fpSaida, "\tcall _%s\n", m_functionToCall);

	fprintf(fpSaida, "\taddl $%d,%%esp\n", m_currentFunctionParams);
//	fprintf(fpSaida, "\tmovl $0,%%eax\n");
}

void gen_end(void) {
	int i, j;

	for (i = 0; i <= globals->queueEnd; i++) {
		char element[MAX_STRING_SIZE] = "";
		StringQueue_getElementByIndex(globals, i, element);

		fprintf(fpSaida, "\t.globl	_%s\n", element);
		fprintf(fpSaida, "\t.type	_%s,object\n", element);
		fprintf(fpSaida, "\t.comm	_%s,4\n", element);
	}

	fprintf(fpSaida, "\t.extern	_printf\n");
	fprintf(fpSaida,"\t.data\n");

	for (i = 0; i <= stringsInSourceFile->queueEnd; i++) {
		char element[MAX_STRING_SIZE] = "";
		StringQueue_getElementByIndex(stringsInSourceFile, i, element);

		fprintf(fpSaida,"_$str_%d:\n", i);
		fprintf(fpSaida,"\t.byte ");

		for (j = 0; j < MAX_STRING_SIZE; j++) {
			if (element[j] == 0) break;
			fprintf(fpSaida,"%d,", element[j]);
		}

		fprintf(fpSaida, "0\n");

		printf("Elemento: %s\n", element);
	}
	fclose(fpSaida);
}

void gen_end_function(void) {
	fprintf(fpSaida, "_$prepare_end_%s:\n", m_currentFunction);
	fprintf(fpSaida, "\tpopl	%%edi\n");
	fprintf(fpSaida, "\tpopl	%%ebp\n");

	fprintf(fpSaida, "\tret\n");
	fprintf(fpSaida, "_$end_%s:\n", m_currentFunction);
	fprintf(fpSaida, "\t.globl	_%s\n", m_currentFunction);
}

void gen_declare_var(char p_name[MAX_STRING_SIZE]) {
	// TODO: symbol table
	StringQueue_insert(globals, p_name);
}

void gen_assign_string(char p_name[MAX_STRING_SIZE], char p_value[MAX_STRING_SIZE]) {
	int pos = StringQueue_insert(stringsInSourceFile, p_value);
	fprintf(fpSaida, "\tmovl	$_$str_%d,_%s\n", pos, p_name);
}

void gen_assign_int(char p_name[MAX_STRING_SIZE], int p_value) {
	fprintf(fpSaida, "\tmovl	$%d,_%s\n", p_value, p_name);
}



void gen_function_add_param(const char paramName[MAX_STRING_SIZE]) {

}

void gen_function_begin(void) {
}

void gen_exp_eval_var(int memPos) {
	char * instr;
	if (m_operator != 0) {
		switch(m_operator) {
			case '*': instr = "imull"; break;
			case '-': instr = "subl"; break;
		}
		m_operator = 0;
	} else {
		instr = "movl";
	}
	fprintf(fpSaida, "\t%s	%d(%%ebp),%%eax\n", instr, memPos);
}

void gen_exp_eval_operator_eq(void) {
	fprintf(fpSaida, "\tmovl	%%eax,%%esi\n");
}

void gen_exp_eval_int(int intValue) {
	char * instr;
	if (m_operator != 0) {
		switch(m_operator) {
			case '*': instr = "imull"; break;
			case '-': instr = "subl"; break;
		}
		m_operator = 0;
	} else {
		instr = "movl";
	}

	fprintf(fpSaida, "\t%s	$%d,%%eax\n", instr,intValue);
}

void gen_if_condition_end(void) {
	fprintf(fpSaida, "\tmovl	%%eax,%%edi\n");
	fprintf(fpSaida, "\tcmpl	%%esi,%%edi\n");
	fprintf(fpSaida, "\tjne		_$else_%d\n", i_if);
}

void gen_return_prepare(void) {
}

void gen_return_execute(void) {
	fprintf(fpSaida, "\tjmp		_$prepare_end_%s\n", m_currentFunction);
}

void gen_assignment_left_side(int memPos) {
	m_left_side_memPos = memPos;
}

void gen_assignment_operator(void) {

}

void gen_exp_eval_operator_mul(void) {
	m_operator = '*';
}

void gen_exp_eval_operator_sub(void) {
	m_operator = '-';
}

void gen_exp_eval_operator_gt(void) {
	fprintf(fpSaida, "\tmovl	%%eax,%%esi\n");
}

void gen_exp_eval_end(void) {
	fprintf(fpSaida, "\tmovl	%%eax,%d(%%ebp)\n", m_left_side_memPos);
}

void gen_boolexp_eval_end(void) {
	fprintf(fpSaida, "\tmovl	%%eax,%%edi\n");
	fprintf(fpSaida, "\tcmpl	%%esi,%%edi\n");
}

void gen_while_prepare(void) {
	c_while++;
	i_while = c_while;
	fprintf(fpSaida, "_$begin_while_%d:\n", c_while);
}

void gen_while_condition_end(void) {
	char * instruction = "jg";
	fprintf(fpSaida, "\t%s	_$while_end_%d\n", instruction, i_while);
}

void gen_while_end(void) {
	fprintf(fpSaida, "\tjmp	_$begin_while_%d\n", i_while);
	fprintf(fpSaida, "_$while_end_%d:\n", i_while);
	i_while--;
}

int main(void) {
	gen_init("hello");

	gen_declare_var("name");
	gen_declare_var("currentYear");

	gen_function_prepare("main");
	gen_function_begin();

		gen_assign_string("name", "James Brown");
		gen_assign_int("currentYear",2005);

		gen_prepare_call("display");
		gen_add_param_int(555);
		gen_execute_call();

	gen_end_function();

	gen_function_prepare("display");
	gen_function_begin();

		gen_prepare_call("fatorial2");
		gen_add_param_int(8);
		gen_execute_call();

		gen_prepare_call("printf");
		gen_add_param_generic("%eax");
		gen_add_param_var("currentYear");
		gen_add_param_var("name");
		gen_add_param_mem(8);
		gen_add_param_char("[%d  %s  %d %d]");
		gen_execute_call();
	gen_end_function();

	gen_function_prepare("fatorial");
		// TODO: symbol table
		gen_function_add_param("param1");
		gen_function_begin();

		// if (param == 0)

		gen_if_prepare();
		gen_exp_eval_var(8);
		gen_exp_eval_operator_eq();
		gen_exp_eval_int(0);
		gen_if_condition_end();

		gen_return_prepare();
		gen_exp_eval_int(1);
		gen_return_execute();

		// else
		fprintf(fpSaida, "_$else_%d:\n", i_if);

		// return param * fatorial(param-1)
		gen_prepare_call("fatorial");

		fprintf(fpSaida, "\tsubl	$1,%%esi\n");
		gen_add_param_generic("%esi");
		gen_execute_call();

		fprintf(fpSaida, "\timull	8(%%ebp),%%eax\n");
	gen_end_function();

	gen_function_prepare("fatorial2");
		// TODO: symbol table
		gen_function_add_param("param1");
		gen_function_begin();

		// int result = 1
		// fprintf(fpSaida, "\tmovl	$1,-4(%%ebp)\n");
		gen_assignment_left_side(-4);
		gen_assignment_operator();
		gen_exp_eval_int(1);
		gen_exp_eval_end();

		// while (param1 > 1) {
		gen_while_prepare();
		gen_exp_eval_var(8);
		gen_exp_eval_operator_gt();
		gen_exp_eval_int(1);
		gen_boolexp_eval_end();
		gen_while_condition_end();

		// result := result * param1;
		gen_assignment_left_side(-4);
		gen_assignment_operator();
		gen_exp_eval_var(-4);
		gen_exp_eval_operator_mul();
		gen_exp_eval_var(8);
		gen_exp_eval_end();

		// param1 = param1 - 1
		gen_assignment_left_side(8);
		gen_assignment_operator();
		gen_exp_eval_var(8);
		gen_exp_eval_operator_sub();
		gen_exp_eval_int(1);
		gen_exp_eval_end();

		gen_while_end();

		gen_return_prepare();
		gen_exp_eval_var(-4);
		gen_return_execute();

	gen_end_function();

	gen_end();
	printf("Gerando codigo");
	return 0;
}
