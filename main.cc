#include <iostream> //iostream like cin and cout
#include <fstream>//saveload
#include <thread>   //maybe i will make it later...
#include <slimeengine/slimeengine> //SGE!
#include <string>    //STRINGS
#include <random>    //RANDOM GENERATOR
#include <sstream>   //FOR CONVVERTING
#include <gst/gst.h> //AUDIOLIB
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"//IMAGE LOAD!
#include <limits.h>
#include <unistd.h>

/*
-----------------------------------------
*********** GNU MUHA PROJECT# ***********
****** MAIN CODEFILE AND CODE BASE ******
** VERSION 0.2 *** LICENSE GNU GPL 3.0 **
**** By UtoECat *************************
** MY SOCIAL : https://vk.com/utoecat
**           : https://www.youtube.com/
channel/UCNFNQFuXvCjmTdr2QfSAsvQ
**           : https://github.com/UtoECat
*****************************************
-----------------------------------------
*/

const double pi = 3.141592653589793238463;//:D
const float muhasize = 50; 
const float size = 360 - muhasize;
#define arrsize 100

//#if _FUCKIN_COSTYL_
int *global_argc;
char **global_argv;
//#endif _FUCKIN_COSTYL_

std::string getpath()
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
static std::string argv_z_str = getpath();
std::string s =prfx + argv_z_str +
fileend;
std::cout << s <<std::endl;
return s;
}

static int score;
static int highscore; //TODO: MAKE IT SAVEABLE!

unsigned int *textures;//textures array;
int TEXCOUNT= 0; // count of textures


struct muha //muha
{
float x;
float y;
float speedx;
float speedy;
int ai;
};

muha* muhaarr = new muha[arrsize + 1];//muha's array :D
int fps = 0; //FPS
std::string fpsstr;//FPS

struct fire 
{
int x;
int y;
int power = 100;
int frame = random()%54;
};


#define firearrsize 50
fire *firearr = new fire[firearrsize];
int firecount = 0;

std::ofstream fout;
std::ifstream fin;

void delfire (int k)
{
//std::cout << "DELETING " << k <<" : "; DEBUG
if (k == firearrsize - 1) {firecount--; std::cout <<std::endl; return;};
for (int i = k; i < firearrsize - 1; ++i) firearr[i] = firearr[i + 1];
//std::cout <<"BOTTOM"<<std::endl; FOR DEBUG :D
firecount--;
}


void addfire (fire a)
{
//std::cout << "ADDING WHILE "<< firecount <<std::endl; DEBUG  TODO: MAKE DEBUG COMPILER KEY!
if (firecount >= firearrsize) delfire(0);
firearr[firecount] = a;
firecount++;
}

GstElement *bin; // GSTREAMER. MAIN MUSIC STREAM
GstBus *bus;
GstMessage *msg;

GstElement *sbin; // GSTREAMER. SOUND STREAM
GstBus *sbus;
GstMessage *smsg;

static void error_cb (GstBus *bus, GstMessage *msg, gpointer data) {
  GError *err;gchar *debug_info;
  gst_message_parse_error (msg, &err, &debug_info);
  g_printerr ("Error in element %s: %s\n", GST_OBJECT_NAME (msg->src), err->message);
  g_printerr ("INFORMATION: %s\n", debug_info ? debug_info : "none");
  g_clear_error (&err);
  g_free (debug_info);
  gst_element_set_state (bin, GST_STATE_READY);
}

