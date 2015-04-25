<?PHP
include('ecoFunction.php');


global $text_size,$line_size, $text_size_2, $fontfile,$title_color,$text_color;
global $left_margin, $right_margin;
global $left_end_pos, $right_end_pos;
global $overlap_dis;
global $line_color_cw;
global $line_color_ccw;
global $num_tfbs;

$overlap_dis = 10;;
$text_size = 15;
$text_size_2 = 10;
$fontfile = "./genemap/verdana.ttf"; 

/******************************************************************

Set the left and right margin of the map, where the genes and tfbs
are shown in between, and the margins are used to show the ruler and
the position of the right and left most position on the sequence.

******************************************************************/
$left_margin = 40;
$right_margin = 60;

/******************************************************************

The size of the region of the sequence which will be shown in the 
map

******************************************************************/
$map_range = 600;


import_request_variables("gP");


include_once("dblink.php");
$link=dblink();
mysql_select_db('ecogene') or die('Unable to select database');

/******************************************************************

Get the left end and right end of the map from the mean of the tfbs
and the range of the map is set 600 bits

******************************************************************/
if(isset($eg_id))
{
$query = "select avg  ( address.left_end + address.right_end)/2  as avg_pos from t_tfbs g, t_address address where g.address_id = address.address_id and g.EG_target = '".$eg_id."'";
}elseif (isset($is_id))
{
	//If the is is inside gene, then it is not intergenic region, then jump to the topic page
	$query = "SELECT  gis.*
			  FROM  t_is_address gis, t_address ga1, t_address ga2 
			  WHERE 
				(ga2.id_type = 'aa' or ga2.id_type = 'nt' or ga2.id_type = 'multi_address' ) and 
 				ga1.address_id=gis.address_id and  gis.id= '".$is_id."' and
 				
				( ( ga2.left_end <= ga1.left_end and ga2.right_end > ga1.right_end) 
 				  or  (ga2.left_end < ga1.left_end and ga2.right_end >= ga1.right_end) )
				";

	$result = mysql_query($query) or die("Query failed : " . mysql_error());
	if(mysql_num_rows($result)>0)
	{
		$query = "SELECT  topic_id from t_is_address where id='".$is_id."'";
		$result= mysql_query($query) or die("Query failed : " . mysql_error().$query);
		$topic_id = mysql_result($result, 0, 'topic_id');
		?>
		<script>

				window.location = ("topic.php?topic_id=<?echo $topic_id?>");

		</script>
		<?php
		
	}
	mysql_free_result($result);
	$query = "select avg  ( address.left_end + address.right_end)/2  as avg_pos from t_is_address g, t_address address where g.address_id = address.address_id and g.id = '".$is_id."'";
}
$result= mysql_query($query) or die("Query failed : " . mysql_error().$query);

$row = mysql_fetch_array($result, MYSQL_ASSOC);

$mean_pos = round($row['avg_pos']);
$left_end_pos = $mean_pos - round($map_range/2);
$right_end_pos = $left_end_pos + $map_range - 1;

mysql_free_result($result);

/******************************************************************

Get the disctinct tfbs names in the region privous set, sign them 
different colors.

******************************************************************/
$query = "select distinct ( g.name ) as name, g.EG_tag as tag  from t_tfbs g , t_address address  where g.address_id = address.address_id and address.left_end >= $left_end_pos  
          and address.right_end  <= $right_end_pos ";


$result= mysql_query($query) or die("Query failed : " . mysql_error().$query);
$num_tfbs = mysql_num_rows($result);
$i = 0;
while ($row = mysql_fetch_array($result, MYSQL_ASSOC))
{
	$gene_color[$i]['name'] = $row['name'];
	$gene_color[$i]['c_r'] =  rand(0,255);
	$gene_color[$i]['c_g'] =  rand(0,255);
	$gene_color[$i]['c_b'] =  rand(0,255);
	$gene_color[$i]['atl'] =  "Go to GenePage";
	
	if($row['tag']!="")
	{
		
		$items = explode (";",$row['tag']);
		
		$n_tag = count($items);
		
		if($n_tag==1)
		{
			
			$gene_color[$i]['eg_id'] =$row['tag'];
			$gene_color[$i]['link'] ="geneInfo.php?eg_id=".$gene_color[$i]['eg_id'];
			
			
		}
		else 
		{
			
			$gene_color[$i]['eg_id'] = $row['tag'];
			$gene_color[$i]['link'] ="ecoSearchProcess.php?&searchType=gene&egid=".$gene_color[$i]['eg_id'];
		}
		
			$i++;
	}
	
	
	
}
mysql_free_result($result);

