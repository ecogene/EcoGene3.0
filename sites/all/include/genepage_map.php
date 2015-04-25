<?PHP
	include_once("ecoFunction.php");
	include_once("dblink.php");
	global $map_magnify;
function genepage_map($tmpfname, &$map, $map_magnify, $eg_id, $add_left_end)
{

	global $im, $map_magnify;
	$im = imagecreatetruecolor(800, 135)
		or die ("Cannot Initialize new GD image stream");
	
	$map = array();	
	$gene_name = drawGenePicture($im,$eg_id,$map);
	
	Imagepng($im,$tmpfname);
	ImageDestroy($im);		

}
function drawGenePicture($im,$eg_id,&$map){
	
	$gene_name = "";
	
	global $GENOME_LENGTH;
	global $im;
	global $map_magnify;
	// the image's background color is the same as the page's background colar
	//*************************************************************************

	$background_color = imagecolorallocate ($im,  255, 255, 255);
	imagefill($im , 0,0, $background_color);

	//  font file. the font used to write string in picture
	//*************************************************************************
	global $fontfile;
	global $line_color_pseudo;
	global $line_color_cw;
	global $line_color_ccw;
	global $line_color_rna;
	global $line_color_is;
	global $line_color_tfsite;
	global $margin, $left_margin, $right_margin, $imx,$imy;
	global $text_size,$line_size;

	$fontfile = "sites/all/include/verdana.ttf";

	//color for clockwise gene
	//*******************************************
	$line_color_cw = imagecolorallocate($im,30,144,255);
	//color for counterclockwise gene
	//*******************************************
	$line_color_ccw = imagecolorallocate($im,0, 0, 255);
	// color for RNA gene
	//*******************************************
	$line_color_rna = imagecolorallocate($im,160,32,240);
	$line_color_pseudo = imagecolorallocate($im,255,0,0);
	$line_color_is = imagecolorallocate($im,0,0,0);
	$line_color_tfsite = imagecolorallocate($im,139, 69, 19);
	// color for the title and the ruler and lengend text
	//*******************************************
	$text_color = imagecolorallocate ($im, 23,43,0);

	$text_size = 8;
	$line_size = 2;

	$left_margin = 20;
	$right_margin = 40;
	$margin = $left_margin+$right_margin;

	$line_y1 = 40;
	$line_y2 = 60;
	$line_y3 = 80;
	$line_y4 = 100;

	// Map lengend
	//********************************************************************
	//********************************************************************
	$imx = imagesx($im);
	$imy = imagesy($im);

	$hig = 124;
	$text_size2 = 7;

	imagettftext ($im, $text_size2, 0, $left_margin+3,$hig+6, $text_color, $fontfile,"Clockwise" );

	imagefilledrectangle($im, $left_margin+54,$hig, $left_margin+79, $hig+6, $line_color_cw);

	imagettftext ($im, $text_size2, 0, $left_margin+83,$hig+6,  $text_color, $fontfile, "Counterclockwise");
	imagefilledrectangle($im, $left_margin+172,$hig, $left_margin+197, $hig+6, $line_color_ccw);

	imagettftext ($im, $text_size2, 0, $left_margin+202,$hig+6,  $text_color, $fontfile,"RNA" );
	imagefilledrectangle($im, $left_margin+228,$hig, $left_margin+253, $hig+6, $line_color_rna);

	imagettftext ($im, $text_size2, 0, $left_margin+258,$hig+6,  $text_color, $fontfile,"Pseudogene" );
	imagefilledrectangle($im, $left_margin+322,$hig, $left_margin+347, $hig+6, $line_color_pseudo);
	//imagettftext ($im, $text_size2, 0, $left_margin+398,$hig+6,  $line_color_pseudo, $fontfile,"*" );

	imagettftext ($im, $text_size2, 0, $left_margin+351,$hig+6,  $text_color, $fontfile,"Intergene" );
	imagefilledrectangle($im, $left_margin+402,$hig, $left_margin+427, $hig+6, $line_color_is);



	//imagettftext ($im, 10, 0, 50, 50, $line_color, "$fontfile", $right_end);


	// Process database operation and get the information of genes, which will shown in
	// in the map picture. The algorithm is described as follow:
	// First get the left_end of the gene of the page, then find all the genes of it's
	// neighbor according the position on the genome. The range is 5000 byte ahead from
	// its left_end and 5000 behind of the left_end.
	// Second, scince the genome is a ring, Spececial attendtion will be paid at the begin and
	// end position.
	//**************************************************************************************
	//**************************************************************************************


	$link=dblink();
	mysql_select_db("ecogene") or die("Could not select database");

	/* Performing SQL query */

	//$query = 'SELECT '.
	//'g.eg_id, g.name,  ga.left_end '.
	//'FROM '.
	//'t_gene g, t_gene_address ga '.
	//'WHERE '.
	//'g.eg_id = ga.eg_id '.
	//'AND '.
	//"g.eg_id = '$eg_id'";

	$query = 'SELECT '.
	'g.eg_id, g.name,  ta.left_end '.
	'FROM '.
	't_gene g, t_address ta '.
	'WHERE '.
	" (g.type = 'aa' OR g.type = 'nt' ) AND ".
//	" (ta.id_type = 'aa' OR ta.id_type = 'nt' ) AND ".
	'g.address_id = ta.address_id '.
	'AND '.
	"g.eg_id = '$eg_id'";
	// avoid using t_gene_address table, used t_address instead
	// Dian Fan 5/25/07
	// pass test in Navicat, the query results are identical.
	// the query is saved in Navicat as Name="genemap_genemap_q1"

	$rst_geneInfo = mysql_query($query) or die("Query failed : " . mysql_error());

	$row = mysql_fetch_array($rst_geneInfo, MYSQL_ASSOC);

	$gene_name = $row['name'];


	$gene_neighbor = intval(10000/pow(2,$map_magnify-1));

	global $add_left_end;

	if(empty($add_left_end))
	{

		$left_end = $row['left_end']-$gene_neighbor/2;
		$right_end = $row['left_end']+$gene_neighbor/2;
	}
	else
	{

		//	$gene_name = $row['name'];
		$left_end = $add_left_end-$gene_neighbor/2;
		$right_end = $add_left_end+$gene_neighbor/2;
	}

	$center = $left_end;

//	is_address($left_end,$right_end,$line_y3,$gene_neighbor,$map);
//	is_address_2($left_end,$right_end,$line_y4,$gene_neighbor,$map);
//

	

	// Ruler of the Map (at the head of the picture)
	//***********************************************************************
	//imagestring ($im, 3, $imx-30,20-5*$line_size,  "kb", $text_color);
	imagettftext ($im, $text_size2+1, 0, $imx-18, 16,  $text_color, $fontfile,"kb" );
	imagettftext ($im, $text_size2+1, 0, $imx-45, 30,  $text_color, $fontfile,"min(Cs)" );

	imagelinethick($im, $left_margin,20,imagesx($im)-$right_margin,20,$text_color,$line_size/2);

	if($left_end<1){
		$nei_left_end=0;
	}

	if($right_end>$GENOME_LENGTH){
		$nei_right_end = $GENOME_LENGTH;
	}

	for ($t=ceil($left_end/1000)*1000;$t<=$right_end;$t=$t+1000) {
		$x = round(($t-$left_end)/$gene_neighbor*($imx-$margin)+$left_margin);
		//	echo $x."  ".$t."<br>";
		if(($t==ceil($left_end/1000)*1000)and ($t-$left_end)>100)
		{
			for($tt=1;$tt<=5;$tt++)
			{
				$pos = ($t-$tt*200);
				$xx = round(($pos-$left_end)/$gene_neighbor*($imx-$margin)+$left_margin);
				if($pos<$left_end) break;
				else
				imagelinethick($im, $xx,20-$line_size,$xx,20,$text_color,round($line_size/2));

			}
		}

		for($tt=1;$tt<=5;$tt++)
		{
			$pos = ($t+$tt*200);
			if($pos < $right_end) {

				//			echo "(".$left_end," ".$pos ." ".$right_end.")".$tt."<br>";
				$xx = round(($pos-$left_end)/$gene_neighbor*($imx-$margin)+$left_margin);
				imagelinethick($im, $xx,20-$line_size,$xx,20,$text_color,round($line_size/2));
			}

		}
		imagelinethick($im, $x,20-2*$line_size,$x,20,$text_color,round($line_size/2));
		//	imagestring ($im, 2, $x-8*$line_size,20-9*$line_size,  intval($t/1000).".0", $text_color);
		if ($t>$GENOME_LENGTH)
		{
			imagettftext ($im, $text_size2, 0, $x-$text_size2*2, 20-$text_size2,  $text_color, $fontfile,intval(($t-$GENOME_LENGTH)/1000).".0" );
		}
		elseif ($t<0)
		{
			imagettftext ($im, $text_size2, 0, $x-$text_size2*2, 20-$text_size2,  $text_color, $fontfile,intval(($t+$GENOME_LENGTH)/1000).".0" );
		}
		else {
			imagettftext ($im, $text_size2, 0, $x-$text_size2*2, 20-$text_size2,  $text_color, $fontfile,intval($t/1000).".0" );
		}
	}
	// Ruler of the Map (at the head of the picture) by minute
	//***********************************************************************
	$step_min = $GENOME_LENGTH*0.05/100;
	for ($t=ceil($left_end/$step_min*100)*$step_min/100;$t<=$right_end;$t=$t+$step_min) {
		$x = round(($t-$left_end)/$gene_neighbor*($imx-$margin)+$left_margin);
		//	echo $x."  ".$t."<br>";
		if(($t==ceil($left_end/$step_min*100)*$step_min/100)and ($t-$left_end)>$step_min*0.1)
		{
			for($tt=1;$tt<=5;$tt++)
			{
				$pos = ($t-$tt*$step_min*0.2);
				$xx = round(($pos-$left_end)/$gene_neighbor*($imx-$margin)+$left_margin);
				if($pos<$left_end) break;
				else
				imagelinethick($im, $xx,20,$xx,20+$line_size,$text_color,round($line_size/2));

			}
		}

		for($tt=1;$tt<=5;$tt++)
		{
			$pos = ($t+$tt*$step_min*0.2);
			if($pos < $right_end) {

				//			echo "(".$left_end," ".$pos ." ".$right_end.")".$tt."<br>";
				$xx = round(($pos-$left_end)/$gene_neighbor*($imx-$margin)+$left_margin);
				imagelinethick($im, $xx,20,$xx,20+$line_size,$text_color,round($line_size/2));
			}

		}
		imagelinethick($im, $x,20,$x,20+2*$line_size,$text_color,round($line_size/2));
		//	imagestring ($im, 2, $x-8*$line_size,20-9*$line_size,  intval($t/1000).".0", $text_color);
		if ($t>$GENOME_LENGTH)
		{
			imagettftext ($im, $text_size2, 0, $x-$text_size2*2, 20+2*$text_size2,  $text_color, $fontfile,  sprintf("%4.2f",floatval(($t-$GENOME_LENGTH)*100/$GENOME_LENGTH)) );
		}
		elseif ($t<0)
		{
			imagettftext ($im, $text_size2, 0, $x-$text_size2*2, 20+2*$text_size2,  $text_color, $fontfile, sprintf("%4.2f", floatval(($t+$GENOME_LENGTH)*100/$GENOME_LENGTH) ));
		}
		else {
			imagettftext ($im, $text_size2, 0, $x-$text_size2*2, 20+2*$text_size2,  $text_color, $fontfile, sprintf("%4.2f",floatval($t*100/$GENOME_LENGTH)) );
		}
	}

	// query to get the all the neighbor genes in the $gene_neighbor byte range
	//***************************************************************
	//****************************************************************
	$flag = 0;
	//$num_gene=0;
	$num_gene=count($map)-1;
	if($left_end<0)
	{

		//	$query = 'SELECT '.
		//	'g.eg_id, g.name, g.description, g.status, g.comments, g.type, '.
		//	'g.type, g.length, g.mnemonic_name, ga.orientation, ga.left_end, ga.right_end '.
		//	'FROM '.
		//	't_gene g, t_gene_address ga '.
		//	'WHERE '.
		//	'g.eg_id = ga.eg_id '.
		//	'AND '.
		//	"ga.right_end > $GENOME_LENGTH + $left_end ORDER BY ga.left_end";
		$query = 'SELECT '.
		'g.eg_id, g.name, g.description, g.status, g.comments, '.
		'g.type, g.length, g.mnemonic_name, ta.orientation, ta.left_end, ta.right_end '.
		'FROM '.
		't_gene g, t_address ta '.
		'WHERE '.
		" (ta.id_type = 'aa' OR ta.id_type = 'nt' ) AND ".
		'g.address_id = ta.address_id '.
		'AND '.
		"ta.right_end > $GENOME_LENGTH + $left_end ORDER BY ta.left_end";
		// avoid using t_gene_address table, used t_address instead
		// Dian Fan 5/25/07
		// pass test in Navicat, the query results are identical.
		// the query is saved in Navicat as Name="genemap_genemap_q2"

		$rst_geneInfo = mysql_query($query) or die("Query failed : " . mysql_error());
		$tempnum = mysql_num_rows($rst_geneInfo);
	

		while ($row = mysql_fetch_array($rst_geneInfo, MYSQL_ASSOC))
		{

			$status = isPseudogene($row['eg_id']);
			$num_gene=$num_gene+1;
			$imx = imagesx($im)-$margin;
			$imy = imagesy($im);
			$is_left_include = true;
			$is_right_include = true;

			$gene_left_end = $row['left_end'];
			$gene_right_end = $row['right_end'];

			if($row['left_end']<$GENOME_LENGTH + $left_end)	{
				$gene_left_end = $GENOME_LENGTH + $left_end;
				$is_left_include = false;
			}

			$x1 = intval(($gene_left_end-($GENOME_LENGTH+$left_end))/$gene_neighbor*$imx)+$left_margin;
			$x2 = intval(($gene_right_end-($GENOME_LENGTH+$left_end))/$gene_neighbor*$imx)+$left_margin;

			$map[$num_gene]['x1'] = $x1;
			$map[$num_gene]['x2'] = $x2;
			$map[$num_gene]['eg_id'] = $row['eg_id'];
//			$map[$num_gene]['y1'] = 20;
//			$map[$num_gene]['y2'] = 75;
			$map[$num_gene]['link'] = 'geneInfo.php';
			$map[$num_gene]['title'] = 'Go to Gene Page';
			$map[$num_gene]['id_name'] = 'eg_id';
			$map[$num_gene]['id'] = $row['eg_id'];

			$flag = $flag+1;
			if(($flag%2)==0){
				drawOneGene($im, $x1,$line_y2,$x2,$line_y2,$row['name'], $row['orientation'], $line_size,$text_size,$is_left_include,$is_right_include,$row['type'], $status);
				$map[$num_gene]['y1'] = 50;
				$map[$num_gene]['y2'] = 75;
			}
			else {
				drawOneGene($im, $x1,$line_y1,$x2,$line_y1, $row['name'], $row['orientation'], $line_size,$text_size,$is_left_include,$is_right_include,$row['type'], $status);
				$map[$num_gene]['y1'] = 20;
				$map[$num_gene]['y2'] = 49;

			}
		}

	}

	$query_split_eg = " SELECT distinct eg_id FROM t_gene_split_address WHERE  eg_id  in (select distinct eg_id from t_pseudogene ) ";
	$result_split_eg = mysql_query($query_split_eg);
	$split_eg = array();
	$i=0;
	while ($row_split_eg = mysql_fetch_array($result_split_eg, MYSQL_ASSOC)) {
		$split_eg[$i]=$row_split_eg['eg_id'];
		$i = $i+1;
	}
	mysql_free_result($result_split_eg);
	$i=0;

	//$query = 'SELECT '.
	//'g.eg_id, g.name, g.description, g.status, g.comments, g.type, '.
	//'g.type, g.length, g.mnemonic_name, ga.orientation, ga.left_end, ga.right_end '.
	//'FROM '.
	//'t_gene g, t_gene_address ga '.
	//'WHERE '.
	//'g.eg_id = ga.eg_id '.
	//'AND '.
	//"g.multi_location!='splitgene' " .
	//'AND '.
	////"g.eg_id not in ( select distinct eg_id from t_gene_split_address where eg_id in ( select distinct eg_id from t_pseudogene ) ) ".
	////'AND '.
	//"ga.right_end > $left_end AND ga.left_end < $right_end ORDER BY ga.left_end";
	$query = 'SELECT '.
	'g.eg_id, g.name, g.description, g.status, g.comments, g.type, '.
	'g.type, g.length, g.mnemonic_name, ta.orientation, ta.left_end, ta.right_end '.
	'FROM '.
	't_gene g, t_address ta '.
	'WHERE '.
	" (ta.id_type = 'aa' OR ta.id_type = 'nt' ) AND ".
	'g.address_id = ta.address_id '.
	'AND '.
	"g.multi_location!='splitgene' " .
	'AND '.
	"g.multi_location!='split_isgene' " .
	'AND '.
	"g.eg_id NOT IN ( SELECT eg_id FROM t_gene_split_address) ".
	'AND '.
	// "g.eg_id not in ( select distinct eg_id from t_gene_split_address where eg_id in ( select distinct eg_id from t_pseudogene ) ) ".
	// 'AND '.
	"ta.right_end > $left_end AND ta.left_end < $right_end ORDER BY ta.left_end";
	// avoid using t_gene_address table, used t_address instead
	// Dian Fan 5/25/07
	// pass test in Navicat, the query results are identical.
	// the query is saved in Navicat as Name="genemap_genemap_q3"

	$rst_geneInfo = mysql_query($query) or die("Query failed : " . mysql_error());
	$tempnum = mysql_num_rows($rst_geneInfo);

//  remove on 10/25/2012, now each is gene has its onw unique eg_id
//	$query_multi_address = 'SELECT '.
//	'ga.id, ga.eg_id, ga.orientation, ga.left_end, ga.right_end, g.name, g.status, g.type, g.multi_location '.
//	'FROM '.
//	't_gene_multi_address ga left join t_gene g on ga.eg_id = g.eg_id '.
//	'WHERE '.
//	"ga.right_end > $left_end AND ga.left_end < $right_end ORDER BY ga.left_end";

	$query_split_address = 'SELECT '.
	'ga.id, ga.eg_id, ga.orientation, ga.left_end, ga.right_end, g.name, g.status, g.type, g.multi_location '.
	'FROM '.
	't_gene_split_address ga left join t_gene g on ga.eg_id = g.eg_id '.
	'WHERE '.
	"ga.right_end > $left_end AND ga.left_end < $right_end  ". 
	//" and ( g.multi_location='splitgene' or g.multi_location='split_isgene' ) ".
	//" and ".
	//" g.eg_id in (select distinct eg_id from t_pseudogene) ".
	" ORDER BY ga.left_end";

	$query_multi_address = $query_split_address;
//	$query_multi_address = "(".$query_multi_address.") union (".$query_split_address.") order by left_end";

	$rst_geneInfo2 = mysql_query($query_multi_address) or die("Query failed 280: " . mysql_error()."\n".$query_multi_address);
	$tempnum2 = mysql_num_rows($rst_geneInfo2);
	$t2 = 0;

	if($tempnum2>0)
	{
		$row2 = mysql_fetch_array($rst_geneInfo2, MYSQL_ASSOC);
	}

	$tempnum1 = mysql_num_rows($rst_geneInfo);
	$t1 = 0;
	while ($row = mysql_fetch_array($rst_geneInfo, MYSQL_ASSOC))
	{   $t1=$t1+1;
	//	$status = isPseudogene($row['status']);
	if (in_array($row['eg_id'], $split_eg)){
		$status = 'PSEUDO';
	} else {
		$status = isPseudogene($row['eg_id']);
	}
	
	
	
	$imx = imagesx($im)-$margin;
	$imy = imagesy($im);
	$is_left_include = true;
	$is_right_include = true;
	$gene_left_end = $row['left_end'];
	$gene_right_end = $row['right_end'];
	if($row['left_end']<$left_end){
		$gene_left_end =$left_end;
		$is_left_include = false;
	}

	if($row['right_end']>$right_end){
		$gene_right_end = $right_end;
		$is_right_include = false;
	}
	$x1 = intval(($gene_left_end-$left_end)/$gene_neighbor*$imx)+$left_margin;
	$x2 = intval(($gene_right_end-$left_end)/$gene_neighbor*$imx)+$left_margin;

	
	//	echo $row['left_end']."<br>";
	while($tempnum2>0 && $t2< $tempnum2 && (($row2['left_end']<=$row['left_end'])||($t1 == $tempnum1)))
	{
		//		echo "I am outloop here <br>";
		if($row2['left_end']!=$row['left_end'])
		{
			//			echo $row['left_end']."<br>".$row2['left_end'].$row2['eg_id'].$row2['name'].$row2['orientation']."<br>";
			$num_gene=$num_gene+1;
			$is_left_include2 = true;
			$is_right_include2 = true;
			$gene_left_end2 = $row2['left_end'];
			$gene_right_end2 = $row2['right_end'];
			if($row2['left_end']<$left_end){
				$gene_left_end2 =$left_end;
				$is_left_include2 = false;
			}

			if($row2['right_end']>$right_end){
				$gene_right_end2 = $right_end;
				$is_right_include2 = false;
			}
			$x12 = intval(($gene_left_end2-$left_end)/$gene_neighbor*$imx)+$left_margin;
			$x22 = intval(($gene_right_end2-$left_end)/$gene_neighbor*$imx)+$left_margin;

			$map[$num_gene]['x1'] = $x12;
			$map[$num_gene]['x2'] = $x22;
			$map[$num_gene]['eg_id'] = $row2['eg_id'];
//			$map[$num_gene]['y1'] = 20;
//			$map[$num_gene]['y2'] = 75;
			$map[$num_gene]['link'] = 'geneInfo.php';
			$map[$num_gene]['title'] = 'Go to Gene Page';
			$map[$num_gene]['id_name'] = 'eg_id';
			$map[$num_gene]['id'] = $row2['eg_id'];
//			if ($map[$num_gene]['multi_location']!='splitgene' && $map[$num_gene]['multi_location']!='split_isgene')//means it is multi_location IS genes
//			{
//				$map[$num_gene]['extra_link'] = "&add_left_end=".$row2['left_end']."&add_right_end=".$row2['right_end'];
//			}
			$flag = $flag+1;
			if (in_array($row2['eg_id'], $split_eg)){
				$status2 = isPseudogeneRules($row2['eg_id'],$row2['left_end']); 
			}
			else {
				$status2 = isPseudogene($row2['eg_id']);
			}
			if(($flag%2)==0){
				drawOneGene($im, $x12,$line_y2,$x22,$line_y2,$row2['name'], $row2['orientation'],  $line_size,$text_size,$is_left_include2,$is_right_include2,$row2['type'], $status2);
				$map[$num_gene]['y1'] = 50;
				$map[$num_gene]['y2'] = 75;
			}
			else {
				drawOneGene($im, $x12,$line_y1,$x22,$line_y1, $row2['name'], $row2['orientation'], $line_size,$text_size,$is_left_include2,$is_right_include2,$row2['type'], $status2);
				$map[$num_gene]['y1'] = 20;
				$map[$num_gene]['y2'] = 49;
			}
		}
		$t2 = $t2+1;
		$row2 = mysql_fetch_array($rst_geneInfo2, MYSQL_ASSOC);
	}
	$num_gene=$num_gene+1;
	$map[$num_gene]['x1'] = $x1;
	$map[$num_gene]['x2'] = $x2;
	//	$map[$num_gene]['eg_id'] = $row['eg_id'];
//	$map[$num_gene]['y1'] = 20;
//	$map[$num_gene]['y2'] = 75;
	$map[$num_gene]['link'] = 'geneInfo.php';
	$map[$num_gene]['title'] = 'Go to Gene Page';
	$map[$num_gene]['id_name'] = 'eg_id';
	$map[$num_gene]['id'] = $row['eg_id'];
	
	$flag = $flag+1;
	if(($flag%2)==0){
		drawOneGene($im, $x1,$line_y2,$x2,$line_y2,$row['name'], $row['orientation'],  $line_size,$text_size,$is_left_include,$is_right_include,$row['type'], $status);
		$map[$num_gene]['y1'] = 50;
		$map[$num_gene]['y2'] = 75;
	}
	else {
		drawOneGene($im, $x1,$line_y1,$x2,$line_y1, $row['name'], $row['orientation'], $line_size,$text_size,$is_left_include,$is_right_include,$row['type'], $status);
		$map[$num_gene]['y1'] = 20;
		$map[$num_gene]['y2'] = 49;

	}
	}

	if($right_end>$GENOME_LENGTH)
	{
		//	$query = 'SELECT '.
		//	'g.eg_id, g.name, g.description, g.status, g.comments, g.type, '.
		//	'g.type, g.length, g.mnemonic_name, ga.orientation, ga.left_end, ga.right_end '.
		//	'FROM '.
		//	't_gene g, t_gene_address ga '.
		//	'WHERE '.
		//	'g.eg_id = ga.eg_id '.
		//	'AND '.
		//	"ga.left_end < $right_end-$GENOME_LENGTH  ORDER BY ga.left_end";
		$query = 'SELECT '.
		'g.eg_id, g.name, g.description, g.status, g.comments, g.type, '.
		'g.type, g.length, g.mnemonic_name, ta.orientation, ta.left_end, ta.right_end '.
		'FROM '.
		't_gene g, t_address ta '.
		'WHERE '.
		" (ta.id_type = 'aa' OR ta.id_type = 'nt' ) AND ".
		'g.address_id = ta.address_id '.
		'AND '.
		"ta.left_end < $right_end- $GENOME_LENGTH  ORDER BY ta.left_end";
		// avoid using t_gene_address table, used t_address instead
		// Dian Fan 5/25/07
		// pass test in Navicat, the query results are identical.
		// the query is saved in Navicat as Name="genemap_genemap_q4"

		$rst_geneInfo = mysql_query($query) or die("Query failed : " . mysql_error());
		$tempnum = mysql_num_rows($rst_geneInfo);

		//echo $right_end;

		while ($row = mysql_fetch_array($rst_geneInfo, MYSQL_ASSOC))
		{

			$status = isPseudogene($row['eg_id']);
			$num_gene=$num_gene+1;
			$imx = imagesx($im)-$margin;
			$imy = imagesy($im);
			$is_left_include = true;
			$is_right_include = true;

			$gene_left_end = $row['left_end'];
			$gene_right_end = $row['right_end'];

			if($row['right_end']> $right_end-$GENOME_LENGTH)	{
				$gene_right_end = $right_end-$GENOME_LENGTH;
				$is_right_include = false;
			}

			$x1 = intval(($gene_left_end+$GENOME_LENGTH-$left_end)/$gene_neighbor*$imx)+$left_margin;
			$x2 = intval(($gene_right_end+$GENOME_LENGTH-$left_end)/$gene_neighbor*$imx)+$left_margin;

			$map[$num_gene]['x1'] = $x1;
			$map[$num_gene]['x2'] = $x2;
			//		$map[$num_gene]['eg_id'] = $row['eg_id'];
//			$map[$num_gene]['y1'] = 20;
//			$map[$num_gene]['y2'] = 75;
			$map[$num_gene]['link'] = 'geneInfo.php';
			$map[$num_gene]['title'] = 'Go to Gene Page';
			$map[$num_gene]['id_name'] = 'eg_id';
			$map[$num_gene]['id'] = $row['eg_id'];

			$flag = $flag+1;
			if(($flag%2)==0){
				drawOneGene($im, $x1,$line_y2,$x2,$line_y2,$row['name'], $row['orientation'],  $line_size,$text_size,$is_left_include,$is_right_include,$row['type'], $status);
				$map[$num_gene]['y1'] = 50;
				$map[$num_gene]['y2'] = 75;
			}
			else {
				drawOneGene($im, $x1,$line_y1,$x2,$line_y1, $row['name'], $row['orientation'],  $line_size,$text_size,$is_left_include,$is_right_include,$row['type'], $status);
				$map[$num_gene]['y1'] = 20;
				$map[$num_gene]['y2'] = 49;

			}
		}

	}

	// disable the lines below if you want to turn off regulonDB info

	imagettftext ($im, $text_size2, 0, $left_margin+430,$hig+6,  $text_color, $fontfile,"TF site" );
	imagefilledrectangle($im, $left_margin+469,$hig, $left_margin+494, $hig+6, $line_color_tfsite);
	tfbs_address($left_end,$right_end,$line_y3,$gene_neighbor,$map);

	is_address_2($left_end,$right_end,$line_y4,$gene_neighbor,$map);

is_address($left_end,$right_end,$line_y3,$gene_neighbor,$map);

	return $gene_name;
}

