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


typedef struct _CompoundType {

	char * pcData;	

	struct _CompoundType * pNext; 

} CompoundType, *pCompoundType;

typedef struct _UrlChainType {

	/* Either <pcData> contains _integral ULR-injection string ... */
	char * pcData;

	/* either _composite ULR-injection is stored in few <pcCompData> fields. */
	struct _CompoundType * pCompound;
	
	unsigned long uloDataAddr; /* Opt'l */

	struct _UrlChainType * pNextChain; 

} UrlChainType, *pUrlChainType;

#define CreateUrl(x) _CreateUrl(__func__, (x))

#define AppendUrl(x, y) _AppendUrl(__func__, (x), (y))

#define DeleteCompound(x) _DeleteCompound(__func__, (x))

#define DeleteUrl(x) _DeleteUrl(__func__, (x))

#define DisplayString(x) _DisplayString(__func__, (x))

#define DisplayCompound(x) _DisplayCompound(__func__, (x))

#define DisplayEntireUrl(x) _DisplayEntireUrl(__func__, (x))


#endif /* _LISTS_H_ */
