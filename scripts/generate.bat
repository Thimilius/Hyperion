@echo off
pushd %~dp0\..\
conan install . -s build_type=Debug -s compiler.runtime=MTd -if "build/debug"
conan install . -s build_type=Release -s compiler.runtime=MT -if "build/release"
premake5.exe vs2019 --editor --audio=none --physics=bullet
popd