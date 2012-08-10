#ifndef _TEXTURE_H_
#define _TEXTURE_H_
#include <string>
#include <vector>
#include "LTGA.h"
#include "Graphics.h"

using namespace std;

struct texPair
{
	string name;
	GLuint textureNum;
};

class TextureContainer{
	vector<texPair> list;
	GLuint failTextureNum;

public:
	TextureContainer()
	{
		list = vector<texPair>();
		string failName("Resources/fail.tga");
		failTextureNum = add(failName);

	}
	~TextureContainer()
	{
	}

	//Creates and allocates new obstacle
	GLuint add( string filename )
	{
		//start by looking to see if we've added it already
		int numTextures = (int)list.size();
		for(int x=0; x<numTextures; x++)
		{
			if(list.at(x).name.compare(filename)==0)
				return list.at(x).textureNum;
		}

		LTGA* myTexture = new LTGA(filename);
		
		byte* myPixels = myTexture->GetPixels();

		if(myPixels == NULL) return list.at(0).textureNum;

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		GLuint myTextureNum;
		glGenTextures(1, &myTextureNum);

		texPair tP;
		tP.name = filename;
		tP.textureNum = myTextureNum;
		list.push_back(tP);
		
		glBindTexture(GL_TEXTURE_2D, myTextureNum);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		int imageWidth = myTexture->GetImageWidth();
		int imageHeight = myTexture->GetImageHeight();

		int pixelDepth = myTexture->GetPixelDepth();
		int alphaDepth = myTexture->GetAlphaDepth();
		int imageType = myTexture->GetImageType();

		//copy to the graphics card
		if(alphaDepth == 8)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, myPixels); 
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, myPixels); 
		}

		delete myTexture;
		myTexture = NULL;

		return myTextureNum;
		
	}

/*	void remove( int i )
	{
	}
	*/

	void bind( string name )
	{
		//look through list of textures and find the one that matches
		int numTextures = (int)list.size();
		for(int x=0; x<numTextures; x++)
		{
			if(list.at(x).name.compare(name) == 0)	
			{
				//bind here
				glBindTexture(GL_TEXTURE_2D, list.at(x).textureNum);
				return;
			}
		}

		//if we don't have the texture, bind on the fail texture
		glBindTexture(GL_TEXTURE_2D, failTextureNum);
		
	}

	void unbind( void )
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	//goes through all textures and removes them all
	void clean( void )
	{
	}

};

#endif