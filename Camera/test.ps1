function Test-Input {
    param (
        [string]$value
    )

    if (-not $value) {
        throw "The input value cannot be null or empty."
    }

    Write-Host "Valid input: $value" -ForegroundColor Yellow
}

try {
    Write-Host "Starting process..." -ForegroundColor Green

    # Call the function that might throw an error
    Test-Input -value ""

    # Additional code that won't be executed if Test-Input throws an error
    Write-Host "This line will not be reached if an error occurs." -ForegroundColor Green

} catch {
    Write-Error "A critical error occurred: $_"
    # Additional error handling if needed
    return  # Exit the script with a non-zero status
}

Write-Host "Process completed."