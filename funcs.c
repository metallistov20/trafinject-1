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

#include <curl/curl.h>

#include <libxml/parser.h>
#include <libxml/tree.h>

#include "inject.h"
#include "verbose.h"
#include "opcodes.h"
#include "constants.h"
#include "lists.h"
#include "xmls.h"
#include "auxiliary.h"

/* Buffer for <iUpgradeFirmware()> */
static char cUrlUploadFile[MAX_URL_SIZE];

/* Another buffer for <iUpgradeFirmware()> */
static char cUrlUpgrade[MAX_URL_SIZE];

/* Extended edition of current SW framework is expected to process the ErrCode */
static CURLcode res;

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
	/* Put XML section <TL-SL5428E> into structure <pUrlChain> */
	parse_xml_cast(root_element, "TL-SL5428E");

	/* Glue particles of <pUrlChain> into full-blown URLs */
	GlueUrl(pUrlChain);

#if (DEBUG_URL)
	DisplayUrl(pUrlChain);
#endif /* (DEBUG_URL) */

	/* Put URLs into wire */
	return DeployUrlEx(pUrlChain, 1);
}

/*
 Function to close the opened secure HTTP connection on network
 switches TL-SLxxxxx.
*/
int iCloseSite()
{
	/* Put XML section <Logout> into structure <pUrlChain> */
	parse_xml_cast(root_element, "Logout");

	/* Glue particles of <pUrlChain> into full-blown URLs */
	GlueUrl(pUrlChain);

#if (DEBUG_URL)
	DisplayUrl(pUrlChain);
#endif /* (DEBUG_URL) */

	/* Put URLs into wire */
	return DeployUrlEx(pUrlChain, 0);
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
	/* Put XML section <SNMP_Community> into structure <pUrlChain> */
	parse_xml_cast(root_element, "SNMP_Community");

	/* Glue particles of <pUrlChain> into full-blown URLs */
	GlueUrl(pUrlChain);

#if (DEBUG_URL)
	DisplayUrl(pUrlChain);
#endif /* (DEBUG_URL) */

	/* Put URLs into wire */
	return DeployUrl(pUrlChain);
}

/*
 Function to simulate user activity in web interface 
 of network switches TL-SLxxxxx.
 
 Such activity is:
   clicking "Save changes" button in main menu of web interface.
*/
int iSaveSite()
{
	/* Put XML section <Save_Config> into structure <pUrlChain> */
	parse_xml_cast(root_element, "Save_Config");

	/* Glue particles of <pUrlChain> into full-blown URLs */
	GlueUrl(pUrlChain);

#if (DEBUG_URL)
	DisplayUrl(pUrlChain);
#endif /* (DEBUG_URL) */

	/* Put URLs into wire */
	return DeployUrl(pUrlChain);
}

/*
 Function to upload new firmware onto switch and upgrade it there 
*/
int iUpgradeFirmware()
{
	/* First portion of URLs */

	/* Put XML section <Firmware_Upgrade> into structure <pUrlChain> */
	parse_xml_cast(root_element, "Firmware_Upgrade");

	/* Glue particles of <pUrlChain> into full-blown URLs */
	GlueUrl(pUrlChain);

#if (DEBUG_URL)
	DisplayUrl(pUrlChain);
#endif /* (DEBUG_URL) */

	/* Put URLs into wire */
	res= DeployUrl(pUrlChain);

	/* If sending first portion of URLs failed then entire procedure failed */ 
	if (res != CURLE_OK)
	{
		DURL("%s: direct part of SetStaticIp failed ERR_CODE(%d)\n", "", res);

		return res;
	}

	/* Otherwise, clean structures for second portion */
	DeleteUrlEx(&pUrlChain);


	/* Second portion of URLs */
	sprintf (cUrlUploadFile, "/usr/bin/curl -F filedata=@%s http://%s/ ", cFwName, cIpAddr);
	DURL("%s: cUrl3 = %s\n", cArg0, cUrlUploadFile);

#if (IDLE_RUN)
		DURL("%s: idle run mode: the HTTP injection was not physically sent\n", "");
#else
	/* Button <Upload file>. Put URLs into wire with system() command  */
	system(cUrlUploadFile);
#endif /* (IDLE_RUN) */


	/* Third portion of URLs */
	sprintf (cUrlUpgrade, "/usr/bin/curl --form submit=@%s --form submit=upgrade --form _tid_=%s  http://%s/userRpm/FirmwareAdRpm.htm",  cFwName, _tid_, cIpAddr);
	DURL("%s: cUrl4 = %s\n", cArg0, cUrlUpgrade);

#if (IDLE_RUN)
		DURL("%s: idle run mode: the HTTP injection was not physically sent\n", "");
#else
	/* Button <Upgrade>. Put URLs into wire with system() command  */
	system(cUrlUpgrade);
#endif /* (IDLE_RUN) */

	/* Forth portion of URLs */

	/* Put XML section <Firmware_Upgrade_backdraft> into structure <pUrlChain> */	
	parse_xml_cast(root_element, "Firmware_Upgrade_backdraft");

	/* Glue particles of <pUrlChain> into full-blown URLs */
	GlueUrl(pUrlChain);

#if (DEBUG_URL)
	DisplayUrl(pUrlChain);
#endif /* (DEBUG_URL) */

	/* Put URLs into wire */
	res = DeployUrl(pUrlChain);

	/* If sending second portion of URLs (a.k.a. 'backfraft' traffic) failed then verbose */ 
	if (res != CURLE_OK)
	{
		DURL("%s: reverse part of SetStaticIp failed ERR_CODE(%d)\n", "", res);
	}

	/* Firmware was being flashed into EEROM now */
	return INJ_SUCCESS;
}

