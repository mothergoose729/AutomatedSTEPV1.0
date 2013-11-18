#include <iostream>
#include <fstream>
#include <string>
#include <ctype.h>
using namespace std;

void unpackModDirectories(string);
void cleanup(string);
void specialOps(ifstream&, string);

const char* CONFIG = "source/config";
const char* ARCHIVENAME = "source/name";
const char* SOURCEDIRECTORYFILETXT = "source/sourceDirectory";
const char* UNARCHIVECOMMAND = "7z x \"";
const char* UNARCHIVEFLAGS = "\" -aoa -y -o";
const char* dataDir = "data";
const char* XCOPYCOMMAND = "xcopy /f /e /y /c \"";
const char* DELETE = "rd /s /q \"";
const char* ROBOCOPY = "robocopy /MOVE /e /is \"";
const char* MKDIR = "mkdir \""; 
int main(int argc, char *argv[])
{
    string command;
    string buffer;
    string sourceDir;
    ifstream sourceDirFile(SOURCEDIRECTORYFILETXT);
    getline(sourceDirFile, sourceDir);
    sourceDirFile.close();
    ifstream config(CONFIG);
    command = MKDIR + sourceDir + "\"";
    cout << command << endl;
    system(command.c_str());
    command = MKDIR + sourceDir + "/" + dataDir + "\"";
    cout << command << endl;
    system(command.c_str());
    command = UNARCHIVECOMMAND;
    system("mkdir temp");
    while(getline(config, buffer))
    {
     if (buffer[0] == '!')
     {
      specialOps(config, buffer);
     }
     else
     {
      command+= buffer + UNARCHIVEFLAGS + sourceDir;
      cout << command << endl;
      system(command.c_str());
      command = UNARCHIVECOMMAND;
      unpackModDirectories(sourceDir);
      cleanup(sourceDir);
      system("Pause");
     }
     system("rd /s /q temp"); 
     system("mkdir temp");
    }
    system("PAUSE");
    return EXIT_SUCCESS;
}

void unpackModDirectories(string sourceDir)
{
     string command = ROBOCOPY;
     string buffer;
     system("dir /b /a:d skyrim > dirList.temp");
     ifstream dirList("dirList.temp");
     while(getline(dirList, buffer))
    {
     for(int i = 0; buffer[i] != '\0'; i++)
     { 
        buffer[i] = tolower(buffer[i]); 
     } 
     if (buffer != dataDir && buffer != "textures" && buffer != "meshes" && buffer != "video" && buffer != "strings" && buffer != "scripts" && buffer != "sound" && buffer != "interface")
     {
      command+= sourceDir + "\\" + buffer + "\" " + sourceDir;
      cout << command << endl;
      system(command.c_str());
      command = ROBOCOPY;
     }
    }
    
    dirList.close();
    system("del /q dirList.temp");
}
void cleanup(string sourceDir)
{
     string command = ROBOCOPY;
     string buffer;
     system("dir /b /a:d skyrim > dirList.temp");
     ifstream dirList("dirList.temp");
     while(getline(dirList, buffer))
    {
     if (buffer != dataDir)
     {
      command+= sourceDir + "\\" + buffer + "\" " + sourceDir + "\\" + dataDir + "\\" + buffer + "\"";
      cout << command << endl;
      system(command.c_str());
      command = ROBOCOPY;
     }
    }
    command = "move /y " + sourceDir + "\\" + "*.esp " + sourceDir + "\\" + dataDir;
    cout << command << endl;
    system(command.c_str());
    command = "move /y " + sourceDir + "\\" + "*.bsa " + sourceDir + "\\" + dataDir;
    cout << command << endl;
    system(command.c_str());
    command = "move /y " + sourceDir + "\\" + "*.bsl " + sourceDir + "\\" + dataDir;
    cout << command << endl;
    system(command.c_str());
    dirList.close();
    system("del /q dirList.temp");
}

void specialOps(ifstream& config, string buffer)
{/*
 string command = UNARCHIVECOMMAND + buffer + UNARCHIVEFLAGS + "temp";
 system(command.c_str());
 while(getline(config, buffer))
 {
    if (buffer == "!")
    {
     return;
     }
     else
     {
         if (buffer.find(" & ") == buffer.npos || buffer.find(" & ") == buffer.npos)
         {
         }
      }
    }
   }
*/}
