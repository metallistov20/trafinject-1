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
#ifndef _OPCODES_H_
#define _OPCODES_H_


/* Opcode definition for operation 'Open Site' */
#define DO_OPEN_OP	0

/* Opcode definition for operation 'Close Site' */
#define DO_CLOSE_OP	1

/* Opcode definition for operation 'Create SANMP group, Enable SNMP srv, and Save' */
#define DO_CREATE_OP	2

/* Opcode definition for operation 'Save' */
#define DO_SAVE_OP	3

/* Opcode definition for operation 'Create ACL group, Set Up ACL, and Save' */
#define DO_ACL_OP	4

/* Opcode definition for operation 'Upload dirmware, and Write it' */
#define DO_FIRMWARE_OP	5

/* Opcode definition for switch reboot operation */
#define DO_REBOOT_OP	6

/* Opcode definition for manual assigning an IP to switch */
#define DO_IPSET_OP	7



/* Opcode definition for no operaiotn */
#define DO_NO_OP	(-1)


#endif /* _OPCODES_H_ */
