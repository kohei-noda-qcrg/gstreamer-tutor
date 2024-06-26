#include <gst/gst.h>
#ifdef __APPLE__
#include <TargetConditionals.h>
#endif

typedef struct _CustomData
{
    GstElement *pipeline;
    GstElement *source;
    GstElement *convert;
    GstElement *resample;
    GstElement *sink;
} CustomData;

static void pad_added_handler(GstElement *src, GstPad *pad, CustomData *data);

int tutorial_main(int argc, char *argv[])
{
    gst_init(&argc, &argv);

    CustomData data;
    data.source = gst_element_factory_make("uridecodebin", "source");
    data.convert = gst_element_factory_make("audioconvert", "convert");
    data.resample = gst_element_factory_make("audioresample", "resample");
    data.sink = gst_element_factory_make("autoaudiosink", "sink");

    data.pipeline = gst_pipeline_new("test-pipeline");

    if (!data.pipeline || !data.source || !data.convert || !data.resample || !data.sink)
    {
        g_printerr("Not all elements could be created.\n");
        return -1;
    }

    gst_bin_add_many(GST_BIN(data.pipeline), data.source, data.convert, data.resample, data.sink, NULL);
    // Don't link data.source because of no source pads.
    if (!gst_element_link_many(data.convert, data.resample, data.sink, NULL) == TRUE)
    {
        g_printerr("Elements could not be linked.\n");
        gst_object_unref(data.pipeline);
        return -1;
    }

    g_object_set(data.source, "uri", "https://gstreamer.freedesktop.org/data/media/sintel_trailer-480p.webm", NULL);

    // data.source pads are not initialliy available because demuxer pad is a dynamic pad.
    g_signal_connect(data.source, "pad-added", G_CALLBACK(pad_added_handler), &data);

    if (gst_element_set_state(data.pipeline, GST_STATE_PLAYING) == GST_STATE_CHANGE_FAILURE)
    {
        g_printerr("Unable to set the pipeline to the playing state.\n");
        gst_object_unref(data.pipeline);
        return -1;
    }

    auto bus = gst_element_get_bus(data.pipeline);
    gboolean terminate = FALSE;
    do
    {
        auto msg =
            gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE,
                                       GST_MESSAGE_STATE_CHANGED | GST_MESSAGE_ERROR | GST_MESSAGE_EOS);
        if (msg != NULL)
        {
            switch (GST_MESSAGE_TYPE(msg))
            {
            case GST_MESSAGE_ERROR:
                GError *err;
                gchar *debug_info;

                gst_message_parse_error(msg, &err, &debug_info);
                g_printerr("Error received from element %s: %s\n", GST_OBJECT_NAME(msg->src), err->message);
                g_printerr("Debugging information: %s\n", debug_info ? debug_info : "none");
                g_clear_error(&err);
                g_free(debug_info);
                terminate = TRUE;
                break;
            case GST_MESSAGE_EOS:
                g_print("End-Of-Stream reached.\n");
                terminate = TRUE;
                break;
            case GST_MESSAGE_STATE_CHANGED:
                if (GST_MESSAGE_SRC(msg) == GST_OBJECT(data.pipeline))
                {
                    GstState old_state, new_state, pending_state;
                    gst_message_parse_state_changed(msg, &old_state, &new_state, &pending_state);
                    g_print("Pipeline state change form %s to %s:\n",
                            gst_element_state_get_name(old_state), gst_element_state_get_name(new_state));
                }
                break;
            default:
                g_printerr("Unexpected message received.\n");
                break;
            }
            gst_message_unref(msg);
        }

    } while (!terminate);

    gst_object_unref(bus);
    gst_element_set_state(data.pipeline, GST_STATE_NULL);
    gst_object_unref(data.pipeline);
    return 0;
}

static void unref_caps_and_sink_pad(GstCaps *new_pad_caps, GstPad *sink_pad)
{
    if (new_pad_caps != NULL)
    {
        gst_caps_unref(new_pad_caps);
    }
    if (sink_pad != NULL)
    {
        gst_object_unref(sink_pad);
    }
}

static void pad_added_handler(GstElement *src, GstPad *new_pad, CustomData *data)
{
    GstCaps *new_pad_caps = NULL;
    auto *sink_pad = gst_element_get_static_pad(data->convert, "sink");
    if (gst_pad_is_linked(sink_pad))
    {
        g_print("We are already linked. Ignoring.\n");
        unref_caps_and_sink_pad(new_pad_caps, sink_pad);
        return;
    }

    // Check pad's type
    new_pad_caps = gst_pad_get_current_caps(new_pad);
    auto new_pad_struct = gst_caps_get_structure(new_pad_caps, 0);
    auto new_pad_type = gst_structure_get_name(new_pad_struct);
    if (!g_str_has_prefix(new_pad_type, "audio/x-raw"))
    {
        g_print("It has type '%s' which is not raw audio. Ignoring.\n", new_pad_type);
        unref_caps_and_sink_pad(new_pad_caps, sink_pad);
        return;
    }

    // link
    auto ret = gst_pad_link(new_pad, sink_pad);
    if (GST_PAD_LINK_FAILED(ret))
    {
        g_print("Type is '%s' but link failed.\n", new_pad_type);
    }
    else
    {
        g_print("Link succeeded (type '%s').\n", new_pad_type);
    }
}

int main(int argc, char *argv[])
{
#if defined(__APPLE__) && TARGET_OS_MAC && !TARGET_OS_IPHONE
    return gst_macos_main((GstMainFunc)tutorial_main, argc, argv, NULL);
#else
    return tutorial_main(argc, argv);
#endif
}