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

#ifndef _XMLS_H_
#define _XMLS_H_

#define parse_xml_cast(x,y) _parse_xml_cast(__func__, (x), (y))

/* Disclose type <pUrlChainType> */
#include "lists.h"

/* (Make in visible to others) Pointer to single-binded URL list for current command */
extern pUrlChainType  pUrlChain;

/* (Make in visible to others) Pointer to auxiliary data for current XML file */
extern pXmlAuxType pAuxiliary;

#endif /* _XMLS_H_ */
