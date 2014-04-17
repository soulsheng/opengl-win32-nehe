
#include "CChaffBomb.h"

#include <math.h>			// Math Library Header File
#include <stdio.h>					// Header File For Standard Input/Output
#include <gl\gl.h>					// Header File For The OpenGL32 Library


#define	BOX_PARTICLES	1			// Box or Plane

#define LEN_PARTICLE	1.0f		// Lenth  x
#define WID_PARTICLE	0.1f		// Width  z
#define HEI_PARTICLE	0.1f		// Height y

#define	MAX_PARTICLES	1000		// Number Of Particles To Create
#define BMP_PARTICLES	"Data/Wood.bmp"// Particle.bmp


void CChaffBomb::DrawParticle(float x, float y, float z )
{
#if !BOX_PARTICLES	// Quad Plane
	glBegin(GL_TRIANGLE_STRIP);						// Build Quad From A Triangle Strip
	glTexCoord2d(1,1); glVertex3f(x+0.5f,y+0.5f,z); // Top Right
	glTexCoord2d(0,1); glVertex3f(x-0.5f,y+0.5f,z); // Top Left
	glTexCoord2d(1,0); glVertex3f(x+0.5f,y-0.5f,z); // Bottom Right
	glTexCoord2d(0,0); glVertex3f(x-0.5f,y-0.5f,z); // Bottom Left
	glEnd();

#else	// Cubic Box

	glBegin(GL_QUADS);
	// Front Face
	glTexCoord2d(0,0); glVertex3f(x-length, y-height, z+width);
	glTexCoord2d(1,0); glVertex3f(x+length, y-height, z+width);
	glTexCoord2d(1,1); glVertex3f(x+length, y+height, z+width);
	glTexCoord2d(0,1); glVertex3f(x-length, y+height, z+width);
	// Back Face
	glTexCoord2d(1,0); glVertex3f(x-length, y-height, z-width);
	glTexCoord2d(1,1); glVertex3f(x-length, y+height, z-width);
	glTexCoord2d(0,1); glVertex3f(x+length, y+height, z-width);
	glTexCoord2d(0,0); glVertex3f(x+length, y-height, z-width);
	// Top Face
	glTexCoord2d(0,1); glVertex3f(x-length, y+height, z-width);
	glTexCoord2d(0,0); glVertex3f(x-length, y+height, z+width);
	glTexCoord2d(1,0); glVertex3f(x+length, y+height, z+width);
	glTexCoord2d(1,1); glVertex3f(x+length, y+height, z-width);
	// Bottom Face
	glTexCoord2d(1,1); glVertex3f(x-length, y-height, z-width);
	glTexCoord2d(0,1); glVertex3f(x+length, y-height, z-width);
	glTexCoord2d(0,0); glVertex3f(x+length, y-height, z+width);
	glTexCoord2d(1,0); glVertex3f(x-length, y-height, z+width);
	// Right face
	glTexCoord2d(1,0); glVertex3f(x+length, y-height, z-width);
	glTexCoord2d(1,1); glVertex3f(x+length, y+height, z-width);
	glTexCoord2d(0,1); glVertex3f(x+length, y+height, z+width);
	glTexCoord2d(0,0); glVertex3f(x+length, y-height, z+width);
	// Left Face
	glTexCoord2d(0,0); glVertex3f(x-length, y-height, z-width);
	glTexCoord2d(1,0); glVertex3f(x-length, y-height, z+width);
	glTexCoord2d(1,1); glVertex3f(x-length, y+height, z+width);
	glTexCoord2d(0,1); glVertex3f(x-length, y+height, z-width);
	glEnd();

#endif
}

CChaffBomb::CChaffBomb()
	:length( LEN_PARTICLE ), width( WID_PARTICLE ), height( HEI_PARTICLE )
	,m_particles( NULL ), slowdown( 2.0f )
	,centerX( 0.0f ), centerY( 0.0f ), centerZ( 0.0f )
{
}

CChaffBomb::~CChaffBomb()
{

}

void CChaffBomb::RefreshParticle(int loop)
{
	m_particles[loop].x+=m_particles[loop].xi/(slowdown*1000);// Move On The X Axis By X Speed
	m_particles[loop].y+=m_particles[loop].yi/(slowdown*1000);// Move On The Y Axis By Y Speed
	m_particles[loop].z+=m_particles[loop].zi/(slowdown*1000);// Move On The Z Axis By Z Speed

	m_particles[loop].yi+=m_particles[loop].yg;			// Take Pull On Y Axis Into Account
}

void CChaffBomb::DrawChaffBomb()
{
	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping
	glBindTexture(GL_TEXTURE_2D, textureID);			// Select Our Texture

	for (int loop=0;loop<MAX_PARTICLES;loop++)					// Loop Through All The Particles
	{
		DrawParticle( m_particles[loop].x, m_particles[loop].y, m_particles[loop].z);		// Done Building Triangle Strip

		RefreshParticle( loop );
	}
}

AUX_RGBImageRec *CChaffBomb::LoadBMP(char *Filename)				// Loads A Bitmap Image
{
	FILE *File=NULL;								// File Handle
	if (!Filename)									// Make Sure A Filename Was Given
	{
		return NULL;							// If Not Return NULL
	}
	File=fopen(Filename,"r");						// Check To See If The File Exists
	if (File)										// Does The File Exist?
	{
		fclose(File);								// Close The Handle
		return auxDIBImageLoad(Filename);			// Load The Bitmap And Return A Pointer
	}
	return NULL;									// If Load Failed Return NULL
}

int CChaffBomb::LoadGLTextures()									// Load Bitmap And Convert To A Texture
{
	int Status=FALSE;								// Status Indicator
	AUX_RGBImageRec *TextureImage[1];				// Create Storage Space For The Textures
	memset(TextureImage,0,sizeof(void *)*1);		// Set The Pointer To NULL

	if (TextureImage[0]=LoadBMP( BMP_PARTICLES ))	// Load Particle Texture
	{
		Status=TRUE;								// Set The Status To TRUE
		glGenTextures(1, &textureID);				// Create One Texture

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
	}

	if (TextureImage[0])							// If Texture Exists
	{
		if (TextureImage[0]->data)					// If Texture Image Exists
		{
			free(TextureImage[0]->data);			// Free The Texture Image Memory
		}
		free(TextureImage[0]);						// Free The Image Structure
	}
	return Status;									// Return The Status
}

bool CChaffBomb::initialize(float centerX, float centerY, float centerZ)
{
	if (!LoadGLTextures())								// Jump To Texture Loading Routine
	{
		return false;									// If Texture Didn't Load Return FALSE
	}

	if( m_particles )
		delete[] m_particles;

	m_particles = new CParticle[MAX_PARTICLES];

	for (int i=0;i<MAX_PARTICLES;i++)
	{
		m_particles[i].x = centerX;
		m_particles[i].y = centerY;
		m_particles[i].z = centerZ;
	}

	this->centerX = centerX;
	this->centerY = centerY;
	this->centerZ = centerZ;

	return true;									// If Texture Didn't Load Return FALSE
}

void CChaffBomb::unInitialize()
{
	glDeleteTextures(1, &textureID);

	if ( m_particles )
	{
		delete m_particles;
		m_particles = NULL;
	}
}

CParticle::CParticle()
{
	xi=float((rand()%50)-26.0f)*10.0f;		// Random Speed On X Axis
	yi=float((rand()%50)-25.0f)*10.0f;		// Random Speed On Y Axis
	zi=float((rand()%50)-25.0f)*10.0f;		// Random Speed On Z Axis
	yg=-0.8f;								// Set Vertical Pull Downward
}
