add_library(ui mainwindow.cpp canvaswidget.cpp compositeselection.cpp fileSelector.cpp ImageViewer.cpp utilities.cpp)
target_link_libraries(ui PRIVATE camera processing io Qt5::Widgets ${OpenCV_LIBS})
target_include_directories(ui PUBLIC ../../camera/src/  ../../processing/src/ ../../io/src)