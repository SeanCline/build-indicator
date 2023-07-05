#pragma once

#include <future>
#include <atomic>

// Forward Declarations.
namespace Gif2UnicornHat {
	class Animation;
	class ImageDisplay;
}

class AsyncAnimationPlayer
{
public: // Construction.
	AsyncAnimationPlayer();
	~AsyncAnimationPlayer();

private: // Non-copyable.
	AsyncAnimationPlayer(const AsyncAnimationPlayer&) = delete;
	AsyncAnimationPlayer& operator=(const AsyncAnimationPlayer&) = delete;

public: // Animation settings.
	void setBrightness(Gif2UnicornHat::ImageDisplay& hat, double);
	
public: // Animation functions.
	enum class AnimationStatus { finished, cancelled };
	std::shared_future<AnimationStatus> playAnimation(Gif2UnicornHat::ImageDisplay& hat, const Gif2UnicornHat::Animation&);
	void cancelCurrentAnimation();

private: // Async Schtuff.
	std::shared_future<AnimationStatus> currentFuture_;
	std::promise<void> cancelPromise_; //< set_value() cancels current async action.
};
