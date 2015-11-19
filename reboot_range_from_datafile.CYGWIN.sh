#! ./sh.exe
#
# Script to reboot a set of network switches which addresses are
# stored in separate text file, name of file passed via cmdline.
#
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

export PATH=./:$PATH

# CygWIN's executable started from command line requires this 
mkdir ./tmp

# Time (seconds) to wait for remote switch to respond before recognizing it as non-responding. INTEGER
sec2wait=2

while IFS='' read -r line || [[ -n "$line" ]]; do

	echo "Ip address obtained datafile: $line"

	# No check for consistency by now
	ip=$line

	# File to store results for given target (IP address)
	LOG_PATH="log_for_$ip.TXT"

	# Let's check it the target is onlie
  	if ping -n 1 -w $sec2wait $ip &> /dev/null > $LOG_PATH

	# if switch appears be ONLINE
	then
		# Comment 
		echo "Rebooting the switch at $ip remotely. (Supressing STDOUT output is obvious.)" | tee -a ${LOG_PATH}

		# Do actual reboot of the target ( i.g. 1) open session; 2) issue reboot command ; 3) close session 
		./inject.CYGWIN.sh $ip reboot 5428E >> ${LOG_PATH}

		# Cmt. #1
		echo "Reboot command issued to switch $ip." | tee -a ${LOG_PATH}

	# if switch appears be OFFLINE
	else
		# Do nothing, only verbose
		echo "Switch at address ${ip} is down." | tee -a ${LOG_PATH}
	fi

done < "$1"

exit

rmdir ./tmp
