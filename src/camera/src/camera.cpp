#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <iostream>
#include <memory>
#include <pthread.h>
#include <filesystem>
#include <algorithm>
#include "common_tests.h"
#include "camera.h"

#ifndef LIBGPHOTO2_SAMPLES_H
#define LIBGPHOTO2_SAMPLES_H

#ifdef __cplusplus
extern "C" {
#endif

extern int sample_autodetect (CameraList *list, GPContext *context);
extern int sample_open_camera (Camera ** camera, const char *model, const char *port, GPContext *context);
extern int get_config_value_string (Camera *, const char *, char **, GPContext *);
extern int set_config_value_string (Camera *, const char *, const char *, GPContext *);
int canon_enable_capture (Camera *camera, int onoff, GPContext *context);
extern int camera_auto_focus (Camera *list, GPContext *context, int onoff);
extern int camera_manual_focus (Camera *list, int tgt, GPContext *context);


#if !defined (O_BINARY)
	/*To have portable binary open() on *nix and on Windows */
	#define O_BINARY 0
#endif

#ifdef __cplusplus
};
#endif

#endif /* !defined(LIBGPHOTO2_SAMPLES_H) */

static int camera_eosviewfinder (Camera *list, GPContext *context, int onoff);

static int camera_bulb(Camera *camera, GPContext *context, int onoff);

static void errordumper(GPLogLevel level, const char *domain, const char *str, void *data);

static int camera_toggle_config(Camera *camera, GPContext *context, int onoff, string setting);

static void
capture_preview_to_memory(Camera *camera, 
                            GPContext *context, 
                            const char **ptr, 
                            unsigned long int *size);

static void
capture_to_memory(Camera *camera, 
                GPContext *context, 
                CameraFilePath* camera_file_path, 
                CameraFile *file, 
                const char **ptr, 
                unsigned long int *size);

static GPContext* create_context(void);


optional<TimeLength> ParseTimeLength(string str)
{
	const string delimiter = ":";
	size_t pos = 0;
	vector<string> tokens;
	while ((pos = str.find(delimiter)) != std::string::npos) {
		tokens.push_back(str.substr(0, pos));
		str = str.substr(pos + delimiter.length());
	}

	if(str.length() > 0)
	{
		tokens.push_back(str);
	}

	if(tokens.size() >= 3)
	{
		int hours = stoi(tokens[0]);
		int minutes = stoi(tokens[1]);
		int seconds = stoi(tokens[2]);

		return TimeLength(hours, minutes, seconds);
	}
	else
	{
		return {};
	}
}

RemoteCamera::RemoteCamera()
{
	context = create_context();
	gp_log_add_func(GP_LOG_ERROR, errordumper, NULL);
	gp_camera_new(&camera); 
	isLiveViewThreadOpen = false;
}

int RemoteCamera::connect()
{
	int retval = gp_camera_init(camera, context);

	if(retval == GP_ERROR_IO_USB_CLAIM)
	{
		throw CameraUSBClaimException();
	}

	else if (retval != GP_OK) 
	{
		throw CameraConnectionException();
	}

	return 1;
}	

Mat RemoteCamera::snap_picture()
{
	char* buffer;
	unsigned long buffer_size = 0;
	CameraFilePath camera_file_path;
	printf("Capturing To Memory\n");
	capture_to_memory(camera, context, &camera_file_path, camera_file, (const char**)&buffer, &buffer_size);

	if(buffer_size == 0)
		throw CameraOperationException("snap picture");

    auto m =  imdecode(Mat(1, buffer_size, CV_8UC1, buffer), CV_LOAD_IMAGE_UNCHANGED);
	Mat img;
	cvtColor(m, img, CV_BGR2BGRA);
	auto clonedImage = img.clone();
	delete buffer;
	return clonedImage;
}

void RemoteCamera::StartLiveView()
{
	isLiveViewThreadOpen = true;
	int status = camera_eosviewfinder(camera, context, 1);
	if(status < GP_OK)
	{
		throw CameraOperationException("start live view");
	}
	workerThread = thread(&RemoteCamera::ViewThreadWorker,this);
}

void RemoteCamera::StopLiveView()
{
	isLiveViewThreadOpen = false;
	workerThread.join();
	int status = camera_eosviewfinder(camera, context, 0);

	if(status < GP_OK)
	{
		throw CameraOperationException("stop live view");
	}
}

void RemoteCamera::StartLongExposure(LongExposureShots shots)
{
	isLongExposureThreadOpen = true;
	currentShot = shots;
	workerThread = thread(&RemoteCamera::LongExposureThreadWorker,this);
}

