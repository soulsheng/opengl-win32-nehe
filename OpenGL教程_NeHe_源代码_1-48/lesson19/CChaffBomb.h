
#include <gl\glaux.h>				// Header File For The Glaux Library

struct	CParticle				// Create A Structure For Particle
{
	float	x;						// X Position
	float	y;						// Y Position
	float	z;						// Z Position
	float	xi;						// X Speed
	float	yi;						// Y Speed
	float	zi;						// Z Speed
	float	yg;						// Y Gravity
	CParticle();
};							// Particles Structure

class CChaffBomb
{
public:
	CChaffBomb();
	~CChaffBomb();
	void DrawChaffBomb();

	bool initialize(float centerX=0.0f, float centerY=0.0f, float centerZ=0.0f);
	void unInitialize();

protected:
	void DrawParticle(float x, float y, float z );
	void RefreshParticle(int loop);

	AUX_RGBImageRec *LoadBMP(char *Filename);
	int LoadGLTextures();

private:
	CParticle*	m_particles;
	float length, width, height;	//	箔条形状，长宽高
	GLuint	textureID;				//	箔条纹理图片
	float	slowdown;			//	速度倍数

	float centerX, centerY, centerZ;	// 中心坐标
};