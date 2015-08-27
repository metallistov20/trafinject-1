
LIBRARY NAME:		trafinject v.1.0

LIBRARY PURPOSE:	L7 traffic injections to automate remote setting of L2/L3 network hardware.


Abstract:


Current library implements hypertext traffic injections for automation of remote setting on network-switching and -routing hardware.
This library was tested on TL-SL2218, TL-SL2428, TL-SL5428E, and most of below operations have successfully passed the testing.
Library is still under development and its up-to-date revision is permanently stored at  https://github.com/metallistov20/trafinject/  . Feel free to deploy under your tasks and to contribute.


Short description of functions:


int iOpenSite( )

Function int iOpenSite( ) performs ‘opening’ of web-interface of switch. I.e. simulates passing authentication data (username and password) to a authentication form (applet) and receives response with session id ( tID hexadecimal number needed for further L7 requests).

Two-step operation. In first message it does referencing to <http://<TARGET_IP>/logon/LogonRpm.htm> with POST payload containing user authentication credentials, on second actually opens the site – references page <http://<TARGET_IP>/> with POST payload containing user ID and level, on this the HTTPS packet with response arrives to STDOUT of shell. This response contains session ID, which is 32-digit hexadecimal, and which is valid for all other operations till current session is closed by iCloseSite operation.

int iCloseSite( )

Function int iCloseSite( ) simulates clicking onto ‘Logout’ button (menu entry) in web-interface of switch.

Two-step operation. In first message it performs referencing to <http://<TARGET_IP>/userRpm/Logout.htm> without payload, and in second – to <http://<TARGET_IP>/>. On this the finalizing HTTPS response arrives to STDOUT of shell. After this the session ID created on iOpenSite and used by other functions during previous steps is not valid anymore, and dynamically generated HTM-pages are destroyed ( not available for referencing ).

int iCreateSnmp( )

Function int iCreateSnmp( ) creates SNMP group with read-write access rights with given name. Such group is necessary if once wants to access SNMP strings of switch via SNMP protocol.

Five-step operation. In first message it references to <http://<TARGET_IP>> with no extra payload, in second – to <http://<TARGET_IP>/userRpm/SNMPv3CommunityConfigRpm.htm>, in third – to <http://<TARGET_IP>/userRpm/SNMPv3CommunityConfigRpm.htm/?s_userlevel=1&_tid_=<tID>>, in fourth – to to <http://<TARGET_IP>/userRpm/SNMPv3CommunityConfigRpm.htm/?txt_comname=<SNMP_GROUP>&comRight=2&comView=0&button=Add&_tid_=<tID>>, in fifth - <http://<TARGET_IP>/userRpm/SNMPv3GlobalConfigRpm.htm?snmpState=1&button=stateSubmit&_tid_=<tID>> .

int iSaveSite( )

Function int iSaveSite( ) simulates clicking ‘Save Config’ button (menu entry) in web-interface of switch.

Four-step operation. In first message it references to <http://<TARGET_IP>> with no extra payload, in second – to  <http://<TARGET_IP>/userRpm/ConfigsaveRpm.htm?s_userlevel=1&_tid_=<tID>&_tid_=<tID>>, in third – to <http://<TARGET_IP>/userRpm/ConfigsaveImg.htm>, and in fourth – to <http://<TARGET_IP>/userRpm/ConfigsaveFileRpm.htm?_tid_=<tID>>.

int iUpgradeFirmware( )

Function int iUpgradeFirmware( ) uploads binary file with firmware image on switch, and initiates firmware writing into flash ( on switch side ) . 

