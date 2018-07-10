/*
 * Widget.cpp
 *
 *  Created on: 24 Jun 2018
 *      Author: zal
 */

#include "gui/Widget.h"

#include <osg/Geometry>

#include "NodeMasks.h"
#include "gui/GuiManager.h"

namespace od
{

    Widget::Widget(GuiManager &gm)
    : mGuiManager(gm)
    , mOrigin(WidgetOrigin::TopLeft)
    , mPositionInParentSpace(0.0, 0.0)
    , mZIndexInParentSpace(0)
    , mParentWidget(nullptr)
    {
        this->setNodeMask(NodeMasks::Gui);
        this->setMatrix(osg::Matrix::identity());
    }

    osg::Vec2 Widget::getDimensionsInPixels()
    {
        if(this->getDimensionType() == WidgetDimensionType::Pixels)
        {
            return this->getDimensions();

        }else
        {
            return osg::componentMultiply(this->getParentDimensionsInPixels(), this->getDimensions());
        }
    }

    osg::Vec2 Widget::getParentDimensionsInPixels()
    {
        return (mParentWidget != nullptr) ? mParentWidget->getDimensionsInPixels() : mGuiManager.getScreenResolution();
    }

    std::pair<int32_t, int32_t> Widget::getParentZRange()
    {
        return (mParentWidget != nullptr) ? mParentWidget->getZRange() : mGuiManager.getWidgetZRange();
    }

    void Widget::setOrigin(WidgetOrigin origin)
    {
        mOrigin = origin;

        _updateMatrix();
    }

    void Widget::setPosition(const osg::Vec2 &pos)
    {
        mPositionInParentSpace = pos;

        _updateMatrix();
    }

    void Widget::setZIndex(int32_t z)
    {
        mZIndexInParentSpace = z;

        _updateMatrix();
    }

    void Widget::setVisible(bool b)
    {
        this->setNodeMask(b ? NodeMasks::Gui : NodeMasks::Hidden);
    }

    void Widget::setParent(Widget *parent)
    {
        mParentWidget = parent;

        _updateMatrix();
    }

    void Widget::_updateMatrix()
    {
        osg::Matrix matrix = osg::Matrix::identity();

        osg::Vec2 origin;
        switch(mOrigin)
        {
        case WidgetOrigin::TopRight:
            origin = osg::Vec2(1.0, 0.0);
            break;

        case WidgetOrigin::BottomRight:
            origin = osg::Vec2(1.0, 1.0);
            break;

        case WidgetOrigin::BottomLeft:
            origin = osg::Vec2(0.0, 1.0);
            break;

        case WidgetOrigin::Center:
            origin = osg::Vec2(0.5, 0.5);
            break;

        case WidgetOrigin::TopLeft:
        default:
            origin = osg::Vec2(0.0, 0.0);
            break;
        }

        matrix.postMultTranslate(osg::Vec3(-origin, 0.0));

        osg::Vec2 widgetSizeInParentSpace = osg::componentDivide(this->getDimensionsInPixels(), this->getParentDimensionsInPixels());
        matrix.postMultScale(osg::Vec3(widgetSizeInParentSpace, 1.0));

        matrix.postMultTranslate(osg::Vec3(mPositionInParentSpace, 0.0));

        std::pair<int32_t, int32_t> parentZRange = this->getParentZRange();
        float newZPosition = (mZIndexInParentSpace + parentZRange.first)/(parentZRange.second - parentZRange.first + 0.1);
        matrix.postMultTranslate(osg::Vec3(0.0, 0.0, newZPosition));

        this->setMatrix(matrix);
    }

}
