echo "1. Creating ACL group"
./inject.sh 192.168.0.1 acl 5428E 97 >./_acl.log
echo "1. ACL group creation command issued"

echo "2. Saving created ACL in the switch. (Supressing STDOUT output is obvious.)"
./inject.sh 192.168.0.1 save 5428E >./_save.log
echo "2. Saved"
