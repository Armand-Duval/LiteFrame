find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick QuickControls2 QuickDialogs2)

qt_standard_project_setup()

qt_policy(SET QTP0001 NEW)
qt_policy(SET QTP0004 NEW)

qt_add_executable(lf_editor_app
    main.cpp
    editor_controller.cpp
)

qt_add_qml_module(lf_editor_app
    URI LiteFrame
    VERSION 1.0
    QML_FILES
        qml/Main.qml
    SOURCES
        preview_item.cpp
        preview_item.h
)

target_link_libraries(lf_editor_app
    PRIVATE
        lf_core
        Qt6::Core
        Qt6::Gui
        Qt6::Qml
        Qt6::Quick
        Qt6::QuickControls2
        Qt6::QuickDialogs2
)

set_target_properties(lf_editor_app PROPERTIES
    OUTPUT_NAME lf-editor
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)
