#include "bus.hpp"
#include "clock.hpp"

int main(int argc, char *argv[])
{
    gst_init(&argc, &argv);

    // Video and audio are streamed on different ports (rtp)
    // Audio: 7001
    // Video: 7002
    // gst-launch-1.0.exe -v udpsrc port=7001 caps="application/x-rtp" ! rtpopusdepay ! opusdec ! audioconvert ! queue ! autoaudiosink  udpsrc port=7002 caps="application/x-rtp,media=(string)video" ! rtph264depay ! avdec_h264 ! videoconvert ! queue ! autovideosink

    GMainLoop *loop = g_main_loop_new(NULL, FALSE);

    GstElement *pipeline = gst_pipeline_new("streaming");
    GstElement *audio_source = gst_element_factory_make("udpsrc", "audio_source");
    g_object_set(audio_source, "port", 7001, "caps", gst_caps_from_string("application/x-rtp"), NULL);
    GstElement *audio_depay = gst_element_factory_make("rtpopusdepay", "audio_depay");
    GstElement *audio_decode = gst_element_factory_make("opusdec", "audio_decode");
    GstElement *audio_convert = gst_element_factory_make("audioconvert", "audio_convert");
    GstElement *audio_queue = gst_element_factory_make("queue", "audio_queue");
    GstElement *audio_sink = gst_element_factory_make("autoaudiosink", "audio_sink");

    GstElement *video_source = gst_element_factory_make("udpsrc", "video_source");
    g_object_set(video_source, "port", 7002, "caps", gst_caps_from_string("application/x-rtp"), NULL);
    GstElement *video_rtp_jitter_buffer = gst_element_factory_make("rtpjitterbuffer", "video_rtp_jitter_buffer");
    g_object_set(video_rtp_jitter_buffer, "mode", 1, "do-retransmission", TRUE, "drop-on-latency", TRUE, "latency", 1000, NULL);
    GstElement *video_depay = gst_element_factory_make("rtph264depay", "video_depay");
    GstElement *video_decode = gst_element_factory_make("avdec_h264", "video_decode");
    GstElement *video_convert = gst_element_factory_make("videoconvert", "video_convert");
    GstElement *video_queue = gst_element_factory_make("queue", "video_queue");
    GstElement *video_sink = gst_element_factory_make("autovideosink", "video_sink");

    gst_bin_add_many(GST_BIN(pipeline), audio_source, audio_depay, audio_decode, audio_convert, audio_queue, audio_sink, video_source, video_rtp_jitter_buffer, video_depay, video_decode, video_convert, video_queue, video_sink, NULL);
    gst_element_link_many(audio_source, audio_depay, audio_decode, audio_convert, audio_queue, audio_sink, NULL);
    gst_element_link_many(video_source, video_rtp_jitter_buffer, video_depay, video_decode, video_convert, video_queue, video_sink, NULL);

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