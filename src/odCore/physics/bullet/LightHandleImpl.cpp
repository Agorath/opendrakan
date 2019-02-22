/*
 * LightHandleImpl.cpp
 *
 *  Created on: 17 Feb 2019
 *      Author: zal
 */

#include <odCore/physics/bullet/LightHandleImpl.h>

#include <odCore/Light.h>
#include <odCore/LevelObject.h>

#include <odCore/physics/bullet/BulletAdapter.h>
#include <odCore/physics/bullet/BulletPhysicsSystem.h>

namespace odBulletPhysics
{

    LightHandle::LightHandle(od::Light *light, btCollisionWorld *collisionWorld)
    : mLight(light)
    , mCollisionWorld(collisionWorld)
    {
        mShape = std::make_unique<btSphereShape>(mLight->getRadius());

        mCollisionObject = std::make_unique<btCollisionObject>();
        mCollisionObject->setCollisionShape(mShape.get());

        btTransform worldTransform = BulletAdapter::makeBulletTransform(light->getLevelObject().getPosition(), glm::quat(1, 0, 0, 0));
        mCollisionObject->setWorldTransform(worldTransform);

        mCollisionObject->setUserPointer(static_cast<odPhysics::Handle*>(this));

        mCollisionWorld->addCollisionObject(mCollisionObject.get(), BulletCollisionGroups::LIGHT, BulletCollisionGroups::ALL);
    }

    LightHandle::~LightHandle()
    {
        mCollisionObject->setUserPointer(nullptr);
        mCollisionWorld->removeCollisionObject(mCollisionObject.get());
    }

    void LightHandle::setRadius(float radius)
    {
        mShape->setUnscaledRadius(radius);
    }

    void LightHandle::setPosition(const glm::vec3 &pos)
    {
        btTransform worldTransform = BulletAdapter::makeBulletTransform(pos, glm::quat(1, 0, 0, 0));
        mCollisionObject->setWorldTransform(worldTransform);
    }

    od::Light *LightHandle::getLight()
    {
        return mLight;
    }

}
