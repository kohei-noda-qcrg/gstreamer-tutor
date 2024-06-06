#include <gst/gst.h>

int main(int argc, char *argv[]){

    gst_init(&argc,&argv);

    auto player = gst_element_factory_make("oggvorbisplayer", "player");
    g_object_set(G_OBJECT(player), "localtion", "helloworld.ogg", NULL);

    gst_element_set_state(GST_ELEMENT(player), GST_STATE_PLAYING);
}