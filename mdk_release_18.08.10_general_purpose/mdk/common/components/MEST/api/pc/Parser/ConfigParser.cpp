#include "ConfigParser.h"

using namespace std;

const std::string ConfigParser::emptyString = "";

ConfigParser::ConfigParser(const char *filename)
{
    clear();
    this->filename = filename;
}

ConfigParser::ConfigParser(const string &filename)
{
    clear();
    this->filename = filename;
}

ConfigParser::~ConfigParser()
{

}

void ConfigParser::clear()
{
    sectFlag = attrFlag = 0LL;
    filename.clear();
}

int ConfigParser::isAttrIdx(const string &attr)
{
    for (size_t i = 0; i < attributes.size(); i++)
        if (attributes[i] == attr)
            return i;
    return -1;
}

bool ConfigParser::isAttr(const string &attr)
{
    return (isAttrIdx(attr) != -1);
}

const string& ConfigParser::getAttr(const string &attr)
{
    int idx = isAttrIdx(attr);
    if (idx == -1)
        return emptyString;
    return values[idx];
}

int ConfigParser::isValidAttr(const string &attr)
{
    for (size_t i = 0; i < valid_attributes.size(); i++)
        if (valid_attributes[i] == attr)
            return i;
    return -1;
}

int ConfigParser::isValidSection(const string &attr)
{
    for (size_t i = 0; i < valid_sections.size(); i++)
        if (valid_sections[i] == attr)
            return i;
    return -1;
}

void ConfigParser::addValidAttribute(const std::string &attr)
{
    valid_attributes.push_back(attr);
}

void ConfigParser::addValidSection(const std::string &sec)
{
    valid_sections.push_back(sec);
}
