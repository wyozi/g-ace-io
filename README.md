Binary IO module for Garry's Mod.

## API
Similar to http://wiki.garrysmod.com/page/Category:file
```
table, table  gaceio.List(name) // Returns two tables. Files and folders
              gaceio.CreateDir(name)
              gaceio.Delete(name)
boolean       gaceio.Exists(name)
boolean       gaceio.IsDir(name)
string        gaceio.Read(name)
number        gaceio.Size(name)
number        gaceio.Time(name)
              gaceio.Write(name, text)
```
All functions return ```false, error_string``` on error, which means you might have to check for existence of ```error_string``` when using ```gaceio.Exists(name)``` or other functions that return a boolean.

## Usage
Grab binaries from Downloads tab, dump them into garrysmod/lua/bin, call ```require("gaceio")``` and have fun.

## Compilation
You need Garry's Bootil https://github.com/garrynewman/bootil
Remember to change BuildProjects.lua "links" location to where-ever you compiled bootil to.
