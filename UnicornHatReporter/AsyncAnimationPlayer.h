#pragma once

#include <future>
#include <atomic>

// Forward Declarations.
namespace Gif2UnicornHat {
	class Animation;
}

class AsyncAnimationPlayer
{
public: // Construction.
	AsyncAnimationPlayer();
	~AsyncAnimationPlayer();

private: // Non-copyable.
	AsyncAnimationPlayer(const AsyncAnimationPlayer&) = delete;
	AsyncAnimationPlayer& operator=(const AsyncAnimationPlayer&) = delete;

public: // Animation functions.
	enum class AnimationStatus { finished, cancelled };
	std::shared_future<AnimationStatus> playAnimation(const Gif2UnicornHat::Animation&);
	void cancelCurrentAnimation();

private: // Async Schtuff.
	std::shared_future<AnimationStatus> currentFuture_;
	std::atomic<bool> cancelAnimationFlag_;
};
