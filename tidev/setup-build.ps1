<#
.SYNOPSIS
    Sets up the C++ project environment by installing dependencies and building the project.

.DESCRIPTION
    This script automates the setup process by cloning vcpkg, installing GLFW, building GLAD, and configuring the project with CMake.

.PARAMETER buildDir
    The directory where the build files will be generated.

.PARAMETER vcpkgDir
    The directory where vcpkg will be cloned.

.PARAMETER gladZipUrl
    The URL to download the GLAD source code.

.PARAMETER gladZipFile
    The local path for the downloaded GLAD zip file.

.PARAMETER gladExtractDir
    The directory where GLAD will be extracted.

.PARAMETER gladDestDir
    The destination directory for GLAD include and source files.

.PARAMETER vcpkgToolchain
    The path to the vcpkg toolchain file.

.PARAMETER vcpkgTriplet
    The vcpkg triplet to use for installing packages.

. 
    .\setup.ps1
#>

# import module
# Import the module
# $modulePath = Join-Path $PSScriptRoot".\\powershellModule.psm1"
param(
    [string]$buildDir = (Join-Path $PSScriptRoot "build"),
    [string]$vcpkgDir = (Join-Path $PSScriptRoot "vcpkg"),
    [string]$gladZipUrl = "https://github.com/Dav1dde/glad/archive/refs/heads/master.zip",
    [string]$gladZipFile = (Join-Path $PSScriptRoot "glad.zip"),
    [string]$gladExtractDir = (Join-Path $PSScriptRoot "glad-master"),
    [string]$gladDestDir = (Join-Path $PSScriptRoot "thirdParty\glad"),
    [string]$vcpkgToolchain = "scripts\buildsystems\vcpkg.cmake",
    [string]$vcpkgTriplet = "x64-windows-static"
)

function Write-ProgressMessage {
    param([string]$Message
    )
    Write-Host "=== $Message ==="
}

function Test-GladFiles {
    return (Test-Path (Join-Path $gladDestDir "include\glad\glad.h")) -and
           (Test-Path (Join-Path $gladDestDir "include\KHR\khrplatform.h")) -and
           (Test-Path (Join-Path $gladDestDir "src\glad.c"))
}

function Get-VCPKG {
    param (
        [String] $vcpkgDir=$vcpkgDir)

    if (-not $vcpkgDir) {
        Write-Error "vcpkgDir parameter is required."
        return $false
    }

    if (Test-Path (Join-Path $vcpkgDir 'vcpkg.exe')) {
        Write-ProgressMessage "vcpkg.exe is available"
        return $true
    }

    Write-ProgressMessage "Cloning vcpkg"
    try {
        git clone https://github.com/microsoft/vcpkg.git $vcpkgDir
    } catch {
        Write-Error "Failed to clone vcpkg repository: $_"
        return $false
    }

    Write-ProgressMessage "Bootstrapping vcpkg"
    try {
        & (Join-Path $vcpkgDir "bootstrap-vcpkg.bat")
    } catch {
        Write-Error "Failed to bootstrap vcpkg: $_"
        return $false
    }

    return $true
}

function Test-Dependencies {
    $dependencies = @('git', 'cmake')
    foreach ($dep in $dependencies) {
        if (-not (Get-Command $dep -ErrorAction SilentlyContinue)) {
            Write-Error "$dep is not installed or not found in PATH."
            exit 1
        }
    }
}

