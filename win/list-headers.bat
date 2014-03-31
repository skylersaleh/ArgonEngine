@echo off
for /f "delims=" %%A in ('forfiles /p %1 /s /m *.hpp /c "cmd /c echo @relpath"') do (
  set "file=%%~A"
  setlocal enableDelayedExpansion
  echo %1\!file:~2!
  endlocal
)
for /f "delims=" %%A in ('forfiles /p %1 /s /m *.h /c "cmd /c echo @relpath"') do (
  set "file=%%~A"
  setlocal enableDelayedExpansion
  echo %1\!file:~2!
  endlocal
)
