#Overview#
This project runs on a [Raspberry Pi](https://en.wikipedia.org/wiki/Raspberry_Pi) and polls a Jenkins API URI periodically, displaying the last build status for a software development team to monitor the [continuous integration](https://en.wikipedia.org/wiki/Continuous_integration) status. This practice is described here: [http://en.wikipedia.org/wiki/Build_light_indicator](http://en.wikipedia.org/wiki/Build_light_indicator).

There are (currently) 2 ways to display the build status:

1. By displaying it on a [Pimoroni UnicornHat](http://shop.pimoroni.com/products/unicorn-hat). This is the by far the most plug-and-play solution.
2. By driving PWM GPIO pins which are connected to lights via solid state relays or power MOSFETs.

![Image of build-indicator running on a UnicornHat](docs/UnicornHatPhoto.jpg)

If this sounds interesting, continue on to [Installation and Usage](docs/InstallationAndUsage.md)
