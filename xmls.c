/*
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

#include <string.h>

#include <libxml/parser.h>
#include <libxml/tree.h>

#include "lists.h"
#include "xmls.h"


/* Static array in scope of curr. file. We store aux strings StrTokIdx() inside  */
static char * aLastToken[3];

/* Variable to store URL list for current command */
static pUrlChainType  pUrlChain;


static char * strTokIdx(char *s1, const char *delimit, int iIdx)
{
char ** lastToken;
char *tmp;

    lastToken =  &(aLastToken[iIdx]) ;
//TODO: why can't initialile here?    *lastToken = NULL;

    /* Skip leading delimiters if new string. */
    if ( s1 == NULL )
    {
        s1 = *lastToken;

        if (s1 == NULL)

            return NULL;
    }
    else
    {
        s1 += strspn(s1, delimit);
    }

    /* Find end of segment */
    tmp = strpbrk(s1, delimit);

    if (tmp)
    {
        /* Found another delimiter, split string and save state. */
        *tmp = '\0';

        *lastToken = tmp + 1;    }
    else
    {
        /* Last segment, remember that. */
        *lastToken = NULL;
    }

    return s1;
}



static void _unat(char * tkn)
{
char *_localCopy;

char *_localToken;

int iChunked = -1;

pUrlChainType  pUrlChainTmp;

	pUrlChainTmp = (pUrlChainType)CreateUrl(&pUrlChain);
	
	_localCopy=strndup(tkn, strlen(tkn));

	_localToken=strTokIdx(_localCopy, "@", 2);

	//AppendUrl(/*pUrlChainTmp*/pUrlChain, _localToken);
//printf(" >>>>[%s]   <%p> \n", "_unat", pUrlChain->pcData);
//.	pUrlChain->pcData = malloc(strlen(_localToken));
//printf(" >>>>[%s]   <%p> \n", "_unat", pUrlChain->pcData);
//.	memcpy(pUrlChain->pcData, _localToken, strlen(_localToken));
//printf(" >>>>[%s]   <%p> \n", "_unat", pUrlChain->pcData);
//.printf(" >>>>[%s]   %s \n", "_unat", pUrlChain->pcData);

	while( _localToken != NULL ) 
	{
	pCompoundType pCompoundTmp;

		iChunked++;

		/*pUrlChain->pCompound = (pCompoundType) */ //.CreateCompound(&(pUrlChain->pCompound));

		printf("\t\t%s\n", _localToken );

		//.AppendCompound(pUrlChain->pCompound, _localToken);
		AppendUrl(pUrlChain, _localToken);

		_localToken = strTokIdx(NULL, "@", 2);
	}
printf(" The element is <%s>\n", (iChunked>0)?"SPLIT INTO PARTS":"INTEGRAL");

	DisplayUrl(pUrlChainTmp);

	DeleteUrl(pUrlChainTmp);

	free(_localCopy);

printf("\n\n");


}



static void _untab(char * tkn)
{
char *_localCopy;
char *_localToken;

char *cParcedOut;

	_localCopy=strndup(tkn, strlen(tkn));

	_localToken=strTokIdx(_localCopy, "\t", 1);

	while( _localToken != NULL ) 
	{
		cParcedOut = strndup(_localToken+strlen("URL=\""), strlen(_localToken) - strlen("URL=\"") );

		cParcedOut[strlen(cParcedOut) -1 ] = 0;

		//TODO: REMOVE! printf("\t{%s}\n", _localToken );
		//TODO: REMOVE! printf("\t[%s]\n", cParcedOut );

		_unat(cParcedOut);

		_localToken = strTokIdx(NULL, "\t", 1);

		free (cParcedOut);
	}


	free(_localCopy);
}

static void _unret(char * tkn)
{
char *_localCopy;
char *_localToken;

	_localCopy=strndup(tkn, strlen(tkn));

	_localToken=strTokIdx(_localCopy, "\n", 0);

	while( _localToken != NULL ) 
	{
//		printf(">>>>%s<<<<\n", _localToken );

		_untab(_localToken);

		_localToken = strTokIdx(NULL, "\n", 0);
	}



	free(_localCopy);

}


void _find_named_element(const char * caller, xmlNode * a_node, const char * template)
{
	xmlNode *cur_node = NULL;

	for (cur_node = a_node; cur_node; cur_node = cur_node->next)
	{
		if (XML_ELEMENT_NODE == cur_node->type)
		{
			/* DEBUG: printf("[%s]: name=%s  type=%s \n", caller,  cur_node->name, "XML_ELEMENT_NODE"); */
	
			/* El't has been found by template?  */
			if ( 0 == strcmp (template, cur_node->name) )
			{
				xmlNode *_ch_cur_node = NULL;

				/* melde sich */
				printf("[%s]: The element name=<%s> has been found (type=%s)\n", caller,  cur_node->name, "XML_ELEMENT_NODE");

				/* Print its contents */
				for (_ch_cur_node = cur_node->children; _ch_cur_node; _ch_cur_node = _ch_cur_node->next)
				{


					if ( XML_TEXT_NODE == _ch_cur_node->type)
					{						

						//c_unret() --> _untab() --> _unat() ... go!
						_unret(_ch_cur_node->content);

					}
					

				}

				/* and get away */
				break;
			}
		}


		/* If not found by template let's try with its children */
		find_named_element(cur_node->children, template);
	}
}

void _print_element_names(const char * caller, xmlNode * a_node)
{
	xmlNode *cur_node = NULL;

	for (cur_node = a_node; cur_node; cur_node = cur_node->next)
	{
#if (1)
		if (XML_ELEMENT_NODE == cur_node->type)

			printf("[%s]: name=%s  type=%s \n", caller,  cur_node->name, "XML_ELEMENT_NODE");
#endif /* (0) */


		if ( XML_TEXT_NODE == cur_node->type)
			printf("[%s]:   type=%d  content=(%s)\n", caller,  cur_node->type, cur_node->content);


		print_element_names(cur_node->children);
	}
}
