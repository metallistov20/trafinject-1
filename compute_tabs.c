/*
 Routine to prepend each string of the input file with amount
 of 'tabs' (tabulation characters) found in this string.

 (C) Copyright 2015, TP-Link Inc, konstantin.mauch@tp-link.com

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License as
 published by the Free Software Foundation; either version 2 of
 the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT any WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 MA 02111-1307 USA
*/

/* printf(), getline() */
#include <stdio.h>

/* free() */
#include <stdlib.h>

int main(int argc, char *argv[])
{
/* Target char for which we will search in each line */
char c = '\t';

char * line = NULL;

size_t len = 0;

	while (-1 != getline(&line, &len, stdin))
	{
		int count = 0;

		char * s = line;

		/* For each string till the end of text file */
		while (*s)

			/* Check each symbol in the string */
			if(*s++ == c)

				/* If founf then increase counter */
				count++;

		/* Print to STDOUT: firstly - counter (anount of target chars found in this string), then - string itself */
		printf("%d %s", count, line);
	}

	/* Release mewory allocated by <getline> */
	if(line)
		free(line);

	return 0;
}
