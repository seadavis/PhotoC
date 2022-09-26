#include "camera_util.h"

using namespace cv;
using namespace std;

static int
_lookup_widget(CameraWidget*widget, const char *key, CameraWidget **child) {
	int ret;
	ret = gp_widget_get_child_by_name (widget, key, child);
	if (ret < GP_OK)
		ret = gp_widget_get_child_by_label (widget, key, child);
	return ret;
}

void
ctx_error_func (GPContext *context, const char *str, void *data)
{
        fprintf  (stderr, "\n*** Contexterror ***              \n%s\n",str);
        fflush   (stderr);
}

void
ctx_status_func (GPContext *context, const char *str, void *data)
{
        fprintf  (stderr, "%s\n", str);
        fflush   (stderr);
}

GPContext* CameraUtil::sample_create_context() {
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

	printf("Set in preview Mode\n");
out:
	gp_widget_free (widget);
	return ret;
}

void CameraUtil::errordumper(GPLogLevel level, const char *domain, const char *str,
                 void *data) {
  fprintf(stdout, "%s\n", str);
}

void
CameraUtil::capture_to_memory(Camera *camera, GPContext *context, CameraFilePath* camera_file_path, CameraFile *file, const char **ptr, unsigned long int *size) {
	int retval;
	

	printf("Capturing.\n");

	/* NOP: This gets overridden in the library to /capt0000.jpg */
	strcpy(camera_file_path->folder, "/");
	strcpy(camera_file_path->name, "foo.jpg");

	retval = gp_camera_capture(camera, GP_CAPTURE_IMAGE, camera_file_path, context);
	printf("  Retval: %d\n", retval);

	printf("Pathname on the camera: %s/%s\n", camera_file_path->folder, 
                                                camera_file_path->name);

	retval = gp_file_new(&file);
	printf("  Retval: %d\n", retval);
	retval = gp_camera_file_get(camera, camera_file_path->folder, camera_file_path->name,
		     GP_FILE_TYPE_NORMAL, file, context);
	printf("  Retval: %d\n", retval);

  
	gp_file_get_data_and_size (file, ptr, size);

	/*gp_file_free(file); */
}

void
CameraUtil::capture_preview_to_memory(Camera *camera, GPContext *context, const char **ptr, unsigned long int *size) {
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