@echo on

@rem set the variables
set ESP-IDF_PATH=D:\esp\esp-idf
set project_path=D:\robot
set PORT=COM3

@rem run the esp idf shell
call %ESP-IDF_PATH%\export.bat
cd %project_path%

::idf.py set-target esp32
::idf.py reconfigure
::idf.py size-components
::idf.py clean
::idf.py fullclean

@rem run the build command
idf.py build

:: check if the build was successful
if %errorlevel%==0 goto :continue
goto endprogram

:continue
set /p flash="Do you want to flash the esp32? (y/n): "
if %flash%==y goto flash
if %flash%==Y goto flash
goto endprogram

:flash
idf.py -p %PORT% flash monitor
:endprogram
exit