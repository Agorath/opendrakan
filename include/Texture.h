/*
 * Texture.h
 *
 *  Created on: 1 Feb 2018
 *      Author: zal
 */

#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <osg/Image>

#include "SrscFile.h"
#include "Asset.h"

namespace od
{

	class TextureFactory;

    class Texture : public Asset, public osg::Image
    {
    public:

        Texture(RecordId id);

        void loadFromRecord(TextureFactory &factory, DataReader dr);

        void exportToPng(const FilePath &path);

        // implement Asset
        virtual const char *getAssetTypeName() const { return "texture"; }

        // override osg::Image
        virtual const char *libraryName() const { return "od";    }
        virtual const char *className()   const { return "Texture"; }


    private:

        uint32_t mWidth;
        uint32_t mHeight;
        uint16_t mBitsPerPixel;
        uint16_t mAlphaBitsPerPixel;
        uint32_t mColorKey;
        RecordId mMipMapId;
        RecordId mAlternateId;
        uint16_t mFlags;
        uint16_t mMipMapNumber;
        AssetRef mClassRef;
        uint32_t mUsageCount;
        uint32_t mCompressionLevel;
        uint32_t mCompressedSize;
    };

    typedef osg::ref_ptr<Texture> TexturePtr;
}




#endif /* TEXTURE_H_ */
