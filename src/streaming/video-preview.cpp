#include "bus.hpp"
#include "clock.hpp"

int main(int argc, char *argv[])
{
    gst_init(&argc, &argv);

    GMainLoop *loop = g_main_loop_new(NULL, FALSE);
    GstElement *pipeline = gst_pipeline_new("streaming");

    GstElement *video_source = gst_element_factory_make("ksvideosrc", "video_source");
    g_object_set(video_source, "device-index", 0, NULL);

    GstElement *video_convert = gst_element_factory_make("videoconvert", "video_convert");
    GstElement *prev_video_enc_queue = gst_element_factory_make("queue", "prev_video_enc_queue");
    GstElement *video_encode = gst_element_factory_make("x264enc", "video_encode");
    g_object_set(video_encode, "tune", "zerolatency", NULL);
    GstElement *video_pay = gst_element_factory_make("rtph264pay", "video_pay");
    g_object_set(video_pay, "config-interval", 1, NULL);
    GstElement *video_queue = gst_element_factory_make("queue", "video_queue");

    GstElement *video_depay = gst_element_factory_make("rtph264depay", "video_depay");
    GstElement *video_decode = gst_element_factory_make("avdec_h264", "video_decode");
    GstElement *video_convert2 = gst_element_factory_make("videoconvert", "video_convert2");

    GstElement *video_sink = gst_element_factory_make("autovideosink", "autovideosink");

    /*
     * Remove pay/depay and encode/decode
     */
    // gst_bin_add_many(GST_BIN(pipeline), video_source, video_convert, video_queue, video_sink, NULL);
    // gst_element_link_many(video_source, video_convert, video_queue, video_sink, NULL);

    /*
     * Remove video pay/depay
     */
    gst_bin_add_many(GST_BIN(pipeline), video_source, video_convert, video_queue, video_encode, video_decode, video_convert2, video_sink, NULL);
    gst_element_link_many(video_source, video_convert, video_queue, video_encode, video_decode, video_convert2, video_sink, NULL);

    /*
     * all
     */
    // gst_bin_add_many(GST_BIN(pipeline), video_source, video_convert, video_queue, video_encode, video_pay, video_depay, video_decode, video_convert2, video_sink, NULL);
    // gst_element_link_many(video_source, video_convert, video_queue, video_encode, video_pay, video_depay, video_decode, video_convert2, video_sink, NULL);

    GstBus *bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
    gst_bus_add_watch(bus, gst_bus_call, loop);

    g_print("Starting pipeline\n");
    gst_element_set_state(pipeline, GST_STATE_PLAYING);
    g_print("Running loop\n");

    g_timeout_add(1000, (GSourceFunc)cb_print_position, pipeline);
    g_main_loop_run(loop);

    g_print("Returned from loop\n");
    gst_object_unref(bus);
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);

    return 0;
}