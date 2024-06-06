#include <gst/gst.h>

int main(int argc, char *argv[]){
    
    gst_init(&argc,&argv);

    auto element = gst_element_factory_make("fakesrc", "source");

    auto elem_name = gst_object_get_name(GST_OBJECT(element));

    gchar *name;
    g_object_get(G_OBJECT(element), "name", &name, NULL);
    g_print("The name of the element is '%s and %s'.\n", name, elem_name);
    g_free(name);

    gst_object_unref(GST_OBJECT(element));
    return 0;
}