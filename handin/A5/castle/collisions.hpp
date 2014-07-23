#ifndef CS488_COLLISIONS_HPP
#define CS488_COLLISIONS_HPP

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <set>
#include <stdlib.h>
#include <vector>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#include "imageloader.h"
#include "vec3f.h"
#include "texture.hpp"

using namespace std;

float randomFloat();
extern const float GRAVITY;
extern const float BOX_SIZE;
extern const float TIME_BETWEEN_UPDATES;
extern const int TIMER_MS;

//Stores information regarding a ball
struct Ball {
  Vec3f v; //Velocity
  Vec3f pos; //Position
  float r; //Radius
  Vec3f color;
};

enum Wall {WALL_LEFT, WALL_RIGHT, WALL_FAR, WALL_NEAR, WALL_TOP, WALL_BOTTOM};

//Stores a pair of balls
struct BallPair {
  Ball* ball1;
  Ball* ball2;
};

//Stores a ball and a wall
struct BallWallPair {
  Ball* ball;
  Wall wall;
};

extern const int MAX_OCTREE_DEPTH;
extern const int MIN_BALLS_PER_OCTREE;
extern const int MAX_BALLS_PER_OCTREE;

//Our data structure for making collision detection faster
class Octree {
  private:
    Vec3f corner1; //(minX, minY, minZ)
    Vec3f corner2; //(maxX, maxY, maxZ)
    Vec3f center;//((minX + maxX) / 2, (minY + maxY) / 2, (minZ + maxZ) / 2)

    /* The children of this, if this has any.  children[0][*][*] are the
     * children with x coordinates ranging from minX to centerX.
     * children[1][*][*] are the children with x coordinates ranging from
     * centerX to maxX.  Similarly for the other two dimensions of the
     * children array.
     */
    Octree *children[2][2][2];
    //Whether this has children
    bool hasChildren;
    //The balls in this, if this doesn't have any children
    set<Ball*> balls;
    //The depth of this in the tree
    int depth;
    //The number of balls in this, including those stored in its children
    int numBalls;

    //Adds a ball to or removes one from the children of this
    void fileBall(Ball* ball, Vec3f pos, bool addBall) {
      //Figure out in which child(ren) the ball belongs
      for(int x = 0; x < 2; x++) {
        if (x == 0) {
          if (pos[0] - ball->r > center[0]) {
            continue;
          }
        }
        else if (pos[0] + ball->r < center[0]) {
          continue;
        }

        for(int y = 0; y < 2; y++) {
          if (y == 0) {
            if (pos[1] - ball->r > center[1]) {
              continue;
            }
          }
          else if (pos[1] + ball->r < center[1]) {
            continue;
          }

          for(int z = 0; z < 2; z++) {
            if (z == 0) {
              if (pos[2] - ball->r > center[2]) {
                continue;
              }
            }
            else if (pos[2] + ball->r < center[2]) {
              continue;
            }

            //Add or remove the ball
            if (addBall) {
              children[x][y][z]->add(ball);
            }
            else {
              children[x][y][z]->remove(ball, pos);
            }
          }
        }
      }
    }

    //Creates children of this, and moves the balls in this to the children
    void haveChildren() {
      for(int x = 0; x < 2; x++) {
        float minX;
        float maxX;
        if (x == 0) {
          minX = corner1[0];
          maxX = center[0];
        }
        else {
          minX = center[0];
          maxX = corner2[0];
        }

        for(int y = 0; y < 2; y++) {
          float minY;
          float maxY;
          if (y == 0) {
            minY = corner1[1];
            maxY = center[1];
          }
          else {
            minY = center[1];
            maxY = corner2[1];
          }

          for(int z = 0; z < 2; z++) {
            float minZ;
            float maxZ;
            if (z == 0) {
              minZ = corner1[2];
              maxZ = center[2];
            }
            else {
              minZ = center[2];
              maxZ = corner2[2];
            }

            children[x][y][z] = new Octree(Vec3f(minX, minY, minZ),
                             Vec3f(maxX, maxY, maxZ),
                             depth + 1);
          }
        }
      }

      //Remove all balls from "balls" and add them to the new children
      for(set<Ball*>::iterator it = balls.begin(); it != balls.end();
          it++) {
        Ball* ball = *it;
        fileBall(ball, ball->pos, true);
      }
      balls.clear();

      hasChildren = true;
    }

