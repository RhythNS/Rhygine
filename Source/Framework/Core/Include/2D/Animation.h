#pragma once
#include <vector>

/// <summary>
/// Helper class for managing keyframe based animations.
/// </summary>
/// <typeparam name="T">The keyframe type.</typeparam>
template <class T>
class Animation
{
public:
	enum class PlaybackMode
	{
		Normal, Reversed, NormalLoop, ReversedLoop, PingPongLoop
	};

	Animation(std::vector<T*> keyFrames, float keyFrameDuration, PlaybackMode mode = PlaybackMode::Normal) :
		keyFrames(keyFrames), keyframeDuration(keyframeDuration), playbackMode(mode)
	{ }

	/// <summary>
	/// Gets a keyframe with total elapsed time.
	/// </summary>
	/// <param name="time">Time in total elapsed time. Not delta time!</param>
	/// <returns>The current keyframe.</returns>
	T* GetKeyframe(float time)
	{
		if (time < 0)
			time = 0;

		int at = (int)(time / keyframeDuration);

		switch (playbackMode)
		{
		case PlaybackMode::Normal:
			return InnerGetNormal(at);

		case PlaybackMode::Reversed:
			return InnerGetReversed(at);

		case PlaybackMode::NormalLoop:
			at %= keyFrames.size();
			return InnerGetNormal(at);

		case PlaybackMode::ReversedLoop:
			at %= keyFrames.Length;
			return InnerGetReversed(at);

		case PlaybackMode.PingPongLoop:
			at %= keyFrames.Length * 2;
			if (at < keyFrames.Length)
				return InnerGetNormal(at);
			else
			{
				at -= keyFrames.Length;
				return InnerGetReversed(at);
			}
		}

		return nullptr;
	}

private:
	inline T* InnerGetNormal(int& at)
	{
		return keyFrames[std::min(at, keyFrames.size() - 1)];
	}
	inline T* InnerGetReversed(int& at)
	{
		return keyFrames[std::min(keyFrames.size() - 1 - at, 0)];
	}

	std::vector<T*> keyFrames;
	float keyframeDuration;
	PlaybackMode playbackMode;
};
