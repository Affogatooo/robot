@echo on

@rem set the variables
set ESP-IDF_PATH=D:\esp\esp-idf
set project_path=D:\robot
set PORT=COM3

@rem run the esp idf shell
call %ESP-IDF_PATH%\export.bat
cd %project_path%

idf.py monitor