#pragma once
#include "Movable.h"
#include <vector>

using namespace std;

class Move
{
private:
	vector<Movable*> movingObjects;

public:
	void moveAll(void) {
		unsigned int s = movingObjects.size();
		for (unsigned int i = 0; i < s; i++) {
			movingObjects[i]->move();
		}
	}

	void add( Movable* r)
	{
		movingObjects.push_back(r);
	}

	void remove( Movable* m)
	{
		for(int x=0; x<(int)movingObjects.size(); x++)
		{
			if(movingObjects.at(x) == m)
			{
				movingObjects.erase(movingObjects.begin()+x);
				return;
			}
		}
	}
};

