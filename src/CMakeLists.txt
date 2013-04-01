add_executable(WayOfLife mapinfo.cpp fileloader.cpp soundsystem.cpp gameevent.cpp eventmanager.cpp tile.cpp ../gorilla/Gorilla.cpp gameengine.cpp 
	baseapplication.cpp)

target_link_libraries(WayOfLife ${OGRE_LIBRARIES} ${OIS_LIBRARIES} ${OPENAL_LIBRARIES})


