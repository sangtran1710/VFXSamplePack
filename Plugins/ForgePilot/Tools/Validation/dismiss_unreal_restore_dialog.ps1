param(
    [int]$SettleDelayMs = 300
)

$ErrorActionPreference = "Stop"

Add-Type @"
using System;
using System.Runtime.InteropServices;
public static class ForgePilotRestoreWin32 {
    [DllImport("user32.dll")] public static extern bool EnumWindows(EnumWindowsProc lpEnumFunc, IntPtr lParam);
    public delegate bool EnumWindowsProc(IntPtr hWnd, IntPtr lParam);
    [DllImport("user32.dll")] public static extern int GetWindowText(IntPtr hWnd, System.Text.StringBuilder text, int count);
    [DllImport("user32.dll")] public static extern int GetWindowTextLength(IntPtr hWnd);
    [DllImport("user32.dll")] public static extern bool IsWindowVisible(IntPtr hWnd);
    [DllImport("user32.dll")] public static extern bool SetForegroundWindow(IntPtr hWnd);
    [DllImport("user32.dll")] public static extern bool ShowWindow(IntPtr hWnd, int nCmdShow);
}
"@

$Windows = [System.Collections.Generic.List[object]]::new()
$Callback = [ForgePilotRestoreWin32+EnumWindowsProc]{
    param([IntPtr]$hWnd, [IntPtr]$lParam)
    if (-not [ForgePilotRestoreWin32]::IsWindowVisible($hWnd)) { return $true }
    $Length = [ForgePilotRestoreWin32]::GetWindowTextLength($hWnd)
    if ($Length -le 0) { return $true }
    $Builder = New-Object System.Text.StringBuilder ($Length + 1)
    [void][ForgePilotRestoreWin32]::GetWindowText($hWnd, $Builder, $Builder.Capacity)
    $Title = $Builder.ToString()
    if ($Title -like '*Restore Packages*') {
        $Windows.Add([pscustomobject]@{ Handle = $hWnd; Title = $Title }) | Out-Null
    }
    return $true
}
[ForgePilotRestoreWin32]::EnumWindows($Callback, [IntPtr]::Zero) | Out-Null

if ($Windows.Count -eq 0) {
    Write-Output "No Restore Packages dialog found."
    exit 0
}

$Dialog = $Windows[0]
[void][ForgePilotRestoreWin32]::ShowWindow($Dialog.Handle, 9)
[void][ForgePilotRestoreWin32]::SetForegroundWindow($Dialog.Handle)
Start-Sleep -Milliseconds $SettleDelayMs

$Shell = New-Object -ComObject WScript.Shell
$Shell.SendKeys("{TAB}")
Start-Sleep -Milliseconds 100
$Shell.SendKeys("{TAB}")
Start-Sleep -Milliseconds 100
$Shell.SendKeys("{ENTER}")

Write-Output "Dismissed Restore Packages dialog."
