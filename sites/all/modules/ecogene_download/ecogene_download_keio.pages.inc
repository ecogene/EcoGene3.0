<?PHP
include_once ("sites/all/include/ecoFunction.php");
define ("SEQ_LINE", 75);

global $GENOME_LENGTH;
global $tmpdr;
global $extension;
global $del_rbs;

$tmpdr = 'temp/';
set_time_limit(1000);

function download_keio_return()
{
	drupal_set_title("Keio Experimental Results");
	global $del_rbs;	
	global $extension;
	$del_rbs = $_REQUEST['del_rbs'];
//		print_r($_POST);
//	print_r($_FILES);
	db_set_active("ecogene");	
	
	$content = '';
	// process post paramenters
	// entension and overlap parameters
//	$extension = isset($_REQUEST['extention'])? $_REQUEST['extention']: 0;
//	$overlap_thre = isset($_REQUEST['overlap_thre'])? $_REQUEST['overlap_thre']: 0;
	
	
	
	if ($_REQUEST["extention"]) {
		
		$extension = $_POST["extention"];	
	}
	else {
		$extension = 0;
	}
	if ($_REQUEST["overlap_thre"]) {

		$overlap_thre = $_POST["overlap_thre"];
	
	}
	else {
		$overlap_thre = 0;
	}
	
		
	// get ecogene eck and eg ids

	$query = db_query("SELECT eg_id, eck_id FROM  t_gene");
	$num_gene = 0;
	while($row=$query->fetchAssoc())
	{
		$eg[$num_gene] = $row['eg_id'];
		$eck[$num_gene] = $row['eck_id'];
		$num_gene ++;
	}
	
	
	//keio file
	if($_REQUEST['default_file']=='keio')
	{
		$filename_tmp = "sites/all/modules/ecogene_download/keio";
	}
	elseif($_FILES['userfile']['tmp_name'])
	{
		$filename_tmp = $_FILES['userfile']['tmp_name'];
	}else 
	{
		$content .= "<em>File is not valid!</em>";
		db_set_active("default");	
		return $content;
	}
	//process the file
	// and check data integraty
	
	$del_eck = array();
	$del_none = array();
	$del_eck_wrong_end = array();
	$del_eck_nonfound = array();
	$del_eck_updated = array();
	$content .= read_keio_file($filename_tmp, $eg, $eck, $del_eck, $del_none, $del_eck_wrong_end, $del_eck_nonfound, $del_eck_updated);
	
	$gene = array();
	$gene_eg = array();
	keio_get_gene_info($gene, $gene_eg, $extension);
	
	$double_del = array();
	$wrong_del = array();
	$result = array();
	$tri_del = array();
	keio_deletion($gene, $gene_eg, $del_eck, $overlap_thre, 
	$double_del, $wrong_del, $result, $num_double, $tri_del);
//	echo $overlap_thre;
//	echo count($del_eck);
	//write experimental results
	if(strcmp($del_rbs,'del')==0)
 	{
		$filename_detailed = $tmpdr.'result_detailed'.date("mdy-His");
		$f = fopen($filename_detailed,'w');	
		fputs($f, "Deletion Position \t\t\tOriginal Deletion In the Keio file\t\t \t\tSecond Deletion Found\t\t\t\t\t\t\t\r\n");
		fputs($f, "ECK\tLeft End \tRight End\tEG_id\tName\tLeft End \tRight End\tOverlap\t5' or 3' end\tEG_id\tName\tOrientation\tLeft End\tRight End\tLeft End Extension\tRight End Extension\r\n");
		foreach ($double_del as $value) {
			$string =  $value["eck"] ."\t" .$value["left_end"] ."\t" .$value["right_end"] ."\t" . $value["1_eg_id"] ."\t" . $value["1_name"] ."\t" . $value["1_left_end"] ."\t".$value["1_right_end"] ."\t" .$value["overlap"] ."\t".$value["end_type"] ."'\t" .$value["2_eg_id"]."\t" .$value["2_name"]."\t" .$value["2_ori"]."\t".$value["2_left_end"]."\t".$value["2_right_end"]."\t".$value["2_left_end_ext"]."\t".$value["2_right_end_ext"]."\r\n";
		
			fputs($f, $string);
		}
		fclose($f);
	
		$filename_simple = $tmpdr.'result_simple'.date("mdy-His");
		$f = fopen($filename_simple,'w');	
		fputs($f, "eck\tleft_end\tright_end\teg_id_1\teg_id_2\toverlap\t5' or 3' end\r\n");
		foreach ($double_del as $value) {
			$string = $value["eck"] ."\t" .$value["left_end"] ."\t" .$value["right_end"] ."\t" . $value["1_eg_id"] ."\t".$value["2_eg_id"] ."\t" .$value["overlap"]."\t" .$value["end_type"]."'\r\n";
		
			fputs($f, $string);
		}
		fclose($f);
	
		$filename_suspicion = $tmpdr.'deletion_suspicion'.date("mdy-His");
		$f = fopen($filename_suspicion,'w');	
		fputs($f, "eck\tname\tleft_end\tright_end\teg_id\tgene_left_end\tgene_right_end\tdistance\r\n");
		foreach ($wrong_del as $value) {
		
			fputs($f, $value);
		}
		fclose($f);



	$content .= "<hr><h2 align=\"center\">Experimental Results: ".(count($result)-count($tri_del)*2) ." Total Second Deletion, and ".count($tri_del)." Triple Deletion, ".sprintf("%6.4f",100*(count($result)-count($tri_del))/count($del_eck))."% </h2>";
	if(count($wrong_del)>0)
 	{
//	$content .= "<hr><table width='50%' align=center border='1'>";
	$content .= "<p align=\"center\"><em>Note: There are ".count($wrong_del)." records where the deletion positions do not happen inside the genes specified. <br />They are still included in the experiment. </em><A HREF='$filename_suspicion'>Download</a> These Suspicioned Records </p>";
 	}
 	if(count($tri_del)>0)
 	{
 		$content .= "<p align=\"center\"><em>Note: There are ".count($tri_del)." triple deletion records. </em>(";
 		foreach ($tri_del as $value) {
		$content .= $value." ";
		}
		$content .= "</p>";
 	}	

	$content .= "<p><b>Download Results: </b></p><p></font><A HREF='$filename_detailed'>Detailed Version</a></p><p>
	 <A HREF='$filename_simple'> Simple Version</a></p>";
//	$content .= "<table width='90%' align=center border='1'>";
//	$content .= "<tr><TD align=\"right\" colspan=\"16\" valign=\"bottom\"><b>Download Results: </b></font><A HREF='$filename_detailed'>Detailed Version</a> <A HREF='$filename_simple'> Simple Version</a></td></tr>";
//	
//	$content .= "<tr><th colspan=3 align=center>  Deletion Position </th><th colspan=4 align=center>Original Deletion In the Keio file</th><th colspan=9 align=center>Second Deletion Found</th></tr>";
////	
//	$content .= "<tr><th>ECK </th><th>Left End </th><th>Right End</th><th>EG_id</th><th>Name</th><th>Left End </th><th>Right End</th><th>Overlap</th><th>5' or 3' </th><th>EG_id</th><th>Name</th><th>Orientation</th><th>Left End </th><th>Right End</th><th>Left End Extension </th><th>Right End Extension</th></tr>";
//	
//
//	foreach ($result as $value) {
//		$content .= $value;
//	}
//	$content .= "</table>";
 }else{
 	
 	$filename_detailed = $tmpdr.'result_detailed'.date("mdy-His");
	$f = fopen($filename_detailed,'w');	
	fputs($f, "Deletion Position \t\t\tGene Position\t\t \t\tAdjacent Gene RBS Deletion Found\t\t\t\t\t\t\t\r\n");
	fputs($f, "ECK\tLeft End \tRight End\tEG_id\tName\tLeft End \tRight End\tDeletion_Length\t5' or 3' end\tEG_id\tName\tOrientation\tDeleted\tRetained\r\n");
	foreach ($double_del as $value) {
		
		$string =  $value["eck"] ."\t" .$value["left_end"] ."\t" .$value["right_end"] ."\t" . $value["1_eg_id"] ."\t" . $value["1_name"] ."\t" . $value["1_left_end"] ."\t".$value["1_right_end"] ."\t" .$value["overlap"] ."\t".$value["end_type"] ."'\t" .$value["2_eg_id"]."\t" .$value["2_name"]."\t" .$value["2_ori"]."\t".$value["string_1"]."\t".$value["string_2"]."\t"."\r\n";
		
		fputs($f, $string);
		
	}
	fclose($f);
	
	$filename_simple = $tmpdr.'result_simple'.date("mdy-His");
	$f = fopen($filename_simple,'w');	
	fputs($f, "eck\tleft_end\tright_end\teg_id_1\teg_id_2\toverlap\t5' or 3' end\r\n");
	foreach ($double_del as $value) {
		
		$string = $value["eck"] ."\t" .$value["left_end"] ."\t" .$value["right_end"] ."\t" . $value["1_eg_id"] ."\t".$value["2_eg_id"] ."\t" .$value["overlap"]."\t" .$value["end_type"]."'\r\n";
		
		fputs($f, $string);
		
	}
	fclose($f);
	
	$filename_suspicion = $tmpdr.'deletion_suspicion'.date("mdy-His");
	$f = fopen($filename_suspicion,'w');	
	fputs($f, "eck\tname\tleft_end\tright_end\teg_id\tgene_left_end\tgene_right_end\tdistance\r\n");
	foreach ($wrong_del as $value) {
		
		fputs($f, $value);
	}
	fclose($f);

	$content .= "<p><b>Download Results: </b></p><p></font><A HREF='$filename_detailed'>Detailed Version</a></p><p>
	 <A HREF='$filename_simple'> Simple Version</a></p>";
	
// 	$content .= "<table width='90%' align=center border='1'>";
//	$content .= "<tr><TD align=\"right\" colspan=\"14\" valign=\"bottom\"><b>Downlooad Results: </b></font><A HREF='$filename_detailed'>Detailed Version</a> <A HREF='$filename_simple'> Simple Version</a></td></tr>";
//	
//	$content .= "<tr><th colspan=3 align=center>  Deletion Position </th><th colspan=5 align=center>Original Deletion In the Keio file</th><th colspan=6 align=center>Second Deletion Found</th></tr>";
////	
//	$content .= "<tr><th>ECK </th><th>Left End </th><th>Right End</th><th>EG_id</th><th>Name</th><th>Left End </th><th>Right End</th><th>Overlap</th><th>5' or 3' </th><th>EG_id</th><th>Name</th><th>Orientation</th><th>Gap</th><th>Overlap </th></tr>";
//	
//
//	foreach ($result as $value) {		
//			$content .= $value;
//		
//	}
//	$content .= "</table>";
 }
 
	db_set_active("default");	
	return $content;
	
	
}


