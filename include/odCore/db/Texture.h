/*
 * Texture.h
 *
 *  Created on: 1 Feb 2018
 *      Author: zal
 */

#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <memory>
#include <vector>

#include <odCore/SrscFile.h>
#include <odCore/WeakRefPtr.h>

#include <odCore/db/Asset.h>
#include <odCore/db/Class.h>

namespace odRender
{
    class Image;
    class Texture;
}

namespace odDb
{

	class TextureFactory;

    class Texture : public Asset
    {
    public:

        Texture(AssetProvider &ap, od::RecordId id, TextureFactory &factory);
        ~Texture();

        inline uint32_t getWidth() const { return mWidth; }
        inline uint32_t getHeight() const { return mHeight; }
        inline uint8_t *getRawR8G8B8A8Data() { return mRgba8888Data.get(); }
        inline bool hasAlpha() const { return mHasAlphaChannel; };

        /**
         * Returns whether this has the "next frame" flag set (is part of a texture animation).
         * Sadly, there is no way to tell whether a texture is animated, how many frames it has
         * and which frames are part of the animation without loading all textures following the
         * one in question and checking if they are "next frame"-textures.
         */
        inline bool isNextFrame() const { return mIsNextFrame; }

        inline bool isAnimation() const { return mAnimFrameCount > 1; }
        inline size_t getAnimationFrameCount() const { return mAnimFrameCount; }

        void exportToPng(const od::FilePath &path);

        virtual void load(od::SrscFile::RecordInputCursor cursor) override;
        virtual void postLoad() override;

        /**
         * Provides an odRender::Image object that can be used to create Texture objects.
         *
         * An Image encapsulates pixel data storage, so pixel data may be shared between multiple textures.
         */
        od::RefPtr<odRender::Image> getRenderImage(odRender::Renderer *renderer);


    private:

        void _loadFromRecord(od::DataReader &dr);
        unsigned char _filter16BitChannel(uint16_t color, uint32_t mask, uint32_t shift);

        TextureFactory &mTextureFactory;

        uint32_t mWidth;
        uint32_t mHeight;
        uint16_t mBitsPerPixel;
        uint16_t mAlphaBitsPerPixel;
        uint32_t mColorKey;
        AssetRef mNextMipMapRef;
        AssetRef mAlternateRef;
        AssetRef mBumpMapRef;
        uint8_t mAnimationFps;
        uint8_t mFlags;
        uint16_t mMipMapNumber;
        AssetRef mMaterialClassRef;
        uint32_t mUsageCount;
        uint32_t mCompressionLevel;
        uint32_t mCompressedSize;

        bool mIsNextFrame;

        size_t mAnimFrameCount;

        bool mHasAlphaChannel;
        od::RefPtr<Class> mMaterialClass;
        std::unique_ptr<odRfl::RflClass> mMaterialInstance;

        std::unique_ptr<uint8_t[]> mRgba8888Data;

        od::WeakObserverRefPtr<odRender::Image> mRenderImage;
    };

    template <>
    struct AssetTraits<Texture>
    {
        static const char *name()
        {
            return "Texture";
        }
    };

}




#endif /* TEXTURE_H_ */
