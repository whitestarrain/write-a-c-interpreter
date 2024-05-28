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

// clang-format off
// instructions
enum
{
    LEA, IMM, JMP, CALL, JZ, JNZ, ENT, ADJ, LEV, LI, LC, SI, SC, PUSH,
    OR, XOR, AND, EQ, NE, LT, GT, LE, GE, SHL, SHR, ADD, SUB, MUL, DIV, MOD,
    OPEN, READ, CLOS, PRTF, MALC, MSET, MCMP, EXIT
};

// tokens and classes (operators last and in precedence order)
enum {
    Num=128, Fun, Sys, Glo, Loc, Id,
    Char, Else, Enum, If, Int, Return, Sizeof, While,
    Assign, Cond, Lor, Lan, Or, Xor, And, Eq, Ne, Lt, Gt, Le, Ge, Shl, Shr, Add, Sub, Mul, Div, Mod, Inc, Dec, Brak
};

int  token_val;    // value of current token (mainly for number)
int *current_id,   // current parsed ID
    *symbols;      // symbol table

// fields of identifier
enum {Token, Hash, Name, Type, Class, Value, BType, BClass, BValue, IdSize};

// type of variable/function
enum { CHAR, INT, PTR };
int *idmain;   // the `main` function

// clang-format on

/**
 * lexical analyzer
 * get next token
 */