//$query = "select * from t_is_address_tmp where left_end >= $left_end_pos and right_end <= $right_end_pos ";

$query = "select g.* from t_is_address_tmp g, t_address address where g.address_id = address.address_id 
		 and address.left_end >= $left_end_pos and address.right_end <= $right_end_pos ";

$result= mysql_query($query) or die("Query failed : " . mysql_error().$query);

while ($row = mysql_fetch_array($result, MYSQL_ASSOC))
{
	$gene_color[$i]['name'] = $row['name'];
	$gene_color[$i]['c_r'] =  rand(0,255);
	$gene_color[$i]['c_g'] =  rand(0,255);
	$gene_color[$i]['c_b'] =  rand(0,255);
	$gene_color[$i]['type'] = 'REP';
	$gene_color[$i]['atl'] =  "Go to TopicPage";

	$name2 = $row['name2'];
	$query = "select * from t_is_address t1, t_is_address_tmp t2 where t1.name2=t2.name2 and t2.name2= '$name2'";
	$rst= mysql_query($query) or die("Query failed : " . mysql_error().$query);
	$row = mysql_fetch_array($rst, MYSQL_ASSOC);
	$gene_color[$i]['link'] = "topic.php?topic_id=".$row['topic_id'];
			$i++;
	
	
}
mysql_free_result($result);
//$query = "select * from t_is_address where left_end >= $left_end_pos ".
//		"and right_end <= $right_end_pos and  (name2 not like 'REP%' and name2 not like 'IHF%')";
$query = "select g.* from t_is_address g, t_address address where g.address_id = address.address_id 
		 and address.left_end >= $left_end_pos ".
		"and address.right_end <= $right_end_pos and  (name2 not like 'REP%' and name2 not like 'IHF%')";

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
	$gene_color[$i]['link'] = "topic.php?topic_id=".$row['topic_id'];
	
	$i++;	
}

$num_diff_gene = $i;

if(($num_diff_gene)<1)
{
	$num_diff_gene=1;
}
mysql_free_result($result);
/******************************************************************

Get the individual tfbs in the region, and show them in multiple lines
so that they are not ovlapped.

******************************************************************/


$query = "select  g.name as name, g.EG_tag as EG_tag, g.EG_target as EG_target, g.type as type, address.left_end as left_end, address.right_end as right_end , address.orientation as orientation from t_tfbs g , t_address address  where g.address_id = address.address_id and address.left_end >= $left_end_pos  
          and address.right_end  <= $right_end_pos ";

$result= mysql_query($query) or die("Query failed : " . mysql_error().$query);
$i = 0;
while ($row = mysql_fetch_array($result, MYSQL_ASSOC))
{
	$gene[$i]['name'] = $row['name'];
	$gene[$i]['EG_target'] = $row['EG_target'];
	$gene[$i]['EG_tag'] = $row['EG_tag'];
	$gene[$i]['type'] = $row['type'];
	$gene[$i]['left_end'] = $row['left_end'];
	$gene[$i]['right_end'] = $row['right_end'];
	$gene[$i]['orientation'] = $row['orientation'];
	$gene[$i]['line'] = 0;
	$gene[$i]['type'] = 'NULL';
//	$gene[$i]['eg_id'] = $row['eg_id'];
	$i++;
	
}

$medpoint = $i;
$num_gene_line = gene_overlapped(&$gene, 0, $medpoint, 1);
mysql_free_result($result);

//$query = "select * from t_is_address where (name2 like 'REP%' or name2 like 'IHF%') and left_end >= $left_end_pos and right_end <= $right_end_pos ";
$query = "select g.* from t_is_address g, t_address address where g.address_id = address.address_id 
		 and address.left_end >= $left_end_pos ".
		"and address.right_end <= $right_end_pos and  (name2 like 'REP%' or name2 like 'IHF%')";

$result= mysql_query($query) or die("Query failed : " . mysql_error().$query);

while ($row = mysql_fetch_array($result, MYSQL_ASSOC))
{
	$gene[$i]['name'] = $row['name2'];
	
	$gene[$i]['left_end'] = $row['left_end'];
	$gene[$i]['right_end'] = $row['right_end'];
	$gene[$i]['orientation'] = 'Nodirection';
	$gene[$i]['line'] = 0;
	$gene[$i]['type'] = 'REP';
	$gene[$i]['topic_id'] = $row['topic_id'];

	$i++;
	
}

