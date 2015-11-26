#include "Compiler.h"
#include "Util.h"
#include "Environment.h"
#include "FileInfo.h"
#include "features.h"

#include <iostream>

std::shared_ptr<Compiler> Compiler::create(std::shared_ptr<Environment> environment)
{
  static Compiler s;
  std::shared_ptr<Compiler> rtn(new Compiler(s));
  rtn->environment = environment;
  rtn->name = DEFAULT_CXX;

  if(environment->getCompilerName() != "")
  {
    rtn->name = environment->getCompilerName();
  }

  if(rtn->name == "cl")
  {
    Util::execute("call \"C:\\Program Files (x86)\\Microsoft Visual Studio 12.0\\VC\\vcvarsall.bat\"");
  }

  return rtn;
}

std::string Compiler::getName()
{
  return name;
}

void Compiler::addIncludeDirectory(std::string directory)
{
  includeDirectories.push_back(directory);
}

std::string Compiler::getObjectSuffix()
{
  if(name == "cl")
  {
    return "obj";
  }

  return "o";
}

std::string Compiler::getExecutableSuffix()
{
  if(FileInfo::getFileName(name) == "em++")
  {
    return "html";
  }

  if(name == "cl")
  {
    return "exe";
  }

  return "";
}

void Compiler::compile(std::string sourceUnit, std::string output)
{
  std::string program = name;
  std::string includeFragment = "";

  for(int i = 0; i < includeDirectories.size(); i++)
  {
    if(program == "cl")
    {
      includeFragment += " /I" + includeDirectories.at(i);
    }
    else
    {
      includeFragment += " -I" + includeDirectories.at(i);
    }
  }

  std::string command;

  if(program == "cl")
  {
    command = "call \"C:\\Program Files (x86)\\Microsoft Visual Studio 12.0\\VC\\vcvarsall.bat\" && " + program +
    //command = program +
      " /EHsc /c /DWINDOWS" +
      includeFragment +
      " " + sourceUnit +
      " /MD" +
      " /Fo" + output;
  }
  else
  {
    command = program + " -std=c++11 -c" +
      includeFragment +
      " " + sourceUnit +
      " -o" +
      " " + output;
  }

  std::cout << command << std::endl;
  std::string result = Util::execute(command);
}

void Compiler::addObjectDirectory(std::string directory)
{
  objectDirectories.push_back(directory);
}

void Compiler::addLibDirectory(std::string directory)
{
  libDirectories.push_back(directory);
}

void Compiler::addLib(std::string path)
{
  libs.push_back(path);
}

void Compiler::link(std::string output)
{
  std::string objectsFragment = "";

  for(int i = 0; i < objectDirectories.size(); i++)
  {
    objectsFragment += " " + objectDirectories.at(i) + Util::fixPath("/*.") + getObjectSuffix();
  }

  std::string libsFragment = "";

  for(int i = 0; i < libDirectories.size(); i++)
  {
    if(name == "cl")
    {
      libsFragment += " /LIBPATH:" + libDirectories.at(i);
    }
    else
    {
      libsFragment += " -L" + libDirectories.at(i);
    }
  }

  for(int i = 0; i < libs.size(); i++)
  {
    libsFragment += " " + libs.at(i);
  }

  std::string program = name;

  if(FileInfo::getFileName(name) == "em++")
  {
    libsFragment = "";
  }
  else if(name == "cl")
  {
    libsFragment += " SDL.lib SDLmain.lib SDL_mixer.lib glew32.lib opengl32.lib";
  }
  else
  {
    libsFragment += " -lGL -lGLEW -lSDL -lSDL_mixer";
  }

  std::string command;

  if(program == "cl")
  {
    command = "call \"C:\\Program Files (x86)\\Microsoft Visual Studio 12.0\\VC\\vcvarsall.bat\" && link" +
    //command = "link" +
      objectsFragment +
      " /out:" + output +
      " /subsystem:console" +
      libsFragment;
  }
  else
  {
    command = program +
      objectsFragment +
      " -o" +
      " " + output +
      libsFragment;
  }

  std::cout << command << std::endl;
  std::string result = Util::execute(command);
}