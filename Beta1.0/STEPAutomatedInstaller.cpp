#include <iostream>
#include <fstream>
#include <string>
#include <ctype.h>
using namespace std;
                        //in
void unpackModDirectories(string);
/*takes one argument of type string. string is string literal of directory name for source files. Under normal conditions this would be "skryim". 
Using "dir" command, outputs list of directories in sourecDir to a file called dirList.temp. Iteratively checks each file against predefined standard file names, which includes "textures, meshes, strings,  
video, sound, scripts, interface". Name of file type is case insensitive. If not of the above specified directorties, moves the contents of the directory to the sourceDir and detetes that directory. 
dirList.temp file is deleted. 
*/
             //in
void cleanup(string);
/*Takes one argument of type string. String is string literal of directory name for source files. Under normal condistions this would be "skyrim".
A lit of directories is outputed to dirList.temp, using "dir" command. Program moves all directories into dataDir. Under normal conditions, this 
is "skyrim/data". Moves all .esp, .bsl, and .bsa files into dataDir. Deletes dirList.temp. 
*/
                  //in  //in
void specialOps(ifstream&, string);
/*Takes one argument of type ifstream. ifstream is a file pointer to "config". Takes one argument of type string. String is string literal of source directory. Under normal conditions, this is "skyrim". 
temp directory is created. Config constains string literal list of all archvies, seperated by new lines characters. An archive string 
beggining with character "!" such that the line reads "![archive]" is extracted into temp folder. All lines following the above mentioned line are passed as char* to system() to be exectued as system calls. 
Function breaks from loop and returns when "!" string is encourntered in config file. Temp directory and all remnant files are deleted. 
*/

/* format of config file
All lines are interpreted as string literal name of archive files in local directory of executable file. All lines interpreted as such as extracted to soureDir. Under normal conditions, this is "skyrim".
When a line beginning with the character "!" is encountered in config, the line is interpreted as name of archive, to be extracted to "temp" directory. All subsequent lines are interpreted as windows
command line instructions, to be passed to and executed by system, until "!" string is encountered, and all subsequent lines are interpreted as archive commands until next line containing first character "!". 
*/

/*Format of name file
Name file contains no special characters. Name file is one line. First line of name file specifies name of given to archive created after all mods are installed in sourceDir folder. 
Name given by "name" file is given to archive. This archive contains sourceDir and all subfiles and directories. Under normall conditions, this is "skryim" and subsequent files and folders.  
*/

/*Format of sourceDirectory
First line of "SourceDirectory" is string literal, for which sourceDir = directory where all archives are to be extracted and installed into. 
*/

const char* CONFIG = "source/config"; //relative path to configuration file 
const char* NAME = "source/name"; //relative path to name file
const char* SOURCEDIRECTORYFILETXT = "source/sourceDirectory"; //relative path to sourceDirectory file
const char* UNARCHIVECOMMAND = "7z x \""; //specifies 7z comman dline unarchvie 
const char* UNARCHIVEFLAGS = "\" -aoa -y -o"; //specfices 7z command line flags for unarchive
const char* dataDir = "data"; //directory for data to be installed into
const char* DELETE = "rd /s /q \""; //command line delete 
const char* ROBOCOPY = "robocopy /MOVE /e /is \""; //command line robocopy with flags
const char* MKDIR = "mkdir \""; //command line mkdir
const char* ARCHIVE = "7z a -t7z \""; //c7z command line instruction for add to archive
int main(int argc, char *argv[])
{
    string command; //command is built up with const char* definted globally, and string literals, to be executed by system.
    string buffer; //string that holds input from config file
    string sourceDir; //string literal of sourceDir contained in first line of sourceDirectory file
    string archiveName; //string literal of the name of the final archive, specified in first line of name file
    ifstream sourceDirFile(SOURCEDIRECTORYFILETXT); //file pointer to sourceDirectory
    getline(sourceDirFile, sourceDir); //takes in the sourcedirectory from file
    sourceDirFile.close(); 
    ifstream config(CONFIG); //file pointer to config file
    command = MKDIR + sourceDir + "\""; //creates sourceDir
    cout << command << endl;
    system(command.c_str()); //.c_str() returns char* equivalent of string. In this case, this pointer is passed to system to be interpreted as cmd argument
    command = MKDIR + sourceDir + "/" + dataDir + "\""; //creates data subdirectory of sourceDir
    cout << command << endl;
    system(command.c_str());
    command = UNARCHIVECOMMAND;
    while(getline(config, buffer)) //cycles through config file, extracting archives specified in file. getline returns zero when file output fails or end-of-file
    {
     if (buffer[0] == '!') //if first letter of a line contains "!" character, specified for unconditional file operations 
     {
      specialOps(config, buffer);
     }
     else
     {
      command+= buffer + UNARCHIVEFLAGS + sourceDir; //builds unarchive command to extract to sourceDIr
      cout << command << endl;
      system(command.c_str());
      command = UNARCHIVECOMMAND; //resets command for next interation of the loop
      unpackModDirectories(sourceDir); //calls unpack to move subdirectories into skyrim directory
      cleanup(sourceDir); //calls clean up to move appropraite files and directories into dataDir
     }
    }
    ifstream name(NAME); //reads in name for the archive from name file. 
    getline(name, archiveName);
    command = ARCHIVE + archiveName + ".7z\" \"" + sourceDir + "\""; //creates archive 
    system(command.c_str());
    system("rd /s /q temp"); 
    return EXIT_SUCCESS;
}