$num_gene = $i;
$num_gene_line = gene_overlapped(&$gene, $medpoint, $num_gene, $num_gene_line+1);
$medpoint = $i;
mysql_free_result($result);
//$query = "select * from t_is_address t1, t_address t2 where ".
//		"(t1.name2 not like 'REP%' and t1.name2 not like 'IHF%') ".
//		"and t1.left_end >= $left_end_pos and t1.right_end <= $right_end_pos ".
//		" and t1.address_id=t2.address_id and t2.id_type='igr'";

//$query = "select * from t_is_address t1, t_address t2 where ".
//		"(t1.name2 not like 'REP%' and t1.name2 not like 'IHF%') ".
//		"and t1.left_end >= $left_end_pos and t1.right_end <= $right_end_pos ".
//		" and t1.address_id=t2.address_id 
//		  and (t2.id_type='igr' or t2.id_type='prophage' or t2.id_type='rig' or t2.id_type='is')";

$query = "select * from t_is_address t1, t_address t2 where ".
		"(t1.name2 not like 'REP%' and t1.name2 not like 'IHF%') ".
		"and t2.left_end >= $left_end_pos and t2.right_end <= $right_end_pos ".
		" and t1.address_id=t2.address_id 
		  and (t2.id_type='igr' or t2.id_type='prophage' or t2.id_type='line2' or t2.id_type='rig' or t2.id_type='is')";
		
$result= mysql_query($query) or die("Query failed : " . mysql_error().$query);

while ($row = mysql_fetch_array($result, MYSQL_ASSOC))
{
	$gene[$i]['name'] = $row['name2'];	
	$gene[$i]['left_end'] = $row['left_end'];
	$gene[$i]['right_end'] = $row['right_end'];
	$gene[$i]['orientation'] = $row['orientation'];
	$gene[$i]['line'] = 0;
	$gene[$i]['type'] = 'igr';
	$gene[$i]['topic_id'] = $row['topic_id'];

	$i++;
	
}

$num_gene = $i;
$num_gene_line = gene_overlapped(&$gene, $medpoint, $num_gene, $num_gene_line+1);

mysql_free_result($result);
/******************************************************************

Initiate the map

******************************************************************/

$im = imagecreatetruecolor(800, $num_gene_line*30+50 + $num_diff_gene*20)
or die ("Cannot Initialize new GD image stream");
$map = array();
		
$background_color = imagecolorallocate ($im,  255, 255, 255);		
imagefill($im , 0,0, $background_color);
$title_color = imagecolorallocate($im, 255,0,240);
$text_color = imagecolorallocate ($im, 23,43,0);

/******************************************************************

Draw the title on the map and the legends, including the coloered
brick for different tfbs

******************************************************************/
draw_title($gene_color, $im, $map);

/******************************************************************

Draw genes in the region

******************************************************************/

//color for clockwise gene
//*******************************************
$line_color_cw = imagecolorallocate($im,30,144,255); 
//color for counterclockwise gene
//*******************************************
$line_color_ccw = imagecolorallocate($im,0, 0, 255);
$start_map = sizeof($map);

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
			drawOneGene($im, $pos_1,$line_y2,$pos_2,$line_y2,$row['name'], $row['orientation'], $line_size,$text_size_2,$is_left_include,$is_right_include,$row['type'], $status);
			
		}
		else {
			drawOneGene($im, $pos_1,$line_y1,$pos_2,$line_y1, $row['name'], $row['orientation'], $line_size,$text_size_2,$is_left_include,$is_right_include,$row['type'], $status);
			
		}
		$map[$i+$start_map]['x_1'] =   $pos_1;
		$map[$i+$start_map]['x_2'] =   $pos_2; 
		$map[$i+$start_map]['y_1'] =   $line_y1-10;
		$map[$i+$start_map]['y_2'] = 	$line_y2+10;
		$map[$i+$start_map]['atl'] = "Left End: ".$row['left_end']."  Right End: ".$row['right_end']; ;
		$map[$i+$start_map]['eg_id'] = $row['eg_id']; 
		$map[$i+$start_map]['link'] = "geneInfo.php?eg_id=".$row['eg_id']; 
		$i = $i+1;
	}
	
mysql_free_result($rst_geneInfo);
/******************************************************************

Draw tfbs in the region

******************************************************************/
draw_gene($gene, $im, $num_gene_line, $num_diff_gene,$gene_color,$map);

/******************************************************************

Create the picture and map

******************************************************************/

