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

#ifndef _LISTS_H_
#define _LISTS_H_

#include "constants.h"

/* List of particlers of which consists certain (not all) URLs; signle-binded */
typedef struct _CompoundType
{
	/* String to represent URL */
	char * pcData;

	/* in case of Vocabulary - pointer to variable corr. to keyword <pcData> */
	void * pVar;

	/* Next string in chain */
	struct _CompoundType * pNext; 

} CompoundType, *pCompoundType;

/* List of URLs; signle-binded */
typedef struct _UrlChainType
{
	/* either _composite ULR-injection is stored in few <pcCompData> fields. */
	struct _CompoundType * pCompound;
	
	/* Service data. Optional. */
	unsigned long uloDataAddr;

	/* Exact IP-address of target */
	char IpAddrMain[MAX_IP_SIZE];

	/* Auxilary IP-address, such as new IP address after assignment */
	char IpAddrAux[MAX_IP_SIZE];

	/* Cumulative URL string made of compounds distributed across Compound->pcData */
	char * pcSumm;

	/* Next URL in chain */
	struct _UrlChainType * pNextChain; 

} UrlChainType, *pUrlChainType;

/* S.-b.-list, contains volabulary, which is s.b.-list, either */
typedef struct _XmlAuxType
{
	/* Vocabulary of keywords relevant to this XML file */
	struct _CompoundType * pVocabulary;	

} XmlAuxType, *pXmlAuxType;

#define DeleteUrlEx(x) _DeleteUrlEx(__func__, (x))

#define DeleteCompoundEx(x) _DeleteCompoundEx(__func__, (x))

#define DisplayString(x) _DisplayString(__func__, (x))

#define DisplayCompound(x) _DisplayCompound(__func__, (x))

#define DisplayUrl(x) _DisplayUrl(__func__, (x))

#define AppendUrl(x, y) _AppendUrl(__func__, (x), (y))

#define AppendCompound(x, y, z) _AppendCompound(__func__, (x), (y), (z))

#define GlueCompound(x, y) _GlueCompound(__func__, (x), (y))

#define GlueUrl(x) _GlueUrl(__func__, (x))

#define DeployUrl(x) _DeployUrl(__func__, (x))

#define DeployUrlEx(x, y) _DeployUrlEx(__func__, (x), (y))

#define AppendXmlAux(x) _AppendXmlAux(__func__, (x))

#define DeleteVocabularyEx(x) _DeleteVocabularyEx(__func__, (x))

#define DeleteXmlAuxEx(x) _DeleteXmlAuxEx(__func__, (x))

#define XmlAuxCreateEx() _XmlAuxCreateEx(__func__)

#define DisplayXmlAux(x) _DisplayXmlAux(__func__, (x))



#endif /* _LISTS_H_ */
