/*
 * HumanControl.h
 *
 *  Created on: 2 Apr 2018
 *      Author: zal
 */

#ifndef INCLUDE_RFL_DRAGON_HUMANCONTROL_H_
#define INCLUDE_RFL_DRAGON_HUMANCONTROL_H_

#include <odCore/db/Animation.h>

#include <odCore/physics/Handles.h>
#include <odCore/physics/CharacterController.h>

#include <odCore/input/Action.h>
#include <odCore/input/CursorListener.h>

#include <odCore/anim/SkeletonAnimationPlayer.h>

#include <odCore/render/Handle.h>

#include <dragonRfl/classes/HumanControlFields.h>
#include <dragonRfl/LocalPlayer.h>
#include <dragonRfl/Actions.h>

namespace dragonRfl
{
    class DragonRfl;

	class HumanControl : public HumanControlFields, public LocalPlayer
	{
	public:

		HumanControl(DragonRfl &rfl);
		virtual ~HumanControl();

		virtual void onLoaded(od::LevelObject &obj) override;
		virtual void onSpawned(od::LevelObject &obj) override;
		virtual void onUpdate(od::LevelObject &obj, float relTime) override;
		virtual void onMoved(od::LevelObject &obj) override;

		virtual float getYaw() const override { return mYaw; }
		virtual void setYaw(float f) override { mYaw = f; }
		virtual float getPitch() const override { return mPitch; }
		virtual void setPitch(float f) override { mPitch = f; }
		virtual glm::vec3 getPosition() override;
		virtual od::LevelObject &getLevelObject() override;
		virtual odPhysics::Handle *getPhysicsHandle() override;


	 private:

		enum class State
		{
		    Idling,
		    TurningLeft,
		    TurningRight,
		    RunningForward,
		    RunningBackward
		};

		void _handleMovementAction(odInput::ActionHandle<Action> *action, odInput::InputEvent event);
		void _handleCursorMovement(const glm::vec2 &posNdc);
		void _playAnim(const odRfl::AnimRef &animRef, bool skeletonOnly, bool looping);

		DragonRfl &mRfl;

		float mYaw;
		float mPitch;
		od::LevelObject *mPlayerObject;

		State mState;
		float mLastUpdatedYaw;

		std::unique_ptr<odAnim::SkeletonAnimationPlayer> mAnimPlayer;

		od::RefPtr<odRender::Handle> mRenderHandle;
		od::RefPtr<odPhysics::ObjectHandle> mPhysicsHandle;
		std::unique_ptr<odPhysics::CharacterController> mCharacterController;

		od::RefPtr<odInput::ActionHandle<Action>> mForwardAction;
		od::RefPtr<odInput::ActionHandle<Action>> mBackwardAction;
		od::RefPtr<odInput::CursorListener> mCursorListener;

	};

}

OD_DEFINE_RFLCLASS_TRAITS(dragonRfl::DragonRfl, 0x0009, "Player", "Human Control", dragonRfl::HumanControl);

#endif /* INCLUDE_RFL_DRAGON_HUMANCONTROL_H_ */
