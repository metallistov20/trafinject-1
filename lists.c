#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "lists.h"

/* Variable to store URL list for current command */
pUrlChainType  pUrlChain;


/* Create initial chain of URL list */
pUrlChainType _CreateUrl(const char * caller, pUrlChainType pNewUrlChain)
{
	/* only one chain, for breginning */
	pNewUrlChain = (pUrlChainType) malloc ( sizeof (UrlChainType) );

	/* check if successful */
	if (!pNewUrlChain)
		/* TODO: verbose"failure on creation" error */
		return  NULL;

	/* a lock-up  */
	pNewUrlChain->pNextChain = NULL;
	

	/* NULL identifies "failure on creation" error */
	return pNewUrlChain;
}

/* Append URL list with new new chain with contents <_NameOfItem> */
void _AppendUrl(const char * caller, pUrlChainType pThisUrlChain, char * pcData)
{
pUrlChainType pChild, pbTempUrlChain;

	/* point with first temporary element to head of chain */
	pChild = pThisUrlChain;

	/* allocate a space for new record in chain */
	pbTempUrlChain = (pUrlChainType) malloc ( sizeof (UrlChainType));

	/* Skip everything */
	while (NULL != pChild->pNextChain )

		/* til the tail */
		pChild = pChild->pNextChain;
		
	/* if previous memory allocation was successful */
	if(pbTempUrlChain != NULL)
	{
		/* allocate a space needed for item's name */
		pbTempUrlChain->pcData = (char *) malloc (strlen(pcData));
		
		/* do copy item's name */
		strcpy(pbTempUrlChain->pcData, pcData);
		
		/* set a lock-up */
		pbTempUrlChain->pNextChain = NULL;		

		/* append a new chain entry to the end of existing chain */
		pChild->pNextChain = pbTempUrlChain;
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

void _DisplayEntireUrl(const char * caller, pUrlChainType pThisUrlChainPar)
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
