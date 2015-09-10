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

#if (0)

char _tid_ [MAX_TID_SIZE] = "_tid_";

char ip_address [MAX_IP_SIZE] = "ip_address";

char ip_mask [MAX_IP_SIZE] = "ip_mask";

char txt_comname [MAX_SNMP_SIZE] = "txt_comname";

char aclId [MAX_SNMP_SIZE] = "aclId";

#else

#include "constants.h"

char _tid_ [MAX_TID_SIZE];

char ip_address [MAX_IP_SIZE];

char ip_mask [MAX_IP_SIZE];

char txt_comname [MAX_SNMP_SIZE];

char aclId [MAX_SNMP_SIZE];


#include "auxiliary.h"

struct _QuineType QuineArray[] =
{
	{ &_tid_  , "_tid_" },

	{ &ip_address  , "ip_address"},

	{ &ip_mask , "ip_mask"},

	{ &txt_comname , "txt_comname"},

	{ &txt_comname , "txt_comname"},

	{ &aclId , "aclId"},

	/* End of array */
	{0, 0}
};

#endif /* (0) */

char cIpAddr [MAX_IP_SIZE];

char cFwName [MAX_STR_SIZE];

char cXmlName [MAX_STR_SIZE];


