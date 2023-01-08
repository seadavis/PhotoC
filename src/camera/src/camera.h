#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <gphoto2/gphoto2.h>
#include <gphoto2/gphoto2-camera.h>

using namespace cv;
using namespace std;

 
    class CameraOperationException : public runtime_error {
        public:
            CameraOperationException(string operation_name) : runtime_error("Unable to perform a:  " + operation_name + ". Check camera connection.") {};
    
    };

class ICamera
{

    public:

        /**
         * connects to a camera.
         * Just connects to the first camera that
         * it can find
         **/
        virtual int connect() = 0;

        /*
        snaps a picutre and returns the snapped picture in an OpenCV matrix.
        */
        virtual Mat snap_picture() = 0;
};


class FakeCamera : public ICamera
{
    public:
        FakeCamera(string pic_path);
		int connect() override;
		Mat snap_picture() override;

    private:
        string pic_path;
};

class RemoteCamera : public ICamera
{
	public:
        RemoteCamera();
		int connect() override;

		Mat snap_picture() override;
        ~RemoteCamera();

    private:
        Camera	*camera;
        CameraFile *camera_file;
        GPContext *context;
        char *buffer;
        unsigned long buffer_size;

};