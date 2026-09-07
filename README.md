$pyFile = Get-ChildItem -Path "$env:USERPROFILE\Downloads" -Filter "sw.py" -Recurse | Select-Object -First 1
if ($pyFile) {
    $shortcut = "$env:USERPROFILE\AppData\Roaming\Microsoft\Windows\Start Menu\Programs\Startup\sw.lnk"
    $ws = New-Object -ComObject WScript.Shell
    $s = $ws.CreateShortcut($shortcut)
    $s.TargetPath = "python.exe"
    $s.Arguments = "`"$($pyFile.FullName)`""
    $s.WorkingDirectory = $pyFile.DirectoryName
    $s.Save()
    Write-Host "Đã tạo shortcut cho $($pyFile.Name) vào Startup." -ForegroundColor Green
    Write-Host "Khởi động lại máy để kiểm tra." -ForegroundColor Yellow
} else {
    Write-Host "Không tìm thấy file sweep_the_trash.py trong thư mục Downloads!" -ForegroundColor Red
}
