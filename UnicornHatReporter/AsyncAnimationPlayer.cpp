#include "AsyncAnimationPlayer.h"

#include <UnicornHat.h>
#include <Animation.h>

#include <future>
#include <atomic>
#include <functional>

using namespace std;
using namespace Gif2UnicornHat;
using AnimationStatus = AsyncAnimationPlayer::AnimationStatus;


AsyncAnimationPlayer::AsyncAnimationPlayer()
{
	
}


AsyncAnimationPlayer::~AsyncAnimationPlayer()
{
	
}


namespace {
	AnimationStatus playAnimationWorker(const atomic<bool>& isCancelled, Animation animation)
	{
		UnicornHat& hat = UnicornHat::instance();
		for (int loopNum = 0; loopNum < animation.numLoops() || animation.numLoops() == 0; ++loopNum) {
			for (auto& frame : animation.frames()) {
				if (isCancelled) {
					return AnimationStatus::cancelled;
				}
				
				hat.showImage(frame.image);
				this_thread::sleep_for(frame.duration); // TODO: Check the cancelled flag while waiting so long frames can't hang cancelCurrentAnimation(). Maybe replace the flag with a future of its own.
			}
		}
		
		return AnimationStatus::finished;
	}
}


shared_future<AnimationStatus> AsyncAnimationPlayer::playAnimation(const Animation& animation)
{
	cancelCurrentAnimation();
	currentFuture_ = std::async(launch::async, playAnimationWorker, cref(cancelAnimationFlag_), animation);
	return currentFuture_;
}


void AsyncAnimationPlayer::cancelCurrentAnimation()
{
	if (currentFuture_.valid()) {
		cancelAnimationFlag_ = true;
		currentFuture_.wait();
		cancelAnimationFlag_ = false;
	}
}