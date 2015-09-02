#include <string.h>

#include <libxml/parser.h>
#include <libxml/tree.h>

#include "xmls.h"


/* Static array with global scope. We sore aux strings StrTokIdx() inside  */
char * aLastToken[3];

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


char * strTokIdx(char *s1, const char *delimit, int iIdx)
{
char ** lastToken;
char *tmp;

    lastToken =  &(aLastToken[iIdx]) ;
    *lastToken = NULL;

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







void _unat(char * tkn)
{
char *_localCopy;

char *_localToken;

int iChunked = -1;

	//CreateUrl(pUrlChain);
	
	_localCopy=strndup(tkn, strlen(tkn));

	_localToken=strTokIdx(_localCopy, "@", 2);

	while( _localToken != NULL ) 
	{
		iChunked++;

		printf("\t\t%s\n", _localToken );
		//AppendUrl(pUrlChain, _localToken);

		_localToken = strTokIdx(NULL, "@", 2);
	}
printf(" The element is <%s>\n", (iChunked>0)?"SPLIT INTO PARTS":"INTEGRAL");

	free(_localCopy);


}



void _untab(char * tkn)
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

void _unret(char * tkn)
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

