#Overview#
This project runs on a [Raspberry Pi](https://en.wikipedia.org/wiki/Raspberry_Pi) and polls a Jenkins API URI periodically, displaying the last build status for a software development team to monitor the [continuous integration](https://en.wikipedia.org/wiki/Continuous_integration) status. This practice is described here: [http://en.wikipedia.org/wiki/Build_light_indicator](http://en.wikipedia.org/wiki/Build_light_indicator).

There are (currently) 2 ways to display the build status:

1. By displaying it on a [Pimoroni UnicornHat](http://shop.pimoroni.com/products/unicorn-hat). This is the by far the most plug-and-play solution.
2. By driving PWM GPIO pins which are connected to lights via solid state relays or power mosfets.

#Installation#
These instructions assume a fresh installation of [Raspbian](http://www.raspbian.org/).

This project requires GCC 4.9 (or better). I find that the easiest way to get GCC 4.9 is to install or upgrade to Raspbian Jessie. It is possible to pull only the specific packages, or build them oneself, but I will leave that to the reader if it is really necessary. See [Installing Raspbian](InstallingRaspbian.md) for more details.

##Install dependencies##
	sudo apt-get install build-essential git libboost-program-options-dev libcurl4-openssl-dev libcurlpp-dev libgif-dev

##Clone and build build-indicator##
	cd ~
	git clone --recursive https://github.com/SeanCline/build-indicator.git
	cd build-indicator
	make dependencies && make

#Usage#

##Example usage##

If you wanted to report the Apache Thrift CI build status, on GPIO pins `7`, `8`, and `9`, you would do something like:

	sudo ./build-indicator --reporter=PWM --status-uri="https://builds.apache.org/job/Thrift/lastBuild/api/json"
	
If you instead wanted to report the build status on a [UnicornHat](http://shop.pimoroni.com/products/unicorn-hat):
	
	sudo ./build-indicator --reporter=UnicornHat --status-uri="https://builds.apache.org/job/Thrift/lastBuild/api/json"

For more advanced usage, consult the command line parameters.

##Command Line Parameters##

###General Parameters###

	--reporter <arg>     The module that will be used to report build status. (e.g. PWM, UnicornHat)
	--status-uri <arg>   The Jenkins '/api/json' url to periodically extract build status from.
	-t [ --polling-period ] <arg> (Default: 30) Seconds between status updates from the Jenkins server.

###PWM Reporter Parameters###

	-r [ --red-pin ] <arg>     (Default: 8)   GPIO pin number for red light.
	-y [ --yellow-pin ] <arg>  (Default: 9)   GPIO pin number for yellow light.
	-g [ --green-pin ] <arg>   (Default: 7)   GPIO pin number for green light.
	-p [ --fade-period ] <arg> (Default: 500) Millisecond period for pulsing LEDs to blink.

Consult the [WiringPi documentation](http://wiringpi.com/pins/) for GPIO pin numbers.
	
###UnicornHat Reporter Parameters###

	--brightness <arg>   (Default: 0.2)            Unicorn Hat brightness (0 to 1)
	--boot-gif <arg>     (Default: boot.gif)       GIF to show when starting up.
	--success-gif <arg>  (Default: successful.gif) GIF to show when build succeeded.
	--failed-gif <arg>   (Default: failed.gif)     GIF to show when build failed.
	--building-gif <arg> (Default: building.gif)   GIF to show when currently building.
	--unknown-gif <arg>  (Default: unknown.gif)    GIF to show when build status is unknown.
