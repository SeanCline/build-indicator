#pragma once

#include "FadePin.h"

#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>

class SignalTower
{
public: // Enums.
	enum class LightState { solid, pulsing };

public: // Construction.
	SignalTower(int redPinNumber, int yellowPinNumber, int greenPinNumber,
	            std::chrono::milliseconds fadeTime = std::chrono::milliseconds(1000));
	~SignalTower();
	
public: // Light stuff.
	void setRedState(LightState, int brightness = 0, bool fade = true);
	void setYellowState(LightState, int brightness = 0, bool fade = true);
	void setGreenState(LightState, int brightness = 0, bool fade = true);
	void turnAllOff(bool fade = true);
	void runSelfTest(); // Fades in Red, then Yellow, then Green. So you know to swap any cables.
	
private: // Worker thread function.
	void runUpdateLoop();

private: // Data.
	LightState redState_    = LightState::solid;
	LightState yellowState_ = LightState::solid;
	LightState greenState_  = LightState::solid;
	
	FadePin redPin_, yellowPin_, greenPin_;

private: // Worker thread.
	std::thread updateThread_;
	std::atomic<bool> exitThreadFlag_;
	std::chrono::milliseconds fadeTime_;
	std::mutex mutex_;
};
