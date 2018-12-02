/**
 * SER 431
 * https://speakerdeck.com/javiergs/ser431-lecture-04
 By Preston Goulet(pegoulet) & Taylor Greeff(tgreeff)

 TODO: fix camera; allow use of mouse to move
 **/

#include "Common.h"

// display
void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
	//Updates
	float time = calculate_frame_time();
	ps.updateParticles(time);
	rain.updateParticles(time);
	leaves.updateParticles(time);
	jetPath.update();
	jetPath.updateRotation();
	updateGameObjects(time);
	updateLights();
	updateCamera();

	// projection
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glViewport(0, 0, window_width, window_height);
	gluPerspective(45, window_ratio, 10, 100000);
	// view
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	//===========================================================================
	//  Camera
	//===========================================================================
	gluLookAt(camera_x, camera_y, camera_z,
		camera_x + lookx, camera_y + looky, camera_z + lookz,
		0.0f, 1.0f, 0.0f);

	glPushMatrix();
	glScalef(scale, scale, scale);
	glTranslatef(0.0f, 0.0f, 0.0f);
	glPopMatrix();


	// Raises and lowers the boxes to reveal the flames
	if (raiseAndLower) {
		if (lowerBox == false && boxMovement < 200) {
			boxMovement += 1;
			if (boxMovement > 199) {
				lowerBox = true;
			}
		}
		else {
			boxMovement -= 1;
			if (boxMovement == 0) {
				raiseAndLower = false;
				lowerBox = false;
				doOnce = true;
				generateRandomNumber();
			}
		}
	}

	//====================
	//	Draw Objects
	//====================
 	//flag
	glPushMatrix();
	glTranslatef(400, shadowHeight + 700, playArea);
	glScalef(100, 100, 100);
	draw_nurb(textures[9]);
	glPopMatrix();	

	if (isReflectionOn) {
		// boxes reflections
		glPushMatrix();
		glTranslatef(0, shadowHeight, playArea);
		glScalef(1.0, -10.0, 1.0);
		glCallList(display2);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(200, shadowHeight, playArea);
		glScalef(1.0, -1.0, 1.0);
		glCallList(display3);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(-200, shadowHeight, playArea);
		glScalef(1.0, -1.0, 1.0);
		glCallList(display4);
		glPopMatrix();

		//flag reflection
		glPushMatrix();
		
		glTranslatef(400, shadowHeight - 700, playArea);
		glScalef(100, -100, 100);
		draw_nurb3();
		glPopMatrix();
	}
    
	if (areRecursiveFractalsOn) {
		// Material for lightning bolt
	

		// Determine where lightning bolt will be
		if (countDown % 200 == 0) {
			generateLightningPos();

			if (showLightning == true) {
				showLightning = false;
			}
			else {
				showLightning = true;
			}

		}


		// bolt ----------------------------------------
		if (showLightning == true) {
			glPushMatrix();
			glTranslatef(lightning_x, 500, lightning_z);
			glScalef(5000, 5000, 5000);
			int size = thunderbolt.size();
			int in_gray = (size / 2);
			int counter = 0;
			float width = .1;
			for (Line line : thunderbolt) {
				glBegin(GL_QUADS);
				glVertex3f(line.x1, line.y1, line.z1);
				glVertex3f(line.x1 + width, line.y1, line.z1 + width);
				glVertex3f(line.x2, line.y2, line.z2);
				glVertex3f(line.x2 + width, line.y2, line.z2 + width);
				glEnd();
				counter++;
				width -= 0.005;
			}
			glPopMatrix();
			// end
		}
	}
    
	//========================================
	//	Shadows
	//========================================
	glPushMatrix();
	// Tell GL new light source position
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	// Shadows
	if (areShadowsOn) {
		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_ALWAYS, 1, 0xFFFFFFFF);
		glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
	}
	// Draw floor using blending to blend in reflection
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1.0, 1.0, 1.0, 0.3);
	glPushMatrix();
	glDisable(GL_LIGHTING);
	glTranslatef(-900, shadowHeight, -900 + playArea);
	glCallList(displayBrick);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glDisable(GL_BLEND);
	// Shadows
	if (areShadowsOn) {
		glStencilFunc(GL_EQUAL, 1, 0xFFFFFFFF);
		glStencilOp(GL_ZERO, GL_ZERO, GL_ZERO);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_LIGHTING);  /* Force the 50% black. */
		glColor4f(0.0, 0.0, 0.0, 0.5);
		glPushMatrix();
		// Project the shadow
		glMultMatrixf((GLfloat *)shadow_matrix);
		// boxes
		glDisable(GL_DEPTH_TEST);
		glScalef(0, 10, 0);
		glCallList(display2);
		glTranslatef(0, shadowHeight, playArea);
		glCallList(display3);
		glTranslatef(200, shadowHeight + boxMovement, playArea);
		glCallList(display4);
		glTranslatef(-200, shadowHeight + boxMovement + 50, playArea);
		glScalef(100, 100, 100);
		draw_nurb3();
		//jet
		glPushMatrix();
		glTranslatef(pathLocation[0], pathLocation[1], pathLocation[2]);
		glScalef(100, 100, 100);
		glTranslatef(jet.position[0], jet.position[1], jet.position[2]);
		glRotatef(jet.rotation[0], 1.0, 0.0, 0.0);
		glRotatef(jet.rotation[1], 0.0, 1.0, 0.0);
		glRotatef(jet.rotation[2], 0.0, 0.0, 1.0);
		glCallList(jetMesh);
		glPopMatrix();

		glEnable(GL_DEPTH_TEST);
		glPopMatrix();

		glDisable(GL_BLEND);
		glEnable(GL_LIGHTING);
		// To eliminate depth buffer artifacts, use glDisable(GL_POLYGON_OFFSET_FILL);
		glDisable(GL_STENCIL_TEST);
	}

	// box 1
	glPushMatrix();
	glTranslatef(0, shadowHeight, playArea);
	glScalef(1.0, 10, 1.0);
	glCallList(display2);
	glPopMatrix();
	// box 2
	glPushMatrix();
	glTranslatef(200, shadowHeight + boxMovement, playArea);
	glCallList(display3);
	glPopMatrix();
	// box 3
	glPushMatrix();
	glTranslatef(-200, shadowHeight + boxMovement, playArea);
	glCallList(display4);
	glPopMatrix();

	//Boxes to be collected

	if (box1) {
		glPushMatrix();
		glTranslatef(box1x, shadowHeight, box1z);
		glCallList(display3);
		glPopMatrix();
	}
	
	if (box2) {
		glPushMatrix();
		glTranslatef(box2x, shadowHeight, box2z);
		glCallList(display4);
		glPopMatrix();
	}
	
	if (box3) {
		glPushMatrix();
		glTranslatef(box3x, shadowHeight, box3z);
		glCallList(display3);
		glPopMatrix();
	}
	
	if (box4) {
		glPushMatrix();
		glTranslatef(box4x, shadowHeight, box4z);
		glCallList(display4);
		glPopMatrix();
	}
	
	if (box5) {
		glPushMatrix();
		glTranslatef(box5x, shadowHeight, box5z);
		glCallList(display3);
		glPopMatrix();
	}
	
	if (box6) {
		glPushMatrix();
		glTranslatef(box6x, shadowHeight, box6z);
		glCallList(display4);
		glPopMatrix();
	}
	
	if (box7) {
		glPushMatrix();
		glTranslatef(box7x, shadowHeight, box7z);
		glCallList(display3);
		glPopMatrix();
	}
	
	if (box8) {
		glPushMatrix();
		glTranslatef(box8x, shadowHeight, box8z);
		glCallList(display4);
		glPopMatrix();
	}
	
	if (box9) {
		glPushMatrix();
		glTranslatef(box9x, shadowHeight, box9z);
		glCallList(display3);
		glPopMatrix();
	}
	
	if (box10) {
		glPushMatrix();
		glTranslatef(box10x, shadowHeight, box10z);
		glCallList(display4);
		glPopMatrix();
	}

	if (box1 == true && camera_x > box1x - 100 && camera_x < box1x + 100 && camera_z > box1z - 100 && camera_z < box1z + 100) {
		guesses++;
		box1 = false;
	}
	else if (box2 == true && camera_x > box2x - 100 && camera_x < box2x + 100 && camera_z > box2z - 100 && camera_z < box2z + 100) {
		guesses++;
		box2 = false;
	}
	else if (box10 == true && camera_x > box10x - 100 && camera_x < box10x + 100 && camera_z > box10z - 100 && camera_z < box10z + 100) {
		guesses++;
		box10 = false;
	}
	else if (box3 == true && camera_x > box3x - 100 && camera_x < box3x + 100 && camera_z > box3z - 100 && camera_z < box3z + 100) {
		guesses++;
		box3 = false;
	}
	else if (box4 == true && camera_x > box4x - 100 && camera_x < box4x + 100 && camera_z > box4z - 100 && camera_z < box4z + 100) {
		guesses++;
		box4 = false;
	}
	else if (box5 == true && camera_x > box5x - 100 && camera_x < box5x + 100 && camera_z > box5z - 100 && camera_z < box5z + 100) {
		guesses++;
		box5 = false;
	}
	else if (box6 == true && camera_x > box6x - 100 && camera_x < box6x + 100 && camera_z > box6z - 100 && camera_z < box6z + 100) {
		guesses++;
		box6 = false;
	}
	else if (box7 == true && camera_x > box7x - 100 && camera_x < box7x + 100 && camera_z > box7z - 100 && camera_z < box7z + 100) {
		guesses++;
		box7 = false;
	}
	else if (box8 == true && camera_x > box8x - 100 && camera_x < box8x + 100 && camera_z > box8z - 100 && camera_z < box8z + 100) {
		guesses++;
		box8 = false;
	}
	else if (box9 == true && camera_x > box9x - 100 && camera_x < box9x + 100 && camera_z > box9z - 100 && camera_z < box9z + 100) {
		guesses++;
		box9 = false;
	}
	


	//========Regular 3d environment=====================================
	

	glEnable(GL_BLEND);
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPushMatrix();
	glColor4f(0.7, 0.0, 0.0, 0.9);
	glColor4f(1.0, 1.0, 1.0, 0.9);

	// skybox
	glPushMatrix();
	glDisable(GL_LIGHTING);
	glTranslatef(-skyBoxMeshSize / 2, -skyBoxMeshSize / 2, -skyBoxMeshSize / 2);
	glCallList(display5);
	glEnable(GL_LIGHTING);
	glPopMatrix();

	//Water
	glPushMatrix();
	glTranslatef(-boundaryMeshSize / 2, -50, -boundaryMeshSize / 2);
	glCallList(display6);
	glPopMatrix();

	//runway
	for (int x = 0; x < 10; x++) {
		glPushMatrix();
		glTranslatef(runway_x, runway_y, -(1800 * x) + runway_z);
		glCallList(runway);
		glPopMatrix();
	}

	glEnable(GL_LIGHTING);
	glDisable(GL_BLEND);
	glEnable(GL_LIGHT0);

	//Particles
	if (leftBox) {
		glPushMatrix();
		glTranslatef(-150, shadowHeight + 5, playArea + 50);
		glScalef(.5, .5, .5);
		ps.drawDefaultParticles();
		glPopMatrix();
	}
	else {
		glPushMatrix();
		glTranslatef(250, shadowHeight + 5, playArea + 50);
		glScalef(.5, .5, .5);
		ps.drawDefaultParticles();
		glPopMatrix();
	}

	//Camera Location
	glPushMatrix();
	glTranslatef(camera_x, camera_y, camera_z);
	glRotatef(angle + 180, 0.0f, 1.0f, 0.0f);
	rain.draw();
	glScalef(0.5, 0.5, 0.5);
	leaves.draw();
	glPopMatrix();

	//bounding box
	if (areBoundingBoxesOn) {	
		glPushMatrix();
		glDisable(GL_LIGHTING);
		glColor3f(1.0, 1.0, 0.0);
		glTranslatef(pathLocation[0], pathLocation[1], pathLocation[2]);
		glScalef(100, 100, 100);
		glTranslatef(jet.position[0], jet.position[1], jet.position[2]);
		glRotatef(jet.rotation[0], 1.0, 0.0, 0.0);
		glRotatef(jet.rotation[1], 0.0, 1.0, 0.0);
		glRotatef(jet.rotation[2], 0.0, 0.0, 1.0);		
		glCallList(boundingBox);
		glEnable(GL_LIGHTING);
		glPopMatrix();
	}

	//jet
	glPushMatrix();
	jetPath.update();
	jetPath.updateRotation();
	glTranslatef(pathLocation[0], pathLocation[1], pathLocation[2]);
	glScalef(100, 100, 100);
	jetPath.drawPath();
	glTranslatef(jet.position[0], jet.position[1], jet.position[2]);
	glRotatef(jet.rotation[0], 1.0, 0.0, 0.0);
	glRotatef(jet.rotation[1], 0.0, 1.0, 0.0);
	glRotatef(jet.rotation[2], 0.0, 0.0, 1.0);	
	glCallList(jetMesh); 
	glPopMatrix();

	//plane
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glTranslatef(-perlinMeshSize / 2, 0, -perlinMeshSize / 2);
	if (isMultiscaleTerrainOn) {
		glCallList(display1);
	}
	else {
		glCallList(displayLists[2]);
	}
	
	glPopMatrix();

	//runway
	for (int x = 0; x < 10; x++) {
		glPushMatrix();
		glTranslatef(runway_x, runway_y, -(1800 * x) + runway_z);
		glCallList(runway);
		glPopMatrix();
	}

	glPushMatrix();
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	if (isLightArrowOn) {
		drawLightArrow();
	}
	glPopMatrix();


	//end
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glDisable(GL_CULL_FACE);



	//=======================================================================
	// Lighting disabled from here on to allow other colors on the screen
	//=======================================================================
	countDown = countDown - 1;
	string countDownString = to_string(countDown);
	glDisable(GL_LIGHTING);

	// texto
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, window_width, 0, window_height);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glColor3f(0.0, 1.0, 0.0);
	renderBitmapString(0.0, window_height - 13.0f, 0.0f, "Use [Arrows] to move in plain");
	renderBitmapString(0.0, window_height - 26.0f, 0.0f, "Use [W and S] to look up and down");
	renderBitmapString(0.0, window_height - 39.0f, 0.0f, "Use [L and R] to select the left or right boxes respectively.");
	renderBitmapString((window_width / 2) - 50, (window_height / 10) * 9, 0.0f, "Time Left: ");
	renderBitmapString((window_width / 2) + 40, (window_height / 10) * 9, 0.0f, countDownString.c_str());
	if (getInBox) {
		renderBitmapString((window_width / 2) - 150, ((window_height / 10) * 9) + 20, 0.0f, "Go to the area with the Flag to make a choice!");
	}
	else if (correctChoice) {
		glColor3f(0.0, 0.0, 1.0);
		renderBitmapString((window_width / 2) - 50, ((window_height / 10) * 9) + 20, 0.0f, "Correct!!!!");
	}
	else {
		glColor3f(1.0, 0.0, 0.0);
		renderBitmapString((window_width / 2) - 50, ((window_height / 10) * 9) + 20, 0.0f, "Wrong :(");
	}

	string scoreString = to_string(totalScore);
	glColor3f(1.0, 1.0, 1.0);
	renderBitmapString(10.0, window_height / 8, 0.0f, "Score: ");
	renderBitmapString(70, window_height / 8, 0.0f, scoreString.c_str());

	if (isGameModeOn) {
		renderBitmapString(10.0, window_height / 9, 0.0f, "Game Mode On");
	} 
	else {
		renderBitmapString(10.0, window_height / 9, 0.0f, "View Mode On");
		renderBitmapString(0.0, window_height - 52.0f, 0.0f, "Use [A and D] to move up and down");
	}
	string guessCount = to_string(guesses);
	renderBitmapString(10.0, (window_height / 9) + 40, 0.0f, "Guesses: ");
	renderBitmapString(80.0, (window_height / 9) + 40, 0.0f, guessCount.c_str());

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();


	//=====================================
	// Second Viewport
	//=====================================
	glViewport(0, 0, window_width / 4, window_height / 4);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Draw box for hud
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.0, 1200.0, 0.0, 250.0);

	glColor3f(.200, .200, .200);
	//Bottom tunnel
	glShadeModel(GL_FLAT);
	glBegin(GL_QUADS);
	glVertex2f(0, 0);//bottom left
	glVertex2f(window_width, 0);//bottom right
	glVertex2f(window_width, window_height); //top right
	glVertex2f(0, window_height);//top left
	glEnd();

	
	glPopMatrix();
	glEnable(GL_LIGHTING);
	glutSwapBuffers();
}

// main
int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_STENCIL);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(window_width, window_height);
	glutCreateWindow("The Game");
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutIdleFunc(display);
	
	glutSpecialFunc(specialkeys);
	glutKeyboardFunc(callbackKeyboard);
	init();
	glutMainLoop();
	return 0;
}
