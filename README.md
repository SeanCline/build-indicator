#Overview#
This project polls a Jenkins API url periodically and drives the Raspberry Pi's GPIO pins to indicate the build status. The intention is that the the GPIO pins will be used in conjunction with solid state relays or power mosfets to drive a drive a bright light to indicate the current build status. This is particularly useful for a team to monitor the build status during continuous integration as described here: [http://en.wikipedia.org/wiki/Build_light_indicator](http://en.wikipedia.org/wiki/Build_light_indicator).

#Installation#
These instructions assume a fresh installation of [Raspbian](http://www.raspbian.org/).

**Note:** This project was developed with GCC 4.8. There is a good chance it will work properly on on 4.7, but if not, you may want to follow the steps in *Upgrading to Raspbian Jessie*.

##Upgrading to Raspbian Jessie##
    sudo apt-get update
    sudo apt-get upgrade
    sudo sed -i 's/wheezy/jessie/' "/etc/apt/sources.list"
    sudo sed -i 's/wheezy/jessie/' "/etc/apt/sources.list.d/collabora.list"
    sudo sed -i 's/wheezy/jessie/' "/etc/apt/sources.list.d/raspi.list"
    sudo apt-get update
    sudo apt-get dist-upgrade

##Install dependencies##
    sudo apt-get install build-essential git libboost-program-options-dev libcurl4-openssl-dev libcurlpp-dev

##Clone, build, and install [WiringPi](http://wiringpi.com/)##
    cd ~
    git clone git://git.drogon.net/wiringPi
    cd wiringPi
    ./build

#Clone and build build-indicator##
    cd ~
    git clone https://github.com/SeanCline/build-indicator.git
	 cd build-indicator
	 make

#Usage#

##Instructions to run build-indicator##

    Commandline parameters:
      --status-uri <arg>                          The Jenkins '/api/json' url to periodically extract build status from.
      -t [ --polling-period ] <arg> (Default: 30) Seconds between status updates from the Jenkins server.
      -r [ --red-pin ] <arg>        (Default: 8)  GPIO pin number for red light.
      -y [ --yellow-pin ] <arg>     (Default: 9)  GPIO pin number for yellow light.
      -g [ --green-pin ] <arg>      (Default: 7)  GPIO pin number for green light.

For example, the command `./build-indicator --status-uri="https://builds.apache.org/job/Thrift/lastBuild/api/json"` would start monitoring the Apache Thrift project every 30 seconds and reporting the status of its most recent build on the default pin setup.

Consult the [WiringPi documentation](http://wiringpi.com/pins/) for pin numbers.
