/*
 * StaticLight.cpp
 *
 *  Created on: Aug 15, 2018
 *      Author: zal
 */

#include <dragonRfl/classes/StaticLight.h>

#include <dragonRfl/RflDragon.h>

#include <odCore/rfl/Rfl.h>

#include <odCore/LevelObject.h>
#include <odCore/Level.h>
#include <odCore/Engine.h>

#include <odCore/physics/PhysicsSystem.h>

namespace dragonRfl
{

    StaticLight::StaticLight(DragonRfl &rfl)
    : mColor(0xffffffff)
    , mIntensityScaling(1.0)
    , mRadius(1.0)
    , mLightMap(odDb::AssetRef::NULL_REF)
    , mQualityLevelRequired(0)
    , mNeedsUpdate(true)
    {
    }

    void StaticLight::probeFields(odRfl::FieldProbe &probe)
    {
        probe("Point Light")
                (mColor, "Color")
                (mIntensityScaling, "Intensity Scaling")
                (mRadius, "Radius (lu)")
                (mLightMap, "Light Map")
                (mQualityLevelRequired, "Quality Level Required");
    }

    void StaticLight::onLoaded(od::LevelObject &obj)
    {
        obj.setObjectType(od::LevelObjectType::Light);

        glm::vec4 color = mColor.asColorVector();
        mLightColorVector = glm::vec3(color.r, color.g, color.b);
    }

    void StaticLight::onSpawned(od::LevelObject &obj)
    {
        mLight = od::make_refd<od::Light>(obj.getLevel().getEngine().getPhysicsSystem());
        mLight->setColor(mLightColorVector);
        mLight->setRadius(mRadius);
        mLight->setIntensityScaling(mIntensityScaling);
        mLight->setRequiredQualityLevel(mQualityLevelRequired);
        mLight->setPosition(obj.getPosition());

        obj.setEnableRflUpdateHook(true);
        mNeedsUpdate = true;
    }

    void StaticLight::onUpdate(od::LevelObject &obj, float relTime)
    {
        if(!mNeedsUpdate || mLight == nullptr)
        {
            return;
        }

        mLight->updateAffectedList();
        mNeedsUpdate = false;
    }

    void StaticLight::onDespawned(od::LevelObject &obj)
    {
        mLight = nullptr;
    }

    OD_REGISTER_RFLCLASS(DragonRfl, StaticLight);

}

