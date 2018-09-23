
# config for odCore

set(SOURCES
        "anim/Animator.cpp"
        "anim/MotionAnimator.cpp"
        "anim/SkeletonAnimationPlayer.cpp"
        "audio/SoundManager.cpp"
        "audio/Source.cpp"
        "db/Animation.cpp"
        "db/AnimationFactory.cpp"
        "db/Asset.cpp"
        "db/AssetProvider.cpp"
        "db/Class.cpp"
        "db/ClassFactory.cpp"
        "db/Database.cpp"
        "db/DbManager.cpp"
        "db/Model.cpp"
        "db/ModelFactory.cpp"
        "db/Sequence.cpp"
        "db/SequenceFactory.cpp"
        "db/Skeleton.cpp"
        "db/Sound.cpp"
        "db/SoundFactory.cpp"
        "db/Texture.cpp"
        "db/TextureFactory.cpp"
        "gui/ContainerWidget.cpp"
        "gui/GuiManager.cpp"
        "gui/TexturedQuad.cpp"
        "gui/Widget.cpp"
        "gui/WidgetGroup.cpp"
        "gui/WidgetIntersectVisitor.cpp"
        "light/Light.cpp"
        "light/LightManager.cpp"
        "light/LightState.cpp"
        "physics/BulletCallbacks.cpp"
        "physics/CharacterController.cpp"
        "physics/DebugDrawer.cpp"
        "physics/ModelBounds.cpp"
        "physics/PhysicsManager.cpp"
        "rfl/ClassBuilderProbe.cpp"
        "rfl/FieldProbe.cpp"
        "rfl/PrefetchProbe.cpp"
        "rfl/Rfl.cpp"
        "rfl/RflClass.cpp"
        "rfl/RflField.cpp"
        "rfl/RflManager.cpp"
        "rfl/RflMessage.cpp"
        "DataStream.cpp"
        "Engine.cpp"
        "Exception.cpp"
        "FilePath.cpp"
        "GeodeBuilder.cpp"
        "InputManager.cpp"
        "Layer.cpp"
        "Level.cpp"
        "LevelObject.cpp"
        "Logger.cpp"
        "OsgSerializers.cpp"
        "SegmentedGeode.cpp"
        "ShaderManager.cpp"
        "SrscFile.cpp"
        "StringUtils.cpp"
        "TextureAtlas.cpp"
        "UpdateCallback.cpp"
        "ZStream.cpp")
    
    
add_library(odCore SHARED ${SOURCES})

target_link_libraries(odCore ${OPENSCENEGRAPH_LIBRARIES} ${ZLIB_LIBRARIES} ${BULLET_LIBRARIES} ${OPENAL_LIBRARY})
target_include_directories(odCore PRIVATE ${OPENSCENEGRAPH_INCLUDE_DIRS} ${ZLIB_INCLUDE_DIRS} ${BULLET_INCLUDE_DIRS} ${OPENAL_INCLUDE_DIR})
    
    