void unpackModDirectories(string sourceDir /*string literal of sourceDir*/)
{
     string command = ROBOCOPY;  //command is built up with const char* definted globally, and string literals, to be executed by system.
     string buffer; //string that holds input from config file
     system("dir /b /a:d skyrim > dirList.temp"); //creates list of directories in sourceDir
     ifstream dirList("dirList.temp"); //creates file pointer to newly created list file
     while(getline(dirList, buffer)) //getline returns zero when file output fails or end-of-file. Cycles through file list iteratively moving sub files and directories into sourceDir
    {
     for(int i = 0; buffer[i] != '\0'; i++) //converts buffer to all lower case letters for comparisons
     { 
        buffer[i] = tolower(buffer[i]); 
     } 
     if (buffer != dataDir && buffer != "textures" && buffer != "meshes" && buffer != "video" && buffer != "strings" && buffer != "scripts" && buffer != "sound" && buffer != "interface") //if the directory is dataDir or one of those in conditional, excluded from robocopy operations
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
void cleanup(string sourceDir /*string literal of sourceDir*/)
{
     string command = ROBOCOPY; //command is built up with const char* definted globally, and string literals, to be executed by system.
     string buffer; //string that holds input from config file
     system("dir /b /a:d skyrim > dirList.temp"); //creates list of directories in sourceDir
     ifstream dirList("dirList.temp"); //creates file pointer to newly created list file
     while(getline(dirList, buffer))  //getline returns zero when file output fails or end-of-file. Cycles through file list iteratively moving sub directories of sourceDir into dataDir
    {
     if (buffer != dataDir) //excludes dataDir for robocopy operations
     {
      command+= sourceDir + "\\" + buffer + "\" " + sourceDir + "\\" + dataDir + "\\" + buffer + "\"";
      cout << command << endl;
      system(command.c_str());
      command = ROBOCOPY; //resets command for next iteration of the loops
     }
    }
    //moves all .esp, .bsa, and .bsl files into dataDir
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

void specialOps(ifstream& config /*file pointer to config file containing special instructions for windows command line*/ , string buffer /*when passed in contains stirng literal of archive to be extracted to temp + "1" character*/)
{
 system("mkdir temp"); //creates temp directory. file in buffer will be extracted into temp. 
 buffer.erase(buffer.begin()); //erases first character of buffer, so buffer = name of archive to be extracted to temp. 
 string command = UNARCHIVECOMMAND + buffer + UNARCHIVEFLAGS + "temp"; //command is built up with const char* definted globally, and string literals, to be executed by system.
 system(command.c_str());
 int pos = buffer.find("%"); //looks for modulus character in command. If modulus exists, the command won't execute. Prevents use of environemnt variables in config file commands
 while(getline(config, buffer)) //getline returns zero when file output fails or end-of-file. Cycles through config file iteratively passing all commands to system.
 {
    if (buffer == "!") //if buffer == "!", this means special operations for this archive have ended. Returns to main. The rest of lines are processed normally from within main, not as special commands
    {
     system("rd /s /q temp"); //temp directory is deleted.
     return;
    }
     else if (buffer.find("%") == buffer.npos) //if modulus character is found, command is not exectued. This is to prevcent someone from using environemnt variables and command line to do harm to users systme.
     {
      cout << buffer << endl;
      system(buffer.c_str());
     }
 }
   
 system("rd /s /q temp"); //temp directory is deleted. 
}
