# Installation #
These instructions assume a fresh installation of [Raspbian Stretch](https://www.raspberrypi.org/downloads/raspbian/).

## Install dependencies ##
	sudo apt-get install build-essential git libboost-program-options-dev libcurl4-openssl-dev libcurlpp-dev libgif-dev

## Clone and build ##
	cd ~
	git clone --recursive https://github.com/SeanCline/build-indicator.git
	cd build-indicator
	make dependencies && make

# Usage #

## Example usage ##

If you wanted to report the build status of the Apache Thrift project on a [UnicornHat](http://shop.pimoroni.com/products/unicorn-hat), you would run:
	
	sudo ./build-indicator --reporter=UnicornHat --status-uri="https://builds.apache.org/job/Thrift/api/json"

If you, instead, wanted to report the build status, on GPIO pins `7`, `8`, and `9`, you would change the `reporter` like so:

	sudo ./build-indicator --reporter=PWM --status-uri="https://builds.apache.org/job/Thrift/api/json"

For more advanced usage, consult the command line parameters.

## Command Line Parameters ##

### General Parameters ###

	--reporter <arg>     The module that will be used to report build status. (e.g. PWM, UnicornHat)
	--status-uri <arg>   The Jenkins '/api/json' url to periodically extract build status from.
	-t [ --polling-period ] <arg> (Default: 30) Seconds between status updates from the Jenkins server.

### PWM Reporter Parameters ###

	-r [ --red-pin ] <arg>     (Default: 8)   GPIO pin number for red light.
	-y [ --yellow-pin ] <arg>  (Default: 9)   GPIO pin number for yellow light.
	-g [ --green-pin ] <arg>   (Default: 7)   GPIO pin number for green light.
	-p [ --fade-period ] <arg> (Default: 500) Millisecond period for pulsing LEDs to blink.

Consult the [WiringPi documentation](http://wiringpi.com/pins/) for GPIO pin numbers.
	
### UnicornHat Reporter Parameters ###

	--brightness <arg>   (Default: 0.2)            Unicorn Hat brightness (0 to 1)
	--boot-gif <arg>     (Default: boot.gif)       GIF to show when starting up.
	--success-gif <arg>  (Default: successful.gif) GIF to show when build succeeded.
	--failed-gif <arg>   (Default: failed.gif)     GIF to show when build failed.
	--building-gif <arg> (Default: building.gif)   GIF to show when currently building.
	--unknown-gif <arg>  (Default: unknown.gif)    GIF to show when build status is unknown.
