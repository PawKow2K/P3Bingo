cmake_minimum_required(VERSION 3.19)
project(Bingo LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

find_package(Qt6 6.8 REQUIRED COMPONENTS Core Widgets Network)

qt_standard_project_setup()

qt_add_executable(Bingo
	WIN32 MACOSX_BUNDLE
	main.cpp
	mainwindow.cpp
	mainwindow.h
	mainwindow.ui

	game.h game.cpp
	tile.h
	tiledelegate.h tiledelegate.cpp
	app.qrc
	editlist.h editlist.cpp editlist.ui
	client.h client.cpp
	networkmessages.h networkmessages.cpp
	server.h
	server.cpp
	bingodef.h
	teamdata.h
	tile.cpp
	teamdata.cpp
	connectionipinput.h connectionipinput.cpp connectionipinput.ui
	hostportinput.ui
	hostportinput.h
	hostportinput.cpp
)

target_link_libraries(Bingo
	PRIVATE
		Qt::Core
		Qt::Widgets
		Qt::Network
)

include(GNUInstallDirs)

install(TARGETS Bingo
	BUNDLE  DESTINATION .
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
	TARGET Bingo
	OUTPUT_SCRIPT deploy_script
	NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
