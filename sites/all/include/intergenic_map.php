<?php

include_once("ecoFunction.php");
include_once("dblink.php");
global $map_magnify;
	
function intergenic_map($tmpfname, $map, $map_magnify, $eg_id, $add_left_end)
{

	global $im, $map_magnify;
	$im = imagecreatetruecolor(800, 135)
		or die ("Cannot Initialize new GD image stream");
	
	$map = array();	
	$gene_name = drawGenePicture($im,$eg_id,$map);
	
	Imagepng($im,$tmpfname);
	ImageDestroy($im);		

}


?>