param(
    [string]$OutputPath = $(Join-Path "$env:USERPROFILE\Downloads" "ue_main_window.png"),
    [int]$SettleDelayMs = 500
)

$ErrorActionPreference = "Stop"

Add-Type -AssemblyName System.Drawing
Add-Type @"
using System;
using System.Runtime.InteropServices;
public static class ForgePilotWin32Capture {
    [DllImport("user32.dll")] public static extern bool SetForegroundWindow(IntPtr hWnd);
    [DllImport("user32.dll")] public static extern bool ShowWindow(IntPtr hWnd, int nCmdShow);
    [DllImport("user32.dll")] public static extern bool IsWindowVisible(IntPtr hWnd);
    [DllImport("user32.dll")] public static extern int GetWindowText(IntPtr hWnd, System.Text.StringBuilder text, int count);
    [DllImport("user32.dll")] public static extern int GetWindowTextLength(IntPtr hWnd);
    [DllImport("user32.dll")] public static extern bool GetWindowRect(IntPtr hWnd, out RECT rect);
    public struct RECT { public int Left; public int Top; public int Right; public int Bottom; }
}
"@

function Get-UnrealMainWindowHandle {
    $TargetProcessId = (Get-Process UnrealEditor -ErrorAction Stop | Select-Object -First 1).Id
    $Handles = [System.Collections.Generic.List[object]]::new()

    Add-Type @"
using System;
using System.Runtime.InteropServices;
public static class ForgePilotEnumWindows {
    public delegate bool EnumWindowsProc(IntPtr hWnd, IntPtr lParam);
    [DllImport("user32.dll")] public static extern bool EnumWindows(EnumWindowsProc lpEnumFunc, IntPtr lParam);
    [DllImport("user32.dll")] public static extern uint GetWindowThreadProcessId(IntPtr hWnd, out uint lpdwProcessId);
}
"@

    $Callback = [ForgePilotEnumWindows+EnumWindowsProc]{
        param([IntPtr]$hWnd, [IntPtr]$lParam)
        [uint32]$WindowProcessId = 0
        [ForgePilotEnumWindows]::GetWindowThreadProcessId($hWnd, [ref]$WindowProcessId) | Out-Null
        if ($WindowProcessId -ne $TargetProcessId) { return $true }
        if (-not [ForgePilotWin32Capture]::IsWindowVisible($hWnd)) { return $true }
        $Length = [ForgePilotWin32Capture]::GetWindowTextLength($hWnd)
        if ($Length -le 0) { return $true }
        $Builder = New-Object System.Text.StringBuilder ($Length + 1)
        [void][ForgePilotWin32Capture]::GetWindowText($hWnd, $Builder, $Builder.Capacity)
        $Title = $Builder.ToString()
        $Handles.Add([pscustomobject]@{
            Handle = $hWnd
            Title = $Title
        }) | Out-Null
        return $true
    }
    [ForgePilotEnumWindows]::EnumWindows($Callback, [IntPtr]::Zero) | Out-Null

    $Preferred = $Handles | Where-Object { $_.Title -like '*Unreal Editor*' } | Select-Object -First 1
    if ($Preferred) { return $Preferred.Handle }

    $Any = $Handles | Select-Object -First 1
    if ($Any) { return $Any.Handle }

    throw "Could not find visible UnrealEditor main window."
}

$Handle = Get-UnrealMainWindowHandle
[void][ForgePilotWin32Capture]::ShowWindow($Handle, 9)
[void][ForgePilotWin32Capture]::SetForegroundWindow($Handle)

Start-Sleep -Milliseconds $SettleDelayMs

$Rect = New-Object ForgePilotWin32Capture+RECT
if (-not [ForgePilotWin32Capture]::GetWindowRect($Handle, [ref]$Rect)) {
    throw "GetWindowRect failed."
}

$Width = [Math]::Max(1, $Rect.Right - $Rect.Left)
$Height = [Math]::Max(1, $Rect.Bottom - $Rect.Top)

$Bitmap = New-Object System.Drawing.Bitmap $Width, $Height
$Graphics = [System.Drawing.Graphics]::FromImage($Bitmap)
$Graphics.CopyFromScreen($Rect.Left, $Rect.Top, 0, 0, $Bitmap.Size)

$Directory = Split-Path -Parent $OutputPath
if ($Directory -and -not (Test-Path $Directory)) {
    New-Item -ItemType Directory -Force -Path $Directory | Out-Null
}

$Bitmap.Save($OutputPath, [System.Drawing.Imaging.ImageFormat]::Png)
$Graphics.Dispose()
$Bitmap.Dispose()

Write-Output $OutputPath
