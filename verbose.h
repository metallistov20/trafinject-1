/*
 Tested switches: TL-SL2428, TL-SL2218, TL-SL5428E

 (C) Copyright 2015, TP-Link Inc, konstantin.mauch@tp-link.com

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License as
 published by the Free Software Foundation; either version 2 of
 the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT cAby WARRANTY; without even the implied warranty of
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


/* DCOMMON macro */
#if (DEBUG_COMMON)
	#define DCOMMON(format, ...) fprintf (stdout, format, __VA_ARGS__)
#else
	#define DCOMMON(x) 
#endif /* (DEBUG_COMMON) */


/* DURL macro */
#if (DEBUG_URL)
	#define DURL(format, ...) fprintf (stdout, format, __VA_ARGS__)
#else
	#define DURL(x)
#endif /* (DURL) */

#define VERBOSE_STATUS(x) \
	{int iErr; if (INJ_SUCCESS == ( iErr =  x() ) ) { printf("operation %d is DONE!\n",  iOperation ); } else { /* verbose an error */  printf("operation WAS NOT %d done.\n",  iOperation ); } }


#endif /* _VERBOSE_H_ */