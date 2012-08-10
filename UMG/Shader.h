#ifndef _SHADER_H_
#define _SHADER_H_

#include "Settings.h"
#ifdef ENABLE_SHADERS



#include <fstream>
#include <iostream>
#include <string>

using namespace std;

//shaderType must be either GL_VERTEX_SHADER or GL_FRAGMENT_SHADER

class Shader{

string shaderSource;
string errorMessages;
int shaderNumber;

public:
	Shader(void){}

	void initialize(string filename, GLenum shaderType){
        errorMessages.clear();

        ifstream instream(filename.c_str());

        if(!instream.good())
		{
            errorMessages+="Error: Shader \"";
            errorMessages+=filename;
            errorMessages+="\" could not be read!\n";
        }

        shaderSource.clear();

        while(instream.good())
		{
            string line;
            getline(instream, line);
            shaderSource+=line;
            shaderSource+='\n';
        }

        //cout<<"creating the shader number"<<endl;

        shaderNumber = glCreateShader(shaderType);

        if(shaderNumber==0)
		{
            errorMessages+="Error: glCreateShader failed with shaderType ";
            errorMessages+=shaderType;
            errorMessages+="\n";
        }

        const char* str = shaderSource.c_str();
        int len = (int)shaderSource.length();

        glShaderSourceARB(shaderNumber, 1, &str, &len);
        glCompileShaderARB(shaderNumber);

        //check to see if the compile succeeded or not
        int compiled;
        glGetObjectParameterivARB(shaderNumber, GL_OBJECT_COMPILE_STATUS_ARB, &compiled);
        if(!compiled){
            errorMessages+="Could not compile shader:\n";
            char log[2048];
            int logLength;
            glGetInfoLogARB(shaderNumber, 2047, &logLength, log);
            errorMessages+=log;
        }

        //cout<<"Finished with shader "<<filename<<endl;


    }

    string getErrors(){
        return errorMessages;
    }    

    int object(){
        return shaderNumber;
    }

    bool isReady(){
        return errorMessages.size()==0;
    }
    

};

        


#endif
#endif