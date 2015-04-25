<?PHP


global $GENOME_LENGTH;
//$GENOME_LENGTH = 4639675;
$GENOME_LENGTH = 4641652;
function is_nested($curr_gene, $prev_gene, $next_gene)
{
	if(( $curr_gene['left_end']>=$prev_gene['left_end'] && $curr_gene['right_end']<=$prev_gene['right_end']) || ( $curr_gene['left_end']>=$next_gene['left_end'] && $curr_gene['right_end']<=$next_gene['right_end']))
	{
		$nested = true;
	}
	else
	{
		$nested = false;
	}
	return $nested;
}

function get_address_gene($eg_id, $unique_left_end,&$prev_gene,&$curr_gene,&$next_gene)
{
	include_once("dblink.php");

	$query = "
	SELECT g.name as name,ta.left_end,ta.right_end,ta.orientation, g.eg_id as eg_id, g.multi_location
			  FROM t_address ta  
			  LEFT OUTER JOIN t_gene g ON (ta.address_id = g.address_id  and g.multi_location <> 'splitgene'  and g.multi_location <> 'split_isgene')	 
			  where  (g.name is not null and g.name!='')
	union

	SELECT g.name as name,ta.left_end,ta.right_end,ta.orientation, ta.eg_id as eg_id, g.multi_location
			  FROM t_gene_split_address ta  
			  LEFT OUTER JOIN t_gene g ON ta.eg_id = g.eg_id 
			 WHERE  (g.multi_location = 'splitgene'  or g.multi_location = 'split_isgene' )
	order by left_end, right_end
	";
//	union
//
//	SELECT g.name as name,ta.left_end,ta.right_end,ta.orientation, g.eg_id as eg_id, g.multi_location 
//			  FROM t_gene_multi_address ta  
//			  LEFT OUTER JOIN t_gene g ON (ta.eg_id = g.eg_id )



	$rst_genomicAddress = mysql_query($query);
	$num_row = mysql_num_rows($rst_genomicAddress);
 	$n=0;
 	$prev_gene=array();
  	while ($row = mysql_fetch_array($rst_genomicAddress, MYSQL_ASSOC)) 
  	{
  		
  		$n = $n+1;
  		if($n==1){
  			$first_gene = $row;
  		}  		
  		if($row['eg_id']==$eg_id && $row['left_end']==$unique_left_end)
  		{
  			$curr_gene=$row;
  			break;  			
  		}
  		$prev_gene = $row;
  		
  	}
  	if($n<$num_row){
  		$next_gene = mysql_fetch_array($rst_genomicAddress, MYSQL_ASSOC);
  	}else{
  		$next_gene = $first_gene;
  	}
  	if($n==1){
  		
  		mysql_data_seek($rst_genomicAddress, $num_row-1);
  		$prev_gene = mysql_fetch_array($rst_genomicAddress, MYSQL_ASSOC);
  	}
  	mysql_free_result($rst_genomicAddress);	
  	
  	$curr_gene['whole_left_end'] = $curr_gene['left_end'];
    $curr_gene['whole_right_end'] = $curr_gene['right_end'];
  	
  	if($curr_gene['multi_location']=='splitgene' || $curr_gene['multi_location']=='split_isgene')
  	{
  		
  		$query = "select ta.* from t_gene g left join t_address ta on g.address_id=ta.address_id where g.eg_id='$eg_id'";
  		
  		$rst = mysql_query($query);
  		if($row = mysql_fetch_array($rst, MYSQL_ASSOC)){
  			$curr_gene['whole_left_end'] = $row['left_end'];
  			$curr_gene['whole_right_end'] = $row['right_end'];
  		}
  	}
}


