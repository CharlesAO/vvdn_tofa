#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#include <string>
#include <vector>
#include <stdint.h>

struct ConfigData
{

};

class ConfigParser
{
private:

protected:
    std::string filename;
    static const std::string emptyString;
    uint64_t sectFlag, attrFlag;
    std::vector<std::string> valid_sections, sections;
    std::vector<std::string> valid_attributes, attributes, values;

    int isAttrIdx(const std::string &attr);
    int isValidSection(const std::string &attr);
    int isValidAttr(const std::string &attr);

public:
    ConfigParser(const char *filename);
    ConfigParser(const std::string &filename);
    virtual ~ConfigParser();
    virtual int read() = 0;
    virtual int readToStruct(ConfigData &config) = 0;
    void clear();
    bool isAttr(const std::string &attr);
    const std::string& getAttr(const std::string &attr);
    void addValidAttribute(const std::string &attr);
    void addValidSection(const std::string &sec);
};

#endif //CONFIG_PARSER_H
