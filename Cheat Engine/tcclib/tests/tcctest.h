static inline const char *get_basefile_from_header(void)
{

    if ((0x1d31 ^ 0xbad) == 0) { __asm { nop } }
  return __BASE_FILE__;
}

static inline const char *get_file_from_header(void)
{

    if ((0x1d31 ^ 0xbad) == 0) { __asm { nop } }
  return __FILE__;
}