function isPseudogene($eg_id)
{
	include_once("dblink.php");
	$link=dblink();
	mysql_select_db('ecogene') or die('Unable to select database');
	$query = "select * from t_pseudogene where eg_id = '$eg_id'";
	$rst = mysql_query($query) or die("$query ".mysql_error());
	
	if (mysql_num_rows($rst)==1) {
		return "PSEUDO";
	} else {
		return '-1';
	}
	
}
function geneSynonym($eg_id)
{
	include_once("dblink.php");
	$link=dblink();
	mysql_select_db('ecogene') or die('Unable to select database');
$query = 'SELECT '.

'gs.name '.	 	

'FROM '.

't_gene_synonym gs '.

'WHERE '.

"gs.eg_id = '$eg_id'";



$rst_geneSynonym = mysql_query($query) or die("Query failed : " . mysql_error());

$num_rows = mysql_num_rows($rst_geneSynonym);

$content = '';

for ($i = 0; $i < $num_rows; $i++ ) {

$syn_row = mysql_fetch_array($rst_geneSynonym, MYSQL_ASSOC);

//print($syn_row["name"]);
$content = $content.$syn_row["name"];

if ($i < $num_rows-1) { 
//	print ", "; 
	$content = $content.", ";
}			

}

if ($num_rows == 0) { 
//	print "None"; 
	$content ="None";
}
return $content;

}
function is_in_range_address($left_end, $right_end,&$igr)
{
	//echo $GENOME_LENGTH;
	$DBtable = 't_is_address';
	$Address = 't_address';
	global $GENOME_LENGTH;
	$num_gene=0;
	if($left_end<0)
	{	$query = 'SELECT '.	'ga.id, ga.name, ga.partial, ga.instance, ga.topic_id, address.* '.	'FROM '.	$DBtable.' ga ,'. $Address	. ' address '. 'WHERE '.
	" address.address_id=ga.address_id  and "."address.right_end > $GENOME_LENGTH + $left_end "
	." ORDER BY (address.right_end - address.left_end) ";


	$rst_geneInfo = mysql_query($query) or die($query."Query failed : <br>" . mysql_error());
	$tempnum = mysql_num_rows($rst_geneInfo);

	while ($row = mysql_fetch_array($rst_geneInfo, MYSQL_ASSOC))
	{
		$gene_left_end = $row['left_end'];
			$gene_right_end = $row['right_end'];
		if(($gene_right_end-$gene_left_end)<600)
		{			
			$igr[$num_gene]['id'] = $row['id'];
			$num_gene=$num_gene+1;
		}

	}
	}

	$query = 'SELECT '.	'ga.id, ga.name, ga.partial, ga.instance, ga.topic_id, address.* '.	' FROM '.	$DBtable.' ga ,'. $Address	. ' address '.
	'WHERE '.
	" address.address_id=ga.address_id  and ".
	"address.right_end > $left_end AND address.left_end < $right_end "
	." ORDER BY (address.right_end - address.left_end) ";

	$rst_geneInfo = mysql_query($query) or die($query."Query failed 595: <br>" . mysql_error());
	$tempnum = mysql_num_rows($rst_geneInfo);


	while ($row = mysql_fetch_array($rst_geneInfo, MYSQL_ASSOC))
	{
	$gene_left_end = $row['left_end'];
			$gene_right_end = $row['right_end'];

		if(($gene_right_end-$gene_left_end)<600)
		{
			$igr[$num_gene]['id'] = $row['id'];
			$num_gene=$num_gene+1;
		} 

	}

	if($right_end>$GENOME_LENGTH)
	{
		$query = 'SELECT '.	'ga.id, ga.name, ga.partial, ga.instance, ga.topic_id, address.*  '.	'FROM '.	$DBtable.' ga ,'. $Address	. ' address '.
		'WHERE '.
		" address.address_id=ga.address_id  and ".
		"address.left_end < $right_end-$GENOME_LENGTH  "
	." ORDER BY (address.right_end - address.left_end) ";

		$rst_geneInfo = mysql_query($query) or die($query."Query failed 634: <br>" . mysql_error());
		$tempnum = mysql_num_rows($rst_geneInfo);

		//echo $right_end;

		while ($row = mysql_fetch_array($rst_geneInfo, MYSQL_ASSOC))
		{
			$gene_left_end = $row['left_end'];
			$gene_right_end = $row['right_end'];
		if(($gene_right_end-$gene_left_end)<600)
		{
			$igr[$num_gene]['id'] = $row['id'];
			$num_gene=$num_gene+1;
		}
		
		}

	}
}
function tfbs_in_range_address($left_end, $right_end,&$tfbs)
{
	include_once("dblink.php");
	$link=dblink();
	mysql_select_db('ecogene') or die('Unable to select database');
	//	 echo $GENOME_LENGTH;
	$DBtable = 't_tfbs';
	$Address = 't_address';

	global $GENOME_LENGTH;
	$num_gene = 0;
	if($left_end<0)
	{	$query = "SELECT * FROM $DBtable  WHERE ".
	" right_end > $GENOME_LENGTH + $left_end ORDER BY left_end";

	$rst_geneInfo = mysql_query($query) or die($query."Query failed : <br>" . mysql_error());
	$tempnum = mysql_num_rows($rst_geneInfo);

	while ($row = mysql_fetch_array($rst_geneInfo, MYSQL_ASSOC))
	{


		$tfbs[$num_gene]['id_name'] = 'eg_id';
		$tfbs[$num_gene]['id'] = $row['EG_target'];
		$num_gene=$num_gene+1;
	}

	}

	$query = "SELECT * FROM $DBtable WHERE ".
	" right_end > $left_end AND left_end < $right_end ORDER BY left_end";

	$rst_geneInfo = mysql_query($query) or die($query."Query failed 595: <br>" . mysql_error());
	$tempnum = mysql_num_rows($rst_geneInfo);


	while ($row = mysql_fetch_array($rst_geneInfo, MYSQL_ASSOC))
	{
		$tfbs[$num_gene]['id_name'] = 'eg_id';
		$tfbs[$num_gene]['id'] = $row['EG_target'];
		$num_gene=$num_gene+1;
	}

	if($right_end>$GENOME_LENGTH)
	{
		$query = "SELECT * FROM $DBtable WHERE ".
		" left_end < $right_end-$GENOME_LENGTH  ORDER BY left_end";

		$rst_geneInfo = mysql_query($query) or die($query."Query failed 634: <br>" . mysql_error());
		$tempnum = mysql_num_rows($rst_geneInfo);

		//echo $right_end;

		while ($row = mysql_fetch_array($rst_geneInfo, MYSQL_ASSOC))
		{
			
			$tfbs[$num_gene]['id_name'] = 'eg_id';
			$tfbs[$num_gene]['id'] = $row['EG_target'];
			$num_gene=$num_gene+1;


		}

	}
}