function read_keio_file($filename_tmp, $eg, $eck, &$del_eck, &$del_none, &$del_eck_wrong_end, &$del_eck_nonfound, &$del_eck_updated)
{

	$content = '';
	$fp_tmp = fopen($filename_tmp,'r');
	
	$nline = 0;
	$num_none = 0;
	$num_eck = 0;
	while($line = fgets($fp_tmp))
	{
		$nline = $nline+1;
		
		$rest = trim($line,"\r\n\t ");
		$array_line = explode("\t", $rest);
		if (!(count($array_line)==5 ))
		{
			$content .= "\n\n".$nline." has format error !\n\n";
		}
		$array_line[0] = trim($array_line[0],"\t ");
		$array_line[1] = trim($array_line[1],"\t ");
		$array_line[2] = trim($array_line[2],"\t ");
		$array_line[3] = trim($array_line[3],"\t ");
		$array_line[4] = trim($array_line[4],"\t ");
		
		
			$left_end = intval($array_line[3]);
		  	$right_end = intval($array_line[4]);
		  	$name = $array_line[1];
		  	
		if ($array_line[0]=='none')
		{	 	 	
		  	
		  	
		  	$del_none[$num_none]["left_end"] = $left_end;
		  	$del_none[$num_none]["right_end"] = $right_end;
		  	$del_none[$num_none]["eck"] = $array_line[0];
		  	$del_none[$num_none]["name"] = $array_line[1];
		  	$del_none[$num_none]["ori"] = $array_line[2];			  	

		  	
		  	$num_none = $num_none + 1;
			
		  }
		  else
		  {
		  	$del_eck[$num_eck]["left_end"] = $left_end;
		  	$del_eck[$num_eck]["right_end"] = $right_end;		  	
		  	
		  	$del_eck[$num_eck]["eck"] = $array_line[0];
		  	$del_eck[$num_eck]["name"] = $array_line[1];
		  	$del_eck[$num_eck]["ori"] = $array_line[2];		

		  	if(!(is_numeric($array_line[3]) && is_numeric($array_line[4])))
			{
//		  		echo $array_line[0] ."\t" .$array_line[1] ."\t" .$array_line[2] ."\t" .$array_line[3] ."\t" .$array_line[4] ."<br>";
		  		$del_eck_wrong_end[] = $line;
		  		continue;
			}	  	
		  	
		  	$key = array_search($del_eck[$num_eck]["eck"], $eck);
		  	if($key!==false)
		  	{
		  		$del_eck[$num_eck]["eg_id"] = $eg[$key];	
		  		$del_eck_found[] = $eg[$key]."\t".$line;
		  		
		  		$num_eck = $num_eck + 1;
		  	}
		  	else 
		  	{
				$query = db_select("t_gene","g");
				$query->addField("g","eg_id");
				$query->condition("g.name", $name);
				
				$query_sy = db_select("t_gene_synonym","gy");
				$query_sy->addField("gy","eg_id");
				$query_sy->condition("gy.name", $name);
				
				$query->union($query_sy);
				
				$row = $query->execute()->fetchAssoc();
				
				if($row)
				{
					$del_eck[$num_eck]["eg_id"] = $row['eg_id'];
					
					$query = db_select("t_gene","g");
					$query->addField("g","eck_id");
					$query->condition("g.eg_id", $row[eg_id]);
				
//					$query = "
//		  					select g.eck_id  FROM  t_gene g	where g.eg_id = '$row[eg_id]'
//		  					"	;
//
//					$rst_eck = mysql_query($query) or die("Query failed : " . mysql_error());
//					$row2 = mysql_fetch_array($rst_eck, MYSQL_ASSOC);
					
					$row2 = $query->execute()->fetchAssoc();
					$del_eck_updated[] = $row['eg_id']."\t".$row2['eck_id']."\t".$line;
					
//					mysql_free_result($rst_eck);					
					$num_eck = $num_eck + 1;
				}
				else 
				{
					$del_eck_nonfound[] = $line;
					
				}
//				mysql_free_result($rst_gene);
		  	}
		  	
		  	
		  	
		  }
		  
		 
	}
	
	fclose($fp_tmp);
	
	$filename_none = $tmpdr.'none_problem'.date("mdy-His");
	$fnone = fopen($filename_none,'w');	

	for($num_none=0; $num_none<count($del_none); $num_none++)
	{
		
				  
		  	$outstring = $del_none[$num_none]["eck"] ."\t" .$del_none[$num_none]["name"] ."\t" .$del_none[$num_none]["ori"] ."\t" .$del_none[$num_none]["left_end"] ."\t" .$del_none[$num_none]["right_end"] ."\r\n";
		  	fputs($fnone, $outstring);
		
	}
	fclose($fnone);
	
	$filename_eck = $tmpdr.'eck_found'.date("mdy-His");
	$f = fopen($filename_eck,'w');	
	foreach ($del_eck_found as $value) {
		fputs($f, $value);
	}
	fclose($f);
	
	
	$filename_updated = $tmpdr.'eck_updated'.date("mdy-His");
	$feck = fopen($filename_updated,'w');	
	foreach ($del_eck_updated as $value) {
		fputs($feck, $value);
	}
	fclose($feck);
	
	$filename_noise = $tmpdr.'eck_na_interval'.date("mdy-His");
	$f = fopen($filename_noise,'w');	
	foreach ($del_eck_wrong_end as $value) {
		fputs($f, $value);
	}
	fclose($f);
	
	$filename_nonfound = $tmpdr.'eck_nonfound'.date("mdy-His");
	$f = fopen($filename_nonfound,'w');	
	foreach ($del_eck_nonfound as $value) {
		fputs($f, $value);
	}
	fclose($f);
	

	
	$content.= '<table align="center" border="1">';
	$content.= '<th colspan="4">Keio Data File Summary (Note: Only records in set 1 and 2  are used for this experiment.)</th>';

	$n_total = count($del_eck_found)+count($del_eck_updated)+count($del_eck_nonfound)+count($del_eck_wrong_end)+count($del_none);

	$content.= "<tr><td><b> 1 </b></td><td> Records whose ECKs have been found in EcoGene</td>
					<td>".count($del_eck_found).
					"</td><td><A HREF='$filename_eck'>download</a></td></tr>";

	$content.= "<tr><td> <b> 2 </b> </td><td> Records whose ECKs can not be found in EcoGene, but names/synonyms have been found in EcoGene.
							We update the ECKs from orginal file (third column) with ECKs retrieved from EcoGene (second column) using these names/synonyms.</td>
							<td>".count($del_eck_updated)."</td>
							<td><A HREF='$filename_updated'>download</a></td></tr>";

	$content.= "<tr><td> <b> 3 </b> </td><td> Records that can been found in EcoGene using neither ECK or name/synonym. </td>
							<td>".count($del_eck_nonfound)."</td>
							<td><A HREF='$filename_nonfound'>download</a></td></tr>";

	$content.= "<tr><td> <b> 4 </b> </td><td> Noisy data (Either left_end or right_end is not available) </td>
	<td>".count($del_eck_wrong_end)."</td><td><A HREF='$filename_noise'>download</a></td></tr>";

	$content.= "<tr><td><b> 5 </b> </td><td> Records whose ECKs and names are none (The deletions are not inside any gene). </td>
				<td>".count($del_none)."</td>
				<td><A HREF='$filename_none'>download</a></td>";
	$content.= "<tr><td></td><td><b> Total: </b></td><td colspan='2'> $n_total</td></tr>";


	$content.= "</table>";
	
	return $content;
	
	
}	

