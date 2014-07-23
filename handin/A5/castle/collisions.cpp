/* Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above notice and this permission notice shall be included in all copies
 * or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
/* File for "Collision Detection" lesson of the OpenGL tutorial on
 * www.videotutorialsrock.com
 */
 //http://www.videotutorialsrock.com/opengl_tutorial/collision_detection/text.php
#include "collisions.hpp"

//Returns a random float from 0 to < 1
float randomFloat() {
	return (float)rand() / ((float)RAND_MAX + 1);
}

const float GRAVITY = 8.0f;
const float BOX_SIZE = 12.0f; //The length of one side of the box
//The amount of time between each time that we handle collisions and apply the
//effects of gravity
const float TIME_BETWEEN_UPDATES = 0.01f;
const int TIMER_MS = 500; //The number of milliseconds to which the timer is set

const int MAX_OCTREE_DEPTH = 6;
const int MIN_BALLS_PER_OCTREE = 3;
const int MAX_BALLS_PER_OCTREE = 6;

int NUM_SOUNDS = 2, ms_b1, ms_b2, ms_b3, ms_b4, ms_b5, ms_b6, ms_b7, ms_b8;
bool PLAY_SOUND = false;

//Puts potential ball-ball collisions in potentialCollisions.  It must return
//all actual collisions, but it need not return only actual collisions.
void potentialBallBallCollisions(vector<BallPair> &potentialCollisions, vector<Ball*> &balls, Octree* octree) {
	//Fast method
	octree->potentialBallBallCollisions(potentialCollisions);
	/*
	//Slow method
	for(unsigned int i = 0; i < balls.size(); i++) {
		for(unsigned int j = i + 1; j < balls.size(); j++) {
			BallPair bp;
			bp.ball1 = balls[i];
			bp.ball2 = balls[j];
			potentialCollisions.push_back(bp);
		}
	}
	*/
}

//Puts potential ball-wall collisions in potentialCollisions.  It must return
//all actual collisions, but it need not return only actual collisions.
void potentialBallWallCollisions(vector<BallWallPair> &potentialCollisions, vector<Ball*> &balls, Octree* octree) {
	//Fast method
	octree->potentialBallWallCollisions(potentialCollisions);

	/*
	//Slow method
	Wall walls[] =
		{WALL_LEFT, WALL_RIGHT, WALL_FAR, WALL_NEAR, WALL_TOP, WALL_BOTTOM};
	for(unsigned int i = 0; i < balls.size(); i++) {
		for(int j = 0; j < 6; j++) {
			BallWallPair bwp;
			bwp.ball = balls[i];
			bwp.wall = walls[j];
			potentialCollisions.push_back(bwp);
		}
	}
	*/
}

//Moves all of the balls by their velocity times dt
void moveBalls(vector<Ball*> &balls, Octree* octree, float dt) {
	for(unsigned int i = 0; i < balls.size(); i++) {
		Ball* ball = balls[i];
		Vec3f oldPos = ball->pos;
		ball->pos += ball->v * dt;
		octree->ballMoved(ball, oldPos);
	}
}

//Decreases the y coordinate of the velocity of each ball by GRAVITY *
//TIME_BETWEEN_UPDATES
void applyGravity(vector<Ball*> &balls) {
	for(unsigned int i = 0; i < balls.size(); i++) {
		Ball* ball = balls[i];
		ball->v -= Vec3f(0, GRAVITY * TIME_BETWEEN_UPDATES, 0);
	}
}

//Returns whether two balls are colliding
bool testBallBallCollision(Ball* b1, Ball* b2) {
	//Check whether the balls are close enough
	float r = b1->r + b2->r;
	if ((b1->pos - b2->pos).magnitudeSquared() < r * r) {
		//Check whether the balls are moving toward each other
		Vec3f netVelocity = b1->v - b2->v;
		Vec3f displacement = b1->pos - b2->pos;
		return netVelocity.dot(displacement) < 0;
	}
	else
		return false;
}

