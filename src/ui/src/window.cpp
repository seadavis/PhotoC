#include "window.h"
#include <QPushButton>
#include <QImageReader>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "camera_util.h"

using namespace cv;
using namespace std;

void Window::handleButton(){
 
  Camera	*camera;
    CameraFile *cameraFile;
    CameraFilePath camera_file_path;
	GPContext *context = CameraUtil::sample_create_context();
    FILE 	*f;
    char* data;
	unsigned long size;
    int retval;
	

	gp_log_add_func(GP_LOG_ERROR, CameraUtil::errordumper, NULL);
	gp_camera_new(&camera); 
	/* When I set GP_LOG_DEBUG instead of GP_LOG_ERROR above, I noticed that the
	 * init function seems to traverse the entire filesystem on the camera.  This
	 * is partly why it takes so long.
	 * (Marcus: the ptp2 driver does this by default currently.)
	 */
	printf("Camera init.  Takes about 10 seconds.\n");
	retval = gp_camera_init(camera, context);
	if (retval != GP_OK) {
		printf("  Retval of gp_camera_init: %d\n", retval);

		exit (1);
	}

    printf("Capturing To Memory\n");
    CameraUtil::capture_to_memory(camera, context, &camera_file_path, cameraFile, (const char**)&data, &size);
    printf("Captured To memory Writing to OpenCV Mat\n");

    cout << "Displaying OpenCV image, size:  " <<  size << "\n";

    Mat img = imdecode(Mat(1, size, CV_8UC1, data), CV_LOAD_IMAGE_UNCHANGED);
    if(img.empty())
    {
        std::cout << "Could not read the image From the camera: ";
        return;
		
	  }

	Mat out;
	cv::resize(img, out, cv::Size(480, 320));


  cout << "Image Total:  " << img.total() << " Element Size: " << img.elemSize() << "\n";
  cout << "Format: " << type2str(img.type()) << "\n";
  cvtColor(out,out,CV_BGR2RGB);
  pixmap = new QPixmap();
  image = new QImage(out.data, out.cols, out.rows, QImage::Format_RGB888);
  label->setPixmap(QPixmap::fromImage(*image));
}

Window::Window(QWidget *parent) :
 QWidget(parent)
 {
    // Set size of the window
    setFixedSize(600, 400);

    label = new QLabel(this);
    label->setGeometry(10, 10, 480, 320);

    button = new QPushButton("Snap!", this);
    button->setGeometry(15, 15, 100, 50);

    connect(button, &QPushButton::released, this, &Window::handleButton);
}

