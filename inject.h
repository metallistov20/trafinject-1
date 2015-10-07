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

#ifndef _INJECT_H_
#define _INJECT_H_

#include <curl/curl.h>

#include <libxml/parser.h>
#include <libxml/tree.h>

#include "constants.h"


/* Intermediate URL structure */
CURL *curl;

/* Flag to define extra payload should be attached  */
int iExtra;

/* Extra payload for POST messages (address of string) */
char ** pcPtr2Extra1, **pcPtr2Extra2;



/* Index of desired operation (can be 'create', 'save', 'ACL', 'firmware' )*/
int iOperation;

/*  Program name */
char cArg0[MAX_URL_SIZE];



/* Payload of POST method during user authenticate */
const char *cPostMethodString;

/* Extra payload of POST method during user  authenticate */
const char *cPostMethodString2;

/* Ptr to XML data for in memory */
xmlNode *root_element;

/* Ptr to XML data document */
xmlDoc *doc;



#endif /* _INJECT_H_ */
