/*
 * AssetFactory.h
 *
 *  Created on: 7 Feb 2018
 *      Author: zal
 */

#ifndef INCLUDE_ASSETFACTORY_H_
#define INCLUDE_ASSETFACTORY_H_

#include <map>

#include <odCore/FilePath.h>
#include <odCore/SrscFile.h>
#include <odCore/Logger.h>
#include <odCore/Exception.h>
#include <odCore/WeakRefPtr.h>

#include <odCore/db/Asset.h>

namespace odDb
{
	class AssetProvider;

	/**
	 * @brief Asset factory base that handles caching and loading.
	 *
	 * Instantiation is handled by implementing factories.
	 */
	template <typename _AssetType>
	class AssetFactory
	{
	public:

	    typedef _AssetType AssetType;

		AssetFactory(const AssetFactory &f) = delete;
		virtual ~AssetFactory() = default;

		inline AssetProvider &getAssetProvider() { return mAssetProvider; }
		inline od::SrscFile &getSrscFile() { return mSrscFile; }

		od::RefPtr<_AssetType> getAsset(od::RecordId assetId)
        {
            // we access the cache using the []-operator, so we create an entry for the asset if if not existed yet.
            //  since most of the time the assets we are looking for are either cached or can be loaded from the container,
            //  doing it this way will save us one traversal of the map, as we don't have to use insert() to add a newly loaded
            //  asset to the cache. for the rare case that we can not find an asset in the file, just erase the unnecessary entry.
            od::WeakObserverRefPtr<_AssetType> &asset = mAssetCache[assetId];
            if(!asset.isNull())
            {
                Logger::debug() << AssetTraits<_AssetType>::name() << " " << std::hex << assetId << std::dec << " found in cache";
                return asset.aquire();
            }

            // asset was not cached or got deleted. let implementation handle loading
            Logger::debug() << AssetTraits<_AssetType>::name() << " " << std::hex << assetId << std::dec << " not found in cache. Loading from container " << mSrscFile.getFilePath().fileStr();
            od::RefPtr<_AssetType> loaded = this->loadAsset(assetId);
            if(loaded == nullptr)
            {
                mAssetCache.erase(assetId);

                Logger::error() << AssetTraits<_AssetType>::name() << " " << std::hex << assetId << std::dec << " neither found in cache nor asset container " << mSrscFile.getFilePath().fileStr();
                throw od::NotFoundException("Asset not found in cache or asset container");
            }

            asset = loaded; // this effectively inserts the asset into the cache

            return loaded;
        }


	protected:

		AssetFactory(AssetProvider &ap, od::SrscFile &assetContainer)
        : mAssetProvider(ap)
        , mSrscFile(assetContainer)
        {
        }

		/**
         * @brief Interface method for asking child factories to create an assed if it wasn't found in cache.
         *
         * This method will get called by AssetFactory when an asset that is not yet cached is requested.
         * The implementing factory should instantiate a new asset with whatever parameters needed and return it.
         * nullptr should never be returned.
         *
         * Loading is handled by the parent AssetFactory.
         */
		virtual od::RefPtr<_AssetType> createNewAsset(od::RecordId id) = 0;

		od::RefPtr<_AssetType> loadAsset(od::RecordId id)
        {
            auto cursor = mSrscFile.getFirstRecordOfTypeId(AssetTraits<_AssetType>::baseType(), id);
            if(!cursor.isValid())
            {
                return nullptr;
            }

            auto newAsset = createNewAsset(id);
            newAsset->load(std::move(cursor));

		    // FIXME: in contrast to load(...), postLoad() is not synchronized
		    //  this is not a problem atm, as asset loading is not threaded. later we might want to add a second mutex or something
		    newAsset->postLoad();

		    return newAsset;
        }


	private:

		AssetProvider &mAssetProvider;
		od::SrscFile &mSrscFile;

		std::map<od::RecordId, od::WeakObserverRefPtr<_AssetType>> mAssetCache;
	};


	/**
	 * @brief Template for assets that don't need a special factory.
	 *
	 * This is useful if your asset only has the (AssetProvider, RecordId) constructor. Simply
	 * typedef this as your asset factory and your are good to go.
	 */
	template <typename _AssetType>
	class GenericAssetFactory : public AssetFactory<_AssetType>
	{
	public:

	    GenericAssetFactory(AssetProvider &ap, od::SrscFile &assetContainer)
        : AssetFactory<_AssetType>(ap, assetContainer)
        {
        }

	    virtual ~GenericAssetFactory() = default;


	protected:

	    virtual od::RefPtr<_AssetType> createNewAsset(od::RecordId id) override
	    {
	        return od::make_refd<_AssetType>(AssetFactory<_AssetType>::getAssetProvider(), id);
	    }

	};

}

#endif /* INCLUDE_ASSETFACTORY_H_ */