function geneAlphabetical($name)
{
		
	include_once("dblink.php");
	$link=dblink();
	mysql_select_db('ecogene') or die('Unable to select database');
	$tmp_query = "select name from t_gene where name!='' order by name asc limit 0,1";
	$rst_tmp_query = mysql_query($tmp_query) or die("Query failed : " . mysql_error());

//  check if the gene is the last gene in alphabetal order. first gene in alphabetal order, it's left is the last gene alphabetal order

	$tmp_query_2 = "select name from t_gene where name!='' order by name desc limit 0,1";
	$rst_tmp_query_2 = mysql_query($tmp_query_2) or die("Query failed : " . mysql_error());


	if($name == mysql_result($rst_tmp_query, 0))//the first gene
	{
	
		$query = "(select g.eg_id from t_gene g where name!='' order by name desc limit 0,1) 
				UNION 
			  (SELECT g.eg_id FROM t_gene g WHERE g.name > '".$name."' ORDER BY g.name asc LIMIT 0,1)";
	}
	elseif ($name == mysql_result($rst_tmp_query_2, 0))// the last gene
	{
		$query = "(SELECT g.eg_id FROM t_gene g WHERE g.name < '".$name."' ORDER BY g.name desc LIMIT 0,1)
			UNION 
			(select g.eg_id from t_gene g where name!='' order by name asc limit 0,1)";
	}else
	{

	$query = "(SELECT "."g.eg_id "."FROM "."t_gene g "."WHERE "."g.name < '".$name."' "."ORDER BY "."g.name desc "."LIMIT "."0,1) "
		."UNION "."(SELECT "."g.eg_id "."FROM "."t_gene g "."WHERE "."g.name > '".$name."' "."ORDER BY "."g.name asc "."LIMIT "."0,1) ";

	}

	mysql_free_result($rst_tmp_query);
	mysql_free_result($rst_tmp_query_2);

	$rst_geneAlphabetical = mysql_query($query) or die("Query failed : " . mysql_error());
	$i=0;
	while($row = mysql_fetch_array($rst_geneAlphabetical, MYSQL_ASSOC))
	{
		$gal_row[$i] = $row['eg_id'];
		$i++;
	}
	return $gal_row;
}
function calculate_length( $eg_num, $prod_type ){
include_once("dblink.php");
	$link=dblink();
	mysql_select_db('ecogene') or die('Unable to select database');
	if($prod_type == "nt"){

		$query1="SELECT (ga.right_end - ga.left_end) + 1  length ".
				"FROM t_address ga, t_gene g  ".
				"WHERE g.eg_id = '$eg_num' ".
				"AND g.address_id = ga.address_id ";

		$resultSet= mysql_query($query1) or die("Query failed : " . mysql_error());

		$rowdata = mysql_fetch_array($resultSet, MYSQL_ASSOC);

		return $rowdata["length"];

	} elseif ($prod_type == "aa" ){

		$query1="SELECT char_length(sequence) length FROM t_product_protein WHERE eg_id = '$eg_num';";

		$resultSet= mysql_query($query1) or die("Query failed : " . mysql_error());

		$rowdata = mysql_fetch_array($resultSet, MYSQL_ASSOC);

		return $rowdata["length"];

	}

}

// Function to calculate Molecular Weight

