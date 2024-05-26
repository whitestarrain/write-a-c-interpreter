#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// default size of text/data/stack
int poolsize;

// read source code
int   token;           // current token
char *src, *old_src;   // pointer to source code string
int   line;            // line number

// runtime struction
int *text,       // text segment
    *old_text,   // for dump text segment
    *stack;      // stack
char *data;      // data segment

// registers
int *pc,   // program counter
    *bp,   // point to the bottom of stack
    *sp,   // point to the top of stack
    ax,    // register to store the results of calculations
    cycle;

// instructions
// clang-format off
enum
{
    LEA, IMM, JMP, CALL, JZ, JNZ, ENT, ADJ, LEV, LI, LC, SI, SC, PUSH,
    OR, XOR, AND, EQ, NE, LT, GT, LE, GE, SHL, SHR, ADD, SUB, MUL, DIV, MOD,
    OPEN, READ, CLOS, PRTF, MALC, MSET, MCMP, EXIT
};
// clang-format on

/**
 * get next token
 */
void next()
{
    token = *src++;
    return;
}

/**
 * analytical expression
 */
void expression() {}

/**
 * program entry
 */
void program()
{
    next();
    while (token > 0) {
        printf("token is %c\n", token);
        next();
    }
}

/**
 * virtual machine entry
 */
int eval()
{
    int op, *tmp;
    while (1) {
        op = *pc++;

        /* MOV */
        if (op == IMM) {
            // load immediate value to ax
            ax = *pc++;
        }
        else if (op == LC) {
            // load character to ax, address in ax
            ax = *(char *)ax;
        }
        else if (op == LI) {
            // load integer to ax, address in ax
            ax = *(int *)ax;
        }
        else if (op == SC) {
            // save character to address, value in ax, address on stack
            ax = *(char *)*sp++ = ax;
        }
        else if (op == SI) {
            // save integer to address, value in ax, address on stack
            *(int *)*sp++ = ax;
        }

        // PUSH
        else if (op == PUSH) {
            // push the value of ax into the stack
            *--sp = ax;
        }

        // JMP <addr>
        else if (op == JMP) {
            // jump to the address
            pc = (int *)*pc;
        }

        /* JGE, CMPL */
        else if (op == JZ) {
            // jump if ax is zero
            pc = ax ? pc + 1 : (int *)*pc;
        }
        else if (op == JNZ) {
            // jump if ax is not zero
            pc = ax ? (int *)*pc : pc + 1;
        }

        // CALL
        else if (op == CALL) {
            // call subroutine
            *--sp = (int)(pc + 1);   // push pc to stack
            pc    = (int *)*pc;      // jump
        }

        // ENT <size>
        else if (op == ENT) {
            // make new stack frame
            *--sp = (int)bp;      // push ebp
            bp    = sp;           // mov ebp, esp
            sp    = sp - *pc++;   // sub <size>, esp // space for variable
        }

        // ADJ <size>
        else if (op == ADJ) {
            // remove arguments from frame
            sp = sp + *pc++;   // add esp, <size>
        }

        /* mov + pop + ret */
        // LEV
        else if (op == LEV) {
            // leave current frame, restore call frame and pc
            sp = bp;             // mov esp, ebp
            bp = (int *)*sp++;   // pop ebp
            pc = (int *)*sp++;   // ret
        }

        // LEA <offset>
        else if (op == LEA) {
            // load address for argument
            ax = (int)(bp + *pc++);
        }

        /* operators */
        // clang-format off
        else if (op == OR) ax = *sp++ | ax;
        else if (op == XOR) ax = *sp++ ^ ax;
        else if (op == AND) ax = *sp++ & ax;
        else if (op == EQ) ax = *sp++ == ax;
        else if (op == NE) ax = *sp++ != ax;
        else if (op == LT) ax = *sp++ < ax;
        else if (op == LE) ax = *sp++ <= ax;
        else if (op == GT) ax = *sp++ > ax;
        else if (op == GE) ax = *sp++ >= ax;
        else if (op == SHL) ax = *sp++ << ax;
        else if (op == SHR) ax = *sp++ >> ax;
        else if (op == ADD) ax = *sp++ + ax;
        else if (op == SUB) ax = *sp++ - ax;
        else if (op == MUL) ax = *sp++ * ax;
        else if (op == DIV) ax = *sp++ / ax;
        else if (op == MOD) ax = *sp++ % ax;
        // clang-format on

        /* builtin function */
        else if (op == EXIT) {
            printf("exit(%d)", *sp);
            return *sp;
        }
        else if (op == OPEN) {
            ax = open((char *)sp[1], sp[0]);
        }
        else if (op == CLOS) {
            ax = close(*sp);
        }
        else if (op == READ) {
            ax = read(sp[2], (char *)sp[1], *sp);
        }
        else if (op == PRTF) {
            tmp = sp + pc[1];
            ax  = printf((char *)tmp[-1], tmp[-2], tmp[-3], tmp[-4], tmp[-5], tmp[-6]);
        }
        else if (op == MALC) {
            ax = (int)malloc(*sp);
        }
        else if (op == MSET) {
            ax = (int)memset((char *)sp[2], sp[1], sp[0]);
        }
        else if (op == MCMP) {
            ax = memcmp((char *)sp[2], (char *)sp[1], sp[0]);
        }

        // others
        else {
            printf("unknown instruction: %d\n", op);
            return -1;
        }
    }
}

int main(int argc, char *argv[])
{
    int i, fd;
    argc--;
    argv++;

    poolsize = 256 * 1024;
    line     = 1;

    // open source file
    if ((fd = open(*argv, 0)) < 0) {
        printf("could not open(%s)\n", *argv);
        return -1;
    }

    if (!(src = old_src = malloc(poolsize))) {
        printf("could not malloc(%d) for souce area\n", fd);
        return -1;
    }

    // read the source file
    if ((i = read(fd, src, poolsize - 1)) < 0) {
        printf("read() return %d\n", i);
        return -1;
    }
    src[i] = 0;   // add EOF character
    close(fd);

    // allocate memory for virtual machine
    if ((text = old_text = malloc(poolsize)) < 0) {
        printf("could not malloc(%d) for text area", poolsize);
        return -1;
    }
    if ((data = malloc(poolsize)) < 0) {
        printf("could not malloc(%d) for data area", poolsize);
        return -1;
    }
    if ((stack = malloc(poolsize)) < 0) {
        printf("could not malloc(%d) for stack area", poolsize);
        return -1;
    }
    memset(text, 0, poolsize);
    memset(data, 0, poolsize);
    memset(stack, 0, poolsize);

    // initialization registers
    bp = sp = (int *)((int)stack + poolsize);
    ax      = 0;

    i         = 0;
    text[i++] = IMM;
    text[i++] = 10;
    text[i++] = PUSH;
    text[i++] = IMM;
    text[i++] = 20;
    text[i++] = ADD;
    text[i++] = PUSH;
    text[i++] = EXIT;
    pc        = text;

    program();
    return eval();
}
