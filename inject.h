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

/* Intermediate URL structure */
CURL *curl;

/* Flag to define extra payload should be attached  */
int iExtra;

/* IP adress of target switch */
char cIpAddr[MAX_IP_SIZE];

/* Session tID, valid between calls <iOpenSite> and <post_site>  */
char cTid[MAX_TID_SIZE];

/* Name of SNMP community to be created on target switch */
char cSnmp[MAX_SNMP_SIZE];

/* Parameters of ACL froup to be created on target switch */
char cAcl[MAX_STR_SIZE];

/* static IP address to be assigned to a switch */
char cNewIpAddr[MAX_IP_SIZE];

/* sbnet mask to be assigned along with static IP address */
char cMask[MAX_IP_SIZE];

/* Name of switch model. 5 chars long, so assuming MAX_SNMP_SIZE is enough */
char cModel[MAX_STR_SIZE];

/* Name of firmware to upload and burn. Assuming length MAX_SNMP_SIZE is sifficient */
char cFwName[MAX_STR_SIZE];


char ** pcPtr2Extra1, **pcPtr2Extra2;

#endif /* _INJECT_H_ */
