/*
 * Model.cpp
 *
 *  Created on: 2 Feb 2018
 *      Author: zal
 */

#include "db/Model.h"

#include <algorithm>
#include <limits>
#include <osg/Geometry>
#include <osg/LOD>
#include <osg/FrontFace>

#include "db/Asset.h"
#include "Exception.h"
#include "db/Database.h"
#include "OdDefines.h"
#include "db/ModelFactory.h"
#include "db/Texture.h"
#include "db/Skeleton.h"

#define OD_POLYGON_FLAG_DOUBLESIDED 0x02

namespace od
{

	Model::Model(Database &db, RecordId modelId)
	: Asset(db, modelId)
	, mModelName("")
	, mVerticesLoaded(false)
	, mTexturesLoaded(false)
	, mPolygonsLoaded(false)
	{
	}

	void Model::loadNameAndShading(ModelFactory &factory, DataReader &&dr)
	{
		dr >> mModelName;

		uint32_t shadingType;
		dr >> shadingType;
	}

	void Model::loadVertices(ModelFactory &factory, DataReader &&dr)
	{
		uint16_t vertexCount;
		dr >> vertexCount;

		mVertices.reserve(vertexCount);
		for(size_t i = 0; i < vertexCount; ++i)
		{
			osg::Vec3f vertex;

			dr >> vertex;

			mVertices.push_back(vertex);
		}

		mVerticesLoaded = true;
	}

	void Model::loadTextures(ModelFactory &factory, DataReader &&dr)
	{
		uint32_t textureCount;
		dr >> textureCount;

		mTextureRefs.reserve(textureCount);
		for(size_t i = 0; i < textureCount; ++i)
		{
			AssetRef textureRef;
			dr >> textureRef;

			if(!textureRef.isNullTexture())
			{
				mTextureRefs.push_back(textureRef);
			}
		}

		mTexturesLoaded = true;
	}

	void Model::loadPolygons(ModelFactory &factory, DataReader &&dr)
	{
		if(!mTexturesLoaded || !mVerticesLoaded)
		{
			throw Exception("Must load vertices and textures before loading polygons!");
		}

		uint16_t polygonCount;
		dr >> polygonCount;

		mPolygons.reserve(polygonCount);
		for(size_t i = 0; i < polygonCount; ++i)
		{
			uint16_t flags;
			uint16_t textureIndex;
			uint16_t vertexCount;

			dr >> flags
			   >> vertexCount
			   >> textureIndex;

			Polygon poly;
			poly.doubleSided = flags & OD_POLYGON_FLAG_DOUBLESIDED;
			poly.texture = mTextureRefs[textureIndex];
			poly.vertexCount = vertexCount;

			if(poly.vertexCount != 3 && poly.vertexCount != 4)
			{
				throw UnsupportedException("Can't load model with non-triangle/non-quad primitives");
			}

			for(size_t i = 0; i < poly.vertexCount; ++i)
			{
				uint16_t vertexIndex;

				dr >> vertexIndex
				   >> poly.uvCoords[i];

				poly.vertexIndices[i] = vertexIndex; // TODO: instead of doing it this way, maybe add a "readAs<type>" modifier?
			}

			mPolygons.push_back(poly);
		}

		mPolygonsLoaded = true;
	}

