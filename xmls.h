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

#define print_element_names(x) _print_element_names(__func__, (x))

#define find_named_element(x,y) _find_named_element(__func__, (x), (y))

/* Disclose type <pUrlChainType> */
#include "lists.h"

/* Make in visible from within other files */
extern pUrlChainType  pUrlChain;

#endif /* _XMLS_H_ */
