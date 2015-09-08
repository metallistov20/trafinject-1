echo "1. Remote reboot of the switch. (Supressing STDOUT output is obvious.)"
./inject.sh 192.168.0.1 reboot 5428E >./_reboot.log
echo "1. Reboot command issued"
