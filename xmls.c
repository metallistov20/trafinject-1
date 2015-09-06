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

#include "xmls.h"
#include "lists.h"
#include "verbose.h"


/* Static array in scope of curr. file. We store aux strings StrTokIdx() inside  */
static char * aLastToken[3];

/* Variable to store URL list for current command */
pUrlChainType  pUrlChain;


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
        s1 += strspn(s1, delimit);


    /* Find end of segment */
    tmp = strpbrk(s1, delimit);

    if (tmp)
    {
        /* Found another delimiter, split string and save state. */
        *tmp = '\0';

        *lastToken = tmp + 1;
    }
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

pUrlChainType pUrlLastChain = pUrlChain;

	_localCopy=strndup(tkn, strlen(tkn));

	_localToken=strTokIdx(_localCopy, "@", 2);

	/* Shift <pUrlLastChain> to the end of <pUrlChain> */
	if (NULL != pUrlLastChain) while (pUrlLastChain->pNextChain) pUrlLastChain=pUrlLastChain->pNextChain;

	while( _localToken != NULL ) 
	{
		iChunked++;

		DXMLAUX("%s: \t\t%s\n", "", _localToken);

		if (INJ_SUCCESS != AppendCompound(&pUrlLastChain->pCompound, _localToken) )
		{
			_localToken = NULL;

			DXMLAUX("%s: FAILURE: memory error on appenging new compound\n", "");

			/* exiting from while-loop. */
			break;
		};

		_localToken = strTokIdx(NULL, "@", 2);	}


	if (iChunked>0) 
		DXMLAUX("%s: <URL STRING IS SPLIT INTO %d PARTS>\n", "", iChunked+1);
	else
		DXMLAUX("%s: <INTEGRAL URL STRING>\n", "");

	free(_localCopy);
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
		if (strlen (_localToken) <= ( strlen("URL=\"") + strlen("URL=\"") ) )
			break;
		else
		{
			cParcedOut = strndup(_localToken+strlen("URL=\""), strlen(_localToken) - strlen("URL=\"") );

			cParcedOut[strlen(cParcedOut) -1 -1 ] = 0;
		
			if (INJ_SUCCESS != AppendUrl(&pUrlChain, "(aux;dta;)") )
			{
				DXMLAUX("%s: FAILURE: memory error on appenging new URL\n", "");

				_localToken = NULL;

				free (cParcedOut);

				/* exiting from while-loop */
				break;

			}

			_unat(cParcedOut);

			_localToken = strTokIdx(NULL, "\t", 1);

			free (cParcedOut);
		}
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
			//DXML("[%s]: name=%s  type=%s \n", caller,  cur_node->name, "XML_ELEMENT_NODE");
	
			/* Element has been found by template?  */
			if ( 0 == strcmp (template, cur_node->name) )
			{
				xmlNode *_ch_cur_node = NULL;

				DXML("[%s]: The element name=<%s> has been found (type=%s)\n", caller,  cur_node->name, "XML_ELEMENT_NODE");

				/* Go and parce it, store results of paring into URL&CMPND structures  */
				for (_ch_cur_node = cur_node->children; _ch_cur_node; _ch_cur_node = _ch_cur_node->next)
				{
					if ( XML_TEXT_NODE == _ch_cur_node->type)
					{
						//DXML("[%s]: about to parse=<%s> \n", caller,  _ch_cur_node->content);

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

			DXMLAUX("[%s]: name=%s  type=%s \n", caller,  cur_node->name, "XML_ELEMENT_NODE");
#endif /* (0) */


		if ( XML_TEXT_NODE == cur_node->type)

			DXMLAUX("[%s]:   type=%d  content=(%s)\n", caller,  cur_node->type, cur_node->content);

		print_element_names(cur_node->children);
	}
}
