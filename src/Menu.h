#pragma once

#include <GL/glut.h>

bool isReflectionOn = true;
bool areBoundingBoxesOn = true;
bool isFogOn = true;
bool areParticlesOn = true;
bool areShadowsOn = true;
bool isLightArrowOn = true;
bool areCurveLinesOn = true;
bool areCurvePointsOn = true;
bool areLightsOn = true;
bool areTexturesWithImagesOn = true;
bool isProceduralTextureGenOn = true;
bool areMaterialsOn = false;
bool isGameModeOn = true;
bool areCollisionsOn = true;
bool isMultiscaleTerrainOn = true;
bool areRecursiveFractalsOn = true;
bool areNURBsGridsOn = true;

void menuListener(int option) {
	switch (option) {
	case 0:
		isReflectionOn = !isReflectionOn;
		break;
	case 1:
		areBoundingBoxesOn = !areBoundingBoxesOn;
		break;
	case 2:
		isFogOn = !isFogOn;
		break;
	case 3:
		areParticlesOn = !areParticlesOn;
		break;
	case 4:
		areShadowsOn = !areShadowsOn;
		break;
	case 5:
		isLightArrowOn = !isLightArrowOn;
		break;
	case 6:
		areCurveLinesOn = !areCurveLinesOn;
		break;
	case 7:
		areCurvePointsOn = !areCurvePointsOn;
		break;
	case 8:
		areLightsOn = !areLightsOn;
		break;
	case 9:
		areTexturesWithImagesOn = !areTexturesWithImagesOn;
		break;
	case 10:
		isProceduralTextureGenOn = !isProceduralTextureGenOn;
		break;
	case 11:
		areMaterialsOn = !areMaterialsOn;
		break;
	case 12:
		isGameModeOn = !isGameModeOn;
		break;
	case 13:
		areCollisionsOn = !areCollisionsOn;
		break;
	case 14:
		isMultiscaleTerrainOn = !isMultiscaleTerrainOn;
		break;
	case 15:
		areRecursiveFractalsOn = !areRecursiveFractalsOn;
		break;
	case 16:
		areNURBsGridsOn = !areNURBsGridsOn;
		break;
	}
	glutPostRedisplay();
}

void addMenu() {
	int optionsMenu = glutCreateMenu(menuListener);

	//add entries to submenu Colores
	glutAddMenuEntry("Toggle Reflection", 0);
	glutAddMenuEntry("Toggle Bounding Boxes", 1);
	glutAddMenuEntry("Toggle Fog", 2);
	glutAddMenuEntry("Toggle Particles", 3);
	glutAddMenuEntry("Toggle Shadow", 4);
	glutAddMenuEntry("Toggle Light Arrow", 5);
	glutAddMenuEntry("Toggle Curves", 6);
	glutAddMenuEntry("Toggle Curve Points", 7);
	glutAddMenuEntry("Toggle Light", 8);
	glutAddMenuEntry("Toggle Textures with images", 9);
	glutAddMenuEntry("Toggle Procedural Texture Generation", 10);
	glutAddMenuEntry("Toggle Materials", 11);
	glutAddMenuEntry("Toggle Game Mode", 12);
	glutAddMenuEntry("Toggle Collisions", 13);
	glutAddMenuEntry("Toggle Multiscale Terrain", 14);
	glutAddMenuEntry("Toggle Recursive Fractals", 15);
	glutAddMenuEntry("Toggle NURBS grid", 16);

	// create main menu
	int menu = glutCreateMenu(menuListener);
	glutAddSubMenu("Options", optionsMenu);

	// attach the menu to the right button
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}
