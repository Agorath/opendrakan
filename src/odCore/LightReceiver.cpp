/*
 * LightReceiver.cpp
 *
 *  Created on: Feb 26, 2019
 *      Author: zal
 */

#include <odCore/LightReceiver.h>

#include <odCore/Light.h>

#include <odCore/physics/Handles.h>
#include <odCore/physics/PhysicsSystem.h>

namespace od
{

    void LightReceiver::removeAffectingLight(od::Light *light)
    {

    }

    void LightReceiver::addAffectingLight(od::Light *light)
    {

    }

    void LightReceiver::clearLightList()
    {

    }

    void LightReceiver::updateAffectingLights(odPhysics::PhysicsSystem &ps, odPhysics::Handle *handle)
    {
        this->clearLightList();

        odPhysics::ContactTestResultVector results;

        static const odPhysics::PhysicsTypeMasks::Mask mask = odPhysics::PhysicsTypeMasks::Light;

        ps.contactTest(handle, mask, results);

        for(auto &r : results)
        {
            odPhysics::LightHandle *handle = r.handleB->asLightHandle();
            if(handle != nullptr)
            {
                Light &light = handle->getLight();
                this->addAffectingLight(&light);
                light.addAffected(handle);
            }
        }
    }

}



