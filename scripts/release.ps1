# ==============================================================================
# Timer420 Automatic Release & Git Push Script
# ==============================================================================
param(
    [string]$Version = "0.2.0",
    [switch]$SkipPush = $false
)

$ErrorActionPreference = "Stop"

Write-Host "=================================================" -ForegroundColor Cyan
Write-Host "   Timer420 Release & Push Pipeline: v$Version" -ForegroundColor Cyan
Write-Host "=================================================" -ForegroundColor Cyan

# 1. Verify / Run Packaging
Write-Host "`n[1/4] Running Release Packaging..." -ForegroundColor Yellow
& "$PSScriptRoot/package.ps1" -Version $Version

# 2. Check Git Status
Write-Host "`n[2/4] Checking Git Repository Status..." -ForegroundColor Yellow
$gitStatus = git status --porcelain
if ($gitStatus) {
    Write-Host "Staging and committing updated release files..." -ForegroundColor Gray
    git add -A
    git commit -m "feat(release): 2026 update v$Version with automated release workflow"
} else {
    Write-Host "Working tree is clean." -ForegroundColor Gray
}

# 3. Create or Update Git Release Tag
Write-Host "`n[3/4] Creating Git Tag v$Version..." -ForegroundColor Yellow
$tagName = "v$Version"
$existingTag = git tag -l $tagName
if ($existingTag) {
    Write-Host "Tag $tagName already exists. Updating tag to current HEAD..." -ForegroundColor Yellow
    git tag -d $tagName
}
git tag -a $tagName -m "Timer420 Release $tagName (2026 Update)"
Write-Host "Created tag: $tagName" -ForegroundColor Green

# 4. Push to Git Server
if (-not $SkipPush) {
    Write-Host "`n[4/4] Pushing changes and release tags to git server..." -ForegroundColor Yellow
    git push origin main
    git push origin $tagName --force
    Write-Host "`n>>> Successfully pushed v$Version to git server!" -ForegroundColor Green
    Write-Host "GitHub Actions release workflow will automatically build and publish release assets." -ForegroundColor Cyan
} else {
    Write-Host "`n[4/4] Skipped remote push as requested (-SkipPush)." -ForegroundColor Yellow
}
