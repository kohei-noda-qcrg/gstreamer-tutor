#include <gst/gst.h>

auto print_structure(const GstStructure *structure) -> void{
    auto num_fields = gst_structure_n_fields(structure);

    for (auto i = 0; i < num_fields; i++){
        const gchar *name = gst_structure_nth_field_name(structure, i);
        const GValue *value = gst_structure_get_value(structure, name);
        g_print("    %s: %s\n", name, g_strdup_value_contents(value));
    }
}

auto print_caps (const GstCaps *caps)-> void {
    g_print("Caps: \n");
    for (auto i = 0; i < gst_caps_get_size(caps); i++){
        print_structure(gst_caps_get_structure(caps, i));
    }
}

int main(int argc, char *argv[]){
    gst_init(&argc, &argv);

    auto deviceMonitor = gst_device_monitor_new();
    // add filter (only video devices)
    gst_device_monitor_add_filter(deviceMonitor, "Video/Source", NULL);
    // add filter (only audio devices)
    // gst_device_monitor_add_filter(deviceMonitor, "Audio/Source", NULL);
    GList *list = gst_device_monitor_get_devices(deviceMonitor);
    for(GList *l = list; l != NULL; l = l->next){
        GstDevice *device = GST_DEVICE(l->data);
        GstCaps *caps = gst_device_get_caps(device);
        gchar *capsStr = gst_caps_to_string(caps);
        print_caps(caps);
        g_print("Device class: %s\n", gst_device_get_device_class(device));
        g_print("Device name: %s\n", gst_device_get_display_name(device));
    }
    g_free(list);
    gst_object_unref(deviceMonitor);
    gst_deinit();

    return 0;
}