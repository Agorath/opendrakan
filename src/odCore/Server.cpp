/*
 * Server.cpp
 *
 *  Created on: Sep 26, 2019
 *      Author: zal
 */

#include <odCore/Server.h>

#include <thread>
#include <chrono>

#include <odCore/Level.h>

#include <odCore/physics/bullet/BulletPhysicsSystem.h>

#include <odCore/rfl/RflManager.h>

namespace od
{

    Server::Server(odDb::DbManager &dbManager, odRfl::RflManager &rflManager)
    : mDbManager(dbManager)
    , mRflManager(rflManager)
    , mHasInitialLevelOverride(false)
    , mIsDone(false)
    {
        mPhysicsSystem = std::make_unique<odBulletPhysics::BulletPhysicsSystem>(nullptr);
    }

    Server::~Server()
    {
    }

    void Server::loadLevel(const od::FilePath &levelPath)
    {

    }

    void Server::run()
    {
        Logger::info() << "OpenDrakan server starting...";

        Logger::info() << "Server set up. Starting main server loop";

        float targetUpdateIntervalUs = (1e6/60.0);
        auto targetUpdateInterval = std::chrono::microseconds((int64_t)targetUpdateIntervalUs);
        auto lastUpdateStartTime = std::chrono::high_resolution_clock::now();
        while(!mIsDone.load(std::memory_order_relaxed))
        {
            auto loopStart = std::chrono::high_resolution_clock::now();

            double relTime = 1e-6 * std::chrono::duration_cast<std::chrono::microseconds>(loopStart - lastUpdateStartTime).count();
            lastUpdateStartTime = loopStart;

            if(mLevel != nullptr)
            {
                mLevel->update(relTime);
            }

            mPhysicsSystem->update(relTime);

            auto loopEnd = std::chrono::high_resolution_clock::now();
            auto loopTime = loopEnd - loopStart;
            if(loopTime < targetUpdateInterval)
            {
                std::this_thread::sleep_for(targetUpdateInterval - loopTime);

            }else
            {
                float loopTimeMs = 1e-3 * std::chrono::duration_cast<std::chrono::microseconds>(loopTime).count();
                Logger::warn() << "Server tick took too long (" << loopTimeMs << "ms, target was " << (targetUpdateIntervalUs*1e-3) << "ms)";
            }
        }

        Logger::info() << "Shutting down server gracefully";
    }

}


