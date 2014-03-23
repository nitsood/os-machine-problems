make
if [ $? -gt 0 ]
then
  exit 1
fi
sudo mount -o loop dev_kernel_grub.img /mnt/os_mp2
sudo cp kernel.bin /mnt/os_mp2
echo "\nkernel deployed"
echo "disk unmounting.."
sleep 2
sudo umount /mnt/os_mp2
echo "done"
echo "bochs started"
bochs -qf bochsrc.bxrc
