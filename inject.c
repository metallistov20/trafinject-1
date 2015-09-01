/*
 Tested switches: TL-SL2428, TL-SL2218, TL-SL5428E

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
#include <getopt.h>

#include <curl/curl.h>

#include <libxml/parser.h>
#include <libxml/tree.h>


#include "verbose.h"
#include "opcodes.h"
#include "constants.h"


/* Index of desired operation (can be 'create', 'save', 'ACL', 'firmware' )*/
int iOperation;


/* Intermediate URL structure */
CURL *curl;

/* Extended edition of current SW framework is expected to process the ErrCode */
CURLcode res;

/* Aux URL 1 */
char cUrl1[MAX_URL_SIZE];

/* Aux URL 2 */
char cUrl2[MAX_URL_SIZE];

/* Aux URL 3 */
char cUrl3[MAX_URL_SIZE];

/* Aux URL 4 */
char cUrl4[MAX_URL_SIZE];

/* Aux URL 5 */
char cUrl5[MAX_URL_SIZE];

/* Aux URL, presumably required on certain managed switches */
char cUrlScc[MAX_URL_SIZE];

/*  Program name */
char cArg0[MAX_URL_SIZE];

/* IP adress of target switch */
char cIpAddr[MAX_IP_SIZE];

/* Session tID, valid between calls <iOpenSite> and <post_site>  */
char cTid[MAX_TID_SIZE];

/* Name of SNMP community to be created on target switch */
char cSnmp[MAX_SNMP_SIZE];

/* Parameters of ACL froup to be created on target switch */
char cAcl[MAX_STR_SIZE];

/* static IP address to be assigned to a switch */
char cAddr[MAX_IP_SIZE];

/* sbnet mask to be assigned along with static IP address */
char cMask[MAX_IP_SIZE];

/* Name of switch model. 5 chars long, so assuming MAX_SNMP_SIZE is enough */
char cModel[MAX_STR_SIZE];

/* Name of firmware to upload and burn. Assuming length MAX_SNMP_SIZE is sifficient */
char cFwName[MAX_STR_SIZE];

/* IP adress to ve assigned of target switch */
char cNewIpAddr[MAX_IP_SIZE];


/* Payload of POST method during user authenticate */
static const char *cPostMethodString="username=admin&password=admin&logon=Login";

/* Extra payload of POST method during user  authenticate */
static const char *cPostMethodString2="user=admin&level=1&userID=0";

/* Sometimes we just to post not empty payload */
static const char *cAny="ArbitraryString";

/*
 Function to open the secure HTTP stream on network switches 
 TL-SLxxxxx.

 1. Requests <cPostMethodString> and <cPostMethodString2> are necessary
 to start the dynamic pages for current HTTPS connection.

 2. In addition, request <cPostMethodString2> generates some responce
 traffic which contains session tID, needed for further manipulations.

 E.g. to execute function <iSaveSite>, as well as function 
 <iCreateSnmp>, on TL-SL5428E we require this session tID. On 
 other swiches (such as TL-SL2428, TL-SL2218) we don't require 
 this tID.

 3. Once login on site is successully performed, user 'admin' will
 not be able to login on site from other process. So to open site,
 say, from browser firstly logout (e.g. call function <iCloseSite>
 and then do it).
*/
int iOpenSite()
{
	/* Create URL for first POST injection  */
	strcpy (cUrl1, "http://");
	strcat (cUrl1, cIpAddr);
	strcat (cUrl1, "/logon/LogonRpm.htm");
	DURL("%s: iOpenSite: cUrl1 = %s\n", cArg0, cUrl1);


	/* Create URL for second POST injection  */
	strcpy (cUrl2, "http://");
	strcat (cUrl2, cIpAddr);
	strcat (cUrl2, "/");
	DURL("%s: iOpenSite: cUrl2 = %s\n", cArg0, cUrl2) ;


	/* Write user autentication data, attention (not '-u <parameter>') */
	curl_easy_setopt(curl, CURLOPT_URL, cUrl1);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, cPostMethodString);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(cPostMethodString));
	res = curl_easy_perform(curl);

	/* Write tail of autentication data, results in backflow of HTTP traffic, containing t_gID */
	curl_easy_setopt(curl, CURLOPT_URL, cUrl2);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, cPostMethodString2);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(cPostMethodString2));
	res = curl_easy_perform(curl);

	/* Now the site is opened */
	return INJ_SUCCESS;
}