    //Adds all balls in this or one of its descendants to the specified set
    void collectBalls(set<Ball*> &bs) {
      if (hasChildren) {
        for(int x = 0; x < 2; x++) {
          for(int y = 0; y < 2; y++) {
            for(int z = 0; z < 2; z++) {
              children[x][y][z]->collectBalls(bs);
            }
          }
        }
      }
      else {
        for(set<Ball*>::iterator it = balls.begin(); it != balls.end();
            it++) {
          Ball* ball = *it;
          bs.insert(ball);
        }
      }
    }

    //Destroys the children of this, and moves all balls in its descendants
    //to the "balls" set
    void destroyChildren() {
      //Move all balls in descendants of this to the "balls" set
      collectBalls(balls);

      for(int x = 0; x < 2; x++) {
        for(int y = 0; y < 2; y++) {
          for(int z = 0; z < 2; z++) {
            delete children[x][y][z];
          }
        }
      }

      hasChildren = false;
    }

    //Removes the specified ball at the indicated position
    void remove(Ball* ball, Vec3f pos) {
      numBalls--;

      if (hasChildren && numBalls < MIN_BALLS_PER_OCTREE) {
        destroyChildren();
      }

      if (hasChildren) {
        fileBall(ball, pos, false);
      }
      else {
        balls.erase(ball);
      }
    }

    /* Helper fuction for potentialBallWallCollisions(vector).  Adds
     * potential ball-wall collisions to cs, where w is the type of wall,
     * coord is the relevant coordinate of the wall ('x', 'y', or 'z'), and
     * dir is 0 if the wall is in the negative direction and 1 if it is in
     * the positive direction.  Assumes that this is in the extreme
     * direction of the coordinate, e.g. if w is WALL_TOP, the function
     * assumes that this is in the far upward direction.
     */
    void potentialBallWallCollisions(vector<BallWallPair> &cs,
                     Wall w, char coord, int dir) {
      if (hasChildren) {
        //Recursively call potentialBallWallCollisions on the correct
        //half of the children (e.g. if w is WALL_TOP, call it on
        //children above centerY)
        for(int dir2 = 0; dir2 < 2; dir2++) {
          for(int dir3 = 0; dir3 < 2; dir3++) {
            Octree *child;
            switch (coord) {
              case 'x':
                child = children[dir][dir2][dir3];
                break;
              case 'y':
                child = children[dir2][dir][dir3];
                break;
              case 'z':
                child = children[dir2][dir3][dir];
                break;
            }

            child->potentialBallWallCollisions(cs, w, coord, dir);
          }
        }
      }
      else {
        //Add (ball, w) for all balls in this
        for(set<Ball*>::iterator it = balls.begin(); it != balls.end();
            it++) {
          Ball* ball = *it;
          BallWallPair bwp;
          bwp.ball = ball;
          bwp.wall = w;
          cs.push_back(bwp);
        }
      }
    }
  public:
    //Constructs a new Octree.  c1 is (minX, minY, minZ), c2 is (maxX, maxY,
    //maxZ), and d is the depth, which starts at 1.
    Octree(Vec3f c1, Vec3f c2, int d) {
      corner1 = c1;
      corner2 = c2;
      center = (c1 + c2) / 2;
      depth = d;
      numBalls = 0;
      hasChildren = false;
    }

    //Destructor
    ~Octree() {
      if (hasChildren) {
        destroyChildren();
      }
    }

    //Adds a ball to this
    void add(Ball* ball) {
      numBalls++;
      if (!hasChildren && depth < MAX_OCTREE_DEPTH &&
        numBalls > MAX_BALLS_PER_OCTREE) {
        haveChildren();
      }

      if (hasChildren) {
        fileBall(ball, ball->pos, true);
      }
      else {
        balls.insert(ball);
      }
    }

