ZCU104 Ubuntu Boot Instructions
-----------------------------------------------------------------------------------------
Board name : Zynq UltraScale+ MPSoC ZCU104
Part number : xczuc7ev-2ffvc1156
Tools : Petalinux 2023.2 #you can use other version.
Ubuntu : 20.04 // you can use other version make sure it comaptible with petalinux
*******************************************************************************************************************
Follow the following steps to create images and bin files required to boot board
-------------------------------------------------------------------------------------------------------------------
1 . First clone this repository from this github.
2 . To create images we are going to use the petalinux tool version 2023.2 . So install this tool
from xilinx website.
      https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/embedded-design-tools.html
** User guide you can follow for installation procedure of petalinux :
      https://docs.amd.com/r/en-US/ug1144-petalinux-tools-reference-guide
3 . Download BSP for zcu104
      https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/embedded-design-tools.html
      
4 . Go into the directory where you installed Petalinux.
5 . Run this command to set up the environment variables and configurations required for
Petalinux to function correctly.
      source setting.sh
6 . Run this command to create a new Petalinux project with specific configurations, based on a
provided Board Support Package (BSP) template for the Zynq UltraScale+ MPSoC platform
(zynqMP).
      petalinux-create --force --type project --template zynqMP --source /path to / BSP file --name <projname>
7 . Go into the project directory
cd <proj name>
8 . Run this command to retrieve hardware description files(XSA files) from the specified
directory for use in configuring a Petalinux project.
      petalinux-config --get-hw-description / path to repository clone/ fpga/implement/results/
      
After running above commands one prompt will appear where you need to make these
following changes
* Yocto Settings -> Add pre-mirror url -> change http: to https; change rel-v${PETALINUX_MAJOR_VER}
to rel-v2023.2
* Yocto Settings -> Network State Feeds url -> change http: to https; change
rel-v${PETALINUX_MAJOR_VER} to rel-v2023.2
* Image Packaging Configuration -> EXT4 (if you want the rootfs on the sd card)
* save and exit

9 . Run this command to clean and rebuild the bootloader component of your project.
      petalinux-build -c bootloader -x distclean
10 . Run this command to configure the Linux kernel of a Petalinux project in a silent or
non-interactive mode
      petalinux-config --silentconfig -c kernel
11 .Run this command to build the images .
      petalinux-build
** At this point you may get a number of errors. Just run the previous three commands again.
That usually fixes the errors.
12 . Run this command to create BOOT.BIN file
     petalinux-package --force --boot --u-boot --kernel --offset 0xF40000 --fpga /path to repository clone/fpga/implement/results/top.bit

13 . Copy the boot files to the SD card because we will boot board with the SD card.
** It is assumed that you already partitioned the SD card.
- sudo gparted (make sure you have the correct drive selected!)
- First partition called BOOT, FAT32, 512MB
- Second partition called rootfs, ext4, uses the rest of the card.
cp images/linux/BOOT.BIN /path to sd card/BOOT/
cp images/linux/image.ub /path to sd card/BOOT/
cp images/linux/boot.scr /path to sd card/BOOT/

## Root Filesystem
cd ..
Run these command:
wget
https://releases.linaro.org/debian/images/developer-arm64/latest/linaro-stretch-developer-20180416-89.tar.gz
sudo tar --preserve-permissions -zxvf linaro-stretch-developer-20180416-89.tar.gz
sudo cp --recursive --preserve binary/* /path to sd card /rootfs/ ; sync
14. Hardware setup
1.Connect with power supply of 12 volt
2.SW6 switch setting 1= On remaining off for sd card boot.
3.Connect ethernet cable
4.Connect JTAG cable to board to access the console of board (using minicom / gtkterm /
putty /cutecom)
5.Insert sd card into board
6.Power on board
7.You will see output logs of booting in the serial console .

*********************************************************************************************************************************************