/*
 Function to close the opened secure HTTP connection on network
 switches TL-SLxxxxx.
*/
int iCloseSite()
{
#if (1) //TODO: CHECK IF IT IS NECESSARY ON L2-MANAGED SWITCHES. ASSUMING THAT IS.
	strcpy (cUrlScc, "http://");
	strcat (cUrlScc, &cIpAddr[0]);
	strcat (cUrlScc, "/userRpm/SystemInfoRpm.htm?t=both&_tid_=");
	strcat (cUrlScc, cAny);
	DURL("%s: cUrlScc = %s\n", cArg0, cUrlScc) ;

	curl_easy_setopt(curl, CURLOPT_URL, cUrlScc);
	res = curl_easy_perform(curl);
#endif /* (0) */

	strcpy (cUrl1, "http://");
	strcat (cUrl1, (char*)cIpAddr);
	strcat (cUrl1, "/userRpm/Logout.htm");
	DURL("%s: cUrl1 = %s\n", cArg0, cUrl1);

	strcpy (cUrl2, "http://");
	strcat (cUrl2, cIpAddr);
	strcat (cUrl2, "/");
	DURL("%s: cUrl2 = %s\n", cArg0, cUrl2);


	/* Post reference to calling code (this results on execution of logging current user out) */
	curl_easy_setopt(curl, CURLOPT_URL, cUrl1);
	res = curl_easy_perform(curl);

	/* Finalize the HTTP-exchange with closed site (provides cosy and tide finalization of current session) */
	curl_easy_setopt(curl, CURLOPT_URL, cUrl2);
	res = curl_easy_perform(curl);

	/* Now the site is closed, secure session is terminated, and there's no other way to call 
	iCreateSnmp(), iSaveSite(), etc.. unless the iOpenSite() is called again. */
	return INJ_SUCCESS;
}

/*
 Function to simulate user activity on SNMP tabs of web interface
 of network switches TL-SLxxxxx.
 
 Such activity can be:
   a) creation of SNMP community named <cSnmp> with
 read-write rights;
   b) clicking item <Enable> and button <Aply>.
*/
int iCreateSnmp()
{
	strcpy (cUrl1, "http://");
	strcat (cUrl1, cIpAddr);
	DURL("%s: cUrl1 = %s\n", cArg0, cUrl1);

	strcpy (cUrl2, cUrl1);
	strcat (cUrl2, "/userRpm/SNMPv3CommunityConfigRpm.htm");
	DURL("%s: cUrl2 = %s\n", cArg0, cUrl2);

	strcpy (cUrl3, cUrl2);
	strcat (cUrl3, "?s_userlevel=1&_tid_=");
	strcat (cUrl3, cTid);
	DURL("%s: cUrl3 = %s\n", cArg0, cUrl3);

	strcpy (cUrl4, cUrl2);
	strcat (cUrl4, "?txt_comname=");
	strcat (cUrl4, cSnmp);
	strcat (cUrl4, "&comRight=2&comView=0&button=Add&_tid_=");
	strcat (cUrl4, cTid);
	DURL("%s: cUrl4 = %s\n", cArg0, cUrl4);

	strcpy (cUrl5, cUrl1);
	strcat (cUrl5, "/userRpm/SNMPv3GlobalConfigRpm.htm?snmpState=1&button=stateSubmit&_tid_=");
	strcat (cUrl5, cTid);
	DURL("%s: cUrl5 = %s\n", cArg0, cUrl5);


	/* Ener site TODO: CHECK IF 'CURLOPT_USERPWD' IS NEEDED. ASSUMING THAT IS. */
	curl_easy_setopt(curl, CURLOPT_URL, cUrl1 );
	curl_easy_setopt(curl, CURLOPT_USERPWD, "admin:admin");
	res = curl_easy_perform(curl);

	/* Generate community-generation-page */
	curl_easy_setopt(curl, CURLOPT_URL, cUrl2);
	res = curl_easy_perform(curl);

	/* Link it with its rID */
	curl_easy_setopt(curl, CURLOPT_URL, cUrl3);
	res = curl_easy_perform(curl);

	/* Further preparation */
	curl_easy_setopt(curl, CURLOPT_URL, cUrl4);
	res = curl_easy_perform(curl);

	/* Finally make a Comunity with RW rights */
	curl_easy_setopt(curl, CURLOPT_URL, cUrl5);
	res = curl_easy_perform(curl);

	/* SNMP gruop created */
	return INJ_SUCCESS;
}

