#include <windows.h>

#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>

int main()
{
  WCHAR* path_string_data_with_null_termiated;
  DWORD dword_result = 0;
  dword_result       = GetCurrentDirectoryW(0, NULL);

  if (dword_result == 0)
  {
    fprintf(stderr, "GetCurrentDirectoryW failed at %s:%d\n", __FILE__, __LINE__);
    // TODO GetLastError
    return 1;
  }

  DWORD number_of_characters        = dword_result;
  DWORD number_of_bytes_to_allocate = dword_result * sizeof(WCHAR);
  printf("%d characters\n", number_of_characters);
  printf("%d bytes\n", number_of_bytes_to_allocate);

  path_string_data_with_null_termiated = (WCHAR*)malloc(number_of_bytes_to_allocate);
  if (path_string_data_with_null_termiated == 0)
  {
    fprintf(stderr, "malloc failed at %s:%d\n", __FILE__, __LINE__);
    return 1;
  }

  dword_result = GetCurrentDirectoryW(number_of_characters, path_string_data_with_null_termiated);

  if (dword_result == 0)
  {
    fprintf(stderr, "GetCurrentDirectoryW failed at %s:%d\n", __FILE__, __LINE__);
    // TODO GetLastError
    return 1;
  }

  if (path_string_data_with_null_termiated[number_of_characters - 1] != 0)
  {
    fprintf(stderr, "GetCurrentDirectoryW does not set the last char to 0 %s:%d\n", __FILE__, __LINE__);

    for (DWORD i = 0; i < number_of_bytes_to_allocate; i++)
    {
      putwc(path_string_data_with_null_termiated[i], stdout);
    }

    free(path_string_data_with_null_termiated);

    printf("\n");
    return 1;
  }

  wprintf(L"%s\n", path_string_data_with_null_termiated);

  // print hex values

  char* byte_buffer = (char*)path_string_data_with_null_termiated;
  for (DWORD i = 0; i < number_of_bytes_to_allocate; i++)
  {
    printf("%02x ", byte_buffer[i]);
  }
  printf("\n");

  // convert to multi-byte UTF-8 and print the bytes
  printf("=== UTF-8 printing ===\n");
  int conversion_buffer_size = 0;
  int int_result;
  int_result = WideCharToMultiByte(          //
      CP_UTF8,                               // [in]            UINT                               CodePage,
      0,                                     // [in]            DWORD                              dwFlags,
      path_string_data_with_null_termiated,  // [in]            _In_NLS_string_(cchWideChar)LPCWCH lpWideCharStr,
      number_of_characters - 1,              // [in]            int                                cchWideChar,
      NULL,                                  // [out, optional] LPSTR                              lpMultiByteStr,
      0,                                     // [in]            int                                cbMultiByte,
      NULL,                                  // [in, optional]  LPCCH                              lpDefaultChar,
      NULL                                   // [out, optional] LPBOOL                             lpUsedDefaultChar
  );

  if (int_result == 0)
  {
    fprintf(stderr, "WideCharToMultiByte failed at %s:%d\n", __FILE__, __LINE__);
    // TODO GetLastError
  }
  else
  {
    conversion_buffer_size = int_result;
    printf("%d bytes\n", conversion_buffer_size);
    char* utf8_byte_buffer = (char*)malloc(conversion_buffer_size);
    if (utf8_byte_buffer == 0)
    {
      fprintf(stderr, "malloc failed at %s:%d\n", __FILE__, __LINE__);
    }
    else
    {
      int_result = WideCharToMultiByte(          //
          CP_UTF8,                               // [in]            UINT                               CodePage,
          0,                                     // [in]            DWORD                              dwFlags,
          path_string_data_with_null_termiated,  // [in]            _In_NLS_string_(cchWideChar)LPCWCH lpWideCharStr,
          number_of_characters - 1,              // [in]            int                                cchWideChar,
          utf8_byte_buffer,                      // [out, optional] LPSTR                              lpMultiByteStr,
          conversion_buffer_size,                // [in]            int                                cbMultiByte,
          NULL,                                  // [in, optional]  LPCCH                              lpDefaultChar,
          NULL                                   // [out, optional] LPBOOL                             lpUsedDefaultChar
      );
      if (int_result == 0)
      {
        fprintf(stderr, "WideCharToMultiByte failed at %s:%d\n", __FILE__, __LINE__);
        // TODO GetLastError
      }
      else
      {
        for (int i = 0; i < conversion_buffer_size; i++)
        {
          putc(utf8_byte_buffer[i], stdout);
        }
        printf("\n");

        for (int i = 0; i < conversion_buffer_size; i++)
        {
          printf("%02x ", (0xff & utf8_byte_buffer[i]));
        }
        printf("\n");

        for (int i = 0; i < conversion_buffer_size; i++)
        {
          printf("%d\t", i);
          putc(utf8_byte_buffer[i], stdout);
          printf("\t");
          // printf("%02x ", utf8_byte_buffer[i]);
          printf("%02x ", (0xff & utf8_byte_buffer[i]));
          printf("\n");
        }

        // print python literal bytes string

        printf("b'");
        for (int i = 0; i < conversion_buffer_size; i++)
        {
          printf("\\x%02x", (0xff & utf8_byte_buffer[i]));
        }

        printf("'\n");
      }

      free(utf8_byte_buffer);
    }
  }

  free(path_string_data_with_null_termiated);
  return 0;
}
