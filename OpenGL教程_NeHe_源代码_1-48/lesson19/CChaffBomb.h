
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
	float length, width, height;	//	������״�������
	GLuint	textureID;				//	��������ͼƬ
	float	slowdown;			//	�ٶȱ���

	float centerX, centerY, centerZ;	// ��������
};