#include <gst/gst.h>
#include <string>
#include <limits>
#include <stdexcept>

#define UDP_PORT_MIN 0
#define UDP_PORT_MAX 65535

void print_usage(const char *program_name) {
    g_printerr("Usage: %s <host> <port>\n", program_name);
}

bool udp_port_is_valid(int port) {
    return port >= UDP_PORT_MIN && port <= UDP_PORT_MAX;
}

bool validate_port(const char *port) {
    try {
        auto port_num = std::stoi(port);
        if (!udp_port_is_valid(port_num)) {
            g_printerr("Port number limit exceeded!\n");
            return false;
        }
        g_print("Port number: %d\n", port_num);
    } catch (std::invalid_argument& e) {
        g_printerr("Port must be a number!\n");
        return false;
    }
    return true;
}

int main(int argc, char *argv[]) {
    gst_init(&argc, &argv);

    if (argc != 3) {
        print_usage(argv[0]);
        return -1;
    }

    // host must be a valid IP address
    auto host = argv[1];
    if (!g_hostname_is_ip_address(host)) {
        g_printerr("Host must be a valid IP address!\n");
        print_usage(argv[0]);
        return -1;
    }

    if (!validate_port(argv[2])) {
        print_usage(argv[0]);
        return -1;
    }
    const auto port = std::string(argv[2]);

    // gst-launch-1.0 videotestsrc ! videoconvert ! video/x-raw,width=720,height=480 ! x264enc ! "video/x-h264,profile=(string)high" ! rtph264pay config-interval=1 ! udpsink host=127.0.0.1 port=7001
    std::string video_setting = "! ";
    if (GST_VERSION_MAJOR == 1 && GST_VERSION_MINOR == 24 && GST_VERSION_MICRO == 4) {
        // Cannot use avdec_h264, use h264parse ! d3d11h264dec instead
        video_setting = "! video/x-h264,profile=(string)high ! ";
    }
    std::string pipeline_desc = "videotestsrc ! videoconvert ! video/x-raw,width=480,height=320 ! x264enc " + video_setting +  "rtph264pay config-interval=1 ! queue ! udpsink host=" + std::string(host) + " port=" + port;
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