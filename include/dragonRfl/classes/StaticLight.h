/*
 * StaticLight.h
 *
 *  Created on: Aug 15, 2018
 *      Author: zal
 */

#ifndef INCLUDE_RFL_DRAGON_STATICLIGHT_H_
#define INCLUDE_RFL_DRAGON_STATICLIGHT_H_

#include <odCore/rfl/RflClass.h>
#include <odCore/rfl/Field.h>
#include <odCore/rfl/AssetRefField.h>

#include <glm/vec3.hpp>

#include <odCore/render/Light.h>

namespace dragonRfl
{

    class DragonRfl;

    class StaticLight : public odRfl::RflClass
    {
    public:

        StaticLight(DragonRfl &rfl);

        virtual void probeFields(odRfl::FieldProbe &probe) override;
        virtual void onLoaded(od::LevelObject &obj) override;
        virtual void onSpawned(od::LevelObject &obj) override;
        virtual void onMoved(od::LevelObject &obj) override;
        virtual void onDespawned(od::LevelObject &obj) override;


    protected:

        odRfl::Color        mColor;
        odRfl::Float        mIntensityScaling;
        odRfl::Float        mRadius;
        odRfl::TextureRef   mLightMap;
        odRfl::Enum         mQualityLevelRequired;

        glm::vec3 mLightColorVector;
        od::RefPtr<odRender::Light> mLight;
    };

}

OD_DEFINE_RFLCLASS_TRAITS(dragonRfl::DragonRfl, 0x0084, "Light Source", "Static Light", dragonRfl::StaticLight);

#endif /* INCLUDE_RFL_DRAGON_STATICLIGHT_H_ */
