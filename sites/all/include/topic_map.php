<?php

    
	include_once("ecoFunction.php");
	include_once('eco_map.php');     	// class of map
	include_once("dblink.php");
	

	global $GENOME_LENGTH;  			// come from the ecoFunction
	global $background_color, $foreground_color, $gene_color, $fontfile;
	global $text_size, $line_len, $line_size;


	///////  setting the parameters for drawing the map picture  ////////////////////////
	

	// size of the map
	$diameter = 70;  											// diameter of the circle map of the picture;
    $img_width = $diameter+20; 	
    $img_hight =  $diameter+20; 								// size of the picture
    $cx= $img_width/2;     
    $cy = $img_hight/2; 										// center of th picture
     
    // color, font and size of the text and line of the map     
    $fontfile = 'verdana.ttf'; 	     						
    $text_size =9;
	$line_size =2;
    $line_len = 2;
    
			
	 	
	//////// image is created from here  /////////////////////////////////////////////////////    
     
    header ("Content-type: image/png");
    $img = @ImageCreateFrompng("emptybotton90.PNG")
      	or die("Cannot Initialize new GD image stream"); 	
    
	$background_color = imagecolorallocate ($img,  0, 146, 193);
    $foreground_color = imagecolorallocate($img, 255, 255, 255);
    $gene_color = imagecolorallocate($img, 255, 0, 0);
    
 	 
	$left_end = 1;
	$right_end = $GENOME_LENGTH;									// range of the genome to be included
	$map = new ecomap($img,$img_width, $img_hight, $left_end, $right_end); // initiate the map
	 		
	$genes = get_icon_map_genes($_REQUEST,$num_unique_gene);			// get the info of the genes to be drawed on the map
	
	draw_icon_map($map, &$img, $genes,$num_unique_gene);						// draw the map

	
	imagepng($img);
	imagedestroy($img);
	
