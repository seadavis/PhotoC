/*
 * This code released into the public domain 21 July 2008
 *
 * This program does the equivalent of:
 * gphoto2 --shell
 *   > set-config capture=1
 *   > capture-image-and-download
 * compile with gcc -Wall -o canon-capture -lgphoto2 canon-capture.c
 *
 * Taken from: http://credentiality2.blogspot.com/2008/07/linux-libgphoto2-image-capture-from.html
 *
 * and condensed into simple capture sample
 */

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <gphoto2/gphoto2.h>


#include "samples.h"

static int
_lookup_widget(CameraWidget*widget, const char *key, CameraWidget **child) {
	int ret;
	ret = gp_widget_get_child_by_name (widget, key, child);
	if (ret < GP_OK)
		ret = gp_widget_get_child_by_label (widget, key, child);
	return ret;
}

int
camera_eosviewfinder(Camera *camera, GPContext *context, int onoff) {
	CameraWidget		*widget = NULL, *child = NULL;
	int			ret,val;

	ret = gp_camera_get_config (camera, &widget, context);
	if (ret < GP_OK) {
		fprintf (stderr, "camera_get_config failed: %d\n", ret);
		return ret;
	}
	
	ret = _lookup_widget (widget, "LiveViewMovieMode", &child);
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
out:
	gp_widget_free (widget);
	return ret;
}

static void errordumper(GPLogLevel level, const char *domain, const char *str,
                 void *data) {
  fprintf(stdout, "%s\n", str);
}

static void
capture_to_memory(Camera *camera, GPContext *context, const char **ptr, unsigned long int *size) {
	int retval;
	CameraFile *file;
	CameraFilePath camera_file_path;

	printf("Capturing Preview.\n");

    camera_eosviewfinder(camera, context, 1);

    retval = gp_file_new(&file);
    printf(" Retval From file new: %d\n", retval);
    retval = gp_file_set_name(file, "preview.jpg");
   
	retval = gp_camera_capture_preview(camera, file, context);
	printf(" Retval From Capture preview: %d\n", retval);

	printf("Pathname on the camera: %s/%s\n", camera_file_path.folder, camera_file_path.name);

	for(int i = 0; i < 100; i++)
	{
		gp_file_get_data_and_size (file, ptr, size);
		printf("Got Data of Size %lu\n", *size);
	}
	

	printf("Deleting.\n");
	retval = gp_camera_file_delete(camera, camera_file_path.folder, camera_file_path.name,
			context);
	printf("  Retval: %d\n", retval);
	/*gp_file_free(file); */
}

/*static void
    print_char_array(char *data, int size){
        for(int i = 0; i < size; i++){
            printf("Data: %i, %d", i, data[i]);
        }
    }*/

int
main(int argc, char **argv) {
	Camera	*camera;
	int	retval;
	GPContext *context = sample_create_context();
	//FILE 	*f;
	char	*data;
	unsigned long size;

	gp_log_add_func(GP_LOG_ERROR, errordumper, NULL);
	gp_camera_new(&camera);

	/* When I set GP_LOG_DEBUG instead of GP_LOG_ERROR above, I noticed that the
	 * init function seems to traverse the entire filesystem on the camera.  This
	 * is partly why it takes so long.
	 * (Marcus: the ptp2 driver does this by default currently.)
	 */
	printf("Camera init.  Takes about 10 seconds.\n");
	retval = gp_camera_init(camera, context);
	if (retval != GP_OK) {
		printf("  Retval of capture_to_file: %d\n", retval);
		exit (1);
	}
	//capture_to_file(camera, context, "foo.jpg");




	capture_to_memory(camera, context, (const char**)&data, &size);
    //print_char_array(data, size);

	/*f = fopen("foo2.jpg", "wb");
	if (f) {
		retval = fwrite (data, size, 1, f);
		if (retval != (int)size) {
			printf("  fwrite size %ld, written %d\n", size, retval);
		}
		fclose(f);
	} else
		printf("  fopen foo2.jpg failed.\n");*/
	gp_camera_exit(camera, context);
	return 0;
}