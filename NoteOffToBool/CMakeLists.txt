cmake_minimum_required(VERSION 3.19)

project(NoteOffToBool)

build_gmpi_plugin(
	PROJECT_NAME ${PROJECT_NAME}
	HAS_GUI
	HAS_DSP
	SOURCE_FILES	 
	 NoteOffToBoolGui.cpp
	 NoteOffToBool.cpp
)