void RemoteCamera::StopLongExposure()
{
	isLongExposureThreadOpen = false;
	workerThread.join();
}

void RemoteCamera::LongExposureThreadWorker()
{
	auto start = high_resolution_clock::now();
	auto now = high_resolution_clock::now();

	while(isLongExposureThreadOpen && 
			duration_cast<chrono::milliseconds>(now - start) < currentShot.Length.ToMilliseconds())
	{
		
		int status = camera_bulb(camera, context, 1);
		this_thread::sleep_for(currentShot.Interval.ToMilliseconds());
		status = camera_bulb(camera, context, 0);

		CameraEventType event;
		void *data = NULL;
		int test = 0;


		/* This is equivalent to emptying out the queue*/
		while(event != GP_EVENT_FILE_ADDED)
		{
			gp_camera_wait_for_event(camera,1000, &event, &data, context);
		}

		char* buffer;
		unsigned long bufferSize = 0;
		CameraFilePath* path = (CameraFilePath*)data;
		CameraFile* addedFile;

		gp_file_new(&addedFile);
		gp_camera_file_get(camera, path->folder, path->name, GP_FILE_TYPE_NORMAL, addedFile, context);
		gp_file_get_data_and_size(addedFile,(const char**)&buffer,&bufferSize);
		if(bufferSize > 0)
		{
			auto m =  imdecode(Mat(1, bufferSize, CV_8UC1, buffer), CV_LOAD_IMAGE_UNCHANGED);
			Mat img;
			cvtColor(m, img, CV_BGR2BGRA);
			auto clonedImage = img.clone();
			this->receiver->Receive(img);
			gp_camera_file_delete(camera, path->folder, path->name, context);
			gp_file_free(addedFile);
			free(data);
			event = GP_EVENT_UNKNOWN;
		}

	}

	isLongExposureThreadOpen = false;
}

void RemoteCamera::ViewThreadWorker()
{
	while(isLiveViewThreadOpen)
	{
		char* buffer;
		unsigned long buffer_size = 0;
		capture_preview_to_memory(camera, context, (const char**)&buffer, &buffer_size);
		if(buffer_size > 0)
		{
			auto m =  imdecode(Mat(1, buffer_size, CV_8UC1, buffer), CV_LOAD_IMAGE_UNCHANGED);
			Mat img;
			cvtColor(m, img, CV_BGR2BGRA);
			auto clonedImage = img.clone();
			delete buffer;
			receiver->Receive(img);

		}

		this_thread::sleep_for (chrono::milliseconds(17));
	}
}

RemoteCamera::~RemoteCamera()
{
	gp_camera_exit(camera, context);
}

FakeCamera::FakeCamera(string filePath, string folderPath)
{
	this->picPath = filePath;
	this->longExposureFolderPath = folderPath;
	this->isLongExposureThreadOpen = true;
	this->isLongExposureInProgress = false;
	workerThread = thread(&FakeCamera::LongExposureThreadWorker,this);
}

Mat FakeCamera::snap_picture()
{
	Mat m = imread(picPath);
	Mat img;
    cvtColor(m, img, CV_BGR2BGRA);
	return img;
}

int FakeCamera::connect()
{
	return 1;
}

void FakeCamera::StartLiveView()
{
	
}

void FakeCamera::StopLiveView()
{
	
}

void FakeCamera::StartLongExposure(LongExposureShots shots)
{
	isLongExposureInProgress = true;
	this->currentShot = shots;

}

void FakeCamera::StopLongExposure()
{
	isLongExposureInProgress = false;
}


void FakeCamera::LongExposureThreadWorker()
{
	int picturesTaken = 0;
	 
	vector<string> pictures;

	for (const auto & entry : filesystem::directory_iterator(longExposureFolderPath))
  	{
		pictures.push_back(entry.path().c_str());
  	}	   

	sort(pictures.begin(), pictures.end());

	time_point<system_clock> start;

	while(isLongExposureThreadOpen)
	{
		while(isLongExposureInProgress)
		{
			if(picturesTaken == 0)
				start = high_resolution_clock::now();

			auto now = high_resolution_clock::now();

			if(duration_cast<chrono::milliseconds>(now - start) < currentShot.Length.ToMilliseconds())
			{
				auto currentPicture = pictures[picturesTaken%pictures.size()];
				cout << "Image Path: " << currentPicture << "\n";
				Mat img = common::tests::loadStdImage(currentPicture);
				this->receiver->Receive(img);
				this_thread::sleep_for(currentShot.Interval.ToMilliseconds());
				picturesTaken++;
			}
			else
			{
				picturesTaken = 0;
				isLongExposureInProgress = false;
			}
		}

		this_thread::sleep_for(milliseconds(30));
	}
}

