disk_name="$1"

if [ -f "$disk_name" ]; then
    rm "$disk_name"
fi

dd if=/dev/zero of="$disk_name" bs=1G count=2
