
echo "1. Creating SNMP on the switch. (Supressing STDOUT output is obvious.)"
./inject.sh 192.168.0.1 create 2428 SNMP_COM8 >./_create.log
echo "1. Created or found existing"

echo "2. Writing S/N into switch"
./enroll.sh 192.168.0.1 2428 SNMP_COM8
echo "2. Writen (or left intact)"

echo "3. Saving altered S/N in the switch. (Supressing STDOUT output is obvious.)"
./inject.sh 192.168.0.1 save 2428 >./_save.log
echo "3. Saved"
