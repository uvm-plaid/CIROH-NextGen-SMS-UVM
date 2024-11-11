<?php
// Source: https://stackoverflow.com/questions/3785055/how-can-i-create-a-simple-index-html-file-which-lists-all-files-directories 
// List all the files in the current directory which are not this file, the
// directory itself, or the parent directory
$path = ".";
$dh = opendir($path);
$i=1;
while (($file = readdir($dh)) !== false) {
    if ($file != "." 
        && $file != ".." 
        && $file != "index.php" 
    ) {
        echo "<a href='$path/$file'>$file</a><br /><br />";
        $i++;
    }
}
closedir($dh);
?>