static void eos_cb (GstBus *bus, GstMessage *msg, gpointer data) {
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

bool collisionm (muha &p, muha &w) // COLLISION BETWEEN MUHA P AND W
{
if ( (
(p.x < w.x + muhasize) &&
(p.x+ muhasize > w.x )
) && (
(p.y < w.y + muhasize) &&
(p.y+ muhasize > w.y )
) ) return true; else return false;
}

bool collisionm (muha &p, fire &w) // COLLISION BETWEEN MUHA P AND W
{
if ( (
(p.x < w.x + muhasize*2) &&
(p.x+ muhasize > w.x )
) && (
(p.y < w.y + muhasize*2) &&
(p.y+ muhasize > w.y )
) ) return true; else return false;
}

bool collisionFire (muha &p)
{
bool r = false;
for (int i = 0; i < firecount; i++) if (collisionm (p,firearr[i])&& (firearr[i].power > 60) && (muhaarr[i].ai != 6)) return true;
return r;
}





bool collisionall (muha &p,int index)
{
bool r = false;
for (int i = 0; i <= arrsize; i++) if ((i != index)&&collisionm (p,muhaarr[i])&& (muhaarr[i].ai != 6)&& (muhaarr[i].ai != 9)) return true;
return r;
}

int collisionallget (muha &p)
{
bool r = false;
for (int i = 0; i <= arrsize; i++) if ( collisionm (p,muhaarr[i]) && (muhaarr[i].ai != 6) && (muhaarr[i].ai != 9)) return i;
return -1;
}

void speed(float &spd) //PANIC SPEED
{
spd = spd * ((random() %101) - 50) / 20.0;
if (spd == 0) spd = 1;
if (abs(spd) > 20) spd = spd / 2;
if (abs(spd) < 0.5) spd = spd * 2;
}

/*int point = 0;
void speedx(float &spd)
{
spd = cos(2 * pi * point / 360) * 0.5 + 0;
}

void speedy(float &spd)
{
spd = sin(2 * pi * point / 360) * 0.5 + 0;
point++; if (point > 360) point = 0;
}*/

bool limitm (float &x, float &y) // LIMIT MUHA MOVEMENT
{
    bool mo = false;
 if (x > glutGet(GLUT_WINDOW_WIDTH)) {x =  glutGet(GLUT_WINDOW_WIDTH);mo = true;}; if (x < 0){ x = 0; mo = true;};
 if (y >  glutGet(GLUT_WINDOW_HEIGHT)) {y =  glutGet(GLUT_WINDOW_HEIGHT); mo = true;}; if (y < 0) { y = 0; mo = true;};
return mo;
}

float GetAngle (float ax,float ay,float bx,float by) // СКАЛЯРНОЕ ПРОИЗВЕДЕНИЕ ВЕКТОРОВ
{
return (ax * bx + ay * by) / (  sqrt( pow(ax, 2) + pow(ay, 2)) * sqrt( pow(bx, 2) + pow(by, 2))  );
}

bool fireshow = false;

void fOnMouse(int pinn, int i2, int x, int y) // KILL HIM!!!
{
if ((i2 == 0)&&(pinn == 0)) {
muha mouse;mouse.x = x;mouse.y = y;
int cache = collisionallget(mouse);
if ( (cache != -1) && (muhaarr[cache].ai != 6) && (muhaarr[cache].ai != 9)) {muhaarr[cache].ai = 6;
gst_element_set_state (sbin, GST_STATE_READY);//start playing
gst_element_set_state (sbin, GST_STATE_PLAYING);//start playing
score = score + 5;
};
}

if ((i2 == 1)&&(pinn == 2)) fireshow = false; 

if ((i2 == 0)&&(pinn == 2)) 
{
fire fr; fr.x = x; fr.y = y; fr.power = 100;
fr.frame = random()%54;
addfire(fr);
fireshow = true;
};

//std::cout << pinn << " : " << i2 << std::endl; DEBUG

}

void CalculateMuhas()
{
muha *m;
for (int i = 0; i <= arrsize; i++)
{
m = &muhaarr[i];
if (m->ai == 0) // MUHA PANIC AI STATE (FLYING)
{
m->x = m->x + m->speedx; speed(m->speedx);
m->y = m->y + m->speedy; speed(m->speedy);
if (random() % 45 == 1) m->ai = 1;
}

else if (m->ai == 1) // MUHA GOING AI STATE
{
m->x = m->x + m->speedx;
m->y = m->y + m->speedy;
if (collisionall(*m,i) && (random()%60 == 1)) m->ai = 0;
if (random() % 15 == 1) m->ai = 2;
if (random() % 45 == 1) m->ai = 0;
}

else if (m->ai == 2) // MUHA IDLE AI STATE
{
if (collisionall(*m,i) && (random()%40 == 1)) m->ai = 0;
if (random() % 95 == 1) m->ai = 1;
if (random() % 115 == 1) speed(m->speedx);
if (random() % 115 == 1) speed(m->speedy);
}

else if (m->ai == 9) // MUHA BURNED AI
{
if ((random() % 700 == 1)) {m->ai = 6; m->x = random()%glutGet(GLUT_WINDOW_WIDTH);m->y = random()%glutGet(GLUT_WINDOW_HEIGHT); m -> speedx = 0;};
}


else if (m->ai == 6) // MUHA DEAD STYLE :D AI
{
m->speedx++;
if ((random() % 90 == 1) &&(m->speedx > 1000)) {m->ai = 2; m->x = random()%glutGet(GLUT_WINDOW_WIDTH);m->y = random()%glutGet(GLUT_WINDOW_HEIGHT); m -> speedx = 0;};
};

if( (m -> ai != 9) && (collisionFire(*m))) 
{if(m -> ai != 6)score++;m->ai = 9; };

if ( limitm(m->x,m->y) && (m->ai = 1) ) {m->ai = 2; speed(m->speedy);speed(m->speedx);}; //IF TOUCH BORDER - ROTATE


};

fire *f;
for (int i = 0; i < firearrsize; ++i)
{
f = &firearr[i];
if (f->power > 0) f->power= f->power - 4;
}

}

void MyDrawFunc()
{
sge::drawBegin();

static muha *m ; static float angle = 0; static muha cache;
for (int i = 0; i <= arrsize; i++)
{
glPushMatrix();
m = &muhaarr[i];

glColor4f(i/(arrsize*1.0),1,i/(arrsize*1.0),1);
glBindTexture(GL_TEXTURE_2D, textures[0]);

if (m->ai == 6) 
{glColor4f(i/(arrsize*1.0),1,i/(arrsize*1.0),((1000 - m->speedx)/1000));
glBindTexture(GL_TEXTURE_2D, textures[1]);};

angle = acos(GetAngle (0,10,m->speedx, m->speedy * -1))*180/pi;
if (m->speedx < 0) angle = angle * -1;

if (m->ai == 9) 
{glColor4f(1,1,1,((1000 - m->speedx)/1000));
glBindTexture(GL_TEXTURE_2D, textures[8]);};

glTranslatef(m->x,m->y,0);
glRotatef(angle,0,0,1);
    glPointSize(1);
glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0);glVertex3f(-muhasize/2,-muhasize/2,0);
		glTexCoord2f(1, 0);glVertex3f(muhasize/2,-muhasize/2,0);
		glTexCoord2f(1, 1.0);glVertex3f(muhasize/2,muhasize/2,0);
        glTexCoord2f(0.0, 1.0);glVertex3f(-muhasize/2,muhasize/2,0);
glEnd();

glPopMatrix();
}
  
glPushMatrix();
fire *f;
for (int i = 0; i < firearrsize; ++i)
{
f = &firearr[i];
glPushMatrix();
glTranslatef(f->x,f->y,0.9);
glColor4f(1,1,1,f->power/150.0);
f->frame++; if (f->frame > 54) f->frame= 0;
glBindTexture(GL_TEXTURE_2D, textures[2 + (f->frame/10)]);
glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0);glVertex3f(-muhasize,-muhasize,0);
		glTexCoord2f(1, 0);glVertex3f(muhasize,-muhasize,0);
		glTexCoord2f(1, 1.0);glVertex3f(muhasize,muhasize,0);
        glTexCoord2f(0.0, 1.0);glVertex3f(-muhasize,muhasize,0);
glEnd();
glPopMatrix();
}; 

glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, 0);
    glPushMatrix();
    glTranslatef(3.0,35,1);  
    glScalef(0.1,-0.1,0.007);
    glColor4f(1,1,1,1); 
    glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*) (fpsstr.c_str()) );
    
    glPopMatrix();
    glPushMatrix();
     glTranslatef(3.0,20,1);  
    glScalef(0.15,-0.15,0.007);
    glColor4f(1,1,1,1); 
    static int scrclr = 0;
    scrclr++;
    if (scrclr>100) scrclr = 0;
    if ((scrclr>33)&&(score>highscore))        glColor4f(0.8,0,0,1);
    if ((scrclr>66)&&(score>highscore))        glColor4f(1,0,0,1);
    glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*) (("Score : " + dblToStr(score)).c_str()) );

    glPopMatrix();

sge::drawEnd();
}

void fMouseMove (int x, int y)
{
muha mice; mice.x = x; mice.y = y;
fire mouse;mouse.x = x;mouse.y = y;
mouse.frame = random()%54;
if (fireshow) addfire(mouse);
if ((random()%25) == 1)
{
 int indx = collisionallget(mice);
 if (indx == -1) return;
 muhaarr[indx].ai = 0;
};
}

void EMPITYDRAW () {}

void myFPS()
{
    static int start = glutGet(GLUT_ELAPSED_TIME);
    static int frames = 0;
    int timer = glutGet(GLUT_ELAPSED_TIME)-start;
    frames++;
 
    if((timer) >= 1000)
    {
        fps = frames; 
        frames = 0;
        timer = 0;
        start = glutGet(GLUT_ELAPSED_TIME);
        fpsstr = "FPS : " + dblToStr(fps);

        if (score > highscore){
        fout.open("score.cfg");
        highscore = score;
        fout << highscore;
        fout.close();
     };
    }
}





