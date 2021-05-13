
/*
-----------------------------------------
*********** SLIME GAME ENGINE ***********
******    DRAW CODEBASE HEADER     ******
********** API : OPENGL (GLUT) **********
** VERSION 0.0 *** LICENSE GNU GPL 3.0 **
**** By UtoECat *************************
** MY SOCIAL : https://vk.com/utoecat
**           : https://www.youtube.com/
channel/UCNFNQFuXvCjmTdr2QfSAsvQ
**           : https://github.com/UtoECat
*****************************************
-----------------------------------------
*/
#ifndef __SLIMEENGINE_DRAW_INCLUDE_STATE__
#define __SLIMEENGINE_DRAW_INCLUDE_STATE__

#include <core.h>

#define glutPopPush glutPopPushMatrix
/* THIS FUNCTION USES FOR MAKE A PopMatrix-PushMatrix Conveyor */
void glutPopPushMatrix()
{
glPopMatrix;
glPushMatrix;
}



namespace sge
{

void NULLDrawFunc();/* STD EMPITY AND DEBUG FUNCTION FOR GLUT Redisplay Callback */

typedef int GLUT_DISPLAYMODE_FLAGS; // Flags, that used to setup display modes : GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH for example
using tDrawFunc = void (*)(); // Type for std glut redisplay callback func
struct sgeWindow // WINDOW TYPE (TODO : MOVE TO WINDOW.H FILE!!!)
{
GLuint id;
tDrawFunc windowDrawFunc = NULLDrawFunc; //TODO: MAKE MORE CALLBACKS
};

void drawInit (int argc,char **argv,GLUT_DISPLAYMODE_FLAGS flgs); // Init draw... 

void createWindow (sgeWindow &window, const char* caption); // TODO : MOVE TO WINDOW!
void controlWindow (sgeWindow &window, int winposx, int winposy, int winwigth, int winheight); // TODO : MOVE TO WINDOW.H!
void setWindow (sgeWindow win); // TODO : MOVE TO SYNTAX SUGAR!

void setDrawFunc(sgeWindow &win, tDrawFunc func); // SET DRAW FUNCTION TO WINDOW

extern bool SLIMEENGINE_DRAW_INIT_STATE; // ANTI-IDIOT SEQURITY!

void drawBegin(GLfloat RED_CLEAR_COLOR = 0, GLfloat GREEN_CLEAR_COLOR = 0, GLfloat BLUE_CLEAR_COLOR = 0, GLfloat ALPHA_COLOR = 1);
void drawEnd();
/* THESE TWO FUNCTIONS ARE USED TO SET BEGIN AND END DRAWCYCLE! IT MEANS, THAT THEY MAKE READY
ALL IMPORTANT STUFF TO BEGIN DRAWING PROCESS! USE ALL OF THEM ONLY ONSE IN CYCLE!!!! 
EXAMPLE :
while(true)
{
drawBegin(0,0,0,1);

..drawing scenes process

drawEnd();
P.S DRAWCYCLE IS PROBABLY GLUT REDRAW CALLBACK FUNCTION!
} */

extern tDrawFunc stdDrawFunc; // stdDrawFunc.. DEPRECATED!!!
void setDrawFunc(tDrawFunc func); // PROBABLY DEPRECATED,  BUT DON'T REMOVE!!!   DDOOONN'TT!!!

}
#endif 
