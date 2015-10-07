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
#include <stdbool.h>
#include <string.h>
#include <getopt.h>

#include <curl/curl.h>

#include <libxml/parser.h>
#include <libxml/tree.h>


#include "verbose.h"
#include "opcodes.h"
#include "constants.h"
#include "lists.h"
#include "xmls.h"
#include "auxiliary.h"


/* Index of desired operation (can be 'create', 'save', 'ACL', 'firmware' )*/
int iOperation;

/* Intermediate URL structure */
CURL *curl;


/* Payload of POST method during user authenticate */
const char *cPostMethodString="username=admin&password=admin&logon=Login";

/* Extra payload of POST method during user  authenticate */
const char *cPostMethodString2="user=admin&level=1&userID=0";

char cArg0[MAX_URL_SIZE];

/* pointers onto cPostMethodString, cPostMethodString2 */
char ** pcPtr2Extra1, **pcPtr2Extra2;

/* Extra payload should be attached  */
int iExtra;

/* Ptr to XML data for in memory */
xmlNode *root_element = NULL;

/* Ptr to XML data document */
xmlDoc *doc = NULL;




int main (int argc, char **argv)
{
int iOption;

	/* Avoid dafault 0 value */
	iOperation=DO_NO_OP;

	/* Assign program name, requirted for output*/
	strcpy (cArg0, argv[0]);

	iExtra = 0;

	/* TODO: find better place for this init */
	pcPtr2Extra1 = (char**)&cPostMethodString;
	pcPtr2Extra2 = (char**)&cPostMethodString2;

	/* Parsing command line arguments */
	while (1)
	{
	static struct option long_options[] =
	{
		/* Singletons: opearion names */
		{"open",no_argument, 0, 	'o'},
		{"close",  no_argument, 0,	'x'},

		{"create",no_argument, 0,	'c'},
		{"save",  no_argument, 0,	's'},
		{"ACL",	  no_argument, 0,	'a'},
		{"upgrade",  no_argument, 0,	'r'},
		{"reboot",  no_argument, 0,	'b'},
		{"ipassign",  no_argument, 0,	'g'},// TODO: change to 'static' or sort of that.

		/* Couples: names of variables and their values */
		{"target",  required_argument, 0,	't'},
		{"id",   required_argument, 0, 		'i'},
		{"community",required_argument, 0,	'u'},
		{"filename",required_argument, 0,	'f'},
		{"acl-data",required_argument, 0,	'l'},
		{"ip-addr",required_argument, 0,	'0'},
		{"ip-mask",required_argument, 0,	'1'},
		{"xml-data",required_argument, 0,	'd'},

		/* End of array */
		{0, 0, 0, 0}
	};

	/* Index of the option */
	int option_index = 0;

		/* Get each paramter */
		iOption = getopt_long (argc, argv, "oxcsarbg:t:i:u:f:l:0:1:d:", long_options, &option_index);

		/* Break cycle at the end of the options */
		if (-1 == iOption) break;

		/* Parce each parameter */
		switch (iOption)
		{

			/* Single: open site */
			case 'o':
				DCOMMON("%s: option -k\n", cArg0);
				iOperation = DO_OPEN_OP;
				break;

			/* Single: close site */
			case 'x':
				DCOMMON("%s: option -x\n", cArg0);
				iOperation = DO_CLOSE_OP;
				break;

			/* Single: create SNMP group */
			case 'c':
				DCOMMON("%s: option -c\n", cArg0);
				iOperation = DO_CREATE_OP;
				break;

			/* Single: save changes on site  */
			case 's':
				DCOMMON("%s: option -s\n", cArg0);
				iOperation = DO_SAVE_OP;
				break;

			/* Single: perform ACL setting */
			case 'a':
				DCOMMON("%s: option -a\n", cArg0);
				iOperation = DO_ACL_OP;
				break;

			/* Single: firmware upload and upgrade */
			case 'r':
				DCOMMON("%s: option -r\n", cArg0);
				iOperation = DO_FIRMWARE_OP;
				break;

			/* Single: switch reboot */
			case 'b':
				DCOMMON("%s: option -b\n", cArg0);
				iOperation = DO_REBOOT_OP;
				break;

			/* Single: ipset */
			case 'g':
				DCOMMON("%s: option -g\n", cArg0);
				iOperation = DO_IPSET_OP;
				break;


			/* Couple: IP addr of target switch */
			case 't':
				DCOMMON("%s: option -t with value `%s'\n", cArg0, optarg);
				strcpy(cIpAddr, optarg);
				break;

			/* Couple: tID of the session */
			case 'i':
				DCOMMON("%s: option -i with value `%s'\n", cArg0, optarg);
				strcpy(_tid_, optarg);
				break;

			/* Couple: SNMP community name to be created on target switch*/
			case 'u':
				DCOMMON("%s: option -u with value `%s'\n", cArg0, optarg);
				strcpy(txt_comname, optarg);
				break;

			/* Couple: Firmware name to be uploaded and upgraded on switch*/
			case 'f':
				DCOMMON("%s: option -f with value `%s'\n", cArg0, optarg);
				strcpy(cFwName, optarg);
				break;

			/* Couple: Filename of XML file with data */
			case 'd':
				DCOMMON("%s: option -d with value `%s'\n", cArg0, optarg);
				strcpy(cXmlName, optarg);
				break;

			/* Couple: Assign ACL setings */
			case 'l':
				DCOMMON("%s: option -l (--acl-data) with value `%s'\n", cArg0, optarg);
				strcpy(aclId, optarg);
				break;

			/* Couple: ip address */
			case '0':
				DCOMMON("%s: option -0 (--ip-addr) with value `%s'\n", cArg0, optarg);
				strcpy(ip_address, optarg);
				break;

			/* Couple: ip network mask */
			case '1':
				DCOMMON("%s: option -1 (--ip-mask) with value `%s'\n", cArg0, optarg);
				strcpy(ip_mask, optarg);
				break;


			case '?':
				/* getopt_long prints an error message, so we don't */
				break;

			default:
				DCOMMON("%s: bad usage, exiting", cArg0);
				abort ();
		}
	} /* Command line arguments were parsed */

	/* Ensure that file <cast.XXXXX.txt.xml>. Generated by <make_xml.sh> */
	if ( NULL == cXmlName )
	{
		DCOMMON("%s: ERROR: name of XML file to parse should be passed on command line\n", cArg0);

		return INJ_PAR_ERROR;
	}

	/* To this moment <voc.c> and <voc.h> are nearby, otherwise <autogen.cmd+make> was failed */
	if (INJ_SUCCESS != XmlAuxCreateEx() ) 
	{
		DCOMMON("%s: ERROR: no rules to handle (%s) learned\n", cArg0, cXmlName);

		return INJ_NOAUX_ERROR;
	}

	/* Check potential ABI mismatches between the version it was compiled for and the actual shared library used */
	LIBXML_TEST_VERSION

	/* Get the contents of <cast.XXXXX.txt.xml> into <doc> */
	doc = xmlReadFile(cXmlName, NULL, 0);

	if (NULL == doc)
	{
		DCOMMON("%s: ERROR: could not parse file %s\n", cArg0, cXmlName);

		return INJ_XML_ERROR;
	}

	/* Get the root node of the XML data stored in the <doc> */
	root_element = xmlDocGetRootElement(doc);

	if(NULL != ( curl = curl_easy_init() ) )
	{
		/* At this time point we assume all parameters parsed OK, so let's call inj. primitives */
		switch (iOperation)
		{
			case DO_OPEN_OP:
				VERBOSE_STATUS(iOpenSite)
				break;

			case DO_CLOSE_OP:
				VERBOSE_STATUS(iCloseSite)
				break;

			case DO_CREATE_OP:
				VERBOSE_STATUS(iCreateSnmp)
				break;

			case DO_SAVE_OP:
				VERBOSE_STATUS(iSaveSite)
				break;

			case DO_ACL_OP:
				VERBOSE_STATUS(iAclGroup)
				break;

			case DO_FIRMWARE_OP:
				VERBOSE_STATUS(iUpgradeFirmware)
				break;

			case DO_REBOOT_OP:
				VERBOSE_STATUS(iRebootSwitch)
				break;

			case DO_IPSET_OP:
				VERBOSE_STATUS(iAssignIp)
				break;


			case DO_NO_OP:
			default:
				DCOMMON("%s: there's no operation with OPCODE=%d, exiting\n", cArg0, iOperation);
				break;
			
		}

		/* Close URL lib */
		curl_easy_cleanup(curl);

		/* Delete entire list with URLs along with its compounds */
		DeleteUrlEx(&pUrlChain);
	}
	else
	{
		DCOMMON("%s: Can't initialize lib cURL, so can't proceed \n", cArg0);
	}

	/* Delete vocabuilary, et al*/
	DeleteXmlAuxEx(&pAuxiliary);

	/* Free the document */
	xmlFreeDoc(doc);

	/* Free the global variables that may have been allocated by the parser */
	xmlCleanupParser();

 	exit (0);

	/* Basically is not seen; but is kept here for accuracy */
	DCOMMON("%s: this line is not seen\n", cArg0, iOperation);
}
