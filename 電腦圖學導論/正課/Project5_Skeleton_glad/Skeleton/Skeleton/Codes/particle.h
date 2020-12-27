#pragma once

#include <glad/glad.h>
#include <stdlib.h>
#include <ctime>
#include <cmath>


typedef struct tag_PARTICLE
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
	tag_PARTICLE* pNext;//下一particle 
	tag_PARTICLE* pPrev;//上一particle   
} Particle;


void InitParticle(Particle& ep);
void AddParticle(Particle ex);
void DeleteParticle(Particle** p);

void Explosion1(Particle* par);
void Explosion2(Particle* par);
void Explosion3(Particle* par);
void Explosion4(Particle* par);
void Explosion5(Particle* par);
void Explosion6(Particle* par);
void Explosion7(Particle* par);

void ProcessParticles();
void DrawParticles();


class Particle2
{
public:
};