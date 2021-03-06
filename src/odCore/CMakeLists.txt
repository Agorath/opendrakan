
# config for odCore

set(SOURCES
        "anim/Skeleton.cpp"
        "anim/SkeletonAnimationPlayer.cpp"
        "db/Animation.cpp"
        "db/Asset.cpp"
        "db/AssetProvider.cpp"
        "db/Class.cpp"
        "db/ClassFactory.cpp"
        "db/Database.cpp"
        "db/DbManager.cpp"
        "db/Model.cpp"
        "db/ModelBounds.cpp"
        "db/Sequence.cpp"
        "db/SkeletonBuilder.cpp"
        "db/Sound.cpp"
        "db/Texture.cpp"
        "db/TextureFactory.cpp"
        "gui/Gui.cpp"
        "gui/Widget.cpp"
        "input/Action.cpp"
        "input/CursorListener.cpp"
        "input/InputManager.cpp"
        "physics/bullet/BulletCallbacks.cpp"
        "physics/bullet/BulletPhysicsSystem.cpp"        
        "physics/bullet/DebugDrawer.cpp"
        "physics/bullet/LayerHandleImpl.cpp"
        "physics/bullet/LightHandleImpl.cpp"
        "physics/bullet/ManagedCompoundShape.cpp"
        "physics/bullet/ModelShapeImpl.cpp"
        "physics/bullet/ObjectHandleImpl.cpp"
        "physics/CharacterController.cpp"
        "physics/Handles.cpp"
        "render/GuiQuad.cpp"
        "render/Renderer.cpp"
        "rfl/ClassBuilderProbe.cpp"
        "rfl/DefaultObjectClass.cpp"
        "rfl/Field.cpp"
        "rfl/FieldProbe.cpp"
        "rfl/ObjectBuilderProbe.cpp"
        "rfl/PrefetchProbe.cpp"
        "rfl/Rfl.cpp"
        "rfl/RflClass.cpp"
        "rfl/RflManager.cpp"
        "rfl/RflMessage.cpp"
        "BoundingBox.cpp"
        "BoundingSphere.cpp"
        "ConfigFile.cpp"
        "DataStream.cpp"
        "Engine.cpp"
        "Exception.cpp"
        "FilePath.cpp"
        "Layer.cpp"
        "Level.cpp"
        "LevelObject.cpp"
        "Light.cpp"
        "NuLogger.cpp"
        "ObjectLightReceiver.cpp"
        "RefCounted.cpp"
        "GlmSerializers.cpp"
        "SrscFile.cpp"
        "StringUtils.cpp"
        "ZStream.cpp")
    
find_package(Threads REQUIRED)
#find_package(glm 0.9.8.3 REQUIRED) FIXME: this breaks the travis build on ubuntu xenial. skip for now. build works without it
find_package(ZLIB REQUIRED)
find_package(Bullet 2.8.3 REQUIRED Collision LinearMath)
    
    
add_library(odCore STATIC ${SOURCES})

target_link_libraries(odCore
                        ${CMAKE_THREAD_LIBS_INIT}
                        ${ZLIB_LIBRARIES} 
                        ${BULLET_LIBRARIES})
                    
# since some glm versions (not necessarily older ones) require this:
target_compile_definitions(odCore PUBLIC GLM_ENABLE_EXPERIMENTAL)
                    
target_include_directories(odCore PRIVATE ${ZLIB_INCLUDE_DIRS})
target_include_directories(odCore PUBLIC ${GLM_INCLUDE_DIRS} ${BULLET_INCLUDE_DIRS})
    
    