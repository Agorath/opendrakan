/*
 * Class.h
 *
 *  Created on: 11 Feb 2018
 *      Author: zal
 */

#ifndef INCLUDE_CLASS_H_
#define INCLUDE_CLASS_H_

#include <osg/Referenced>
#include <osg/ref_ptr>
#include <memory>

#include <odCore/db/Asset.h>
#include <odCore/db/Model.h>
#include <odCore/rfl/Rfl.h>
#include <odCore/rfl/ClassBuilderProbe.h>

namespace odRfl
{
	class RflClassRegistrar;
	class RflClass;
	class Rfl;
}

namespace odDb
{
    class ClassFactory;

	class Class : public Asset, public osg::Referenced
	{
	public:

		Class(AssetProvider &ap, od::RecordId classId);

		inline bool hasModel() const { return mModel != nullptr; }
        inline osg::ref_ptr<Model> getModel() { return mModel; }
        inline std::string getName() const { return mClassName; }

        void loadFromRecord(ClassFactory &factory, od::DataReader dr);
        std::unique_ptr<odRfl::RflClass> makeInstance();


	private:

        std::string mClassName;
        AssetRef mModelRef;
        osg::ref_ptr<Model> mModel;
        uint16_t mRflClassId;
        odRfl::ClassBuilderProbe mClassBuilder;
        uint16_t mIconNumber;

        odRfl::Rfl *mRfl;
        odRfl::RflClassRegistrar *mRflClassRegistrar;

	};

	template <>
    struct AssetTraits<Class>
    {
        static const char *name()
        {
            return "Class";
        }
    };

}

#endif /* INCLUDE_CLASS_H_ */
