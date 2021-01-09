@echo off
pushd %~dp0\..\
if exist build ( rd /S /Q build )
if exist "run_tree\*.dll" ( del /Q run_tree\*.dll > nul )
if exist "run_tree\*.exe" ( del /Q run_tree\*.exe > nul )
popd