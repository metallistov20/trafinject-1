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

/* CURLOPT_URL */
#include <curl/curl.h>

#include "lists.h"
#include "inject.h"
#include "verbose.h"


int _AppendUrl(const char * caller, pUrlChainType * ppThisUrlChain, char * pcData)
{
pUrlChainType pChild, pTempUrlChain;

	if (NULL == *ppThisUrlChain)
	{
		/* only one chain, for breginning */
		*ppThisUrlChain = (pUrlChainType) calloc ( 1, sizeof (UrlChainType) );

		/* check if successful */
		if (NULL == *ppThisUrlChain)
		{
			/* TODO: verbose"failure on creation" error */
			return INJ_MEM_ERROR;
		}
	}
	else
	{
		/* point with first temporary element to head of chain */
		pChild = *ppThisUrlChain;

		pTempUrlChain = (pUrlChainType) calloc ( 1, sizeof (UrlChainType) );

		if (NULL == pTempUrlChain)
		{

			/* TODO: verbose"failure on creation" error */
			return INJ_MEM_ERROR;
		}

		/* Skip everything */
		while ( (NULL != pChild) && (NULL != pChild->pNextChain ) )
		{
			/* til the tail */
			pChild = pChild->pNextChain;
		}

		/* if nex chunk was created allright */
		if(NULL != pTempUrlChain)
		{		
			/* attach a new chain entry to the end of existing chain */
			pChild->pNextChain = pTempUrlChain;
		}
		else
		{
			/* TODO: verbose memory for new ch. was not allocated  */
			return INJ_MEM_ERROR;
		}
	}
	return INJ_SUCCESS;
}

int _AppendCompound(const char * caller, pCompoundType * pThisCompound, char * pcData)
{
pCompoundType pChild, pTempCompound;

	if (NULL == *pThisCompound)
	{
		/* only one chain, for breginning */
		*pThisCompound = (pCompoundType) calloc ( 1, sizeof (CompoundType) );

		/* check if successful */
		if (NULL == *pThisCompound)
		{
			/* TODO: verbose"failure on creation" error */
			return INJ_MEM_ERROR;
		}

		(*pThisCompound)->pcData = calloc (1, strlen (pcData) +1 );
	
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
			return INJ_MEM_ERROR;
		}

		/* Skip everything */
		while ( (NULL != pChild) && (NULL != pChild->pNext ) )
		{
			/* til the tail */
			pChild = pChild->pNext;
		}

		/* if nex chunk was created allright */
		if(NULL != pTempCompound)
		{
			/* allocate a space needed for item's name */
			pTempCompound->pcData = (char *) calloc ( 1, strlen(pcData) + 1 );
		
			/* do copy item's name */
			strcpy(pTempCompound->pcData, pcData);
		
			/* attach a new chain entry to the end of existing chain */
			pChild->pNext = pTempCompound;
		}
		else
		{
			/* TODO: verbose memory for new ch. was not allocated  */
			return INJ_MEM_ERROR;
		}
	}
	return INJ_SUCCESS;
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

void _DeleteCompoundEx(const char * caller, pCompoundType * ppThisCompound)
{
pCompoundType pChild, pThisCompound = *ppThisCompound;

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

	*ppThisCompound = NULL;
}

