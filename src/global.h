#ifndef GLOBAL_H_
#define GLOBAL_H_

#pragma once

#include <memory>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <windows.h>
#include <gl/glew.h>
#include <gl/wglew.h>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glm/ext.hpp"

using namespace std;

#define FOR(q,n) for(int q=0;q<n;q++)
#define SFOR(q,s,e) for(int q=s;q<=e;q++)
#define RFOR(q,n) for(int q=n;q>=0;q--)
#define RSFOR(q,s,e) for(int q=s;q>=e;q--)

#define ESZ(elem) (int)elem.size()

#define A_KEY  0x41
#define D_KEY  0x44
#define E_KEY  0x45
#define K_KEY  0x4B
#define L_KEY  0x4C
#define R_KEY  0x52
#define S_KEY  0x53
#define W_KEY  0x57
#define F1_KEY 0x70
#define F2_KEY 0x71
#define SPACE_KEY 0x20

extern int WINDOW_X;
extern int WINDOW_Y;

extern const bool FULL_SCREEN;
extern const bool VSYNC_ENABLED;

extern const float PI;
extern const float SCREEN_DEPTH;
extern const float SCREEN_NEAR;

extern float SCREEN_ASPECT_RATIO;
extern float FOV;

extern bool WIREFRAME;
extern bool VOLUMETRIC_LIGHT;

extern std::vector<std::string> split(std::string str, std::string pattern);

const float INV_RAND_MAX = 1.0 / (RAND_MAX + 1);
inline float random(float max=1.0) { return max * rand() * INV_RAND_MAX; }
inline float random(float min, float max) { return min + (max - min) * INV_RAND_MAX * rand(); }
inline int random(int max=RAND_MAX) { return rand()%(max+1); }

#endif /* GLOBAL_H_ */
