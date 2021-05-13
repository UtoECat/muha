
/*
-----------------------------------------
*********** SLIME GAME ENGINE ***********
****** MAIN COREFILE AND CODE BASE ******
** VERSION 0.0 *** LICENSE GNU GPL 3.0 **
**** By UtoECat *************************
** MY SOCIAL : https://vk.com/utoecat
**           : https://www.youtube.com/
channel/UCNFNQFuXvCjmTdr2QfSAsvQ
**           : https://github.com/UtoECat
*****************************************
-----------------------------------------
*/

#ifndef __SLIMEENGINE_CORE_INCLUDE_STATE__
#define __SLIMEENGINE_CORE_INCLUDE_STATE__

#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/freeglut.h>

#define __SLIMEENGINE_CORE_VERSION 1.0
namespace sge
{

typedef unsigned char byte;
typedef void* tdataptr;

using tTexturearr = GLuint*;
using tIndex = unsigned int;

struct tVector {double x; double y;};
struct tColor {byte r; byte g; byte b;byte a;};

struct tPhysicvec
{
float KX; // SX + KX = SX;
float KY;
float SX; // POSX + SX = POSX;
float SY;
};

struct tVisual
{
tColor color;
int texture;
};

struct tTransformvec
{
tVector position;
tVector size;
float z;
float angle;
};

struct tGameObj
{
tTransformvec transform;
tPhysicvec    physic;
tVisual       visual;
tIndex        ID;
tIndex        parent;
tdataptr      data;
};

struct tScene
{
tGameObj *objects;

};

struct tMessage
{
int msgtype;
tdataptr msgdata;
};

class tParent
{
public:
virtual void onCreate (tGameObj &obj, tIndex index, tScene scene) = 0;
virtual void onDestroy (tGameObj &obj, tIndex index, tScene scene) = 0;
virtual void onProcess (tGameObj &obj, tIndex index, tScene scene) = 0;
virtual void onDraw (tGameObj &obj, tIndex index, tScene scene) = 0;
virtual bool onCollision (tGameObj &objwith, tGameObj &objto) = 0;
virtual void onMessage (tGameObj &obj, tGameObj &sender, tMessage *msg) = 0;
};

};
#endif 
