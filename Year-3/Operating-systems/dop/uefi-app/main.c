#include <efi.h>
#include <efilib.h>

EFI_STATUS
efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
  InitializeLib(ImageHandle, SystemTable);

  UINTN MapSize, MapKey;
  UINTN DescriptorSize;
  UINT32 DescriptorVersion;
  EFI_MEMORY_DESCRIPTOR* Map = NULL;

  SystemTable->BootServices->GetMemoryMap(&MapSize, Map, &MapKey, &DescriptorSize, &DescriptorVersion);
  SystemTable->BootServices->AllocatePool(EfiLoaderData, MapSize, (void **)&Map);
  SystemTable->BootServices->GetMemoryMap(&MapSize, Map, &MapKey, &DescriptorSize, &DescriptorVersion);


  UINTN MmapEntries = MapSize / DescriptorSize;
  for (UINTN i = 0; i < MmapEntries; ++i) {
    EFI_MEMORY_DESCRIPTOR *Desc = (EFI_MEMORY_DESCRIPTOR *)((UINTN) Map + (i * DescriptorSize));
    Print(L"Type=%d, ", Desc->Type);
    Print(L"Attr=%d, ", Desc->Attribute);
    Print(L"Size=%d, ", Desc->NumberOfPages * EFI_PAGE_SIZE);
    Print(L"PhysStart=%d, ", Desc->PhysicalStart);
    Print(L"VirtStart=%d, ", Desc->VirtualStart);
    Print(L"Pad=%d\n", Desc->Pad);
  }
  SystemTable->BootServices->FreePool((void *)Map);
  return EFI_SUCCESS;
}

