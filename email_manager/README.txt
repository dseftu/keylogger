To get this configured on Visual Studio, go into project properties and do...


C/C++ -> General -> Additional Include Directories
	add zlib
		libpng
		libharu
		curl
	
	
C/C++ -> General -> Preprocessor
	add CURL_STATICLIB
	

Linker -> Input
	add curl\libcurl_a.lib
		libharu\libhpdfd.lib
		libharu\libhpdfsd.lib
		libpng\libpng16d.lib
		zlib\zlibd.lib








