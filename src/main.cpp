/*
Base code
Currently will make 2 FBOs and textures (only uses one in base code)
and writes out frame as a .png (Texture_output.png)

Winter 2017 - ZJW (Piddington texture write)
2017 integration with pitch and yaw camera lab (set up for texture mapping lab)
*/

#include <iostream>
#include <glad/glad.h>

#include "GLSL.h"
#include "Program.h"
#include "MatrixStack.h"
#include "Shape.h"
#include "WindowManager.h"
#include "GLTextureWriter.h"
#include "Player.h"
#include "Projectile.h"
#include "Enemy.h"
#include "Game.h"
#include "PowerUp.h"

// value_ptr for glm
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;

class Application : public EventCallbacks
{

public:

	WindowManager * windowManager = nullptr;

	// Our shader program
	std::shared_ptr<Program> prog;
	std::shared_ptr<Program> texProg;
	std::shared_ptr<Program> phong_prog;

	// Shape to be used (from obj file)
	shared_ptr<Shape> shape;
	shared_ptr<Shape> cylinder;
	shared_ptr<Shape> sphere;
	shared_ptr<Shape> plane;

	vector<GLfloat> lightPos;

	// Contains vertex information for OpenGL
	GLuint VertexArrayID;

	// Data necessary to give our triangle to OpenGL
	GLuint VertexBufferID;

	//geometry for texture render
	GLuint quad_VertexArrayID;
	GLuint quad_vertexbuffer;

	GLuint groundTextureID;
	shared_ptr<Texture> texture;
	//ground plane info
	GLuint GrndBuffObj, GrndNorBuffObj, GrndTexBuffObj, GIndxBuffObj;
	int gGiboLen;

	//reference to texture FBO
	GLuint frameBuf[2];
	GLuint texBuf[2];
	GLuint depthBuf;

	bool FirstTime = true;
	bool Moving = false;
	int gMat = 0;

	float Theta = 0;
	float Phi = 0;

	double curPosX, curPosY;

	bool mouseDown = false;
	bool wDown = false;
	bool aDown = false;
	bool sDown = false;
	bool dDown = false;
	bool spaceDown = false;
	bool shiftDown = false;

	float xCamPos = 0.0;
	float yCamPos = -20.0;
	float zCamPos = 0.0;

	vector<float> head_x;
	vector<float> head_z;

	vec3 lookAtCenter = vec3(0.0, 0.0, 0.0);

	float timeNow;
	float timePrev;
	float timeInterval;

	Game game;

	Player player;
	Projectile arrow;
	Enemy enemy;

	vector<Enemy> enemies;
	vector<Projectile> projectiles;
	vector<PowerUp> powerUps;

	bool stillPlaying = true;

	float dimX = 20.0f;
	float dimZ = 20.0f;

	void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
	{
		if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		else if (key == GLFW_KEY_M && action == GLFW_PRESS)
		{
			gMat = (gMat + 1) % 4;
		}
		
		// moves camera to the left
		else if (key == GLFW_KEY_A)
		{
			if (action == GLFW_PRESS)
			{
				aDown = true;
			}
			else if (action == GLFW_RELEASE)
			{
				aDown = false;
			}
		}

		// moves camera right
		else if (key == GLFW_KEY_D)
		{
			if (action == GLFW_PRESS)
			{
				dDown = true;
			}
			else if (action == GLFW_RELEASE)
			{
				dDown = false;
			}
		}

		// moves camera forwards
		else if (key == GLFW_KEY_W)
		{
			if (action == GLFW_PRESS)
			{
				wDown = true;
			}
			else if (action == GLFW_RELEASE)
			{
				wDown = false;
			}
		}

		// moves camera backwards
		else if (key == GLFW_KEY_S)
		{
			if (action == GLFW_PRESS)
			{
				sDown = true;
			}
			else if (action == GLFW_RELEASE)
			{
				sDown = false;
			}
		}

		// raises camera vertically
		else if (key == GLFW_KEY_SPACE)
		{
			if (action == GLFW_PRESS)
			{
				spaceDown = true;
			}
			else if (action == GLFW_RELEASE)
			{
				spaceDown = false;
			}
		}

		// prevents camera from moving vertically
		else if (key == GLFW_KEY_LEFT_SHIFT)
		{
			if (action == GLFW_PRESS)
			{
				shiftDown = true;
			}
			else if (action == GLFW_RELEASE)
			{
				shiftDown = false;
			}
		}
	}

