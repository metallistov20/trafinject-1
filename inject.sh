#!/bin/bash
#
# Script to deploy cURL-based executable 'inject'
#
# 1. Establishes secure session with switch.
# 2. Obtains session_ID(variable $tID) for this session, strictly 
# needed for TL-SL5428E, and not needed for TL-SL2218, TL-SL2428.
# 3. If called as <./inject.sh XXX.XXX.XXX.XXX *create* XXX XXX >
# creates SNMP group, enables SNMP subsystem, and saves these changes.
# 4. If called as <./inject.sh XXX.XXX.XXX.XXX *save* XXX >
# saves changes only.
# 5. If called 
#
# ----------------------------------------------------------------------
# 4. Tested on switches: TL-SL2428, TL-SL2218, TL-SL5428E
# 
# (C) Copyright 2015, TP-Link Inc, konstantin.mauch@tp-link.com
# 
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of
# the License, or (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston,
# MA 02111-1307 USA

# Session Create SaVe (text file with temporary data)
TMP_NAME=tID_etc_CCSV.TXT

tID_STR="var g_tid = "
tID="NotAssigned"
tID_e=$tID


	IP="$(. ./functions.sh && CheckIp "$1")"

	OPERATION="$(. ./functions.sh && CheckOp "$2")"

	MODEL="$(. ./functions.sh && CheckModel "$3")"

	SNMP_GRP="$(. ./functions.sh && CheckSnmp "$4")"

	ACL_GRP="$(. ./functions.sh && CheckAcl "$4")"

	FILENAME="$(. ./functions.sh && CheckFilename "$4")"

	#IP_ADDR="$(. ./functions.sh && CheckIP "$4")" -- sure, we strech it
	#IP_MASK="$(. ./functions.sh && CheckMask "$4")" -- sure, we strech it

	echo "Paramaters passed to <$0> are:";
		echo "IP <$IP>";
		echo "OPERATION <$OPERATION>";
		echo "MODEL <$MODEL>";
		echo "SNMP_GRP <$SNMP_GRP>";
		echo "ACL_GRP <$ACL_GRP>";

	./inject --open --target="$IP">$TMP_NAME

	while read line; do

	 if [[ $line =~ $tID_STR ]]; then

		 tID_complete_ENTRY=($(echo $line));

		        tID_dirty_ENTRY=${tID_complete_ENTRY[3]}

		 tID_exact_ENTRY=${tID_dirty_ENTRY#"'"}

		 tID=${tID_exact_ENTRY%%??}

		 echo "$tID" && break
	 fi

	done < ./$TMP_NAME

	rm -f ./$TMP_NAME


	if [[ $MODEL == "5428E" ]]; then

		if [[ $tID == $tID_e ]]; then

			echo "<$0>: iID is empty. (Though shoul not be on TL-SL5428E)";

			# site was opened. but not tID ofund. closing site, going out .

			./inject --close --target="$IP"

			exit -1
		fi
	fi

	if  [[ $OPERATION == "upgrade" ]]; then

		./inject --upgrade --target="$IP" --id="$tID" --filename=$FILENAME

		# 1. no use to call --save because normally it will reboot right now)

		# 2. but in case smth went wrong (and it is not being rebooted now) way we need close site

		./inject --close --target="$IP"

		exit 0

	elif  [[ $OPERATION == "create" ]]; then

		echo "<$0>: Simulating user manipulations on SNMP Tab in web interface of (TL-SL$MODEL) switch";

		./inject --create --id="$tID" --target="$IP" --community="$SNMP_GRP"

	elif  [[ $OPERATION == "reboot" ]]; then

		echo "<$0>: Performing remote reboot of (TL-SL$MODEL) switch";

		./inject --reboot --id="$tID" --target="$IP" 

		./inject --close --target="$IP"

		exit 0

	elif  [[ $OPERATION == "acl" ]]; then

		echo "<$0>: Doing ACL settings ( --acl-data=$ACL_GRP )";

		./inject --ACL --id="$tID" --target="$IP" --acl-data="$ACL_GRP"

		./inject --close --target="$IP"

		exit 0

	elif  [[ $OPERATION == "static" ]]; then

		echo "<$0>: Assigning static IP address ( --ip-addr="$4" --ip-mask="$5")";

		./inject --ipassign --id="$tID" --target="$IP" --ip-addr="$4" --ip-mask="$5"

		# The adress on switch has already been changed. Beware what you pass as <--target> .
		./inject --close --target="$4"

		exit 0

	elif  [[ $OPERATION == "save" ]]; then

		echo "<$0>: To save changes alegedly done earlier in web interface of (TL-SL$MODEL) switch ...";	

		# will call --save below

	else
		echo "<$0>: strange case, exiting (nothing to do here)"

		# some really odd case- the faster we gat away from here the better. but opened site must be closed before

		./inject --close --target="$IP"

		exit -1
	fi


	echo "<$0>: Simulating click <Save> button in web interface of (TL-SL$MODEL) switch";

	./inject --save --id="$tID" --target="$IP"

	./inject --close --target="$IP"
	
exit 0

