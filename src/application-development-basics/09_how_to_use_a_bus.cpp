#include <gst/gst.h>

static GMainLoop *loop;


static gboolean my_bus_callback(GstBus *bus, GstMessage *message, gpointer data){
    g_print("Got %s message\n", GST_MESSAGE_TYPE_NAME(message));

    switch (GST_MESSAGE_TYPE(message))
    {
    case GST_MESSAGE_ERROR:
        GError *err;
        gchar *debug;

        gst_message_parse_error(message, &err, &debug);
        g_print("Error: %s\n", err->message);
        g_free(debug);

        g_main_loop_quit(loop);
        break;
    case GST_MESSAGE_EOS:
        // EOS: End Of Stream
        g_main_loop_quit(loop);
        break;
    default:
        break;
    }

    return TRUE;
}

gint main(gint argc, gchar *argv[]){
    gst_init(&argc, &argv);

    auto pipeline = gst_pipeline_new("mypipeline");
    auto bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
    auto bus_watch_id = gst_bus_add_watch(bus, my_bus_callback, NULL);
    // g_signal_connect(bus, "message::error", G_CALLBACK(cb_mesage_error), NULL);
    // g_signal_connect(bus, "message::eos", G_CALLBACK(cb_mesage_eos), NULL);

    gst_object_unref(bus);

    auto loop = g_main_loop_new(NULL, FALSE);
    g_main_loop_run(loop);
    g_print("MAIN LOOP");

    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);
    g_source_remove(bus_watch_id);
    g_main_loop_unref(loop);

    return 0;
}