	void Model::loadLodsAndBones(ModelFactory &factory, DataReader &&dr)
	{
	    mVertexAffections.resize(mVertices.size());

		uint16_t lodCount;
		std::vector<std::string> lodNames;

		dr >> DataReader::Ignore(16*4) // bounding info (16 floats)
		   >> lodCount;

		if(lodCount == 0)
		{
			throw Exception("Expected at least one LOD in model");
		}

		lodNames.resize(lodCount - 1);
		for(int32_t i = 0; i < lodCount - 1; ++i)
		{
			dr >> lodNames[i];
		}


		mSkeletonBuilder.reset(new SkeletonBuilder);

		// node info
		uint16_t nodeInfoCount;
		dr >> nodeInfoCount;
		for(size_t i = 0; i < nodeInfoCount; ++i)
		{
			char nodeName[33] = { 0 };
			int32_t jointInfoIndex;

			dr.read(nodeName, 32);
			dr >> jointInfoIndex;

			mSkeletonBuilder->addBoneNode(std::string(nodeName), jointInfoIndex);
		}

		// joint info
		uint16_t jointInfoCount;
		dr >> jointInfoCount;
		for(size_t jointIndex = 0; jointIndex < jointInfoCount; ++jointIndex)
		{
			osg::Matrixf inverseBoneTransform;
			int32_t meshIndex;
            int32_t firstChildIndex;
            int32_t nextSiblingIndex;

            dr >> inverseBoneTransform
			   >> meshIndex
			   >> firstChildIndex
			   >> nextSiblingIndex;

            mSkeletonBuilder->addJointInfo(inverseBoneTransform, meshIndex, firstChildIndex, nextSiblingIndex);

            // affected vertex lists, one for each LOD
            for(size_t lodIndex = 0; lodIndex < lodCount; ++lodIndex)
            {
				uint16_t affectedVertexCount;
				dr >> affectedVertexCount;
				for(size_t vertexIndex = 0; vertexIndex < affectedVertexCount; ++vertexIndex)
				{
					uint32_t affectedVertexIndex;
					float weight;
					dr >> affectedVertexIndex
					   >> weight;

					if(lodIndex != 0) // for now, use only first lod as we don't have lod info yet
					{
						continue;
					}

					if(affectedVertexIndex >= mVertexAffections.size())
					{
						throw Exception("Affected vertex's index in bone data out of bounds");
					}

					BoneAffection &vAff = mVertexAffections[affectedVertexIndex];

					if(vAff.affectingBoneCount >= 4) // ignore any bones past the fourth. there should be none anyways
					{
						continue;
					}

					vAff.boneIndices[vAff.affectingBoneCount] = jointIndex;
					vAff.boneWeights[vAff.affectingBoneCount] = weight;
					vAff.affectingBoneCount++;
				}
            }
		}

		// lod info
		mLodMeshInfos.resize(lodCount);
		for(size_t lodIndex = 0; lodIndex < lodCount; ++lodIndex)
		{
			uint16_t meshCount;
			dr >> meshCount;

			if(meshCount != 1)
			{
				throw UnsupportedException("Multi-mesh-models currently unsupported");
			}

			for(size_t meshIndex = 0; meshIndex < meshCount; ++meshIndex)
			{
				LodMeshInfo &mesh = mLodMeshInfos[lodIndex];

				dr >> mesh.distanceThreshold
				   >> mesh.usage
				   >> mesh.nodeIndex
				   >> mesh.firstVertexIndex
				   >> mesh.vertexCount
				   >> mesh.firstPolygonIndex
				   >> mesh.polygonCount;

				mesh.lodName = (lodIndex == 0) ? mModelName : lodNames[lodIndex - 1];
			}
		}


		// animations refs
		uint16_t animCount;
		dr >> animCount;
		mAnimationRefs.resize(animCount);
		for(size_t animIndex = 0; animIndex < animCount; ++animIndex)
		{
			// the database index of this is always zero. however, i see no reason not to support loading
			//  animations cross-database, so we don't enforce this.
			dr >> mAnimationRefs[animIndex];
		}


		// channels
		uint16_t channelCount;
		dr >> channelCount;
		for(size_t channelIndex = 0; channelIndex < channelCount; ++channelIndex)
		{
			uint32_t nodeIndex;
			osg::Matrixf xformA;
			osg::Matrixf xformB;
            uint16_t capCount;

            dr >> nodeIndex
			   >> xformA
			   >> xformB
			   >> capCount;

            mSkeletonBuilder->makeChannel(nodeIndex);

            for(size_t capIndex = 0; capIndex < capCount; ++capIndex)
            {
            	uint32_t firstCapFaceIndex;
                uint32_t capFaceCount;
                uint32_t firstPartFaceIndex;
                uint32_t partFaceCount;
                uint32_t unk;
                uint16_t vertexCount;

                dr >> firstCapFaceIndex
				   >> capFaceCount
				   >> firstPartFaceIndex
				   >> partFaceCount
				   >> unk
				   >> vertexCount;

				for(size_t vertexIndex = 0; vertexIndex < vertexCount; ++vertexIndex)
				{
					uint32_t affectedVertexIndex;
					float weight;
					dr >> affectedVertexIndex
					   >> weight;
				}
            }
		}
 	}

	void Model::buildGeometry()
	{
		if(!mTexturesLoaded || !mVerticesLoaded || !mPolygonsLoaded)
		{
			throw Exception("Must load at least vertices, textures and polygons before building geometry");
		}

		if(mLodMeshInfos.size() > 0)
		{
			osg::ref_ptr<osg::LOD> lodNode(new osg::LOD);
			lodNode->setRangeMode(osg::LOD::DISTANCE_FROM_EYE_POINT);
			lodNode->setCenterMode(osg::LOD::USE_BOUNDING_SPHERE_CENTER);

			for(auto it = mLodMeshInfos.begin(); it != mLodMeshInfos.end(); ++it)
			{
				GeodeBuilder gb(getDatabase());

				// FIXME: LOD meshes have holes. somehow we don't cover all faces here. gotta be something with those "LOD caps"
				auto verticesBegin = mVertices.begin() + it->firstVertexIndex;
				auto verticesEnd = mVertices.begin() + it->vertexCount + it->firstVertexIndex;
				gb.setVertexVector(verticesBegin, verticesEnd);

				auto polygonsBegin = mPolygons.begin() + it->firstPolygonIndex;
				auto polygonsEnd = mPolygons.begin() + it->polygonCount + it->firstPolygonIndex;
				gb.setPolygonVector(polygonsBegin, polygonsEnd);

				osg::ref_ptr<osg::Geode> newGeode(new osg::Geode);
				gb.build(newGeode);

				float minDistance = it->distanceThreshold;
				float maxDistance = ((it+1) == mLodMeshInfos.end()) ? std::numeric_limits<float>::max() : (it+1)->distanceThreshold;
				lodNode->addChild(newGeode, minDistance, maxDistance);
			}

			this->addChild(lodNode);

		}else
		{
			GeodeBuilder gb(getDatabase());
			gb.setVertexVector(mVertices.begin(), mVertices.end());
			gb.setPolygonVector(mPolygons.begin(), mPolygons.end());

			osg::ref_ptr<osg::Geode> newGeode(new osg::Geode);
			gb.build(newGeode);

			this->addChild(newGeode);
		}

        // model faces are oriented CW for some reason
        this->getOrCreateStateSet()->setAttribute(new osg::FrontFace(osg::FrontFace::CLOCKWISE), osg::StateAttribute::ON);
	}
}

