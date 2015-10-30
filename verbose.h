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

#ifndef _VERBOSE_H_
#define _VERBOSE_H_


/* Allow general-purpose printf output */
#define DEBUG_COMMON	1 

/* Show URLs for verification */
#define DEBUG_URL	1

/* Very specific URL-composing info needed mostly while coding and never else */
#define DEBUG_URL_AUX	0

/* Show XMLs particles during XML-parsing */
#define DEBUG_XML	0

/* Very specific XML-paring info needed mostly while coding and never else */
#define DEBUG_XML_AUX	0

/* Show vocabulary generation and usage */
#define DEBUG_VOC	0

/* Show workflow of function methods */
#define DEBUG_FUNC	1



/* Display common debug info */
#if (DEBUG_COMMON)
	#define DCOMMON(format, ...) fprintf (stdout, format, __VA_ARGS__)
#else
	#define DCOMMON(format, ...)
#endif /* (DEBUG_COMMON) */


/* Display debug URL */
#if (DEBUG_URL)
	#define DURL(format, ...) fprintf (stdout, format, __VA_ARGS__)
#else
	#define DURL(format, ...) 
#endif /* (DEBUG_URL) */

/* Very specific URL-composing info */
#if (DEBUG_URL_AUX)
	#define DURLAUX(format, ...) fprintf (stdout, format, __VA_ARGS__)
#else
	#define DURLAUX(format, ...)
#endif /* (DEBUG_URL_AUX) */

/* Debug XML-parsing and related info */
#if (DEBUG_XML)
	#define DXML(format, ...) fprintf (stdout, format, __VA_ARGS__)
#else
	#define DXML(format, ...)
#endif /* (DEBUG_XML) */

/* XML-parsing info with merkyp for */
#if (DEBUG_XML_AUX)
	#define DXMLAUX(format, ...) fprintf (stdout, format, __VA_ARGS__)
#else
	#define DXMLAUX(format, ...)
#endif /* (DEBUG_XML_AUX) */

/* Show vocabulary generation and usage */
#if (DEBUG_VOC)
	#define DVOC(format, ...) fprintf (stdout, format, __VA_ARGS__)
#else
	#define DVOC(format, ...)
#endif /* (DEBUG_VOC) */

/* Show workflow of function methods */
#if (DEBUG_FUNC)
	#define DFUNC(format, ...) fprintf (stdout, format, __VA_ARGS__)
#else
	#define DFUNC(format, ...)
#endif /* (DEBUG_FUNC_VOC) */




/* Report operation execution was finished depending on its status */
#define VERBOSE_STATUS(x) \
	{int iErr; if (INJ_SUCCESS == ( iErr =  x() ) ) { printf("operation %d is DONE!\n",  iOperation ); } else { /* verbose an error */  printf("operation WAS NOT %d done.\n",  iOperation ); } }

/* Report generalized operation of Type #1 (Meta1) execution was finished depending on its status */
#define VERBOSE_STATUS1(x,y) \
	{int iErr; if (INJ_SUCCESS == ( iErr =  x(y) ) ) { printf("Type #1 operation %d is DONE!\n",  iOperation ); } else { /* verbose an error */  printf("Type #1 operation WAS NOT %d done.\n",  iOperation ); } }

/* Report generalized operation of Type #2 (Meta2) execution was finished depending on its status */
#define VERBOSE_STATUS2(x,y,z) \
	{int iErr; if (INJ_SUCCESS == ( iErr =  x(y,z) ) ) { printf("Type #2 operation %d is DONE!\n",  iOperation ); } else { /* verbose an error */  printf("Type #2 operation WAS NOT %d done.\n",  iOperation ); } }


#endif /* _VERBOSE_H_ */
