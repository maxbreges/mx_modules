cmake_minimum_required(VERSION 3.19)

project(Slider1)

build_gmpi_plugin(
	PROJECT_NAME ${PROJECT_NAME}
	HAS_DSP
	HAS_GUI
	SOURCE_FILES
	  Slider1.h
	  Slider1.cpp
	  Slider1Gui.h
	  Slider1Gui.cpp

	../sharedLegacyWidgets/BitmapWidget.cpp
        ../sharedLegacyWidgets/BitmapWidget.h
        ../sharedLegacyWidgets/EditWidget.cpp
        ../sharedLegacyWidgets/EditWidget.h
        ../shared/FontCache.h
        ../shared/FontCache.cpp
        ../shared/ImageCache.h
        ../shared/ImageCache.cpp
        ../shared/ImageMetadata.h
        ../shared/ImageMetadata.cpp

        ${se_sdk_folder}/it_enum_list.h
        ${se_sdk_folder}/it_enum_list.cpp
	${se_sdk_folder}/smart_audio_pin.h
        ${se_sdk_folder}/smart_audio_pin.cpp

        ../sharedLegacyWidgets/TextEditWidget.cpp
        ../sharedLegacyWidgets/TextEditWidget.h
        ../sharedLegacyWidgets/TextWidget.cpp
        ../sharedLegacyWidgets/TextWidget.h
        ../sharedLegacyWidgets/Widget.cpp
        ../sharedLegacyWidgets/Widget.h
	../sharedLegacyWidgets/WidgetHost.h

)
