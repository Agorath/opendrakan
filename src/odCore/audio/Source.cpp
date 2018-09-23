/*
 * Source.cpp
 *
 *  Created on: 23 Sep 2018
 *      Author: zal
 */

#include <odCore/audio/Source.h>

#include <mutex>

#include <odCore/audio/SoundManager.h>
#include <odCore/db/Sound.h>

namespace od
{

    Source::Source(SoundManager &soundManager)
    : mSoundManager(soundManager)
    , mSourceId(0)
    {
        std::lock_guard<std::mutex> lock(mSoundManager.getWorkerMutex());

        alGenSources(1, &mSourceId);
        mSoundManager.doErrorCheck("Could not generate source");
    }

    Source::~Source()
    {
        std::lock_guard<std::mutex> lock(mSoundManager.getWorkerMutex());

        mSoundManager.removeSource(this);

        alDeleteSources(1, &mSourceId);
        mSoundManager.doErrorCheck("Could not delete source");
    }

    void Source::setPosition(float xPos, float yPos, float zPos)
    {
        std::lock_guard<std::mutex> lock(mSoundManager.getWorkerMutex());

        alSource3f(mSourceId, AL_POSITION, xPos, yPos, zPos);
        mSoundManager.doErrorCheck("Could not set source position");
    }

    void Source::setVelocity(float xVel, float yVel, float zVel)
    {
        std::lock_guard<std::mutex> lock(mSoundManager.getWorkerMutex());

        alSource3f(mSourceId, AL_VELOCITY, xVel, yVel, zVel);
        mSoundManager.doErrorCheck("Could not set source velocity");
    }

    void Source::setDirection(float xDir, float yDir, float zDir)
    {
        std::lock_guard<std::mutex> lock(mSoundManager.getWorkerMutex());

        alSource3f(mSourceId, AL_DIRECTION, xDir, yDir, zDir);
        mSoundManager.doErrorCheck("Could not set source direction");
    }

    void Source::setRelative(bool relative)
    {
        std::lock_guard<std::mutex> lock(mSoundManager.getWorkerMutex());

        ALint alBoolRelative = relative ? AL_TRUE : AL_FALSE;

        alSourcei(mSourceId, AL_SOURCE_RELATIVE, alBoolRelative);
        mSoundManager.doErrorCheck("Could not set source relative state");
    }

    void Source::setSound(Sound *s)
    {

    }

}