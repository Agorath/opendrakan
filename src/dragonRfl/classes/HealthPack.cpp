/*
 * HealthPack.cpp
 *
 *  Created on: 14 Feb 2018
 *      Author: zal
 */

#include <dragonRfl/classes/HealthPack.h>

#include <dragonRfl/RflDragon.h>
#include <odCore/rfl/Rfl.h>

namespace dragonRfl
{

	HealthPack::HealthPack(DragonRfl &rfl)
	: mHealthBoost(0)
	{
	}

	void HealthPack::probeFields(odRfl::FieldProbe &probe)
	{
	    ItemCommon::probeFields(probe);

	    probe.beginCategory("Health Pack");
	    probe.registerField(mHealthBoost, "Health Boost");
	}


    OD_REGISTER_RFLCLASS(DragonRfl, HealthPack);

}
