add_library(ui mainwindow.cpp canvaswidget.cpp)
target_link_libraries(ui PRIVATE camera processing Qt5::Widgets ${OpenCV_LIBS})
target_include_directories(ui PUBLIC ../../camera/src/  ../../processing/src/)