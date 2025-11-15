qt_add_library(dialog STATIC dialog.cpp dialog.hpp dialog.ui)
target_include_directories(dialog INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(dialog
    PUBLIC
        Qt::Core
        Qt::Widgets
)
