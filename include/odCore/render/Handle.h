/*
 * Handle.h
 *
 *  Created on: May 15, 2019
 *      Author: zal
 */

#ifndef INCLUDE_ODCORE_RENDER_HANDLE_H_
#define INCLUDE_ODCORE_RENDER_HANDLE_H_

#include <mutex>

#include <glm/vec3.hpp>

#include <odCore/RefCounted.h>

namespace od
{
    class Light;
}

namespace odRender
{
    class Model;
    class Rig;
    class FrameListener;

    enum class RenderMode
    {
        Normal,
        Sky
    };

    class Handle : public od::RefCounted
    {
    public:

        /**
         * Provides a mutex that can be used to synchronize access to this node with the
         * render thread. Note that this will already be locked during any renderer-provided
         * callbacks. Trying trying to lock it again inside a callback will cause a deadlock.
         */
        virtual std::mutex &getMutex() = 0;

        virtual Model *getModel() = 0;
        virtual void setModel(Model *model) = 0;

        virtual void setVisible(bool visible) = 0;
        virtual void setModelPartVisible(size_t partIndex, bool visible) = 0;

        virtual void setRenderMode(RenderMode rm) = 0;

        virtual void addFrameListener(FrameListener *listener) = 0;
        virtual void removeFrameListener(FrameListener *listener) = 0;

        virtual void setEnableColorModifier(bool b) = 0;
        virtual void setColorModifier(const glm::vec4 &cm) = 0;

        virtual Rig *getRig() = 0;

        virtual void addLight(od::Light *light) = 0;
        virtual void removeLight(od::Light *light) = 0;
        virtual void clearLightList() = 0;
        virtual void setGlobalLight(const glm::vec3 &direction, const glm::vec3 &diffuse, const glm::vec3 &ambient) = 0;
    };

}


#endif /* INCLUDE_ODCORE_RENDER_HANDLE_H_ */
