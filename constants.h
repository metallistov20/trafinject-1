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

#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_


/* Operation done successfully */
#define INJ_SUCCESS	0

/* Operation is not yet impelmented */
#define INJ_NOT_IMPL	(-8)



/* Assuming that 1K shoudl be enought for Barracuda */
#define MAX_URL_SIZE 	1024

/* 12 digits + 3 dots */
#define MAX_IP_SIZE 	15

/* Depends on switch SW, Assuming that 110 is enough */
#define MAX_SNMP_SIZE 	110

/* Depends on switch SW, Assuming that 64 is enough */
#define MAX_TID_SIZE 	64

/* Auxiliary strings, those not library- or Barracuda-dependant */
#define MAX_STR_SIZE 64


#endif /* _CONSTANTS_H_ */