/*
 Function to simulate user activity in web interface 
 of network switches TL-SLxxxxx.
 
 Such activity is:
   clicking "Save changes" button in main menu of web interface.
*/
int iSaveSite()
{
	strcpy (cUrl1, "http://");
	strcat (cUrl1, cIpAddr);
	DURL("%s: cUrl1 = %s\n", cArg0, cUrl1) ;

	/* strings like : userRpm/ConfigsaveRpm.htm?s_userlevel=1&_tid_=dd9ac104c8e66d9a&_tid_=dd9ac104c8e66d9a */
	strcpy (cUrl3, cUrl1);
	strcat (cUrl3, "/userRpm/ConfigsaveRpm.htm?s_userlevel=1&_tid_=");
	strcat (cUrl3, cTid);
	strcat (cUrl3, "&_tid_=");
	strcat (cUrl3, cTid);
	DURL("%s: cUrl3 = %s\n", cArg0, cUrl3);

	/* strings like : userRpm/ConfigsaveImg.htm HTTP/1.1  */
	strcpy (cUrl4, cUrl1);
	strcat (cUrl4, "/userRpm/ConfigsaveImg.htm");
	DURL("%s: cUrl4 = %s\n", cArg0, cUrl4);

	/* strings like : userRpm/ConfigsaveFileRpm.htm?_tid_=ce263c00cb83ba57 HTTP/1.1  */
	strcpy (cUrl2, cUrl1);
	strcat (cUrl2, "/userRpm/ConfigsaveFileRpm.htm?_tid_=");
	strcat (cUrl2, cTid);
	DURL("%s: cUrl2 = %s\n", cArg0, cUrl2);


	/* Enter site TODO: CHECK IF 'CURLOPT_USERPWD' IS NEEDED. ASSUMING THAT IS. */
	curl_easy_setopt(curl, CURLOPT_URL, cUrl1 );
	curl_easy_setopt(curl, CURLOPT_USERPWD, "admin:admin");
	res = curl_easy_perform(curl);

	/* Save */
	curl_easy_setopt(curl, CURLOPT_URL, cUrl3);
	res = curl_easy_perform(curl);

	/* Generate button */
	curl_easy_setopt(curl, CURLOPT_URL, cUrl4);
	res = curl_easy_perform(curl);

	/* Wait for responce to arrive. TODO: CHECK IF IT IS NECESSARY ON L2-MANAGED SWITCHES. ASSUMING THAT IS. */
	curl_easy_setopt(curl, CURLOPT_URL, cUrl2);
	res = curl_easy_perform(curl);

	/* Chages on site were saved */
	return INJ_SUCCESS;
}