function write_double(&$double_del,&$result,&$num_double,$del_eck,$gene,$gene_adj,$overlap,$left_right)
 {
 	global $del_rbs;
 	global $extension;
 	if((strcmp($left_right,'left')==0))
 	{
 		$end_type = ((strcmp($gene_adj['ori'],'Clockwise')==0)?'3':'5');
 	}else {
 		$end_type = ((strcmp($gene_adj['ori'],'Clockwise')==0)?'5':'3');
 	}
 	
 	
		$double_del[$num_double]["left_end"] = $del_eck["left_end"];
		$double_del[$num_double]["right_end"] = $del_eck["right_end"];
		$double_del[$num_double]["eck"] = $del_eck["eck"];
		$double_del[$num_double]["1_eg_id"] = $gene["eg_id"];
		$double_del[$num_double]["1_name"] = $gene["name"];
		$double_del[$num_double]["1_left_end"] = $gene["left_end"];
		$double_del[$num_double]["1_right_end"] = $gene["right_end"];
		
		$double_del[$num_double]["overlap"] = $overlap;
		$double_del[$num_double]["end_type"] = $end_type;
		$double_del[$num_double]["2_eg_id"] = $gene_adj["eg_id"];
		$double_del[$num_double]["2_name"] = $gene_adj["name"];
		$double_del[$num_double]["2_ori"] = $gene_adj["ori"];
	if(strcmp($del_rbs,'del')==0)	
	{
		
		
		
		
		$double_del[$num_double]["2_left_end"] = $gene_adj["left_end"];
		$double_del[$num_double]["2_right_end"] = $gene_adj["right_end"];
		$double_del[$num_double]["2_left_end_ext"] = $gene_adj["left_end_ext"];
		$double_del[$num_double]["2_right_end_ext"] = $gene_adj["right_end_ext"];
		
		$result[$num_double] = "<tr><td>".$del_eck["eck"]."</td><td>".$del_eck["left_end"] ."</td><td>" .$del_eck["right_end"] ."</td><td>" . $gene["eg_id"] ."</td><td>" . $gene["name"] ."</td><td>" . $gene["left_end"] ."</td><td>".$gene["right_end"] ."</td><td>" . $overlap ."</td><td>" .$double_del[$num_double]["end_type"]."</td><td>" .$gene_adj["eg_id"] ."</td><td>" . $gene_adj["name"] ."</td><td>".$gene_adj["ori"] ."</td><td>"  . $gene_adj["left_end"] ."</td><td>".$gene_adj["right_end"] ."</td><td>"  . $gene_adj["left_end_ext"] ."</td><td>".$gene_adj["right_end_ext"]."</td><tr>";
		
		$num_double++;		
	}elseif(strcmp($end_type,'5')==0)
	{
//		get_sequence($gene_adj["eg_id"],$extension,0,&$main_string,&$up_string, &$down_string);	
		$main_string='';
				$up_string='';
				$down_string='';
		get_sequence($gene_adj["eg_id"],$extension,0,$main_string,$up_string, $down_string);
		$string = $up_string.$main_string; 
		$string_1 = (($overlap>$extension)? substr($string,0,$extension): substr($string,0,$overlap));//overlap
		$string_2 = (($overlap>$extension)? "---": substr($string,$overlap,($extension - $overlap)));//gap
		$double_del[$num_double]["string_1"] = $string_1;
		$double_del[$num_double]["string_2"] = $string_2;
		
		$result[$num_double] = "<tr><td>".$del_eck["eck"]."</td><td>".$del_eck["left_end"] ."</td><td>" .$del_eck["right_end"] ."</td><td>" . $gene["eg_id"] ."</td><td>" . $gene["name"] ."</td><td>" . $gene["left_end"] ."</td><td>".$gene["right_end"] ."</td><td>" . $overlap ."</td><td>" .$double_del[$num_double]["end_type"]."</td><td>" .$gene_adj["eg_id"] ."</td><td>" . $gene_adj["name"]."</td><td>" . $gene_adj["ori"]."</td><td>" . $double_del[$num_double]["string_2"] ."</td><td>" . $double_del[$num_double]["string_1"]."</td><tr>";
		$num_double++;		
	}
		
		
 }
 
