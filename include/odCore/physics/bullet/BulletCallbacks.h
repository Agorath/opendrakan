/*
 * BulletCallbacks.h
 *
 *  Created on: Jun 4, 2018
 *      Author: zal
 */

#ifndef INCLUDE_PHYSICS_BULLETCALLBACKS_H_
#define INCLUDE_PHYSICS_BULLETCALLBACKS_H_

#include <BulletCollision/CollisionDispatch/btCollisionWorld.h>

#include <odCore/physics/Handles.h>
#include <odCore/physics/PhysicsSystem.h>

namespace odBulletPhysics
{
    class ClosestNotMeConvexResultCallback : public btCollisionWorld::ClosestConvexResultCallback
    {
    public:

        ClosestNotMeConvexResultCallback(btCollisionObject *me);

        virtual btScalar addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace);


    protected:

        btCollisionObject *mMe;
    };


    class ClosestRayCallback : public btCollisionWorld::RayResultCallback
    {
    public:

        ClosestRayCallback(const btVector3 &start, const btVector3 &end, odPhysics::PhysicsTypeMasks::Mask mask, odPhysics::Handle *exclude, odPhysics::RayTestResult &result);

        virtual btScalar addSingleResult(btCollisionWorld::LocalRayResult& rayResult, bool normalInWorldSpace) override;


    private:

        odPhysics::RayTestResult &mResult;
        btVector3 mStart;
        btVector3 mEnd;
        od::RefPtr<odPhysics::Handle> mExclude;
    };


    class AllRayCallback : public btCollisionWorld::RayResultCallback
    {
    public:

        AllRayCallback(const btVector3 &start, const btVector3 &end, odPhysics::PhysicsTypeMasks::Mask mask, odPhysics::RayTestResultVector &results);

        inline size_t getHitCount() const { return mHitCount; }

        virtual btScalar addSingleResult(btCollisionWorld::LocalRayResult& rayResult, bool normalInWorldSpace) override;


    private:

        odPhysics::RayTestResultVector &mResults;
        btVector3 mStart;
        btVector3 mEnd;
        size_t mHitCount;
    };


    class ContactResultCallback : public btCollisionWorld::ContactResultCallback
    {
    public:

        ContactResultCallback(odPhysics::Handle *me, odPhysics::PhysicsTypeMasks::Mask mask, odPhysics::ContactTestResultVector &results);

        inline size_t getContactCount() const { return mContactCount; }

        virtual btScalar addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1) override;


    private:

        odPhysics::Handle *mMe;
        odPhysics::ContactTestResultVector &mResults;
        size_t mContactCount;

        const btCollisionObject* mLastObject;
    };

}


#endif /* INCLUDE_PHYSICS_BULLETCALLBACKS_H_ */
