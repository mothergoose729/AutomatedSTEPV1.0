#include <iostream>
#include <fstream>
#include <string>
using namespace std;

void unpackModDirectories(string);
void cleanup(string);

const char* CONFIG = "source/config";
const char* ARCHIVENAME = "source/name";
const char* SOURCEDIRECTORYFILETXT = "source/sourceDirectory";
const char* UNARCHIVECOMMAND = "7z x \"";
const char* UNARCHIVEFLAGS = "\" -aoa -y -o";
const char* dataDir = "Data";
const char* XCOPYCOMMAND = "xcopy /f /s /y \"";
const char* DELETE = "rd /s /q \"";
int main(int argc, char *argv[])
{
    string command = UNARCHIVECOMMAND;
    string buffer;
    string sourceDir;
    ifstream sourceDirFile(SOURCEDIRECTORYFILETXT);
    getline(sourceDirFile, sourceDir);
    sourceDirFile.close();
    ifstream config(CONFIG);
    while(getline(config, buffer))
    {
     command+= buffer + UNARCHIVEFLAGS + sourceDir;
     cout << command << endl;
     system(command.c_str());
     command = UNARCHIVECOMMAND;
    }
    cleanup(sourceDir);
    unpackModDirectories(sourceDir);
    cleanup(sourceDir);
    system("PAUSE");
    return EXIT_SUCCESS;
}

void unpackModDirectories(string sourceDir)
{
     string command = XCOPYCOMMAND;
     string buffer;
     system("dir /b /a:d skyrim > dirList.temp");
     ifstream dirList("dirList.temp");
     
     while(getline(dirList, buffer))
    {
     if (buffer != dataDir && buffer != "textures" && buffer != "meshes" && buffer != "video" && buffer != "strings" && buffer != "scripts" && buffer != "sound")
     {
      command+= sourceDir + "\\" + buffer + "\\*\" " + sourceDir;
      cout << command << endl;
      system(command.c_str());
      command = DELETE + sourceDir + "\\" + buffer + "\"";
      cout << command << endl;
      system(command.c_str());
      command = XCOPYCOMMAND;
     }
    }
    
    dirList.close();
    system("del /q dirList.temp");
}
void cleanup(string sourceDir)
{
     string command;
     command = "mkdir " + sourceDir + "\\" + dataDir;
     cout << command << endl;
     system(command.c_str());
     command = "move /y " + sourceDir + "\\" + "textures " + sourceDir + "\\" + dataDir;
     cout << command << endl;
     system(command.c_str());
     command = "move /y " + sourceDir + "\\" + "meshes " + sourceDir + "\\" + dataDir;
     cout << command << endl;
     system(command.c_str());
     command = "move /y " + sourceDir + "\\" + "video " + sourceDir + "\\" + dataDir;
     cout << command << endl;
     system(command.c_str());
     command = "move /y " + sourceDir + "\\" + "strings " + sourceDir + "\\" + dataDir;
     cout << command << endl;
     system(command.c_str());
     command = "move /y " + sourceDir + "\\" + "interface " + sourceDir + "\\" + dataDir;
     cout << command << endl;
     system(command.c_str());
     command = "move /y " + sourceDir + "\\" + "scripts " + sourceDir + "\\" + dataDir;
     cout << command << endl;
     system(command.c_str());
     command = "move /y " + sourceDir + "\\" + "sound " + sourceDir + "\\" + dataDir;
     cout << command << endl;
     system(command.c_str());
     command = "move /y " + sourceDir + "\\" + "*.esp " + sourceDir + "\\" + dataDir;
     cout << command << endl;
     system(command.c_str());
     command = "move /y " + sourceDir + "\\" + "*.bsa " + sourceDir + "\\" + dataDir;
     cout << command << endl;
     system(command.c_str());
}
