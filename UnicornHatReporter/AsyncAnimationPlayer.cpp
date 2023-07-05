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


void AsyncAnimationPlayer::setBrightness(ImageDisplay& hat, double brightness)
{
	hat.setBrightness(brightness);
}


namespace {
	AnimationStatus playAnimationWorker(ImageDisplay& hat, future<void> cancelled, Animation animation)
	{
		int numLoops = (animation.numFrames() == 1) ? 1 : animation.numLoops(); //< Don't loop if there is only one frame.
		for (int loopNum = 0; loopNum < numLoops || numLoops == 0; ++loopNum) {
			for (auto& frame : animation.frames()) {				
				hat.showImage(frame.image);
				
				// Wait for the next frame or until we are cancelled.
				bool isCancelled = (cancelled.wait_for(frame.duration) == future_status::ready);
				if (isCancelled) {
					return AnimationStatus::cancelled;
				}
			}
		}
		
		return AnimationStatus::finished;
	}
}


shared_future<AnimationStatus> AsyncAnimationPlayer::playAnimation(ImageDisplay& hat, const Animation& animation)
{
	cancelCurrentAnimation();
	
	cancelPromise_ = promise<void>();
	currentFuture_ = std::async(launch::async, playAnimationWorker, std::ref(hat), cancelPromise_.get_future(), animation);
	return currentFuture_;
}


void AsyncAnimationPlayer::cancelCurrentAnimation()
{
	if (currentFuture_.valid()) {
		cancelPromise_.set_value();
		currentFuture_.wait();
	}
}