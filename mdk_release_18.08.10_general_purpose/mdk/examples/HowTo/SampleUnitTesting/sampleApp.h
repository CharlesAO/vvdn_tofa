
// @brief Increase or decrease with 'value' the first 'pxToChange' values from input line
// @param width multiple of 8
// @param operation: 0 - subtraction, 1 - addition
// @param pxToChange, equal or lower than 'width' and multiple of 8
// @param out is a pointer to the result

void sampleApp (unsigned char *in, unsigned int width, unsigned char value, unsigned char operation, unsigned int pxToChange, unsigned char *out);
void sampleApp_asm (unsigned char *in, unsigned int width, unsigned char value, unsigned char operation, unsigned int pxToChange, unsigned char *out);
