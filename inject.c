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
		/* Singletons: operation names */
		{"open",no_argument, 		0,'o'},// <SYSTEMNAME>
		{"close",  no_argument, 	0,'x'},// Logout

		{"create",no_argument, 		0,'c'},// SNMP_Group
		{"save", no_argument, 		0,'s'},// Save_Config
		{"ACL",	no_argument, 		0,'a'},// ACL_Create
		{"upgrade", no_argument, 	0,'r'},// Firmware_Upgrade, Firmware_Upgrade_backdraft
		{"reboot", no_argument, 	0,'b'},// System_Reboot
		{"ipassign", no_argument, 	0,'g'},// System_IP, System_IP_backdraft

		{"ipV6assign", no_argument, 	0,'e'},// System_IPv6
		{"acontrol", no_argument, 	0,'k'},// Access_Control
		{"psecure", no_argument, 	0,'h'},// Port_Security
		{"pmirror", no_argument, 	0,'j'},// Port_Mirro
		{"vlancrt", no_argument, 	0,'m'},// VLAN_Config_Create
		{"vlanco", no_argument, 	0,'n'},// VLAN_Config
		{"iprange", no_argument, 	0,'p'},// IP_Range
		{"pfilter", no_argument, 	0,'q'},// Port_Filter
		{"ping", no_argument, 		0,'y'},// Ping
		{"tracert", no_argument, 	0,'z'},// Tracert
		{"cable", no_argument, 		0,'v'},// Cable_Test
		{"looback", no_argument, 	0,'w'},// Loopback

		/* Couples: names of variables and their values */
		{"target", required_argument,	0,'t'},// e.g. --target=192.168.0.1
		{"id", required_argument,	0,'i'},// e.g. --id=4b7e2773bc9deeab
		{"community", required_argument,0,'u'},// e.g. --community=NEW_SNMP_CMTY_X
		{"filename", required_argument,	0,'f'},// e.g. --filename=./TL-SL5428Ev3_en_up.bin
		{"acl-data", required_argument,	0,'l'},// e.g. --acl-data=NEW_ACL_GRP_X
		{"ip-addr", required_argument,	0,'0'},// e.g. --ip-addr=192.168.0.199
		{"ip-mask", required_argument,	0,'1'},// e.g. --ip-mask=255.255.255.0
		{"xml-data", required_argument,	0,'d'},// e.g. --xml-data=cast.2218.txt.xml

		{"t_mode", required_argument,	0,'2'},//
		{"t_key", required_argument,	0,'3'},//
		{"t_stat", required_argument,	0,'4'},//
		{"chk_", required_argument,	0,'5'},//
		{"cb_", required_argument,	0,'6'},//
		{"list_", required_argument,	0,'7'},//
		{"member_", required_argument,	0,'8'},//
		{"proof", required_argument,	0,'9'},//e.g. proof=www.speedtest.net/ru/

		/* End of array */
		{0, 0, 0, 0}
	};

	/* Index of the option */
	int option_index = 0;

		/* Get each paramter */
		iOption = getopt_long (argc, argv, "oxcsarbgefhjmnpqyzvw:t:i:u:f:l:0:1:d:2:3:4:5:6:7:8:", long_options, &option_index);

		/* Break cycle at the end of the options */
		if (-1 == iOption) break;

		/* Parce each parameter */
		switch (iOption)
		{

			/* Single: open site */
			case 'o':
				DCOMMON("%s: option -%c\n", cArg0, iOption);
				iOperation = DO_OPEN_OP;
				break;

			/* Single: close site */
			case 'x':
				DCOMMON("%s: option -%c\n", cArg0, iOption);
				iOperation = DO_CLOSE_OP;
				break;

			/* Single: create SNMP group */
			case 'c':
				DCOMMON("%s: option -%c\n", cArg0, iOption);
				iOperation = DO_CREATE_OP;
				break;

			/* Single: save changes on site  */
			case 's':
				DCOMMON("%s: option -%c\n", cArg0, iOption);
				iOperation = DO_SAVE_OP;
				break;

			/* Single: perform ACL setting */
			case 'a':
				DCOMMON("%s: option -%c\n", cArg0, iOption);
				iOperation = DO_ACL_OP;
				break;

			/* Single: firmware upload and upgrade */
			case 'r':
				DCOMMON("%s: option -%c\n", cArg0, iOption);
				iOperation = DO_FIRMWARE_OP;
				break;

			/* Single: switch reboot */
			case 'b':
				DCOMMON("%s: option -%c\n", cArg0, iOption);
				iOperation = DO_REBOOT_OP;
				break;

			/* Single: ipset */
			case 'g':
				DCOMMON("%s: option -%c\n", cArg0, iOption);
				iOperation = DO_IPSET_OP;
				break;


			/* Single: ip v6 set */
			case 'e':
				DCOMMON("%s: option -%c\n", cArg0, iOption);
				iOperation = DO_IPV6SET_OP;
				break;
			/* Single: access */
			case 'k':
				DCOMMON("%s: option -%c\n", cArg0, iOption);
				iOperation = DO_ACNTL_OP;
				break;
			/* Single: port security */
			case 'h':
				DCOMMON("%s: option -%c\n", cArg0, iOption);
				iOperation = DO_PSEC_OP;
				break;
			/* Single: port mirroring */
			case 'j':
				DCOMMON("%s: option -%c\n", cArg0, iOption);
				iOperation = DO_PMIR_OP;
				break;
			/* Single: vlan conf. create */
			case 'm':
				DCOMMON("%s: option -%c\n", cArg0, iOption);
				iOperation = DO_VLANCR_OP;
				break;
			/* Single: vlan configure */
			case 'n':
				DCOMMON("%s: option -%c\n", cArg0, iOption);
				iOperation = DO_VLANC_OP;
				break;
			/* Single: ip range */
			case 'q':
				DCOMMON("%s: option -%c\n", cArg0, iOption);
				iOperation = DO_IPRAN_OP;
				break;
			/* Single: port filter */
			case 'p':
				DCOMMON("%s: option -%c\n", cArg0, iOption);
				iOperation = DO_PFILT_OP;
				break;
			/* Single: ping */
			case 'y':
				DCOMMON("%s: option -%c\n", cArg0, iOption);
				iOperation = DO_PING_OP;
				break;
			/* Single: tracert */
			case 'z':
				DCOMMON("%s: option -%c\n", cArg0, iOption);
				iOperation = DO_TRACERT_OP;
				break;
			/* Single: cable test */
			case 'v':
				DCOMMON("%s: option -%c\n", cArg0, iOption);
				iOperation = DO_CABLE_OP;
				break;
			/* Single: loopback */
			case 'w':
				DCOMMON("%s: option -%c\n", cArg0, iOption);
				iOperation = DO_LOOPBK_OP;
				break;



			/* Couple: IP addr of target switch */
			case 't':
				DCOMMON("%s: option -%c with value `%s'\n", cArg0, iOption, optarg);
				strcpy(cIpAddr, optarg);
				break;

			/* Couple: tID of the session */
			case 'i':
				DCOMMON("%s: option -%c with value `%s'\n", cArg0, iOption, optarg);
				strcpy(_tid_, optarg);
				break;

			/* Couple: SNMP community name to be created on target switch*/
			case 'u':
				DCOMMON("%s: option -%c with value `%s'\n", cArg0, iOption, optarg);
				strcpy(txt_comname, optarg);
				break;

			/* Couple: Firmware name to be uploaded and upgraded on switch*/
			case 'f':
				DCOMMON("%s: option -%c with value `%s'\n", cArg0, iOption, optarg);
				strcpy(cFwName, optarg);
				break;

			/* Couple: Filename of XML file with data */
			case 'd':
				DCOMMON("%s: option -%c with value `%s'\n", cArg0, iOption, optarg);
				strcpy(cXmlName, optarg);
				break;

			/* Couple: Assign ACL setings */
			case 'l':
				DCOMMON("%s: option -%c (--acl-data) with value `%s'\n", cArg0, iOption, optarg);
				strcpy(aclId, optarg);
				break;

			/* Couple: ip address */
			case '0':
				DCOMMON("%s: option -%c (--ip-addr) with value `%s'\n", cArg0, iOption, optarg);
				strcpy(ip_address, optarg);
				break;

			/* Couple: ip network mask */
			case '1':
				DCOMMON("%s: option -%c (--ip-mask) with value `%s'\n", cArg0, iOption, optarg);
				strcpy(ip_mask, optarg);
				break;

			/* Couple:  */
			case '2':
				DCOMMON("%s: option -%c (...) with value `%s'\n", cArg0, iOption, optarg);
				strcpy(idle, optarg);
				break;
			/* Couple:  */
			case '3':
				DCOMMON("%s: option -%c (...) with value `%s'\n", cArg0, iOption, optarg);
				strcpy(idle, optarg);
				break;
			/* Couple:  */
			case '4':
				DCOMMON("%s: option -%c (...) with value `%s'\n", cArg0, iOption, optarg);
				strcpy(idle, optarg);
				break;
			/* Couple:  */
			case '5':
				DCOMMON("%s: option -%c (...) with value `%s'\n", cArg0, iOption, optarg);
				strcpy(idle, optarg);
				break;
			/* Couple:  */
			case '6':
				DCOMMON("%s: option -%c (...) with value `%s'\n", cArg0, iOption, optarg);
				strcpy(idle, optarg);
				break;
			/* Couple:  */
			case '7':
				DCOMMON("%s: option -%c (...) with value `%s'\n", cArg0, iOption, optarg);
				strcpy(idle, optarg);
				break;
			/* Couple:  */
			case '8':
				DCOMMON("%s: option -%c (...) with value `%s'\n", cArg0, iOption, optarg);
				strcpy(idle, optarg);
				break;
			/* Couple:  */
			case '9':
				DCOMMON("%s: option -%c (...) with value `%s'\n", cArg0, iOption, optarg);
				strcpy(idle, optarg);
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

			/* TODO: this is a stub only; no real implementation */
			case DO_IPV6SET_OP:
				VERBOSE_STATUS2(iMeta2, "idle1", "idle2")
				break;

			/* TODO: these are stubs only; no real implementation */
			case DO_ACNTL_OP:
			case DO_PSEC_OP:
			case DO_PMIR_OP:
			case DO_VLANCR_OP:
			case DO_VLANC_OP:
			case DO_IPRAN_OP:
			case DO_PFILT_OP:
			case DO_PING_OP:
			case DO_TRACERT_OP:
			case DO_CABLE_OP:
			case DO_LOOPBK_OP:
				VERBOSE_STATUS1(iMeta1, "idle")
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
