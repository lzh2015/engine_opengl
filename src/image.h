#ifndef IMAGE_H_
#define IMAGE_H_

#include "global.h"

class ImageTool
{
public:
	static ImageTool& GetInstance();
	BYTE* LoadImage(const string & a_sPath, int & width, int & height, int & bpp);

private:
	ImageTool();
	~ImageTool();
	ImageTool(ImageTool const&);  // Don't Implement
	void operator=(ImageTool const&); // Don't implement
	void loadHeightMap(const string & filename,unsigned char ** str); // only for debug
};

inline void ImageTool::loadHeightMap(const string & filename,
		unsigned char ** str) {
	ifstream in_file(filename.c_str(), ios::binary);

	//if (in_file == NULL) {
	//	cout << "generate heightmap fails" << endl;
	//	return;
	//}

	in_file.seekg(0, ios_base::end);
	int size = (int) in_file.tellg();
	in_file.seekg(0, ios_base::beg);

	*str = new unsigned char[size];

	in_file.read((char *) *str, size);

	in_file.close();
}


#endif /* IMAGE_H_ */