//Handles all ball-ball collisions
void handleBallBallCollisions(vector<Ball*> &balls, Octree* octree) {
	vector<BallPair> bps;
	potentialBallBallCollisions(bps, balls, octree);
	for(unsigned int i = 0; i < bps.size(); i++) {
		BallPair bp = bps[i];

		Ball* b1 = bp.ball1;
		Ball* b2 = bp.ball2;
		if (testBallBallCollision(b1, b2)) {
			//Make the balls reflect off of each other
			Vec3f displacement = (b1->pos - b2->pos).normalize();
			b1->v -= 2 * displacement * b1->v.dot(displacement);
			b2->v -= 2 * displacement * b2->v.dot(displacement);
			if (PLAY_SOUND) {
				SM.PlaySound(b1->sound);
			}
		}
	}
}

//Returns the direction from the origin to the wall
Vec3f wallDirection(Wall wall) {
	switch (wall) {
		case WALL_LEFT:
			return Vec3f(-1, 0, 0);
		case WALL_RIGHT:
			return Vec3f(1, 0, 0);
		case WALL_FAR:
			return Vec3f(0, 0, -1);
		case WALL_NEAR:
			return Vec3f(0, 0, 1);
		case WALL_TOP:
			return Vec3f(0, 1, 0);
		case WALL_BOTTOM:
			return Vec3f(0, -1, 0);
		default:
			return Vec3f(0, 0, 0);
	}
}

//Returns whether a ball and a wall are colliding
bool testBallWallCollision(Ball* ball, Wall wall) {
	Vec3f dir = wallDirection(wall);
	//Check whether the ball is far enough in the "dir" direction, and whether
	//it is moving toward the wall
	return ball->pos.dot(dir) + ball->r > BOX_SIZE / 2 &&
			ball->v.dot(dir) > 0;
}

//Handles all ball-wall collisions
void handleBallWallCollisions(vector<Ball*> &balls, Octree* octree) {
	vector<BallWallPair> bwps;
	potentialBallWallCollisions(bwps, balls, octree);
	for(unsigned int i = 0; i < bwps.size(); i++) {
		BallWallPair bwp = bwps[i];

		Ball* b = bwp.ball;
		Wall w = bwp.wall;
		if (testBallWallCollision(b, w)) {
			//Make the ball reflect off of the wall
			Vec3f dir = (wallDirection(w)).normalize();
			b->v -= 2 * dir * b->v.dot(dir);
			if (PLAY_SOUND) {
				SM.PlaySound(b->sound);
			}
		}
	}
}

//Applies gravity and handles all collisions.  Should be called every
//TIME_BETWEEN_UPDATES seconds.
void performUpdate(vector<Ball*> &balls, Octree* octree) {
	applyGravity(balls);
	handleBallBallCollisions(balls, octree);
	handleBallWallCollisions(balls, octree);
}

//Advances the state of the balls by t.  timeUntilUpdate is the amount of time
//until the next call to performUpdate.
void advance(vector<Ball*> &balls, Octree* octree, float t, float &timeUntilUpdate) {
	while (t > 0) {
		if (timeUntilUpdate <= t) {
			moveBalls(balls, octree, timeUntilUpdate);
			performUpdate(balls, octree);
			t -= timeUntilUpdate;
			timeUntilUpdate = TIME_BETWEEN_UPDATES;
		}
		else {
			moveBalls(balls, octree, t);
			timeUntilUpdate -= t;
			t = 0;
		}
	}
}

vector<Ball*> _balls; //All of the balls in play
float _angle = 0.0f; //The camera angle
Octree* _octree; //An octree with all af the balls
//The amount of time until performUpdate should be called
float _timeUntilUpdate = 0;
GLuint _textureId;

//Deletes everything.  This should be called when exiting the program.
void cleanup() {
	for(unsigned int i = 0; i < _balls.size(); i++) {
		delete _balls[i];
	}
	delete _octree;
}

