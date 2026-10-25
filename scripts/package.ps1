# Timer420 Release Packaging Script
param(
    [string]$Version = "0.2.0"
)

$ErrorActionPreference = "Stop"

Write-Host "Packaging Timer420 v$Version..." -ForegroundColor Green

$DistDir = "dist"
$ZipName = "Timer420-v$Version-windows-x64.zip"

if (Test-Path $DistDir) {
    Remove-Item -Recurse -Force $DistDir
}

New-Item -ItemType Directory -Path $DistDir | Out-Null
New-Item -ItemType Directory -Path "$DistDir/docs" | Out-Null

$ExePath = if (Test-Path "build/Release/Timer420.exe") {
    "build/Release/Timer420.exe"
} elseif (Test-Path "build/Timer420.exe") {
    "build/Timer420.exe"
} elseif (Test-Path "build_ninja/Timer420.exe") {
    "build_ninja/Timer420.exe"
} else {
    (Get-ChildItem -Path . -Filter "Timer420.exe" -Recurse | Select-Object -ExpandProperty FullName -First 1)
}

if (-not $ExePath -or -not (Test-Path $ExePath)) {
    throw "Timer420.exe binary not found. Please compile the project first."
}

Write-Host "Using binary: $ExePath" -ForegroundColor Cyan
Copy-Item $ExePath -Destination "$DistDir/Timer420.exe"
Copy-Item "README.md" -Destination "$DistDir/"
Copy-Item "LICENSE" -Destination "$DistDir/"
Copy-Item "CHANGELOG.md" -Destination "$DistDir/"
if (Test-Path "docs/USER_GUIDE.md") {
    Copy-Item "docs/USER_GUIDE.md" -Destination "$DistDir/docs/"
}

Compress-Archive -Path "$DistDir/*" -DestinationPath $ZipName -Force

Write-Host "Successfully packaged release archive: $ZipName" -ForegroundColor Cyan
