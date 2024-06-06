#include <gst/gst.h>

int main(int argc, char *argv[])
{
    gst_init(&argc,&argv);

    auto pipeline = gst_pipeline_new("mypipeline");

    auto source = gst_element_factory_make("fakesrc", "source");
    auto filter = gst_element_factory_make("identity", "filter");
    auto sink = gst_element_factory_make("fakesink", "sink");

    gst_bin_add_many(GST_BIN(pipeline), source, filter, sink, NULL);

    if (!gst_element_link_many(source, filter, sink, NULL)){
        g_warning("Failed to link elements!");
    }

    // Change element states
    // GST_STATE_NULL or GST_STATE_READY or GST_STATE_PAUSED or GST_STATE_PLAYING
    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    // (06_linking_elements.exe:7916): GStreamer-CRITICAL **: 12:16:35.296:
    // Trying to dispose object "source", but it still has a parent "mypipeline".
    // You need to let the parent manage the object instead of unreffing the object directly.
    // gst_object_unref(GST_OBJECT(source));
    // gst_object_unref(GST_OBJECT(filter));
    // gst_object_unref(GST_OBJECT(sink));
    gst_object_unref(GST_OBJECT(pipeline));
    return 0;
}