// create any number of balls passed into numBalls
void createBalls(int numBalls) {
	for(int i = 0; i < numBalls; i++) {
		Ball* ball = new Ball();
		ball->pos = Vec3f(8 * randomFloat() - 4,
						  8 * randomFloat() - 4,
						  8 * randomFloat() - 4);
		ball->v = Vec3f(8 * randomFloat() - 4,
						8 * randomFloat() - 4,
						8 * randomFloat() - 4);
		ball->r = 0.1f * randomFloat() + 0.1f;
		ball->color = Vec3f(0.6f * randomFloat() + 0.2f,
							0.6f * randomFloat() + 0.2f,
							0.6f * randomFloat() + 0.2f);


		switch (rand() % NUM_SOUNDS) {
			case 1:
				ball->sound = ms_b1;
				break;
			case 2:
				ball->sound = ms_b2;
				break;
			case 3:
				ball->sound = ms_b3;
				break;
			case 4:
				ball->sound = ms_b4;
				break;
			case 5:
				ball->sound = ms_b5;
				break;
			default:
				ball->sound = ms_b1;
				break;
		}

		_balls.push_back(ball);
		_octree->add(ball);
	}
}

void initRendering() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

/*
	ImageBMP *image = loadBMP("vtr.bmp");
	_textureId = load_texture(image);
	delete image;
*/
}

void handleResize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (float)w / (float)h, 1.0, 200.0);
}

void drawScene() {
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

/*
	glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -20.0f);
	glRotatef(-_angle, 0.0f, 1.0f, 0.0f);
*/

	//Draw the balls
	for(unsigned int i = 0; i < _balls.size(); i++) {
		Ball* ball = _balls[i];
		glPushMatrix();
		glTranslatef(ball->pos[0], ball->pos[1], ball->pos[2]);
		glColor3f(ball->color[0], ball->color[1], ball->color[2]);
	  GLUquadric *quadric = gluNewQuadric();
	  gluQuadricOrientation(quadric, GLU_OUTSIDE);
	  gluSphere(quadric, ball->r, 32, 32);
		glPopMatrix();
	}
}

//Called every TIMER_MS milliseconds
void update(int value) {
	advance(_balls, _octree, (float)TIMER_MS / 1000.0f, _timeUntilUpdate);
	_angle += (float)TIMER_MS / 100;
	if (_angle > 360) {
		_angle -= 360;
	}

	//glutPostRedisplay();
	//glutTimerFunc(TIMER_MS, update, 0);
}

/*
int main(int argc, char** argv) {
	srand((unsigned int)time(0)); //Seed the random number generator

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(400, 400);

	glutCreateWindow("Collision Detection - videotutorialsrock.com");
	initRendering();

	_octree = new Octree(Vec3f(-BOX_SIZE / 2, -BOX_SIZE / 2, -BOX_SIZE / 2),
						 Vec3f(BOX_SIZE / 2, BOX_SIZE / 2, BOX_SIZE / 2), 1);

	glutDisplayFunc(drawScene);
	glutKeyboardFunc(createBalls);
	glutReshapeFunc(handleResize);
	glutTimerFunc(TIMER_MS, update, 0);

	glutMainLoop();
	return 0;
}
*/

void initBounceSounds() {

  char b1[50] = "sounds/jump.wav";
  ms_b1 = SM.LoadSound(b1);
  //char b2[50] = "sounds/bounce2.mp3";
  ms_b2 = SM.LoadSound(b1);
  char b3[50] = "sounds/bounce3.mp3";
  ms_b3 = SM.LoadMusic(b3);
  char b4[50] = "sounds/bounce4.mp3";
  ms_b4 = SM.LoadMusic(b4);
  char b5[50] = "sounds/bounce5.mp3";
  ms_b5 = SM.LoadMusic(b5);
}
