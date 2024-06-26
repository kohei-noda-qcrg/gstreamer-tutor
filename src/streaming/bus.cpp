#include "bus.hpp"

gboolean gst_bus_call(GstBus *bus, GstMessage *msg, gpointer data)
{
    GMainLoop *loop = (GMainLoop *)data;

    switch (GST_MESSAGE_TYPE(msg))
    {
    case GST_MESSAGE_EOS:
        g_print("End of stream\n");
        g_main_loop_quit(loop);
        break;

    case GST_MESSAGE_ERROR:
    {
        gchar *debug;
        GError *error;

        gst_message_parse_error(msg, &error, &debug);

        g_printerr("Error: %s\n", error->message);
        g_printerr("Debug details: %s\n", debug);
        g_free(debug);
        g_free(error);
        g_error_free(error);

        g_main_loop_quit(loop);
        break;
    }
    case GST_MESSAGE_STATE_CHANGED:
    {
        GstState old_state, new_state, pending_state;
        gst_message_parse_state_changed(msg, &old_state, &new_state, &pending_state);
        g_print("Element %s changed state from %s to %s\n", GST_OBJECT_NAME(msg->src), gst_element_state_get_name(old_state), gst_element_state_get_name(new_state));
        return TRUE;
    }
    case GST_MESSAGE_STREAM_STATUS:
    {
        GstStreamStatusType type;
        GstElement *owner;
        gst_message_parse_stream_status(msg, &type, &owner);
        g_print("Stream status message from %s\n", GST_ELEMENT_NAME(owner));
        g_debug("test");
        return TRUE;
    }
    case GST_MESSAGE_NEW_CLOCK:
    {
        GstClock *clock = NULL;
        gst_message_parse_new_clock(msg, &clock);
        if (clock != NULL)
        {
            g_print("New clock: %s\n", GST_OBJECT_NAME(clock));
        }
        else
        {
            g_print("New clock: NULL\n");
        }
    }
    case GST_MESSAGE_STREAM_START:
    {
        g_print("Stream started\n");
        return TRUE;
    }
    case GST_MESSAGE_LATENCY:
    {
        g_print("Latency message from %s\n", GST_OBJECT_NAME(msg->src));
        return TRUE;
    }
    default:
        break;
    }

    return TRUE;
}
