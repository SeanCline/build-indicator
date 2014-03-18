#pragma once

class FadePin {
public: // Construction.
	FadePin(int pinNum);

public: // Member functions.
	void tick(); //< Nudge the actual value one closer to the actual value.
	
	int getDesiredValue();
	void setDesiredValue(int value);
	
	int getActualValue();
	void setActualValue(int value);
	
private: // Data.
	int pinNumber_;
	int desiredValue_, actualValue_;
};