function Get-MSBuildPath {
    $vswherePath = Join-Path "${env:ProgramFiles(x86)}" "Microsoft Visual Studio\Installer\vswhere.exe"
    if (-not (Test-Path $vswherePath)) {
        Write-Error "vswhere.exe not found. Please ensure Visual Studio is installed."
        exit 1
    }

    $vsPath = & $vswherePath -latest -products * -requires Microsoft.Component.MSBuild -property installationPath -format value
    if (-not $vsPath) {
        Write-Error "Visual Studio with MSBuild not found."
        exit 1
    }

    $msbuildPath = Join-Path $vsPath "MSBuild\Current\Bin\MSBuild.exe"
    if (-not (Test-Path $msbuildPath)) {
        Write-Error "MSBuild.exe not found."
        exit 1
    }

    return $msbuildPath
}
function New_folder {
    param (
        [string]$thirdPartyRoot=".\ThirdParty"
    )
    
    # make third party root directory

$message="checking the existing directory: $thirdPartyRoot"
Write-Host $message -ForegroundColor DarkGreen
if (-not(powershellModule\Test-FolderExistence -FolderName $thirdPartyRoot))
    {
        Write-Host "making  $thirdPartyRoot" -ForegroundColor Cyan
        Write-Output "Creating folder: $thirdPartyRoot"
        New-Item -ItemType Directory -Path $thirdPartyRoot
    }
else
{
    Write-Host "The directory already exists: $thirdPartyRoot" -ForegroundColor Green
}
}
function Install-Package 
{
    param (
        [string] $packageName="glfw3",
        [string] $vcpkgDir=$vcpkgDir
    )
    # Write-ProgressMessage "Installing GLFW ($vcpkgTriplet) using vcpkg"
    try 
    {
        & (Join-Path $vcpkgDir "vcpkg.exe") install ("${packageName}:$vcpkgTriplet") --triplet $vcpkgTriplet
    } 
    catch 
    {
        Write-Error "Failed to install GLFW using vcpkg."
        return $false
    }
    return $true
}
# Start of the script
Test-Dependencies

# Step 1: Clone and bootstrap vcpkg
# if (-Not (Test-Path -Path $vcpkgDir)) {
#     Write-ProgressMessage "Cloning vcpkg"
#     try {
#         git clone https://github.com/microsoft/vcpkg.git $vcpkgDir
#     } catch {
#         Write-Error "Failed to clone vcpkg repository."
#         exit 1
#     }
# }

# Write-ProgressMessage "Bootstrapping vcpkg"
# try {
#     & (Join-Path $vcpkgDir "bootstrap-vcpkg.bat")
# } catch {
#     Write-Error "Failed to bootstrap vcpkg."
#     exit 1
# }
if (-not(Get-VCPKG $vcpkgDir))
{
    Write-Error "Failed to clone or bootstrap vcpkg."
    exit 1
}
Write-Host "------------build glfw3------------" -BackgroundColor Cyan
Install-Package "glfw3"
Write-Host "===========build glew===========" -BackgroundColor Cyan

Install-Package "glew"

Write-Host "+++++++++++build glad+++++++++++" -BackgroundColor Cyan

Install-Package "glad"
# # Step 2: Install GLFW (static) using vcpkg
# Write-ProgressMessage "Installing GLFW ($vcpkgTriplet) using vcpkg"
# try {
#     & (Join-Path $vcpkgDir "vcpkg.exe") install "glfw3:$vcpkgTriplet" --triplet $vcpkgTriplet
# } catch {
#     Write-Error "Failed to install GLFW using vcpkg."
#     exit 1
# }


