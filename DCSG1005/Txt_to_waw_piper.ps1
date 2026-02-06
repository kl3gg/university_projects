# ============================================
# Purpose:   Converts all .txt files basepath as to .wav
#            to use with Piper tts (https://github.com/rhasspy/piper)
#
# ============================================

<#
.SYNOPSIS
Converts all .txt files in BasePath to .wav using Piper TTS.

.DESCRIPTION
The script goes through the folder the file is located in and looks for .txt files,
reads the contents of each file and sends the text to Piper to generate a .wav file
with the same filename in the same folder.

The script supports -WhatIf and -Confirm via ShouldProcess (CmdletBinding).
If Piper returns an error code, an error is logged and the status is set to "Failed".

.PARAMETER BasePath
The folder to scan for .txt files. Default is $PSScriptRoot.

.PARAMETER PiperExe
Path to piper.exe in system.

.PARAMETER Model
Path to the Piper ONNX model (language model) to be used in Piper.

.PARAMETER Config
Path to the JSON configuration file for the selected ONNX model.

.EXAMPLE
.\tts-script.ps1
Runs in the folder where the script is located and converts all .txt files to .wav

.EXAMPLE
.\tts-script.ps1 -BasePath "[full path]" -WhatIf
Simulates the execution of the script in the specified folder without creating .wav files.
#>

                            # Aktiverer -WhatIf og -Confirm
[CmdletBinding(SupportsShouldProcess = $true, ConfirmImpact = 'Medium')]
param(
    [Parameter(Mandatory = $false)]
    [ValidateNotNullOrEmpty()]
    [string]$BasePath = $PSScriptRoot,

    [Parameter(Mandatory = $false)]
    [ValidateNotNullOrEmpty()]
    [string]$PiperExe = "[path to piper exe]",

    [Parameter(Mandatory = $false)]
    [ValidateNotNullOrEmpty()]
    [string]$Model = "[path to ONNX model]",

    [Parameter(Mandatory = $false)]
    [ValidateNotNullOrEmpty()]
    [string]$Config = "[path to JSON]"

)

# Retrieves all .txt files in BasePath
$getChildItemParams= @{
    Path        = $BasePath
    Filter      = '*.txt'
    File        = $true
    ErrorAction = 'Stop'
}

$results =
    Get-ChildItem @getChildItemParams |
        Where-Object { $_.Length -gt 0 } | # Hopper over tomme .txt filer
        ForEach-Object {
            $inputFile = $_.FullName
            $outputFile = Join-Path -Path $_.DirectoryName -ChildPath ($_.BaseName + '.wav')
            $status = 'Skipped'
            $exitCode = $null

            if ($PSCmdlet.ShouldProcess($outputFile, "Generate WAV from $inputFile")) {
                Get-Content -Path $inputFile -Encoding UTF8 -Raw |
                    & $PiperExe -m $Model -c $Config -f $outputFile

                $exitCode = $LASTEXITCODE  # Checking if Piper ran without errors
                if ($exitCode -eq 0) {
                    $status = 'OK'
                }  # If Piper returns an error code
                else {
                    $status = 'Failed'
                    Write-Error ("Piper failed on {0} (exitcode {1})" -f $_.BaseName, $exitCode)
                }
            }
            # Returns an object with the result for each file
            [pscustomobject]@{
                Name      = $_.BaseName
                InputTxt  = $inputFile
                Status    = $status
            }
        }
# Creates a table with the status code for each file
$results |
    Select-Object -Property Name, Status |
    Format-Table -AutoSize
