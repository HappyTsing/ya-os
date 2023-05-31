cd os3_module
make
sudo insmod os3_module.ko
make clean
cd ../call_os3_module
make
sudo insmod call_os3_module_add_time.ko
make clean
sudo rmmod call_os3_module_add_time # 必须先删除call_os3_module_add_time，再删除os3_module，否则是无法删除的，会有报错
sudo rmmod os3_module
dmesg