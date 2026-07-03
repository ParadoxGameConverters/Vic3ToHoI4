$files = get-childitem -path "."
Foreach ($item in $files){
	if ($item.name -eq "rename.ps1")
	{
		continue
	}
	if ($item.name -eq "medium")
	{
		continue
	}
	if ($item.name -eq "small")
	{
		continue
	}
	$parts = $item.name -split '_'
	
	$new_name = $parts[0]
	if ($parts[1] -eq "communist")
	{
		$new_name += "_communism.tga" 
	}
	elseif ($parts[1] -eq "democratic")
	{
		$new_name += "_democratic.tga" 
	}
	elseif ($parts[1] -eq "fascist")
	{
		$new_name += "_fascism.tga" 
	}
	elseif ($parts[1] -eq "nonaligned")
	{
		$new_name += "_neutrality.tga" 
	}
	else
	{
		write $parts[1]
	}
	Rename-item -path "$item" -newname "$new_name"
}