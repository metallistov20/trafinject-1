/*
 Routine to prepend each string of the input file with amount
 of special characters ( tabulation sign) found in this string.

 (C) Copyright 2015, TP-Link Inc, konstantin.mauch@tp-link.com

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License as
 published by the Free Software Foundation; either version 2 of
 the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT cAby WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 MA 02111-1307 USA
*/

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
