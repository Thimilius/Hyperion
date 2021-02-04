$base_path = Resolve-Path ($PSScriptRoot + "/../projects/hyperion")

$loc_include = (gci $base_path/include -include *.hpp,*.cpp -recurse | select-string .).Count
$loc_src = (gci $base_path/source -include *.hpp,*.cpp -recurse | select-string .).Count
$loc_all = $loc_include + $loc_src

Write-Host "Lines of code: " -NoNewLine
Write-Host $loc_all -ForegroundColor Yellow

Write-Host "    - include: " -NoNewLine
Write-Host $loc_include -ForegroundColor Yellow

Write-Host "    - src:     " -NoNewLine
Write-Host $loc_src -ForegroundColor Yellow