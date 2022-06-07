#include "util.h"
#include "prog1.h"
#include <stdio.h>

A_stm prog(void) {

return 
 A_CompoundStm(A_AssignStm("a",
                 A_OpExp(A_NumExp(5), A_plus, A_NumExp(3))),
  A_CompoundStm(A_AssignStm("b",
      A_EseqExp(A_PrintStm(A_PairExpList(A_IdExp("a"),
                 A_LastExpList(A_OpExp(A_IdExp("a"), A_minus, 
                                       A_NumExp(1))))),
              A_OpExp(A_NumExp(10), A_times, A_IdExp("a")))),
   A_PrintStm(A_LastExpList(A_IdExp("b")))));
}

int maxargs(A_stm stmt){
    if (stmt->kind == A_assignStm) {
        if (stmt->u.assign.exp->kind == A_eseqExp) {
            return maxargs(stmt->u.assign.exp->u.eseq.stm);
        }
        else {
            return 0;
        }
    }
    else if (stmt->kind == A_printStm) {
        A_expList expList = stmt->u.print.exps;
        int args = 1;
        while (expList->kind != A_lastExpList) {
            A_exp head = expList->u.pair.head;
            A_expList tail = expList->u.pair.tail;
            expList = tail;
            if (head->kind == A_eseqExp) {
                args += maxargs(head->u.eseq.stm);
            }
        }
        A_exp head = expList->u.pair.head;
        if (head->kind == A_eseqExp) {
            args += maxargs(head->u.eseq.stm);
        }
        return args;
    }
    else {
        A_stm left = stmt->u.compound.stm1;
        A_stm right = stmt->u.compound.stm2;
        return maxargs(left) + maxargs(right);
    }
}