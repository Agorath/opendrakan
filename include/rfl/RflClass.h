/*
 * RflClass.h
 *
 *  Created on: 27 Jan 2018
 *      Author: zal
 */

#ifndef INCLUDE_RFL_RFLCLASS_H_
#define INCLUDE_RFL_RFLCLASS_H_

#include "rfl/RflFieldProbe.h"

namespace od
{

    class Object;

	class RflClass
	{
	public:

		RflClass(RflFieldProbe &probe);
		virtual ~RflClass() = default;

		void setLevelObject(Object &obj);


	private:

		Object *mLevelObject;

	};

}

#endif /* INCLUDE_RFL_RFLCLASSTYPE_H_ */
