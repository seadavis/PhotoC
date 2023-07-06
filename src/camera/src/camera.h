#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <gphoto2/gphoto2.h>
#include <gphoto2/gphoto2-camera.h>
#include <functional>
#include <thread>
#include <atomic>
#include <optional>
#pragma once

using namespace cv;
using namespace std;

struct TimeLength
{
	int hours;
	int minutes;
	int seconds;

	TimeLength(int hours, int minutes, int seconds) : hours(hours), minutes(minutes), seconds(seconds){};


};


inline bool operator==(const TimeLength& lhs, const TimeLength& rhs)
{
    return lhs.hours == rhs.hours && lhs.minutes == rhs.minutes && lhs.seconds == rhs.seconds;
}

struct LongExposure
{

	/**
	 * Specifies the interval in which long
	 * expsosures are taken
	*/
	TimeLength Interval;

	/**
	 * Specifies how long the interval should last.
	*/
	TimeLength Length;

	LongExposure(TimeLength interval, TimeLength length) : Interval(interval), Length(length){};

};

inline bool operator==(const LongExposure& lhs, const LongExposure& rhs)
{
    return lhs.Interval == rhs.Interval && lhs.Length == rhs.Length;
}

/**
	 * Decomposes a time length from a string.
	 * Expected format is HH:mm:ss
	*/
optional<TimeLength> ParseTimeLength(string str);
 
class CameraOperationException : public runtime_error {
    public:
        CameraOperationException(string operation_name) : runtime_error("Unable to perform a:  " + operation_name + ". Check camera connection.") {};

};

    class CameraUSBClaimException : public runtime_error {
    public:
        CameraUSBClaimException() : runtime_error("Unable to claim USB for camera. You may already be connected.") {};

};

class CameraConnectionException : public runtime_error {
    public:
        CameraConnectionException() : runtime_error("Unable to connect to USB camera. Check your cables.") {};

};


class IReceiveImages
{
    public:
        virtual void Receive(Mat m) = 0;
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

        virtual void StartLiveView() = 0;

        virtual void StopLiveView() = 0;

        void SetReceiver(IReceiveImages* receiver)
        {
            this->receiver = receiver;
        }

    protected:
        IReceiveImages* receiver;
};


class FakeCamera : public ICamera
{
    public:
        FakeCamera(string pic_path);
		int connect() override;
		Mat snap_picture() override;
        void StartLiveView() override;
        void StopLiveView() override;

    private:
        string pic_path;
};

class RemoteCamera : public ICamera
{
	public:
        RemoteCamera();
		int connect() override;
		Mat snap_picture() override;
        void StartLiveView() override;
        void StopLiveView() override;

        ~RemoteCamera();

    private:
        Camera	*camera;
        CameraFile *camera_file;
        GPContext *context;
        thread workerThread;
        atomic<bool> isLiveViewThreadOpen;
        void ViewThreadWorker();

};


