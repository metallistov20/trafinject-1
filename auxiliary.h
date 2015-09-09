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

char _tid_ [MAX_TID_SIZE];

char ip_address [MAX_IP_SIZE];

char ip_mask [MAX_IP_SIZE];

char txt_comname [MAX_SNMP_SIZE];

char aclId [MAX_SNMP_SIZE];

char cIpAddr [MAX_IP_SIZE];

char cFwName [MAX_STR_SIZE];

char cXmlName [MAX_STR_SIZE];


#if (0)

//TODO: they are old

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

/* Name of XML file with cast of given switch */
char cXmlName[MAX_STR_SIZE];

#endif /* (0) */


#define AUX_FNAME "./auxiliary.c"

#endif /* _AUXILIARY_H_ */
