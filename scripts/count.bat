@echo off
pushd %~dp0\..
cloc hyperion\include hyperion\source hyperion-editor\include hyperion-editor\source
popd
