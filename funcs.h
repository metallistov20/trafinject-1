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

#ifndef _FUNCS_H_
#define _FUNCS_H_

/* Open secure HTTP connection and force CURL to produce session ID at stdout */
int iOpenSite();

/* Close secure HTTP connection */
int iCloseSite();

/* Create SNMP group, needed to <snmpset/snmpget> called from <enroll.sh> */
int iCreateSnmp();

/* Save changes recently done on switch */
int iSaveSite();

/* Upload FW and flash it into switch (then switch reboots) */
int iUpgradeFirmware();

/* Create ACL group (demended by ISP) */
int iAclGroup();

/* Reboot switch */
int iRebootSwitch();

/* Assign static IPv4 address with netmask */
int iAssignIp();

/* ; see TODO in inplementation */
int iBindMacIp();

/* Enable ededicated port of switch; see TODO in inplementation */
int iEnablePort();

/* Perform typical operation cycle; parameter is name of XML-node describing URLs of this operation */
int iMeta1(char *);

/* Perform atypical operation cycle; parameter #1 is XML-node-name of 'direct' flow of URLs; parameter #2 is XML-node-name of backdraft URLs. */
int iMeta2(char *, char *);

#endif /* _FUNCS_H_ */
