copy frameworks\runtime-src\proj.win32\Debug.win32\Boids.exe debug\
copy *.dll debug\
copy frameworks\runtime-src\proj.win32\Debug.win32\libcocos2d.dll debug\
copy *.ini debug\
copy *.profile debug\
xcopy src debug\src\ /E /Y
xcopy res debug\res\ /E /Y
xcopy terrains debug\terrains\  /E /Y
pause