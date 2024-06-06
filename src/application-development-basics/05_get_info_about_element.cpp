#include <gst/gst.h>

int main(int argc, char *argv[])
{
    gst_init(&argc, &argv);

    auto factory = gst_element_factory_find("fakesrc");
    if (!factory)
    {
        g_print("You don't have the 'fakesrc' element installed!");
        return -1;
    }

    g_print(
        "The '%s' element is a member of the category %s.\n"
        "Description: %s\n",
        gst_plugin_feature_get_name(GST_PLUGIN_FEATURE(factory)),
        gst_element_factory_get_metadata(factory, GST_ELEMENT_METADATA_KLASS),
        gst_element_factory_get_metadata(factory, GST_ELEMENT_METADATA_DESCRIPTION));

    gst_object_unref(factory);
    return 0;
}