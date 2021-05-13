/*
-----------------------------------------
*********** SLIME GAME ENGINE ***********
******     DRAW CODEBASE FILE      ******
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

#include <core.h>
#include <GL/gl.h>//GL & GLUT
#include <GL/glut.h>
#include <GL/freeglut.h>

namespace sge {

void NULLDrawFunc();

using tDrawFunc = void (*)();
tDrawFunc stdDrawFunc = NULLDrawFunc;

struct sgeWindow
{
GLuint id;
tDrawFunc windowDrawFunc = NULLDrawFunc; //TODO: MAKE MORE CALLBACKS
};

void setDrawFunc(sgeWindow &win, tDrawFunc func)
{
win.windowDrawFunc = func;
}

void setDrawFunc(tDrawFunc func)
{
stdDrawFunc = func; 
glutDisplayFunc(stdDrawFunc);
}

static bool SLIMEENGINE_DRAW_INIT_STATE = false;

void drawCycleInit ()
{
glEnable(GL_TEXTURE_2D); 
glEnable(GL_BLEND);
glEnable(GL_NORMALIZE);
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
glEnable(GL_ALPHA_TEST);
SLIMEENGINE_DRAW_INIT_STATE = true;  
}


void drawBegin(GLfloat RED_CLEAR_COLOR = 0, GLfloat GREEN_CLEAR_COLOR = 0, GLfloat BLUE_CLEAR_COLOR = 0, GLfloat ALPHA_COLOR = 1)
{
if (!SLIMEENGINE_DRAW_INIT_STATE) { drawCycleInit();};
glClearColor(RED_CLEAR_COLOR, GREEN_CLEAR_COLOR, BLUE_CLEAR_COLOR, ALPHA_COLOR);
glClear(GL_COLOR_BUFFER_BIT);
glMatrixMode(GL_PROJECTION);   //
glLoadIdentity();              //
glOrtho(0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), 0, -1.5, 1.5);//
glMatrixMode(GL_MODELVIEW);    //

glLoadIdentity();
glPushMatrix();
}

void drawEnd()
{
glPopMatrix();
glutSwapBuffers();
}


typedef int GLUT_DISPLAYMODE_FLAGS;
bool SGE_DRAW_INIT_STATE = false;

void drawInit (int argc,char **argv,GLUT_DISPLAYMODE_FLAGS flgs)
{
glutInit(&(argc),argv); //init glut
glutInitDisplayMode(flgs);
//glutDisplayFunc(stdDrawFunc);
SGE_DRAW_INIT_STATE = true;
}




void NULLDrawFunc()
{
drawBegin();

glColor4f(1,0,0,1);
glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0);glVertex3f(0,0,0);
		glTexCoord2f(1, 0);glVertex3f(1024,0,0);
		glTexCoord2f(1, 1.0);glVertex3f(1024,768,0);
        glTexCoord2f(0.0, 1.0);glVertex3f(0,768,0);
glEnd();

drawEnd();
}

using tDrawFunc = void (*)();


void createWindow (sgeWindow &window, const char* caption)
{
if (SGE_DRAW_INIT_STATE) 
{
window.id = glutCreateWindow(caption);
glutDisplayFunc(window.windowDrawFunc);
};
}

void controlWindow (sgeWindow &window, int winposx, int winposy, int winwigth, int winheight)
{
/*FGAPI void    FGAPIENTRY glutSetWindow( int window );
FGAPI int     FGAPIENTRY glutGetWindow( void );*/
int activeWindow = glutGetWindow();
glutSetWindow(window.id);
glutReshapeWindow(winwigth, winheight);
glutPositionWindow(winposx, winposy);
glutSetWindow(activeWindow);
}

void setWindow (sgeWindow win)
{
glutSetWindow(win.id);
}

}
