#ifndef _MODEL_H_
#define _MODEL_H_

#include <vector>
#include <fstream>
#include <string>
#include "Render.h"

using namespace std;

struct vertex{
	float v[3];
};

struct normal{
	float n[3];
};

struct textureCoord{
	float t[2];
};

struct indexes{
	int num;
	int vertexIndex[4];
	int textureIndex[4];
	int normalIndex[4];
};

class Model: public Renderable
{
private:
	vector<vertex> verteces;
	vector<normal> normals;
	vector<textureCoord> textureCoordinates;
	vector<indexes> triangleIndexes;

	void getIndexes(string&, int&, int&, int&);

public:
	string name;

	Model()
	{
		verteces = vector<vertex>();
		normals = vector<normal>();
		textureCoordinates = vector<textureCoord>();
		triangleIndexes = vector<indexes>();
	}
	~Model(){}

	
	bool load(string);

	void render(void);

};



class ModelContainer
{
private:
	Render* renderer;
	vector<Model*> list;
	Model* failModel;

public:
	ModelContainer(Render* r)
	{
		renderer = r;
		
		failModel = add("Resources/fail.obj");
	}
	~ModelContainer()
	{
		while(list.size()>0)
			delete list.at(0);
	}

	Model* add(string filename)
	{
		//check to see if model has already been loaded!
		int numModels = (int)list.size();
		for(int x=0; x<numModels; x++)
		{
			if(list.at(x)->name.compare(filename)==0)
				return list.at(x);
		}

		//if not, create the model from the file
		Model* m = new Model();
		if(! m->load(filename))
		{
			delete m;
			return failModel;
		}

		list.push_back(m);
		return m;
		//renderer->add(m);  //only for debugging!
	}


};

#endif