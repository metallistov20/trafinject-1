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
#endif /* (DURL) */

/* Very specific URL-composing info */
#if (DEBUG_URL_AUX)
	#define DURLAUX(format, ...) fprintf (stdout, format, __VA_ARGS__)
#else
	#define DURLAUX(format, ...)
#endif /* (DXML) */

/* Debug XML-parsing and related info */
#if (DEBUG_XML)
	#define DXML(format, ...) fprintf (stdout, format, __VA_ARGS__)
#else
	#define DXML(format, ...)
#endif /* (DXML) */

/* XML-parsing info with merkyp for */
#if (DEBUG_XML_AUX)
	#define DXMLAUX(format, ...) fprintf (stdout, format, __VA_ARGS__)
#else
	#define DXMLAUX(format, ...)
#endif /* (DXMLAUX) */

/* Show vocabulary generation and usage */
#if (DEBUG_VOC)
	#define DVOC(format, ...) fprintf (stdout, format, __VA_ARGS__)
#else
	#define DVOC(format, ...)
#endif /* (DXML) */




/* Report operation execution was finished depending on its status */
#define VERBOSE_STATUS(x) \
	{int iErr; if (INJ_SUCCESS == ( iErr =  x() ) ) { printf("operation %d is DONE!\n",  iOperation ); } else { /* verbose an error */  printf("operation WAS NOT %d done.\n",  iOperation ); } }


#endif /* _VERBOSE_H_ */
