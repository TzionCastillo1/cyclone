# Additional clean files

file(REMOVE_RECURSE
  "bootloader/bootloader.bin"
  "bootloader/bootloader.elf"
  "bootloader/bootloader.map"
  "config/sdkconfig.cmake"
  "config/sdkconfig.h"
  "encryption_keys.bin.S"
  "esp-idf/esptool_py/flasher_args.json.in"
  "esp-idf/mbedtls/x509_crt_bundle"
  "flash_app_args"
  "flash_bootloader_args"
  "flash_project_args"
  "flasher_args.json"
  "ldgen_libraries"
  "ldgen_libraries.in"
  "partition_encrypted.bin.S"
  "project_elf_src_esp32s3.c"
  "sample.bin.S"
  "unit-test-app.bin"
  "unit-test-app.map"
  "x509_crt_bundle.S"
)