void next()
{
    char *last_pos;
    int   hash; // hash value
    while ((token = *src)) {
        ++src;
        if (token == '\n') {
            // new line
            ++line;
        }
        else if (token == '#') {
            // skip macro, because we will not support it
            while (*src != 0 && *src != '\n') {
                src++;
            }
        }
        else if ((token >= 'a' && token <= 'z') || (token >= 'A' && token <= 'Z') || token == '_') {
            // parse identifier
            last_pos = src - 1;
            hash     = token;
            while ((*src >= 'a' && *src <= 'z') || (*src >= 'A' && *src <= 'Z') || (*src >= '0' && *src <= '9') || (*src == '_')) {
                hash = hash * 147 + *src;
                src++;
            }

            // look for existing identiifier, linear search
            current_id = symbols;
            while (current_id[Token]) {
                if (current_id[Hash] == hash &&
                    !memcmp((char *)current_id[Name], last_pos, src - last_pos)) {
                    // found one, return
                    token = current_id[Token];
                    return;
                }
                current_id = current_id + IdSize;
            }

            // store new ID
            current_id[Name] = (int)last_pos;   // 32-bit machine, sizeof(int) == sizeof(char *)
            current_id[Hash] = hash;
            token = current_id[Token] = Id;
            return;
        }
        else if (token >= '0' && token <= '9') {
            // parse number, three kinds: dec(123) hex(0x123) oct(017)
            token_val = token - '0';
            if (token_val > 0) {
                // dec, starts with [1-9]
                while (*src >= '0' && *src <= '9') {
                    token_val = token_val * 10 + *src++ - '0';
                }
            }
            else {
                // starts with number 0
                if (*src == 'x' || *src == 'X') {
                    // hex
                    token = *++src;
                    while ((token >= '0' && token <= '9') || (token >= 'a' && token <= 'f') ||
                           (token >= 'A' && token <= 'F')) {
                        // (token & 15) to get the hex single digit value of token (from c4)
                        token_val = token_val * 16 + (token & 15) + (token >= 'A' ? 9 : 0);
                        token     = *++src;
                    }
                }
                else {
                    // oct
                    while (*src >= '0' && *src <= '7') {
                        token_val = token_val * 8 + *src++ - '0';
                    }
                }
            }
            token = Num;
            return;
        }
        else if (token == '"' || token == '\'') {
            // parse string literal, currently, the only supported escape character is '\n',
            // store the string literal into data
            last_pos = data;
            while (*src != 0 && *src != token) {
                token_val = *src++;
                if (token_val == '\\') {
                    // escape character
                    token_val = *src++;
                    if (token_val == 'n') {
                        token_val = '\n';
                    }
                }
                if (token == '"') {
                    *data++ = token_val;
                }
            }
            src++;
            if (token == '"') {
                // token is string, token_val is string address
                token_val = (int)last_pos;
            }
            else {
                // if it is a single character, return Num token
                token = Num;
            }
            return;
        }
        else if (token == '/') {
            // comments or divide operator
            if (*src == '/') {
                // skip comments
                while (*src != 0 && *src != '\n') {
                    ++src;
                }
            }
            else {
                // divide operator
                token = Div;
                return;
            }
        }
        else if (token == '=') {
            // parse '==' and '='
            if (*src == '=') {
                src++;
                token = Eq;
            }
            else {
                token = Assign;
            }
            return;
        }
        else if (token == '+') {
            // parse '+' and '++'
            if (*src == '+') {
                src++;
                token = Inc;
            }
            else {
                token = Add;
            }
            return;
        }
        else if (token == '-') {
            // parse '-' and '--'
            if (*src == '-') {
                src++;
                token = Dec;
            }
            else {
                token = Sub;
            }
            return;
        }
        else if (token == '!') {
            // parse '!='
            if (*src == '=') {
                src++;
                token = Ne;
            }
            return;
        }
        else if (token == '<') {
            // parse '<=', '<<', '<'
            if (*src == '=') {
                src++;
                token = Le;
            }
            else if (*src == '<') {
                src++;
                token = Shl;
            }
            else {
                token = Lt;
            }
            return;
        }
        else if (token == '>') {
            // parse '>=', '>>', '>'
            if (*src == '=') {
                src++;
                token = Ge;
            }
            else if (*src == '>') {
                src++;
                token = Shr;
            }
            else {
                token = Gt;
            }
            return;
        }
        else if (token == '|') {
            // parse '|', '||'
            if (*src == '|') {
                src++;
                token = Lor;
            }
            else {
                token = Or;
            }
            return;
        }
        else if (token == '&') {
            // parse '&&', '&'
            if (*src == '&') {
                src++;
                token = Lan;
            }
            else {
                token = And;
            }
            return;
        }
        else if (token == '^') {
            token = Xor;
            return;
        }
        else if (token == '%') {
            token = Mod;
            return;
        }
        else if (token == '*') {
            token = Mul;
            return;
        }
        else if (token == '[') {
            token = Brak;
            return;
        }
        else if (token == '?') {
            token = Cond;
            return;
        }
        else if (token == '~' || token == ';' || token == '{' || token == '}' || token == '(' ||
                 token == ')' || token == ']' || token == ',' || token == ':') {
            // directly return the character as token;
            return;
        }
    }
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

    /* allocate memory for virtual machine */
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
    if ((symbols = malloc(poolsize)) < 0) {
        printf("could not malloc(%d) for symbols table", poolsize);
        return -1;
    }
    memset(text, 0, poolsize);
    memset(data, 0, poolsize);
    memset(stack, 0, poolsize);

    /* initialization registers */
    bp = sp = (int *)((int)stack + poolsize);
    ax      = 0;

    /* add keywords to symbol table */
    src = "char else enum if int return sizeof while open read close printf malloc memset memcmp "
          "exit void main";
    i   = Char;
    // add keywords to symbol table
    while (i <= While) {
        next();
        current_id[Token] = i++;
    }
    // add library to symbol table
    i = OPEN;
    while (i <= EXIT) {
        next();
        current_id[Class] = Sys;
        current_id[Type]  = INT;
        current_id[Value] = i++;
    }
    // clang-format off
    next(); current_id[Token] = Char;   // handle void type
    next(); idmain = current_id;        // keep track of main
    // clang-format on

    /* open and read source file */
    if ((fd = open(*argv, 0)) < 0) {
        printf("could not open(%s)\n", *argv);
        return -1;
    }
    if (!(src = old_src = malloc(poolsize))) {
        printf("could not malloc(%d) for souce area\n", fd);
        return -1;
    }
    if ((i = read(fd, src, poolsize - 1)) < 0) {
        printf("read() return %d\n", i);
        return -1;
    }
    src[i] = 0;   // add EOF character
    close(fd);

    program();
    return eval();
}
