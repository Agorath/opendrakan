/*
 * Handles.cpp
 *
 *  Created on: Feb 17, 2019
 *      Author: zal
 */

#include <odCore/physics/Handles.h>

namespace odPhysics
{

    LayerHandle *Handle::asLayerHandle()
    {
        return nullptr;
    }

    ObjectHandle *Handle::asObjectHandle()
    {
        return nullptr;
    }

    LightHandle *Handle::asLightHandle()
    {
        return nullptr;
    }


    LayerHandle *LayerHandle::asLayerHandle()
    {
        return this;
    }

    Handle::Type LayerHandle::getHandleType()
    {
        return Type::Layer;
    }


    ObjectHandle::ObjectHandle()
    : mLightCallback(nullptr)
    {
    }

    ObjectHandle *ObjectHandle::asObjectHandle()
    {
        return this;
    }

    Handle::Type ObjectHandle::getHandleType()
    {
        return Type::Object;
    }


    LightHandle *LightHandle::asLightHandle()
    {
        return this;
    }

    Handle::Type LightHandle::getHandleType()
    {
        return Type::Light;
    }
}

