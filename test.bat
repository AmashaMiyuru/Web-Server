:: it turn off the echoing commanf prompt, it only show output
@echo off   
setlocal enabledelayedexpansion

:: Set the URL to send requests to
set URL=http://localhost:8089/about.html

:: Number of requests per second
set REQUESTS_PER_SECOND=10

:: Duration to run (in seconds)
set DURATION=10

:: Loop for the given duration
for /L %%i in (1,1,%DURATION%) do (
    echo Sending 10 requests to %URL% (Iteration %%i)
    for /L %%j in (1,1,%REQUESTS_PER_SECOND%) do (
        start /B curl -s -o NUL -w "%%{http_code}" %URL%
    )
    timeout /T 1 >nul
)

echo Test completed.
pause
