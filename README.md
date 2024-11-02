# DLL Manul Mapping via IAT Hooking
* Gets address of `TranslateMessage`
* Replaces address of hooked function with shellcode
* Maps DLL into the remote process's memory
* Writes back address of hooked function 