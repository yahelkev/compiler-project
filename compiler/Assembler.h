#pragma once

typedef struct fileHeader {
	unsigned short pcType;
	unsigned short numSections;
	unsigned int timedate;
	unsigned int symbolTablePtr;
	unsigned int numOfSymbols;
	unsigned short OpHeaderSize;
	unsigned short flags;
}fileHeader;

typedef struct sectionHeadrer {
	unsigned char name[8];
	unsigned int virtualSize;
	unsigned int virtualAddress;
	unsigned int rawDataSize;
	unsigned int dataAddress;
	unsigned int ptrToRelocation;
	unsigned int lineNumbrsPtr;
	unsigned short numOfRelocations;
	unsigned short numOfLineNums;
	unsigned int flags;
}sectionHeadrer;

typedef struct symbolTableSection {
	unsigned char name[8];
	unsigned int value;
	unsigned short sectionNum;
	unsigned short type;
	unsigned char storageClass;
	unsigned char numOfAuxSymbols;
}symbosTableSection;

typedef struct relocationTableSection {
	unsigned char RVA[8];
	unsigned char symbolTableIndex[8];
	unsigned int type;
}relocationTableSection;