$tmpfname="./temp/regulon".$eg_id.$is_id.".png";

		Imagepng($im,$tmpfname);

		ImageDestroy($im);

echo "<MAP NAME=\"map1\">";
//echo "<AREA NAME=\"area0\" COORDS=\"".$map[0]['x_1'].",".$map[0]['y_1'].",".$map[0]['x_2'].",".$map[0]['y_2']."\" title=\"".$map[0]['atl']."\" HREF=\"".$map[0]['link']."\"".">";

//for ($t=1;$t<count($map);$t++)
//{
//	
//	echo "<AREA NAME=\"area".$t."\" COORDS=\"".$map[$t]['x_1'].",".$map[$t]['y_1'].",".$map[$t]['x_2'].",".$map[$t]['y_2']."\" title=\"".$map[$t]['atl']."\" HREF=\"".$map[$t]['link']."\"".">";
//}
$t=0;
foreach ($map as $map_t)
{
	
	echo "<AREA NAME=\"area".$t."\" COORDS=\"".$map_t['x_1'].",".$map_t['y_1'].",".$map_t['x_2'].",".$map_t['y_2']."\" title=\"".$map_t['atl']."\" HREF=\"".$map_t['link']."\"".">";
	
	$t++;
}
echo"</MAP>";


?>

<?PHP
echo "<center><IMG src=\"".$tmpfname."\" usemap=\"#map1\" border=1>";
if(isset($eg_id))
{
$query = "select  address.left_end, address.right_end, address.orientation   from t_gene g, t_address address where g.address_id = address.address_id and g.eg_id = '".$eg_id."'";

$result= mysql_query($query) or die("Query failed : " . mysql_error().$query);
$row = mysql_fetch_array($result, MYSQL_ASSOC);
if($row['orientation']=='Clockwise')
{
	$us = ($row['left_end']-$left_end_pos>0? $row['left_end']-$left_end_pos:0);
	$ds = ($row['right_end']-$right_end_pos<0? $right_end_pos - $row['right_end']:0);
}
else 
{
	$us = ($row['right_end']-$right_end_pos<0? $right_end_pos - $row['right_end']:0);
	$ds = ($row['left_end']-$left_end_pos>0? $row['left_end']-$left_end_pos:0);
}


echo "<form name=dnaForm action='/drupal/?q=gene/$eg_id/dnasequence' target=\"_parent\" method=\"post\">
<input type=hidden name=eg_id value='$eg_id' size=3>
<input type=hidden name=submit_tfbs value=1>
<input type=hidden name=type value='tfbs'>
 <input type=hidden name=us value='$us' >
<input type=hidden name=ds value='$ds' >
<a href=\"javascript:document.dnaForm.submit()\" style='CURSOR: hand'><img border=\"0\" src=\"getImage.php?text=DNA SEQUENCE\"  alt=\"View SEQUENCE\" onClick=\"javascript:document.dnaForm.submit()\"></A>
<BR>";
mysql_free_result($result);
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

function draw_gene($gene, $im, $num_gene_line, $num_diff_gene,$gene_color,&$map)
{
	global $text_size, $text_size_2, $fontfile, $title_color,$text_color;
	global $left_margin, $right_margin;
	global $left_end_pos, $right_end_pos;	

	$line_size = 4;
	
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
		
		$y1 = 60 + $num_diff_gene*20 + $line*30;
		
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
			imagettftext ($im, $text_size-7, 0, (($pos_1+$pos_2)/2)-strlen($gene[$i]['name'])*3, $y1+16, $line_color, $fontfile, $gene[$i]['name']);
		}
		
		$map[$i+$start_map]['x_1'] =   intval($pos_1);
		$map[$i+$start_map]['x_2'] =   intval($pos_2); 
		$map[$i+$start_map]['y_1'] =   60 + $num_diff_gene*20 + $line*30;
		$map[$i+$start_map]['y_2'] = 	70 + $num_diff_gene*20+ $line*30;
		$map[$i+$start_map]['atl'] = "Left End: ".$gene[$i]['left_end']."  Right End: ".$gene[$i]['right_end'];
		
		$items = explode (";", $gene[$i]['EG_target']);
		
		$n_tag = count($items);
		
		if ($gene[$i]['type'] != 'REP' && $gene[$i]['type'] != 'igr') {
//change the link to regulon website using eg_tag eg_id, link to t_biodatabase_link to retrieve the rregulon entry. link to the RegulonDB link (biodatabse_id = 63) for each EG_id.
		
			$query_regulon = "select t_biodatabase.biodatabase_url, t_biodatabase_link.accession_id from t_biodatabase_link, t_biodatabase where t_biodatabase_link.eg_id='$items[0]' and t_biodatabase.biodatabase_id=63 and t_biodatabase.biodatabase_id=t_biodatabase_link.biodatabase_id";
			$rst_regulon = mysql_query($query_regulon) or die("Query failed : " . mysql_error());	
			$row = mysql_fetch_array($rst_regulon, MYSQL_ASSOC);
			
			$map[$i+$start_map]['link'] = $row['biodatabase_url'].$row['accession_id'];
			
//			if($n_tag==1)
//			{
//				$map[$i+$start_map]['eg_id'] = $gene[$i]['EG_tag'];
//				$map[$i+$start_map]['link'] = "geneInfo.php?eg_id=".$map[$i+$start_map]['eg_id'];
//
//			}
//			else
//			{
//
//				$map[$i+$start_map]['eg_id'] = $gene[$i]['EG_tag'];
//				$map[$i+$start_map]['link'] ="ecoSearchProcess.php?&searchType=gene&egid=".$map[$i+$start_map]['eg_id'];
//			}
		} else {
				$map[$i+$start_map]['link'] = "topic.php?topic_id=".$gene[$i]['topic_id'];
		}
		

	}
	
}