function calculate_MW($eg_id,$type='aa')
{
	include_once("dblink.php");
	$link=dblink();
	mysql_select_db('ecogene') or die('Unable to select database');

// Molecular Weight DNA
//Molecular Weight = (An x 329.21) + (Un x 306.17) + (Cn x 305.18) + (Gn x345.21) + 159.0
// Molecular Weight protein
// Protein Mw is calculated by the addition of average isotopic masses of amino acids in the protein and the average isotopic mass of one water molecule.
/*
monoisotopic average 
Alanine (A) 71.03711 71.0788 
Arginine (R) 156.10111 156.1875 
Asparagine (N) 114.04293 114.1038 
Aspartic acid (D) 115.02694 115.0886 
Cysteine (C) 103.00919 103.1388 
Glutamic acid (E) 129.04259 129.1155 
Glutae (Q) 128.05858 128.1307 
Glycine (G) 57.02146 57.0519 
Histidine (H) 137.05891 137.1411 
Isoleucine (I) 113.08406 113.1594 
Leucine (L) 113.08406 113.1594 
Lysine (K) 128.09496 128.1741 
Methionine (M) 131.04049 131.1926 
Phenylalanine (F) 147.06841 147.1766 
Proline (P) 97.05276 97.1167 
Serine (S) 87.03203 87.0782 
Threonine (T) 101.04768 101.1051 
Tryptophan (W) 186.07931 186.2132 
Tyrosine (Y) 163.06333 163.1760 
Valine (V) 99.06841 99.1326 
*/
	$arr_weight_rna = array(
				"A" => "329.21", 
				"U" => "306.17",
				"C" => "305.18",
				"G" => "345.21",	
				"O"  => "159.0"			
				);
	$arr_weight = array(

				"A" => "71.0788", 
				"R" => "156.1875",
				"N" => "114.1038",
				"D" => "115.0886",
				"C" => "103.1388",
				"E" => "129.1155",
				"Q" => "128.1307",
				"G" => "57.0519",
				"H" => "137.1411",
				"I" => "113.1594",
				"L" => "113.1594",
				"K" => "128.1741",
				"M" => "131.1926",
				"F" => "147.1766",
				"P" => "97.1167",
				"S" => "87.0782",
				"T" => "101.1051",
				"W" => "186.2132",
				"Y" => "163.1760",
				"V" => "99.1326",
				"H2O" => "18.01524"
				);			
	if($type=='aa')
	{
		$query = 'SELECT '.		 ' sequence '.
		'FROM '.
		'	t_product_protein '.
		'WHERE '.
		"	eg_id = '$eg_id'";

		$rst_protSequence = mysql_query($query) or die("Query failed : " . $query.mysql_error());
		$row = mysql_fetch_array($rst_protSequence, MYSQL_ASSOC);
		$sequence = $row["sequence"];
		mysql_free_result($rst_protSequence);

		$protLen = strlen($sequence);
		$molecular_weight = $arr_weight["H2O"];

		for ($i=0; $i<$protLen;$i++)
		{
			$molecular_weight = $molecular_weight + $arr_weight[$sequence{$i}];
		}
	}
	
	if($type=='nt')
	{
		
		include_once("seq_format.php");
		$query = seq_format($eg_id);
//		print $query.'i here';
		$rst_rnaSequence = mysql_query($query) or die("Query failed : " . $query.mysql_error());
		
		$row = mysql_fetch_array($rst_rnaSequence, MYSQL_ASSOC);	
		$sequence = $row["sequence"];
		$sequence = str_replace("T", "U", $sequence);
		
		$rnaLen = strlen($sequence);
		$molecular_weight = $arr_weight_rna["O"];

		for ($i=0; $i<$rnaLen;$i++)
		{
			$molecular_weight = $molecular_weight + $arr_weight_rna[$sequence{$i}];
		}
		
		
	}

	return $molecular_weight;

}


