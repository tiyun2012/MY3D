# Import the module
Write-Host "start progress from $PSScriptRoot"
Set-Location $PSScriptRoot
# import the module
$parentFolder = Split-Path -Path $PSScriptRoot -Parent
$modulePath = Join-Path -Path $parentFolder "PowershellModule\powershellModule.psm1"
# $modulePath ="C:\Users\pc\Desktop\Dev\MY3D\Powersell\powershellModule.psm1"
if (-not(Test-Path $modulePath))
    { 
        Write-Error "Module file not found: $modulePath"
        return
    }

    try {
        Write-Progress -Activity "1: -----importing module:  $modulePath" -Status "start importing" -PercentComplete 0
        Import-Module $modulePath -Verbose -Force
        Write-Progress -Activity "importing module $modulePath" -Status "done" -PercentComplete 100
        }
    catch {
        Write-Error "Failed to import module: $_"
        return
    }
Get-Command -Module powershellModule
# Set up logging and error handling
$logFile =Join-Path $PSScriptRoot "setup.log"
if ( -not (powershellModule\Test-FileExistence -FilePath $logFile))
{
    Add-Content -Path $logFile -Value empty strings
}
powershellModule\Write-ProgressLog -logFile $logFile -clean $true

# make third party root directory
[string]$thirdPartyRoot=Join-Path $PSScriptRoot "ThirdParty"
if (-not(powershellModule\New-Folder -Name $thirdPartyRoot))
{
    return
}
# imgui
Write-Host "--- Checking Dear ImGui Module-----" -ForegroundColor Yellow
$imguiUri = "https://github.com/ocornut/imgui/archive/refs/heads/master.zip"
$imguiZip=Join-Path $thirdPartyRoot imguifile.zip
# $imguiZip="${PSScriptRoot}\${thirdPartyRoot}"

$imguiRoot=Join-Path $thirdPartyRoot "Imgui"
powershellModule\New-Folder -Name $imguiRoot
$imguiList = @("imgui_widgets.cpp","imgui_demo.cpp","imgui_draw.cpp","imgui.h",
                        "imgui.cpp","imstb_truetype.h",
                        "imgui_tables.cpp","imgui_internal.h","imconfig.h","imstb_rectpack.h","imstb_textedit.h",
                        "backends/imgui_impl_glfw.h","backends/imgui_impl_opengl3.h")
if ((powershellModule\Test-FilesMissing -RootPath $imguiRoot -FileList $imguiList))
{
    try
        {

            # Extract specific Dear ImGui files
            
                        # dowload and copy files
                powershellModule\Invoke-WebFile -Url $imguiUri -DestinationPath $imguiZip
                powershellModule\Expand-SpecificFilesFromZip -zipFilePath $imguiZip -destinationPath $imguiRoot -filesTracked $imguiList
                Remove-Item $imguiZip
        }
                
    catch
        {
            Write-Error "Failed to download: $_"
            return
        }
}
else {
    Write-Host "-------------imgui is available------------------------" -ForegroundColor Yellow
}

# Glad
Write-Host "--- Checking  Glad:: -----" -ForegroundColor Yellow
$gladUri = "https://github.com/Dav1dde/glad/archive/refs/heads/master.zip"
$gladZip=Join-Path $thirdPartyRoot Gladfile.zip

$gladRoot=Join-Path $thirdPartyRoot "glad"
powershellModule\New-Folder -Name $gladRoot
powershellModule\Invoke-WebFile -Url $gladUri -DestinationPath $gladZip
$visualStudioPath=powershellModule\Get-MSBuildPath
powershellModule\Expand-Zip -ZipFilePath $gladZip -ExtractToPath $gladRoot
$gladFolder =Join-Path $gladRoot "glad-master"
$cmakeListDir =$gladFolder #Join-Path $glewFolder "build\cmake"
powershellModule\Invoke-LibraryBuild `
    -SourceDir $gladFolder `
    -CMakeListsDir $cmakeListDir `
    -LibraryName "GLAD"
try
    {
        Write-Host "--------------cleanup glad-------------"
        # powershellModule\Invoke-WebFile -Url $glewUri -DestinationPath $glewZip
        Remove-Item $gladZip
    }
catch
{
    Write-Error "Failed to download: $_"
    return
}
$GladList = @(
    (Join-Path $gladFolder "build/src"),
    (Join-Path $gladFolder "Release"),
    (Join-Path $gladFolder "include")
)
Copy-Folders $GladList $gladRoot

# Glew
Write-Host "--- Checking  Glew:: -----" -ForegroundColor Yellow
$glewUri = "https://github.com/nigels-com/glew/archive/refs/heads/master.zip"
$glewZip=Join-Path $thirdPartyRoot Glewfile.zip

$glewRoot=Join-Path $thirdPartyRoot "glew"
powershellModule\New-Folder -Name $glewRoot
powershellModule\Invoke-WebFile -Url $glewUri -DestinationPath $glewZip
# $visualStudioPath=powershellModule\Get-MSBuildPath
powershellModule\Expand-Zip -ZipFilePath $glewZip -ExtractToPath $glewRoot
$glewFolder =Join-Path $glewRoot "glew-master"
$cmakeListDir = Join-Path $glewFolder "build\cmake"
powershellModule\Invoke-LibraryBuild `
    -SourceDir $glewFolder `
    -CMakeListsDir $cmakeListDir `
    -LibraryName "GLEW"
# $GlewList = @()
try
    {
        Write-Host "--------------cleanup glew-------------"
        # powershellModule\Invoke-WebFile -Url $glewUri -DestinationPath $glewZip
        Remove-Item $glewZip
    }
catch
{
    Write-Error "Failed to download: $_"
    return
}






Write-Host "------------Settings successfully completed.---------------------------" -ForegroundColor Green


#>

