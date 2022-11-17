#include <efi.h>
#include <efilib.h>

EFI_STATUS
efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
  InitializeLib(ImageHandle, SystemTable);

  UINTN MapSize, MapKey;
  UINTN DescriptorSize;
  UINT32 DescriptorVersion;
  EFI_MEMORY_DESCRIPTOR *Map = NULL;

  SystemTable->BootServices->GetMemoryMap(&MapSize, Map, &MapKey,
                                          &DescriptorSize, &DescriptorVersion);
  SystemTable->BootServices->AllocatePool(EfiLoaderData, MapSize,
                                          (void **)&Map);
  SystemTable->BootServices->GetMemoryMap(&MapSize, Map, &MapKey,
                                          &DescriptorSize, &DescriptorVersion);

  UINTN MmapEntries = MapSize / DescriptorSize;
  for (UINTN i = 0; i < MmapEntries; ++i) {
    EFI_MEMORY_DESCRIPTOR *Desc =
        (EFI_MEMORY_DESCRIPTOR *)((UINTN)Map + (i * DescriptorSize));
    Print(L"Type=%u, ", Desc->Type);
    Print(L"PhysStart=%lu, ", (UINT64)Desc->PhysicalStart);
    Print(L"VirtStart=%lu, ", (UINT64)Desc->VirtualStart);
    Print(L"Size=%lu, ", (UINT64)Desc->NumberOfPages * EFI_PAGE_SIZE);
    Print(L"Attr=%lX\n", (UINT64)Desc->Attribute);
  }
  SystemTable->BootServices->FreePool((void *)Map);
  return EFI_SUCCESS;
}
