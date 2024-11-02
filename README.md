# DLL Manul Mapping via IAT Hooking
Simple DLL Manual Mapper that uses frequently used iat functions to execute shellcode. Program doesn't create any threads.

![IAT DLL Injector](https://raw.githubusercontent.com/rft0/iat-dll-injector/refs/heads/main/img/cp.gif)

## Process
* Gets address of `TranslateMessage`
* Replaces address of hooked function with shellcode
* Maps DLL into the remote process's memory
* Writes back address of hooked function

## Usage
* Open executable normally.
* pass arguments via command line. (injector64.exe <process> <dllpath>)

You can use [Kernel Mode DLL Injector](https://github.com/rft0/km-dll-mapper) for similiar and extended program.
