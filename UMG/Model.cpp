#include "Model.h"

bool Model::load(string filename)
{
	//Fancy code goes here
	ifstream in(filename.c_str(), fstream::in);

	if(!in.good()) 
		return false;

	char temp[256];

	int line = 0;

	while(!in.eof())
	{
		//read a character
		string type;
		in >> type;

		if(type.size() == 0) 
			break;

		if( type.at(0) == '#' )
		{
			in.getline(temp, 256);
		}
		else if( type.at(0) == 'o' )
		{
			in >> name;
		}
		else if( type.at(0) == 'v' )
		{
			if( type.size() == 1)
			{
				vertex v;
				in >> v.v[0];
				in >> v.v[1];
				in >> v.v[2];
				verteces.push_back(v);
			}
			else if( type.at(1) == 'n' )
			{
				normal n;
				in >> n.n[0];
				in >> n.n[1];
				in >> n.n[2];
				normals.push_back(n);
			}
			else
			{
				textureCoord tC;
				in >> tC.t[0];
				in >> tC.t[1];
				textureCoordinates.push_back(tC);
			}

			//in.getline(temp, 256);
		}
		else if( type.at(0) == 'f' )
		{
			indexes i;
			//char t;

			in.getline(temp, 256);

			string face = string(temp);
			face = face.substr(1, face.size()); //remove leading space

			vector<string> parts;
			
			int index = 0;

			while(index != face.size())
			{
				if(face.at(index) == ' ')
				{
					parts.push_back(face.substr(0, index));
					face = face.substr(index+1, face.size());
					index = 0;
				}
				else
				{
					index++;
				}
			}

			parts.push_back(face);

			i.num = (int)parts.size();

			int vertexIndex;
			int normalIndex;
			int textureIndex;
			
			getIndexes(parts.at(0), vertexIndex, normalIndex, textureIndex);
			i.vertexIndex[0] = vertexIndex;
			i.normalIndex[0] = normalIndex;
			i.textureIndex[0] = textureIndex;
			getIndexes(parts.at(1), vertexIndex, normalIndex, textureIndex);
			i.vertexIndex[1] = vertexIndex;
			i.normalIndex[1] = normalIndex;
			i.textureIndex[1] = textureIndex;
			getIndexes(parts.at(2), vertexIndex, normalIndex, textureIndex);
			i.vertexIndex[2] = vertexIndex;
			i.normalIndex[2] = normalIndex;
			i.textureIndex[2] = textureIndex;
			
			if(i.num == 4)
			{
				getIndexes(parts.at(3), vertexIndex, normalIndex, textureIndex);
				i.vertexIndex[3] = vertexIndex;
				i.normalIndex[3] = normalIndex;
				i.textureIndex[3] = textureIndex;
			}


			triangleIndexes.push_back(i);
		}	
		else
		{
			in.getline(temp, 256);
		}

		line++;
	}

	in.close();
	return true;

}

void Model::getIndexes(string& s, int& v, int& n, int&t)
{
	int index = 0;
	int tStartIndex;
	while(s.at(index) != '/')
	{
		index++;
	}

	v = atoi(s.substr(0, index).c_str());
	index++;
	tStartIndex=index;

	while(s.at(index) != '/')
	{
		index++;
	}
	t = atoi(s.substr(tStartIndex, index).c_str());
	n = atoi(s.substr(index+1, s.size()).c_str());
}




void Model::render()
{
	glPushMatrix();

	//temporary!
//	glTranslatef(75.0f, 0.0f, -50.0f);
//	glScalef(0.5f, 0.5f, 0.5f);
	//glRotatef(, 1.0f, 0.0f, 0.0f);
//	glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	//Fancy code goes here

	bool areRenderingQuads = true;
	glBegin(GL_QUADS);
	
	int vertex1, vertex2, vertex3, vertex4;
	int normal1, normal2, normal3, normal4;
	int texture1, texture2, texture3, texture4;
	
	for(int x=0; x<(int)triangleIndexes.size(); x++)
	{
		indexes i = triangleIndexes.at(x);
		
		if( glGetError() != GL_NO_ERROR )
		{
			i.num = 0;
		}

		if(i.num == 3 && areRenderingQuads)
		{
			glEnd();
			glBegin(GL_TRIANGLES);
			areRenderingQuads = false;
		}
		else if(i.num == 4 && !areRenderingQuads)
		{
			glEnd();
			glBegin(GL_QUADS);
			areRenderingQuads = true;
		}

		vertex1 = i.vertexIndex[0] - 1;
		normal1 = i.normalIndex[0] - 1;
		texture1 = i.textureIndex[0] - 1;
		glNormal3fv(normals.at(normal1).n);
		if(texture1 != -1)
			glTexCoord2f(textureCoordinates.at(texture1).t[0], textureCoordinates.at(texture1).t[1]);
		glVertex3fv(verteces.at(vertex1).v);

		vertex2 = i.vertexIndex[1] - 1;
		normal2 = i.normalIndex[1] - 1;
		texture2 = i.textureIndex[1] - 1;
		glNormal3fv(normals.at(normal2).n);
		if(texture2 != -1)
			glTexCoord2f(textureCoordinates.at(texture2).t[0], textureCoordinates.at(texture2).t[1]);
		glVertex3fv(verteces.at(vertex2).v);

		vertex3 = i.vertexIndex[2] - 1;
		normal3 = i.normalIndex[2] - 1;
		texture3 = i.textureIndex[2] - 1;
		glNormal3fv(normals.at(normal3).n);
		if(texture3 != -1)
			glTexCoord2f(textureCoordinates.at(texture3).t[0], textureCoordinates.at(texture3).t[1]);
		glVertex3fv(verteces.at(vertex3).v);


		if(areRenderingQuads)
		{
			vertex4 = i.vertexIndex[3] - 1;
			normal4 = i.normalIndex[3] - 1;
			texture4 = i.textureIndex[3] - 1;
			glNormal3fv(normals.at(normal4).n);
			if(texture4 != -1)
				glTexCoord2f(textureCoordinates.at(texture4).t[0], textureCoordinates.at(texture4).t[1]);
			glVertex3fv(verteces.at(vertex4).v);
		}
		
		if( glGetError() != GL_NO_ERROR )
		{
			i.num = 0;
		}

	}
	glEnd();
	glPopMatrix();
}