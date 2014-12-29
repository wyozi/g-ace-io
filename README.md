Binary IO module for Garry's Mod.

## API
Similar to http://wiki.garrysmod.com/page/Category:file
```
table, table  gaceio.List(path) // Returns two tables. Files and folders
              gaceio.CreateDir(path)
              gaceio.Delete(path)
boolean       gaceio.Exists(path)
boolean       gaceio.IsDir(path)
string        gaceio.Read(path)
number        gaceio.Size(path)
number        gaceio.Time(path)
              gaceio.Write(path, str)
```

Path is relative to GarrysMod folder (aka the folder from which you launch srcds or gmod client). So for example to list addons you would use ```gaceio.List("./garrysmod/addons")```.

All functions return ```false, error_string``` on error, which means you might have to check for existence of ```error_string``` when using ```gaceio.Exists(name)``` or other functions that return a boolean.

## Usage
Grab binaries from Downloads tab, dump them into garrysmod/lua/bin, call ```require("gaceio")``` and have fun.

## Compilation
You need Garry's Bootil https://github.com/garrynewman/bootil
Remember to change BuildProjects.lua "links" location to where-ever you compiled bootil to.