function keio_get_gene_info(&$gene, &$gene_eg, $extension)
{
	db_set_active("ecogene");
 	$query = db_query("
 		SELECT 
			g.name ,  g.eg_id, g.eck_id, ta.orientation, ta.left_end, ta.right_end  FROM t_address ta, t_gene g
		WHERE 
			g.address_id = ta.address_id and
			g.eg_id not in (select eg_id from t_gene_multi_address )
		order by ta.left_end
		");
	

 	$gene = array();
	$num_gene = 0;
	while ($row = $query->fetchAssoc())
	{
			$gene[$num_gene]["left_end"] = $row["left_end"];
		  	$gene[$num_gene]["right_end"] = $row["right_end"];
		  	$gene[$num_gene]["eg_id"] = $row["eg_id"];
		  	
		  	$gene[$num_gene]["name"] = $row["name"];
		  	$gene[$num_gene]["ori"] = $row["orientation"];
		  	
		  	$gene[$num_gene]["left_end"] = $gene[$num_gene]["left_end"];
		  	$gene[$num_gene]["right_end"] = $gene[$num_gene]["right_end"];
		  	
		  	if($gene[$num_gene]["ori"] == "Clockwise")
		  	{
		  		$gene[$num_gene]["left_end_ext"] = $gene[$num_gene]["left_end"] - $extension;
		  		$gene[$num_gene]["right_end_ext"] = $gene[$num_gene]["right_end"];
		  	}
		  	if($gene[$num_gene]["ori"] == "Counterclockwise")	
		  	{
		  		$gene[$num_gene]["right_end_ext"] = $gene[$num_gene]["right_end"] + $extension;
		  		$gene[$num_gene]["left_end_ext"] = $gene[$num_gene]["left_end"];
		  	}
		  		
			$gene_eg[$num_gene] = $row["eg_id"];
			$num_gene++;
	}
}
function keio_deletion($gene, $gene_eg, $del_eck, $overlap_thre, &$double_del, &$wrong_del, &$result, &$num_double, &$tri_del)
{
	global $GENOME_LENGTH;
	$num_gene = count($gene);
	$content = '';
	$double_del = array();
	$wrong_del = array();
	$tri_del = array();
	$result = array();
	$num_double = 0;
	

//	$i=4;
	for($i=0; $i<count($del_eck); $i++)
	{
		$eg_id = $del_eck[$i]["eg_id"];

		$key = array_search($eg_id, $gene_eg);

//		echo $key;
//		echo $gene_eg[$key];
//	print_r($gene[$key]);
//	print_r($del_eck[$i]);
		if ($key ===false) {
//			echo $i."\t".$del_eck[$i]["eg_id"] ."\t" .$del_eck[$i]["eck"] ."\t" .$del_eck[$i]["name"] ."\t" .$del_eck[$i]["ori"] ."\t" .$del_eck[$i]["left_end"] ."\t" .$del_eck[$i]["right_end"] ."<br>";
			continue;
		}
		if (!(($gene[$key]["left_end"]-0) < $del_eck[$i]["left_end"]) || !(($gene[$key]["right_end"]+0) > $del_eck[$i]["right_end"]))
		{
			if(!(($gene[$key]["left_end"]-0) < $del_eck[$i]["left_end"]) && !(($gene[$key]["right_end"]+0) > $del_eck[$i]["right_end"]))
				$distance = $gene[$key]["left_end"] - $del_eck[$i]["left_end"]-$gene[$key]["right_end"] + $del_eck[$i]["right_end"];
			elseif(!(($gene[$key]["left_end"]-0) < $del_eck[$i]["left_end"]))
				$distance = $gene[$key]["left_end"] - $del_eck[$i]["left_end"];
			else 
				$distance = -$gene[$key]["right_end"] + $del_eck[$i]["right_end"];
			
				$wrong_del[] = $del_eck[$i]["eck"] ."\t".$del_eck[$i]["name"] ."\t".$del_eck[$i]["left_end"]."\t".$del_eck[$i]["right_end"] ."\t" .$del_eck[$i]["eg_id"] ."\t" . $gene[$key]["left_end"]."\t".$gene[$key]["right_end"]."\t".$distance."\r\n";
//		continue;
		}
	
	
		if($key>0 && $key<$num_gene-1)
		{
			$double_flag = false;
			$tri_flag = false;
			if(($gene[$key-1]["right_end_ext"] >= $del_eck[$i]["left_end"]) )
			{
//			if(($gene[$key-1]["right_end_ext"] >= $del_eck[$i]["left_end"]) && !($gene[$key-1]["left_end_ext"] <= $del_eck[$i]["left_end"]))
//			echo $del_eck[$i]["left_end"];
				$overlap = $gene[$key-1]["right_end_ext"] - $del_eck[$i]["left_end"] + 1;
				$key_2 = $key-1;
				$double_flag = true;
			
				if($overlap>$overlap_thre)
				{
								
					write_double($double_del,$result,$num_double,$del_eck[$i],$gene[$key] ,$gene[$key_2],$overlap,'left');
				
				}
			}
		if(($gene[$key+1]["left_end_ext"] <= $del_eck[$i]["right_end"]) )
		{
//			if(($gene[$key+1]["left_end_ext"] <= $del_eck[$i]["right_end"]) && !($gene[$key+1]["right_end_ext"] >= $del_eck[$i]["right_end"]))
//			echo $del_eck[$i]["left_end"]." ";
			$overlap = $del_eck[$i]["right_end"] - $gene[$key+1]["left_end_ext"] + 1;
			$key_2 = $key+1;
			
			if( $overlap>$overlap_thre)
			{
				write_double($double_del,$result,$num_double,$del_eck[$i],$gene[$key] ,$gene[$key_2],$overlap,'right');
				
			}
			
			if($double_flag) $tri_flag = true; 
			
		}
	}
	if($key==0)
	{
		$double_flag = false;
		if(($gene[$num_gene-1]["right_end_ext"]>$GENOME_LENGTH) && (($gene[$num_gene-1]["right_end_ext"]%$GENOME_LENGTH) >= $del_eck[$i]["left_end"]))
		{
			$overlap = $gene[$num_gene-1]["right_end_ext"]%$GENOME_LENGTH - $del_eck[$i]["left_end"] + 1;
			$key_2 = $num_gene-1;
			$double_flag = true;
			
			if( $overlap>$overlap_thre)
			{
				write_double($double_del,$result,$num_double,$del_eck[$i],$gene[$key] ,$gene[$key_2],$overlap,'left');
				
			}
		}
		if(($gene[$key+1]["left_end_ext"] <= $del_eck[$i]["right_end"]))
		{
			$overlap = $del_eck[$i]["right_end"] - $gene[$key+1]["left_end_ext"] + 1;
			$key_2 = $key+1;
			
			if( $overlap>$overlap_thre)
			{
				write_double($double_del,$result,$num_double,$del_eck[$i],$gene[$key] ,$gene[$key_2],$overlap,'right');
				
			}
			if($double_flag) $tri_flag = true; 
			
		}
	}
	if($key==$num_gene-1)
	{
		$double_flag = false;
		if(($gene[$key-1]["right_end_ext"] >= $del_eck[$i]["left_end"]))
		{
			$overlap = $gene[$key-1]["right_end_ext"] - $del_eck[$i]["left_end"] + 1;
			$key_2 = $key-1;
			$double_flag = true;
			
			if($overlap>$overlap_thre)
			{
				write_double($double_del,$result,$num_double,$del_eck[$i],$gene[$key] ,$gene[$key_2],$overlap,'left');
				
			}
		}
		if(($gene[0]["left_end_ext"]<0) && ($gene[0]["left_end_ext"]%$GENOME_LENGTH <= $del_eck[$i]["right_end"]))
		{
			$overlap = $del_eck[$i]["right_end"] - $gene[0]["left_end_ext"]%$GENOME_LENGTH + 1;
			$key_2 = 0;
			
			if( $overlap>$overlap_thre)
			{
				write_double($double_del,$result,$num_double,$del_eck[$i],$gene[$key] ,$gene[$key_2],$overlap,'right');
				
			}
			if($double_flag) $tri_flag = true; 
			
		}
	}
	if ($tri_flag)
		$tri_del[]=$gene[$key]["eg_id"];
		
	
	
	}
}
