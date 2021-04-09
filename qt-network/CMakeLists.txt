find_package(Qt5 COMPONENTS Core Gui Network Widgets)
set(CMAKE_AUTOMOC ON)

add_executable("simple-server" "simple_server.cpp")
target_link_libraries("simple-server" Qt5::Core Qt5::Network)

add_subdirectory(message)

if (WIN32)
	add_custom_command(TARGET "simple-server" POST_BUILD COMMAND ${Qt5_DIR}/../../../bin/windeployqt $<TARGET_FILE:simple-server>)
endif (WIN32)