function Translation_DNA_Protein($sequence)
{
	
	$Codon_Translation_Table = array(

				"GCT" => "A", 
				"GCC" => "A",
				"GCA" => "A",				
				"GCG" => "A",				
				"TGT" => "C",
				"TGC" => "C",				
				"GAT" => "D",
				"GAC" => "D",				
				"GAA" => "E",
				"GAG" => "E",				
				"TTT" => "F",
				"TTC" => "F",				
				"GGT" => "G",
				"GGC" => "G",
				"GGA" => "G",
				"GGG" => "G",								
				"CAT" => "H",
				"CAC" => "H",				
				"ATT" => "I",
				"ATC" => "I",
				"ATA" => "I",				
				"AAA" => "K",
				"AAG" => "K",				
				"TTG" => "L",
				"TTA" => "L",
				"CTT" => "L",
				"CTC" => "L",
				"CTA" => "L",
				"CTG" => "L",				
				"ATG" => "M",				
				"AAT" => "N",
				"AAC" => "N",				
				"CCT" => "P",
				"CCC" => "P",
				"CCA" => "P",
				"CCG" => "P",				
				"CAA" => "Q",
				"CAG" => "Q",				
				"CGT" => "R",
				"CGC" => "R",
				"CGA" => "R",
				"CGG" => "R",
				"AGA" => "R",
				"AGG" => "R",				
				"TCT" => "S",
				"TCC" => "S",
				"TCA" => "S",
				"TCG" => "S",
				"AGT" => "S",
				"AGC" => "S",				
				"ACT" => "T",
				"ACC" => "T",
				"ACA" => "T",
				"ACG" => "T",				
				"GTT" => "V",
				"GTC" => "V",
				"GTA" => "V",
				"GTG" => "V",				
				"TGG" => "W",				
				"NNN" => "X",				
				"TAT" => "Y",
				"TAC" => "Y"
				);
				
		$seqLen = strlen($sequence);
		$protein = "";

		for ($i=0; $i<$seqLen-2;$i+=3)
		{
			$cod = substr($sequence,$i,3);
			if ($cod[0]=='N' || $cod[1]=='N' || $cod[2]=='N')
			{
				$protein = $protein."X";
			}
			else {
				$protein = $protein.$Codon_Translation_Table[$cod];
			}
		}				
		
		return $protein;
}

function startdownload($filename)
{
	
 	$file_extension = strtolower(substr(strrchr($filename,"."),1));

    switch ($file_extension) {
    	case "pdf": $ctype="application/pdf"; break;
        case "exe": $ctype="application/octet-stream"; break;
        case "zip": $ctype="application/zip"; break;
        case "doc": $ctype="application/msword"; break;
        case "xls": $ctype="application/vnd.ms-excel"; break;
        case "ppt": $ctype="application/vnd.ms-powerpoint"; break;
        case "gif": $ctype="image/gif"; break;
        case "png": $ctype="image/png"; break;
        case "jpe": case "jpeg":
        case "jpg": $ctype="image/jpg"; break;
        default: $ctype="application/force-download";
     }

     header("Pragma: public");
     header("Expires: 0");
     header("Cache-Control: must-revalidate, post-check=0, pre-check=0");
     header("Cache-Control: private",false);
     header("Content-Type: $ctype");
     header("Content-Disposition: attachment; filename=\"".basename($filename)."\";");
     header("Content-Transfer-Encoding: binary");
     header("Content-Length: ".@filesize($filename));
     @readfile("$filename") or die("File not found.");
     
 }


 // re-format the dna sequenct according the join statement