int LoadTexture (std::string FileName);
unsigned int* TexturesArrayInit (int count);
int LoadAlphaTexture (std::string FileName);

int main(int argc, char** argv) 
{ 
    global_argc = &argc;
    global_argv = argv;
    for (int i = 0; i < arrsize; i++){ muhaarr[i].x = random()%1025; muhaarr[i].y = random()%769; muhaarr[i].ai = 0;};
    sge::drawInit (argc,argv,GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    sge::sgeWindow mywindow;
    sge::createWindow (mywindow, "TEST");
    sge::controlWindow(mywindow, 0, 0, 1024, 768);
    sge::setDrawFunc(MyDrawFunc);
    glutMouseFunc(fOnMouse);
    glutMotionFunc(fMouseMove);
      gst_init (&argc, &argv);

fin.open("score.cfg");
fin >> highscore;
fin.close();

      bin = gst_element_factory_make ("playbin", "MyPlayBin");
  if (!bin) { g_printerr ("Not all elements could be created.\n"); return -1;}
  /* Modify the source's properties */
  g_object_set (bin, "uri", get_patch("/muhi.mp3").c_str(), NULL);//first set
  g_signal_connect (bin, "about_to_finish", G_CALLBACK (about_to_finish_han), NULL);//setup callback

  gst_element_set_state (bin, GST_STATE_PLAYING);//start playing
  bus = gst_element_get_bus (bin);/* Wait until error or EOS */ // NOW EOS - IMPOSSIBLE! ONLY ERROR!
  gst_bus_add_signal_watch (bus);
  g_signal_connect (G_OBJECT (bus), "message::error", (GCallback)error_cb, NULL);
  g_signal_connect (G_OBJECT (bus), "message::eos", (GCallback)eos_cb, NULL);
  gst_object_unref (bus);

 sbin = gst_element_factory_make ("playbin", "MySNDBin");
 g_object_set (sbin, "uri", get_patch("/kill.mp3").c_str(), NULL);//first set
 //g_signal_connect (sbin, "about_to_finish", G_CALLBACK (about_to_finish_han), NULL);//setup callback
 gst_element_set_state (sbin, GST_STATE_READY);//start playing
 sbus = gst_element_get_bus (sbin);/* Wait until error or EOS */ // NOW EOS - IMPOSSIBLE! ONLY ERROR!
 gst_bus_add_signal_watch (sbus);
  g_signal_connect (G_OBJECT (sbus), "message::error", (GCallback)error_cb, NULL);
  g_signal_connect (G_OBJECT (sbus), "message::eos", (GCallback)eos_cb, NULL);
  gst_object_unref (sbus);



  textures = TexturesArrayInit(10);
  LoadAlphaTexture("muha.png");
  LoadAlphaTexture("f.png");
  LoadAlphaTexture("fire1.png");
  LoadAlphaTexture("fire2.png");
  LoadAlphaTexture("fire3.png");
  LoadAlphaTexture("fire4.png");
  LoadAlphaTexture("fire5.png");
  LoadAlphaTexture("fire6.png");
  LoadAlphaTexture("mumu.png");

    while (true) {
glutMainLoopEvent();
glutPostRedisplay();
CalculateMuhas();
//glutSetWindow(mywindow.id);MyDrawFunc();
myFPS();
};
    return 0; 
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
