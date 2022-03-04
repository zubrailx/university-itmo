#include<stdio.h>
#include<inttypes.h>
#include<stdlib.h>
#include<string.h>
#include"../include/structure.h"

#define offsetof(type, element) ((size_t)&(((type *)0)->element))

enum return_status {
	OK = 0,
	FILE_NOT_OPEN,
	INVALID_SIGNATURE
};

int main() {
	FILE* file = fopen("data/input.exe", "rb");
	if (file == NULL) { return FILE_NOT_OPEN; }
  // read doc and set file pointer to optional header
	IMAGE_DOS_HEADER image_doc_header;
	fread(&image_doc_header, sizeof(IMAGE_DOS_HEADER), 1, file);
	if (image_doc_header.e_magic != 0x5a4d) { return INVALID_SIGNATURE; }
	fseek(file, image_doc_header.e_lfanew, SEEK_SET);
	// read image_nt_header
	NT_HEADERS nt_headers;
	fread(&nt_headers, sizeof(NT_HEADERS), 1, file);
	// set pointer to sections
	fseek(file, image_doc_header.e_lfanew + offsetof(NT_HEADERS, OptionalHeader) +
			nt_headers.FileHeader.SizeOfOptionalHeader, SEEK_SET);
	// read sections
	uint16_t section_table_size = nt_headers.FileHeader.NumberOfSections;
	SECTION_TABLE *section_table_arr = malloc(sizeof(SECTION_TABLE) * section_table_size);
	for (uint16_t i = 0; i < section_table_size; ++i) {
		fread(section_table_arr + i, sizeof(SECTION_TABLE), 1, file);
	}
	
	// write section binary to files
  char* name = "output/sections/";
  char *filename = malloc(strlen(name) + 8 + 4); // +4 = .bin
	for (uint16_t i = 0; i < section_table_size; ++i) {
		uint32_t section_size = section_table_arr[i].SizeOfRawData;
		uint32_t section_physical_offset = section_table_arr[i].PointerToRawData; 
		void *data = malloc(section_size);
    strcpy(filename, name);
    strcat(filename, (char*) section_table_arr[i].Name);
    strcat(filename, ".bin");
		FILE *fi = fopen(filename, "wb");

		fseek(file, section_physical_offset, SEEK_SET);
		fread(data, section_size, 1, file);
		fwrite(data, section_size, 1, fi);

		fclose(fi);
		free(data);
	}
  free(filename);

	// print section info and entry address
	FILE* file_oi = fopen("output/output.txt", "w");
	fprintf(file_oi, "AddressOfEntryPoint = ""%"PRIu32 "\n\n", nt_headers.OptionalHeader.StandardFields.AddressOfEntryPoint);
	for (uint16_t i = 0; i < section_table_size; ++i) {
		fprintf(file_oi, "Section: %.8s\n", (char*)section_table_arr[i].Name);
		fprintf(file_oi, "\t(PhysicalAddress)|(VirtualSize) = %"PRIu32 "\n", section_table_arr[i].Misc.PhysicalAddress);
		fprintf(file_oi, "\tVirtualAddress                  = %"PRIu32 "\n", section_table_arr[i].VirtualAddress);
		fprintf(file_oi, "\tSizeOfRawData                   = %"PRIu32 "\n", section_table_arr[i].SizeOfRawData);
		fprintf(file_oi, "\tPointerToRowData                = %"PRIu32 "\n", section_table_arr[i].PointerToRawData);
		fprintf(file_oi, "\tPointerToRelocations            = %"PRIu32 "\n", section_table_arr[i].PointerToRelocations);
		fprintf(file_oi, "\tPointerToLinenumbers            = %"PRIu32 "\n", section_table_arr[i].PointerToLinenumbers);
		fprintf(file_oi, "\tNumberOfRelocations             = %"PRIu16 "\n", section_table_arr[i].NumberOfRelocations);
		fprintf(file_oi, "\tNumberOfLinenumbers             = %"PRIu16 "\n", section_table_arr[i].NumberOfLinenumbers);
		fprintf(file_oi, "\tCharacteristics                 = %x" "\n", section_table_arr[i].Characteristics);     
	}

	fclose(file_oi);
	fclose(file);
  return 0;
}

