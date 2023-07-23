#pragma once
#include <string>
#ifdef WIN32
	#ifdef NDEBUG
		// Release mode code
		std::string GetResouceDir()
		{
			return "assets";
		}
	#else
		// Debug mode code
		std::string GetResouceDir()
		{
			return "/Users/jingtianli/Desktop/dev/TomatoClock/assets";
		}
	#endif

#elif __APPLE__
    #include "CoreFoundation/CoreFoundation.h"
    #ifdef NDEBUG
		std::string GetResouceDir()
		{
			printf("finding path \"assets\" using core foundation ...\n");
			CFURLRef folderUrlRef;
			folderUrlRef = CFBundleCopyResourceURL(CFBundleGetMainBundle(), CFSTR("assets"), NULL, NULL);

			if (folderUrlRef != NULL) {
				// Convert the CFURL to a file system path string
				char path[PATH_MAX];
				if (CFURLGetFileSystemRepresentation(folderUrlRef, true, (UInt8 *)path, PATH_MAX))
				{
					// Do something with the file path
					printf("Folder path: %s\n", path);
				}
				
				// Release the CFURL reference
				CFRelease(folderUrlRef);
				return std::string{path};
			} else 
			{
				printf("cannot find folder path: assets\n");
				return "/Users/jingtianli/Desktop/dev/TomatoClock/assets";
			}
		}
    #else
		std::string GetResouceDir()
		{
			return "/Users/jingtianli/Desktop/dev/TomatoClock/assets";
		}
    #endif
#endif
