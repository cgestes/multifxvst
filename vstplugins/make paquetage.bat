REM SCRIPT EXECUTER AVANT PAQUETAGE
copy /Y "ControlsGUI.dll" "gratomatic\ControlGUI.dll_dyn"
copy /Y "ControlsGUI stat.dll" "gratomatic\ControlGUI.dll_stat"
del /Q /F "gratomatic\ControlGUI.dll"
pause
