#include <gst/gst.h>

static void cb_new_pad(GstElement *element, GstPad *pad, gpointer data)
{
    auto name = gst_pad_get_name(pad);
    g_print("A new pad %s was created\n", name);
    g_free(name);
}

int main(int argc, char *argv[])
{
    gst_init(&argc, &argv);

    auto pipeline = gst_pipeline_new("mypipeline");
    auto source = gst_element_factory_make("filesrc", "source");
    if (!source)
    {
        g_print("Source cannot created properly.");
        return 1;
    }
    g_object_set(source, "location", argv[1], NULL);
    auto demux = gst_element_factory_make("oggdemux", "demux");
    if (!demux)
    {
        g_print("demux cannot created properly.");
        return 1;
    }

    gst_bin_add_many(GST_BIN(pipeline), source, demux, NULL);
    gst_element_link_pads(source, "src", demux, "sink");
    g_signal_connect(demux, "pad-added", G_CALLBACK(cb_new_pad), NULL);

    gst_element_set_state(GST_ELEMENT(pipeline), GST_STATE_PLAYING);
    auto loop = g_main_loop_new(NULL, FALSE);
    g_main_loop_run(loop);
}