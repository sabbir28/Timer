# Timer420 Release Packaging Script
param(
    [string]$Version = "0.1.0"
)

$ErrorActionPreference = "Stop"

Write-Host "Packaging Timer420 v$Version..." -ForegroundColor Green

$DistDir = "dist"
$ZipName = "Timer420-v$Version-win64.zip"

if (Test-Path $DistDir) {
    Remove-Item -Recurse -Force $DistDir
}

New-Item -ItemType Directory -Path $DistDir | Out-Null
New-Item -ItemType Directory -Path "$DistDir/docs" | Out-Null

Copy-Item "build/Release/Timer420.exe" -Destination "$DistDir/"
Copy-Item "README.md" -Destination "$DistDir/"
Copy-Item "LICENSE" -Destination "$DistDir/"
Copy-Item "CHANGELOG.md" -Destination "$DistDir/"
Copy-Item "docs/USER_GUIDE.md" -Destination "$DistDir/docs/"

Compress-Archive -Path "$DistDir/*" -DestinationPath $ZipName -Force

Write-Host "Successfully packaged release archive: $ZipName" -ForegroundColor Cyan
