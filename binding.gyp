{
  "targets": 
  [
    {
      "target_name": "switcher_addon",
      "cflags": [ "-std=c++0x",
                  "-I/usr/local/include/switcher-0.2",
                  "-pthread", 
		  "-I/usr/include/gstreamer-0.10", 
 		  "-I/usr/include/glib-2.0", 
		  "-I/usr/lib/x86_64-linux-gnu/glib-2.0/include", 
		  "-I/usr/include/libxml2",
		  "-I/usr/include/json-glib-1.0",
		  "-I/usr/include/glib-2.0",
	 	  "-I/usr/lib/x86_64-linux-gnu/glib-2.0/include"
		  ],
     'link_settings': {
          'libraries': [
              "-lswitcher-0.2",
     	      "-pthread",
	      "-lgstreamer-0.10",
	      "-lgobject-2.0", 
              "-lgmodule-2.0",
              "-lgthread-2.0",
              "-lrt",
              "-lxml2",
              "-lglib-2.0",
              "-ljson-glib-1.0",
              "-lgio-2.0"
          ]	   
      },	  
      "sources": [ "switcher_addon.cpp" ]
    }
  ]	      
}
