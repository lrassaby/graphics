#include "shader.h"
#include <iostream>
#include <fstream>
#include <vector>

Shader::Shader(void) {}

Shader::~Shader(void) {}

// read a file and return as a string
string Shader::readFile(const char* path) { 
    string content;
    ifstream fileStream(path, ios::in);

    if (!fileStream.is_open()) { 
        cerr << "File could not be opened" << endl;
        return "";
    }

    string line = "";
    while (!fileStream.eof()) { 
        getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}

GLuint Shader::loadShader(const char* vertPath, const char* fragPath) { 
    //generate shader names
    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

    //get shader src
    string vertShaderStr = readFile(vertPath);
    string fragShaderStr = readFile(fragPath);

    const char* vertShaderSrc = vertShaderStr.c_str();
    const char* fragShaderSrc = fragShaderStr.c_str();

    GLint result = GL_FALSE;
    int logLength;

    //compile vertex shader
    glShaderSource(vertShader, 1, &vertShaderSrc, NULL);
    glCompileShader(vertShader);
    //glGetShaderiv(vertShader, GL_COMPILE_STATUS, &result);
    //glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &logLength);
    //vector<char> vertShaderError(logLength);
    char error[1024];
    glGetShaderInfoLog(vertShader, 1024, NULL, error);
    cout << "Compiler errors: \n" << error << endl;

    //compile fragment shader
    glShaderSource(fragShader, 1, &fragShaderSrc, NULL);
    glCompileShader(fragShader);
    // glGetShaderiv(fragShader, GL_COMPILE_STATUS, &result);
    // glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &logLength);
    // vector<char> fragShaderError(logLength);
    glGetShaderInfoLog(fragShader, 1024, NULL, error);
    cout << "Compiler errors: \n" << error << endl;

    //link the program
    GLuint program = glCreateProgram();
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &result);
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
    vector<char> programError(logLength > 1 ? logLength : 1);
    glGetProgramInfoLog(program, logLength, NULL, &programError[0]);
    cout << &programError[0] << endl;

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

    return program;
}