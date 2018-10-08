/*
 * Object.h
 *
 *  Created on: 8 Feb 2018
 *      Author: zal
 */

#ifndef OBJECT_H_
#define OBJECT_H_


#include <memory>
#include <list>
#include <osg/Group>
#include <osg/PositionAttitudeTransform>
#include <LinearMath/btMotionState.h>

#include <odCore/db/Class.h>
#include <odCore/anim/SkeletonAnimationPlayer.h>
#include <odCore/rfl/RflMessage.h>
#include <odCore/light/LightState.h>

namespace od
{

    class Level;
    class Layer;

    typedef uint32_t LevelObjectId;

    enum class LevelObjectState
    {
        NotLoaded,
        Loaded,
        Spawned,
        Destroyed
    };

    enum class LevelObjectType
    {
        Normal,
        Light,
        Detector
    };

    enum class SpawnStrategy
    {
        Never,
        WhenInSight,
        Always
    };

    class LevelObject : public osg::Group, public btMotionState
    {
    public:

        LevelObject(Level &level);
        ~LevelObject();

        inline LevelObjectId getObjectId() const { return mId; }
        inline Class *getClass() { return mClass; }
        inline RflClass *getClassInstance() { return mRflClassInstance.get(); }
        inline Level &getLevel() { return mLevel; }
        inline osg::Vec3 getPosition() const { return mTransform->getPosition(); }
        inline osg::Vec3 getScale() const { return mTransform->getScale(); }
        inline osg::Quat getRotation() const { return mTransform->getAttitude(); }
        inline osg::PositionAttitudeTransform *getPositionAttitudeTransform() { return mTransform; }
        inline osg::Group *getSkeletonRoot() { return mSkeletonRoot; }
        inline LevelObjectState getState() const { return mState; }
        inline LevelObjectType getObjectType() const { return mObjectType; }
        inline void setSpawnStrategy(SpawnStrategy s) { mSpawnStrategy = s; }
        inline const std::vector<osg::ref_ptr<LevelObject>> &getLinkedObjects() const { return mLinkedObjects; }
        inline bool isVisible() const { return mIsVisible; }

        void loadFromRecord(DataReader dr);
        void spawned();
        void despawned();
        void destroyed();
        void update(double simTime, double relTime);
        void messageReceived(LevelObject &sender, RflMessage message);

        void setPosition(const osg::Vec3 &v);
        void setRotation(const osg::Quat &q);
        void setScale(const osg::Vec3 &scale);
        void setVisible(bool v);

        void setObjectType(LevelObjectType type);

        Layer *getLightingLayer();
        Layer *getLayerBelowObject();

        /**
         * @brief Attaches this object to target object.
         *
         * Attaches this LevelObject to the LevelObject \c target. Any transformation applied to \c target will also be applied to
         * \c this. This only happens in a relative context. For instance, any offset between this and \c target will be maintained
         * unless \c ignoreTranslation is true and the scale ration between these two object will be maintained unless
         * \c ignoreScale is true.
         *
         * If \c ignoreTranslation is true, translation changes applied to \c target will not be transferred to this object.
         * If \c ignoreRotation is true, rotation  changesapplied to \c target will not be transferred to this object.
         * If \c ignoreScale is true, scaling changes applied to \c target will not be transferred to this object.
         *
         * If \c ignoreTranslation, \c ignoreRotation and \c ignoreScale are all true, this method will throw.
         *
         * Calling this with \c target as \c nullptr is equivalent to calling \c detach().
         *
         * If this is called on a LevelObject that is already attached to something, it will be detached first.
         *
         * If the attachment target despawns, this object will automatically be detached.
         */
        void attachTo(LevelObject *target, bool ignoreTranslation, bool ignoreRotation, bool ignoreScale);

        /**
         * @brief Attaches this object to bone/channel of target character object.
         *
         * Attaches this LevelObject to the channel with index \c channelIndex in LevelObject \c target.
         * Any transformation applied to the respective channel in \c target will also be applied to
         * \c this. Any rotations transferred from the channel to this will pivot relative to the channel's origin.
         * If \c target has no skeleton or no channel with the given index, this method will throw.
         *
         * If \c clearOffset is true, \c this will be moved to target channel prior to attachment so their transformation will be kept equal
         * for the duration of the attachment.
         *
         * If this is called on a LevelObject that is already attached to something, it will be detached first.
         */
        void attachToChannel(LevelObject *target, size_t channelIndex, bool clearOffset = false);

        /**
         * @brief Detaches this object from any objects. Will do nothing if this object was not attached to anything.
         */
        void detach();

        /**
         * @brief Enables or disables the RFL update hook.
         *
         * Do not disable the update callback by calling this method from the update hook itself! Doing so messes with
         * OSG's update traversal and will likely cause it to segfault.
         */
        void setEnableRflUpdateHook(bool enableHook);
        void messageAllLinkedObjects(RflMessage message);
        void requestDestruction();

        // override osg::Group
        virtual const char *libraryName() const override { return "od";    }
        virtual const char *className()   const override { return "LevelObject"; }

        // implement btMotionState
        virtual void getWorldTransform(btTransform& worldTrans) const override;
        virtual void setWorldTransform(const btTransform& worldTrans) override;


    private:

        void _onTransformChanged(LevelObject *transformChangeSource);
        void _updateLayerBelowObject();
        void _attachmentTargetsTransformUpdated(LevelObject *transformChangeSource); // pass along source so we can detect circular attachments
        void _detachAllAttachedObjects();
        void _setVisible(bool b); // just so we can switch visibility internally without producing logs everytime


        Level &mLevel;
        LevelObjectId mId;
        AssetRef mClassRef;
        osg::ref_ptr<Class> mClass;
        std::unique_ptr<RflClass> mRflClassInstance;
        uint32_t mLightingLayerId;
        osg::ref_ptr<Layer> mLightingLayer;
        osg::Vec3f mInitialPosition;
        uint32_t mFlags;
        uint16_t mInitialEventCount;
        std::vector<uint16_t> mLinks;
        osg::Vec3f mInitialScale;
        osg::Quat  mInitialRotation;

        osg::ref_ptr<osg::PositionAttitudeTransform> mTransform;
        osg::ref_ptr<osg::Group> mSkeletonRoot;
        LevelObjectState mState;
        LevelObjectType mObjectType;
        SpawnStrategy mSpawnStrategy;
        bool mIsVisible;

        std::vector<osg::ref_ptr<LevelObject>> mLinkedObjects;

        osg::ref_ptr<od::LevelObject> mAttachmentTarget;
        std::list<osg::ref_ptr<od::LevelObject>> mAttachedObjects;
        osg::Vec3 mAttachmentTranslationOffset;
        osg::Quat mAttachmentRotationOffset;
        osg::Vec3 mAttachmentScaleRatio;
        bool mIgnoreAttachmentTranslation;
        bool mIgnoreAttachmentRotation;
        bool mIgnoreAttachmentScale;

        bool mLayerBelowObjectDirty;
        osg::ref_ptr<Layer> mLayerBelowObject;

        osg::ref_ptr<osg::NodeCallback> mUpdateCallback;
        osg::ref_ptr<LightStateCallback> mLightingCallback;

        bool mRflUpdateHookEnabled;
    };

}



#endif /* OBJECT_H_ */
