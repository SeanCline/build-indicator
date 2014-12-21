CXX = g++

# Colourized output. Call with: $(call print, "z0mg t3h culurz!")
define print
      @tput setaf 6
      @echo $1
      @tput sgr0
endef

# Project information.
SOURCES = main.cpp BuildStatus.cpp SignalTower.cpp FadePin.cpp ProgramOptions.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = build-indicator

# 3rd Party Libraries
GIF2UNICORNPATH = ./Gif2UnicornHat
LDFLAGS += -lcurl -lcurlpp -lwiringPi -lpthread -lboost_program_options -L$(GIF2UNICORNPATH) -lGif2UnicornHat
INCLUDES += -I$(GIF2UNICORNPATH)

# Build Flags
OPTIMIZATION_LEVEL = -O0 -g -ggdb
WARNINGS = -Wall -Wextra -Wcast-align -Wcast-qual -Wconversion -Wformat=2 -Winit-self -Winvalid-pch -Wmissing-format-attribute -Wmissing-include-dirs -Wpacked -Wredundant-decls -Wunreachable-code
STRICTNESS = -pedantic
CXXFLAGS = -std=c++11 $(OPTIMIZATION_LEVEL) $(WARNINGS) $(STRICTNESS)

# Targets
all: $(SOURCES) $(EXECUTABLE)

$(OBJECTS):
.cpp.o:
	$(call print, "Building $< into $@")
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(EXECUTABLE): $(OBJECTS)
	@make -C $(GIF2UNICORNPATH)
	$(call print, "Linking $(OBJECTS) into $@")
	@$(CXX) $(LDFLAGS) $(OBJECTS) -o $@

clean:
	$(call print, "Cleaning...")
	@rm -f $(OBJECTS) $(EXECUTABLE)
	@make -C $(GIF2UNICORNPATH) clean
	$(call print, "Done cleaning.")
