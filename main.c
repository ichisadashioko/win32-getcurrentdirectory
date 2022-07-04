#include <windows.h>

#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>

int main()
{
  char* path_string_data_with_null_termiated;
  DWORD dword_result = 0;
  dword_result       = GetCurrentDirectoryA(0, NULL);

  if (dword_result == 0)
  {
    fprintf(stderr, "GetCurrentDirectoryA failed at %s:%d\n", __FILE__, __LINE__);
    // TODO GetLastError
    return 1;
  }

  DWORD number_of_bytes_to_allocate    = dword_result;
  path_string_data_with_null_termiated = (char*)malloc(number_of_bytes_to_allocate);
  if (path_string_data_with_null_termiated == 0)
  {
    fprintf(stderr, "malloc failed at %s:%d\n", __FILE__, __LINE__);
    return 1;
  }

  dword_result = GetCurrentDirectoryA(number_of_bytes_to_allocate, path_string_data_with_null_termiated);

  if (dword_result == 0)
  {
    fprintf(stderr, "GetCurrentDirectoryA failed at %s:%d\n", __FILE__, __LINE__);
    // TODO GetLastError
    return 1;
  }

  printf("%d bytes\n", number_of_bytes_to_allocate);

  if (path_string_data_with_null_termiated[number_of_bytes_to_allocate - 1] != 0)
  {
    fprintf(stderr, "GetCurrentDirectoryA does not set the last char to 0 %s:%d\n", __FILE__, __LINE__);

    for (DWORD i = 0; i < number_of_bytes_to_allocate; i++)
    {
      putc(path_string_data_with_null_termiated, stdout);
    }

    free(path_string_data_with_null_termiated);

    printf("\n");
    return 1;
  }

  printf("%s\n", path_string_data_with_null_termiated);

  // print hex values

  for (DWORD i = 0; i < number_of_bytes_to_allocate; i++)
  {
    printf("%02x ", path_string_data_with_null_termiated[i]);
  }
  printf("\n");
  free(path_string_data_with_null_termiated);
  return 0;
}