<#
# Step 3: Download and build GLAD if not already set up
if (Test-GladFiles) {
    Write-ProgressMessage "GLAD is already set up"
} else {
    if (-Not (Test-Path -Path $gladZipFile)) {
        Write-ProgressMessage "Downloading GLAD"
        try {
            Invoke-WebRequest -Uri $gladZipUrl -OutFile $gladZipFile -ErrorAction Stop
        } catch {
            Write-Error "Failed to download GLAD."
            exit 1
        }
    }

    if (-Not (Test-Path -Path $gladExtractDir)) {
        Write-ProgressMessage "Extracting GLAD"
        try {
            Expand-Archive -Path $gladZipFile -DestinationPath $PSScriptRoot -Force -ErrorAction Stop
        } catch {
            Write-Error "Failed to extract GLAD."
            exit 1
        }
    }

    Write-ProgressMessage "Generating GLAD project with CMake"
    $gladBuildDir = Join-Path $gladExtractDir "build"
    if (-Not (Test-Path -Path $gladBuildDir)) { New-Item -ItemType Directory -Path $gladBuildDir | Out-Null }

    # Detecting the Visual Studio generator
    $cmakeGenerator = ""
    $vswherePath = Join-Path "${env:ProgramFiles(x86)}" "Microsoft Visual Studio\Installer\vswhere.exe"
    if (Test-Path $vswherePath) {
        $vsVersion = (& $vswherePath -latest -products * -property catalog_productLineVersion -format value)
        if ($vsVersion) {
            $cmakeGenerator = "Visual Studio $vsVersion"
        } else {
            Write-Warning "Could not detect Visual Studio version. Defaulting to Visual Studio 17 2022."
            $cmakeGenerator = "Visual Studio 17 2022"
        }
    } else {
        Write-Warning "vswhere.exe not found. Defaulting to Visual Studio 17 2022."
        $cmakeGenerator = "Visual Studio 17 2022"
    }

    # Generate GLAD project
    try {
        cmake -B $gladBuildDir -S $gladExtractDir -G "$cmakeGenerator" `
            -DGLAD_PROFILE="core" -DGLAD_API="gl=4.6" -DGLAD_GENERATOR="c" -DBUILD_SHARED_LIBS=OFF
    } catch {
        Write-Error "Failed to generate GLAD project with CMake."
        exit 1
    }

    Write-ProgressMessage "Building GLAD"
    $msbuildPath = Get-MSBuildPath
    try {
        & $msbuildPath (Join-Path $gladBuildDir "ALL_BUILD.vcxproj") /p:Configuration=Release
    } catch {
        Write-Error "Failed to build GLAD with MSBuild."
        exit 1
    }

    Write-ProgressMessage "Copying GLAD build files"
    try {
        # Ensure destination directories exist
        $gladIncludeDir = Join-Path $gladDestDir "include"
        $gladSrcDir = Join-Path $gladDestDir "src"
        New-Item -ItemType Directory -Path (Join-Path $gladIncludeDir "glad") -Force | Out-Null
        New-Item -ItemType Directory -Path (Join-Path $gladIncludeDir "KHR") -Force | Out-Null
        New-Item -ItemType Directory -Path $gladSrcDir -Force | Out-Null

        # Copy files
        Copy-Item -Path (Join-Path $gladBuildDir "include\glad\glad.h") -Destination (Join-Path $gladIncludeDir "glad") -Force
        Copy-Item -Path (Join-Path $gladBuildDir "include\KHR\khrplatform.h") -Destination (Join-Path $gladIncludeDir "KHR") -Force
        Copy-Item -Path (Join-Path $gladBuildDir "src\glad.c") -Destination $gladSrcDir -Force
    } catch {
        Write-Error "Failed to copy GLAD build files."
        exit 1
    }

    # Clean up
    Write-ProgressMessage "Cleaning up temporary files"
    try {
        Remove-Item -Recurse -Force $gladExtractDir
        Remove-Item -Force $gladZipFile
    } catch {
        Write-Warning "Failed to clean up temporary files."
    }
}

# Step 4: Build your project
Write-ProgressMessage "Configuring and building the project"
$toolchainFile = Join-Path $vcpkgDir $vcpkgToolchain

try {
    cmake -G "Visual Studio 17 2022" -B $buildDir -S $PSScriptRoot `
        -DCMAKE_TOOLCHAIN_FILE="$toolchainFile" `
        -DCMAKE_BUILD_TYPE=Release `
        -DVCPKG_TARGET_TRIPLET=$vcpkgTriplet
    if ($LASTEXITCODE -ne 0) {
        Write-Error "CMake configuration failed."
        exit $LASTEXITCODE
    }

    cmake --build $buildDir --config Release
    if ($LASTEXITCODE -ne 0) {
        Write-Error "CMake build failed."
        exit $LASTEXITCODE
    }
} catch {
    Write-Error "Failed to configure and build the project."
    exit 1
}

Write-ProgressMessage "Build process completed successfully!"
#>