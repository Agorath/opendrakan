/*
 * SoundEffect.h
 *
 *  Created on: Sep 25, 2018
 *      Author: zal
 */

#ifndef INCLUDE_DRAGONRFL_SOUNDEFFECT_H_
#define INCLUDE_DRAGONRFL_SOUNDEFFECT_H_

#include <random>

#include <odCore/rfl/RflClass.h>
#include <odCore/rfl/RflField.h>

namespace odAudio
{
    class Source;
}

namespace dragonRfl
{
    class DragonRfl;

    class SoundEffect : public odRfl::RflClass
    {
    public:

        enum class PlayMode
        {
            Once,
            Looping,
            Intermittent,
            Triggered,
            LoopingAndTriggered
        };

        enum class Location
        {
            EffectSite,
            Player,
            Camera
        };

        SoundEffect(DragonRfl &rfl);

        virtual void probeFields(odRfl::FieldProbe &probe) override;

        virtual void onLoaded(od::LevelObject &obj) override;
        virtual void onSpawned(od::LevelObject &obj) override;
        virtual void onDespawned(od::LevelObject &obj) override;
        virtual void onUpdate(od::LevelObject &obj, double simTime, double relTime) override;
        virtual void onMessageReceived(od::LevelObject &obj, od::LevelObject &sender, odRfl::RflMessage message) override;
        virtual void onMoved(od::LevelObject &obj) override;


    private:

        void _playRandomSound();

        odRfl::RflSoundRefArray            mSounds;
        odRfl::RflEnumImpl<PlayMode, 0, 4> mPlayMode;
        odRfl::RflFloat                    mAveragePeriod;
        odRfl::RflFloat                    mPeriodRandomDeviation;
        odRfl::RflEnumImpl<Location, 0, 3> mLocation;

        odAudio::Source *mSoundSource;
        double mTimeUntilNextPlay;

        std::minstd_rand mRandomNumberGenerator;
    };

}

OD_DEFINE_RFLCLASS_TRAITS(dragonRfl::DragonRfl, 0x0006, "Special Effect", "Sound Effect", dragonRfl::SoundEffect);

#endif /* INCLUDE_DRAGONRFL_SOUNDEFFECT_H_ */