/*

 Function to upload new firmware onto switch and upgrade it there 

*/
int iUpgradeFirmware()
{
	/* Under construction */
	strcpy (cUrl1, "http://");
	strcat (cUrl1, cIpAddr);
	strcat (cUrl1, "/userRpm/FirmwareRpm.htm?s_userlevel=1&_tid_=");
	strcat (cUrl1, cTid);
	DURL("%s: cUrl1 = %s\n", cArg0, cUrl1);

	/* Under construction */
	strcpy (cUrl2, "http://");
	strcat (cUrl2, cIpAddr);
	strcat (cUrl2, "/help/FirmwareHelpRpm.htm");
	DURL("%s: cUrl2 = %s\n", cArg0, cUrl2);

	/* Under construction */
	sprintf (cUrl3, "/usr/bin/curl -F filedata=@%s http://%s/ ", cFwName, cIpAddr);
	DURL("%s: cUrl3 = %s\n", cArg0, cUrl3);

	/* Under construction */
	sprintf (cUrl4, "/usr/bin/curl --form submit=@%s --form submit=upgrade --form _tid_=%s  http://%s/userRpm/FirmwareAdRpm.htm",  cFwName, cTid, cIpAddr);
	DURL("%s: cUrl4 = %s\n", cArg0, cUrl4);

	/* Under construction */
	strcpy (cUrl5, "http://");
	strcat (cUrl5, cIpAddr);
	strcat (cUrl5, "/userRpm/FirmwareUpdateTempRpm.htm?_tid_");
	strcat (cUrl5, cTid);
	DURL("%s: cUrl5 = %s\n", cArg0, cUrl5);


	/*  Open FW Upgrade page. TODO: CHECK IF 'CURLOPT_USERPWD' IS NEEDED. ASSUMING THAT IS. */
	curl_easy_setopt(curl, CURLOPT_URL, cUrl1 );
	curl_easy_setopt(curl, CURLOPT_USERPWD, "admin:admin");
	res = curl_easy_perform(curl);

	/*  Get aux responce */
	curl_easy_setopt(curl, CURLOPT_URL, cUrl2);
	res = curl_easy_perform(curl);

	/* Upload file */
	system(cUrl3);

	/* Click button upgrade */
	system(cUrl4);

	/* Grabe responce */
	curl_easy_setopt(curl, CURLOPT_URL, cUrl5);
	res = curl_easy_perform(curl);

	/* Firmware was being flashed into EEROM now */
	return INJ_SUCCESS;
}

/* 

Performs an ACL settings (group creation). Tested on switches: TL-SL2428 (TODO: to be tested on TL-SL2218, TL-SL5428E).

*/
int iAclGroup()
{
	strcpy (cUrl1, "http://");
	strcat (cUrl1, cIpAddr);
	DURL("%s: cUrl1 = %s\n", cArg0, cUrl1);

	strcpy (cUrl2, cUrl1);
	strcat (cUrl2, "/userRpm/ACLRuleCreateRpm.htm?s_userlevel=1&_tid_=");
	strcat (cUrl2, cTid);
	DURL("%s: cUrl2 = %s\n", cArg0, cUrl2);

	strcpy (cUrl3, cUrl1);
	strcat (cUrl3, "/userRpm/ACLRuleCreateRpm.htm?submit=Submit&aclId=");
	strcat (cUrl3, cAcl);
	strcat (cUrl3, "&ruleOrder=1&_tid_=");
	strcat (cUrl3, cTid);
	DURL("%s: cUrl3 = %s\n", cArg0, cUrl3);

	/* Enter site TODO: CHECK IF 'CURLOPT_USERPWD' IS NEEDED. ASSUMING THAT IS. */
	curl_easy_setopt(curl, CURLOPT_URL, cUrl1 );
	curl_easy_setopt(curl, CURLOPT_USERPWD, "admin:admin");
	res = curl_easy_perform(curl);

	/* TODO: add comment 1 */
	curl_easy_setopt(curl, CURLOPT_URL, cUrl2);
	res = curl_easy_perform(curl);

	/* TODO: add comment 2 */
	curl_easy_setopt(curl, CURLOPT_URL, cUrl3);
	res = curl_easy_perform(curl);

}

