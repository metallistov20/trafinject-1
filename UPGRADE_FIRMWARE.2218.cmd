echo "1. Upgrading FW on the switch. (Supressing STDOUT output is obvious.)"
./inject.sh 192.168.0.1 upgrade 2218 TL-SL2218v1_underTest.bin >./_upgrade.log
echo "1. Upgraded in case was different"

