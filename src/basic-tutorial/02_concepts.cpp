#include <gst/gst.h>
#ifdef __APPLE__
#include <TargetConditionals.h>
#endif

int tutorial_main(int argc, char *argv[])
{
    gst_init(&argc, &argv);

    auto source = gst_element_factory_make("videotestsrc", "source");
    auto filter = gst_element_factory_make("vertigotv", "filter");
    auto sink = gst_element_factory_make("autovideosink", "sink");

    auto pipeline = gst_pipeline_new("test-pipeline");
    if (!pipeline || !source || !filter || !sink)
    {
        g_printerr("Not all element could be created.\n");
        return -1;
    }

    gst_bin_add_many(GST_BIN(pipeline), source, filter, sink, NULL);
    if (gst_element_link_many(source, filter, sink, NULL) != TRUE)
    {
        g_printerr("Elements could not be linked.\n");
        gst_object_unref(pipeline);
        return -1;
    }

    g_object_set(source, "pattern", 0, NULL);
    auto state_change_ret = gst_element_set_state(pipeline, GST_STATE_PLAYING);
    g_print("state_change_ret, %d\n", state_change_ret);
    if (state_change_ret == GST_STATE_CHANGE_FAILURE)
    {
        g_printerr("Unable to setup the pipeline to the playing state.\n");
        gst_object_unref(pipeline);
        return -1;
    }

    auto bus = gst_element_get_bus(pipeline);
    auto msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE,
                                          GST_MESSAGE_ERROR | GST_MESSAGE_EOS);

    if (msg != NULL)
    {
        switch (GST_MESSAGE_TYPE(msg))
        {
        case GST_MESSAGE_ERROR:
            GError *err;
            gchar *debug_info;
            gst_message_parse_error(msg, &err, &debug_info);
            g_printerr("Error received from element %s: %s\n",
                       GST_OBJECT_NAME(msg->src), err->message);
            g_printerr("Debugging information: %s\n", debug_info ? debug_info : "none");
            g_clear_error(&err);
            g_free(debug_info);
            break;
        case GST_MESSAGE_EOS:
            g_print("End-Of-Stream reached.\n");
            break;
        default:
            g_printerr("Unexpeceted message received.\n");
            break;
        }
        gst_message_unref(msg);
    }

    gst_object_unref(bus);
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);
    return 0;
}

int main(int argc, char *argv[])
{
#if defined(__APPLE__) && TARGET_OS_MAC && !TARGET_OS_IPHONE
    return gst_macos_main((GstMainFunc)tutorial_main, argc, argv, NULL);
#else
    return tutorial_main(argc, argv);
#endif
}