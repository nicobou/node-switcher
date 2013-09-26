{
  "targets": 
  [
    {
      "target_name": "switcher_addon",
      "cflags": [ "-std=c++0x", '<!@(pkg-config switcher-0.2 --cflags)'],
      "link_settings": {
          'libraries': ['<!@(pkg-config switcher-0.2 --libs)']
         }	   
      ,	  
      "sources": [ "switcher_addon.cpp" ]
  
  }
  ]	      
}