    //Removes a ball from this
    void remove(Ball* ball) {
      remove(ball, ball->pos);
    }

    //Changes the position of a ball in this from oldPos to ball->pos
    void ballMoved(Ball* ball, Vec3f oldPos) {
      remove(ball, oldPos);
      add(ball);
    }

    //Adds potential ball-ball collisions to the specified set
    void potentialBallBallCollisions(vector<BallPair> &collisions) {
      if (hasChildren) {
        for(int x = 0; x < 2; x++) {
          for(int y = 0; y < 2; y++) {
            for(int z = 0; z < 2; z++) {
              children[x][y][z]->
                potentialBallBallCollisions(collisions);
            }
          }
        }
      }
      else {
        //Add all pairs (ball1, ball2) from balls
        for(set<Ball*>::iterator it = balls.begin(); it != balls.end();
            it++) {
          Ball* ball1 = *it;
          for(set<Ball*>::iterator it2 = balls.begin();
              it2 != balls.end(); it2++) {
            Ball* ball2 = *it2;
            //This test makes sure that we only add each pair once
            if (ball1 < ball2) {
              BallPair bp;
              bp.ball1 = ball1;
              bp.ball2 = ball2;
              collisions.push_back(bp);
            }
          }
        }
      }
    }

    //Adds potential ball-wall collisions to the specified set
    void potentialBallWallCollisions(vector<BallWallPair> &collisions) {
      potentialBallWallCollisions(collisions, WALL_LEFT, 'x', 0);
      potentialBallWallCollisions(collisions, WALL_RIGHT, 'x', 1);
      potentialBallWallCollisions(collisions, WALL_BOTTOM, 'y', 0);
      potentialBallWallCollisions(collisions, WALL_TOP, 'y', 1);
      potentialBallWallCollisions(collisions, WALL_FAR, 'z', 0);
      potentialBallWallCollisions(collisions, WALL_NEAR, 'z', 1);
    }
};

//Puts potential ball-ball collisions in potentialCollisions.  It must return
//all actual collisions, but it need not return only actual collisions.
void potentialBallBallCollisions(vector<BallPair> &potentialCollisions, vector<Ball*> &balls, Octree* octree);

//Puts potential ball-wall collisions in potentialCollisions.  It must return
//all actual collisions, but it need not return only actual collisions.
void potentialBallWallCollisions(vector<BallWallPair> &potentialCollisions, vector<Ball*> &balls, Octree* octree);

//Moves all of the balls by their velocity times dt
void moveBalls(vector<Ball*> &balls, Octree* octree, float dt);

//Decreases the y coordinate of the velocity of each ball by GRAVITY *
//TIME_BETWEEN_UPDATES
void applyGravity(vector<Ball*> &balls);

//Returns whether two balls are colliding
bool testBallBallCollision(Ball* b1, Ball* b2);

//Handles all ball-ball collisions
void handleBallBallCollisions(vector<Ball*> &balls, Octree* octree);

//Returns the direction from the origin to the wall
Vec3f wallDirection(Wall wall);

//Returns whether a ball and a wall are colliding
bool testBallWallCollision(Ball* ball, Wall wall);

//Handles all ball-wall collisions
void handleBallWallCollisions(vector<Ball*> &balls, Octree* octree);


//Applies gravity and handles all collisions.  Should be called every
//TIME_BETWEEN_UPDATES seconds.
void performUpdate(vector<Ball*> &balls, Octree* octree);

//Advances the state of the balls by t.  timeUntilUpdate is the amount of time
//until the next call to performUpdate.
void advance(vector<Ball*> &balls, Octree* octree, float t, float &timeUntilUpdate);

extern vector<Ball*> _balls; //All of the balls in play
extern float _angle;
extern Octree* _octree; //An octree with all af the balls
//The amount of time until performUpdate should be called
extern float _timeUntilUpdate;
extern GLuint _textureId;

//Deletes everything.  This should be called when exiting the program.
void cleanup();

void createBalls(int numBalls);

void initRendering();

void handleResize(int w, int h);

void drawScene();

//Called every TIMER_MS milliseconds
void update(int value);

#endif