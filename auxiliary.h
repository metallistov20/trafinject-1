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

#ifndef _AUXILIARY_H_
#define _AUXILIARY_H_


/* Make variables defined in <voc.c> visible to others */
#include "voc.h"

/* Special type to provide correspondence betwee variable address and its name; needed for vocabulary */
typedef struct _QuineType
{
	/* Tip: This <void*> variable dereferences into <char**> */
	void * vpQuineVar;

	/* Keyword, and at the same time name of the variable itself */
	const char * pcQuineVarValue;

} QuineType, * pQuineType;


/* (Make it visible to others) */
extern QuineType QuineArray[];


/* (Make it visible to others) */
extern char cIpAddr [MAX_IP_SIZE];

/* (Make it visible to others) */
extern char cFwName [MAX_STR_SIZE];

/* (Make it visible to others) */
extern char cXmlName [MAX_STR_SIZE];


#endif /* _AUXILIARY_H_ */
