echo "1. Assigining static IP settings"
./inject.sh 192.168.0.38 static 5428E 192.168.0.40 255.255.255.0 >./_static.log
echo "1. IP address settings issued"

echo "2. Saving changed IP adddress settings. (Supressing STDOUT output is obvious.)"
./inject.sh 192.168.0.40 save 5428E >./_save.log
echo "2. Saved"
