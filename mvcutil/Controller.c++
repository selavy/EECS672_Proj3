// Controller.c++: a basic Controller (in Model-View-Controller sense)

#include <GL/gl.h>
#include <GL/freeglut.h>

#include "Controller.h"
#include "ModelView.h"

Controller* Controller::curController = NULL;

Controller::Controller(const std::string& name, int glutRCFlags) :
	vpWidth(-1), vpHeight(1), doubleBuffering(false), glClearFlags(GL_COLOR_BUFFER_BIT)
{
	Controller::curController = this;

	// indicate we do not yet have any models by setting min to +1 and max to -1:
	overallWCBoundingBox[0] = overallWCBoundingBox[2] = overallWCBoundingBox[4] = 1.0;
	overallWCBoundingBox[1] = overallWCBoundingBox[3] = overallWCBoundingBox[5] = -1.0;

	// First create the window and its Rendering Context (RC)
	int windowID = createWindow(name, glutRCFlags);

	// Then initialize the newly created OpenGL RC
	establishInitialCallbacksForRC(); // the callbacks for this RC
}

Controller::~Controller()
{
	if (this == curController)
		curController = NULL;
}

void Controller::addModel(ModelView* m)
{
	if (m == NULL)
		return;
	models.push_back(m);
	updateWCBoundingBox(m);
}

bool Controller::checkForErrors(std::ostream& os, const std::string& context)
	// CLASS METHOD
{
	bool hadError = false;
	GLenum e = glGetError();
	while (e != GL_NO_ERROR)
	{
		os << "CheckForErrors (context: " <<  context
		   << "): " << (char*)gluErrorString(e) << std::endl;
		e = glGetError();
		hadError = true;
	}
	return hadError;
}

int Controller::createWindow(const std::string& windowTitle, int glutRCFlags) // CLASS METHOD
{
	// The following calls enforce use of only non-deprecated functionality.
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	// Now create the window and its Rendering Context.
	if (glutRCFlags != 0)
	{
		glutInitDisplayMode(glutRCFlags);
		doubleBuffering = (glutRCFlags & GLUT_DOUBLE) != 0;
		if ((glutRCFlags & GLUT_DEPTH) != 0)
			glClearFlags |= GL_DEPTH_BUFFER_BIT;
	}
	int windowID = glutCreateWindow(windowTitle.c_str());
	if ((glutRCFlags & GLUT_DEPTH) != 0)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
	return windowID;
}

void Controller::displayCB() // CLASS METHOD
{
	if (curController != NULL)
		curController->handleDisplay();
}

void Controller::establishInitialCallbacksForRC()
{
	glutDisplayFunc(Controller::displayCB);
	glutKeyboardFunc(Controller::keyboardCB);
	glutReshapeFunc(Controller::reshapeCB);
}

Controller* Controller::getCurrentController()
{
	return curController;
}

void Controller::getOverallWCBoundingBox(double* xyzLimits) const
{
	for (int i=0 ; i<6 ; i++)
		xyzLimits[i] = overallWCBoundingBox[i];
}

double Controller::getViewportAspectRatio() const
{
	return static_cast<double>(vpHeight) / static_cast<double>(vpWidth);
}

// For now, we assume the "region of interest" is just the whole model.
void Controller::getWCRegionOfInterest(double* xyzLimits) const
{
	for (int i=0 ; i<6 ; i++)
		xyzLimits[i] = overallWCBoundingBox[i];
}

void Controller::handleDisplay()
{
	// clear the frame buffer
	glClear(glClearFlags);

	for (std::vector<ModelView*>::iterator it=models.begin() ; it<models.end() ; it++)
		(*it)->render();

	if (doubleBuffering)
		glutSwapBuffers(); // does an implicit glFlush()
	else
		glFlush(); // force execution of queued OpenGL commands

	checkForErrors(std::cout, "Controller::handleDisplay");
}

void Controller::handleKeyboard(unsigned char key, int x, int y)
{
	const unsigned char ESC = 27;
	if (key == ESC)
		exit(0);

	// No other key is currently used by the Controller, so we just send
	// the event information to the model.

	double ldsX, ldsY; // only coord system known to both Controller and ModelView
	screenXYToLDS(x, y, ldsX, ldsY);

	// Pass the event to each registered ModelView. This approach works as long as
	// either (i) we are assured that at most one ModelView instance will respond, or
	// (ii) it doesn't matter if more than one respond.
	for (std::vector<ModelView*>::iterator it=models.begin() ; it<models.end() ; it++)
		(*it)->handleCommand(key, ldsX, ldsY);

	glutPostRedisplay();
}