function draw_icon_map($map, $img, $genes, $num_unique_gene)
{
	global $GENOME_LENGTH;
	global $background_color, $foreground_color, $gene_color, $fontfile;
	global $diameter, $cx, $cy, $line_size, $text_size, $line_len;
	$i=0;
	$number = count($genes);
	While ($i < $number) {

		$left = $genes[$i]['left_end'];
		$angle = round($left/$GENOME_LENGTH*360000)/1000-90;

		$arc = deg2rad($angle);
		$x1 = round($cx + cos($arc)*($diameter/2));
		$x2 = round($cx + cos($arc)*($diameter/2+$line_size));
		$y1 = round($cy + sin($arc)*($diameter/2));
		$y2 = round($cy + sin($arc)*($diameter/2+$line_size));
		
		$map->imagelinethick($img, $x1, $y1, $x2, $y2, $gene_color, 2);

		$i++;
	}
	imagefilledellipse($img, $cx, $cy, $diameter, $diameter, $foreground_color);
	imagefilledellipse($img, $cx, $cy, $diameter-$line_len, $diameter-$line_len, $background_color);
	$font = new ecomapfont($foreground_color, null,$text_size,$fontfile);

	$number = $num_unique_gene;
	$text_elem
		 = new textofmap($font->color,null,null,null,null,$font->size,$font->fontfile);
	imagettftext ($map->img, $text_elem->size, null, $cx-strlen($number)*$font->size/2.5,$cy, $foreground_color, $text_elem->fontfile, $number  );
	if($number<=1)
		imagettftext ($map->img, $text_elem->size, null, $cx-4*$font->size/2.5,$cy+$font->size*1.5, $foreground_color, $text_elem->fontfile, "gene"  );	
	else
		imagettftext ($map->img, $text_elem->size, null, $cx-5*$font->size/2.5,$cy+$font->size*1.5, $foreground_color, $text_elem->fontfile, "genes"  );	
			 
	$map->imagelinethick($map->img, $cx ,$cy-$diameter/2,  $cx, $cy-$diameter/2+4,$foreground_color,2);

	$font->size = $font->size-2;
	imagettftext ($map->img, $font->size, 0, $cx-$font->size*2,$cy-$diameter/2+8+$font->size, $font->color, $font->fontfile, '0/100' );
}
function get_icon_map_genes($request, &$num_unique_gene)
{
	$number = 0;
	$link=dblink();
	mysql_select_db("ecogene") or die( "Could not select database");
	if (array_key_exists('topic_id', $request) && $request["topic_id"]!="")
	{
		$topic_id =  $request["topic_id"];
	
		$searchegs = "SELECT count(t_gene.eg_id) as total from t_topic_gene_link left join t_gene on t_topic_gene_link.eg_id=t_gene.eg_id 
                          where t_topic_gene_link.topic_id='$topic_id'";
		$result = mysql_query($searchegs) or die("Query failed : " .$query. mysql_error());
		$num_unique_gene = mysql_result($result,0,'total');
		mysql_free_result($result);

		$searchegs = "select eg_id from t_topic_gene_link where topic_id='$topic_id' and eg_id not in (select eg_id from t_gene_multi_address)";
		$searchegs_2 = "select eg_id from t_topic_gene_link where topic_id='$topic_id' and eg_id  in (select eg_id from t_gene_multi_address)";
	
		$query = "	Select gene.name, address.left_end, address.right_end, address.orientation from t_gene gene left join t_address address on gene.address_id=address.address_id where eg_id in (".$searchegs.") 
	union 
	Select g.name, gene.left_end, gene.right_end, gene.orientation from  t_gene_multi_address gene left join t_gene g on gene.eg_id=g.eg_id where gene.eg_id in  (".$searchegs_2.") 
				order by left_end";
		$result = mysql_query($query) or die("Query failed : " .$query. mysql_error());
		$number = mysql_num_rows($result);
		
		 
	
	}elseif(array_key_exists('pubmed_id', $request) && $request["pubmed_id"]!="" && array_key_exists('exp_id', $request) && $request["exp_id"]!="")
	{
		
		$pubmed_id =  $request["pubmed_id"];
		$exp_id =  $request["exp_id"];
	
		$query = "SELECT g.name, ad.left_end, ad.right_end, ad.orientation
						FROM t_array_data a JOIN t_gene g ON a.eg_id=g.eg_id JOIN t_address ad ON g.address_id=ad.address_id
						WHERE pubmed_id=$pubmed_id AND exp_id=$exp_id";
		
		$result = mysql_query($query) or die("Query failed : " .$query. mysql_error());
		$number = mysql_num_rows($result);
		
		$num_unique_gene = $number;
		
	
	}
	elseif(array_key_exists('egids', $request) && $request["egids"]!="")
	{	
		
		$egids =$request['egids'];
		
		$searchegs = "";
		$egidA = explode(',',$egids);
		
		foreach($egidA as $value) {
			$searchegs = $searchegs."'".$value."',";
		}
		$searchegs = strrev(substr(strrev($searchegs),1));
		$searchegs_1 = "select eg_id from t_gene where eg_id in (".$searchegs.") and eg_id not in (select eg_id from t_gene_multi_address)";
		$searchegs_2 = "select eg_id from t_gene_multi_address where eg_id in (".$searchegs.")";

		$query = "	Select gene.name, address.left_end, address.right_end, address.orientation from t_gene gene left join t_address address on gene.address_id=address.address_id where eg_id in (".$searchegs_1.")
	union 
	Select g.name, gene.left_end, gene.right_end, gene.orientation from  t_gene_multi_address gene left join t_gene g on gene.eg_id=g.eg_id where gene.eg_id in (".$searchegs_2.") 
				order by left_end";
			
		$result = mysql_query($query) or die("Query failed : " .$query. mysql_error());
		$number = mysql_num_rows($result);
		$num_unique_gene = count($egidA);
	
	}
		
	$i=0;
	$genes = array();
	While ($i < $number) {
		
		$genes[$i]["left_end"] = mysql_result($result,$i,'left_end');
		$i++;
		
	}
	if ($link) 	mysql_close($link);
	
	return $genes;
	
	
}


?>
 