/*
 * Class.cpp
 *
 *  Created on: 11 Feb 2018
 *      Author: zal
 */

#include "db/Class.h"

#include "Logger.h"
#include "db/Database.h"
#include "db/ClassFactory.h"
#include "rfl/Rfl.h"
#include "rfl/RflFieldProbe.h"

namespace od
{

    Class::Class(Database &db, RecordId classId)
    : Asset(db, classId)
    , mRflClassId(0)
    , mIconNumber(0)
    {
    }

    void Class::loadFromRecord(ClassFactory &factory, DataReader dr)
    {
        dr >> mClassName
           >> DataReader::Ignore(2)
           >> mModelRef
           >> mRflClassId
		   >> mIconNumber;

        mClassBuilder.readFieldRecord(dr, false);

        if(mModelRef.assetId != 0)
        {
            try
            {
                mModel = getDatabase().getModelByRef(mModelRef);

            }catch(NotFoundException &e)
            {
                Logger::warn() << "Model of class " << mClassName << " not found. Leaving invisible";
                mModel = nullptr;
            }
        }
    }

    std::unique_ptr<RflClass> Class::makeInstance()
	{
    	RflClassBuilder dummyObjectBuilder;

    	return this->makeInstance(dummyObjectBuilder);
	}

    std::unique_ptr<RflClass> Class::makeInstance(RflClassBuilder &objectBuilder)
	{
    	Logger::debug() << "Instantiating class " << std::hex << getAssetId() << std::dec;

    	try
        {
        	RflClassRegistrar &cr = Rfl::getSingleton().getClassRegistrarById(mRflClassId);

        	RflFieldProbe probe;
        	std::unique_ptr<RflClass> newInstance = cr.createClassInstance(probe);

        	// first, fill fields with template. then, let the builder we got from object override all changed values
        	mClassBuilder.fillFields(probe);
        	objectBuilder.fillFields(probe);

        }catch(NotFoundException &e)
        {
        	// ignore these for now as there are way more RflClasses than we have implemented
        	//Logger::warn() << "Tried to instantiate class with unknown RFLClass " << std::hex << mRflClassId << std::dec;
        }

        return nullptr;
	}

}
