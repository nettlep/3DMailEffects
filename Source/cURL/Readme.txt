update to Version
	curl-7.9
	OpenSSL 0.9.6b 	
change make method from MFC dynamic DLL to Static DLL 
				Update Time 2001/10/04
----------------------------------------------------------------------
[system32]
	SSL DLL need for this library
[include]
	Header file Directory

[lib] Debug version of Curl Library

How to Use:
1.Copy DLL to %windows%\system32
2.Set Visual C++ include Directory to [include] Dir
3.Set Visual C++ Library Directory to lib Dir
4.Add lib to link option of Project Setting
5.include MFC Support
6.enjoy

					

					Jeff Huang 2001/08/26
					page588@mail2.intellect.com.tw
