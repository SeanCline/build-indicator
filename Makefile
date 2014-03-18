CXX = g++

# Colourized output. Call with: $(call print, "z0mg t3h culurz!")
define print
      @tput setaf 6
      @echo $1
      @tput sgr0
endef

# Project information.
SOURCES = main.cpp BuildStatus.cpp SignalTower.cpp FadePin.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = build-indicator
PCH = pch.h

# Build Flags
OPTIMIZATION_LEVEL = -O0 -g -ggdb
WARNINGS = -Wall -Wextra -Wcast-align -Wcast-qual -Wconversion -Wformat=2 -Winit-self -Winvalid-pch -Wmissing-format-attribute -Wmissing-include-dirs -Wpacked -Wredundant-decls -Wunreachable-code
STRICTNESS = -pedantic
CXXFLAGS = -std=c++11 $(OPTIMIZATION_LEVEL) $(WARNINGS) $(STRICTNESS)

# 3rd Party Libraries
LDFLAGS += -lcurl -lcurlpp -lwiringPi -lpthread -lboost_program_options
#INCLUDES += 

# Targets
all: $(SOURCES) $(EXECUTABLE)

$(PCH).gch: $(PCH)
	$(call print, "Building precompiled header $< into $@")
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $<
	
$(OBJECTS): $(PCH).gch
.cpp.o:
	$(call print, "Building $< into $@")
	$(CXX) $(CXXFLAGS) $(INCLUDES) -include $(PCH) -c $< -o $@

$(EXECUTABLE): $(OBJECTS)
	$(call print, "Linking $(OBJECTS) into $@")
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@

clean:
	$(call print, "Cleaning...")
	rm -f $(OBJECTS)
	rm -f $(EXECUTABLE)
	rm -f $(PCH).gch
	$(call print, "Done cleaning.")
