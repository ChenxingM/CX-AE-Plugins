@echo off
:: deploy.bat - Quick launcher for deploy.ps1
:: Right-click and "Run as Administrator"

cd /d "%~dp0"
powershell -ExecutionPolicy Bypass -File "%~dp0deploy.ps1" %*
pause