/* 

Issue reboot command. Tested on switches: TL-SL2428, TL-SL2218, TL-SL5428E.

*/
int iRebootSwitch()
{
	strcpy (cUrl1, "http://");
	strcat (cUrl1, cIpAddr);
	strcat (cUrl1, "/userRpm/RestoreRpm.htm?s_userlevel=1&_tid_=");
	strcat (cUrl1, cTid);
	DURL("%s: cUrl1 = %s\n", cArg0, cUrl1);

	strcpy (cUrl2, "http://");
	strcat (cUrl2, cIpAddr);
	strcat (cUrl2, "/userRpm/ReiniRstAdRpm.htm?restore=Reset&_tid_=");
	strcat (cUrl2, cTid);
	DURL("%s: cUrl2 = %s\n", cArg0, cUrl2);

	strcpy (cUrl3, "http://");
	strcat (cUrl3, cIpAddr);
	strcat (cUrl3, "/userRpm/ReiniRstAdTempRpm.htm?_tid_=");
	strcat (cUrl3, cTid);
	DURL("%s: cUrl3 = %s\n", cArg0, cUrl3);

	/* TODO: add comment 1 */
	curl_easy_setopt(curl, CURLOPT_URL, cUrl1 );
	res = curl_easy_perform(curl);

	/* TODO: add comment 2 */
	curl_easy_setopt(curl, CURLOPT_URL, cUrl2);
	res = curl_easy_perform(curl);

	/* TODO: add comment 3 */
	curl_easy_setopt(curl, CURLOPT_URL, cUrl3);
	res = curl_easy_perform(curl);

	/*  TODO: add comm */
	return INJ_SUCCESS;
}

/* 

Set a static IP address with subnet mask. 

Was tested on TL-SL2428, TL-SL2218, TL-SL5428E.

*/
int iAssignIp()
{
	/* TODO: CHECK IF MANDATORY. ASSUMING THAT IS. */
	strcpy (cUrl1, "http://");
	strcat (cUrl1, cIpAddr);
	strcat (cUrl1, "/SystemInfoRpm.htm?s_userlevel=1&_tid_=");
	strcat (cUrl1, cTid);
	strcat (cUrl1, "&_tid_=");
	strcat (cUrl1, cTid);	
	DURL("%s: cUrl1 = %s\n", cArg0, cUrl1);

	/* Enter Tab <IP v 4 address> */
	strcpy (cUrl2, "http://");
	strcat (cUrl2, cIpAddr);
	strcat (cUrl2, "/userRpm/SystemIpRpm.htm?s_userlevel=1&_tid_=");
	strcat (cUrl2, cTid);
	DURL("%s: cUrl2 = %s\n", cArg0, cUrl2);

	/* Prepare envelope with new address and subnem mask (adn rest) */
	strcpy (cUrl3, "http://");
	strcat (cUrl3, cIpAddr);
	strcat (cUrl3, "/userRpm/SystemIpRpm.htm?ip_mode=0&ip_mgmt_vlanid=1&ip_address=");
	strcat (cUrl3, cAddr);
	strcat (cUrl3, "&ip_mask=");
	strcat (cUrl3, cMask);
	strcat (cUrl3, "&ip_gateway=&submit=Apply&_tid_=");
	strcat (cUrl3, cTid);
	DURL("%s: cUrl3 = %s\n", cArg0, cUrl3);

#if (1)
// TODO: L4 (TCP) generates retransmission here. Why?
// 192.168.0.138	HTTP	638	GET /userRpm/SystemIpRpm.htm?ip_mode=0&ip_mgmt_vlanid=1&ip_address=192.168.0.139&ip_mask=255.255.255.0&ip_gateway=&submit=Apply&_tid_=88cd6bf5f8f06e81
// 192.168.0.139

	/* Target address is already new. TODO: CHECK IF MANDATORY. ASSUMING THAT IS. */
	strcpy (cUrl5, "http://");
	strcat (cUrl5, cAddr);
	strcat (cUrl5, "/");
	DURL("%s: cUrl5 = %s\n", cArg0, cUrl5);
#endif /* (0) */

	/* Target address is already new */
	strcpy (cUrl4, "http://");
	strcat (cUrl4, cAddr);
	strcat (cUrl4, "/userRpm/SystemInfoRpm.htm?s_userlevel=1&_tid_=");
	strcat (cUrl4, cTid);
	DURL("%s: cUrl4 = %s\n", cArg0, cUrl4);

	/* TODO: add comment 1 */
	curl_easy_setopt(curl, CURLOPT_URL, cUrl1 );
	res = curl_easy_perform(curl);

	/* TODO: add comment 2 */
	curl_easy_setopt(curl, CURLOPT_URL, cUrl2);
	res = curl_easy_perform(curl);

	/* TODO: add comment 3 */
	curl_easy_setopt(curl, CURLOPT_URL, cUrl3);
	res = curl_easy_perform(curl);

	/*  TODO: CHECK IF MANDATORY. ASSUMING THAT IS. */
	curl_easy_setopt(curl, CURLOPT_URL, cUrl5);
	res = curl_easy_perform(curl);

	/* TODO: add comment 4 */
	curl_easy_setopt(curl, CURLOPT_URL, cUrl4);
	res = curl_easy_perform(curl);


	/*  TODO: add comm */
	return INJ_SUCCESS;
}