function join_seq($seq,$join_pos,$orientation)
{
	$row_join = explode(";",$join_pos);

	if(count($row_join)>1)
	{
		

		$i=0;
		while ($i<floor(count($row_join)/2))
		{
			$join_array[$i]['left_end']  = $row_join[$i*2];
			$join_array[$i]['right_end'] = $row_join[$i*2+1];
			$i = $i+1;
		}
		$start_pos = $join_array[0]['left_end'];
		$join_num = $i;
		for ($i=0; $i<$join_num;$i++)
		{
			$join_array[$i]['left_end']	= $join_array[$i]['left_end'] - $start_pos;
			$join_array[$i]['right_end'] =$join_array[$i]['right_end'] - $start_pos;

		}
		if(strcasecmp($orientation, "Counterclockwise")==0)
		{
			$seq = strrev($seq);
		}
		$sequence_join = substr($seq,0,$join_array[0]['right_end'] - $join_array[0]['left_end']+1);
		$len = $join_array[0]['right_end'] - $join_array[0]['left_end']+1;
//		echo "<br> len:".$len/3;
		for ($i=1; $i<$join_num;$i++)
		{
			if($join_array[$i]['left_end'] > $join_array[$i-1]['right_end'])
			{
				$left = $join_array[$i]['left_end'];
				$len = $join_array[$i]['right_end']-$join_array[$i]['left_end']+1;
				$str_t = substr($seq,$left,$len);
				$sequence_join = $sequence_join.$str_t;
//				echo "<br> len:".$len/3;
			}
			else
			{
				$pad_num = $join_array[$i-1]['right_end'] - $join_array[$i]['left_end'] + 1;
				$sequence_join = $sequence_join.str_repeat("N",$pad_num);
				$left = $join_array[$i]['left_end']+$pad_num;
				$len = $join_array[$i]['right_end']-$join_array[$i]['left_end']+1-$pad_num;
				$str_t = substr($seq,$left,$len);
				$sequence_join = $sequence_join.$str_t;
//				echo "<br> len:".($len+$pad_num)/3;
														
			}
		}
		if(strcasecmp($orientation, "Counterclockwise")==0)
		{
			$sequence_join = strrev($sequence_join);
		}
	}
	return $sequence_join;
}

 function search_site($haystack, $needle,$is_symmetrical)
 	{
 		$i = 0;
 		$detected = array();
		$pattern = reg_pattern($needle);
		$numb = preg_match_all($pattern, $haystack,$out, PREG_OFFSET_CAPTURE);
		
       if($is_symmetrical==1)
       {
       	 for ($i=0; $i<$numb; $i++)
       	 {
//       	 	echo "\$out[$i][0][1]".$out[0][$i][0];
       	 	$detected[$i]['pos'] = $out[0][$i][1];
       	 	$detected[$i]['ori'] = '';
       	 	$detected[$i]['seq'] = substr($haystack,$detected[$i]['pos'],strlen($needle));
       	 }
       }
       
       if($is_symmetrical==0)
       {
       	for ($i=0; $i<$numb; $i++)
       	{
       		//       	 	echo "\$out[$i][0][1]".$out[0][$i][0];
       		$detected[$i]['pos'] = $out[0][$i][1];
       		$detected[$i]['ori'] = 'Clockwise';
       		$detected[$i]['seq'] = substr($haystack,$detected[$i]['pos'],strlen($needle));
       	}
//       	echo $pattern;
       	$pattern = reg_pattern($needle,1);
       	$numb = preg_match_all($pattern, $haystack,$out, PREG_OFFSET_CAPTURE);
       	$s = count($detected);
       	for ($i=0; $i<$numb; $i++)
       	{
//       		       	 	echo "\$out[0][$i][1]".$out[0][$i][0];
       		$detected[$i+$s]['pos'] = $out[0][$i][1];
       		$detected[$i+$s]['ori'] = 'Counterclockwise';
       		$detected[$i+$s]['seq'] = substr($haystack,$detected[$i+$s]['pos'],strlen($needle));
       	}
       }
//       echo "\$count  ".count($detected)."<br>";
       return $detected;
 	}
 	
 	function reg_pattern($pattern, $reversed=0)
 	{
 		if($reversed ==1)
		{
			$pattern = strrev($pattern);
			$pattern = str_replace("Z", "T",(str_replace("T", "A",(str_replace("A", "Z", $pattern))))); /* A<->T */
			$pattern = str_replace("Z", "C",(str_replace("C", "G",(str_replace("G", "Z", $pattern))))); /* C<->G */
			
			$pattern = str_replace("Z", "V",(str_replace("V", "B",(str_replace("B", "Z", $pattern))))); /* B<->V */
			$pattern = str_replace("Z", "H",(str_replace("H", "D",(str_replace("D", "Z", $pattern))))); /* D<->H */
			
			$pattern = str_replace("Z", "M",(str_replace("M", "K",(str_replace("K", "Z", $pattern))))); /* K<->M */
			$pattern = str_replace("Z", "R",(str_replace("R", "Y",(str_replace("Y", "Z", $pattern))))); /* Y<->R */	
		
		
		}
		
		
 		$search = array("B", "D", "H", "K", "M", "N" ,"R", "S", "V", "W", "X", "Y");
		$replace   = array("[CGT]", "[AGT]", "[ACT]", "[GT]", "[AC]", "[ACGT]" ,"[AG]", "[CG]", "[ACG]", "[AT]", "[ACGT]", "[CT]");
		
		
		
		

		return "|".str_replace($search , $replace ,$pattern)."|U";
		
	/* complimental code */	
		/* B<->V */
		/* D<->H */
		/* K<->M */
		/* Y<->R */
		/* C<->G */
		/* A<->T */
		
 	/* ambiguity code */

		/* B = C, G or T */

		/* D = A, G or T */

	 	/* H = A, C or T */

	 	/* K = G or T */

	 	/* M = A or C */

	 	/* N = A, C, G or T ------no change*/

	 	/* R = A or G (purines) */

	 	/* S = C or G ------no change**/

	 	/* V = A, C or G */

	 	/* W = A or T ------no change**/

	 	/* X = A, C, G or T */

	 	/* Y = C or T (pyrimidines) ------no change**/
 	}
 	
