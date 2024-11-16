# Define parameters for build, vcpkg, glad, and glfw directories relative to the script location
param(
    [string]$buildDir = ".\build",
    [string]$vcpkgDir = ".\vcpkg",
    [string]$gladZipUrl = "https://github.com/Dav1dde/glad/archive/refs/heads/master.zip",
    [string]$gladZipFile = ".\glad.zip",
    [string]$gladExtractDir = ".\glad-master",
    [string]$gladDestDir = ".\thirdParty\glad",
    [string]$glfwDestDir = ".\thirdParty\glfw",
    [string]$vcpkgToolchain = "scripts/buildsystems/vcpkg.cmake"
)

# Function to display progress messages
function Show-Progress {
    param(
        [string]$message
    )
    Write-Host "=== $message ==="
}

# Function to test if the necessary GLAD files exist
function Test-GladFiles {
    if ((Test-Path "$gladDestDir\include\glad\glad.h") -and 
        (Test-Path "$gladDestDir\include\KHR\khrplatform.h") -and 
        (Test-Path "$gladDestDir\src\glad.c")) {
        return $true
    } else {
        return $false
    }
}

# Function to test if the necessary GLFW files exist
function Test-GlfwFiles {
    if ((Test-Path "$glfwDestDir\include\GLFW\glfw3.h") -and 
        (Test-Path "$glfwDestDir\lib\glfw3.lib")) {
        return $true
    } else {
        return $false
    }
}

# Step 1: Check if vcpkg is already cloned
if (-Not (Test-Path -Path $vcpkgDir)) {
    Show-Progress "Cloning vcpkg"
    git clone https://github.com/microsoft/vcpkg.git $vcpkgDir
} else {
    Show-Progress "vcpkg already exists"
}

# Step 2: Bootstrap vcpkg
Show-Progress "Bootstrapping vcpkg"
& "$vcpkgDir\bootstrap-vcpkg.bat"

# Step 3: Install GLFW using vcpkg if not already set up
if (Test-GlfwFiles) {
    Show-Progress "GLFW is already set up in thirdParty/glfw"
} else {
    Show-Progress "Installing GLFW using vcpkg"
    & "$vcpkgDir\vcpkg.exe" install glfw3:x64-windows

    # Copy necessary files to thirdParty/glfw
    Show-Progress "Copying GLFW files"
    if (-Not (Test-Path "$glfwDestDir\include\GLFW")) {
        New-Item -ItemType Directory -Path "$glfwDestDir\include\GLFW" -Force
    }
    if (-Not (Test-Path "$glfwDestDir\lib")) {
        New-Item -ItemType Directory -Path "$glfwDestDir\lib" -Force
    }
    Copy-Item -Path "$vcpkgDir\installed\x64-windows\include\GLFW\glfw3.h" -Destination "$glfwDestDir\include\GLFW" -Force
    Copy-Item -Path "$vcpkgDir\installed\x64-windows\lib\glfw3.lib" -Destination "$glfwDestDir\lib" -Force
}

# Step 4: Check if GLAD is already set up
if (Test-GladFiles) {
    Show-Progress "GLAD is already set up in thirdParty/glad"
} else {
    # Step 4.1: Download GLAD zip file if it doesn't exist
    if (-Not (Test-Path -Path $gladZipFile)) {
        Show-Progress "Downloading GLAD from GitHub"
        Invoke-WebRequest -Uri $gladZipUrl -OutFile $gladZipFile
    }

    # Step 4.2: Extract the GLAD zip file
    if (-Not (Test-Path -Path $gladExtractDir)) {
        Show-Progress "Extracting GLAD"
        Expand-Archive -Path $gladZipFile -DestinationPath .
    }

    # Step 4.3: Generate Visual Studio 2022 project files using CMake
    Show-Progress "Generating GLAD project with CMake"
    $gladBuildDir = "$gladExtractDir\build"
    if (-Not (Test-Path -Path $gladBuildDir)) {
        New-Item -ItemType Directory -Path $gladBuildDir
    }
    cmake -B $gladBuildDir -S $gladExtractDir -G "Visual Studio 17 2022" `
        -DGLAD_PROFILE="core" `
        -DGLAD_API="gl=4.6" `
        -DGLAD_GENERATOR="c" `
        -DBUILD_SHARED_LIBS=OFF `
        -DGLAD_INSTALL=ON

    # Step 4.4: Build the GLAD project using MSBuild (VS2022)
    Show-Progress "Building GLAD with MSBuild"
    & "C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" `
        "$gladBuildDir\ALL_BUILD.vcxproj" /p:Configuration=Release

    # Step 4.5: Copy necessary files to thirdParty/glad
    Show-Progress "Copying GLAD files to thirdParty"
    if (-Not (Test-Path "$gladDestDir\include\glad")) {
        New-Item -ItemType Directory -Path "$gladDestDir\include\glad" -Force
    }
    if (-Not (Test-Path "$gladDestDir\include\KHR")) {
        New-Item -ItemType Directory -Path "$gladDestDir\include\KHR" -Force
    }
    if (-Not (Test-Path "$gladDestDir\src")) {
        New-Item -ItemType Directory -Path "$gladDestDir\src" -Force
    }

    Copy-Item -Path "$gladExtractDir\include\glad\glad.h" -Destination "$gladDestDir\include\glad" -Force
    Copy-Item -Path "$gladExtractDir\include\KHR\khrplatform.h" -Destination "$gladDestDir\include\KHR" -Force
    Copy-Item -Path "$gladExtractDir\src\glad.c" -Destination "$gladDestDir\src" -Force

    # Clean up
    Remove-Item -Recurse -Force $gladExtractDir
    Remove-Item -Force $gladZipFile
}

# Step 5: Create a build directory if it doesn't exist
if (-Not (Test-Path -Path $buildDir)) {
    Show-Progress "Creating build directory"
    New-Item -ItemType Directory -Path $buildDir
}

# Step 6: Configure the project with CMake
Show-Progress "Configuring CMake"
cmake -B $buildDir -S . -DCMAKE_TOOLCHAIN_FILE="$vcpkgDir\$vcpkgToolchain" -DCMAKE_BUILD_TYPE=Release

# Step 7: Build the project
Show-Progress "Building the project"
cmake --build $buildDir --config Release

Show-Progress "Build process completed successfully!"