function draw_title($gene_color, $im, &$map)
{
	global $text_size, $text_size_2, $fontfile, $title_color,$text_color;
	global $left_margin, $right_margin;
	global $left_end_pos, $right_end_pos;
	global $num_tfbs;
	$width = imagesx($im);
	$heigh = imagesy($im);
	

	
	if($num_tfbs>0)
	{
		imagettftext ($im, $text_size, 0, $left_margin,$text_size+10, $title_color, $fontfile, "Transcription Factor Binding Sites from RegulonDB" );
		$map[0]['x_1'] =   $left_margin;
		$map[0]['x_2'] =   $width-200;
		$map[0]['y_1'] =   10;
		$map[0]['y_2'] = 	$text_size+20;
		$map[0]['atl'] = "Go to RegulonDB Site";
		//		$map[0]['eg_id'] = "http://regulondb.ccg.unam.mx/";
		$map[0]['link'] = "http://regulondb.ccg.unam.mx/";
		imagelinethick($im,$width-358, $text_size+16, $width-245, $text_size+16, $title_color, 1);
	}
	else
	{
		imagettftext ($im, $text_size, 0, $left_margin,$text_size+10, $title_color, $fontfile, "Intergenic Region" );
	}
		
	$sz_map = sizeof($map);	
	for($i=0; $i<sizeof($gene_color); $i++)
	{
		$color = imagecolorallocate($im, $gene_color[$i]['c_r'],$gene_color[$i]['c_g'],$gene_color[$i]['c_b']);
		if ($gene_color[$i]['type'] == 'igr') {
			$name = $gene_color[$i]['name'];

			for ($k=0;$k<strlen($name);$k++) {
				$ch = $name[$k];
				if (ereg("([0-9])",$ch)) {
					break;
				}
			}
			
			$name = substr($name,0,$k);
			imagettftext ($im, $text_size_2, 0, $width-120, 20 + $i*20, $color, $fontfile, $name );
		} else {
			imagettftext ($im, $text_size_2, 0, $width-120, 20 + $i*20, $color, $fontfile,$gene_color[$i]['name'] );
		}
		imagefilledrectangle($im, $width-160 ,10 + $i*20,  $width-130, 20 + $i*20, $color);
		
		$map[$i+$sz_map]['x_1'] =   $width-160;
		$map[$i+$sz_map]['x_2'] =   $width-100; 
		$map[$i+$sz_map]['y_1'] =   10 + $i*20;
		$map[$i+$sz_map]['y_2'] = 	20 + $i*20;
		$map[$i+$sz_map]['atl'] = 	$gene_color[$i]['atl'];//"Go to GenePage";
//		$map[$i+1]['eg_id'] = $gene_color[$i]['eg_id']; 
		$map[$i+$sz_map]['link'] = $gene_color[$i]['link'];
		
	}
	if($i<1)
		$i=1;
	imagettftext ($im, $text_size_2+1, 0, $width-$right_margin, $i*20+16,  $text_color, $fontfile,"kb" );
//	imagettftext ($im, $text_size_2+1, 0, $width-$right_margin, $i*20+30,  $text_color, $fontfile,"min(Cs)" );
	imagettftext ($im, $text_size_2+1, 0, $width-$right_margin-20, $i*20+40,  $text_color, $fontfile,$right_end_pos );
	imagettftext ($im, $text_size_2+1, 0, $left_margin, $i*20+40,  $text_color, $fontfile,$left_end_pos );
	$line_size = 2;
	imagelinethick($im, $left_margin,$i*20+20,imagesx($im)-$right_margin,$i*20+20,$text_color,$line_size/2);
	imagelinethick($im, $left_margin,$i*20+15,$left_margin,$i*20+25,$text_color,$line_size/2);
	imagelinethick($im, imagesx($im)-$right_margin,$i*20+15,imagesx($im)-$right_margin,$i*20+25,$text_color,$line_size/2);
}

