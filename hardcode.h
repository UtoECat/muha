std::string get_patch (std::string fileend);
void addfire (fire a);
void delfire (int k);
void error_cb (GstBus *bus, GstMessage *msg, gpointer data);
void eos_cb (GstBus *bus, GstMessage *msg, gpointer data);
void about_to_finish_han (GstElement *playbin,
gpointer udata);
std::string dblToStr (double d);
int LoadTexture (std::string FileName);
unsigned int* TexturesArrayInit (int count);
int LoadAlphaTexture (std::string FileName);
