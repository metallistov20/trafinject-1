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

#if (0)

	char _tid_ [MAX_TID_SIZE];

	char ip_address [MAX_IP_SIZE];

	char ip_mask [MAX_IP_SIZE];

	char txt_comname [MAX_SNMP_SIZE];

	char aclId [MAX_SNMP_SIZE];

#else
	/* This is something everyone can enjoy . TODO: dismiss current stuff, rest bring back to <inject.h> */
	#include "voc.h"

#endif /* (0) */

typedef struct _QuineType
{
	/* Tip: This <void*> variable dereferences into <char**> */
	void * vpQuineVar;

	/* Keyword, and at the same time name of the variable itself */
	const char * pcQuineVarValue;

} QuineType, * pQuineType;


/* (Make in visible for others) */
extern QuineType QuineArray[];

char cIpAddr [MAX_IP_SIZE];

char cFwName [MAX_STR_SIZE];

char cXmlName [MAX_STR_SIZE];




#endif /* _AUXILIARY_H_ */