int iBindMacIp()
{
	DCOMMON("%s: MAC-IP binding OPCODE=%d is not yet implemented\n", cArg0, iOperation);

	/* Opetation is not yet implemented */
	return INJ_NOT_IMPL;
}

/* 

Not tested. TODO: test on 2218, 2428, 5428E

*/
int iEnablePort()
{
#if (0)
//TODO: remove what is below
 GET /userRpm/SystemInfoRpm.htm?t=port&_tid_=58b10980f6f32c2f
 GET /userRpm/PortStatusSetRpm.htm?s_userlevel=1&_tid_=58b10980f6f32c2f
 GET /userRpm/PortStatusSetRpm.htm?txt_ipaddr=&state=1&spd=0&flowctrl=0&chk_1=1&chk_2=1&chk_3=1&submit=Apply&_tid_=58b10980f6f32c2f&t_port_name= 
#endif /* (0) */

	/* TODO: add comment */
	strcpy (cUrl1, "http://");
	strcat (cUrl1, cIpAddr);
	strcat (cUrl1, "/userRpm/SystemInfoRpm.htm?t=port&_tid_=");
	strcat (cUrl1, cTid);
	DURL("%s: cUrl1 = %s\n", cArg0, cUrl1);

	/* TODO: CHECK IF MANDATORY. ASSUMING THAT IS. */
	strcpy (cUrl2, "http://");
	strcat (cUrl2, cIpAddr);
	strcat (cUrl2, "/userRpm/PortStatusSetRpm.htm?s_userlevel=1&_tid_=");
	strcat (cUrl2, cTid);
	DURL("%s: cUrl2 = %s\n", cArg0, cUrl2);

	/* Prepare message with new address and subnem mask (and rest) */
	strcpy (cUrl3, "http://");
	strcat (cUrl3, cIpAddr);
	strcat (cUrl3, "/userRpm/PortStatusSetRpm.htm?txt_ipaddr=&state=1&spd=0&flowctrl=0&chk_1=1&chk_2=1&chk_3=1&submit=Apply&_tid_=");
	strcat (cUrl3, cTid);
	strcat (cUrl3, "&t_port_name= ");
	DURL("%s: cUrl3 = %s\n", cArg0, cUrl3);

	/* TODO: add comment 1 */
	curl_easy_setopt(curl, CURLOPT_URL, cUrl1 );
	res = curl_easy_perform(curl);

	/* TODO: add comment 2 */
	curl_easy_setopt(curl, CURLOPT_URL, cUrl2);
	res = curl_easy_perform(curl);

	/* TODO: add comment 3 */
	curl_easy_setopt(curl, CURLOPT_URL, cUrl3);
	res = curl_easy_perform(curl);

	/* Opetation is not yet implemented */
	return INJ_SUCCESS;
}