FakeCamera::~FakeCamera()
{
	isLongExposureThreadOpen = false;
	workerThread.join();
}

/* section reserved for C code specific to the camera */
static int
_lookup_widget(CameraWidget*widget, const char *key, CameraWidget **child) {
	int ret;
	ret = gp_widget_get_child_by_name (widget, key, child);
	if (ret < GP_OK)
		ret = gp_widget_get_child_by_label (widget, key, child);
	return ret;
}

static void
ctx_error_func (GPContext *context, const char *str, void *data)
{
        fprintf  (stderr, "\n*** Contexterror ***              \n%s\n",str);
        fflush   (stderr);
}

static void
ctx_status_func (GPContext *context, const char *str, void *data)
{
        fprintf  (stderr, "%s\n", str);
        fflush   (stderr);
}

static GPContext* create_context() {
	GPContext *context;

	/* This is the mandatory part */
	context = gp_context_new();

	/* All the parts below are optional! */
        gp_context_set_error_func (context, ctx_error_func, NULL);
        gp_context_set_status_func (context, ctx_status_func, NULL);

	/* also:
	gp_context_set_cancel_func    (p->context, ctx_cancel_func,  p);
        gp_context_set_message_func   (p->context, ctx_message_func, p);
        if (isatty (STDOUT_FILENO))
                gp_context_set_progress_funcs (p->context,
                        ctx_progress_start_func, ctx_progress_update_func,
                        ctx_progress_stop_func, p);
	 */
	return context;
}

static int
camera_bulb(Camera *camera, GPContext *context, int onoff)
{
	return camera_toggle_config(camera, context, onoff, "bulb");
}

static int
camera_eosviewfinder(Camera *camera, GPContext *context, int onoff) 
{
	return camera_toggle_config(camera, context, onoff, "LiveViewMovieMode");
}


static int
camera_toggle_config(Camera *camera, GPContext *context, int onoff, string setting)
{
	CameraWidget		*widget = NULL, *child = NULL;
	int			ret,val;

	ret = gp_camera_get_config (camera, &widget, context);
	if (ret < GP_OK) {
		fprintf (stderr, "camera_get_config failed: %d\n", ret);
		return ret;
	}

	ret = _lookup_widget (widget, setting.c_str(), &child);
	if (ret < GP_OK) {
		fprintf (stderr, "lookup 'LiveViewMode' failed: %d\n", ret);
		goto out;
	}

	ret = gp_widget_get_value (child, &val);
	if (ret < GP_OK) {
		fprintf (stderr, "could not get widget value: %d\n", ret);
		goto out;
	}
	val = onoff;
	ret = gp_widget_set_value (child, &val);
	if (ret < GP_OK) {
		fprintf (stderr, "could not set widget value to 1: %d\n", ret);
		goto out;
	}

	ret = gp_camera_set_config (camera, widget, context);
	if (ret < GP_OK) {
		fprintf (stderr, "could not set config tree to eosviewfinder: %d\n", ret);
		goto out;
	}

	printf("Set in preview Mode\n");
out:
	gp_widget_free (widget);
	return ret;
}

static void errordumper(GPLogLevel level, const char *domain, const char *str,
                 void *data) {
  fprintf(stdout, "%s\n", str);
}

static void capture_to_memory(Camera *camera, GPContext *context, CameraFilePath* camera_file_path, CameraFile *file, const char **ptr, unsigned long int *size) {
	int retval;
	

	/* NOP: This gets overridden in the library to /capt0000.jpg */
	strcpy(camera_file_path->folder, "/");
	strcpy(camera_file_path->name, "foo.jpg");

	retval = gp_camera_capture(camera, GP_CAPTURE_IMAGE, camera_file_path, context);

	if(retval < 0)
	{
		throw CameraOperationException("snap picture");
	}

	retval = gp_file_new(&file);
	retval = gp_camera_file_get(camera, camera_file_path->folder, camera_file_path->name,
		     GP_FILE_TYPE_NORMAL, file, context);

	gp_file_get_data_and_size (file, ptr, size);

	/*gp_file_free(file); */
}

static void capture_preview_to_memory(Camera *camera, GPContext *context, const char **ptr, unsigned long int *size) {
	int retval;
	CameraFile *file;
	CameraFilePath camera_file_path;

	printf("Capturing Preview.\n");

    retval = gp_file_new(&file);
    retval = gp_file_set_name(file, "preview.jpg");
   
	retval = gp_camera_capture_preview(camera, file, context);

	gp_file_get_data_and_size (file, ptr, size);
}