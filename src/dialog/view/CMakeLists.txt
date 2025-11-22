qt_add_library(view STATIC view.hpp)
target_include_directories(view INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(view PUBLIC Qt::Core Qt::Widgets)
