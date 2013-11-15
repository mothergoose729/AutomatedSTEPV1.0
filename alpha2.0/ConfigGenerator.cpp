#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main(int argc, char *argv[])
{
    string archive;
    system("dir /b /a:-d > config.temp");
    ifstream temp("config.temp");
    ifstream test("config", std::ios::binary);
    if (test)
    {
     cout << "Config already exists. Please remove from directory to generate a new one." << endl;
     system("PAUSE");
     exit(0);
    }
    test.close();
    ofstream config("config");
    while(getline(temp, archive))
    {
     int pos = archive.find(".");
     if (archive != "config" && archive != "config.temp" && archive != "ConfigGenerator.exe" && (archive.find(".zip") != archive.npos  || archive.find(".rar") != archive.npos || archive.find(".7z") != archive.npos))
     {
            config << archive << '\n';
     }
    }
    
    temp.close();
    system("del config.temp /f /q");
}
