/*
 * Model.cpp
 *
 *  Created on: May 15, 2019
 *      Author: zal
 */

#include <odOsg/render/Model.h>

#include <algorithm>

#include <odCore/Exception.h>
#include <odCore/Downcast.h>

namespace odOsg
{

    Model::Model()
    : mGeode(new osg::Geode)
    {
    }

    size_t Model::getGeometryCount()
    {
        return mGeometries.size();
    }

    odRender::Geometry *Model::getGeometry(size_t index)
    {
        if(index >= mGeometries.size())
        {
            throw od::Exception("Geometry index out of bounds");
        }

        return mGeometries[index];
    }

    void Model::addGeometry(odRender::Geometry *g)
    {
        if(g == nullptr)
        {
            return;
        }

        auto geo = od::confident_downcast<Geometry>(g);

        mGeometries.emplace_back(g);

        mGeode->addDrawable(geo->getOsgGeometry());
    }

    void Model::removeGeometry(odRender::Geometry *g)
    {
        auto it = std::find(mGeometries.begin(), mGeometries.end(), g);
        if(it != mGeometries.end())
        {
            mGeode->removeDrawable((*it)->getOsgGeometry());

            mGeometries.erase(it);
        }
    }

    bool Model::geometriesShareVertexData()
    {
        return false;
    }

    void Model::setLightingMode(odRender::LightingMode lm)
    {
        osg::StateSet *ss = mGeode->getOrCreateStateSet();

        switch(lm)
        {
        case odRender::LightingMode::OFF:
            ss->removeDefine("LIGHTING");
            ss->removeDefine("SPECULAR");
            break;

        case odRender::LightingMode::AMBIENT_DIFFUSE:
            ss->setDefine("LIGHTING");
            ss->removeDefine("SPECULAR");
            break;

        case odRender::LightingMode::AMBIENT_DIFFUSE_SPECULAR:
            ss->setDefine("LIGHTING");
            ss->setDefine("SPECULAR");
            break;
        }
    }

}

