# Define parameters for build, vcpkg, glad, and glfw directories relative to the script location
param(
    [string]$buildDir = ".\build",
    [string]$vcpkgDir = ".\vcpkg",
    [string]$gladZipUrl = "https://github.com/Dav1dde/glad/archive/refs/heads/master.zip",
    [string]$gladZipFile = ".\glad.zip",
    [string]$gladExtractDir = ".\glad-master",
    [string]$gladDestDir = ".\thirdParty\glad",
    [string]$vcpkgToolchain = "scripts/buildsystems/vcpkg.cmake"
)

function Show-Progress {
    param([string]$message)
    Write-Host "=== $message ==="
}

function Test-GladFiles {
    if ((Test-Path "$gladDestDir\include\glad\glad.h") -and 
        (Test-Path "$gladDestDir\include\KHR\khrplatform.h") -and 
        (Test-Path "$gladDestDir\src\glad.c")) {
        return $true
    } else {
        return $false
    }
}

# Step 1: Clone and bootstrap vcpkg
if (-Not (Test-Path -Path $vcpkgDir)) {
    Show-Progress "Cloning vcpkg"
    git clone https://github.com/microsoft/vcpkg.git $vcpkgDir
}
Show-Progress "Bootstrapping vcpkg"
& "$vcpkgDir\bootstrap-vcpkg.bat"

# Step 2: Install GLFW (static) using vcpkg
Show-Progress "Installing GLFW (static) using vcpkg"
& "$vcpkgDir\vcpkg.exe" install glfw3:x64-windows-static

# Step 3: Download and build GLAD if not already set up
if (Test-GladFiles) {
    Show-Progress "GLAD is already set up"
} else {
    if (-Not (Test-Path -Path $gladZipFile)) {
        Show-Progress "Downloading GLAD"
        Invoke-WebRequest -Uri $gladZipUrl -OutFile $gladZipFile
    }
    if (-Not (Test-Path -Path $gladExtractDir)) {
        Show-Progress "Extracting GLAD"
        Expand-Archive -Path $gladZipFile -DestinationPath .
    }

    Show-Progress "Generating GLAD project with CMake"
    $gladBuildDir = "$gladExtractDir\build"
    if (-Not (Test-Path -Path $gladBuildDir)) { New-Item -ItemType Directory -Path $gladBuildDir }
    cmake -B $gladBuildDir -S $gladExtractDir -G "Visual Studio 17 2022" `
        -DGLAD_PROFILE="core" -DGLAD_API="gl=4.6" -DGLAD_GENERATOR="c" -DBUILD_SHARED_LIBS=OFF

    Show-Progress "Building GLAD"
    & "C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" `
        "$gladBuildDir\ALL_BUILD.vcxproj" /p:Configuration=Release

    Show-Progress "Copying GLAD build files"
    if (-Not (Test-Path "$gladDestDir\include\glad")) { New-Item -ItemType Directory -Path "$gladDestDir\include\glad" -Force }
    if (-Not (Test-Path "$gladDestDir\include\KHR")) { New-Item -ItemType Directory -Path "$gladDestDir\include\KHR" -Force }
    if (-Not (Test-Path "$gladDestDir\src")) { New-Item -ItemType Directory -Path "$gladDestDir\src" -Force }

    Copy-Item -Path "$gladBuildDir\include\glad\glad.h" -Destination "$gladDestDir\include\glad" -Force
    Copy-Item -Path "$gladBuildDir\include\KHR\khrplatform.h" -Destination "$gladDestDir\include\KHR" -Force
    Copy-Item -Path "$gladBuildDir\src\glad.c" -Destination "$gladDestDir\src" -Force

    # Clean up
    Remove-Item -Recurse -Force $gladExtractDir
    Remove-Item -Force $gladZipFile
}

# Step 4: Build your project
Show-Progress "Configuring and building the project"
cmake -B $buildDir -S . -DCMAKE_TOOLCHAIN_FILE="$vcpkgDir\$vcpkgToolchain" -DCMAKE_BUILD_TYPE=Release
cmake --build $buildDir --config Release

Show-Progress "Build process completed successfully!"
