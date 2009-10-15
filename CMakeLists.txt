
cmake_minimum_required(VERSION 2.6)
project(PeakEngine)

if(WIN32)
	set(LUA_INCLUDE_DIR C:\\Lua\\5.1\\include)
	set(LUA_LIBRARIES lua51)
	set(SDL_INCLUDE_DIR C:/MinGW/include/SDL)
	add_definitions(-DGUICHAN_DLL_IMPORT)
else(WIN32)
	find_package(Lua51)
	find_package(SDL)
endif(WIN32)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-unused-parameter")

set(ENGINE_SRC
src/core/Client.cpp
src/core/Game.cpp
src/core/Engine.cpp
src/core/Server.cpp
src/entity/ClientEntity.cpp
src/entity/Entity.cpp
src/entity/EntityManager.cpp
src/entity/EntityMap.cpp
src/entity/FloatProperty.cpp
src/entity/Property.cpp
src/entity/ServerEntity.cpp
src/entity/Vector2FProperty.cpp
src/entity/Vector3FProperty.cpp
src/entity/IntProperty.cpp
src/network/LocalConnection.cpp
src/support/Buffer.cpp
src/support/Mutex.cpp
src/support/OS.cpp
src/support/ScopedLock.cpp
src/support/Thread.cpp
)

include_directories(include ${LUA_INCLUDE_DIR} ${SDL_INCLUDE_DIR})

add_library(PeakEngine SHARED ${ENGINE_SRC})
if(WIN32)
	target_link_libraries(PeakEngine SDL SDL_image SDL_mixer guichan_opengl guichan_sdl guichan opengl32 glu32 glew32 enet ws2_32 winmm luabindd ${LUA_LIBRARIES})
else(WIN32)
	target_link_libraries(PeakEngine SDL SDL_image SDL_mixer guichan_opengl guichan_sdl guichan GL glut GLEW enet luabind ${LUA_LIBRARIES})
endif(WIN32)

install(DIRECTORY include/peakengine/ DESTINATION include/peakengine FILES_MATCHING PATTERN *.hpp)
install(FILES include/PeakEngine.hpp DESTINATION include)
install(TARGETS PeakEngine LIBRARY DESTINATION lib)

set(CMAKE_BUILD_TYPE debug)
