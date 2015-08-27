#!/bin/bash
#
# Script for automatical writing S/N from table 'dev-mac-id.base.txt'
# into network switches TL-SLxxxx.
#
# 1. Ensure you've done 'apt-get install snmp' on your host.
# 3. Ensure you have no MIB databases in /usr/share/snmp, /root/snmp,
#    etc - current script works with not translated IODs.
# 3. Ensure the file 'dev-mac-id.base.txt' is stored current directly.
# ---------------------------------------------------------------------
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

declare -a rawNAME

declare -a rawMAC

declare -a rawENTRY


	IP="$(. ./functions.sh && CheckIp "$1")"

	MODEL="$(. ./functions.sh && CheckModel "$2")"

	SNMP_GRP="$(. ./functions.sh && CheckSnmp "$3")"


	echo "Paramaters passed to <$0> are:";
		echo "IP <$IP>";
		echo "MODEL <$MODEL>";
		echo "SNMP_GRP <$SNMP_GRP>";


	awNAME='snmpget -v1 -c'$SNMP_GRP' '$IP' iso.3.6.1.2.1.1.6.0'
	rawNAME=($($awNAME))

	awMAC='snmpget -v1 -c'$SNMP_GRP' '$IP' iso.3.6.1.2.1.17.1.1.0'
	rawMAC=($($awMAC))

	cnt=0

	for i in ${rawNAME[@]}
	do
		cnt=$((cnt+1))

		if [ $cnt -gt 3 ]; then

			NAME=$NAME$i
		fi	
	done


	cnt=0

	for i in ${rawMAC[@]}
	do
	   cnt=$((cnt+1))

	   # TODO: check parenthesis
	   if [ $cnt -gt 3 ]; then

		MAC=$MAC$i

		# TODO: check parenthesis
		if [ 9 -gt $cnt ]; then

			MAC=$MAC":"
		fi
	   fi	
	done

	REALNAME=$(echo $NAME | sed 's/\"//g')

	echo "Switch location found: $REALNAME"
	echo "Switch MAC found: $MAC"

	# MAC and REALNAME are ready. Let's walk through <dev-mac-id.base.txt> and find correspondencies. Once found - let's act

	cnt=0

	while read line; do

	  rawENTRY=($(echo $line));

	  NEWMAC=$MAC

	  for i in ${rawENTRY[@]}
	  do
	  	#echo $i
		j=${rawENTRY[2]}
		if [[ $i == $MAC ]]; then

		   # this is to skip 'already processed' cases
		   if [[ ! $REALNAME =~ $j ]]; then		
	  		echo "${j} is not in ${REALNAME}. Let's write it this S/N into switch... "

			# TODO: check parenthesis
			if  [ $MODEL == "2218" ] || [ $MODEL == "2428" ]
			then
				# the MAC is present in table: concatenate REALNAME and S/N, and write it back into switch
				ULTINAMENAME=$REALNAME
				ULTINAMENAME=$ULTINAMENAME\($j\)
			# TODO: check parenthesis
			elif  [ $MODEL == "5428E" ]
			then

				# the MAC is present in table: write S/N into switch
				ULTINAMENAME=$j

			else
				echo "Incorrect switch name. Nothing to do here"
				exit -1
			fi

			echo Now we are setting this string as new Serial Number: $ULTINAMENAME

			snmpset -v1 -c$SNMP_GRP 192.168.0.1 iso.3.6.1.2.1.1.6.0 s $ULTINAMENAME

		   else		
		  	echo "${j} was found in ${REALNAME}. Nothing to do here"
		   fi

		fi
	
	  done

	done < ./dev-mac-id.base.txt

exit 0

