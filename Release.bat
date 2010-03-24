@ECHO OFF
CLS

SET RELEASE_DIR=Game
SET MAP_DIR=Maps
SET SPRITE_DIR=Sprites
SET DATA_DIR=Data
SET GAME_EXE_COMPILED=Game Engine.exe
SET GAME_EXE=Melvin the Marvellous Monster from Mars.exe
SET GAME_EXE_DIR=Game Engine\Release
SET LEVEL_DESIGNER_DIR=Level Designer

ECHO Creating new release of Game in folder "%RELEASE_DIR%"...

IF NOT EXIST "%RELEASE_DIR%" MKDIR "%RELEASE_DIR%"

ECHO.
ECHO Copying Maps...
XCOPY "%MAP_DIR%\*.2d" "%RELEASE_DIR%\%MAP_DIR%" /I /Y

ECHO.
ECHO Copying Sprites...
XCOPY "%SPRITE_DIR%" "%RELEASE_DIR%\%SPRITE_DIR%" /I /Y

ECHO.
ECHO Copying Data Files...
XCOPY "%DATA_DIR%" "%RELEASE_DIR%" /I /Y /S

ECHO.
ECHO Copying Sprite Sheet File...
XCOPY "%DATA_DIR%\spritesheets.ini" "%LEVEL_DESIGNER_DIR%" /I /Y
ECHO Done.

ECHO.
ECHO Copying Game Executable...
XCOPY "%GAME_EXE_DIR%\%GAME_EXE_COMPILED%" "%RELEASE_DIR%" /I /Y
IF EXIST "%RELEASE_DIR%\%GAME_EXE%" DEL "%RELEASE_DIR%\%GAME_EXE%"
REN "%RELEASE_DIR%\%GAME_EXE_COMPILED%" "%GAME_EXE%"

ECHO.
ECHO Done!

ECHO.
PAUSE
