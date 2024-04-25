disk_name="$1"
dd if=/dev/zero of="$disk_name" bs=1G count=5
