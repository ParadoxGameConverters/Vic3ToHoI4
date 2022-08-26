#!/bin/bash
# to be run from the project root directory
cd external/Fronter.NET || exit
dotnet restore
dotnet build --configuration=release
cp -r Release/* ../../build/Release-Linux