	void scrollCallback(GLFWwindow* window, double deltaX, double deltaY)
	{
		Theta += (float) deltaX;
	}

	void mouseCallback(GLFWwindow *window, int button, int action, int mods)
	{
		double curPosX, curPosY;

		if (action == GLFW_PRESS)
		{
			mouseDown = true;
			glfwGetCursorPos(window, &curPosX, &curPosY);
			cout << "Pos X " << curPosX << " Pos Y " << curPosY << endl;
			Moving = true;
		}

		if (action == GLFW_RELEASE)
		{
			Moving = false;
			mouseDown = false;
		}
	}

	void resizeCallback(GLFWwindow *window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	void initPlane()
	{
		// create a quad from scratch
		plane = make_shared<Shape>();
		
		// generate positions
		plane->posBuf.push_back(1.0f);
		plane->posBuf.push_back(0.0f);
		plane->posBuf.push_back(1.0f);

		plane->posBuf.push_back(1.0f);
		plane->posBuf.push_back(0.0f);
		plane->posBuf.push_back(-1.0f);

		plane->posBuf.push_back(-1.0f);
		plane->posBuf.push_back(0.0f);
		plane->posBuf.push_back(-1.0f);

		plane->posBuf.push_back(-1.0f);
		plane->posBuf.push_back(0.0f);
		plane->posBuf.push_back(1.0f);

		// generate position indexes
		plane->eleBuf.push_back(0);
		plane->eleBuf.push_back(1);
		plane->eleBuf.push_back(2);

		plane->eleBuf.push_back(0);
		plane->eleBuf.push_back(3);
		plane->eleBuf.push_back(2);

		// generate normals
		for (int i = 0; i < 4; i++)
		{
			plane->norBuf.push_back(0.0);
			plane->norBuf.push_back(1.0);
			plane->norBuf.push_back(0.0);
		}

		plane->resize();
		plane->init();
	}

	void init(const std::string& resourceDirectory)
	{
		int width, height;
		glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);
		GLSL::checkVersion();

		Theta = -3.14159;
		Phi = 0;

		// Set background color.
		glClearColor(.12f, .34f, .56f, 1.0f);
		// Enable z-buffer test.
		glEnable(GL_DEPTH_TEST);

		// Initialize the GLSL program.
		prog = make_shared<Program>();
		prog->setVerbose(true);
		prog->setShaderNames(
			resourceDirectory + "/simple_vert.glsl",
			resourceDirectory + "/simple_frag.glsl");
		if (! prog->init())
		{
			std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
			exit(1);
		}
		prog->addUniform("P");
		prog->addUniform("MV");
		prog->addUniform("View");
		prog->addUniform("MatAmb");
		prog->addUniform("MatDif");
		prog->addUniform("MatSpec");
		prog->addUniform("shine");
		prog->addAttribute("vertPos");
		prog->addAttribute("vertNor");

		// Initialize the Phong program.
		phong_prog = make_shared<Program>();
		phong_prog->setVerbose(true);
		phong_prog->setShaderNames(
			resourceDirectory + "/phong_vert.glsl", 
			resourceDirectory + "/phong_frag.glsl");
		if (!phong_prog->init())
		{
			std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
			exit(1);
		}		
		phong_prog->addUniform("P");
		phong_prog->addUniform("MV");
		phong_prog->addUniform("View");
		phong_prog->addAttribute("vertPos");
		phong_prog->addAttribute("vertNor");
		phong_prog->addUniform("MatAmb");
		phong_prog->addUniform("MatDif");
		phong_prog->addUniform("MatSpec");
		phong_prog->addUniform("shine");
		phong_prog->addUniform("lightPos");
		phong_prog->addUniform("viewPos");

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		// generate the original previous time
		timePrev = glfwGetTime();
		timeNow = timePrev;

		game = Game(sphere, timeNow);

		player = Player(sphere, timeNow);

		enemies.push_back(Enemy(sphere, glfwGetTime(), player, 2.5));

		// the light will be at 0, 0, 0
		lightPos = { 0.0, 0.0, 0.0 };

		head_x = vector<float>();
		head_z = vector<float>();

		// the objects will be randomly placed
		for (int i = 0; i < 28; i++)
		{
			head_x.push_back(-18.0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (36.0))));
			head_z.push_back(-18.0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (36.0))));
		}

		//create two frame buffer objects to toggle between
		glGenFramebuffers(2, frameBuf);
		glGenTextures(2, texBuf);
		glGenRenderbuffers(1, &depthBuf);
		createFBO(frameBuf[0], texBuf[0]);

		//set up depth necessary as rendering a mesh that needs depth test
		glBindRenderbuffer(GL_RENDERBUFFER, depthBuf);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuf);

		//more FBO set up
		GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
		glDrawBuffers(1, DrawBuffers);

		//create another FBO so we can swap back and forth
		createFBO(frameBuf[1], texBuf[1]);
		//this one doesn't need depth

		initTex(resourceDirectory);

		//set up the shaders to blur the FBO just a placeholder pass thru now
		//next lab modify and possibly add other shaders to complete blur
		texProg = make_shared<Program>();
		texProg->setVerbose(true);
		texProg->setShaderNames(
			resourceDirectory + "/tex_vert.glsl",
			resourceDirectory + "/tex_frag0.glsl");
		if (! texProg->init())
		{
			std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
			exit(1);
		}
		texProg->addUniform("P");
		texProg->addUniform("MV");
		texProg->addAttribute("vertPos");
		texProg->addAttribute("vertNor");
		texProg->addAttribute("vertTex");
		texProg->addUniform("Texture0");
	 }

	 // Code to load in the three textures
	void initTex(const std::string& resourceDirectory)
	{
		texture = make_shared<Texture>();
		texture->setFilename(resourceDirectory + "/terrain2.jpg");
		texture->init();
		texture->setUnit(0);
		texture->setWrapModes(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
	}

	void initGeom(const std::string& resourceDirectory)
	{
		// Initialize the obj mesh VBOs etc
		shape = make_shared<Shape>();
		shape->loadMesh(resourceDirectory + "/Nefertiti-100K.obj");
		shape->resize();
		shape->init();

		initQuad();
	}

	void initSphere(const std::string& resourceDirectory)
	{
		// initialize the obj mesh: sphere
		sphere = make_shared<Shape>();
		sphere->loadMesh(resourceDirectory + "/sphere.obj");
		sphere->resize();
		sphere->init();
	}

	/* Helper function to create the framebuffer object and
		associated texture to write to */
	void createFBO(GLuint& fb, GLuint& tex)
	{
		//initialize FBO
		int width, height;
		glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);

		//set up framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, fb);
		//set up texture
		glBindTexture(GL_TEXTURE_2D, tex);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			cout << "Error setting up frame buffer - exiting" << endl;
			exit(0);
		}
	}

	// To complete image processing on the specificed texture
	// Right now just draws large quad to the screen that is texture mapped
	// with the prior scene image - next lab we will process
	
	void ProcessImage(GLuint inTex)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, inTex);

		// example applying of 'drawing' the FBO texture - change shaders
		texProg->bind();
		glUniform1i(texProg->getUniform("texBuf"), 0);
		glUniform2f(texProg->getUniform("dir"), -1, 0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisableVertexAttribArray(0);
		texProg->unbind();
	}

	/**** geometry set up for ground plane *****/
	void initQuad()
	{
		float g_groundSize = 20;
		float g_groundY = -1.5;

		// A x-z plane at y = g_groundY of dim[-g_groundSize, g_groundSize]^2
		//float GrndPos[] = {
		//	-g_groundSize, g_groundY, -g_groundSize,
		//	-g_groundSize, g_groundY,  g_groundSize,
		//	g_groundSize, g_groundY,  g_groundSize,
		//	g_groundSize, g_groundY, -g_groundSize
		//};

		float GrndPos[] = {
			-20.0, -20.0, -20.0,
			-20.0,  20.0, -20.0,
			20.0,   20.0, -20.0,
			20.0,  -20.0, -20.0
		};

		float GrndNorm[] = {
			0, 1, 0,
			0, 1, 0,
			0, 1, 0,
			0, 1, 0,
			0, 1, 0,
			0, 1, 0
		};

		float GrndTex[] = {
			0, 0, // back
			0, 1,
			1, 1,
			1, 0
		};

		unsigned short idx[] = { 0, 1, 2, 0, 2, 3 };

		GLuint VertexArrayID;
		//generate the VAO
		glGenVertexArrays(1, &VertexArrayID);
		glBindVertexArray(VertexArrayID);

		gGiboLen = 6;
		glGenBuffers(1, &GrndBuffObj);
		glBindBuffer(GL_ARRAY_BUFFER, GrndBuffObj);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GrndPos), GrndPos, GL_STATIC_DRAW);

		glGenBuffers(1, &GrndNorBuffObj);
		glBindBuffer(GL_ARRAY_BUFFER, GrndNorBuffObj);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GrndNorm), GrndNorm, GL_STATIC_DRAW);

		glGenBuffers(1, &GrndTexBuffObj);
		glBindBuffer(GL_ARRAY_BUFFER, GrndTexBuffObj);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GrndTex), GrndTex, GL_STATIC_DRAW);

		glGenBuffers(1, &GIndxBuffObj);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GIndxBuffObj);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idx), idx, GL_STATIC_DRAW);
	}

	void renderGround()
	{
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, GrndBuffObj);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, GrndNorBuffObj);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, GrndTexBuffObj);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

		// draw!
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GIndxBuffObj);
		glDrawElements(GL_TRIANGLES, gGiboLen, GL_UNSIGNED_SHORT, 0);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
	}

	void update(float currentTime, float curX, float curY, float posX, float posZ, int width, int height)
	{
		if (spaceDown)
		{
			player.dash();
		}

		player.update(curPosX, curPosY, xCamPos, zCamPos, width, height, timeNow, dimX, dimZ);

		if (player.autoFire && player.autoFireCoolDown <= 0)
		{
			float x = -1.0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2.0)));
			float z = -1.0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2.0)));
			projectiles.push_back(Projectile(sphere, player.pos, normalize(vec3(x, 0.0, z)), 20.0, 1.0, currentTime));
			player.autoFireCoolDown = player.autoFireInterval;
		}

		for (int i = 0; i < enemies.size(); i++)
		{
			Enemy *e = &enemies[i];
			e->update(player, timeNow);
			if (e->collide(player))
			{
				// GAME OVER
				stillPlaying = false;
			}
		}

		for (int i = 0; i < powerUps.size(); i++)
		{
			PowerUp *pu = &powerUps[i];
			pu->update(player, timeNow);
			if (pu->collide(player))
			{
				pu->usePowerUp(&player);

				powerUps.erase(powerUps.begin() + i);
				break;
			}
		}

		updateProjectiles();

		game.generateEnemies(&enemies, player, timeNow);
		game.generatePowerUps(&powerUps, player, timeNow);

		return;
	}
	
	void updateProjectiles()
	{
		for (int i = 0; i < projectiles.size(); i++)
		{
			Projectile *p = &projectiles[i];
			p->update(timeNow);
			if (!p->exists)
			{
				projectiles.erase(projectiles.begin() + i);
				continue;
			}
			for (int k = 0; k < enemies.size(); k++)
			{
				Enemy *e = &enemies[k];
				if (e->collide(*p))
				{
					if (e->canDelete())
					{
						enemies.erase(enemies.begin() + k);
					}
					if (p->canDelete())
					{
						projectiles.erase(projectiles.begin() + i);
					}

					updateProjectiles();
					break;
				}
			}
		}
	}

	void render(GLFWwindow *window)
	{
		// Get current frame buffer size.
		int width, height;
		glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);
		glViewport(0, 0, width, height);

		// get time difference so that lag does not affect camera movement
		timeNow = glfwGetTime();
		timeInterval = (timeNow - timePrev) * 6;
		timePrev = timeNow;

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// find where the user is pointing the camera
		glfwGetCursorPos(window, &curPosX, &curPosY);
		if (curPosY < 0)
		{
			curPosY = 0;
		}
		else if (curPosY > height)
		{
			curPosY = height;
		}
		//Phi = ((curPosY / height) * 3.14159 - (3.14159 / 2)) * (80.0 / 90.0);
		Phi = -3.14159 / 2;
		//Theta = (curPosX / width) * 2 * 3.14159 + 3.14159;
		Theta = 3.14159 / 2;

		lookAtCenter = vec3(cos(Phi) * cos(Theta), sin(Phi), cos(Phi) * sin(Theta));

		// move the position of the camera is WASD is being pressed
		if (wDown)
		{
			if (aDown || dDown)
			{
				zCamPos -= timeInterval * player.speed / sqrt(2);
			}
			else
			{
				zCamPos -= timeInterval * player.speed;
			}
		}
		if (sDown)
		{
			if (aDown || dDown)
			{
				zCamPos += timeInterval * player.speed / sqrt(2);
			}
			else
			{
				zCamPos += timeInterval * player.speed;
			}
		}
		if (aDown)
		{
			if (wDown || sDown)
			{
				xCamPos -= timeInterval * player.speed / sqrt(2);
			}
			else
			{
				xCamPos -= timeInterval * player.speed;
			}
		}
		if (dDown)
		{
			if (wDown || sDown)
			{
				xCamPos += timeInterval * player.speed / sqrt(2);
			}
			else
			{
				xCamPos += timeInterval * player.speed;
			}
		}

		if (xCamPos + player.radius > dimX)
		{
			xCamPos = dimX - player.radius;
		}
		else if (xCamPos - player.radius < -dimX)
		{
			xCamPos = -dimX + player.radius;
		}

		if (zCamPos + player.radius > dimZ)
		{
			zCamPos = dimZ - player.radius;
		}
		else if (zCamPos - player.radius < -dimZ)
		{
			zCamPos = -dimZ + player.radius;
		}

		if (mouseDown)
		{
			player.shoot(&projectiles);
		}

		update(timeNow, curPosX, curPosY, xCamPos, zCamPos, width, height);

		// Clear framebuffer.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/* Leave this code to just draw the meshes alone */
		float aspect = width/(float)height;

		// Create the matrix stacks
		auto P = make_shared<MatrixStack>();
		auto MV = make_shared<MatrixStack>();
		auto View = make_shared<MatrixStack>();

		texProg->bind();

		// Apply perspective projection.
		P->pushMatrix();
			P->perspective(45.0f, aspect, 0.01f, 100.0f);
			glUniformMatrix4fv(texProg->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));
		P->popMatrix();

		// a plane will be generated for the floor.
		MV->pushMatrix();
			texture->bind(texProg->getUniform("Texture0"));

			MV->translate(vec3(-xCamPos, zCamPos, 0.0));
			//MV->scale(vec3(dimX / 5, dimZ /5, 1.0));
			glUniformMatrix4fv(texProg->getUniform("MV"), 1, GL_FALSE, value_ptr(MV->topMatrix()));
			//SetMaterial(0, texProg);
			renderGround();
		MV->popMatrix();

		texProg->unbind();


		// Draw our scene
		prog->bind();

		// Apply perspective projection.
		P->pushMatrix();
			P->perspective(45.0f, aspect, 0.01f, 100.0f);
			glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));
		P->popMatrix();

		View->pushMatrix();
			View->lookAt(vec3(0.0, 0.0, 0.0), lookAtCenter, vec3(0.0, 1.0, 0.0));
			View->translate(vec3(xCamPos, yCamPos, zCamPos));
			glUniformMatrix4fv(prog->getUniform("View"), 1, GL_FALSE, value_ptr(View->topMatrix()));
		View->popMatrix();

		//// a plane will be generated for the floor.
		//MV->pushMatrix();
		//	MV->translate(vec3(0.0, 0.0, 0.0));
		//	MV->scale(vec3(dimX, 1.0, dimZ));
		//	glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, value_ptr(MV->topMatrix()));
		//	SetMaterial(0, prog);
		//	plane->draw(prog);
		//MV->popMatrix();

		MV->pushMatrix();
			MV->loadIdentity();
			MV->translate(vec3(-player.pos.x, 0.0f, -player.pos.z));
			MV->rotate(player.angle, vec3(0, 1, 0));
			SetMaterial(2, prog);
			glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, value_ptr(MV->topMatrix()));
			player.draw(prog);
		MV->popMatrix();

		for (int i = 0; i < projectiles.size(); i++)
		{
			Projectile *p = &projectiles[i];
			MV->pushMatrix();
				MV->loadIdentity();
				MV->translate(vec3(-p->pos.x, 0.0f, -p->pos.z));
				MV->scale(0.25);
				SetMaterial(5, prog);
				glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, value_ptr(MV->topMatrix()));
				p->draw(prog);
			MV->popMatrix();
		}

		for (int i = 0; i < powerUps.size(); i++)
		{
			PowerUp *pu = &powerUps[i];
			MV->pushMatrix();
			MV->loadIdentity();
			MV->translate(vec3(-pu->pos.x, 0.0f, -pu->pos.z));
			SetMaterial(6, prog);
			glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, value_ptr(MV->topMatrix()));
			pu->draw(prog);
			MV->popMatrix();
		}

		for (int i = 0; i < enemies.size(); i++)
		{
			Enemy *e = &enemies[i];
			MV->pushMatrix();
				MV->loadIdentity();
				MV->translate(vec3(-e->pos.x, 0.0f, -e->pos.z));
				MV->rotate(e->angle, vec3(0, 1, 0));
				SetMaterial(4, prog);
				glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, value_ptr(MV->topMatrix()));
				e->draw(prog);
			MV->popMatrix();
		}

		prog->unbind();
	}

	// helper function to set materials for shading
	void SetMaterial(int i, std::shared_ptr<Program> p)
	{
		switch (i)
		{
		case 0: //shiny blue plastic
			glUniform3f(p->getUniform("MatAmb"), 0.02f, 0.04f, 0.2f);
			glUniform3f(p->getUniform("MatDif"), 0.0f, 0.16f, 0.9f);
			glUniform3f(p->getUniform("MatSpec"), 0.14, 0.2, 0.8);
			glUniform1f(p->getUniform("shine"), 120.0);
			break;
		case 1: // flat grey
			glUniform3f(p->getUniform("MatAmb"), 0.13f, 0.13f, 0.14f);
			glUniform3f(p->getUniform("MatDif"), 0.3f, 0.3f, 0.4f);
			glUniform3f(p->getUniform("MatSpec"), 0.3, 0.3, 0.4);
			glUniform1f(p->getUniform("shine"), 4.0);
			break;
		case 2: //brass
			glUniform3f(p->getUniform("MatAmb"), 0.3294f, 0.2235f, 0.02745f);
			glUniform3f(p->getUniform("MatDif"), 0.7804f, 0.5686f, 0.11373f);
			glUniform3f(p->getUniform("MatSpec"), 0.9922, 0.941176, 0.80784);
			glUniform1f(p->getUniform("shine"), 27.9);
			break;
		 case 3: //copper
			glUniform3f(p->getUniform("MatAmb"), 0.1913f, 0.0735f, 0.0225f);
			glUniform3f(p->getUniform("MatDif"), 0.7038f, 0.27048f, 0.0828f);
			glUniform3f(p->getUniform("MatSpec"), 0.9922, 0.941176, 0.80784);
			glUniform1f(p->getUniform("shine"), 27.9);
			break;
		 case 4: // red velvet
			 glUniform3f(p->getUniform("MatAmb"), 0.510, 0.090, 0.169);
			 glUniform3f(p->getUniform("MatDif"), 0.2, 0.03, 0.04);
			 glUniform3f(p->getUniform("MatSpec"), 0.3, 0.3, 0.3);
			 glUniform1f(p->getUniform("shine"), 1.5);
			 break;
		 case 5: // charcoal
			 glUniform3f(p->getUniform("MatAmb"), 0.1, 0.1, 0.1);
			 glUniform3f(p->getUniform("MatDif"), 0.05, 0.05, 0.05);
			 glUniform3f(p->getUniform("MatSpec"), 0.03, 0.03, 0.03);
			 glUniform1f(p->getUniform("shine"), 0.5);
			 break;
		 case 6: // photchromic-ish material: purple -> green -> yellow
			 glUniform3f(p->getUniform("MatAmb"), 0.294, 0.0, 0.510);
			 glUniform3f(p->getUniform("MatDif"), -0.1, 0.4, -0.2);
			 glUniform3f(p->getUniform("MatSpec"), 0.3, 0.3, 0.0);
			 glUniform1f(p->getUniform("shine"), 12);
			 break;
		 default:
			 break;
		}
	}
};

int main(int argc, char **argv)
{
	// Where the resources are loaded from
	std::string resourceDir = "../resources";

	if (argc >= 2)
	{
		resourceDir = argv[1];
	}

	Application *application = new Application();

	// Your main will always include a similar set up to establish your window
	// and GL context, etc.

	WindowManager *windowManager = new WindowManager();
	windowManager->init(868, 868);
	windowManager->setEventCallbacks(application);
	application->windowManager = windowManager;

	// This is the code that will likely change program to program as you
	// may need to initialize or set up different data and state
	application->initSphere(resourceDir);
	application->initGeom(resourceDir);
	application->init(resourceDir);
	application->initPlane();

	// Loop until the user closes the window.
	while ((!glfwWindowShouldClose(windowManager->getHandle())) && application->stillPlaying)
	{
			// Render scene.
			application->render(windowManager->getHandle());

			// Swap front and back buffers.
			glfwSwapBuffers(windowManager->getHandle());
			// Poll for and process events.
			glfwPollEvents();
	}

	// Quit program.
	windowManager->shutdown();
	return 0;
}
