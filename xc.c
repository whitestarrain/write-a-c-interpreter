#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int   token;           // current token
char *src, *old_src;   // pointer to source code string
int   poolsize;        // default size of text/data/stack
int   line;            // line number

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
    return 0;
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

    program();
    return eval();
}
