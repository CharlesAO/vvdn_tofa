#ifndef DENSEOF_CONFIG_PARSER_H
#define DENSEOF_CONFIG_PARSER_H

#include <stdint.h>
#include <vector>
#include <string>

#include "ConfigParser.h"

struct denseofConfigData: ConfigData
{
    uint32_t width, height;
    uint16_t search_mode :1, output_format :1;
    uint16_t half_pixel :1, quarter_pixel :1;
    uint32_t total_num_coords :18, block_num_coords :18;
    std::vector<std::string> ref_fn, src_fn, coords_fn;
    uint32_t raster_step;

    void clear()
    {
        width = height = 0;
        search_mode = output_format = 0;
        half_pixel = quarter_pixel = 0;
        total_num_coords = block_num_coords = 0;
    }
};

class denseofConfigParser: public ConfigParser
{
private:
    bool isFlagAttr(int k);
    bool isFlagSect(int k);
    void parseAttrToStruct(const std::string &attr, const std::string &val,
            ConfigData &config);
public:
    denseofConfigParser(const char *filename);
    denseofConfigParser(const std::string &filename);
    int read();
    int readToStruct(ConfigData &config);
};

#endif //DENSEOF_CONFIG_PARSER_H
