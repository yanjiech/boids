copy frameworks\runtime-src\proj.win32\Release.win32\Boids.exe .\
copy frameworks\runtime-src\proj.win32\Release.win32\libcocos2d.dll .\ 
copy Boids.exe release\
copy *.dll release\
copy *.ini release\
xcopy src release\src\ /E /Y /EXCLUDE:uncopy.txt
xcopy res release\res\ /E /Y
xcopy terrains release\terrains\  /E /Y
pause