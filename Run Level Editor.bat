@ECHO OFF
CLS

SET LEVEL_DESIGNER_DIR=Level Designer

ECHO Compiling 2D Level Designer...
IF NOT EXIST "%LEVEL_DESIGNER_DIR%/bin" MKDIR "%LEVEL_DESIGNER_DIR%/bin"
CD "%LEVEL_DESIGNER_DIR%"
javac src\*.java -d bin
ECHO Done Compiling!
ECHO.
CD bin
ECHO Running Level Designer...
java LevelDesigner
CD "../../"
