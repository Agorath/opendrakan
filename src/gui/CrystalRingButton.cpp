/*
 * CrystalRingButton.cpp
 *
 *  Created on: 5 Aug 2018
 *      Author: zal
 */

#include "gui/CrystalRingButton.h"

#include "Exception.h"
#include "gui/GuiManager.h"
#include "Engine.h"

// max crystal speed in rad/s
#define OD_CRYSTAL_SPEED_MAX       (3.0*M_PI)

// time a crystal needs to speed up from stop to max speed
#define OD_CRYSTAL_SPEED_RISETIME   3.0

// time a crystal at max speed needs to decelerate to stop
#define OD_CRYSTAL_SPEED_FALLTIME   3.0

namespace od
{

    CrystalRingButton::CrystalRingButton(GuiManager &gm, Model *crystalModel, Model *innerRingModel, Model *outerRingModel)
    : Widget(gm)
    , mCrystalModel(crystalModel)
    , mInnerRingModel(innerRingModel)
    , mOuterRingModel(outerRingModel)
    , mCrystalColorInactive(0.38431, 0.36471, 0.54902, 1.0)
    , mCrystalColorActive(0.95686275, 0.25882353, 0.63137255, 1.0)
    , mTransform(new osg::MatrixTransform)
    , mCrystalSpeedPercent(0.0)
    {
        // select whatever model in not null for bounds calculation, starting with outer ring
        osg::ref_ptr<Model> modelForBounds =
                (mOuterRingModel != nullptr) ? mOuterRingModel : ((mInnerRingModel != nullptr) ? mInnerRingModel : mCrystalModel);

        if(modelForBounds == nullptr)
        {
            throw Exception("Passed no non-null models to CrystalRingButton");
        }

        osg::BoundingBox box = modelForBounds->getCalculatedBoundingBox();
        float diameter = box.yMax() - box.yMin(); // ring's "hole" is parallel to y axis, so this should be the best way to get the diameter

        osg::Matrix matrix = osg::Matrix::identity();
        matrix.postMultTranslate(-box.center());
        matrix.postMultScale(osg::Vec3(1/diameter, 1/diameter, 1/diameter));
        matrix.postMultTranslate(osg::Vec3(0.5, 0.5, 0.0));
        mTransform->setMatrix(matrix);

        mTransform->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);
        mTransform->getOrCreateStateSet()->setMode(GL_CULL_FACE, osg::StateAttribute::ON);

        if(mCrystalModel != nullptr)
        {
            mCrystalTransform = new osg::PositionAttitudeTransform;
            mCrystalTransform->addChild(mCrystalModel);

            osg::ref_ptr<osg::Shader> crystalFragShader = gm.getEngine().getShaderManager().loadShader("crystal_fragment.glsl", osg::Shader::FRAGMENT);
            osg::ref_ptr<osg::Program> crystalProg = gm.getEngine().getShaderManager().makeProgram(nullptr, crystalFragShader);
            mCrystalTransform->getOrCreateStateSet()->setAttribute(crystalProg, osg::StateAttribute::ON);

            mColorModifierUniform = new osg::Uniform("colorModifier", mCrystalColorInactive);
            mCrystalTransform->getOrCreateStateSet()->addUniform(mColorModifierUniform);

            mCrystalTransform->setScale(osg::Vec3(0.58, 0.58, 0.58));
            mCrystalTransform->setAttitude(osg::Quat(M_PI, osg::Vec3(0, 1, 0)));

            mTransform->addChild(mCrystalTransform);
        }

        if(mInnerRingModel != nullptr)
        {
            mInnerRingTransform = new osg::PositionAttitudeTransform;
            mInnerRingTransform->addChild(mInnerRingModel);
            mTransform->addChild(mInnerRingTransform);
        }

        if(mOuterRingModel != nullptr)
        {
            mOuterRingTransform = new osg::PositionAttitudeTransform;
            mOuterRingTransform->addChild(mOuterRingModel);
            mTransform->addChild(mOuterRingTransform);
        }

        this->addChild(mTransform);
    }

    bool CrystalRingButton::liesWithinLogicalArea(const osg::Vec2 &pos)
    {
        // logical area is circle around (0.5 0.5) TODO: implement non-circular buttons here
        osg::Vec2 p = pos - osg::Vec2(0.5, 0.5);
        return p.length2() <= 0.25;
    }

    void CrystalRingButton::onMouseDown(const osg::Vec2 &pos, int button)
    {
        Logger::info() << "Button clicked. Interpreting this as desire to quit the game for now.";

        getGuiManager().quit();
    }

    void CrystalRingButton::onUpdate(double simTime, double relTime)
    {
        if(mCrystalTransform == nullptr)
        {
            return;
        }

        if(isMouseOver())
        {
            if(mCrystalSpeedPercent < 1.0)
            {
                mCrystalSpeedPercent = std::min(mCrystalSpeedPercent + relTime/OD_CRYSTAL_SPEED_RISETIME, 1.0);
            }

        }else
        {
            if(mCrystalSpeedPercent > 0.0)
            {
                mCrystalSpeedPercent = std::max(mCrystalSpeedPercent - relTime/OD_CRYSTAL_SPEED_FALLTIME, 0.0);
            }
        }

        // apply scaling function to speed percentage. for now, just use a linear function
        float crystalSpeed = mCrystalSpeedPercent * OD_CRYSTAL_SPEED_MAX;

        // apply rotation to crystal
        osg::Quat q = mCrystalTransform->getAttitude();
        q *= osg::Quat(crystalSpeed * relTime, osg::Vec3(0, -1, 0));
        mCrystalTransform->setAttitude(q);

        _updateCrystalColor();
    }

    void CrystalRingButton::_updateCrystalColor()
    {
        if(mColorModifierUniform == nullptr)
        {
            return;
        }

        // determine color change via lerp
        osg::Vec4 crystalColor = mCrystalColorInactive*(1-mCrystalSpeedPercent) + mCrystalColorActive*mCrystalSpeedPercent;
        mColorModifierUniform->set(crystalColor);
    }

}
