#include "bus.hpp"
#include "clock.hpp"

int main(int argc, char *argv[])
{
    gst_init(&argc, &argv);

    // Video and audio are streamed on different ports (rtp)
    // Audio: 7001
    // Video: 7002
    // gst-launch-1.0.exe -v directsoundsrc! audioconvert ! queue ! opusenc ! rtpopuspay ! udpsink host=127.0.0.1 port=7001 ksvideosrc device-index=0 ! videoconvert ! x264enc tune=zerolatency ! rtph264pay config-interval=1 ! queue ! udpsink host=127.0.0.1 port=7002

    GMainLoop *loop = g_main_loop_new(NULL, FALSE);
    GstElement *pipeline = gst_pipeline_new("streaming");

    GstElement *audio_source = gst_element_factory_make("directsoundsrc", "audio_source");
    GstElement *audio_convert = gst_element_factory_make("audioconvert", "audio_convert");
    GstElement *audio_queue = gst_element_factory_make("queue", "audio_queue");
    GstElement *audio_encode = gst_element_factory_make("opusenc", "audio_encode");
    GstElement *audio_pay = gst_element_factory_make("rtpopuspay", "audio_pay");
    GstElement *audio_sink = gst_element_factory_make("udpsink", "audio_sink");
    g_object_set(audio_sink, "host", "127.0.0.1", "port", 7001, NULL);


    GstElement *video_source = gst_element_factory_make("ksvideosrc", "video_source");
    g_object_set(video_source, "device-index", 0, NULL);
    GstElement *video_convert = gst_element_factory_make("videoconvert", "video_convert");
    GstElement *video_encode = gst_element_factory_make("x264enc", "video_encode");
    g_object_set(video_encode, "tune", "zerolatency", NULL);
    GstElement *video_pay = gst_element_factory_make("rtph264pay", "video_pay");
    g_object_set(video_pay, "config-interval", 1, NULL);
    GstElement *video_queue = gst_element_factory_make("queue", "video_queue");
    GstElement *video_sink = gst_element_factory_make("udpsink", "video_sink");
    g_object_set(video_sink, "host", "127.0.0.1", "port", 7002, NULL);


    gst_bin_add_many(GST_BIN(pipeline), audio_source, audio_convert, audio_queue, audio_encode, audio_pay, audio_sink, video_source, video_convert, video_encode, video_pay, video_queue, video_sink, NULL);
    gst_element_link_many(audio_source, audio_convert, audio_queue, audio_encode, audio_pay, audio_sink, NULL);
    gst_element_link_many(video_source, video_convert, video_encode, video_pay, video_queue, video_sink, NULL);

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