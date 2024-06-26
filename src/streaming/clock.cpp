#include "clock.hpp"

gboolean cb_print_position(GstElement *pipeline)
{
    gint64 pos, len;
    if (gst_element_query_position(pipeline, GST_FORMAT_TIME, &pos) && gst_element_query_duration(pipeline, GST_FORMAT_TIME, &len))
    {
        GstClockTime clock_time = gst_element_get_current_clock_time(pipeline);
        GstClockTime base_time = gst_element_get_base_time(pipeline);
        GstClockTime running_time = gst_element_get_current_running_time(pipeline);
        g_print("Time: %" GST_TIME_FORMAT " / %" GST_TIME_FORMAT ", "
                "clock_time=%" GST_TIME_FORMAT ", base_time=%" GST_TIME_FORMAT
                ", running_time=%" GST_TIME_FORMAT ", clock_time-base_time=%" GST_TIME_FORMAT "\r",
                GST_TIME_ARGS(pos), GST_TIME_ARGS(len),
                GST_TIME_ARGS(clock_time), GST_TIME_ARGS(base_time),
                GST_TIME_ARGS(running_time), GST_TIME_ARGS(clock_time - base_time));
    }

    /* call me again */
    return TRUE;
}
