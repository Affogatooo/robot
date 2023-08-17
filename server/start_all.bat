@REM @echo off

@REM REM Set the paths to the server, client, and frontend
@REM set SERVER_PATH="D:\robot\server\backend\server.py"
@REM set CLIENT_PATH="D:\robot\server\backend\client.py"
@REM set PROXY_SERVER_PATH="D:\robot\server\backend\proxy.py"
@REM set FRONTEND_PATH="D:\robot\server\frontend\"

@REM REM Open the first terminal for the server
@REM start cmd /k python %SERVER_PATH%

@REM REM Wait for a few seconds before opening the second terminal (adjust as needed)
@REM timeout /t 2

@REM REM Open the second terminal for the client
@REM start cmd /k python %CLIENT_PATH%

@REM REM Wait for a few seconds before opening the third terminal (adjust as needed)
@REM timeout /t 2

@REM REM starts the proxy server
@REM start cmd /k python %PROXY_SERVER_PATH%

@REM REM Wait for a few seconds before opening the third terminal (adjust as needed)
@REM timeout /t 2

@REM REM Open the third terminal for the frontend
@REM cd %FRONTEND_PATH%
@REM start cmd /k npm start

@echo off

REM Set the paths to the server, client, proxy server, and frontend
set SERVER_PATH="D:\robot\server\backend\server.py"
set CLIENT_PATH="D:\robot\server\backend\client.py"
set PROXY_SERVER_PATH="D:\robot\server\backend\proxy.py"
set FRONTEND_PATH="D:\robot\server\frontend\"

REM Launch Windows Terminal with split panes
wt -d %~dp0 -p "Server" python %SERVER_PATH% ; split-pane -p "Proxy" python %PROXY_SERVER_PATH%

timeout /t 3
wt -d %~dp0 -p "Frontend" cmd /k  npm start --prefix %FRONTEND_PATH%