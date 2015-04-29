#include "image.h"
#include <FreeImage.h>

#pragma comment(lib, "FreeImage.lib")

ImageTool& ImageTool::GetInstance() {
	static ImageTool instance; // Guaranteed to be destroyed.
									 // Instantiated on first use.
	return instance;
}

ImageTool::ImageTool()
{

}

ImageTool::~ImageTool()
{

}

BYTE* ImageTool::LoadImage(const string & a_sPath, int & width, int & height, int & bpp)
{
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	FIBITMAP* dib(0);

	fif = FreeImage_GetFileType(a_sPath.c_str(), 0); // Check the file signature and deduce its format

	if(fif == FIF_UNKNOWN) // If still unknown, try to guess the file format from the file extension
		fif = FreeImage_GetFIFFromFilename(a_sPath.c_str());

	if(fif == FIF_UNKNOWN) // If still unknown, return failure
		return false;

	if(FreeImage_FIFSupportsReading(fif)) // Check if the plugin has reading capabilities and load the file
		dib = FreeImage_Load(fif, a_sPath.c_str());
	if(!dib)
		return false;

	width = FreeImage_GetWidth(dib);
	height = FreeImage_GetHeight(dib);
	bpp = FreeImage_GetBPP(dib);

	//change from upside down to downside up
	FreeImage_FlipVertical(dib);

	BYTE* bDataPointer = FreeImage_GetBits(dib); // Retrieve the image data

	BYTE * img = (BYTE*)malloc(width * height * bpp / 8);
	memcpy(img, bDataPointer, width * height * bpp / 8);

	// If somehow one of these failed (they shouldn't), return failure
	if(bDataPointer == NULL || FreeImage_GetWidth(dib) == 0 || FreeImage_GetHeight(dib) == 0)
		return false;

	FreeImage_Unload(dib);

	return img;
}
