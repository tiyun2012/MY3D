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

$imguiRoot=Join-Path $thirdPartyRoot "imgui-master"
# powershellModule\New-Folder -Name $imguiRoot
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
                # powershellModule\Expand-SpecificFilesFromZip -zipFilePath $imguiZip -destinationPath $imguiRoot -filesTracked $imguiList
                # Remove-Item $imguiZip
                powershellModule\Expand-Zip -ZipFilePath $imguiZip -ExtractToPath $thirdPartyRoot
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
# $gladRoot=Join-Path $thirdPartyRoot "glad"
$gladFolder =Join-Path $thirdPartyRoot "glad-master"
$GladList=@("glad-master\build\include\glad\glad.h","glad-master\build\include\KHR\khrplatform.h","glad-master\build\Release\glad.lib","glad-master\build\src\glad.c")
if(powershellModule\Test-FilesMissing -RootPath $thirdPartyRoot -FileList $GladList)
{  
    Write-Host "--- Checking  Glad:: -----" -ForegroundColor Yellow
    $gladUri = "https://github.com/Dav1dde/glad/archive/refs/heads/master.zip"
    $gladZip=Join-Path $thirdPartyRoot Gladfile.zip
    
    # powershellModule\New-Folder -Name $gladRoot
    powershellModule\Invoke-WebFile -Url $gladUri -DestinationPath $gladZip
    powershellModule\Expand-Zip -ZipFilePath $gladZip -ExtractToPath $thirdPartyRoot
    # $cmakeListDir =$gladFolder #Join-Path $glewFolder "build\cmake"
    powershellModule\Invoke-LibraryBuild `
        -SourceDir $gladFolder `
        -CMakeListsDir $gladFolder `
        -LibraryName "GLAD"
    
    # Write-Host "--------------copying glad-------------"
    # # Copy all files from glad's build folder to the destination folder.
    # $sourceFolders = @("${gladFolder}\build\include", "${gladFolder}\build\Release", "${gladFolder}\build\src")
    # $destination = $gladRoot
    
    # foreach ($folder in $sourceFolders) {
    #     Copy-Item -Path $folder -Destination $destination -Recurse
    # }
    # try
    #     {
    #         Write-Host "--------------cleanup glad-------------"
    #         Remove-Item $gladZip
    #         # Remove-Item $gladFolder
    #     }
    # catch
    # {
    #     Write-Error "cleanum face issues : $_"
    #     return
    # }

}
else {
    <# Action when all if and elseif conditions are false #>
    Write-Host "glad is available" -ForegroundColor Yellow
}


# glfw https://github.com/glfw/glfw/archive/refs/heads/master.zip
# $glfwRoot=Join-Path $thirdPartyRoot "glfw"
$glfwFolder =Join-Path $thirdPartyRoot "glfw-master"


Write-Host "--- Checking  Glfw:: -----" -ForegroundColor Yellow
$GlfwList=@("glfw-master\build\src\Release\glfw3.lib","glfw-master\include\GLFW\glfw3.h","glfw-master\include\GLFW\glfw3native.h")
if(powershellModule\Test-FilesMissing -RootPath $thirdPartyRoot -FileList $GlfwList)
{
    $glfwUri = "https://github.com/glfw/glfw/archive/refs/heads/master.zip"
    $glfwZip=Join-Path $thirdPartyRoot glfwfile.zip
    
    # powershellModule\New-Folder -Name $glfwRoot
    powershellModule\Invoke-WebFile -Url $glfwUri -DestinationPath $glfwZip
    powershellModule\Expand-Zip -ZipFilePath $glfwZip -ExtractToPath $thirdPartyRoot
    powershellModule\Invoke-LibraryBuild `
            -SourceDir $glfwFolder `
            -CMakeListsDir $glfwFolder `
            -LibraryName "glfw"

}
else {
    <# Action when all if and elseif conditions are false #>
    Write-Host "glad is available" -ForegroundColor Yellow
}






Write-Host "------------Settings successfully completed.---------------------------" -ForegroundColor Green


#>

