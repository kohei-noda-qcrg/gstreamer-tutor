#include <gst/gst.h>

int main(int argc, char *argv[]){
    // // Method: 1
    // GstElement *element;
    // gst_init(&argc,&argv);

    // element = gst_element_factory_make("fakesrc", "source");
    // if(!element){
    //     g_print("Failed to create element of type 'fakesrc'\n");
    //     return -1;
    // }

    // gst_object_unref(GST_OBJECT(element));


    // Method: 2
    gst_init(&argc,&argv);
    auto factory = gst_element_factory_find("fakesrc");
    if (!factory){
        g_print("Failed to find factory of type 'fakesrc'\n");
        return -1;
    }

    auto element = gst_element_factory_create(factory, "src");
    if (!element){
        g_print("Failed to craete element, even though its factory exists!\n");
    }

    gst_object_unref(GST_OBJECT(factory));
    gst_object_unref(GST_OBJECT(element));
    return 0;
}