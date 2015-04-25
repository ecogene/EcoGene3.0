<?PHP
include_once('ecoFunction.php');
include_once("dblink.php");

function genepage_igr_map($left,$right,$eg_id, $url, $weak_site=1)
{

	
 if(stristr($url, 'weak_site=1'))
 {
 	$weak_site = 1;
 }elseif (stristr($url, 'weak_site=0'))
 {
 	$weak_site = 0;
 }
	

$link=dblink();
mysql_select_db('ecogene') or die('Unable to select database');
	
	$map_content = '';
global $base_url;
global $text_size,$line_size, $text_size_2, $fontfile,$title_color,$text_color,$background_color;
global $left_margin, $right_margin;
global $left_end_pos, $right_end_pos;
global $overlap_dis;
global $line_color_cw;
global $line_color_ccw;
global $num_tfbs;
global $fontfile;
$overlap_dis = 10;;
$text_size = 12;
$text_size_2 = 8;
$fontfile = "sites/all/include/verdana.ttf";

$left_end_pos = $left;

/******************************************************************

Set the left and right margin of the map, where the genes and tfbs
are shown in between, and the margins are used to show the ruler and
the position of the right and left most position on the sequence.

******************************************************************/
$left_margin = 10;
$right_margin = 60;
$image_width = 830;
/******************************************************************

The size of the region of the sequence which will be shown in the 
map

******************************************************************/
$map_range = 700;


/******************************************************************

Get the left end and right end of the map from the mean of the tfbs
and the range of the map is set 600 bits

******************************************************************/
//$query = "select distinct ( g.name ) as name, g.EG_tag as tag  from t_tfbs g , t_address address  where g.address_id = address.address_id and address.left_end >= $left  
//          and address.right_end  <= $right ";

$query = "select distinct ( g.name ) as name, g.EG_tag as tag  from t_tfbs g   
			where ( 	(g.left_end >= $left  and g.left_end  <= $right )
						or
						(g.right_end >= $left  and g.right_end  <= $right )	
					)
		";
if($weak_site==0)
{
	$query .= " and g.strong_evidence <> '' and g.strong_evidence is not null";
}
$result= mysql_query($query) or die("Query failed : " . mysql_error().$query);
$num_tfbs = mysql_num_rows($result);
$i = 0;
$gene_color = array();

while ($row = mysql_fetch_array($result, MYSQL_ASSOC))
{
	$gene_color[$i]['name'] = $row['name'];
	$gene_color[$i]['c_r'] =  rand(0,255);
	$gene_color[$i]['c_g'] =  rand(0,255);
	$gene_color[$i]['c_b'] =  rand(0,255);
	$gene_color[$i]['type'] = "";
	$gene_color[$i]['atl'] =  "Go to GenePage";
	
	if($row['tag']!="")
	{
		
		$items = explode (";",$row['tag']);
		
		$n_tag = count($items);
		
		if($n_tag==1)
		{
			
			$gene_color[$i]['eg_id'] =$row['tag'];
			$gene_color[$i]['link'] =$base_url."/?q=gene/".$gene_color[$i]['eg_id'];
			
			
		}
		else 
		{
			
			$gene_color[$i]['eg_id'] = $row['tag'];
			$gene_color[$i]['link'] =$base_url."/?q=ecosearch/gene/search&egid=".$gene_color[$i]['eg_id'];
		}
		
			$i++;
	}
	else 
	{
		$gene_color[$i]['link']='';
		$i++;
	}
	
	
	
}

mysql_free_result($result);

//$query = "select g.* from t_is_address_tmp g, t_address address where g.address_id = address.address_id 
//		 and address.left_end >= $left and address.right_end <= $right 	";

$query = "select g.* from t_is_address_tmp g, t_address address 
			where g.address_id = address.address_id
				and ( 	(address.left_end >= $left  and address.left_end  <= $right )
						or
						(address.right_end >= $left  and address.right_end  <= $right )	
					)
		";

$result= mysql_query($query) or die("Query failed : " . mysql_error().$query);

while ($row = mysql_fetch_array($result, MYSQL_ASSOC))
{
	$gene_color[$i]['name'] = $row['name'];
	$gene_color[$i]['c_r'] =  rand(0,255);
	$gene_color[$i]['c_g'] =  rand(0,255);
	$gene_color[$i]['c_b'] =  rand(0,255);
	$gene_color[$i]['type'] = 'igr';
	$gene_color[$i]['atl'] =  "Go to TopicPage";

	$id = $row['id'];
	$query = "select * from t_is_address where id= '$id'";
	$rst= mysql_query($query) or die("Query failed : " . mysql_error().$query);
	$row = mysql_fetch_array($rst, MYSQL_ASSOC);
	$gene_color[$i]['link'] = $base_url."/?q=topic/".$row['topic_id'];
	
	$i++;	
}


//$query = "select g.* from t_is_address g, t_address address where g.address_id = address.address_id 
//		 and address.left_end >= $left ".
//		"and address.right_end <= $right and g.is_type='IG' and (name2 not like 'REP%' and name2 not like 'IHF%')";

$query = "select g.name2, g.name, g.topic_id,g.id, address.*  from t_is_address g, t_address address where g.address_id = address.address_id 
		 and 
		 	 ( 	(address.left_end >= $left  and address.left_end  <= $right )
						or
						(address.right_end >= $left  and address.right_end  <= $right )	
					)
		 and g.is_type='IG' and (name2 not like 'REP%' and name2 not like 'IHF%')";

$result= mysql_query($query) or die("Query failed : " . mysql_error().$query);

while ($row = mysql_fetch_array($result, MYSQL_ASSOC))
{
	$gene_color[$i]['name'] = $row['name2'];
	$gene_color[$i]['c_r'] =  rand(0,255);
	$gene_color[$i]['c_g'] =  rand(0,255);
	$gene_color[$i]['c_b'] =  rand(0,255);
	$gene_color[$i]['type'] = 'igr';
	$gene_color[$i]['atl'] =  "Go to TopicPage";

	$id = $row['id'];
	$query = "select * from t_is_address where id= '$id'";
	$rst= mysql_query($query) or die("Query failed : " . mysql_error().$query);
	$row = mysql_fetch_array($rst, MYSQL_ASSOC);
	$gene_color[$i]['link'] = $base_url."/?q=topic/".$row['topic_id'];
	
	$i++;	
}
$num_diff_gene = $i;


mysql_free_result($result);

$im = imagecreatetruecolor($image_width, $num_diff_gene>2? $num_diff_gene*20+10:50)
or die ("Cannot Initialize new GD image stream");
$background_color = imagecolorallocate ($im,  255, 255, 255);	

$title_color = imagecolorallocate($im, 255,0,240);
$text_color = imagecolorallocate ($im, 23,43,0);
$map = array();	
imagefill($im , 0,0, $background_color);

/******************************************************************

Draw the title on the map and the legends, including the coloered
brick for different tfbs

******************************************************************/
$map=array();
draw_title($gene_color, $im, $map,$left_end_pos,$right_end_pos);

$tmpfname=file_directory_temp()."/regulon".$left_end_pos.$weak_site."title.png";

		Imagepng($im,$tmpfname);

		ImageDestroy($im);

$map_content =  "<MAP NAME=\"igr_maptitle\">";
$t=0;
foreach ($map as $map_t)
{
	
//	echo "<AREA NAME=\"area".$t."\" COORDS=\"".$map_t['x_1'].",".$map_t['y_1'].",".$map_t['x_2'].",".$map_t['y_2']."\" title=\"".$map_t['atl']."\" HREF=\"".$map_t['link']."\"".">";
	$map_content .= "<AREA NAME=\"area\" target=\"_top\" COORDS=\"".$map_t['x_1'].",".$map_t['y_1'].",".$map_t['x_2'].",".$map_t['y_2']."\" title=\"".$map_t['atl']."\" HREF=\"".$map_t['link']."\"".">";
	
	$t++;
}
$map_content .="</MAP>";

$map_content .= "<IMG src=\"".file_create_url($tmpfname)."\" usemap=\"#igr_maptitle\" border=0>";


$loop = 0;
for ($left_end_pos=$left; $left_end_pos<$right; $left_end_pos+=$map_range)
{
	$loop++;
//	$right_end_pos = $right<$left_end_pos+$map_range-1? $right:$left_end_pos+$map_range-1;
$right_end_pos = $left_end_pos+$map_range-1;


/******************************************************************

Get the individual tfbs in the region, and show them in multiple lines
so that they are not ovlapped.

******************************************************************/

$query = "select  g.evidence, g.name as name, g.EG_tag as EG_tag, g.operon_id as operon_id, g.left_end as left_end, g.right_end as right_end , g.orientation as orientation 
				from t_tfbs g  where ((g.left_end >= $left_end_pos and g.left_end <= $right_end_pos )
				 or (g.right_end <= $right_end_pos and g.right_end >= $left_end_pos ))";

if($weak_site==0)
{
	$query .= " and g.strong_evidence <> '' and g.strong_evidence is not null";
}
$result= mysql_query($query) or die("Query failed : " . mysql_error().$query);
$gene=array();
$i = 0;
$evidence_code = array();
while ($row = mysql_fetch_array($result, MYSQL_ASSOC))
{
	$gene[$i]['name'] = $row['name'];
//	$gene[$i]['EG_target'] = $row['EG_target'];
	$gene[$i]['operon_id'] = $row['operon_id'];
	$gene[$i]['EG_tag'] = $row['EG_tag'];
//	$gene[$i]['type'] = $row['type'];
	$gene[$i]['left_end'] = $row['left_end'];
	$gene[$i]['right_end'] = $row['right_end'];
	$gene[$i]['orientation'] = $row['orientation'];
	$gene[$i]['line'] = 0;
	$gene[$i]['type'] = 'NULL';
//	$gene[$i]['eg_id'] = $row['eg_id'];

	$tfbs_evidence = explode(',', $row['evidence']);
	$s_array = array();
	$w_array = array();
	foreach ($tfbs_evidence as $value) {
		$_e = explode('|', $value);
		foreach ($_e as $ind=>$v) {
			$_e[$ind] = trim($v, "[]");
		}
		if($_e[1] == 's' || $_e[1] == 'S')
		{
			$s_array[] = $_e[0].'|'.$_e[2];
		}elseif($_e[1] == 'w' || $_e[1] == 'W')
		{
			$w_array[] = $_e[0].'|'.$_e[2];
		}
	}
	$gene[$i]['strong_evidence'] = implode(', ', $s_array);
	$gene[$i]['weak_evidence'] = implode(', ', $w_array);
	$gene[$i]['type'] = 'tfbs';
	
	$i++;
	
}

//$medpoint = $i;
//$num_gene_line = gene_overlapped(&$gene, 0, $medpoint, 1);
mysql_free_result($result);

//$query = "select * from t_is_address where (name2 like 'REP%' or name2 like 'IHF%') and left_end >= $left_end_pos and right_end <= $right_end_pos ";
$query = "select g.name2, g.name, g.topic_id, address.* from t_is_address g, t_address address where g.address_id = address.address_id 
		 and ((address.left_end >= $left_end_pos and address.left_end <= $right_end_pos )".
		" or (address.right_end <= $right_end_pos and address.right_end >= $left_end_pos )) and  (g.is_type='IG')";

$result= mysql_query($query) or die("Query failed : " . mysql_error().$query);

while ($row = mysql_fetch_array($result, MYSQL_ASSOC))
{
	$gene[$i]['name'] = $row['name2'];	
	$gene[$i]['left_end'] = $row['left_end'];
	$gene[$i]['right_end'] = $row['right_end'];
	
	if((substr_compare($row['name2'],"REP", 0, 3)==1) || (substr_compare($row['name2'],"IHF", 0, 3)==1) )
		$gene[$i]['orientation'] = 'Nodirection';
	else 
		$gene[$i]['orientation'] = $row['orientation'];
		
	$gene[$i]['line'] = 0;
	$gene[$i]['type'] = 'igr';
	$gene[$i]['topic_id'] = $row['topic_id'];
	
//	$gene[$i]['EG_target'] = "";
	$gene[$i]['operon_id'] = "";
	$gene[$i]['EG_tag'] = "";

	$i++;
	
}

$num_gene = $i;
$num_gene_line = gene_overlapped($gene, 0, $num_gene, 1);

mysql_free_result($result);
if(($num_gene)<1)
{
	continue;
}

/******************************************************************

Initiate the map

******************************************************************/

$im = imagecreatetruecolor($image_width, $num_gene_line*30+80)
or die ("Cannot Initialize new GD image stream");
$map = array();
			
imagefill($im , 0,0, $background_color);
draw_kb($im, $left_end_pos,$right_end_pos);
/******************************************************************

Draw genes in the region

******************************************************************/

//color for clockwise gene
//*******************************************
$line_color_cw = imagecolorallocate($im,30,144,255); 
//color for counterclockwise gene
//*******************************************
$line_color_ccw = imagecolorallocate($im,0, 0, 255);
$map = array();
$start_map = sizeof($map);
$num_diff_gene = 0;
	$query = "select * from t_gene g, t_address address where g.address_id = address.address_id and (( address.left_end > $left_end_pos and  address.left_end < $right_end_pos) or (address.right_end > $left_end_pos and  address.right_end < $right_end_pos))";
	$rst_geneInfo = mysql_query($query) or die("Query failed : " . mysql_error());
	$flag = 0;
	$i = 0;
	while ($row = mysql_fetch_array($rst_geneInfo, MYSQL_ASSOC))
	{ 
		$status = isPseudogene($row['eg_id']);
		$num_gene=$num_gene+1;
		$imx = imagesx($im)-($left_margin+$right_margin);
		$imy = imagesy($im);
		
			$is_left_include2 = true;
			$is_right_include2 = true;
			$gene_left_end2 = $row['left_end'];
			$gene_right_end2 = $row['right_end'];
			if($row['left_end']<$left_end_pos){
				$gene_left_end2 =$left_end_pos;
				$is_left_include2 = false;
			}
	
			if($row['right_end']>$right_end_pos){
				$gene_right_end2 = $right_end_pos;
				$is_right_include2 = false;
			}

		$ims = imagesx($im) - ($left_margin+$right_margin);
		$pos_1 = $left_margin + ($gene_left_end2-$left_end_pos)/($right_end_pos-$left_end_pos)*$ims;
		$pos_2 = $left_margin + ($gene_right_end2-$left_end_pos)/($right_end_pos-$left_end_pos)*$ims;
		
		$line_size = 4;
		
		$line_y1 =  $num_diff_gene*20 + 55;
		$line_y2 =  $num_diff_gene*20 + 65;
		
		$flag = $flag+1;
		
		if(($flag%2)==0){
			drawOneGene($im, $pos_1,$line_y2,$pos_2,$line_y2,$row['name'], $row['orientation'], $line_size,$text_size_2,$is_left_include2,$is_right_include2,$row['type'], $status);
			
		}
		else {
			drawOneGene($im, $pos_1,$line_y1,$pos_2,$line_y1, $row['name'], $row['orientation'], $line_size,$text_size_2,$is_left_include2,$is_right_include2,$row['type'], $status);
			
		}
		$map[$i+$start_map]['x_1'] =   $pos_1;
		$map[$i+$start_map]['x_2'] =   $pos_2; 
		$map[$i+$start_map]['y_1'] =   $line_y1-10;
		$map[$i+$start_map]['y_2'] = 	$line_y2+10;
		$map[$i+$start_map]['atl'] = "Left End: ".$row['left_end']."  Right End: ".$row['right_end']; ;
		$map[$i+$start_map]['eg_id'] = $row['eg_id']; 
		$map[$i+$start_map]['link'] = $base_url."/?q=gene/".$row['eg_id']; 
		$i = $i+1;
	}
	
mysql_free_result($rst_geneInfo);
/******************************************************************

Draw tfbs in the region

******************************************************************/
draw_gene($gene, $im, $num_gene_line, $num_diff_gene,$gene_color,$map,$left_end_pos,$right_end_pos);

/******************************************************************

Create the picture and map

******************************************************************/

$tmpfname=file_directory_temp()."/regulon".$left_end_pos.$weak_site.".png";

		Imagepng($im,$tmpfname);

		ImageDestroy($im);

$map_content .= "<MAP NAME=\"igr_map$loop\">";
$t=0;

foreach ($map as $map_t)
{
	
	$map_content .= "<AREA NAME=\"area\" target=\"_top\" COORDS=\"".$map_t['x_1'].",".$map_t['y_1'].",".$map_t['x_2'].",".$map_t['y_2']."\" title=\"".$map_t['atl']."\" HREF=\"".$map_t['link']."\"".">";
//		echo "<AREA NAME=\"area".$t."\" COORDS=\"".$map_t['x_1'].",".$map_t['y_1'].",".$map_t['x_2'].",".$map_t['y_2']."\" title=\"".$map_t['atl']."\" HREF=\"".$map_t['link']."\"".">";
	
	$t++;
}
$map_content .="</MAP>";



$map_content .= "<IMG src=\"".file_create_url($tmpfname)."\" usemap=\"#igr_map$loop\" border=0>";
}
if(isset($eg_id))
{
$query = "select  address.left_end, address.right_end, address.orientation   from t_gene g, t_address address where g.address_id = address.address_id and g.eg_id = '".$eg_id."'";

$result= mysql_query($query) or die("Query failed : " . mysql_error().$query);
$row = mysql_fetch_array($result, MYSQL_ASSOC);
if($row['orientation']=='Clockwise')
{
	$us = ($row['left_end']-$left)>0? $row['left_end']-$left:0;
	$ds = ($row['right_end']-$right)<0? $right - $row['right_end']:0;
}
else 
{
	$us = ($row['right_end']-$right)<0? $right - $row['right_end']:0;
	$ds = ($row['left_end']-$left)>0? $row['left_end']-$left:0;
}
if ($weak_site == 1){	
	
	if(stristr($url, 'weak_site=1'))
 	{
 		$url = str_replace("weak_site=1", "weak_site=0", $url); 		
 	}else
 	{
 		$url .= "&weak_site=0";
 	} 
	$site_taggle = '<a class="button" href="'.$url.'">Hide Weak TFBSs</a>';
	
}else {
	
	if(stristr($url, 'weak_site=0'))
 	{
 		$url = str_replace("weak_site=0", "weak_site=1", $url); 		
 	}else
 	{
 		$url .= "&weak_site=1";
 	} 
	$site_taggle = '<a class="button" href="'.$url.'">Show Weak TFBSs</a>';	
}
$site_taggle .= "<a title='Classification of weak and strong evidence at RegulonDB' href='http://regulondb.ccg.unam.mx/menu/about_regulondb/evidence_classification/index.jsp'>
TFBS Classification</a>";
$map_content .= '<div><form name=dnaForm action="'.$base_url.'/?q=gene/'.$eg_id.'/dnasequence" target="_parent" method="post">
<input type="hidden" name="eg_id" value="'.$eg_id.'" size="3">
<input type="hidden" name="submit_tfbs" value="1">
<input type="hidden" name="type" value="tfbs">
 <input type="hidden" name="us" value="'.$us.'" >
<input type="hidden" name="ds" value="'.$ds.'" >
<INPUT class="form-submit" type="submit" name="submit" value="View Sequence">
'.$site_taggle.'
<br /></div>';
mysql_free_result($result);

}
return $map_content;
}
function gene_overlapped(&$gene, $start, $end, $num_line)
{
	global  $overlap_dis;
	//$num_line = 1;
	$gene[0]['line'] = 1;
	$flag = 1;
	
	while($flag == 1)
	{
		$flag = 0;
		for($k=0; $k<sizeof($gene); $k++)
		{
			
			if($gene[$k]['line']==0)
			{
				
				$flag_over = 0;
//				for($t=0; $t<sizeof($gene); $t++)
				for($t=$start; $t<$end; $t++)
				{
					if(($gene[$t]['line']==$num_line) && 
						( 
						 (($gene[$k]['left_end'] < $gene[$t]['right_end'] +$overlap_dis) && ($gene[$k]['left_end'] > $gene[$t]['left_end'])-$overlap_dis)
						 || (($gene[$k]['right_end'] < $gene[$t]['right_end']+$overlap_dis) && ($gene[$k]['right_end'] > $gene[$t]['left_end']-$overlap_dis))
						 )
						 )
					{
						$flag_over = 1;
						break;
					}
				}
				if($flag_over==0)
				{
					$gene[$k]['line']=$num_line;
					
				}
				
			}
		}
		for($k=0; $k<sizeof($gene); $k++)
		{
			if($gene[$k]['line']==0)
			{
				$flag = 1;
				$num_line = $num_line + 1;
			    
				break;
			}
		}
		
		
		
	}
	
	return $num_line;
}

function my_strcasecmp($str1, $str2)
{
	$value = -1;
	if(strcasecmp($str1,$str2)==0)
	{
		$value = 0;
	}
	
	$part1 = substr($str1, 0, 3);
	$part2 = substr($str1, 3, strlen($str1)-3);


	if ($part1 == 'REP') {
		if (strpos($str2, $part1)===false || strpos($str2, $part2)===false) {	
			$value = -1;		
		} else {
			$value = 0;
		}
	} elseif ($part1 == 'RIP') {
		if ((strpos($str2, 'REP')===false || strpos($str2, $part2)===false) && (strpos($str2, 'IHF')===false || strpos($str2, $part2)===false)) {	
			$value = -1;		
		} else {
			$value = 0;
		}
		
	}
	
	return $value;

}

function draw_gene($gene, $im, $num_gene_line, $num_diff_gene,$gene_color,&$map,$left_end_pos, $right_end_pos)
{
	global $text_size, $text_size_2, $fontfile, $title_color,$text_color;
	global $left_margin, $right_margin;
	global $base_url;
	$line_size = 4;
	$num_diff_gene=0;
	$start_map = sizeof($map);
	$ims = imagesx($im) - ($left_margin+$right_margin);

	for($i=0; $i<sizeof($gene); $i++)
	{

		for($k=0; $k<sizeof($gene_color); $k++)
		{
			if(my_strcasecmp($gene_color[$k]['name'],$gene[$i]['name'])==0)
			{
				
				$color = imagecolorallocate($im, $gene_color[$k]['c_r'],$gene_color[$k]['c_g'],$gene_color[$k]['c_b']);
		
			}
		}
		$pos_1 = $left_margin + ($gene[$i]['left_end']-$left_end_pos)/($right_end_pos-$left_end_pos)*$ims;
		$pos_2 = $left_margin + ($gene[$i]['right_end']-$left_end_pos)/($right_end_pos-$left_end_pos)*$ims;
		$line = $gene[$i]['line'];
		
		$y1 = 60  + $line*30;
		
		if(!strcmp($gene[$i]['orientation'],'Clockwise')){
			
			imagelinethick ( $im,$pos_1 ,$y1,  $pos_2-$line_size*2, $y1,$color,$line_size*2 );
			arrow($im, $pos_2-$line_size*2, $y1, $pos_2, $y1, $line_size*2, $line_size*2, $color);
			
		}else if(!strcmp($gene[$i]['orientation'],'Counterclockwise')){
			
			imagelinethick ( $im,$pos_1+$line_size*2 ,$y1,  $pos_2, $y1,$color,$line_size*2 );
			arrow($im, $pos_1+$line_size*2, $y1, $pos_1, $y1, $line_size*2, $line_size*2, $color);
			
		}else if(!strcmp($gene[$i]['orientation'],'Bidirectional')){
			
			imagelinethick ( $im,$pos_1+$line_size*2 ,$y1,  $pos_2-$line_size*2, $y1,$color,$line_size*2 );
			arrow($im, $pos_2-$line_size*2, $y1, $pos_2, $y1, $line_size*2, $line_size*2, $color);
			arrow($im, $pos_1+$line_size*2, $y1, $pos_1, $y1, $line_size*2, $line_size*2, $color);
			
		} else {
			
			imagelinethick ( $im,$pos_1 ,$y1,  $pos_2, $y1,$color,$line_size*2 );
//			if ($gene[$i]['type'] == 'REP' || $gene[$i]['type'] == 'igr') {
//				imagettftext ($im, $text_size-7, 0, (($pos_1+$pos_2)/2)-strlen($gene[$i]['name'])*3, $y1+16, $line_color, $fontfile, $gene[$i]['name']);
//			}
		}
	

		if ($gene[$i]['type'] == 'REP' || $gene[$i]['type'] == 'igr') {
			//imagelinethick ( $im,$pos_1 ,$y1,  $pos_2, $y1,$color,$line_size*2 );
			imagettftext ($im, $text_size-7, 0, (($pos_1+$pos_2)/2)-strlen($gene[$i]['name'])*3, $y1+16, $color, $fontfile, $gene[$i]['name']);
		}
		
		$map[$i+$start_map]['x_1'] =   intval($pos_1);
		$map[$i+$start_map]['x_2'] =   intval($pos_2); 
		$map[$i+$start_map]['y_1'] =   60 + $num_diff_gene*20 + $line*30;
		$map[$i+$start_map]['y_2'] = 	70 + $num_diff_gene*20+ $line*30;
		$map[$i+$start_map]['atl'] = "Left End: ".$gene[$i]['left_end']."  Right End: ".$gene[$i]['right_end'];
		
		if ($gene[$i]['type'] == 'tfbs')
		{
			$strong_evidence = empty($gene[$i]['strong_evidence'])?"":"Strong evidence: &#013; ".str_replace(",","&#013; ",$gene[$i]['strong_evidence'])."&#013;";
			$weak_evidence = empty($gene[$i]['weak_evidence'])?"": "Weak evidence: &#013; ".str_replace(",","&#013; ",$gene[$i]['weak_evidence'])."&#013;";
			$map[$i+$start_map]['atl'] =  $strong_evidence.$weak_evidence.$map[$i+$start_map]['atl'];
		} 

		if ($gene[$i]['type'] != 'REP' && $gene[$i]['type'] != 'igr') {

			$reuglon_search_url = 'http://regulondb.ccg.unam.mx/operon?format=jsp&organism=ECK12&type=operon&term=';
			$map[$i+$start_map]['link'] = $reuglon_search_url.$gene[$i]['operon_id'];
			
			
		} else {
				$map[$i+$start_map]['link'] = $base_url."/?q=topic/".$gene[$i]['topic_id'];
		}
		

	}
	
}

function draw_title($gene_color, $im, &$map)
{
	global $text_size, $text_size_2, $fontfile, $title_color,$text_color;
	global $left_margin, $right_margin;
	
	$width = imagesx($im);
	$heigh = imagesy($im);
	

	imagettftext ($im, $text_size, 0, $left_margin,$text_size+10, $title_color, $fontfile, "Intergenic Region & Transcription Factor Binding Sites from RegulonDB" );
//	imagettftext ($im, $text_size, 0, $left_margin,$text_size+30, $title_color, $fontfile, "Transcription Factor Binding Sites from RegulonDB" );
		$map[0]['x_1'] =   $left_margin;
		$map[0]['x_2'] =   $width-200;
		$map[0]['y_1'] =   30;
		$map[0]['y_2'] = 	$text_size+30;
		$map[0]['atl'] = "Go to RegulonDB Site";
		//		$map[0]['eg_id'] = "http://regulondb.ccg.unam.mx/";
		$map[0]['link'] = "http://regulondb.ccg.unam.mx/";
//		imagelinethick($im,$width-352, $text_size+36, $width-240, $text_size+36, $title_color, 1);
		
	$sz_map = sizeof($map);	
	
	for($i=0; $i<sizeof($gene_color); $i++)
	{
		$color = imagecolorallocate($im, $gene_color[$i]['c_r'],$gene_color[$i]['c_g'],$gene_color[$i]['c_b']);
		if ($gene_color[$i]['type'] == 'igr') {
			$name = $gene_color[$i]['name'];

//			for ($k=0;$k<strlen($name);$k++) {
//				$ch = $name[$k];
//				if (preg_match("([0-9])",$ch)) {
//					break;
//				}
//			}
//			
//			$name = substr($name,0,$k);
			imagettftext ($im, $text_size_2, 0, $width-90, 20 + $i*20, $color, $fontfile, $name );
		} else {
			imagettftext ($im, $text_size_2, 0, $width-90, 20 + $i*20, $color, $fontfile,$gene_color[$i]['name'] );
		}
		imagefilledrectangle($im, $width-130 ,10 + $i*20,  $width-100, 20 + $i*20, $color);
		
		$map[$i+$sz_map]['x_1'] =   $width-160;
		$map[$i+$sz_map]['x_2'] =   $width-100; 
		$map[$i+$sz_map]['y_1'] =   10 + $i*20;
		$map[$i+$sz_map]['y_2'] = 	20 + $i*20;
		$map[$i+$sz_map]['atl'] = 	$gene_color[$i]['atl'];//"Go to GenePage";
//		$map[$i+1]['eg_id'] = $gene_color[$i]['eg_id']; 
		$map[$i+$sz_map]['link'] = $gene_color[$i]['link'];
		
	}
	
}
function draw_kb($im, $left_end_pos,$right_end_pos)
{
	global $text_size, $text_size_2, $fontfile, $title_color,$text_color;
	global $left_margin, $right_margin;
	$width = imagesx($im);
	$heigh = imagesy($im);
	$i=1;
	imagettftext ($im, $text_size_2+1, 0, $width-$right_margin+5, $i*20+26,  $text_color, $fontfile,"kb" );
//	imagettftext ($im, $text_size_2+1, 0, $width-$right_margin, $i*20+30,  $text_color, $fontfile,"min(Cs)" );
	imagettftext ($im, $text_size_2+1, 0, $width-$right_margin-20, $i*20+10,  $text_color, $fontfile,$right_end_pos );
	imagettftext ($im, $text_size_2+1, 0, $left_margin, $i*20+10,  $text_color, $fontfile,$left_end_pos );
	$line_size = 2;
	imagelinethick($im, $left_margin,$i*20+20,imagesx($im)-$right_margin,$i*20+20,$text_color,$line_size/2);
	imagelinethick($im, $left_margin,$i*20+15,$left_margin,$i*20+25,$text_color,$line_size/2);
	imagelinethick($im, imagesx($im)-$right_margin,$i*20+15,imagesx($im)-$right_margin,$i*20+25,$text_color,$line_size/2);
}

//function imagelinethick($image, $x1, $y1, $x2, $y2, $color, $thick = 1) 
//{
//    /* this way it works well only for orthogonal lines
//    imagesetthickness($image, $thick);
//    return imageline($image, $x1, $y1, $x2, $y2, $color);
//    */
//    if ($thick == 1) {
//        return imageline($image, $x1, $y1, $x2, $y2, $color);
//    }
//    $t = $thick / 2 - 0.5;
//    if ($x1 == $x2 || $y1 == $y2) {
//        return imagefilledrectangle($image, round(min($x1, $x2) - $t), round(min($y1, $y2) - $t), round(max($x1, $x2) + $t), round(max($y1, $y2) + $t), $color);
//    }
//    $k = ($y2 - $y1) / ($x2 - $x1); //y = kx + q
//    $a = $t / sqrt(1 + pow($k, 2));
//    $points = array(
//        round($x1 - (1+$k)*$a), round($y1 + (1-$k)*$a),
//        round($x1 - (1-$k)*$a), round($y1 - (1+$k)*$a),
//        round($x2 + (1+$k)*$a), round($y2 - (1-$k)*$a),
//        round($x2 + (1-$k)*$a), round($y2 + (1+$k)*$a),
//    );    
//    imagefilledpolygon($image, $points, 4, $color);
//    return imagepolygon($image, $points, 4, $color);
//}
//

//function drawOneGene($im, $x1, $y1, $x2, $y2, $name, $orientation, $line_size,$text_size,$is_left_include,$is_right_include,$type, $status){
//	global $fontfile;
//	
//	global $line_color_cw;
//    global $line_color_ccw;
//    
//	
//	if(!strcmp($orientation,'Clockwise'))
//	{
//		$line_color = $line_color_cw;
//	}
//	else 
//	{
//		$line_color = $line_color_ccw;
//	}
//	if(!strcmp($status,'PSEUDO')){
//		imagettftext ($im, $text_size, 0, (intval($x1+$x2)/2)-13, $y1+15, $line_color, $fontfile, $name."'");
//	}
//  else{
//  	imagettftext ($im, $text_size, 0, (intval($x1+$x2)/2)-13, $y1+15, $line_color, $fontfile, $name);
//  }
//	imagelinethick ( $im, $x1, $y1, $x2, $y2, $line_color,$line_size);
//
//	if(!strcmp($orientation,'Clockwise')){
//
//		if($is_left_include)
//		{
//			
//			imagelinethick ( $im, $x1, $y1-1.5*$line_size-1, $x1, $y1+1.5*$line_size, $line_color,$line_size );
//		}	
//		arrow($im, $x2-$line_size, $y1, $x2+$line_size, $y1, $line_size*2, $line_size*2, $line_color);
//	}
//	else{
//
//		if($is_right_include)
//		{
//			imagelinethick ( $im, $x2, $y1-1.5*$line_size-1, $x2, $y1+1.5*$line_size, $line_color,$line_size );
//		}
//	
//			arrow($im, $x1+$line_size, $y1, $x1-$line_size, $y1,  $line_size*2, $line_size*2, $line_color);
//	}
//	
//	
//}
//function arrow($im, $x1, $y1, $x2, $y2, $alength, $awidth, $color) {
//	
//   $distance = sqrt(pow($x1 - $x2, 2) + pow($y1 - $y2, 2));
//
//   $dx = $x2 + ($x1 - $x2) * $alength / $distance;
//   $dy = $y2 + ($y1 - $y2) * $alength / $distance;
//
//   $k = $awidth / $alength;
//
//   $x2o = $x2 - $dx;
//   $y2o = $dy - $y2;
//
//   $x3 = $y2o * $k + $dx;
//   $y3 = $x2o * $k + $dy;
//
//   $x4 = $dx - $y2o * $k;
//   $y4 = $dy - $x2o * $k;
// $points = array(
//        $x3, $y3,
//        $x4, $y4,
//        $x2, $y2,
//        
//    ); 
// imagefilledpolygon($im, $points, 3, $color);
//
//} 

?>