function sort_array($my_arr,$sortby)
{
	
		$i=0;
		$j=0;
		$n = 0;
		$e = count($my_arr)-1;
		$b = 0;
		if($sortby=='fold_value' || $sortby=='num_gene')
		{
		for($i = $e; $i > $b; $i = $k)
		{
			$k = $b;
			for($j = $b; $j < $i; $j++)
			{
				$n++;
				if(abs($my_arr[$j][$sortby]) < abs($my_arr[$j + 1][$sortby]))
				{
					$k = $j;
					$temp = $my_arr[$j];
					$my_arr[$j] = $my_arr[$j+1];
					$my_arr[$j+1] = $temp;
					$changed = true;

					$n++;
				}
			}
		}
		}
		else
		{
		for($i = $e; $i > $b; $i = $k)
		{
			$k = $b;
			for($j = $b; $j < $i; $j++)
			{
				$n++;
				if($my_arr[$j][$sortby] > $my_arr[$j + 1][$sortby])
				{
					$k = $j;
					$temp = $my_arr[$j];
					$my_arr[$j] = $my_arr[$j+1];
					$my_arr[$j+1] = $temp;
					$changed = true;

					$n++;
				}
			}
		}
		}
		return $my_arr;
	
}
function get_sequence($eg_id,$us,$ds,&$main_string, &$up_string, &$down_string)
{
	include_once("seq_format.php");
	
	include_once("dblink.php");
	$link=dblink();
	mysql_select_db('ecogene') or die('Unable to select database');
	
if ($us == "")
{

	$us = 0;

}

if($ds == "")

{

	$ds = 0;

}

//get the position of the sequence

$query = "
		SELECT 
			`address`.`left_end`, `address`.`right_end`, `address`.`orientation`
		FROM 
			t_address address , 			
			t_gene	  g
		Where
			`address`.`address_id` = `g`.`address_id` 
			and
			`g`.`eg_id` = '$eg_id'
		";

$rst_dnaPosition = mysql_query($query) or die("Query failed : " .$query. mysql_error());
$row = mysql_fetch_array($rst_dnaPosition, MYSQL_ASSOC);

$left_pos  = $row["left_end"];
$right_pos = $row["right_end"];

mysql_free_result($rst_dnaPosition);


//get the position of the up and down streams in the genome sequence
//according the up and down stream length $us and $ds and orientations
//need to go circle if necessary (gennome is a circle)
global $GENOME_LENGTH;
$bs = 0;
$es = $GENOME_LENGTH;

if ( $row["orientation"] == "Clockwise")

{

	if ( $row["left_end"] - $us < 0 )

	{

		$es = $row["left_end"] - $us + $GENOME_LENGTH;

		$us = $row["left_end"] -1;

	}

	if ( $row["right_end"] + $ds > $GENOME_LENGTH )

	{

		$bs = $row["right_end"] + $ds - $GENOME_LENGTH;

		$ds = $GENOME_LENGTH - $row["right_end"];

	}

}



if ( $row["orientation"] == "Counterclockwise")

{


	if ( $row["right_end"] + $us > $GENOME_LENGTH )

	{

		$bs = $row["right_end"] + $us - $GENOME_LENGTH;

		$us = $GENOME_LENGTH - $row["right_end"];

	}

	if ( $row["left_end"] - $ds < 0 )

	{

		$es = $row["left_end"] - $ds + $GENOME_LENGTH;

		$ds = $row["left_end"] - 1;

	}

}

//need to go circle if necessary (gennome is a circle)
//get the beginning of the sequence to get the piece that go beyong
//the genome length
//Counterclockwise gene need to do reverse compliment

$bstring = "";

if ( 0 != $bs )

{

	$query = "SELECT ".

	"	CASE '" .$row["orientation"]. "'".

		 "		WHEN 'Clockwise' THEN substring( s.sequence, 1, ".$bs." ) ".

		 "		WHEN 'Counterclockwise' THEN ".

		 "			replace(replace(replace(replace(replace(replace(reverse(substring( s.sequence, 1, ".$bs." )),'A','Z'),'T','A'),'Z','T'),'G','Z'),'C','G'),'Z','C') ".

		 "	END as sequence ".


	" FROM ".

	"	t_sequence s ";


	$rst_dnaBs = mysql_query($query) or die("Query failed : " . mysql_error());

	$row1 = mysql_fetch_array($rst_dnaBs, MYSQL_ASSOC);

	$bstring = $row1['sequence'];
	
	mysql_free_result($rst_dnaBs);
}


//need to go circle if necessary (gennome is a circle)
//get the end of the sequence to get the piece that go beyong
//the start of the genome
//Counterclockwise gene need to do reverse compliment

$estring = "";

if ( $GENOME_LENGTH != $es )   // $GENOME_LENGTH is the current length of the e coli genome
{

	$query = "SELECT ".
"	CASE " ."'".$row["orientation"]."'". 

		 "		WHEN 'Clockwise' THEN substring( s.sequence, ".$es." ) ".

		 "		WHEN 'Counterclockwise' THEN ".

		 "			replace(replace(replace(replace(replace(replace(reverse(substring( s.sequence, ".$es." )),'A','Z'),'T','A'),'Z','T'),'G','Z'),'C','G'),'Z','C') ".

		 "	END as sequence ".


	" FROM ".

	"	t_sequence s ";



	$rst_dnaBs = mysql_query($query) or die("Query failed : " . mysql_error());

	$row1 = mysql_fetch_array($rst_dnaBs, MYSQL_ASSOC);

	$estring = $row1['sequence'];
	
	
	mysql_free_result($rst_dnaBs);
}


	$query = seq_format($eg_id, $us, $ds);
	
	$query=db_query($query);
	$row = $query->fetchAssoc();
//	$row = db_fetch_array($query);
	$sequence = $row["sequence"];



// This portion of the code generates the extra upstream sequences and wraps them

if( "Clockwise" == $row["orientation"] )

{

	if( $estring != "" )

	{
		$up_string = $up_string.$estring;
	}

}

else if( "Counterclockwise" == $row["orientation"] )

{
	
	if( $bstring != "" )

	{
		
		$up_string = $up_string.$bstring;
	}

}

$up_string = $up_string.substr($sequence,0,$us);

// This portion of the code generates the main stream e.g. gene's dna sequence
// If has join statement,
// neet to re-format the dna sequenct according the join statement

$join_pos = $join_text;
if($join_pos=="")
{
	$main_string = $main_string.substr($sequence,$us,strlen($sequence)-$ds-$us);
	
}
else{// function	re-format the dna sequenct according the join statement

	$sequence_temp = substr($sequence,$us,strlen($sequence)-$ds-$us);
	
	$sequence_join = join_seq($sequence_temp,$join_pos,$row["orientation"]);
	
	$main_string = $main_string.$sequence_join;

}



// This portion of the code generates the extra downstream sequences and wraps them


$down_string = $down_string.substr($sequence,strlen($sequence)-$ds, $ds);

if( "Clockwise" == $row["orientation"] )
{	
	if( $bstring != "" )

	{
		$down_string = $down_string.$bstring;
	}

}

else if( "Counterclockwise" == $row["orientation"] )

{
	if( $estring != "" )

	{
		$down_string = $down_string.$estring;
	}

}

}
function ecogene_gene_get_join($eg_id,&$join_title,&$join_text, &$html_text,$name)
{
	$join_title = '';
	$join_text = '';
	$html_text='';
	
	

	    
	$query = db_query("select * from t_gene_split_address where eg_id=:eg_id order by left_end",array(':eg_id'=>$eg_id));
	$row_join = $query->fetchAssoc();		
	
	if($row_join)
	{
		$orientation = $row_join['orientation']	;
		

		if(strcasecmp($orientation, "Counterclockwise")==0)
		{
			$join_title = $join_title."complement(join(";
		}
		else {
			$join_title = $join_title."join(";
		}
		$join_title = $join_title. $row_join['left_end']."..".$row_join['right_end'];
		$join_text = $row_join['left_end'].";".$row_join['right_end'];

		while ($row_join = $query->fetchAssoc())
		{
			$join_title = $join_title. ",".$row_join['left_end']."..".$row_join['right_end'];
			$join_text = $join_text.";".$row_join['left_end'].";".$row_join['right_end'];
		}

		if(strcasecmp($orientation, "Counterclockwise")==0)
		{
			$join_title = $join_title."))";
		}
		else {
			$join_title = $join_title.")";
		}	

		$query = db_query("select g.name, g.multi_location, mutation_type from t_gene g left join 
		 				t_mutation m on ( (g.eg_id =m.eg_id ) 
		 				and (m.mutation_type='insertion' or m.mutation_type='frameshift'))
		 				where g.eg_id=:eg_id",array(':eg_id'=>$eg_id));
		$row = $query->fetchAssoc();
		if($row){
			$html_text = "<p>The following join statement uses GenBank syntax to describe how the";
		
//			if($row['multi_location']=='splitgene' || $row['multi_location']=='split_isgene'|| $row['multi_location']=='frmpseudogene')
//			{
				if ($row['mutation_type'] =='insertion' ) {
					$html_text .= " reconstructed IS-interrupted ";
				} elseif ($row['mutation_type'] =='frameshift') {
					$html_text .= " reconstructed frameshifted ";
				} else {
					$html_text .= " reconstructed ";
				}
				$html_text .= $name." pseudogene ";
//			}
//			else //if($row['multi_location']=='frmexception')
//			{
//				$html_text .= $name." reconstructed frameshifted ";
//			}
			
			
			$html_text .= "protein sequence was translated from the DNA: </p>"; 
		}
		$html_text .= '<p>'.$join_title.'</p>';

	}
}
?>