function isPseudogeneRules($eg_id, $left_end)
{
	$query_split = " SELECT * FROM t_gene_split_address WHERE  eg_id = '$eg_id' order by left_end";
	$result_split = mysql_query($query_split);
	$row_split = mysql_fetch_array($result_split, MYSQL_ASSOC);
	$min_left_end = $row_split['left_end'];
	$max_left_end = $row_split['left_end'];
	while ($row_split = mysql_fetch_array($result_split, MYSQL_ASSOC)) {
		if($min_left_end > $row_split['left_end'])
		{
			$min_left_end = $row_split['left_end'];
		}
		if($max_left_end < $row_split['left_end'])
		{
			$max_left_end = $row_split['left_end'];
		}
	}
	mysql_free_result($result_split);
	$status='PSEUDO';
	if($left_end>$min_left_end)
	{
		$status=$status.'L';
	}
	if($left_end<$max_left_end)
	{
		$status=$status.'R';
	}	
	return $status;
}
/*
function isPseudogene($gene_status)
{
$array= explode(";", $gene_status);
$count = count($array);
if(strncmp($array[0],"OK",2)==0 || strncmp($array[0],"CORR",4)==0
|| strncmp($array[0],"ALT_INIT",8)==0 || strncmp($array[0],"EXCEP",5)==0)
{

$status = "OK";
}
else
{
for ($j = 1; $j < $count; $j++)
{
if(strncmp($array[$j],"OK",2)==0)
{
$status = "OK"; break;
}
if(strncmp($array[$j],"CORR",4)==0)
{
$status = "OK"; break;
}
if(strncmp($array[$j],"EXCEP",5)==0)
{
$status = "OK"; break;
}
}
if($j == $count)
{

$status = "PSEUDO";
}
}

Return $status;
}
*/
function drawOneGene($im, $x1, $y1, $x2, $y2, $name, $orientation, $line_size,$text_size,$is_left_include,$is_right_include,$type, $status){
	global $fontfile;
	global $line_color_pseudo;
	global $line_color_cw;
	global $line_color_ccw;
	global $line_color_rna;
	global $line_color_tfsite;
	global $line_color_is;
	if(!strncmp($status,'PSEUDO',6))
	{
		$line_color = $line_color_pseudo;
	}
	elseif((!strcmp($status,'Y'))|| (!strcmp($status,'N')))
	{
		$line_color = $line_color_is;
	}
	elseif((!strcmp($status,'tfbs')))
	{
		$line_color = $line_color_tfsite;
	}
	elseif(strcmp($type,'aa'))
	{
		$line_color = $line_color_rna;
	}
	elseif(!strcmp($orientation,'Clockwise'))
	{
		$line_color = $line_color_cw;
	}
	else
	{
		$line_color = $line_color_ccw;
	}
	//	imagestring ($im, 3, (intval($x1+$x2)/2)-13, $y1+5,  $name, $line_color);
	if(!strcmp($status,'PSEUDO')){
		imagettftext ($im, $text_size, 0, (intval($x1+$x2)/2)-13, $y1+15, $line_color, $fontfile, $name."'");
		//		imagettftext ($im, $text_size, 0, (intval($x1+$x2)/2)-13, $y1+15, $line_color, $fontfile, $name);
	}
	elseif(!strcmp($status,'PSEUDOL'))
	{
		imagettftext ($im, $text_size, 0, (intval($x1+$x2)/2)-13, $y1+15, $line_color, $fontfile, "'".$name);
	}
	elseif (!strcmp($status,'PSEUDOR'))
	{
		imagettftext ($im, $text_size, 0, (intval($x1+$x2)/2)-13, $y1+15, $line_color, $fontfile, $name."'");
	}
	elseif (!strcmp($status,'PSEUDOLR'))
	{
		imagettftext ($im, $text_size, 0, (intval($x1+$x2)/2)-13, $y1+15, $line_color, $fontfile, "'".$name."'");
	}
	else{
		imagettftext ($im, $text_size, 0, (intval($x1+$x2)/2)-13, $y1+15, $line_color, $fontfile, $name);
	}
//	imagelinethick ( $im, $x1, $y1, $x2, $y2, $line_color,$line_size);
	if(!strcmp($orientation,'Clockwise')){

		if($is_left_include){
			imagelinethick ( $im, $x1, $y1-1.5*$line_size-1, $x1, $y1+1.5*$line_size, $line_color,$line_size );
		}
		imagelinethick ( $im, $x1, $y1, $x2-2*$line_size, $y2, $line_color,$line_size);
		arrow($im, $x2-2*$line_size, $y1, $x2, $y1, $line_size*2, $line_size*2, $line_color);
	}
	else if(!strcmp($orientation,'Counterclockwise')){

		if($is_right_include){
			imagelinethick ( $im, $x2, $y1-1.5*$line_size-1, $x2, $y1+1.5*$line_size, $line_color,$line_size );
		}
		//	echo "(".$x1." ".$y1.")"."<br>(".$x2." ".$y2.")<br>";
		imagelinethick ( $im, $x1+2*$line_size, $y1, $x2, $y2, $line_color,$line_size);
		arrow($im, $x1+2*$line_size, $y1, $x1, $y1,  $line_size*2, $line_size*2, $line_color);
		
	}else if(!strcmp($orientation,'Bidirectional')){
		
		imagelinethick ( $im, $x1+2*$line_size, $y1, $x2-2*$line_size, $y2, $line_color,$line_size);
		arrow($im, $x2-2*$line_size, $y1, $x2, $y1, $line_size*2, $line_size*2, $line_color);
		arrow($im, $x1+2*$line_size, $y1, $x1, $y1,  $line_size*2, $line_size*2, $line_color);

	}else{

		imagelinethick ( $im, $x1, $y1, $x2, $y2, $line_color,$line_size);
	}

	//imagelinethick($im, 5,50,200,50, $text_color, $thick = 2);
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
	//	echo "2. (".$x2." ".$y2.")"."<br>3. (".$x3." ".$y3.")<br>"."4. (".$x4." ".$y4.")<br>";
	//  imageline($im, $x1, $y1, $dx, $dy, $color);
	//  imageline($im, $x3, $y3, $x4, $y4, $color);
	//  imageline($im, $x3, $y3, $x2, $y2, $color);
	//  imageline($im, $x2, $y2, $x4, $y4, $color);
	$points = array(
	$x3, $y3,
	$x4, $y4,
	$x2, $y2,

	);
	imagefilledpolygon($im, $points, 3, $color);

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

function is_address($left_end, $right_end,$line_y3,$gene_neighbor, &$map)
{
	//echo $GENOME_LENGTH;
	$DBtable = 't_is_address';
	$Address = 't_address';
	global $GENOME_LENGTH;
	global $margin,$left_margin;
	global $imx,$imy,$im;

	global $text_size,$line_size;
	$imx_sx = imagesx($im)-$margin;
	$imy_sy = $imy;
	$num_gene=count($map);
	if($left_end<0)
	{	$query = 'SELECT '.	'ga.id, ga.name, ga.partial, ga.instance, ga.topic_id, address.* '.	'FROM '.	$DBtable.' ga ,'. $Address	. ' address '. 'WHERE '.
	" address.address_id=ga.address_id  and "."address.right_end > $GENOME_LENGTH + $left_end "
	." and (address.id_type!='is' and address.id_type!='prophage' and address.id_type!='line2' and address.id_type!='rig') "
	." ORDER BY (address.right_end - address.left_end) ";


	$rst_geneInfo = mysql_query($query) or die($query."Query failed : <br>" . mysql_error());
	$tempnum = mysql_num_rows($rst_geneInfo);

	while ($row = mysql_fetch_array($rst_geneInfo, MYSQL_ASSOC))
	{

		$status = $row['partial'];
		$type = '';
		//		$num_gene=$num_gene+1;

		$is_left_include = true;
		$is_right_include = true;

		$gene_left_end = $row['left_end'];
		$gene_right_end = $row['right_end'];

		if($row['left_end']<$GENOME_LENGTH + $left_end)	{
			$gene_left_end = $GENOME_LENGTH + $left_end;
			$is_left_include = false;
		}

		$x1 = intval(($gene_left_end-($GENOME_LENGTH+$left_end))/$gene_neighbor*$imx_sx)+$left_margin;
		$x2 = intval(($gene_right_end-($GENOME_LENGTH+$left_end))/$gene_neighbor*$imx_sx)+$left_margin;

		drawOneGene($im, $x1,$line_y3,$x2,$line_y3, $row['name'].$row['instance'], $row['orientation'],  $line_size,$text_size,$is_left_include,$is_right_include,$type, $status);
		$map[$num_gene]['x1'] = $x1;
		$map[$num_gene]['x2'] = $x2;
		$map[$num_gene]['y1'] = $line_y3-3;
		$map[$num_gene]['y2'] = $line_y3+12;
				
//		if(($gene_right_end-$gene_left_end)<300)
		if(($gene_right_end-$gene_left_end)<600)
		{
			$map[$num_gene]['link'] = 'regulon.php';
			$map[$num_gene]['title'] = 'Detaied Map Information';
			
			$query_min="SELECT t.eg_id, ABS(".$gene_left_end." - ta.left_end) as  dis ".

					"FROM t_gene t, t_address ta ".
					
					"WHERE t.address_id = ta.address_id ".
					
					"ORDER BY dis";

			$resultSet= mysql_query($query_min) or die("Query failed : " . mysql_error());

			$rowdata = mysql_fetch_array($resultSet, MYSQL_ASSOC);

			$egid = $rowdata["eg_id"];		
			
			$map[$num_gene]['id_name'] = 'eg_id';
			$map[$num_gene]['id'] = $egid;
			
//			$map[$num_gene]['id_name'] = 'is_id';
//			$map[$num_gene]['id'] = $row['id'];
			$num_gene=$num_gene+1;
		}
		else 
		{
			$map[$num_gene]['link'] = 'topic.php';
			$map[$num_gene]['title'] = 'Topic Page';
			$map[$num_gene]['id_name'] = 'topic_id';
			$map[$num_gene]['id'] = $row['topic_id'];
			$num_gene=$num_gene+1;
		}		
//		$map[$num_gene]['link'] = 'regulon.php';
//		$map[$num_gene]['title'] = 'Detaied Map Information';
//		$map[$num_gene]['id_name'] = 'is_id';
//		$map[$num_gene]['id'] = $row['id'];
//		$num_gene=$num_gene+1;
	}

	}

	$query = 'SELECT '.	'ga.id, ga.name, ga.partial, ga.instance, ga.topic_id, address.* '.	' FROM '.	$DBtable.' ga ,'. $Address	. ' address '.
	'WHERE '.
	" address.address_id=ga.address_id  and ".
	"address.right_end > $left_end AND address.left_end < $right_end "
	." and (address.id_type!='is' and address.id_type!='prophage' and address.id_type!='line2' and address.id_type!='rig') "
	." ORDER BY (address.right_end - address.left_end) ";

	$rst_geneInfo = mysql_query($query) or die($query."Query failed 595: <br>" . mysql_error());
	$tempnum = mysql_num_rows($rst_geneInfo);


	while ($row = mysql_fetch_array($rst_geneInfo, MYSQL_ASSOC))
	{


		$status = $row['partial'];
		$type = '';
		//	$num_gene=$num_gene+1;

		$is_left_include = true;
		$is_right_include = true;
		$gene_left_end = $row['left_end'];
		$gene_right_end = $row['right_end'];
		if($row['left_end']<$left_end){
			$gene_left_end =$left_end;
			$is_left_include = false;
		}

		if($row['right_end']>$right_end){
			$gene_right_end = $right_end;
			$is_right_include = false;
		}
		$x1 = intval(($gene_left_end-$left_end)/$gene_neighbor*$imx_sx)+$left_margin;
		$x2 = intval(($gene_right_end-$left_end)/$gene_neighbor*$imx_sx)+$left_margin;
		drawOneGene($im, $x1,$line_y3,$x2,$line_y3, $row['name'].$row['instance'], $row['orientation'],  $line_size,$text_size,$is_left_include,$is_right_include,$type, $status);
		$map[$num_gene]['x1'] = $x1;
		$map[$num_gene]['x2'] = $x2;
		$map[$num_gene]['y1'] = $line_y3-3;
		$map[$num_gene]['y2'] = $line_y3+12;
		
//				if(($gene_right_end-$gene_left_end)<300)
		if(($gene_right_end-$gene_left_end)<600)
		{
			$map[$num_gene]['link'] = 'regulon.php';
			$map[$num_gene]['title'] = 'Detaied Map Information';
			$query_min="SELECT t.eg_id, ABS(".$gene_left_end." - ta.left_end) as  dis ".

					"FROM t_gene t, t_address ta ".
					
					"WHERE t.address_id = ta.address_id ".
					
					"ORDER BY dis";

			$resultSet= mysql_query($query_min) or die("Query failed : " . mysql_error());

			$rowdata = mysql_fetch_array($resultSet, MYSQL_ASSOC);

			$egid = $rowdata["eg_id"];		
			
			$map[$num_gene]['id_name'] = 'eg_id';
			$map[$num_gene]['id'] = $egid;
			
//			$map[$num_gene]['id_name'] = 'is_id';
//			$map[$num_gene]['id'] = $row['id'];
			$num_gene=$num_gene+1;
		}
		else 
		{
			$map[$num_gene]['link'] = 'topic.php';
			$map[$num_gene]['title'] = 'Topic Page';
			$map[$num_gene]['id_name'] = 'topic_id';
			$map[$num_gene]['id'] = $row['topic_id'];
			$num_gene=$num_gene+1;
		}
//		$map[$num_gene]['link'] = 'regulon.php';
//		$map[$num_gene]['title'] = 'Detaied Map Information';
//		$map[$num_gene]['id_name'] = 'is_id';
//		$map[$num_gene]['id'] = $row['id'];
//		$num_gene=$num_gene+1;
		//		echo $x1." ". $line_y3." ".$x2." ". $row['name']." ".$row['orientation']. " ".$line_size. " ".$text_size." ".$is_left_include. " " .$is_right_include. " ".$type." ".$status;

	}

	if($right_end>$GENOME_LENGTH)
	{
		$query = 'SELECT '.	'ga.id, ga.name, ga.partial, ga.instance, ga.topic_id, address.*  '.	'FROM '.	$DBtable.' ga ,'. $Address	. ' address '.
		'WHERE '.
		" address.address_id=ga.address_id  and ".
		"address.left_end < $right_end-$GENOME_LENGTH  "
	." and (address.id_type!='is' and address.id_type!='prophage' and address.id_type!='line2' and address.id_type!='rig') "
	." ORDER BY (address.right_end - address.left_end) ";

		$rst_geneInfo = mysql_query($query) or die($query."Query failed 634: <br>" . mysql_error());
		$tempnum = mysql_num_rows($rst_geneInfo);

		//echo $right_end;

		while ($row = mysql_fetch_array($rst_geneInfo, MYSQL_ASSOC))
		{
			$status = $row['partial'];
			$type = '';
			//	$num_gene=$num_gene+1;

			$is_left_include = true;
			$is_right_include = true;

			$gene_left_end = $row['left_end'];
			$gene_right_end = $row['right_end'];

			if($row['right_end']> $right_end-$GENOME_LENGTH)	{
				$gene_right_end = $right_end-$GENOME_LENGTH;
				$is_right_include = false;
			}

			$x1 = intval(($gene_left_end+$GENOME_LENGTH-$left_end)/$gene_neighbor*$imx_sx)+$left_margin;
			$x2 = intval(($gene_right_end+$GENOME_LENGTH-$left_end)/$gene_neighbor*$imx_sx)+$left_margin;

			drawOneGene($im, $x1,$line_y3,$x2,$line_y3, $row['name'].$row['instance'], $row['orientation'],  $line_size,$text_size,$is_left_include,$is_right_include,$type, $status);
			$map[$num_gene]['x1'] = $x1;
			$map[$num_gene]['x2'] = $x2;
			$map[$num_gene]['y1'] = $line_y3-3;
			$map[$num_gene]['y2'] = $line_y3+12;
			
//		if(($gene_right_end-$gene_left_end)<300)
		if(($gene_right_end-$gene_left_end)<600)
		{
			$map[$num_gene]['link'] = 'regulon.php';
			$map[$num_gene]['title'] = 'Detaied Map Information';
			$query_min="SELECT t.eg_id, ABS(".$gene_left_end." - ta.left_end) as  dis ".

					"FROM t_gene t, t_address ta ".
					
					"WHERE t.address_id = ta.address_id ".
					
					"ORDER BY dis";

			$resultSet= mysql_query($query_min) or die("Query failed : " . mysql_error());

			$rowdata = mysql_fetch_array($resultSet, MYSQL_ASSOC);

			$egid = $rowdata["eg_id"];		
			
			$map[$num_gene]['id_name'] = 'eg_id';
			$map[$num_gene]['id'] = $egid;
//			$map[$num_gene]['id_name'] = 'is_id';
//			$map[$num_gene]['id'] = $row['id'];
			$num_gene=$num_gene+1;
		}
		else 
		{
			$map[$num_gene]['link'] = 'topic.php';
			$map[$num_gene]['title'] = 'Topic Page';
			$map[$num_gene]['id_name'] = 'topic_id';
			$map[$num_gene]['id'] = $row['topic_id'];
			$num_gene=$num_gene+1;
		}
//			$map[$num_gene]['link'] = 'regulon.php';
//			$map[$num_gene]['title'] = 'Detaied Map Information';
//			$map[$num_gene]['id_name'] = 'is_id';
//			$map[$num_gene]['id'] = $row['id'];
//			$num_gene=$num_gene+1;
		}

	}
}
function is_address_2($left_end, $right_end,$line_y3,$gene_neighbor, &$map)
{
	//echo $GENOME_LENGTH;
	$DBtable = 't_is_address';
	$Address = 't_address';
	global $GENOME_LENGTH;
	global $margin,$left_margin;
	global $imx,$imy,$im;

	global $text_size,$line_size;
	$imx_sx = imagesx($im)-$margin;
	$imy_sy = $imy;
	$num_gene=count($map);
	if($left_end<0)
	{	$query = 'SELECT '.	'ga.id, ga.name, ga.partial, ga.instance, ga.topic_id, address.* '.	'FROM '.	$DBtable.' ga ,'. $Address	. ' address '. 'WHERE '.
	" address.address_id=ga.address_id  and "."address.right_end > $GENOME_LENGTH + $left_end "
	." and (address.id_type='is' or address.id_type='prophage' or address.id_type='line2' or address.id_type='rig') "
	." ORDER BY address.left_end ";


	$rst_geneInfo = mysql_query($query) or die($query."Query failed : <br>" . mysql_error());
	$tempnum = mysql_num_rows($rst_geneInfo);

	while ($row = mysql_fetch_array($rst_geneInfo, MYSQL_ASSOC))
	{

		$status = $row['partial'];
		$type = '';
		//		$num_gene=$num_gene+1;

		$is_left_include = true;
		$is_right_include = true;

		$gene_left_end = $row['left_end'];
		$gene_right_end = $row['right_end'];

		if($row['left_end']<$GENOME_LENGTH + $left_end)	{
			$gene_left_end = $GENOME_LENGTH + $left_end;
			$is_left_include = false;
		}

		$x1 = intval(($gene_left_end-($GENOME_LENGTH+$left_end))/$gene_neighbor*$imx_sx)+$left_margin;
		$x2 = intval(($gene_right_end-($GENOME_LENGTH+$left_end))/$gene_neighbor*$imx_sx)+$left_margin;

		drawOneGene($im, $x1,$line_y3,$x2,$line_y3, $row['name'].$row['instance'], $row['orientation'],  $line_size,$text_size,$is_left_include,$is_right_include,$type, $status);
		$map[$num_gene]['x1'] = $x1;
		$map[$num_gene]['x2'] = $x2;
		$map[$num_gene]['y1'] = $line_y3-3;
		$map[$num_gene]['y2'] = $line_y3+12;		
		$map[$num_gene]['left_end'] = $gene_left_end;
		$map[$num_gene]['right_end'] = $gene_right_end;
// if the length is less than the 600 the size or regulon map, show intergenic map
// otherwise go to topic page directly	
		if(($gene_right_end-$gene_left_end)<300)
		{
			$map[$num_gene]['link'] = 'regulon.php';
			$map[$num_gene]['title'] = 'Detaied Map Information';
			$query_min="SELECT t.eg_id, ABS(".$gene_left_end." - ta.left_end) as  dis ".

					"FROM t_gene t, t_address ta ".
					
					"WHERE t.address_id = ta.address_id ".
					
					"ORDER BY dis";

			$resultSet= mysql_query($query_min) or die("Query failed : " . mysql_error());

			$rowdata = mysql_fetch_array($resultSet, MYSQL_ASSOC);

			$egid = $rowdata["eg_id"];		
			
			$map[$num_gene]['id_name'] = 'eg_id';
			$map[$num_gene]['id'] = $egid;
//			$map[$num_gene]['id_name'] = 'is_id';
//			$map[$num_gene]['id'] = $row['id'];
			$num_gene=$num_gene+1;
		}
		else 
		{
			$map[$num_gene]['link'] = 'topic.php';
			$map[$num_gene]['title'] = 'Topic Page';
			$map[$num_gene]['id_name'] = 'topic_id';
			$map[$num_gene]['id'] = $row['topic_id'];
			$num_gene=$num_gene+1;
		}
	}

	}

	$query = 'SELECT '.	'ga.id, ga.name, ga.partial, ga.instance, ga.topic_id, address.* '.	' FROM '.	$DBtable.' ga ,'. $Address	. ' address '.
	'WHERE '.
	" address.address_id=ga.address_id  and ".
	"address.right_end > $left_end AND address.left_end < $right_end "
	." and (address.id_type='is' or address.id_type='prophage'  or address.id_type='line2' or address.id_type='rig') "
	." ORDER BY address.left_end ";

	$rst_geneInfo = mysql_query($query) or die($query."Query failed 595: <br>" . mysql_error());
	$tempnum = mysql_num_rows($rst_geneInfo);


	while ($row = mysql_fetch_array($rst_geneInfo, MYSQL_ASSOC))
	{


		$status = $row['partial'];
		$type = '';
		//	$num_gene=$num_gene+1;

		$is_left_include = true;
		$is_right_include = true;
		$gene_left_end = $row['left_end'];
		$gene_right_end = $row['right_end'];
		if($row['left_end']<$left_end){
			$gene_left_end =$left_end;
			$is_left_include = false;
		}

		if($row['right_end']>$right_end){
			$gene_right_end = $right_end;
			$is_right_include = false;
		}
		$x1 = intval(($gene_left_end-$left_end)/$gene_neighbor*$imx_sx)+$left_margin;
		$x2 = intval(($gene_right_end-$left_end)/$gene_neighbor*$imx_sx)+$left_margin;
		drawOneGene($im, $x1,$line_y3,$x2,$line_y3, $row['name'].$row['instance'], $row['orientation'],  $line_size,$text_size,$is_left_include,$is_right_include,$type, $status);
		$map[$num_gene]['x1'] = $x1;
		$map[$num_gene]['x2'] = $x2;
		$map[$num_gene]['y1'] = $line_y3-3;
		$map[$num_gene]['y2'] = $line_y3+12;
		
		$map[$num_gene]['left_end'] = $gene_left_end;
		$map[$num_gene]['right_end'] = $gene_right_end;
		// if the length is less than the 600 the size or regulon map, show intergenic map
// otherwise go to topic page directly	
		if(($gene_right_end-$gene_left_end)<300)
		{
			$map[$num_gene]['link'] = 'regulon.php';
			$map[$num_gene]['title'] = 'Detaied Map Information';
			$query_min="SELECT t.eg_id, ABS(".$gene_left_end." - ta.left_end) as  dis ".

					"FROM t_gene t, t_address ta ".
					
					"WHERE t.address_id = ta.address_id ".
					
					"ORDER BY dis";

			$resultSet= mysql_query($query_min) or die("Query failed : " . mysql_error());

			$rowdata = mysql_fetch_array($resultSet, MYSQL_ASSOC);

			$egid = $rowdata["eg_id"];		
			
			$map[$num_gene]['id_name'] = 'eg_id';
			$map[$num_gene]['id'] = $egid;
			
//			$map[$num_gene]['id_name'] = 'is_id';
//			$map[$num_gene]['id'] = $row['id'];
			$num_gene=$num_gene+1;
		}
		else 
		{
			$map[$num_gene]['link'] = 'topic.php';
			$map[$num_gene]['title'] = 'Topic Page';
			$map[$num_gene]['id_name'] = 'topic_id';
			$map[$num_gene]['id'] = $row['topic_id'];
			$num_gene=$num_gene+1;
		}
		
		//		echo $x1." ". $line_y3." ".$x2." ". $row['name']." ".$row['orientation']. " ".$line_size. " ".$text_size." ".$is_left_include. " " .$is_right_include. " ".$type." ".$status;

	}

	if($right_end>$GENOME_LENGTH)
	{
		$query = 'SELECT '.	'ga.id, ga.name, ga.partial, ga.instance, ga.topic_id, address.*  '.	'FROM '.	$DBtable.' ga ,'. $Address	. ' address '.
		'WHERE '.
		" address.address_id=ga.address_id  and ".
		"address.left_end < $right_end-$GENOME_LENGTH  "
	." and (address.id_type='is' or address.id_type='prophage' or address.id_type='line2' or address.id_type='rig') "
	." ORDER BY address.left_end ";

		$rst_geneInfo = mysql_query($query) or die($query."Query failed 634: <br>" . mysql_error());
		$tempnum = mysql_num_rows($rst_geneInfo);

		//echo $right_end;

		while ($row = mysql_fetch_array($rst_geneInfo, MYSQL_ASSOC))
		{
			$status = $row['partial'];
			$type = '';
			//	$num_gene=$num_gene+1;

			$is_left_include = true;
			$is_right_include = true;

			$gene_left_end = $row['left_end'];
			$gene_right_end = $row['right_end'];

			if($row['right_end']> $right_end-$GENOME_LENGTH)	{
				$gene_right_end = $right_end-$GENOME_LENGTH;
				$is_right_include = false;
			}

			$x1 = intval(($gene_left_end+$GENOME_LENGTH-$left_end)/$gene_neighbor*$imx_sx)+$left_margin;
			$x2 = intval(($gene_right_end+$GENOME_LENGTH-$left_end)/$gene_neighbor*$imx_sx)+$left_margin;

			drawOneGene($im, $x1,$line_y3,$x2,$line_y3, $row['name'].$row['instance'], $row['orientation'],  $line_size,$text_size,$is_left_include,$is_right_include,$type, $status);
			$map[$num_gene]['x1'] = $x1;
			$map[$num_gene]['x2'] = $x2;
			$map[$num_gene]['y1'] = $line_y3-3;
			$map[$num_gene]['y2'] = $line_y3+12;
			
		$map[$num_gene]['left_end'] = $gene_left_end;
		$map[$num_gene]['right_end'] = $gene_right_end;
	// if the length is less than the 600 the size or regulon map, show intergenic map
// otherwise go to topic page directly	
		if(($gene_right_end-$gene_left_end)<300)
		{
			$map[$num_gene]['link'] = 'regulon.php';
			$map[$num_gene]['title'] = 'Detaied Map Information';
			$query_min="SELECT t.eg_id, ABS(".$gene_left_end." - ta.left_end) as  dis ".

					"FROM t_gene t, t_address ta ".
					
					"WHERE t.address_id = ta.address_id ".
					
					"ORDER BY dis";

			$resultSet= mysql_query($query_min) or die("Query failed : " . mysql_error());

			$rowdata = mysql_fetch_array($resultSet, MYSQL_ASSOC);

			$egid = $rowdata["eg_id"];		
			
			$map[$num_gene]['id_name'] = 'eg_id';
			$map[$num_gene]['id'] = $egid;
//			
//			$map[$num_gene]['id_name'] = 'is_id';
//			$map[$num_gene]['id'] = $row['id'];
			$num_gene=$num_gene+1;
		}
		else 
		{
			$map[$num_gene]['link'] = 'topic.php';
			$map[$num_gene]['title'] = 'Topic Page';
			$map[$num_gene]['id_name'] = 'topic_id';
			$map[$num_gene]['id'] = $row['topic_id'];
			$num_gene=$num_gene+1;
		}
			
		}

	}
}
function tfbs_address($left_end, $right_end,$line_y3,$gene_neighbor, &$map)
{
	//	 echo $GENOME_LENGTH;
	$DBtable = 't_tfbs';
	$Address = 't_address';
	global $margin,$left_margin;
	global $imx,$imy,$im;

	global $GENOME_LENGTH;

	global $text_size,$line_size;
	$imx_sx = imagesx($im)-$margin;
	$imy_sy = $imy;
	$num_gene=count($map);
	if($left_end<0)
	{	$query = "SELECT * FROM	$DBtable WHERE right_end > $GENOME_LENGTH + $left_end ORDER BY left_end";

	$rst_geneInfo = mysql_query($query) or die($query."Query failed : <br>" . mysql_error());
	$tempnum = mysql_num_rows($rst_geneInfo);

	while ($row = mysql_fetch_array($rst_geneInfo, MYSQL_ASSOC))
	{

		$status = 'tfbs';
		$type = '';
		//		$num_gene=$num_gene+1;

		$is_left_include = true;
		$is_right_include = true;

		$gene_left_end = $row['left_end'];
		$gene_right_end = $row['right_end'];

		if($row['left_end']<$GENOME_LENGTH + $left_end)	{
			$gene_left_end = $GENOME_LENGTH + $left_end;
			$is_left_include = false;
		}

		$x1 = intval(($gene_left_end-($GENOME_LENGTH+$left_end))/$gene_neighbor*$imx_sx)+$left_margin;
		$x2 = intval(($gene_right_end-($GENOME_LENGTH+$left_end))/$gene_neighbor*$imx_sx)+$left_margin;

		drawOneGene($im, $x1,$line_y3,$x2,$line_y3, '', $row['orientation'],  $line_size,$text_size,$is_left_include,$is_right_include,$type, $status);
		$map[$num_gene]['x1'] = $x1;
		$map[$num_gene]['x2'] = $x2;
		$map[$num_gene]['y1'] = $line_y3-3;
		$map[$num_gene]['y2'] = $line_y3+8;
		$map[$num_gene]['link'] = 'regulon.php';
		$map[$num_gene]['title'] = 'Go to RegulonDB';
		//			$map[$num_gene]['eg_id'] = $row['EG_target'];
		$map[$num_gene]['id_name'] = 'eg_id';
		$map[$num_gene]['id'] = $row['EG_target'];
		$num_gene=$num_gene+1;
	}

	}

	$query = "SELECT * FROM $DBtable WHERE right_end > $left_end AND left_end < $right_end ORDER BY left_end";

	$rst_geneInfo = mysql_query($query) or die($query."Query failed 595: <br>" . mysql_error());
	$tempnum = mysql_num_rows($rst_geneInfo);


	while ($row = mysql_fetch_array($rst_geneInfo, MYSQL_ASSOC))
	{


		$status = 'tfbs';
		$type = '';
		//	$num_gene=$num_gene+1;

		$is_left_include = true;
		$is_right_include = true;
		$gene_left_end = $row['left_end'];
		$gene_right_end = $row['right_end'];
		if($row['left_end']<$left_end){
			$gene_left_end =$left_end;
			$is_left_include = false;
		}

		if($row['right_end']>$right_end){
			$gene_right_end = $right_end;
			$is_right_include = false;
		}
		$x1 = intval(($gene_left_end-$left_end)/$gene_neighbor*$imx_sx)+$left_margin;
		$x2 = intval(($gene_right_end-$left_end)/$gene_neighbor*$imx_sx)+$left_margin;
		drawOneGene($im, $x1,$line_y3,$x2,$line_y3, '', $row['orientation'],  $line_size,$text_size,$is_left_include,$is_right_include,$type, $status);

		$map[$num_gene]['x1'] = $x1;
		$map[$num_gene]['x2'] = $x2;
		$map[$num_gene]['y1'] = $line_y3-3;
		$map[$num_gene]['y2'] = $line_y3+3;
		$map[$num_gene]['link'] = 'regulon.php';
		$map[$num_gene]['title'] = 'Go to RegulonDB';
		//			$map[$num_gene]['eg_id'] = $row['EG_target'];
		$map[$num_gene]['id_name'] = 'eg_id';
		$map[$num_gene]['id'] = $row['EG_target'];
		$num_gene=$num_gene+1;
	}

	if($right_end>$GENOME_LENGTH)
	{
		$query = "SELECT * FROM $DBtable WHERE left_end < $right_end-$GENOME_LENGTH  ORDER BY left_end";

		$rst_geneInfo = mysql_query($query) or die($query."Query failed 634: <br>" . mysql_error());
		$tempnum = mysql_num_rows($rst_geneInfo);

		//echo $right_end;

		while ($row = mysql_fetch_array($rst_geneInfo, MYSQL_ASSOC))
		{
			$status = 'tfbs';
			$type = '';
			//	$num_gene=$num_gene+1;

			$is_left_include = true;
			$is_right_include = true;

			$gene_left_end = $row['left_end'];
			$gene_right_end = $row['right_end'];

			if($row['right_end']> $right_end-$GENOME_LENGTH)	{
				$gene_right_end = $right_end-$GENOME_LENGTH;
				$is_right_include = false;
			}

			$x1 = intval(($gene_left_end+$GENOME_LENGTH-$left_end)/$gene_neighbor*$imx_sx)+$left_margin;
			$x2 = intval(($gene_right_end+$GENOME_LENGTH-$left_end)/$gene_neighbor*$imx_sx)+$left_margin;

			drawOneGene($im, $x1,$line_y3,$x2,$line_y3, '', $row['orientation'],  $line_size,$text_size,$is_left_include,$is_right_include,$type, $status);
			$map[$num_gene]['x1'] = $x1;
			$map[$num_gene]['x2'] = $x2;
			$map[$num_gene]['y1'] = $line_y3-3;
			$map[$num_gene]['y2'] = $line_y3+3;
			$map[$num_gene]['link'] = 'regulon.php';
			$map[$num_gene]['title'] = 'Go to RegulonDB';
			//			$map[$num_gene]['eg_id'] = $row['EG_target'];
			$map[$num_gene]['id_name'] = 'eg_id';
			$map[$num_gene]['id'] = $row['EG_target'];
			$num_gene=$num_gene+1;


		}

	}
}
 ?>
