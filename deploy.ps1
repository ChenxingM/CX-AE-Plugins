# deploy.ps1
# Deploys AEX plugins by creating shortcuts in After Effects plugin directory
# Run as Administrator: powershell -ExecutionPolicy Bypass -File deploy.ps1

param(
    [string]$AEVersion = "2025",
    [switch]$Remove
)

# Configuration
$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$SourceDir = Join-Path $ScriptDir "output"
$TargetBase = "C:\Program Files\Adobe\Adobe After Effects $AEVersion\Support Files\Plug-ins"
$TargetDir = Join-Path $TargetBase "CX Animation Tools"

Write-Host "============================================" -ForegroundColor Cyan
Write-Host "  CX AE Plugins Deployment Script" -ForegroundColor Cyan
Write-Host "============================================" -ForegroundColor Cyan
Write-Host ""

# Check if running as administrator
$isAdmin = ([Security.Principal.WindowsPrincipal] [Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)
if (-not $isAdmin) {
    Write-Host "ERROR: This script requires Administrator privileges." -ForegroundColor Red
    Write-Host "Please run PowerShell as Administrator and try again." -ForegroundColor Yellow
    exit 1
}

# Check source directory
if (-not (Test-Path $SourceDir)) {
    Write-Host "ERROR: Source directory not found: $SourceDir" -ForegroundColor Red
    Write-Host "Please build the project first." -ForegroundColor Yellow
    exit 1
}

# Check AE installation
if (-not (Test-Path $TargetBase)) {
    Write-Host "ERROR: After Effects $AEVersion not found at: $TargetBase" -ForegroundColor Red
    Write-Host "Use -AEVersion parameter to specify a different version." -ForegroundColor Yellow
    exit 1
}

# Remove mode
if ($Remove) {
    Write-Host "Removing shortcuts..." -ForegroundColor Yellow
    if (Test-Path $TargetDir) {
        Get-ChildItem -Path $TargetDir -Filter "*.lnk" | ForEach-Object {
            Write-Host "  Removing: $($_.Name)" -ForegroundColor Gray
            Remove-Item $_.FullName -Force
        }
        # Remove directory if empty
        if ((Get-ChildItem -Path $TargetDir | Measure-Object).Count -eq 0) {
            Remove-Item $TargetDir -Force
            Write-Host "  Removed empty directory: CX Animation Tools" -ForegroundColor Gray
        }
    }
    Write-Host "Done!" -ForegroundColor Green
    exit 0
}

# Create target directory if not exists
if (-not (Test-Path $TargetDir)) {
    Write-Host "Creating directory: $TargetDir" -ForegroundColor Yellow
    New-Item -ItemType Directory -Path $TargetDir -Force | Out-Null
}

# Get all .aex files
$aexFiles = Get-ChildItem -Path $SourceDir -Filter "*.aex"

if ($aexFiles.Count -eq 0) {
    Write-Host "WARNING: No .aex files found in: $SourceDir" -ForegroundColor Yellow
    Write-Host "Please build the project first." -ForegroundColor Yellow
    exit 1
}

Write-Host "Source: $SourceDir" -ForegroundColor Gray
Write-Host "Target: $TargetDir" -ForegroundColor Gray
Write-Host ""

# Create WScript.Shell for shortcut creation
$WshShell = New-Object -ComObject WScript.Shell

# Create shortcuts for each .aex file
foreach ($aex in $aexFiles) {
    $shortcutPath = Join-Path $TargetDir "$($aex.BaseName).lnk"

    Write-Host "Creating shortcut: $($aex.Name)" -ForegroundColor White

    # Remove existing shortcut
    if (Test-Path $shortcutPath) {
        Remove-Item $shortcutPath -Force
    }

    # Create shortcut
    $Shortcut = $WshShell.CreateShortcut($shortcutPath)
    $Shortcut.TargetPath = $aex.FullName
    $Shortcut.Description = "CX Animation Tools Plugin"
    $Shortcut.Save()

    Write-Host "  -> $shortcutPath" -ForegroundColor Green
}

Write-Host ""
Write-Host "============================================" -ForegroundColor Cyan
Write-Host "  Deployment Complete!" -ForegroundColor Green
Write-Host "  $($aexFiles.Count) plugin(s) deployed" -ForegroundColor Green
Write-Host "============================================" -ForegroundColor Cyan
Write-Host ""
Write-Host "Restart After Effects to load the plugins." -ForegroundColor Yellow
