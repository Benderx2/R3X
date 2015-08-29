#include <system.h>
#ifndef R3X_PACK_H
#define R3X_PACK_H
#define R3X_PACKAGE_HEADER 0xC002B00B
typedef struct {
	uint32_t PackageHeader;
	uint32_t EXEName;
	uint32_t EXEFilePointer;
	uint32_t EXESize;
	uint32_t ObjFileListPointer;
	uint32_t NativeObjectFileListPointer;
	uint16_t ArchiveVersionMajor;
	uint16_t ArchiveVersionMinor;
	uint32_t Checksum;
} r3x_package_header;

typedef struct {
	uint32_t ObjectName; //! Recasted to CHAR*
	uint32_t ObjectFilePointer; 
	uint32_t ObjectFileSize; 
} r3x_package_object_list;

typedef struct {
	FILE* package_file;
	char** ObjectFiles;
	char** NativeFiles;
} package_info;
#endif