/* 
Performs an ACL settings (group creation). Tested on switches: TL-SL2428 (TODO: to be tested on TL-SL2218, TL-SL5428E). TODO: 10-2015
*/
int iAclGroup()
{
	/* Put XML section <ACL_Create> into structure <pUrlChain> */
	parse_xml_cast(root_element, "ACL_Create");

	/* Glue particles of <pUrlChain> into full-blown URLs */
	GlueUrl(pUrlChain);

#if (DEBUG_URL)
	DisplayUrl(pUrlChain);
#endif /* (DEBUG_URL) */

	/* Put URLs into wire */
	return DeployUrl(pUrlChain);
}

/* 
Issue reboot command. Tested on switches: TL-SL2428, TL-SL2218, TL-SL5428E.
*/
int iRebootSwitch()
{
	/* Put XML section <System_Reboot> into structure <pUrlChain> */
	parse_xml_cast(root_element, "System_Reboot");

	/* Glue particles of <pUrlChain> into full-blown URLs */
	GlueUrl(pUrlChain);

#if (DEBUG_URL)
	DisplayUrl(pUrlChain);
#endif /* (DEBUG_URL) */

	/* Put URLs into wire */
	return DeployUrl(pUrlChain);
}

/* 
Set a static IP address with subnet mask. 

Was tested on TL-SL2428, TL-SL2218, TL-SL5428E.
*/
int iAssignIp()
{
	/* Put XML section <System_IP> into structure <pUrlChain> */
	parse_xml_cast(root_element, "System_IP");

	/* Glue particles of <pUrlChain> into full-blown URLs */
	GlueUrl(pUrlChain);

#if (DEBUG_URL)
	DisplayUrl(pUrlChain);
#endif /* (DEBUG_URL) */	

	/* Put URLs into wire */
	res= DeployUrl(pUrlChain);

	/* If sending first portion of URLs failed then entire procedure failed */ 
	if (res != CURLE_OK)
	{
		DURL("%s: direct part of SetStaticIp failed ERR_CODE(%d)\n", "", res);

		return res;

	}

	/* Otherwise, clean structures for second portion */
	DeleteUrlEx(&pUrlChain);

	memset(&cIpAddr[0],0,MAX_IP_SIZE);
	memcpy(cIpAddr, ip_address, MAX_IP_SIZE);

	/* Put XML section <System_IP_backdraft> into structure <pUrlChain> */	
	parse_xml_cast(root_element, "System_IP_backdraft");

	/* Glue particles of <pUrlChain> into full-blown URLs */
	GlueUrl(pUrlChain);

#if (DEBUG_URL)
	DisplayUrl(pUrlChain);
#endif /* (DEBUG_URL) */

	/* Put URLs into wire */
	res = DeployUrl(pUrlChain);

	/* If sending second portion of URLs (a.k.a. 'backfraft' traffic) failed then verbose */ 
	if (res != CURLE_OK)
	{
		DURL("%s: reverse part of SetStaticIp failed ERR_CODE(%d)\n", "", res);
	}

	return res;
}