function imagelinethick($image, $x1, $y1, $x2, $y2, $color, $thick = 1) 
{
    /* this way it works well only for orthogonal lines
    imagesetthickness($image, $thick);
    return imageline($image, $x1, $y1, $x2, $y2, $color);
    */
    if ($thick == 1) {
        return imageline($image, $x1, $y1, $x2, $y2, $color);
    }
    $t = $thick / 2 - 0.5;
    if ($x1 == $x2 || $y1 == $y2) {
        return imagefilledrectangle($image, round(min($x1, $x2) - $t), round(min($y1, $y2) - $t), round(max($x1, $x2) + $t), round(max($y1, $y2) + $t), $color);
    }
    $k = ($y2 - $y1) / ($x2 - $x1); //y = kx + q
    $a = $t / sqrt(1 + pow($k, 2));
    $points = array(
        round($x1 - (1+$k)*$a), round($y1 + (1-$k)*$a),
        round($x1 - (1-$k)*$a), round($y1 - (1+$k)*$a),
        round($x2 + (1+$k)*$a), round($y2 - (1-$k)*$a),
        round($x2 + (1-$k)*$a), round($y2 + (1+$k)*$a),
    );    
    imagefilledpolygon($image, $points, 4, $color);
    return imagepolygon($image, $points, 4, $color);
}


function drawOneGene($im, $x1, $y1, $x2, $y2, $name, $orientation, $line_size,$text_size,$is_left_include,$is_right_include,$type, $status){
	global $fontfile;
	
	global $line_color_cw;
    global $line_color_ccw;
    
	
	if(!strcmp($orientation,'Clockwise'))
	{
		$line_color = $line_color_cw;
	}
	else 
	{
		$line_color = $line_color_ccw;
	}
	if(!strcmp($status,'PSEUDO')){
		imagettftext ($im, $text_size, 0, (intval($x1+$x2)/2)-13, $y1+15, $line_color, $fontfile, $name."'");
	}
  else{
  	imagettftext ($im, $text_size, 0, (intval($x1+$x2)/2)-13, $y1+15, $line_color, $fontfile, $name);
  }
	imagelinethick ( $im, $x1, $y1, $x2, $y2, $line_color,$line_size);

	if(!strcmp($orientation,'Clockwise')){

		if($is_left_include)
		{
			
			imagelinethick ( $im, $x1, $y1-1.5*$line_size-1, $x1, $y1+1.5*$line_size, $line_color,$line_size );
		}	
		arrow($im, $x2-$line_size, $y1, $x2+$line_size, $y1, $line_size*2, $line_size*2, $line_color);
	}
	else{

		if($is_right_include)
		{
			imagelinethick ( $im, $x2, $y1-1.5*$line_size-1, $x2, $y1+1.5*$line_size, $line_color,$line_size );
		}
	
			arrow($im, $x1+$line_size, $y1, $x1-$line_size, $y1,  $line_size*2, $line_size*2, $line_color);
	}
	
	
}
function arrow($im, $x1, $y1, $x2, $y2, $alength, $awidth, $color) {
	
   $distance = sqrt(pow($x1 - $x2, 2) + pow($y1 - $y2, 2));

   $dx = $x2 + ($x1 - $x2) * $alength / $distance;
   $dy = $y2 + ($y1 - $y2) * $alength / $distance;

   $k = $awidth / $alength;

   $x2o = $x2 - $dx;
   $y2o = $dy - $y2;

   $x3 = $y2o * $k + $dx;
   $y3 = $x2o * $k + $dy;

   $x4 = $dx - $y2o * $k;
   $y4 = $dy - $x2o * $k;
 $points = array(
        $x3, $y3,
        $x4, $y4,
        $x2, $y2,
        
    ); 
 imagefilledpolygon($im, $points, 3, $color);

} 

?>
