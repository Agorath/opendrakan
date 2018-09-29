/*
 * Buffer.h
 *
 *  Created on: 27 Sep 2018
 *      Author: zal
 */

#ifndef INCLUDE_ODCORE_AUDIO_BUFFER_H_
#define INCLUDE_ODCORE_AUDIO_BUFFER_H_

#include <AL/al.h>
#include <stdlib.h>
#include <stdint.h>
#include <memory>

namespace od
{

    class SoundManager;
    class SoundContext;

    class Buffer
    {
    public:

        enum class Format
        {
            Mono8Bit,
            Mono16Bit,
            Stereo8Bit,
            Stereo16Bit
        };

        Buffer(SoundManager &soundManager);
        Buffer(const Buffer &buffer) = delete;
        ~Buffer();

        inline ALuint getId() const { return mBufferId; }

        Format getFormatFor(size_t nBits, size_t nChannels) const;

        void setData(uint8_t *data, size_t size, Format format, size_t frequency);


    private:

        ALenum _toAlFormat(Format format);

        SoundManager &mSoundManager;

        // It seems like buffers need to be destroyed before the last context is destroyed.
        //  To ensure this, every buffer shares ownership over the buffer under which it was created
        //  TODO: we might instead give sole ownership of Buffers to SoundManager, who must then have sole ownership of the context
        std::shared_ptr<SoundContext> mContext;

        ALuint mBufferId;
    };

}

#endif /* INCLUDE_ODCORE_AUDIO_BUFFER_H_ */