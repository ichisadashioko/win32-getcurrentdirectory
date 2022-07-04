# Windows WIN32 API - GetCurrentDirectory

`GetCurrentDirectoryA` works fine on ASCII only characters path. It returns `?` - `63` for characters outside of ASCII range.
