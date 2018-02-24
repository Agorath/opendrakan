/*
 * RflFieldProbe.h
 *
 *  Created on: 27 Jan 2018
 *      Author: zal
 */

#ifndef INCLUDE_RFL_RFLCLASSBUILDER_H_
#define INCLUDE_RFL_RFLCLASSBUILDER_H_

#include <string>
#include <vector>

#include "DataStream.h"
#include "RflField.h"
#include "Exception.h"

namespace od
{

    /**
     * Common interface for classes that can be used to probe an RflClass for it's fields,
     * possibly filling them while they do (see RflClassBuilder).
     */
	class RflFieldProbe
	{
	public:

	    virtual ~RflFieldProbe() = default;

	    virtual void beginCategory(const char *categoryName) = 0;
		virtual void registerField(RflField &field, const char *fieldName) = 0; // TODO: Could use std::string_view once we switch to C++17

	};

	class RflClassBuilder : public RflFieldProbe
	{
	public:

	    RflClassBuilder();

		void readFieldRecord(DataReader &dr, bool isObjectRecord);

		virtual void beginCategory(const char *categoryName) override;
		virtual void registerField(RflField &field, const char *fieldName) override;


	private:

		struct FieldEntry
		{
			RflField::RflFieldType fieldType;
			bool isArray;
			uint16_t index;
			size_t dataOffset; // offset of entry in mFieldData
	        std::string fieldName;
		};

	    std::vector<FieldEntry> mFieldEntries;
	    std::vector<char> mFieldData;
	    size_t mRegistrationIndex;
	};

	class RflObjectBuilder : public RflFieldProbe
	{
	};

}

#endif /* INCLUDE_RFL_RFLCLASSBUILDER_H_ */
