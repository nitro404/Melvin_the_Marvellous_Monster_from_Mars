@ECHO OFF
CLS

SET LEVEL_DESIGNER_DIR=Level Designer
SET DATA_DIR=Data

ECHO Copying Sprite Sheet File...
XCOPY "%DATA_DIR%\spritesheets.ini" "%LEVEL_DESIGNER_DIR%" /I /Y
ECHO Done.

ECHO.
ECHO Compiling 2D Level Designer...
IF NOT EXIST "%LEVEL_DESIGNER_DIR%/bin" MKDIR "%LEVEL_DESIGNER_DIR%/bin"
CD "%LEVEL_DESIGNER_DIR%"
javac src\*.java -d bin
ECHO Done Compiling!
ECHO.
CD bin
ECHO Running Level Designer...
java LevelDesigner -../settings.ini
CD "../../"

pause