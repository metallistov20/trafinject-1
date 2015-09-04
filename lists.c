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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "lists.h"


/* Create initial chain of URL list */
pUrlChainType _CreateUrl(const char * caller, pUrlChainType * pNewUrlChain)
{


	/* only one chain, for breginning */
	*pNewUrlChain = (pUrlChainType) calloc ( 1, sizeof (UrlChainType) );

	/* check if successful */
	if (NULL == *pNewUrlChain)
	{
		/* TODO: verbose"failure on creation" error */
		return  NULL;
	}

	/* TODO: useless, or? NULL identifies "failure on creation" error */
	return *pNewUrlChain;
}

pCompoundType _CreateCompound(const char * caller, pCompoundType * pNewCompound)
{
	/* only one chain, for breginning */
	*pNewCompound = (pCompoundType) calloc ( 1, sizeof (CompoundType) );

	/* check if successful */
	if (NULL == *pNewCompound)
	{
		/* TODO: verbose"failure on creation" error */
		return  NULL;
	}

	/* make it clean */
	//memset ((void*)(*pNewCompound), 0, sizeof (CompoundType));

	
	/* TODO: useless, or? NULL identifies "failure on creation" error */
	return *pNewCompound;
}

void _AppendUrl(const char * caller, pUrlChainType pThisUrlChain, char * pcData)
{
pUrlChainType pChild, pTempUrlChain;

	/* point with first temporary element to head of chain */
	pChild = pThisUrlChain;

	CreateUrl(&pTempUrlChain);

	/* Skip everything */
	while ((NULL != pChild) && (NULL != pChild->pNextChain ) )
	{

		/* til the tail */
		pChild = pChild->pNextChain;
	}

	/* if nex chunk was created allright */
	if(NULL != pTempUrlChain)
	{
		/* allocate a space needed for item's name */
		pTempUrlChain->pcData = (char *) malloc (strlen(pcData)+1);
		
		/* do copy item's name */
		strcpy(pTempUrlChain->pcData, pcData);
		
		/* attach a new chain entry to the end of existing chain */
		pChild->pNextChain = pTempUrlChain;
	}
	else
		/* TODO: verbose memory for new ch. was not allocated  */
		return; 	

}

void _AppendAnyUrl(/*const char * caller, */pUrlChainType * pThisUrlChain, char * pcData)
{
pUrlChainType pChild, pTempUrlChain;

	if (NULL == *pThisUrlChain)
	{
		/* only one chain, for breginning */
		*pThisUrlChain = (pUrlChainType) calloc ( 1, sizeof (UrlChainType) );

		/* check if successful */
		if (NULL == *pThisUrlChain)
		{
			/* TODO: verbose"failure on creation" error */
			return  ;//NULL;
		}

		(*pThisUrlChain)->pcData = malloc ( strlen (pcData) +1 );
	
		strcpy ((*pThisUrlChain)->pcData, pcData);
	}
	else
	{
		/* point with first temporary element to head of chain */
		pChild = *pThisUrlChain;

		pTempUrlChain = (pUrlChainType) calloc ( 1, sizeof (UrlChainType) );

		if (NULL == pTempUrlChain)
		{

			/* TODO: verbose"failure on creation" error */
			return;
		}

		/* Skip everything */
		while ((NULL != pChild) && (NULL != pChild->pNextChain ) )
		{
			/* til the tail */
			pChild = pChild->pNextChain;
		}

		/* if nex chunk was created allright */
		if(NULL != pTempUrlChain)
		{
			/* allocate a space needed for item's name */
			pTempUrlChain->pcData = (char *) malloc ( strlen(pcData) + 1);
		
			/* do copy item's name */
			strcpy(pTempUrlChain->pcData, pcData);
		
			/* attach a new chain entry to the end of existing chain */
			pChild->pNextChain = pTempUrlChain;
		}
		else
		{
			/* TODO: verbose memory for new ch. was not allocated  */
			return;
		}
	}
}

void _AppendAnyCompound(/*const char * caller, */pCompoundType * pThisCompound, char * pcData)
{
pCompoundType pChild, pTempCompound;

/* 
	1. Check if NILL
	2. If so then create new and fulfill
	3. If not then run till the tail, create new, fulfill and attach (one which was created) onto tail
*/

	if (NULL == *pThisCompound)
	{
		/* only one chain, for breginning */
		*pThisCompound = (pCompoundType) calloc ( 1, sizeof (CompoundType) );

		/* check if successful */
		if (NULL == *pThisCompound)
		{
			/* TODO: verbose"failure on creation" error */
			return  ;//NULL;
		}

		(*pThisCompound)->pcData = malloc (strlen (pcData) +1 );
	
		strcpy ((*pThisCompound)->pcData, pcData );
	}
	else
	{
		/* point with first temporary element to head of chain */
		pChild = *pThisCompound;

		pTempCompound = (pCompoundType) calloc ( 1, sizeof (CompoundType) );

		if (NULL == pTempCompound)
		{

			/* TODO: verbose"failure on creation" error */
			return;
		}

		/* Skip everything */
		while ((NULL != pChild) && (NULL != pChild->pNext ) )
		{
			/* til the tail */
			pChild = pChild->pNext;
		}

		/* if nex chunk was created allright */
		if(NULL != pTempCompound)
		{
			/* allocate a space needed for item's name */
			pTempCompound->pcData = (char *) malloc ( strlen(pcData) + 1 );
		
			/* do copy item's name */
			strcpy(pTempCompound->pcData, pcData);
		
			/* attach a new chain entry to the end of existing chain */
			pChild->pNext = pTempCompound;
		}
		else
		{
			/* TODO: verbose memory for new ch. was not allocated  */
			return;
		}
	}
}

