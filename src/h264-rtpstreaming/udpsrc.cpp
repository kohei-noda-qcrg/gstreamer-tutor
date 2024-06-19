#include <gst/gst.h>
#include <string>
int main(int argc, char *argv[]) {
    if (argc != 2) {
        g_printerr("Usage: %s <port>\n", argv[0]);
        return -1;
    }
    const auto port_num = atoi(argv[1]);
    const auto port = std::string(argv[1]);
    if (port_num <= 0) {
        g_printerr("Port must be a natural number!\n");
        return -1;
    }
    gst_init(&argc, &argv);

    g_print("port_num: %d\n", port_num);
    // gst-launch-1.0.exe udpsrc port=port_num caps="application/x-rtp,media=(string)video" ! rtph264depay ! h264parse ! d3d11h264dec  ! videoconvert  ! queue ! autovideosink
    std::string pipeline_desc = "udpsrc port=" + port + " caps=\"application/x-rtp,media=(string)video\" ! rtph264depay ! h264parse ! d3d11h264dec  ! videoconvert  ! queue ! autovideosink";
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