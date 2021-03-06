/*
 * Renderer.h
 *
 *  Created on: 12 Nov 2018
 *      Author: zal
 */

#ifndef INCLUDE_ODOSG_RENDERER_H_
#define INCLUDE_ODOSG_RENDERER_H_

#include <thread>
#include <mutex>
#include <vector>
#include <memory>

#include <osg/Group>
#include <osg/Uniform>
#include <osgViewer/Viewer>

#include <odCore/RefCounted.h>
#include <odCore/BoundingSphere.h>

#include <odCore/render/Renderer.h>

#include <odOsg/render/ShaderFactory.h>

namespace od
{
    class Light;
}

namespace odOsg
{
    class Texture;
    class Camera;
    class GuiNode;
    class Model;

    class Renderer : public odRender::Renderer
    {
    public:

        Renderer();
        ~Renderer();

        inline ShaderFactory &getShaderFactory() { return mShaderFactory; }
        inline osgViewer::Viewer *getViewer() { return mViewer; }

        virtual void onStart() override;
        virtual void onEnd() override;
        virtual void setRendererEventListener(odRender::RendererEventListener *listener) override;

        virtual void setEnableLighting(bool b) override;
        virtual bool isLightingEnabled() const override;

        virtual od::RefPtr<odRender::Handle> createHandle(odRender::RenderSpace space) override;
        virtual od::RefPtr<odRender::Model> createModel() override;
        virtual od::RefPtr<odRender::Geometry> createGeometry(odRender::PrimitiveType primitiveType, bool indexed) override;

        virtual od::RefPtr<odRender::Model> createModelFromDb(odDb::Model *model) override;
        virtual od::RefPtr<odRender::Model> createModelFromLayer(od::Layer *layer) override;

        virtual od::RefPtr<odRender::Image> createImage(odDb::Texture *dbTexture) override;
        virtual od::RefPtr<odRender::Texture> createTexture(odRender::Image *image) override;
        virtual od::RefPtr<odRender::GuiNode> createGuiNode(odGui::Widget *widget) override;
        virtual odRender::GuiNode *getGuiRootNode() override;

        virtual odRender::Camera *getCamera() override;

        void applyLayerLight(const osg::Matrix &viewMatrix, const osg::Vec3 &diffuse, const osg::Vec3 &ambient, const osg::Vec3 &direction);
        void applyToLightUniform(const osg::Matrix &viewMatrix, od::Light *light, size_t index);
        void applyNullLight(size_t index);

        void setFreeLook(bool f);


    private:

        void _setupGuiStuff();

        void _threadedRender();

        od::RefPtr<Model> _buildSingleLodModelNode(odDb::Model *model);
        od::RefPtr<Model> _buildMultiLodModelNode(odDb::Model *model);

        ShaderFactory mShaderFactory;
        std::thread mRenderThread;
        std::mutex mRenderMutex;

        odRender::RendererEventListener *mEventListener;

        od::RefPtr<Camera> mCamera;

        bool mFreeLook;

        osg::ref_ptr<osgViewer::Viewer> mViewer;
        osg::ref_ptr<osg::Group> mSceneRoot;
        osg::ref_ptr<osg::Group> mLevelRoot;

        osg::ref_ptr<osg::Camera> mGuiCamera;
        osg::ref_ptr<osg::Group> mGuiRoot;
        od::RefPtr<GuiNode> mGuiRootNode;

        bool mLightingEnabled;
        osg::ref_ptr<osg::Uniform> mGlobalLightDiffuse;
        osg::ref_ptr<osg::Uniform> mGlobalLightAmbient;
        osg::ref_ptr<osg::Uniform> mGlobalLightDirection;
        osg::ref_ptr<osg::Uniform> mLocalLightsColor;
        osg::ref_ptr<osg::Uniform> mLocalLightsRadius;
        osg::ref_ptr<osg::Uniform> mLocalLightsIntensity;
        osg::ref_ptr<osg::Uniform> mLocalLightsPosition;
    };

}

#endif /* INCLUDE_ODOSG_RENDERER_H_ */
