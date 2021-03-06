/*
 * CrystalRingButton.h
 *
 *  Created on: 5 Aug 2018
 *      Author: zal
 */

#ifndef INCLUDE_GUI_CRYSTALRINGBUTTON_H_
#define INCLUDE_GUI_CRYSTALRINGBUTTON_H_

#include <functional>

#include <odCore/gui/Widget.h>

#include <odCore/render/Handle.h>
#include <odCore/render/Model.h>

#include <odCore/anim/Interpolator.h>

#include <odCore/db/Model.h>
#include <odCore/db/Sound.h>

namespace odAudio
{
    class Source;
}

namespace dragonRfl
{

    class DragonGui;

    class CrystalRingButton : public odGui::Widget
    {
    public:

        CrystalRingButton(DragonGui &gui, odDb::Model *crystalModel, odDb::Model *innerRingModel, odDb::Model *outerRingModel, odDb::Sound *hoverSound, float noteOffset);
        CrystalRingButton(const CrystalRingButton &c) = delete;
        virtual ~CrystalRingButton();

        inline void setInactiveCrystalColor(const glm::vec4 &color) { mCrystalColorInactive = color; }
        inline void setActiveCrystalColor(const glm::vec4 &color) { mCrystalColorActive = color; }
        inline void setClickedCallback(const std::function<void(int)> &callback, int userData) { mClickedCallback = callback; mCallbackUserData = userData; }

        virtual bool liesWithinLogicalArea(const glm::vec2 &pos) override;
        virtual void onMouseDown(const glm::vec2 &pos, int button) override;
        virtual void onMouseEnter(const glm::vec2 &pos) override;
        virtual void onMouseLeave(const glm::vec2 &pos) override;
        virtual void onUpdate(float relTime) override;


    private:

        od::RefPtr<odRender::Handle> mCrystalHandle;
        od::RefPtr<odRender::Handle> mInnerRingHandle;
        od::RefPtr<odRender::Handle> mOuterRingHandle;
        glm::vec4 mCrystalColorInactive;
        glm::vec4 mCrystalColorActive;
        std::function<void(int)> mClickedCallback;
        int mCallbackUserData;

        float mCrystalSpeedPercent; // 0=stop, 1=max speed
        bool mClicked;
        float mRingAnimPercent;
        odAnim::Interpolated<glm::vec4> mCrystalColor;

        od::RefPtr<odAudio::Source> mSoundSource;
    };

}

#endif /* INCLUDE_GUI_CRYSTALRINGBUTTON_H_ */
