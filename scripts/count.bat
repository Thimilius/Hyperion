@echo off
pushd %~dp0\..\projects\
cloc hyperion\include hyperion\source hyperion-editor\include hyperion-editor\source
popd