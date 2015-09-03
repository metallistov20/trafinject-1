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
pUrlChainType _CreateUrl(const char * caller, pUrlChainType pNewUrlChain)
{
	/* only one chain, for breginning */
	pNewUrlChain = (pUrlChainType) malloc ( sizeof (UrlChainType) );

	/* check if successful */
	if (!pNewUrlChain)
	{
		/* TODO: verbose"failure on creation" error */
		return  NULL;
	}
//printf("_CreateUrl 4 pNewUrlChain=<%p>\n", pNewUrlChain);

#if (0)
	/* a lock-up  */
	pNewUrlChain->pNextChain = NULL;
#else
	/* make it clean */
	memset ((void*)pNewUrlChain, 0, sizeof (UrlChainType));
#endif /* (0) */
	
	/* TODO: useless, or? NULL identifies "failure on creation" error */
	return pNewUrlChain;
}

pCompoundType _CreateCompound(const char * caller, pCompoundType pNewCompound)
{
	/* only one chain, for breginning */
	pNewCompound = (pCompoundType) malloc ( sizeof (CompoundType) );

	/* check if successful */
	if (!pNewCompound)
	{
		/* TODO: verbose"failure on creation" error */
		return  NULL;
	}
//printf("_CreateUrl 4 pNewUrlChain=<%p>\n", pNewUrlChain);

#if (0)
	/* a lock-up  */
	pNewCompound->pNext = NULL;
#else
	/* make it clean */
	memset ((void*)pNewCompound, 0, sizeof (CompoundType));
#endif /* (0) */
	
	/* TODO: useless, or? NULL identifies "failure on creation" error */
	return pNewCompound;
}

//void _x_AppendUrl(pUrlChainType pThisUrlChain, char * pcData)
void _AppendUrl(const char * caller, pUrlChainType pThisUrlChain, char * pcData)
{
pUrlChainType pChild, pbTempUrlChain;


printf("_AppendUrl 1 (<%p> <%s>) \n", pThisUrlChain, pcData);
	/* point with first temporary element to head of chain */
	pChild = pThisUrlChain;

#if (0)
	/* allocate a space for new record in chain */
	pbTempUrlChain = (pUrlChainType) malloc ( sizeof (UrlChainType));
#else
//.printf("_AppendUrl 2 pbTempUrlChain <%p> \n", pbTempUrlChain);
	pbTempUrlChain = CreateUrl(pbTempUrlChain);
//.printf("_AppendUrl 2  pbTempUrlChain <%p> \n", pbTempUrlChain);
#endif /* (0) */

	/* Skip everything */
	while ((NULL != pChild) && (NULL != pChild->pNextChain ) )
	{
//.printf(" skipping \n");

		/* til the tail */
		pChild = pChild->pNextChain;
	}

	/* if nex chunk was created allright */
	if(NULL != pbTempUrlChain)
	{
		/* allocate a space needed for item's name */
		pbTempUrlChain->pcData = (char *) malloc (strlen(pcData));
		
		/* do copy item's name */
		strcpy(pbTempUrlChain->pcData, pcData);
		
		/* set a lock-up */ // TODO: remove overabundance
		pbTempUrlChain->pNextChain = NULL;		

		/* attach a new chain entry to the end of existing chain */
		pChild->pNextChain = pbTempUrlChain;
	}
	else
		/* TODO: verbose memory for new ch. was not allocated  */
		return; 	

}

void _AppendCompound(const char * caller, pCompoundType pThisCompound, char * pcData)
{
pCompoundType pChild, pTempCompound;

printf("_AppendCompound 1 (pThisCompound=<%p> pcData=<%s>) \n", pThisCompound, pcData);
	/* point with first temporary element to head of chain */
	pChild = pThisCompound;

printf("_AppendCompound 2 pbTempUrlChain <%p> \n", pThisCompound);
	pThisCompound = CreateCompound(pTempCompound);
printf("_AppendCompound 2  pbTempUrlChain <%p> \n", pThisCompound);


	/* Skip everything */
	while ((NULL != pChild) && (NULL != pChild->pNext ) )
	{
//.printf(" skipping \n");

		/* til the tail */
		pChild = pChild->pNext;
	}

	/* if nex chunk was created allright */
	if(NULL != pTempCompound)
	{
		/* allocate a space needed for item's name */
		pTempCompound->pcData = (char *) malloc (strlen(pcData));
		
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
		
		/* Go to next record. TODO: check if really needed. ASSUMING THAT IS. */
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
		
		/* Go to next record. TODO: check if really needed. ASSUMING THAT IS. */
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
	/* issue item's name */	
	if (NULL != pThisUrlChain->pcData)

		DisplayString((unsigned char *)(pThisUrlChain->pcData));

	if (NULL != pThisUrlChain->pCompound)

		DisplayCompound(pThisUrlChain->pCompound);
	
	/* Go to next record of Chainwork */
	pThisUrlChain =  pThisUrlChain->pNextChain;
    };	    
}
