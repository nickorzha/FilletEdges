"# FilletEdges" 

# Preview Of FilletEdges Working
[Imgur](https://i.imgur.com/VCWL7WF.gifv)

# Steps to Build

* Edit boxFillet.vcxproj, find <ObjectARXPath> and change Value to SDK path
* Launch VS 2017 Developer Command Prompt 
* msbuild /t:Build /p:Configuration=Debug;Platform=x64
* Open Box.dwg in AutoCAD 2019, 
* Run TSOLFILLET