void Controller::handleReshape()
{
	glViewport(0, 0, vpWidth, vpHeight);
}

void Controller::handleSpecialKey(int key, int x, int y)
{
	double ldsX, ldsY; // only coord system known to both Controller and ModelView
	screenXYToLDS(x, y, ldsX, ldsY);

	// Pass the event to each registered ModelView. This approach works as long as
	// either (i) we are assured that at most one ModelView instance will respond, or
	// (ii) it doesn't matter if more than one respond.
	for (std::vector<ModelView*>::iterator it=models.begin() ; it<models.end() ; it++)
		(*it)->handleSpecialCommand(key, ldsX, ldsY);

	glutPostRedisplay();
}

void Controller::keyboardCB(unsigned char key, int x, int y)
{
	if (Controller::curController != NULL)
		Controller::curController->handleKeyboard(key, x, y);
}

void Controller::removeAllModels(bool do_delete)
{
	std::vector<ModelView*>::iterator it = models.begin();
	while (it < models.end())
	{
		if (do_delete)
			delete *it;
		models.erase(it);
		it = models.begin();
	}
}

void Controller::removeModel(ModelView* m)
{
	for (std::vector<ModelView*>::iterator it=models.begin() ; it<models.end() ; it++)
	{
		if (*it == m)
		{
			models.erase(it);
			break;
		}
	}
}

void Controller::reportVersions(std::ostream& os)
{
	const char* glVer = reinterpret_cast<const char*>(glGetString(GL_VERSION));
	const char* glslVer = reinterpret_cast<const char*>
			(glGetString(GL_SHADING_LANGUAGE_VERSION));
	// glGetString can return NULL if no rendering context has been created
	os << "VERSIONS: GL: ";
	if (glVer == NULL)
		os << "NULL (has RC been created?)\n";
	else
		os << glVer << '\n';
	os << "        GLSL: ";
	if (glslVer == NULL)
		os << "NULL (has RC been created?)\n";
	else
		os << glslVer << '\n';
	if ((glVer == NULL) || (glslVer == NULL))
		return;
	int glutVer = glutGet(GLUT_VERSION);
	int glutVerMajor = glutVer/10000;
	int glutVerMinor = (glutVer % 10000) / 100;
	int glutVerPatch = glutVer % 100;
	os << "        GLUT: " << glutVerMajor << '.';
	if (glutVerMinor < 10)
		os << '0';
	os << glutVerMinor << '.';
	if (glutVerPatch < 10)
		os << '0';
	os << glutVerPatch << " (" << glutVer << ")\n";
#ifdef __GLEW_H__
	os << "        GLEW: " << glewGetString(GLEW_VERSION) << '\n';
#endif
}

void Controller::reshapeCB(int width, int height)
{
	if (Controller::curController != NULL)
	{
		Controller::curController->vpWidth = width;
		Controller::curController->vpHeight = height;
		Controller::curController->handleReshape();
	}
}

void Controller::screenXYToLDS(int x, int y, double& ldsX, double& ldsY)
{
	ldsX = 2.0 * static_cast<double>(x) / static_cast<double>(vpWidth) - 1.0;
	// The glut reports pixel coordinates assuming y=0 is at the top of the
	// window. The main OpenGL API assumes y=0 is at the bottom, so note we
	// account for that here.
	ldsY = 2.0 * static_cast<double>(vpHeight - y) / static_cast<double>(vpHeight) - 1.0;
}

void Controller::updateWCBoundingBox(ModelView* m)
{
	if (m == NULL)
		return;
	if (overallWCBoundingBox[0] <= overallWCBoundingBox[1])
	{
		// bounding box already initialized; just update it:
		double xyz[6];
		m->getWCBoundingBox(xyz);
		for (int i=0 ; i<5 ; i+=2)
		{
			if (xyz[i] < overallWCBoundingBox[i])
				overallWCBoundingBox[i] = xyz[i];
			if (xyz[i+1] > overallWCBoundingBox[i+1])
				overallWCBoundingBox[i+1] = xyz[i+1];
		}
	}
	else // use this model to initialize the bounding box
		m->getWCBoundingBox(overallWCBoundingBox);
}
