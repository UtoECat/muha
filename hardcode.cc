#include <iostream> //iostream like cin and cout
#include <fstream>//saveload
#include <thread>   //maybe i will make it later...
#include <string>    //STRINGS
#include <random>    //RANDOM GENERATOR
#include <sstream>   //FOR CONVVERTING
#include <gst/gst.h> //AUDIOLIB
#include <limits.h>
#include <unistd.h>
#include "defines.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"//IMAGE LOAD!

unsigned int *textures;
int TEXCOUNT= 0; // count of textures

extern const int firearrsize;
fire *firearr = new fire[firearrsize];
int firecount = 0;

void delfire (int k)
{
if (k == firearrsize - 1) {firecount--; std::cout <<std::endl; return;};
for (int i = k; i < firearrsize - 1; ++i) firearr[i] = firearr[i + 1];
firecount--;
}


void addfire (fire a)
{
if (firecount >= firearrsize) delfire(0);
firearr[firecount] = a;
firecount++;
}


std::string systemgetpath()
{
  char result[ PATH_MAX ];
  ssize_t count = readlink( "/proc/self/exe", result, PATH_MAX );
  std::string str ( result, (count > 0) ? count : 0 );
    return str.substr(0,str.find_last_of("/"));
}

std::string get_patch (std::string fileend)
{
//YOU PROBABLY WILL KILL ME FOR THIS
static std::string prfx = "file://";
static std::string argv_z_str = systemgetpath();
std::string s =prfx + argv_z_str +
fileend;
std::cout << s <<std::endl;
return s;
}

extern GstElement *bin;

void error_cb (GstBus *bus, GstMessage *msg, gpointer data) {
  GError *err;gchar *debug_info;
  gst_message_parse_error (msg, &err, &debug_info);
  g_printerr ("Error in element %s: %s\n", GST_OBJECT_NAME (msg->src), err->message);
  g_printerr ("INFORMATION: %s\n", debug_info ? debug_info : "none");
  g_clear_error (&err);
  g_free (debug_info);
  gst_element_set_state (bin, GST_STATE_READY);
}

void eos_cb (GstBus *bus, GstMessage *msg, gpointer data) {
  //g_print ("Error. End-Of-Stream reached.\n");
  gst_element_set_state (bin, GST_STATE_READY);
}

void about_to_finish_han (GstElement *playbin,
gpointer udata) // Я ХАН ЁПТА :DD
{
g_object_set (playbin, "uri",
get_patch("/muhi.mp3").c_str(), NULL);
}

std::string dblToStr (double d)
{
std::stringstream sstr;
	sstr.unsetf(std::ios_base::floatfield);
	sstr.unsetf(std::ios_base::fixed);
	sstr.unsetf(std::ios_base::scientific);
sstr << d;
return sstr.str();
}

unsigned int* TexturesArrayInit (int count)
{
unsigned int *arr = new unsigned int [count];
//glGenTextures(count, arr);
std::cout <<  " : " << count;
std::cout << "GENERATED SPACE FOR TEXTURES" <<std::endl;
return arr;
}

int LoadTexture (std::string FileName)
{
unsigned int txtr;
glGenTextures(1, &txtr);
glBindTexture(GL_TEXTURE_2D, txtr);
 
// Устанавливаем параметры наложения и фильтрации текстур (для текущего связанного объекта текстуры)
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
 
// Загрузка и генерация текстуры
int width, height, nrChannels;
unsigned char *data = stbi_load(FileName.c_str(), &width, &height, &nrChannels, 0);
if (data)
{
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    textures[TEXCOUNT] = txtr;
    TEXCOUNT++;
    std::cout << "TEXTURES HAVE BEEN LOADED FROM " << FileName << " TO ID :"<< TEXCOUNT - 1 << std::endl;
    return TEXCOUNT - 1;
}
else
{
    std::cout << "Failed to load texture from " << FileName << std::endl;
    return -1;
}
stbi_image_free(data);
}

int LoadAlphaTexture (std::string FileName)
{
unsigned int txtr;
glGenTextures(1, &txtr);
glBindTexture(GL_TEXTURE_2D, txtr);
 
// Устанавливаем параметры наложения и фильтрации текстур (для текущего связанного объекта текстуры)
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
 
// Загрузка и генерация текстуры
int width, height, nrChannels;
unsigned char *data = stbi_load(FileName.c_str(), &width, &height, &nrChannels, 0);
if (data)
{
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    textures[TEXCOUNT] = txtr;
    TEXCOUNT++;
    std::cout << "TEXTURES HAVE BEEN LOADED FROM " << FileName << " TO ID :"<< TEXCOUNT - 1 << std::endl;
    return TEXCOUNT - 1;
}
else
{
    std::cout << "Failed to load texture from " << FileName << std::endl;
    return -1;
}
stbi_image_free(data);
}