void _AppendCompound(const char * caller, pCompoundType pThisCompound, char * pcData)
{
pCompoundType pChild, pTempCompound;

	/* point with first temporary element to head of chain */
	pChild = pThisCompound;

	CreateCompound(&pTempCompound);

	/* Skip everything */
	while (NULL != pChild->pNext ) 
	{
		/* til the tail */
		pChild = pChild->pNext;
	}

	/* if nex chunk was created allright */
	if(NULL != pTempCompound)
	{
		/* allocate a space needed for item's name */
		pTempCompound->pcData = (char *) malloc (strlen(pcData) + 1);
		
		/* do copy item's name */
		strcpy(pTempCompound->pcData, pcData);
		
		/* set a lock-up */ // TODO: remove overabundance
		pTempCompound->pNext = NULL;		

		/* attach a new chain entry to the end of existing chain */
		pChild->pNext = pTempCompound;
	}
	else
		/* TODO: verbose memory for new ch. was not allocated  */
		return; 	

}

void _DeleteCompound(const char * caller, pCompoundType pThisCompound)
{
pCompoundType pChild;

	/* Walk through entire list and delete each chain */
	while (NULL != pThisCompound)
	{
		/* if space to keep item's name is allocated */
		if (pThisCompound->pcData)
		
		    /* then release this space */
		    free(pThisCompound->pcData);
		    
		/* preserve a pointer to next record */		    
		pChild = pThisCompound->pNext;
		
		/* free space occupied by current record */
		free (pThisCompound);
		
		/* Go to next record */
		pThisCompound = pChild;
	}

}

void _DeleteUrl(const char * caller, pUrlChainType pThisUrlChain)
{
pUrlChainType pChild;

	/* Walk through entire list and delete each chain */
	while (NULL != pThisUrlChain)
	{
		/* if space to keep item's name is allocated */
		if (pThisUrlChain->pcData)
		
		    /* then release this space */
		    free(pThisUrlChain->pcData);

		/* if we have compound data (i.e. binded-list insted of single string) release  */
		if (pThisUrlChain->pCompound)
		
		    /* then release this space */
		    DeleteCompound(pThisUrlChain->pCompound);
		    
		/* preserve a pointer to next record */		    
		pChild = pThisUrlChain->pNextChain;
		
		/* free space occupied by current record */
		free (pThisUrlChain);
		
		/* Go to next record  */
		pThisUrlChain = pChild;
	}
	
	/* done */
	return; 
}

void _DisplayString(const char * caller, char * pcDataPar)
{    
	if (pcDataPar) printf("[%s]: STRING(%s)\n", caller, pcDataPar); else  printf("[%s]  EMPTY_AT(%p)\n", caller, &pcDataPar);
}

void _DisplayCompound(const char * caller, pCompoundType pCompoundPar)
{    
pCompoundType pCompound = pCompoundPar;

    /* process each  entry of chain */
    while (pCompound != NULL )
    {
	/* issue item's name */	
	if (NULL != pCompound->pcData)

		DisplayString((unsigned char *)(pCompound->pcData));
	
	/* Go to next record of Chainwork */
	pCompound =  pCompound->pNext;
    }
}

void _DisplayUrl(const char * caller, pUrlChainType pThisUrlChainPar)
{    
pUrlChainType pThisUrlChain = pThisUrlChainPar;

    /* process each  entry of chain */
    while (pThisUrlChain != NULL )
    {
#if (0)
	/* issue item's technical data, as string. we're not disaplying it */	
	if (NULL != pThisUrlChain->pcData)
	{
		printf("\t[%s]: pcData is (%s)\n", caller, pThisUrlChain->pcData);

		DisplayString((unsigned char *)(pThisUrlChain->pcData));
	}
#endif /* (0) */

	if (NULL != pThisUrlChain->pCompound)
	{
		printf("\t[%s]: pCompund is not NULL, its contents are:\n", caller);

		DisplayCompound(pThisUrlChain->pCompound);
	}
	
	/* Go to next record of Chainwork */
	pThisUrlChain =  pThisUrlChain->pNextChain;
    };	    
}
