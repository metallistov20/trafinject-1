// gcc ./compute_tabs.c -o ./compute_tabs

// printf()
#include <stdio.h>

// getline()
#include <string.h>

// free()
#include <stdlib.h>

int main(int argc, char *argv[])
{
char c = '\t';

char * line = NULL;

size_t len = 0;

        while (-1 != getline(&line, &len, stdin))
        {
                int count = 0;

                char * s = line;

                while (*s) if(*s++ == c)

			count++;

                printf("%d %s", count, line);		
        }

        if(line)

		free(line);
}
