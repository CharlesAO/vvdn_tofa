#include <cv.h>
#include <debug.h>
#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;

void displayMatAttr(cv::Mat* image)
{
	cout << setw(15) << "Width:" <<  image->cols << endl;
	cout << setw(15) << "Height:" <<  image->rows << endl;

	cout << setw(15) << "Pixel Depth:" <<  image->depth() << endl;
	cout << setw(15) << "Channels:" <<  image->channels() << endl;

	cout << setw(15) << "Width Step:" <<  image->step1() << endl;
	cout << setw(15) << "Element Size:" <<  image->elemSize() << endl;
	cout << setw(15) << "Image Size:" <<  image->size() << endl;
}

/* move to TestRunner
void saveFile(std::string fileName, char* buffer, unsigned int bytes)
{
	std::ofstream f;

	f.open(fileName.c_str(), std::ios::out | std::ios::binary);
	f.write(buffer, bytes);
	f.close();
}
*/