void _DeleteUrl(const char * caller, pUrlChainType pThisUrlChain)
{
pUrlChainType pChild;

	/* Walk through entire list and delete each chain */
	while (NULL != pThisUrlChain)
	{
		/* if we have compound data (i.e. binded-list insted of single string) release  */
		if (pThisUrlChain->pCompound)
		
		    /* then release this space */
		    DeleteCompound(pThisUrlChain->pCompound);

		/* if we've allocated area for cumulative URL */
		if (pThisUrlChain->pcSumm)
		
		    /* then release this space */
		    free(pThisUrlChain->pcSumm);

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

void _DeleteUrlEx(const char * caller, pUrlChainType * ppThisUrlChain)
{
pUrlChainType pChild, pThisUrlChain = *ppThisUrlChain;

	/* Walk through entire list and delete each chain */
	while (NULL != pThisUrlChain)
	{
		/* if we have compound data (i.e. binded-list insted of single string) release  */
		if ( pThisUrlChain->pCompound)
		
		    /* then release this space */
		    DeleteCompoundEx( & pThisUrlChain->pCompound );

		/* if we've allocated area for cumulative URL */
		if ( pThisUrlChain->pcSumm)
		
		    /* then release this space */
		    free( pThisUrlChain->pcSumm);

		/* preserve a pointer to next record */		    
		pChild = pThisUrlChain->pNextChain;
		
		/* free space occupied by current record */
		free(pThisUrlChain);
		
		/* Go to next record  */
		pThisUrlChain = pChild;
	}

	*ppThisUrlChain=NULL;
	
	/* done */
	return; 
}

void _DisplayString(const char * caller, char * pcDataPar)
{    
	if (NULL != pcDataPar)
		DXML("[%s]: STRING(%s)\n", caller, pcDataPar);
	else
		DXML("[%s]: EMPTY_STR_AT(%p)\n", caller, &pcDataPar);
}

void _DisplayCompound(const char * caller, pCompoundType pCompoundPar)
{    
pCompoundType pCompound = pCompoundPar;

    /* process each  entry of chain */
    while (NULL != pCompound)
    {
	/* issue item's name */	
	if (NULL != pCompound->pcData)

		DisplayString((char *)(pCompound->pcData));
	
	/* Go to next record of Chainwork */
	pCompound =  pCompound->pNext;
    }
}

void _DisplayUrl(const char * caller, pUrlChainType pThisUrlChainPar)
{    
pUrlChainType pThisUrlChain = pThisUrlChainPar;

    /* process each  entry of chain */
    while (NULL != pThisUrlChain)
    {

	if (NULL != pThisUrlChain->pCompound)
	{
		DXMLAUX("\t[%s]: pCompund is not NULL, its contents are:\n", caller);

		DisplayCompound(pThisUrlChain->pCompound);
	}

	if (NULL != pThisUrlChain->pCompound)
	{

		DXMLAUX("\t[%s]: Cumulative resulting URL is :\n", caller);

		DisplayString(pThisUrlChain->pcSumm);

	}
	
	/* Go to next record of Chainwork */
	pThisUrlChain =  pThisUrlChain->pNextChain;
    };	    
}

static int _GlueCompound(const char * caller, pCompoundType pCompoundPar, pUrlChainType pUrlChain)
{    
pCompoundType pCompound = pCompoundPar;

int iCnt=0;

	/* process each  entry of chain */
	while (NULL != pCompound)
	{
		/* issue item's name */	
		if (NULL != pCompound->pcData)
		{
			/* glue all burt first */
			if ( 0 < iCnt++ )

				/* standatrd delimited for parameters on URL requets string */
				strcat( (char *)(pUrlChain->pcSumm),  "&" );

			strcat( (char *)(pUrlChain->pcSumm),  (char *)(pCompound->pcData) );
		}

		/* Go to next record of Chainwork */
		pCompound =  pCompound->pNext;
	}

	return INJ_SUCCESS;
}

int _GlueUrl(const char * caller, pUrlChainType pThisUrlChainPar)
{    
pUrlChainType pThisUrlChain = pThisUrlChainPar;

    /* process each  entry of chain */
    while (NULL != pThisUrlChain)
    {

	pThisUrlChain->pcSumm = (char *) malloc ( MAX_URL_SIZE*8 );

	if ( NULL == pThisUrlChain->pcSumm ) 
	{
		DXML("\t[%s]: can't allocate %d bytes for URL data:\n", caller, MAX_URL_SIZE );

		return INJ_MEM_ERROR;
	}

	strcpy (pThisUrlChain->pcSumm, "http://");

	if ( NULL != cIpAddr ) 

		strcat (pThisUrlChain->pcSumm, cIpAddr);

	if (NULL != pThisUrlChain->pCompound)
	{
		DXMLAUX("\t[%s]: URL is going to be composed of its particles\n", caller);

		GlueCompound(pThisUrlChain->pCompound, pThisUrlChain);
	}
	
	/* Go to next record of Chainwork */
	pThisUrlChain =  pThisUrlChain->pNextChain;
    };

    return INJ_SUCCESS;
}

int _DeployUrl(const char * caller, pUrlChainType pThisUrlChainPar)
{    
pUrlChainType pThisUrlChain = pThisUrlChainPar;

int iRes;

    /* process each  entry of chain */
    while (NULL != pThisUrlChain)
    {
	if ( NULL == pThisUrlChain->pcSumm ) 
	{
		DXML("\t[%s]: can't allocate %d bytes for URL data:\n", caller, MAX_URL_SIZE );

		return INJ_MEM_ERROR;
	}

	DURL("%s: summURL(at:<%p>;<%p>;<%p>) = %s\n", caller, pThisUrlChain, pThisUrlChain->pcSumm, pThisUrlChain->pNextChain, pThisUrlChain->pcSumm);

	if ( CURLE_OK == ( iRes = curl_easy_setopt(curl, CURLOPT_URL, pThisUrlChain->pcSumm ) ) )
	{
		/* here we start generate the 'live' HTTP traffic */
		iRes = curl_easy_perform(curl);
	}
	else
	{
		DURL("%s: cURL call setopt(CURLOPT_URL) failed with ERR_CODE(%d)\n", caller, iRes); 

		/* don't process other chaing, return immediately */
		return iRes;
	}
	
	/* Go to next record of Chainwork */
	pThisUrlChain =  pThisUrlChain->pNextChain;
    };


    if ( CURLE_OK == iRes)
    {
	/* go out quietly if all correct */
	return  INJ_SUCCESS;
    }
    else	
    {
	/* verbosing CURLcode returned as <iRes> if error occured */
	DURL("%s: recent cURL call failed with ERR_CODE(%d)\n", caller, iRes);

	return   INJ_CURL_ERROR;
    }
}

int _DeployUrlEx(const char * caller, pUrlChainType pThisUrlChainPar, int iExtra)
{    
pUrlChainType pThisUrlChain = pThisUrlChainPar;

int iRes;

/* Index of 'extras' to be processed */
int iExtras = 0;

    /* process each  entry of chain */
    while (NULL != pThisUrlChain)
    {

	if ( NULL == pThisUrlChain->pcSumm ) 
	{
		DXML("\t[%s]: can't allocate %d bytes for URL data:\n", caller, MAX_URL_SIZE );

		return INJ_MEM_ERROR;
	}

	DURL("%s: summURL(at:<%p>;<%p>) = %s\n", caller, (&pThisUrlChain), &(pThisUrlChain->pcSumm) , pThisUrlChain->pcSumm);


	if ( CURLE_OK == ( iRes = curl_easy_setopt(curl, CURLOPT_URL, pThisUrlChain->pcSumm ) ) )
	{
		if ( iExtra )
		{
			switch (iExtras++)
			{
				case 0:
				{
					if( CURLE_OK == ( iRes = curl_easy_setopt(curl, CURLOPT_POSTFIELDS, *pcPtr2Extra1) ))
					{
						if( CURLE_OK == ( iRes = curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, 
							strlen(*pcPtr2Extra1))) )
						{
							break;
						}
					}

					DURL("%s: cURL call setopt(CURLOPT_POSTFIELDS) failed with ERR_CODE(%d)\n", caller, iRes); 

					return   INJ_CURL_ERROR;

				}
				case 1:
					if( CURLE_OK == ( iRes = curl_easy_setopt(curl, CURLOPT_POSTFIELDS, *pcPtr2Extra2) ))
					{
						if( CURLE_OK == ( iRes = curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, 
							strlen(*pcPtr2Extra2))) )
						{
							break;
						}
					}

					DURL("%s: cURL call setopt(CURLOPT_POSTFIELDS) failed with ERR_CODE(%d)\n", caller, iRes); 

					return  INJ_CURL_ERROR;

				default:
					break;
			}


		}

		/* here we start generate the 'live' HTTP traffic */
		iRes = curl_easy_perform(curl);

	}
	else
	{
		DURL("%s: cURL call setopt(CURLOPT_URL) failed with ERR_CODE(%d)\n", caller, iRes); 

		/* don't process other chaing, return immediately */
		return iRes;
	}

	
	/* Go to next record of Chainwork */
	pThisUrlChain =  pThisUrlChain->pNextChain;
    };


    if ( CURLE_OK == iRes)
	/* go out quietly if all correct */
	return  INJ_SUCCESS;
    else
    {
	/* verbosing CURLcode returned as <iRes> if error occured */
	DURL("%s: recent cURL call failed with ERR_CODE(%d)\n", caller, iRes);

	return   INJ_CURL_ERROR;
    }
}
