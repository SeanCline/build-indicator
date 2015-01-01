#Installing Raspbian Jessie*

##Fresh Install (Recommended)##

Follow the instructions to install raspbian vis the [raspbian-ua-netinst](https://github.com/debian-pi/raspbian-ua-netinst) project. After preparing the SD card, create an `installer-config.txt` on the root of the SD card with the line `release=jessie` as explained under [Installer Customization](https://github.com/debian-pi/raspbian-ua-netinst#installer-customization).

Place the SD card in the Pi with a working internet connection and wait for it to finish installing. After installation is finished, you will probably want to setup a `pi` user as is done in the official Raspbian images.

As root:

	apt-get update
	apt-get install sudo
	adduser pi sudo


##Upgrading from Wheezy##

Before doing a major upgrade like this, it is wise to [backup the SD card](http://raspberrypi.stackexchange.com/a/313/3594).

	sudo apt-get update
	sudo apt-get upgrade
	sudo sed -i 's/wheezy/jessie/' "/etc/apt/sources.list"
	sudo sed -i 's/wheezy/jessie/' "/etc/apt/sources.list.d/collabora.list"
	sudo sed -i 's/wheezy/jessie/' "/etc/apt/sources.list.d/raspi.list"
	sudo apt-get update
	sudo apt-get dist-upgrade

(**Note:** Since the change to `systemd`, I've had mixed results when upgrading from wheezy to jessie. I'd highly recommend a fresh install using )