#define print_element_names(x) _print_element_names(__func__, (x))
static void _print_element_names(const char * caller, xmlNode * a_node)
{
	xmlNode *cur_node = NULL;

	for (cur_node = a_node; cur_node; cur_node = cur_node->next)
	{
#if (1)
		if (XML_ELEMENT_NODE == cur_node->type)

			printf("[%s]: name=%s  type=%s \n", caller,  cur_node->name, "XML_ELEMENT_NODE");
#endif /* (0) */


		if ( XML_TEXT_NODE == cur_node->type)
			printf("[%s]:   type=%d  content=(%s)\n", caller,  cur_node->type, cur_node->content);


		print_element_names(cur_node->children);
	}
}

#define strip_nl(x,y) _strip_nl(__func__, (x), (y))
static void _strip_nl(const char * caller, char * a_node, const char * template)
{
char *token;

	/* get the first token */
	token = strtok(a_node, template);

	/* walk through other tokens */
	while( token != NULL ) 
	{
		printf(">>>>%s<<<<\n", token );

		token = strtok(NULL, template);
	}

}




char * strtok1(char *s1, const char *delimit)

{
static char *lastToken = NULL;

char *tmp;

    /* Skip leading delimiters if new string. */
    if ( s1 == NULL )
    {
        s1 = lastToken;
        if (s1 == NULL)         /* End of story? */
            return NULL;
    }
    else
    {
        s1 += strspn(s1, delimit);
    }

    /* Find end of segment */
    tmp = strpbrk(s1, delimit);
    if (tmp)
    {
        /* Found another delimiter, split string and save state. */
        *tmp = '\0';
        lastToken = tmp + 1;
    }
    else
    {
        /* Last segment, remember that. */
        lastToken = NULL;
    }

    return s1;
}

char * strpbrk2(char *s1, char *s2)
{
	for (; *s1; s1++)
	{
		if (strchr (s2, *s1) != 0)
			return ((char *)s1);
	}
	return (0);
}

int strspn2(char *p, char *s)
{
	int i, j;

	for (i = 0; p[i]; i++)
	{
		for (j = 0; s[j]; j++)
		{
			if (s[j] == p[i])
				break;
		}
		if (!s[j])
			break;
	}

	return (i);
}

char * strtok2(char *s1, char *delimit)
{
static char *lastToken = NULL;

char *tmp;

    /* Skip leading delimiters if new string. */
    if ( s1 == NULL )
    {
        s1 = lastToken;

        if (s1 == NULL)
            return NULL;
    }
    else
    {
        s1 += strspn2(s1, delimit);
    }

    /* Find end of segment */
    tmp = strpbrk2(s1, delimit);

    if (tmp)
    {
        /* Found another delimiter, split string and save state. */
        *tmp = '\0';

        lastToken = tmp + 1;
    }
    else
    {
        /* Last segment, remember that. */
        lastToken = NULL;
    }

    return s1;
}



void _unat(char * tkn)
{
char *_localCopy;

char *_localToken;

	_localCopy=strndup(tkn, strlen(tkn));
	_localToken=strtok2(_localCopy, "@");

	while( _localToken != NULL ) 
	{
		printf("\t\t%s\n", _localToken );

		_localToken = strtok2(NULL, "@");
	}

	free(_localCopy);
}



