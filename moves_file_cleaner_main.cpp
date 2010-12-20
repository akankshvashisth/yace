

#include <vector>
#include <fstream>
#include <string>
#include <algorithm>

#include <iostream>
void print( const std::string& t )
{
    std::cout << t << std::endl;
}

void searchAndReplace(std::string& value, const std::string& search, const std::string& replace)
{
    std::string::size_type  next;

    for(next = value.find(search);        // Try and find the first match
        next != std::string::npos;        // next is npos if nothing was found
        next = value.find(search,next)    // search for the next match starting after
        // the last match that was found.
        )
    {
        // Inside the loop. So we found a match.
        value.replace(next,search.length(),replace);   // Do the replacement.
        next += replace.length();                      // Move to just after the replace
        // This is the point were we start
        // the next search from. 
    }
}


typedef std::string String;
typedef std::ifstream IfStream;
typedef std::vector<std::string> Strings;

bool does_not_start_with_number_1_dot( const String& s )
{
    if(!s.empty() && s.size() > 2)
    {
        if(s[0] == '1' && s[1] == '.')
        {
            return false;
        }
    }
    return true;
}

bool characters_to_strip( const char& c )
{
    return( c == '-' || 
            //c == 'K' || 
            //c == 'R' || 
            //c == 'Q' || 
            //c == 'N' || 
            //c == 'B' || 
            c == '+' ||
            c == 'x'
          ); 
    return false;
}

void strip_characters( String& s )
{
    s.erase( std::remove_if(s.begin(), s.end(), characters_to_strip), s.end() );
}

struct file_writer
{
    file_writer(std::ofstream& afile):file(afile){}
    void operator()(const String& s)
    {
        file << s << std::endl;
    }
    std::ofstream& file;
};

struct find_and_replacer
{
    find_and_replacer(const std::string& f, const std::string& r):find(f), replace(r){}
    void operator()(std::string& s)
    {
        searchAndReplace(s, find, replace);
    }
    const std::string find;
    const std::string replace;
};

namespace Indices
{
    enum eIndices
    {
        FileName,
        Outfile_0,
        Do_0,
        Outfile_1,
        Do_1,
        Outfile_2
    };
}


int main()
{
    String config_file("moves_file_cleaner_config");
    String line;     
    IfStream conf(config_file);
    Strings configs;

    while(std::getline(conf, line))
    {
        configs.push_back(line);
    }
  
    IfStream file(configs[Indices::FileName]);
    Strings strings;

    while(std::getline(file, line))
    {
        strings.push_back(line);
    }

    strings.erase( std::remove_if(strings.begin(), strings.end(), does_not_start_with_number_1_dot), strings.end() );

    //std::for_each(strings.begin(), strings.end(), print);

    if(configs[Indices::Do_0][0] == 'Y' || configs[Indices::Do_0][0] == 'y')
    {
        std::ofstream out_file(configs[Indices::Outfile_0]);
        std::for_each(strings.begin(), strings.end(), file_writer(out_file));
        out_file.close();
    }

    std::for_each(strings.begin(), strings.end(), find_and_replacer(" 0-1", " L"));
    std::for_each(strings.begin(), strings.end(), find_and_replacer(" 1-0", " W"));
    std::for_each(strings.begin(), strings.end(), find_and_replacer(" ½-½", " D"));

    std::for_each(strings.begin(), strings.end(), strip_characters);

    if(configs[Indices::Do_1][0] == 'Y' || configs[Indices::Do_1][0] == 'y')
    {
        std::ofstream out_file2(configs[Indices::Outfile_1]);
        std::for_each(strings.begin(), strings.end(), file_writer(out_file2));
        out_file2.close();
    }

    std::for_each(strings.begin(), strings.end(), find_and_replacer(".N", "."));
    std::for_each(strings.begin(), strings.end(), find_and_replacer(".B", "."));
    std::for_each(strings.begin(), strings.end(), find_and_replacer(".R", "."));
    std::for_each(strings.begin(), strings.end(), find_and_replacer(".K", "."));
    std::for_each(strings.begin(), strings.end(), find_and_replacer(".Q", "."));
    std::for_each(strings.begin(), strings.end(), find_and_replacer(" N", " "));
    std::for_each(strings.begin(), strings.end(), find_and_replacer(" B", " "));
    std::for_each(strings.begin(), strings.end(), find_and_replacer(" R", " "));
    std::for_each(strings.begin(), strings.end(), find_and_replacer(" K", " "));
    std::for_each(strings.begin(), strings.end(), find_and_replacer(" Q", " "));

    std::for_each(strings.begin(), strings.end(), find_and_replacer(".00 ", ".e1g1 "));
    std::for_each(strings.begin(), strings.end(), find_and_replacer(" 00 ", " e8g8 "));

    std::for_each(strings.begin(), strings.end(), find_and_replacer(".000 ", ".e1c1 "));
    std::for_each(strings.begin(), strings.end(), find_and_replacer(" 000 ", " e8c8 "));

    std::for_each(strings.begin(), strings.end(), find_and_replacer(" L"," -1.0"));
    std::for_each(strings.begin(), strings.end(), find_and_replacer(" W"," +1.0"));
    std::for_each(strings.begin(), strings.end(), find_and_replacer(" D"," +0.0"));

    std::ofstream out_file3(configs[Indices::Outfile_2]);
    std::for_each(strings.begin(), strings.end(), file_writer(out_file3));
    out_file3.close();

    return 0;
}