/*
 * Buffer.h
 *
 *  Created on: 27 Sep 2018
 *      Author: zal
 */

#ifndef INCLUDE_ODCORE_AUDIO_BUFFER_H_
#define INCLUDE_ODCORE_AUDIO_BUFFER_H_

#include <odCore/RefCounted.h>

namespace odAudio
{

    class SoundManager;
    class SoundContext;

    class Buffer : public od::RefCounted
    {
    public:

        virtual ~Buffer() = default;

    };

}

#endif /* INCLUDE_ODCORE_AUDIO_BUFFER_H_ */
