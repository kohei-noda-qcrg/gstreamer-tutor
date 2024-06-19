#include <gst/gst.h>
#include <string>
int main(int argc, char *argv[]) {
    gst_init(&argc, &argv);

    if (argc != 3) {
        g_printerr("Usage: %s <host> <port>\n", argv[0]);
        return -1;
    }

    // host must be a valid IP address
    auto host = argv[1];
    if (!g_hostname_is_ip_address(host)) {
        g_printerr("Host must be a valid IP address!\n");
        return -1;
    }

    // port must be natural number
    auto port_num = atoi(argv[2]);
    const auto port = std::string(argv[2]);
    if (port_num <= 0) {
        g_printerr("Port must be a natural number!\n");
        return -1;
    }

    // gst-launch-1.0 videotestsrc ! videoconvert ! video/x-raw,width=720,height=480 ! x264enc ! "video/x-h264,profile=(string)high" ! rtph264pay config-interval=1 ! udpsink host=127.0.0.1 port=7001
    std::string pipeline_desc = "videotestsrc ! videoconvert ! video/x-raw,width=480,height=320 ! x264enc ! video/x-h264,profile=high ! rtph264pay config-interval=1 ! queue ! udpsink host=" + std::string(host) + " port=" + port;
    auto pipeline = gst_parse_launch(pipeline_desc.c_str(), NULL);
    gst_element_set_state(pipeline, GST_STATE_PLAYING);
    // wait until error or EOS
    auto bus = gst_element_get_bus(pipeline);
    auto msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE, GST_MESSAGE_ERROR | GST_MESSAGE_EOS);
    if (GST_MESSAGE_TYPE(msg) == GST_MESSAGE_ERROR) {
        g_printerr("An error occurred! Re-run with the GST_DEBUG=*:WARN environment variable set for more details.\n");
    }
    gst_message_unref(msg);
    gst_object_unref(bus);
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);
}