@echo off
setlocal disableDelayedExpansion
for /f "delims=" %%A in ('forfiles /p %1 /s /m *.cpp /c "cmd /c echo @relpath"') do (
  set "file=%%~A"
  setlocal enableDelayedExpansion
  echo %1\!file:~2!
  endlocal
)
setlocal disableDelayedExpansion
for /f "delims=" %%A in ('forfiles /p %1 /s /m *.c /c "cmd /c echo @relpath"') do (
  set "file=%%~A"
  setlocal enableDelayedExpansion
  echo %1\!file:~2!
  endlocal
)
setlocal disableDelayedExpansion
for /f "delims=" %%A in ('forfiles /p %1 /s /m *.cc /c "cmd /c echo @relpath"') do (
  set "file=%%~A"
  setlocal enableDelayedExpansion
  echo %1\!file:~2!
  endlocal
)