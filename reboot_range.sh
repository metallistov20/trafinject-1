#! /bin/bash

# Time (seconds) to wait for remote switch to respond before recognizing it as non-responding. INTEGER
sec2wait=2

# Let's read IP-margins from STDIN (alternatively uncomment the 'INSTANCE', below)
echo "Enter beginning IP address:"
read begin
# INSTANCE(see e-mail): begin="192.168.75.1"

# Let's read IP-margins from STDIN (alternatively uncomment the 'INSTANCE', below)
echo "Enter ending IP address:"
read end
# INSTANCE(see e-mail): end="192.168.75.250"

# Constant part of address range. On our case it's a subnet (see e-mail)
subnet=$(echo $begin | sed 's/\.[0-4]*$//')

# Suffix to define first IP address in range 
start=$(echo $begin | sed 's/.*\.//')

# Suffix to define last IP address in range 
stop=$(echo $end | sed 's/.*\.//')

# Go through all addresses in range
while [[ $start -le $stop ]]
do
	# Concatenate constant part and suffix
	ip=$subnet.$start

	# File to store results for given target (IP address)
	LOG_PATH="log_for_$ip.TXT"

	# Let's check it the target is onlie
  	if ping -c 1 -W $sec2wait $ip &> /dev/null > $LOG_PATH
	# if switch appears be ONLINE
	then
		# Comment 
		echo "Rebooting the switch at $ip remotely. (Supressing STDOUT output is obvious.)" | tee -a ${LOG_PATH}

		# Do actual reboot of the target ( i.g. 1) open session; 2) issue reboot command ; 3) close session 
		./inject.sh $ip reboot 5428E >> ${LOG_PATH}

		# Cmt. #1
		echo "Reboot command issued to switch $ip." | tee -a ${LOG_PATH}

	# if switch appears be OFFLINE
	else
		# Do nothing, only verbose
		echo "Switch at address ${ip} is down." | tee -a ${LOG_PATH}
	fi

	# Calculate next address suffix
	((start++))
done
