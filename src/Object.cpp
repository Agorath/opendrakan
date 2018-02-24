/*
 * Object.cpp
 *
 *  Created on: 8 Feb 2018
 *      Author: zal
 */

#include "Object.h"

#include "Level.h"
#include "Exception.h"
#include "OdDefines.h"
#include "rfl/RflClass.h"

#define OD_OBJECT_FLAG_VISIBLE 0x001
#define OD_OBJECT_FLAG_SCALED  0x100

namespace od
{

    Object::Object(Level &level)
    : mLevel(level)
    , mId(0)
    , mFlags(0)
    , mInitialEventCount(0)
    {
    }

    void Object::loadFromRecord(DataReader dr)
    {
        AssetRef classRef;
        float xPos;
        float yPos;
        float zPos;
        uint16_t xRot;
        uint16_t yRot;
        uint16_t zRot;
        float xScale = 1;
        float yScale = 1;
        float zScale = 1;

        uint16_t dummyLength;

        dr >> mId
           >> classRef
           >> DataReader::Ignore(4)
           >> xPos
           >> yPos
           >> zPos
           >> mFlags
           >> mInitialEventCount
		   >> dummyLength;

        dr.ignore(2*dummyLength);

        dr >> xRot
           >> yRot
           >> zRot;

        if(mFlags & OD_OBJECT_FLAG_SCALED)
        {
            dr >> xScale
               >> yScale
               >> zScale;
        }


        RflClassBuilder builder;
        builder.readFieldRecord(dr, true);

        mClass = mLevel.getClassByRef(classRef);

        mRflClassInstance = mClass->makeInstance();
        if(mRflClassInstance != nullptr)
        {
            mRflClassInstance->setLevelObject(*this);
            mRflClassInstance->probeFields(builder); // let builder override fields
        }

        if(mClass->hasModel() && (mFlags & OD_OBJECT_FLAG_VISIBLE))
        {
			osg::ref_ptr<osg::PositionAttitudeTransform> transform(new osg::PositionAttitudeTransform);
			transform->setAttitude(osg::Quat(
				osg::DegreesToRadians((float)xRot), osg::Vec3(1,0,0),
				osg::DegreesToRadians((float)yRot-90), osg::Vec3(0,1,0),  // -90 deg. determined to be correct through experiment
				osg::DegreesToRadians((float)zRot), osg::Vec3(0,0,1)));
			transform->setPosition(osg::Vec3(xPos, yPos, zPos) * OD_WORLD_SCALE);
			transform->setScale(osg::Vec3(xScale, yScale, zScale));

			transform->addChild(mClass->getModel());
			this->addChild(transform);
        }
    }

}




