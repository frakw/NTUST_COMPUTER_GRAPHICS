#pragma once

#include <glad/glad.h>
#include <stdlib.h>
#include <ctime>
#include <cmath>


struct cube_particle
{
	GLfloat xpos;//(xpos,ypos,zpos)為particle的position
	GLfloat ypos;
	GLfloat zpos;
	GLfloat xspeed;//(xspeed,yspeed,zspeed)為particle的speed 
	GLfloat yspeed;
	GLfloat zspeed;
	GLfloat r;//(r,g,b)為particle的color
	GLfloat g;
	GLfloat b;
	GLfloat life;// particle的壽命 
	GLfloat fade;// particle的衰減速度
	GLfloat size;// particle的大小  
	GLbyte bFire;
	GLbyte nExpl;//哪種particle效果  
	GLbyte bAddParts;// particle是否含有尾巴
	GLfloat AddSpeed;//尾巴粒子的加速度  
	GLfloat AddCount;//尾巴粒子的增加量  
	cube_particle* pNext;//下一particle 
	cube_particle* pPrev;//上一particle   
};


void InitParticle(cube_particle& ep);
void AddParticle(cube_particle ex);
void DeleteParticle(cube_particle** p);

void Explosion1(cube_particle* par);
void Explosion2(cube_particle* par);
void Explosion3(cube_particle* par);
void Explosion4(cube_particle* par);
void Explosion5(cube_particle* par);
void Explosion6(cube_particle* par);
void Explosion7(cube_particle* par);

void ProcessParticles();
void DrawParticles();


class Particle2
{
public:
};