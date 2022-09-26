#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <gphoto2/gphoto2.h>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <memory>

#ifndef LIBGPHOTO2_SAMPLES_H
#define LIBGPHOTO2_SAMPLES_H

#include <gphoto2/gphoto2-camera.h>

#ifdef __cplusplus
extern "C" {
#endif

extern int sample_autodetect (CameraList *list, GPContext *context);
extern int sample_open_camera (Camera ** camera, const char *model, const char *port, GPContext *context);
extern GPContext* sample_create_context(void);

extern int get_config_value_string (Camera *, const char *, char **, GPContext *);
extern int set_config_value_string (Camera *, const char *, const char *, GPContext *);
int canon_enable_capture (Camera *camera, int onoff, GPContext *context);

extern int camera_auto_focus (Camera *list, GPContext *context, int onoff);
extern int camera_eosviewfinder (Camera *list, GPContext *context, int onoff);
extern int camera_manual_focus (Camera *list, int tgt, GPContext *context);

#if !defined (O_BINARY)
	/*To have portable binary open() on *nix and on Windows */
	#define O_BINARY 0
#endif

#ifdef __cplusplus
};
#endif

#endif /* !defined(LIBGPHOTO2_SAMPLES_H) */

namespace CameraUtil
{
void
capture_preview_to_memory(Camera *camera, 
                            GPContext *context, 
                            const char **ptr, 
                            unsigned long int *size);

void
capture_to_memory(Camera *camera, 
                GPContext *context, 
                CameraFilePath* camera_file_path, 
                CameraFile *file, 
                const char **ptr, 
                unsigned long int *size);
void errordumper(GPLogLevel level, const char *domain, const char *str,
                 void *data);

GPContext* sample_create_context();

}