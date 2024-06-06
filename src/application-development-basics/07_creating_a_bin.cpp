#include <gst/gst.h>

int main(int argc, char *argv[])
{
    gst_init(&argc, &argv);

    auto pipeline = gst_pipeline_new("mypipeline");
    auto bin = gst_bin_new("bin");
    auto source = gst_element_factory_make("fakesrc", "source");
    auto sink = gst_element_factory_make("fakesink", "sink");

    gst_bin_add_many(GST_BIN(bin), source, sink, NULL);
    gst_bin_add(GST_BIN(pipeline), bin);

    gst_element_link(source, sink);

    // (07_creating_a_bin.exe:14424): GStreamer-CRITICAL **: 12:39:57.794: gst_bin_get_by_interface: assertion 'G_TYPE_IS_INTERFACE (iface)' failed
    // auto elem = gst_bin_get_by_interface(GST_BIN(pipeline), GST_TYPE_BIN);
    g_print("The element is a bin: %s\n", GST_IS_BIN(pipeline) ? "true" : "false");
    gst_object_unref(pipeline);
    return 0;
}