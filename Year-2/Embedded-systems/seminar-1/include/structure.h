#include<inttypes.h>

typedef struct _IMAGE_DOS_HEADER {
	uint16_t  e_magic;      /* 00: MZ Header signature */
	uint16_t  e_cblp;       /* 02: Bytes on last page of file */
	uint16_t  e_cp;         /* 04: Pages in file */
	uint16_t  e_crlc;       /* 06: Relocations */
	uint16_t  e_cparhdr;    /* 08: Size of header in paragraphs */
	uint16_t  e_minalloc;   /* 0a: Minimum extra paragraphs needed */
	uint16_t  e_maxalloc;   /* 0c: Maximum extra paragraphs needed */
	uint16_t  e_ss;         /* 0e: Initial (relative) SS value */
	uint16_t  e_sp;         /* 10: Initial SP value */
	uint16_t  e_csum;       /* 12: Checksum */
	uint16_t  e_ip;         /* 14: Initial IP value */
	uint16_t  e_cs;         /* 16: Initial (relative) CS value */
	uint16_t  e_lfarlc;     /* 18: File address of relocation table */
	uint16_t  e_ovno;       /* 1a: Overlay number */
	uint16_t  e_res[4];     /* 1c: Reserved words */
	uint16_t  e_oemid;      /* 24: OEM identifier (for e_oeminfo) */
	uint16_t  e_oeminfo;    /* 26: OEM information; e_oemid specific */
	uint16_t  e_res2[10];   /* 28: Reserved words */
	uint32_t e_lfanew;     /* 3c: Offset to extended header */
} IMAGE_DOS_HEADER;


typedef struct _FILE_HEADER{
	uint16_t Machine;
	uint16_t NumberOfSections;
	uint32_t TimeDateStamp;
	uint32_t PointerToSymbolTable;
	uint32_t NumberOfSymbolTable;
	uint16_t SizeOfOptionalHeader;
	uint16_t Characteristics;
} FILE_HEADER;


#pragma pack(push, 4)
typedef struct _COFF_FIELDS {
  uint16_t Magic;
  uint8_t  MajorLinkerVersion;
  uint8_t  MinorLinkerVersion;
  uint32_t SizeOfCode;
  uint32_t SizeOfInitializedData;
  uint32_t SizeOfUninitializedData;
  uint32_t AddressOfEntryPoint;
  uint32_t BaseOfCode;
	uint32_t BaseOfData;
} COFF_FIELDS;
#pragma pack(pop)


#pragma pack(push, 4)
typedef struct _WINDOWS_SPECIFIC_FIELDS {
  uint32_t ImageBase;
  uint32_t SectionAlignment;
  uint32_t FileAlignment;
  uint16_t MajorOperatingSystemVersion;
  uint16_t MinorOperatingSystemVersion;
  uint16_t MajorImageVersion;
  uint16_t MinorImageVersion;
  uint16_t MajorSubsystemVersion;
  uint16_t MinorSubsystemVersion;
  uint32_t Win32VersionValue;
  uint32_t SizeOfImage;
  uint32_t SizeOfHeaders;
  uint32_t CheckSum;
  uint16_t Subsystem;
  uint16_t DllCharacteristics;
  uint32_t SizeOfStackReserve;
  uint32_t SizeOfStackCommit;
  uint32_t SizeOfHeapReserve;
  uint32_t SizeOfHeapCommit;
  uint32_t LoaderFlags;
  uint32_t NumberOfRvaAndSizes;
} WINDOWS_SPECIFIC_FIELDS;
#pragma pack(pop)


typedef struct _IMAGE_DATA_DIRECTORY {
	uint32_t VirtualAddress;
	uint32_t Size;
} IMAGE_DATA_DIRECTORY;

typedef struct _DATA_DIRECTORIES {
	IMAGE_DATA_DIRECTORY ExportTable;
	IMAGE_DATA_DIRECTORY ImportTable;
  IMAGE_DATA_DIRECTORY ResourceTable;
	IMAGE_DATA_DIRECTORY ExceptionTable;
	IMAGE_DATA_DIRECTORY CertificateTable;
	IMAGE_DATA_DIRECTORY BaseRelocationTable;
	IMAGE_DATA_DIRECTORY Debug;
	IMAGE_DATA_DIRECTORY Architecture;
	IMAGE_DATA_DIRECTORY GlobalPtr;
	IMAGE_DATA_DIRECTORY TLSTable;
	IMAGE_DATA_DIRECTORY LoadConfigTable;
	IMAGE_DATA_DIRECTORY BoundImport;
	IMAGE_DATA_DIRECTORY ImportAddressTable;
	IMAGE_DATA_DIRECTORY DelayImportDescriptor;
	IMAGE_DATA_DIRECTORY CLRRuntimeHeader;
	uint64_t             ReservedZeros;
} DATA_DIRECTORIES;


typedef struct _SECTION_TABLE {
  uint8_t Name[8];
  union {
    uint32_t PhysicalAddress;
    uint32_t VirtualSize;
  } Misc;
	uint32_t VirtualAddress;
	uint32_t SizeOfRawData;
	uint32_t PointerToRawData;
	uint32_t PointerToRelocations;
	uint32_t PointerToLinenumbers;
	uint16_t NumberOfRelocations;
	uint16_t NumberOfLinenumbers;
	uint32_t Characteristics;     
} SECTION_TABLE;


typedef struct _OPTIONAL_HEADER {
	COFF_FIELDS             StandardFields;
	WINDOWS_SPECIFIC_FIELDS WindowsSpecificFields;
	DATA_DIRECTORIES        DataDirectories;
} OPTIONAL_HEADER;


typedef struct _NT_HEADERS {
	uint32_t        Signature;
	FILE_HEADER     FileHeader;
	OPTIONAL_HEADER OptionalHeader;
} NT_HEADERS;

