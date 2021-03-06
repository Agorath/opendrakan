/*
 * Gui.cpp
 *
 *  Created on: 21 Dec 2018
 *      Author: zal
 */

#include <odCore/gui/Gui.h>

#include <algorithm>

#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <odCore/Logger.h>
#include <odCore/SrscRecordTypes.h>
#include <odCore/Engine.h>

#include <odCore/render/Renderer.h>

#include <odCore/gui/Widget.h>

namespace odGui
{

    Gui::Gui(odRender::Renderer &renderer)
    : mRenderer(renderer)
    , mMenuMode(false)
    {
        _setupGui();
    }

    Gui::~Gui()
    {
    }

    void Gui::quit()
    {
    }

    void Gui::mouseDown()
    {
        if(!mMenuMode)
        {
            return;
        }

        mCurrentHitWidgets.clear();
        glm::mat4 eye(1.0);
        mRootWidget->intersect(mCursorPosInNdc, eye, mCurrentHitWidgets);

        Logger::debug() << "Hit " << mCurrentHitWidgets.size() << " widgets!";

        for(auto it = mCurrentHitWidgets.begin(); it != mCurrentHitWidgets.end(); ++it)
        {
            it->widget->onMouseDown(it->hitPointInWidget, 0);
        }
    }

    void Gui::addWidget(Widget *widget)
    {
        if(widget == nullptr)
        {
            return;
        }

        mRootWidget->addChild(widget);
    }

    void Gui::removeWidget(Widget *widget)
    {
        if(widget == nullptr)
        {
            return;
        }

        mRootWidget->removeChild(widget);
    }

    void Gui::setMenuMode(bool b)
    {
        mMenuMode = b;

        if(mCursorWidget != nullptr)
        {
            mCursorWidget->setVisible(b);
        }

        this->onMenuModeChanged();
    }

    void Gui::setCursorWidget(Widget *cursor)
    {
        if(mCursorWidget != nullptr)
        {
            this->removeWidget(mCursorWidget);
        }

        mCursorWidget = cursor;
        mCursorWidget->setZIndex(-1000);
        mCursorWidget->setVisible(mMenuMode);
        this->addWidget(mCursorWidget);

        glm::vec4 widgetToNdc = mWidgetSpaceToNdcTransform * glm::vec4(mCursorWidget->getPosition(), 0.0, 1.0);
        mCursorPosInNdc = glm::vec2(widgetToNdc.x, widgetToNdc.y);
    }

    void Gui::setCursorPosition(const glm::vec2 &pos)
    {
        if(mCursorPosInNdc == pos)
        {
            // no change -> no need to perform costly update
            return;
        }

        mCursorPosInNdc = pos;

        // pos is in NDC, we need it in widget space
        glm::vec4 posWs = mNdcToWidgetSpaceTransform * glm::vec4(pos, 0.0, 1.0);

        if(mCursorWidget != nullptr)
        {
            mCursorWidget->setPosition(posWs.x, posWs.y);
        }

        if(!mMenuMode)
        {
            return;
        }

        // okay, here is the algorithm for determining mouse enter/leave:
        //  whenever the mouse moves, we find all widgets under it. we also store all widgets found under the
        //  cursor the last time the mouse moved. now, as the mouse moved, we join the two lists. all widgets
        //  that are unique in that list were either entered or left by the cursor. which one of those events
        //  happened can be determined from the mouse-over state that is stored in those widgets.
        //  FIXME: this only works if every widget in the scenegraph is unique
        mCurrentHitWidgets.clear();
        glm::mat4 eye(1.0);
        mRootWidget->intersect(mCursorPosInNdc, eye, mCurrentHitWidgets);

        mJoinedHitWidgets.clear();
        mJoinedHitWidgets.insert(mJoinedHitWidgets.end(), mCurrentHitWidgets.begin(), mCurrentHitWidgets.end());
        mJoinedHitWidgets.insert(mJoinedHitWidgets.end(), mLastHitWidgets.begin(), mLastHitWidgets.end());

        // find unique widgets in joined list. NOTE: since the apparently is no nice way to do this via STL,
        //  we do it manually using a sort and by iterating over each widget.
        auto pred = [](HitWidgetInfo &a, HitWidgetInfo &b){ return a.widget < b.widget; };
        std::sort(mJoinedHitWidgets.begin(), mJoinedHitWidgets.end(), pred);
        for(auto it = mJoinedHitWidgets.begin(); it != mJoinedHitWidgets.end(); ++it)
        {
            HitWidgetInfo &current = *it;

            // if we ever reach the last element, we know it is unique and need no further checks
            if(it+1 != mJoinedHitWidgets.end())
            {
                HitWidgetInfo &next = *(it+1);
                if(current.widget == next.widget)
                {
                    ++it;
                    continue;
                }
            }

            // widget seems unique -> mouse either left it or entered it
            if(current.widget->isMouseOver())
            {
                current.widget->setMouseOver(false);
                current.widget->onMouseLeave(current.hitPointInWidget);

            }else
            {
                current.widget->setMouseOver(true);
                current.widget->onMouseEnter(current.hitPointInWidget);
            }
        }

        std::swap(mCurrentHitWidgets, mLastHitWidgets);
    }

    void Gui::onMenuModeChanged()
    {
    }

    void Gui::_setupGui()
    {
        mWidgetSpaceToNdcTransform = glm::mat4(1.0);
        mWidgetSpaceToNdcTransform = glm::scale(mWidgetSpaceToNdcTransform, glm::vec3(2.0, -2.0, 1.0));
        mWidgetSpaceToNdcTransform = glm::translate(mWidgetSpaceToNdcTransform, glm::vec3(-0.5, -0.5, 0.0));

        mNdcToWidgetSpaceTransform = glm::inverse(mWidgetSpaceToNdcTransform);

        mRootWidget = od::make_refd<Widget>(*this, mRenderer.getGuiRootNode());
        mRootWidget->setDimensions(glm::vec2(1920, 1080), WidgetDimensionType::Pixels); // FIXME: get actual dimensions here
        mRootWidget->update(0); // the root GuiNode has no reference to the root widget, so it can't trigger the initial update itself
    }

}