/* 
Not tested. TODO: test on 2218, 2428, 5428E. TODO: is it possible <MAC_Address&Static_Address_CRST> without <MAC_Address&Address_Table>? TODO: 10-2015; 
*/
int iBindMacIp()
{
	/* Put XML section  into structure <pUrlChain> */
	parse_xml_cast(root_element, "Static_Address_CRST");

	/* Glue particles of <pUrlChain> into full-blown URLs */
	GlueUrl(pUrlChain);

#if (DEBUG_URL)
	DisplayUrl(pUrlChain);
#endif /* (DEBUG_URL) */

	/* Put URLs into wire */
	return DeployUrl(pUrlChain);
}

/* 
Not tested. TODO: test on 2218, 2428, 5428E. TODO: 10-2015
*/
int iEnablePort()
{
	/* Put XML section <Port_Config> into structure <pUrlChain>.  TODO: which one : MAC_VLAN? Protocol_VLAN? VLAN_VPN? * TODO: 10-2015 */	
	parse_xml_cast(root_element, "Port_Config");

	/* Glue particles of <pUrlChain> into full-blown URLs */
	GlueUrl(pUrlChain);

#if (DEBUG_URL)
	DisplayUrl(pUrlChain);
#endif /* (DEBUG_URL) */

	/* Put URLs into wire */
	return DeployUrl(pUrlChain);
}

/* 
Simple operation. No backdraft, no special handling.
*/
int iMeta1(char * pcXmlEltName)
{
	/* Empty string ised instead of XML-node-name? */
	if (NULL == pcXmlEltName)
	{
		DFUNC("%s: empty XML-node-name\n", "<iMeta1>");

		return INJ_XML_NULL;
	}

	/* Parse named element, and put its XML section into structure <pUrlChain> */	
	parse_xml_cast(root_element, pcXmlEltName);

	/* Glue particles of <pUrlChain> into full-blown URLs */
	GlueUrl(pUrlChain);

#if (DEBUG_URL)
	DisplayUrl(pUrlChain);
#endif /* (DEBUG_URL) */

	/* Put URLs into wire */
	return DeployUrl(pUrlChain);
}

/* 
Complex operation type #1. With backdraft.
*/
int iMeta2(char * pcXmlEltName1, char * pcXmlEltName2)
{
	/* Empty string ised instead some of XML-node-names? */
	if ((NULL == pcXmlEltName1) || (NULL == pcXmlEltName2)  )
	{
		DFUNC("%s: empty XML-node-name(s) \n", "<iMeta2>");

		return INJ_XML_NULL;
	}

	/* Put XML section from node <*pcXmlEltName1>  into structure <pUrlChain> */
	parse_xml_cast(root_element, pcXmlEltName1);

	/* Glue particles of <pUrlChain> into full-blown URLs */
	GlueUrl(pUrlChain);

#if (DEBUG_URL)
	DisplayUrl(pUrlChain);
#endif /* (DEBUG_URL) */

	/* Put URLs into wire */
	res= DeployUrl(pUrlChain);

	/* If sending first portion of URLs failed then entire procedure failed */ 
	if (res != CURLE_OK)
	{
		DURL("%s: direct part of bidirectional funct. failed with ERR_CODE(%d)\n", "<iMeta2>", res);

		return res;

	}

	/* Otherwise, clean structures for second portion */
	DeleteUrlEx(&pUrlChain);

	memset((void*)idle,0,MAX_STR_SIZE);
#if (0)
	memcpy(idle, ?, MAX_STR_SIZE);
#endif /* (0) */

		/* Put XML section from node <*pcXmlEltName2>  into structure <pUrlChain> */
	parse_xml_cast(root_element, pcXmlEltName2);

	/* Glue particles of <pUrlChain> into full-blown URLs */
	GlueUrl(pUrlChain);

#if (DEBUG_URL)
	DisplayUrl(pUrlChain);
#endif /* (DEBUG_URL) */

	/* Put URLs into wire */
	res = DeployUrl(pUrlChain);

	/* If sending second portion of URLs (a.k.a. 'backfraft' traffic) failed then verbose */ 
	if (res != CURLE_OK)
	{
		DURL("%s: reverese part of bidirectional funct. failed with ERR_CODE(%d)\n", "<iMeta2>", res);
	}

	return res;
}
