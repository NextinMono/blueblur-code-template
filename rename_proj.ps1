param(
    [Parameter(Mandatory=$true)]
    [string]$NewName
)

$OLD_NAME = "TemplateMod"
$EXTENSIONS = @(".sln", ".vcxproj", ".vcxproj.filters", ".vcxproj.user", ".props")
$SKIP_DIRS = @(".vs", "bin", "obj", "build")

function Replace-InFile($path, $old, $new) {
    $content = Get-Content $path -Raw -Encoding UTF8
    if ($content -notlike "*$old*") { return }
    $content.Replace($old, $new) | Set-Content $path -Encoding UTF8 -NoNewline
    Write-Host "Updated:        $path"
}

function Rename-All($old, $new) {
    $allItems = Get-ChildItem -Path "." -Recurse -Force | Where-Object {
        $parts = $_.FullName.Split([IO.Path]::DirectorySeparatorChar)
        -not ($parts | Where-Object { $SKIP_DIRS -contains $_ })
    } | Sort-Object { $_.FullName.Length } -Descending

    foreach ($item in $allItems) {
        if ($item.PSIsContainer) {
            if ($item.Name -ieq $OLD_NAME) {
                $newPath = Join-Path $item.Parent.FullName $new
                Rename-Item -Path $item.FullName -NewName $new
                Write-Host "Renamed folder: $($item.FullName) -> $newPath"
            }
        } else {
            if ($EXTENSIONS -contains $item.Extension) {
                Replace-InFile $item.FullName $old $new
            }
            if ($item.Name -like "*$old*") {
                $newName = $item.Name.Replace($old, $new)
                Rename-Item -Path $item.FullName -NewName $newName
                Write-Host "Renamed file:   $($item.Name) -> $newName"
            }
        }
    }
}

Rename-All $OLD_NAME $NewName