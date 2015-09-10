#include <stdio.h>

#include <sys/stat.h>

#define FNAME "cast.5428E.txt.voc"

int main()
{             
FILE *fp;

unsigned long ulo;

struct stat fileStats;

	if (stat(FNAME, &fileStats) != 0)
	{

		printf("/* Can't stat file %s*/ \n", FNAME);

		return -2;

	}
	else
	{
		fp=fopen(FNAME,"r");

		if(NULL == fp)
		{
			printf("/* Some problem on FIRST opening the file %s*/ ", FNAME );

			return -1;
		} 
		else
		{ 
		int i;
		char c[512];

			printf("/* This is AUTOGENERATED file. Do NOT edit it. */\n");

			printf("\n#include \"constants.h\"\n");

			printf("\n#include \"auxiliary.h\"\n\n");

			while ( !feof(fp) )
			{
				i++;

				fscanf(fp, "%s\n", &c);

				printf("/* variable for %d%s keyword found in <%s> file */\n", i, (i==1)?"st":((i==2)?"nd":((i==3)?"rd":"th")), FNAME );

				printf("char %s [MAX_STR_SIZE];\n\n", &c);
			}
			fclose(fp);

			fp=fopen(FNAME,"r");

			if(NULL == fp)
			{
				printf("/* Some problem on SECOND opening the file %s*/ ", FNAME );

				return -1;
			} 
			else
			{ 
			char d[512];

				printf("/* This array provides correspondency between name of variable name and its address %s*/ \n");

				printf("struct _QuineType QuineArray[] =\n{\n");

				while ( !feof(fp) )
				{
					fscanf(fp, "%s\n", &d);

					printf("\t{ &%s   , \"_tid_\" },\n\n", &d);
				}

				fclose(fp);

				printf("\t/* End of array */\n\t{0, 0}\n};\n/* End of AUTOGENERATed file */");
			}	
		}
	}

	return 0;  

}
