cmake_minimum_required(VERSION 3.19)

project(SignalLogger)

build_gmpi_plugin(
    PROJECT_NAME ${PROJECT_NAME}
    HAS_GUI
    HAS_DSP
    SOURCE_FILES 
        ${PROJECT_NAME}.cpp
        ${PROJECT_NAME}Gui.cpp
        PhaseScopeGui.cpp
	${se_shared_folder}/GraphHelpers.h
)
