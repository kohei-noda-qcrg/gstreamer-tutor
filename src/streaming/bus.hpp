#pragma once
#include <gst/gst.h>

gboolean gst_bus_call(GstBus *bus, GstMessage *msg, gpointer data);