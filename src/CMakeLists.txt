add_executable(WayOfLife tile.cpp ../gorilla/Gorilla.cpp tilesetmanager.cpp gameengine.cpp 
	baseapplication.cpp)

target_link_libraries(WayOfLife ${OGRE_LIBRARIES} ${OIS_LIBRARIES})