void _untab(char * tkn)
{
char *_localCopy;
char *_localToken;

char *cParcedOut;

	_localCopy=strndup(tkn, strlen(tkn));

	_localToken=strtok1(_localCopy, "\t");

	while( _localToken != NULL ) 
	{
		cParcedOut = strndup(_localToken+strlen("URL=\""), strlen(_localToken) - strlen("URL=\"") );

		cParcedOut[strlen(cParcedOut) -1 ] = 0;

		//TODO: REMOVE! printf("\t{%s}\n", _localToken );
		//TODO: REMOVE! printf("\t[%s]\n", cParcedOut );

		_unat(cParcedOut);

		_localToken = strtok1(NULL, "\t");

		free (cParcedOut);
	}


	free(_localCopy);
}



#define find_named_element(x,y) _find_named_element(__func__, (x), (y))
static void _find_named_element(const char * caller, xmlNode * a_node, const char * template)
{
	xmlNode *cur_node = NULL;

	for (cur_node = a_node; cur_node; cur_node = cur_node->next)
	{
		if (XML_ELEMENT_NODE == cur_node->type)
		{

			/* DEBUG: printf("[%s]: name=%s  type=%s \n", caller,  cur_node->name, "XML_ELEMENT_NODE"); */
	
			/* El't has been found by template?  */
			if ( 0 == strcmp (template, cur_node->name) )
			{
				xmlNode *_ch_cur_node = NULL;

				/* melde sich */
				printf("[%s]: The element name=<%s> has been found (type=%s)\n", caller,  cur_node->name, "XML_ELEMENT_NODE");

				/* Print its contents */
				for (_ch_cur_node = cur_node->children; _ch_cur_node; _ch_cur_node = _ch_cur_node->next)


					if ( XML_TEXT_NODE == _ch_cur_node->type)
					/* 1. splitting into Newline-terminated */
					{
//printf("[[[[%s]]]]\n", _ch_cur_node->content );
					char *token = _ch_cur_node->content;

						token = strtok(token, "\n");

						/* walk through other tokens */
						while( token != NULL ) 
						{

							//printf(">>>>%s<<<<\n", token );
							_untab(token);

							token = strtok(NULL, "\n");

						}

					}


				/* and get away */
				break;
			}
		}


		/* If not found by template let's try with its children */
		find_named_element(cur_node->children, template);
	}
}


int main (int argc, char **argv)
{
int iOption;

xmlDoc *doc = NULL;

xmlNode *root_element = NULL;

	/* Name of XML file to parse must be given */
	if (argc != 2)
	{

		DCOMMON("%s: ERROR: name of XML file to parse should be passed on command line\n", cArg0);

		return INJ_PAR_ERROR;
	}

	/* Check potential ABI mismatches between the version it was compiled for and the actual shared library used */
	LIBXML_TEST_VERSION

	/* Parse the file and get the DOM */
	doc = xmlReadFile(argv[1], NULL, 0);

	if (NULL == doc)
	{
		DCOMMON("%s: ERROR: could not parse file %s\n", cArg0, argv[1]);

		return INJ_XML_ERROR;
	}

	/*Get the root element node */
	root_element = xmlDocGetRootElement(doc);

#if (0)
	print_element_names(root_element);
#else
	find_named_element(root_element, "TL-SL5428E");

	find_named_element(root_element, "System_Info");

	find_named_element(root_element, "Device_Description");

	find_named_element(root_element, "System_Time");

	find_named_element(root_element, "Daylight_Saving_Time");

	find_named_element(root_element, "System_IP");

	find_named_element(root_element, "System_IPv6");

	find_named_element(root_element, "System_Reboot");

	find_named_element(root_element, "ACL_Create");

	find_named_element(root_element, "Save_Config");

	find_named_element(root_element, "Logout");

#endif /* (0) */

	/* Free the document */
	xmlFreeDoc(doc);

	/* Free the global variables that may have been allocated by the parser */
	xmlCleanupParser();

	return INJ_SUCCESS;

	DCOMMON("%s: this line is not seen\n", cArg0, iOperation);


}
