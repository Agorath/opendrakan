/*
 * DefaultObjectClass.cpp
 *
 *  Created on: Apr 2, 2019
 *      Author: zal
 */

#include <odCore/rfl/DefaultObjectClass.h>

#include <odCore/Engine.h>
#include <odCore/Level.h>
#include <odCore/LevelObject.h>
#include <odCore/Layer.h>

#include <odCore/db/Model.h>
#include <odCore/db/ModelBounds.h>

#include <odCore/render/Renderer.h>
#include <odCore/render/ObjectNode.h>
#include <odCore/render/ModelNode.h>

#include <odCore/physics/PhysicsSystem.h>
#include <odCore/physics/Handles.h>

namespace odRfl
{

    DefaultObjectClass::~DefaultObjectClass()
    {
    }

    void DefaultObjectClass::probeFields(FieldProbe &probe)
    {
    }

    void DefaultObjectClass::onSpawned(od::LevelObject &obj)
    {
        // create render node if applicable
        odRender::Renderer *renderer = obj.getLevel().getEngine().getRenderer();
        if(renderer != nullptr && obj.getClass()->hasModel())
        {
            mRenderNode = renderer->createObjectNode(obj);

            _updateLighting(obj);
        }

        // if we created a rendering handle, create collisionless physics handle, too. this way, lighting will still work on those models.
        odPhysics::PhysicsSystem &ps = obj.getLevel().getEngine().getPhysicsSystem();
        if(mRenderNode != nullptr)
        {
            mPhysicsHandle = ps.createObjectHandle(obj, true);
        }

        mLightReceiver = std::make_unique<od::ObjectLightReceiver>(ps, mPhysicsHandle, mRenderNode);
    }

    void DefaultObjectClass::onDespawned(od::LevelObject &obj)
    {
        mRenderNode = nullptr;
        mPhysicsHandle = nullptr;
        mLightReceiver = nullptr;
    }

    void DefaultObjectClass::onMoved(od::LevelObject &obj)
    {
        _updateLighting(obj);
    }

    void DefaultObjectClass::_updateLighting(od::LevelObject &obj)
    {
        if(mRenderNode != nullptr)
        {
            od::Layer *lightingLayer = (obj.getLightSourceLayer()!= nullptr) ?
                    obj.getLightSourceLayer() : obj.getLevel().getFirstLayerBelowPoint(obj.getPosition());
            if(lightingLayer != nullptr)
            {
                mRenderNode->setGlobalLight(lightingLayer->getLightColor(), lightingLayer->getAmbientColor(), lightingLayer->getLightDirection());
            }
        }

        if(mLightReceiver != nullptr)
        {
            mLightReceiver->updateAffectingLights();
        }
    }

}



