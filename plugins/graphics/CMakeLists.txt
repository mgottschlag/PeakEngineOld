
set(SRC
src/CameraSceneNode.cpp
src/Graphics.cpp
src/GroupSceneNode.cpp
src/ModelSceneNode.cpp
src/RootSceneNode.cpp
src/SceneNode.cpp
)

include_directories(include ../../include)

add_library(PeakGraphics SHARED ${SRC})
target_link_libraries(PeakGraphics PeakEngine Lightfeather)

install(DIRECTORY include/peakgraphics/ DESTINATION include/peakgraphics FILES_MATCHING PATTERN *.hpp)
install(FILES include/PeakGraphics.hpp DESTINATION include)
install(TARGETS PeakGraphics LIBRARY DESTINATION lib)

set(CMAKE_BUILD_TYPE debug)
