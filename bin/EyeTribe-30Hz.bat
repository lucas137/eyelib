@echo off

REM   Usage:
REM
REM   EyeTribe [<config file>] [--help (-?)] {options},
REM   where options is zero or more of:
REM
REM   --port=<tcp port number> (-p)
REM   --remote=<bool> (-r)
REM   --device=<device number> (-d)
REM   --framerate=<30|60>  (-f)

start "EyeTribe server" "C:\Program Files (x86)\EyeTribe\Server\EyeTribe.exe" --framerate=30
