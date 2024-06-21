// gst-launch-1.0 -e ksvideosrc device-index=0 ! videoconvert ! queue ! autovideosink ksvideosrc device-index=1 ! videoconvert ! queue ! autovideosink ksvideosrc device-index=2 ! videoconvert ! queue ! autovideosink
#include <gst/gst.h>
#include <stdio.h>


static gboolean bus_call(GstBus *bus, GstMessage *msg, gpointer data) {
    GMainLoop *loop = (GMainLoop *)data;

    switch (GST_MESSAGE_TYPE(msg)) {
        case GST_MESSAGE_EOS:
            g_print("End of stream\n");
            g_main_loop_quit(loop);
            break;

        case GST_MESSAGE_ERROR: {
            gchar *debug;
            GError *error;

            gst_message_parse_error(msg, &error, &debug);

            g_printerr("Error: %s\n", error->message);
            g_printerr("Debug details: %s\n", debug);
            g_free(debug);
            g_free(error);
            g_error_free(error);

            // g_main_loop_quit(loop);
            break;
        }
        default:
            break;
    }

    return TRUE;
}


int main(int argc, char *argv[]){
    // ./multiple-cameras 3 => 3 cameras

    if (argc != 2){
        printf("Usage: %s <number of cameras>\n", argv[0]);
        return -1;
    }
    auto num_cameras = atoi(argv[1]);
    gst_init(&argc, &argv);
    g_print("Number of cameras: %d\n", num_cameras);

    GMainLoop *loop = g_main_loop_new(NULL, FALSE);
    GstElement *pipeline = gst_pipeline_new("pipeline");
    for (int i = 0; i < num_cameras; i++){
        char source_name[16], xraw_name[16], convert_name[16], queue_name[16], sink_name[16];
        snprintf(source_name, sizeof(source_name), "source_%d", i);
        snprintf(xraw_name, sizeof(xraw_name), "xraw_%d", i);
        snprintf(convert_name, sizeof(convert_name), "convert_%d", i);
        snprintf(queue_name, sizeof(queue_name), "queue_%d", i);
        snprintf(sink_name, sizeof(sink_name), "sink_%d", i);


        GstElement *source = gst_element_factory_make("ksvideosrc", source_name);
        g_object_set(source, "device-index", i, NULL);
        GstElement *convert = gst_element_factory_make("videoconvert", convert_name);
        GstElement *queue = gst_element_factory_make("queue", queue_name);
        GstElement *sink = gst_element_factory_make("autovideosink", sink_name);


        g_print("Camera %d\n", i);
        gst_bin_add_many(GST_BIN(pipeline), source, convert, queue, sink, NULL);
        g_print("Linking elements\n");
        gst_element_link_many(source, convert, queue, sink, NULL);
        g_print("Linked elements\n");
    }

    GstBus *bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
    gst_bus_add_watch(bus, bus_call, loop);
    gst_object_unref(bus);

    g_print("Setting pipeline to playing\n");
    gst_element_set_state(pipeline, GST_STATE_PLAYING);
    g_print("Pipeline is playing\n");

    g_main_loop_run(loop);

    g_print("Returned, stopping playback\n");
    gst_element_set_state(pipeline, GST_STATE_NULL);
    g_print("Deleting pipeline\n");
    gst_object_unref(GST_OBJECT(pipeline));
    g_main_loop_unref(loop);
    return 0;
}