Five-step operation. In first message it references to <http://<TARGET_IP>/userRpm/FirmwareRpm.htm?s_userlevel=1&_tid_=<tID>> , in second – to <http://<TARGET_IP>/help/FirmwareHelpRpm.htm>. TODO: Presumably  this message can be omitted (this presumption is being checked currently). Then the program passes main flow to a shell and form within shell executes two processes: first - < /usr/bin/curl -F filedata=@<LOCAL_FILENAME_OF_FIRMWARE_TO_UPLOAD> http://<TARGET_IP>/  > - on this the necessary fields in http-form get filled;
and second - < /usr/bin/curl --form submit=@<LOCAL_FILENAME_OF_FIRMWARE_TO_UPLOAD> --form submit=upgrade --form _tid_=<tID>  http://<TARGET_IP>/userRpm/FirmwareAdRpm.htm > - on this the form gets submitted. 
Finally, the main flow returns back to a program , and program references fifth page <http://<TARGET_IP>/userRpm/FirmwareUpdateTempRpm.htm?_tid_=<tID>> - on this the server side starts procedure of uploading via HTTP protocol and writing down to a flash the firmware defined as <LOCAL_FILENAME_OF_FIRMWARE_TO_UPLOAD> . Note that after this operation server side gets not responding till flashing down is finished amd server side is rebooted. Once the new firmware contains specific settings (such as different MAC or different IP, etc) then following calls of current program should encounter this.

int iAclGroup( )

Function int iAclGroup( ) creates an ACL group with given ID.

Three-step operation.  In first message it references to <http://<TARGET_IP>> without extra payload, then it references <http://<TARGET_IP>/userRpm/ACLRuleCreateRpm.htm?s_userlevel=1&_tid_=<tID>> page, and finally - <http://<TARGET_IP>/userRpm/ACLRuleCreateRpm.htm?submit=Submit&aclId=<cACL>&ruleOrder=1&_tid_=<tID>>, where  <cACL> is an index of ACL group, which is decimal from range [0 ..299]. in this case the ACL group name is blank.

int iRebootSwitch( )

Function int iRebootSwitch( ) issues request to reboot the switch immediately.

Three-step operation. In first message it references to <http://<TARGET_IP>> without extra payload, in second one – to <http://<TARGET_IP>/userRpm/ReiniRstAdRpm.htm?restore=Reset&_tid_=<tID> > page, on third – to <http://<TARGET_IP>//userRpm/ReiniRstAdTempRpm.htm?_tid_=<tID> > page. This operation reboots the server side as is and immediately, so please  take care to have you recent changes saved, and your further references to a server be issued not earlier than 30-200 seconds (depends on instance of network switch).

int iAssignIp( )

Function int iAssignIp( ) assigns static IP with network mask, and receives confirmation from switch – in response packet the IP-address is already altered. 

Five-step operation. This operation assigns static IP to a server side. IP must be given along with subnet mask, and without DNS server address. Beware that in the middle of procedure the IP get changes so the second half of messages should be “addressed” to a server with already changed target IP. In first message it references to <http://<TARGET_IP>/SystemInfoRpm.htm?s_userlevel=1&_tid_=<tID>&_tid_=<tID>> without extra payload, in second – to <http://<TARGET_IP>/userRpm/SystemIpRpm.htm?s_userlevel=1&_tid_=<tID>>, in third – to <http://<TARGET_IP>/userRpm/SystemIpRpm.htm?ip_mode=0&ip_mgmt_vlanid=1&ip_address=<ADDDRESS_BEING_ASSIGNED>&ip_mask=<MASK_BEING_ASSIGNED>&ip_gateway=&submit=Apply&_tid_=<tID> >. In fourth we need to reference a front page at already changed target IP address, like this: <http://<ADDDRESS_BEING_ASSIGNED>/ >.  TODO: Presumably  this message can be omitted (this presumption is being checked currently). On fifth step – we referencing page at at already changed target IP address but still with same tID, like this: <http://<ADDDRESS_BEING_ASSIGNED>/userRpm/SystemInfoRpm.htm?s_userlevel=1&_tid_=<tID>>. It's clear that after this moment we need to reference any page at already changed target IP address , including “Save Config” and “Close Site” pages. Once parameters saving was done (with “Save Config”) the  changed target IP address will be actual for this server after reboot, and of course during all following sessions.

int iBindMacIp( )

Function int iBindMacIp( ) performs manual binding of given IP and given  MAC address .Four-step operation.This operation provides IP-MAC binding in “Network security” applet. >>>>Being prepared currently<<<<

int iEnablePort( )

Three-step operation to implement port enabling or disabling in “Switching” applet. >>>>Being prepared currently<<<<


Summary:

Library's prospective is: to brecome stable and then to get ported onto L3 network hardware (routers, ADSL modems).
