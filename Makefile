CXX = g++

# Colourized output. Call with: $(call print, "z0mg t3h culurz!")
define print
      @tput setaf 6
      @echo $1
      @tput sgr0
endef

# Project information.
SOURCES = $(wildcard *.cpp) $(wildcard PwmReporter/*.cpp) $(wildcard UnicornHatReporter/*.cpp) $(wildcard ConsoleReporter/*.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = build-indicator

# 3rd Party Libraries
GIF2UNICORNPATH = ./UnicornHatReporter/Gif2UnicornHat
WIRINGPIPATH = ./PwmReporter/wiringPi/wiringPi
WS2812PATH = $(GIF2UNICORNPATH)/UnicornHat/library/legacy/rpi-ws281x/lib/

LDFLAGS += -lcurl -lcurlpp -lgif -lpthread -lboost_program_options -L$(GIF2UNICORNPATH) -lGif2UnicornHat -L$(WS2812PATH) -lws2811 -L$(WIRINGPIPATH) -lwiringPi
INCLUDES += -I./ -I$(GIF2UNICORNPATH) -I$(WIRINGPIPATH)

# Build Flags
OPTIMIZATION_LEVEL = -O0 -g -ggdb
WARNINGS = -Wall -Wextra -Wcast-align -Wcast-qual -Wconversion -Wformat=2 -Winit-self -Winvalid-pch -Wmissing-format-attribute -Wmissing-include-dirs -Wpacked -Wredundant-decls -Wunreachable-code
STRICTNESS = -pedantic
CXXFLAGS = -std=c++1y $(OPTIMIZATION_LEVEL) $(WARNINGS) $(STRICTNESS)

# Targets
all: $(SOURCES) $(EXECUTABLE)

dependencies:
	@make -C $(WIRINGPIPATH) static
	@make -C $(GIF2UNICORNPATH) dependencies
	@make -C $(GIF2UNICORNPATH)

cleandeps:
	@make -C $(WIRINGPIPATH) clean
	@make -C $(GIF2UNICORNPATH) clean
	
.cpp.o:
	$(call print, "Compiling $< into $@")
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(EXECUTABLE): $(OBJECTS)
	$(call print, "Linking $(OBJECTS) into $@")
	@$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

clean:
	$(call print, "Cleaning...")
	@rm -f $(OBJECTS) $(EXECUTABLE)
	$(call print, "Done cleaning.")
