#include <iostream>
#include <fstream>
#include <sstream>

#include "denseofConfigParser.h"

using namespace std;

static int lineNr = 0;
static char line[512];

static void lineWarning(const string &msg = "")
{
    cerr << "Warning on line " << lineNr << ": ";
}

static void lineError(const string &msg = "")
{
    cerr << "Error on line " << lineNr << ": ";
}

denseofConfigParser::denseofConfigParser(const char *filename) :
        ConfigParser(filename)
{
}

denseofConfigParser::denseofConfigParser(const string &filename) :
        ConfigParser(filename)
{
}

int denseofConfigParser::read()
{
    std::ifstream configFile(filename);
    if (!configFile.is_open())
    {
        return -1;
    }

    char line[256];
    int k;

    lineNr = 0;

    while (configFile.getline(line, sizeof(line) - 1))
    {
        string lineStr(line), word;
        stringstream ss(lineStr, ios_base::in);
        bool valueToCome;

        valueToCome = false;
        lineNr++;

        while (ss >> word)
        {
            if (word[0] == '#')
                break;;
            if (valueToCome)
            {
                values.push_back(word);
                valueToCome = false;
//                parseAttrToStruct(attributes[attributes.size() - 1], word);
                continue;
            }

            if ((k = isValidSection(word)) != -1)
            {
                if (isFlagSect(k))
                {
                    lineError();
                    cerr << "Duplicated section: " << word << "\n";
                    exit(1);
                }
                sections.push_back(word);
                sectFlag |= (1 << k);
            }
            else if ((k = isValidAttr(word)) != -1)
            {
//                if (isFlagAttr(k))
//                {
//                    lineError();
//                    cerr << "Duplicated attribute: " << word << "\n";
//                    exit(1);
//                }
                attributes.push_back(word);
                attrFlag |= (1 << k);
            }
            else if (word == "=")
                valueToCome = true;
            else
            {
                lineError();
                cerr << "Broken config file!\n";
//                exit(1);
            }
        }

        if (valueToCome)
        {
            lineError();
            cerr << "Missing value for attribute ";
            cerr << "'" << attributes[attributes.size() - 1] << "' !\n";
            values.push_back("");
        }
    }

    return 0;
}

int denseofConfigParser::readToStruct(ConfigData &config)
{
    std::ifstream configFile(filename);
    if (!configFile.is_open())
    {
        return -1;
    }

    int k;

    lineNr = 0;

    while (configFile.getline(line, sizeof(line) - 1))
    {
        string lineStr(line), word, lw;
        stringstream ss(lineStr, ios_base::in);
        bool valueToCome;
        int cntWords = 0;

        valueToCome = false;
        lineNr++;

        while (ss >> word)
        {
            cntWords++;
            if (word[0] == '#')
                break;
            if (cntWords == 3 && valueToCome)
            {
                valueToCome = false;
                parseAttrToStruct(lw, word, config);
                continue;
            }

            if (cntWords == 1 && (k = isValidSection(word)) != -1)
            {
                if (isFlagSect(k))
                {
                    lineError();
                    cerr << "Duplicated section: " << word << "\n";
                    exit(1);
                }
                sections.push_back(word);
                sectFlag |= (1 << k);
            }
            else if (cntWords == 1 && (k = isValidAttr(word)) != -1)
            {
//                if (isFlagAttr(k))
//                {
//                    lineError();
//                    cerr << "Duplicated attribute: " << word << "\n";
//                    exit(1);
//                }
                attrFlag |= (1 << k);
                lw = word;
            }
            else if (cntWords == 2 && word == "=")
                valueToCome = true;
            else
            {
                lineError();
                cerr << "Broken config file!\n";
                //                exit(1);
            }
        }

        if (valueToCome)
        {
            lineWarning();
            cerr << "Missing value for attribute ";
            cerr << "'" << lw << "'";
            cerr << "\n";
            parseAttrToStruct(lw, word, config);
//            values.push_back("");
        }
    }

    return 0;
}

bool denseofConfigParser::isFlagAttr(int k)
{
    return (attrFlag & (1 << k)) != 0;
}

bool denseofConfigParser::isFlagSect(int k)
{
    return (sectFlag & (1 << k)) != 0;
}

void denseofConfigParser::parseAttrToStruct(const string &attr,
        const string &val, ConfigData &config)
{
    denseofConfigData &cfg = static_cast<denseofConfigData&>(config);
    if (attr == "width")
    {
        cfg.width = atoi(val.c_str());
    }
    else if (attr == "height")
    {
        cfg.height = atoi(val.c_str());
    }
    else if (attr == "search_mode")
    {
        cfg.search_mode = (val == "raster") ? 1 : 0;
    }
    else if (attr == "output_format")
    {
        cfg.output_format = (val == "xy") ? 0 : 1;
    }
    else if (attr == "half_pixel")
    {
        cfg.half_pixel = atoi(val.c_str());
    }
    else if (attr == "quarter_pixel")
    {
        cfg.quarter_pixel = atoi(val.c_str());
    }
    else if (attr == "src_frame")
    {
        cfg.src_fn.push_back(val);
    }
    else if (attr == "ref_frame")
    {
        cfg.ref_fn.push_back(val);
    }
    else if (attr == "coords_file")
    {
        cfg.coords_fn.push_back(val);
    }
    else if (attr == "raster_step")
    {
        cfg.raster_step = atoi(val.c_str());
    }

}
