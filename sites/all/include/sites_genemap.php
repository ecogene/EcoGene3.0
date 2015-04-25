<?php

include_once('ecopngmap.php');
include_once("dblink.php");

//function map_sites($sites, $map_magnify, $left_end, $add_left_end, $tmpfname, $map0fmap)
function sites_map($tmpfname, $sites, $map_magnify,$left_end,&$map0fmap)
// Description: 
//				boolean map_sites(string $sites, int $map_magnify, int $left_end, int $add_left_end, $tmpfname, $area_map) 			
//				takes the input, generate the map image and its area map, save the image to temp file,
// 
//  Parameters: 
//				$sites:  the names of the sites selected.
//				$map_magnify: magnification of the site map. 
//				$left_end: left end of the map (in bit)
//				$add_left_end: additiaon information of the left end .
//				$tmpfname: the file name of the generated site map image
//				$area_map: the area map of the generated site map image
//	
//	Returen values: 	
//				returns ture when success, otherwise false
{
	global $GENOME_LENGTH;
	
	$link=dblink();
	$ecogene_db = "ecogene";
	$mapsearch_db = "mapsearch_db";
	
	
	$searchegs = "";
	$siteA = array();
	if ($sites != "") {
		$siteA = explode(',',$sites);
	}
	foreach($siteA as $value) {
		$searchegs = $searchegs."'".trim($value)."',";
	}
	//remove "," of last item
	$searchegs = strrev(substr(strrev($searchegs),1));

	if($searchegs=='')
		return false;
	
	$gene_neighbor = intval(10000/pow(2,$map_magnify-1));		
//	if(empty($add_left_end))
	{
		
		$left_end =  ($left_end-$gene_neighbor/2+$GENOME_LENGTH)%$GENOME_LENGTH;
		$right_end = ($left_end+$gene_neighbor+$GENOME_LENGTH)%$GENOME_LENGTH;
		
	}
//	else
//	{
//
//		$left_end = ($add_left_end-$gene_neighbor/2+$GENOME_LENGTH)%$GENOME_LENGTH;
//		$right_end = ($add_left_end+$gene_neighbor+$GENOME_LENGTH)%$GENOME_LENGTH;
//	
//	}

	if($left_end<=$right_end)
	{
		$query = "Select substring(s.sequence,".$left_end.",".($right_end-$left_end).") as sequence from t_sequence s";
	}
	else 
	{
		$query = "Select concat(substring(s.sequence,".$left_end.",".($GENOME_LENGTH-$left_end)."), substring(s.sequence,1,".($right_end).")) as sequence from t_sequence s";
	}

	mysql_select_db($ecogene_db) or die( "Could not select database");
	$result = mysql_query($query) or die("Query failed : " . mysql_error());
	$sequence = mysql_result($result,0, 'sequence');

//	mysql_select_db($mapsearch_db) or die( "Could not select database");
	$query = "Select name, sequence, seq,  is_symmetrical FROM ".$mapsearch_db.".t_enzyme WHERE  name in (".$searchegs.") ORDER by name ASC";
	$result = mysql_query($query) or die("Query failed : " . mysql_error());
	$number = mysql_num_rows($result);

	//	map information 
	 $left_margin = 0;
     $right_margin = 80;
     $top_margin = 7;
     $bottom_margin = 0;
     
     $space_line=15;

     $img_width = 830;
     $img_high =  $top_margin + $bottom_margin + $number*$space_line; 
     $fontfile = "sites/all/include/verdana.ttf"; 
     $text_size = 8;
     $line_size = 11;
     
     $top_margin = $line_size + 2;
     
	 $img = imagecreatetruecolor($img_width, $img_high)
			or die ("Cannot Initialize new GD image stream");
			
	$map0fmap = array();
	$background_color = imagecolorallocate ($img,  211, 226, 234);
	$background_color = imagecolorallocate ($img,  255, 255, 255);
	imagefill($img , 0,0, $background_color);
	
 	$map = new ecopngmap($img, $left_margin, $right_margin, $top_margin,  $bottom_margin, $left_end, $right_end, $img_width,$img_high);
 	
 	
 	$i = 0;
 	$t = 0;
 	$m = 0;
 	$n = 0;
 	
 	//color for clockwise gene
	//*******************************************
	$line_color_cw = imagecolorallocate($img,30,144,255);
	//color for counterclockwise gene
	//*******************************************
	$line_color_ccw = imagecolorallocate($img,0, 0, 255);
	
 	 While ($i < $number) {
	// 	 	$elem = new elemofmap($line_color_cw,  $left_end, $right_end, '', 1, -$line_size/2, $space_line, $i); 	 	
	// 	 	$map->draw_element($map->img, $elem);
	// 	 	$elem = new elemofmap($line_color_ccw,  $left_end, $right_end, '', 1, $line_size/2+1, $space_line, $i); 	 		
		$elem = new elemofmap($line_color_cw,  $left_end, $right_end, '', 1, $line_size/2+1, $space_line, $i); 	 
 	 	$map->draw_element($map->img, $elem);
 	 	$i++;
 	 }
 	 $i=0;
 While ($i < $number) {
 	
 	$enzyme_name = mysql_result($result,$i,'name');
	$pattern_disply = mysql_result($result,$i,'sequence');
	$is_symmetrical = mysql_result($result,$i, 'is_symmetrical');
	$pattern = trim(mysql_result($result,$i, 'seq'));
	
	
	$color = imagecolorallocate($img, rand(0,255),rand(0,255),rand(0,255));
	
	$text[$m] = new textofmap($color, ($map->imx)-($map->right_margin)+5, $map->top_margin+$i*$space_line+$text_size/2, 0, $enzyme_name, $text_size,$fontfile);
//	$text[$m] = new textofmap($color, ($map->imx)-($map->right_margin)+5, $map->top_margin+$i*$space_line+$text_size/2, 0, $enzyme_name." ".$pattern, $text_size,$fontfile);
	
	$map0fmap[$n] = new mapofmap($text[$m]->y-$text[$m]->size/2, $text[$m]->y+$text[$m]->size/2, $text[$m]->x, $text[$m]->x+$text[$m]->size*strlen($text[$m]->string),"http://rebase.neb.com/rebase/enz/".$enzyme_name.".html",'Reference from REBASE');	
	
	$n++;
	$m++;
	$map->drawtext($text[$i]);
	
	$detected = search_site($sequence, $pattern, $is_symmetrical);
	
//	echo "\$enzyme_name ".$enzyme_name."<br>";
 	for($k=0; $k<count($detected); $k++)
 	{
 //		 echo "\$count  ".count($detected)."<br>";
 		if($detected[$k]['ori']=='')
 		{
 			$elems[$t] = new elemofmap($color,  ($detected[$k]['pos']+$map->left_end)%$GENOME_LENGTH, ($map->left_end+$detected[$k]['pos'])%$GENOME_LENGTH, $detected[$k]['ori'], $line_size-2, 0, $space_line, $i);
 		}
 		else 
 		{
 				$elems[$t] = new elemofmap($color,  ($detected[$k]['pos']+$map->left_end)%$GENOME_LENGTH, ($map->left_end+$detected[$k]['pos'])%$GENOME_LENGTH, $detected[$k]['ori'], ceil($line_size/2), -1, $space_line, $i);
 		}
// 		print_r($elems);
 		$pos = $elems[$t]->getmapposition($map);
 		
 		$map0fmap[$n] = new mapofmap($pos['top'],$pos['bottom'],$pos['left'],$pos['right'],"",($left_end+$detected[$k]['pos'])%$GENOME_LENGTH." ".$detected[$k]['seq']);	
 //		echo "\$elems  ".print_r($elems[$t])."<br>";
 		$map->draw_element($map->img, $elems[$t]);
 		$t++;
 		$n++;
 	}
	$i++;
 }
		
// 	
	Imagepng($img,$tmpfname);
	ImageDestroy($img);
	return true;
	
}
?>


