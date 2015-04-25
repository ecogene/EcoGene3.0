<?PHP
include_once ("sites/all/include/ecoFunction.php");
define ("SEQ_LINE", 75);
global $GENOME_LENGTH;
global $tmpdr;
global $GENOME_LENGTH_20;
global $GENOME_LENGTH_10;
$GENOME_LENGTH_20=4639675;
$GENOME_LENGTH_10=4639221;
$tmpdr = 'temp/';
set_time_limit(1000);

function download_dbtable_head($fp, $request) {
 
	$options = array();
	$head_arr = array();
  	foreach (db_table_fields_options() as $ind=>$status) {

    	$head_st[$ind] = isset($request[$status['id']])?  1:0;
    	if($head_st[$ind]==1)
    	{
    		$head_arr[$status['weight']] = $status['head'];
    	}
    	
  	}
  	ksort($head_arr); 
  	foreach ($head_arr as $value) {
  		fputs($fp,$value."\t");
  	}
	fputs($fp,"\r\n"); 
	
	return $head_st;
}
function download_dbtable_body($fp,$head_st,$request) {
	
	global $GENOME_LENGTH;
	db_set_active("ecogene");
	
	if($request["database_name"]){
		$databse_name = $request["database_name"];
	}else{
		$databse_name = "ecogene";
	}

	$egidA = array();
	$searchegs = "";
	if ($request['egids'] != "") {
		$egidA = explode(',',$request['egids']);
	}
	foreach($egidA as $value) {
		$searchegs = $searchegs."'".$value."',";
	}
	//remove "," of last item
	$searchegs = strrev(substr(strrev($searchegs),1));	
	
		
	$pseudo_gene = db_query("SELECT eg_id from t_pseudogene")
					->fetchCol();	
	

	$query_str = "select t_gene.*, t_address.left_end,t_address.right_end, t_address.orientation, 
		 	t_product_protein.name as protname, 
			t_product_protein.function as function 
		  from t_gene LEFT JOIN  t_product_protein ON t_gene.eg_id = t_product_protein.eg_id, t_address
		  where t_gene.address_id = t_address.address_id ";

	

	if ( !empty($searchegs )) {

		$query_str .= " and t_gene.eg_id in (".$searchegs.")";

	}
	if (isset($request['is_pseudo'])) {
		$query_str .= " and t_gene.eg_id not in (select distinct eg_id from t_pseudogene)";
	}
	if (($request['gene_type']=='protein')) {
		$query_str .= " and t_gene.type = 'aa'";

	}
	if (($request['gene_type']=='rna')) {
		$query_str .= " and t_gene.type = 'nt'";

	}
	
	$i=0;
	$query = db_query($query_str);
//	$row_gene = db_fetch_array($query);
	while ($row_gene = $query->fetchAssoc()) 
 	{
 		$i = $i+1;

		if($head_st[0]){
 			fputs($fp,rtrim($row_gene["eg_id"],"\t")); 	
 			fputs($fp,"\t");
		}
 		if($head_st[1]){
			if(empty($row_gene['eck_id']))
			{$row_gene['eck_id'] = "Null";}

			fputs($fp,rtrim($row_gene['eck_id'],"\t"));
			fputs($fp,"\t");

		}
		
		if($head_st[2]){
			if(in_array($row_gene["eg_id"], $pseudo_gene))
			{
				fputs($fp,rtrim($row_gene['name'],"\t")."'");
			}
			else
			{
				fputs($fp,rtrim($row_gene['name'],"\t"));
			}
			fputs($fp,"\t");

		}
 		if($head_st[3]){	
 			
				$g_sy = geneSynonym($row_gene['eg_id']);
				
				fputs($fp,$g_sy);
				fputs($fp,"\t");
			
		}
 		if($head_st[4]){
			fputs($fp,rtrim($row_gene['type'],"\r\n\t")); 	
 			fputs($fp,"\t");

		}
 	 	if($head_st[5]){
			fputs($fp,rtrim($row_gene['length'],"\r\n\t"));
			fputs($fp,"\t");

 		}
		if($head_st[6]){
			if($row_gene['orientation']=='Clockwise')
 			{
 				fputs($fp,"Clockwise");
 				$orientation = "Clockwise";
 			}
 			if($row_gene['orientation']=='Counterclockwise')
 			{
 				fputs($fp,"Counterclockwise");
 				$orientation = "Counterclockwise";
 			}
	
 			fputs($fp,"\t");

		}
 		if($head_st[7]){
 			$left_end = $row_gene['left_end'];
 			fputs($fp,$left_end); 	
 			fputs($fp,"\t");

 		}
 		if($head_st[8]){
 			$right_end = $row_gene['right_end'];
 			fputs($fp,$right_end);
 			fputs($fp,"\t");
 	
 		}
 	
 		if($head_st[9]){
 			$centisome = $row_gene['left_end'] / $GENOME_LENGTH * 100;
 			$centisome = sprintf("%.2f",$centisome);
 			fputs($fp,$centisome);
 			fputs($fp,"\t");

 		}
 		if($head_st[10]){
 			$eg_id = $row_gene['eg_id'];
 			$MW="Null";	
 			if( $row_gene["type"] == "aa" ) 
			{
				$MW = sprintf("%.2f",calculate_MW($eg_id)); 
			}
			if( $row_gene["type"] == "nt" ) 
			{
				$MW = sprintf("%.2f",calculate_MW($eg_id,'nt')); 
			}
	  	
	
			fputs($fp,$MW);
    		fputs($fp,"\t");

 		}
 		
 		if($head_st[11]){
 			if(empty($row_gene['protname']))
 			{$row_gene['protname'] = "Null";} 	

 			fputs($fp,trim(str_replace("\r"," ",str_replace("\n"," ",$row_gene['protname'])),"\r\n\t"));
 			fputs($fp,"\t");
 	 	}
 		if($head_st[21]){
 			if(empty($row_gene['function']))
 			{$row_gene['function'] = "Null";} 	

 			fputs($fp,trim(str_replace("\r"," ",str_replace("\n"," ",ucfirst($row_gene['function']))),"\r\n\t"));
 			fputs($fp,"\t");
 	 	}

 	 	if($head_st[12]){
 			if(empty($row_gene['description']))
 			{$row_gene['description'] = "Null";} 	

 			fputs($fp,trim(str_replace("\r"," ",str_replace("\n"," ",$row_gene['description'])),"\r\n\t"));
 			fputs($fp,"\t");
 	 	}
 		if($head_st[13]){
 			if(empty($row_gene['comments']))
 				{$row_gene['comments'] = "Null";}  	

			fputs($fp,trim(str_replace("\r"," ",str_replace("\n"," ",$row_gene['comments'])),"\r\n\t"));
    		fputs($fp,"\t");
 		}
 	
 	
 		
		$acc_ids = array('sp'=>'',
						 'wis'=>'',
						'cg'=>'', 
						'gi'=>'',
						'jw'=>'',
						'asap'=>'',
						'eb'=>'',
								);
		$query_acc = db_query("SELECT * FROM 
							t_biodatabase_link 
							WHERE eg_id = :eg_id AND ( biodatabase_id = 1 or biodatabase_id = 3 or biodatabase_id = 2 or
													   biodatabase_id = 4 or biodatabase_id = 10 or biodatabase_id = 13 or 
													   biodatabase_id = 14 )
							order by accession_id desc ",
							array(':eg_id'=>$row_gene["eg_id"]));
		
		while ($row_acc = $query_acc->fetchAssoc()) 
 		{
 			
 			if($row_acc['biodatabase_id']==1){
 				$acc_ids['sp'] = $row_acc['accession_id'];
 			}
 			elseif($row_acc['biodatabase_id']==3){
 				// 	Only show the valid b number for regular gene which is largest b number in database. 
			//  But all b numbers for isgene should be showing. 
 				if(strcasecmp($row_gene['multi_location'],'isgene')==0)
				{
					$acc_ids['wis'] .= strlen($acc_ids['wis'])>0 ? '; '.$row_acc['accession_id'] : $row_acc['accession_id'];
				}else 
				{					
 					$acc_ids['wis']=$row_acc['accession_id'];
 				}
 			}
 			elseif($row_acc['biodatabase_id']==2){
 				$acc_ids['cg'] = $row_acc['accession_id'];
 			}
 			elseif($row_acc['biodatabase_id']==4){
 				$acc_ids['gi'] .= strlen($acc_ids['cg'])>0 ? '; '.$row_acc['accession_id'] : $row_acc['accession_id'];
 			}
 			elseif($row_acc['biodatabase_id']==10){
 				$acc_ids['jw'] .= strlen($acc_ids['jw'])>0 ? '; '.$row_acc['accession_id'] : $row_acc['accession_id'];
 			}
 			elseif($row_acc['biodatabase_id']==13){
 				$acc_ids['asap'] .= strlen($acc_ids['asap'])>0 ? '; '.$row_acc['accession_id'] : $row_acc['accession_id'];
 			}
 			elseif($row_acc['biodatabase_id']==14){
 				$acc_ids['eb'] .= strlen($acc_ids['eb'])>0 ? '; '.$row_acc['accession_id'] : $row_acc['accession_id'];
 			}
 		}		
 		if($head_st[14]){
				$g_id = (strlen($acc_ids['gi'])>0)? rtrim($acc_ids['gi']): 'Null';			
				fputs($fp,$g_id);
				fputs($fp,"\t");
		}
					
		if($head_st[15]){
				$g_id = (strlen($acc_ids['sp'])>0)? rtrim($acc_ids['sp']): 'Null';			
				fputs($fp,$g_id);
				fputs($fp,"\t");
		}
 		if($head_st[16]){
				$g_id = (strlen($acc_ids['wis'])>0)? rtrim($acc_ids['wis']): 'Null';			
				fputs($fp,$g_id);
				fputs($fp,"\t");
		}
 		if($head_st[17]){
				$g_id = (strlen($acc_ids['asap'])>0)? rtrim($acc_ids['asap']): 'Null';			
				fputs($fp,$g_id);
				fputs($fp,"\t");
		}
 		if($head_st[18]){
				$g_id = (strlen($acc_ids['jw'])>0)? rtrim($acc_ids['jw']): 'Null';			
				fputs($fp,$g_id);
				fputs($fp,"\t");
		}
 		if($head_st[19]){
				$g_id = (strlen($acc_ids['cg'])>0)? rtrim($acc_ids['cg']): 'Null';			
				fputs($fp,$g_id);
				fputs($fp,"\t");
		} 		
 		
 		if($head_st[20]){
				$g_id = (strlen($acc_ids['eb'])>0)? rtrim($acc_ids['eb']): 'Null';			
				fputs($fp,$g_id);
				fputs($fp,"\t");
		}
		
 
 		fseek($fp, -1, SEEK_CUR); 
		fputs($fp,"\r\n");
 	}
	
	
	
	db_set_active("default");
return ;
}
	
function get_download_sequence_head($genome,$from,$to,$reversed,$fp)
{
	$length_subseq = $to-$from+1;
	if($genome=="ECOLI_3")
	{		
		if($reversed==1){

			fputs($fp, ">E. coli K-12 MG1655 U00096.3 (".$to." to ".$from." = ".$length_subseq." bp)\r\n");

		}

		else{

			fputs($fp, ">E. coli K-12 MG1655 U00096.3 (".$from." to ".$to." = ".$length_subseq." bp)\r\n");

		}
	}elseif($genome=="ECOLI_2")
	{		
		if($reversed==1){

			fputs($fp, ">E. coli K-12 MG1655 U00096.2 (".$to." to ".$from." = ".$length_subseq." bp)\r\n");

		}

		else{

			fputs($fp, ">E. coli K-12 MG1655 U00096.2 (".$from." to ".$to." = ".$length_subseq." bp)\r\n");

		}
	}elseif ($genome=="ECOLI_1")
	{
		if($reversed==1){

			fputs($fp, ">E. coli K-12 MG1655 U00096.1 (".$to." to ".$from." = ".$length_subseq." bp)\r\n");

		}

		else{

			fputs($fp, ">E. coli K-12 MG1655 U00096.1 (".$from." to ".$to." = ".$length_subseq." bp)\r\n");

		}
	}
}
function get_download_sequence_body($genome,$from,$to,$reversed,$fp)
{
	
	$length_subseq = $to-$from+1;
	db_set_active("ecogene");

	$args = array(':from'=>$from,':length_subseq'=>$length_subseq, 'genome'=>$genome);
	$query = db_query("SELECT substring( sequence, :from, :length_subseq ) as sequence FROM t_genome  WHERE org_code=:genome ", $args);
	$row = $query->fetchAssoc();
	
	$sequence = $row["sequence"];

	if($reversed==1){

		$sequence = strrev($sequence);

		$sequence = str_replace("Z", "T",(str_replace("T", "A",(str_replace("A", "Z", $sequence))))); /* A<->T */

		$sequence = str_replace("Z", "C",(str_replace("C", "G",(str_replace("G", "Z", $sequence))))); /* C<->G */

		$sequence = str_replace("U", "A", $sequence); /* U<->A */

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

		$sequence = str_replace("Z", "V",(str_replace("V", "B",(str_replace("B", "Z", $sequence))))); /* B<->V */

		$sequence = str_replace("Z", "H",(str_replace("H", "D",(str_replace("D", "Z", $sequence))))); /* D<->H */

		$sequence = str_replace("Z", "M",(str_replace("M", "K",(str_replace("K", "Z", $sequence))))); /* K<->M */

		$sequence = str_replace("Z", "R",(str_replace("R", "Y",(str_replace("Y", "Z", $sequence))))); /* Y<->R */

		

	}
	for ($i=0;$i<strlen($sequence);$i = $i+SEQ_LINE) {	     	

		   fputs($fp, substr($sequence,$i,SEQ_LINE));

		   fputs($fp, "\r\n");		   	

	}
	db_set_active("default");
}
function download_seqlib_return() {
	global $GENOME_LENGTH;
	global $tmpdr;
	
	$request = $_REQUEST;
	
	$lib = isset($request['lib'])? $request['lib']: '';
	
	if ($lib == 'SEQ') {
		$tmpfname = $tmpdr."EcoSEQ".date("mdy-His").".txt"; 
			$catRadioA = $request['catRadioA'];
			$catRadioB = $request['catRadioB'];
			$ds = $request['ds'];
			$us = $request['us'];
	}
	/*if ($lib == 'RNA') {*/
	elseif ($lib == 'RNA') {
		$tmpfname = $tmpdr."EcoRNA".date("mdy-His").".txt"; 
	} else if ($lib == 'DNA') {
		$tmpfname  = $tmpdr."EcoDNA".date("mdy-His").".txt"; 
	} else {
		$tmpfname = $tmpdr."EcoProt".date("mdy-His").".txt"; 
	}
	
	$fp = fopen($tmpfname, "w");
	if(!$fp)	return;
	
	$egidA = array();
	$searchegs = "";

	if ($request['egids'] !== "") {
		$egidA = explode(',',$_POST['egids']);
	}
	foreach($egidA as $value) {
		$searchegs = $searchegs."'".$value."',";
	}
	//remove "," of last item
	$searchegs = strrev(substr(strrev($searchegs),1));
	
	db_set_active("ecogene");

	$query_str = "SELECT g.eg_id,g.name,g.description, ta.left_end,ta.right_end,ta.orientation,tpp.sequence 
		  				FROM t_address ta 
		  					JOIN t_gene g ON (ta.address_id = g.address_id) 
		  					left JOIN t_product_protein tpp ON (g.eg_id = tpp.eg_id)";


	if ($searchegs !== "") {
		$query_str .= " where g.eg_id in (".$searchegs.")";
	}


 	$query = db_query($query_str);
	
	while ($row = $query->fetchAssoc()) 
 	{
 		$description = $row["description"];
		if (strpos($description, ";")) {
			$description = substr($description,0,strpos($description,";"));
		}
		$sequence = $row["sequence"];

		if ($lib == 'Protein') {
		
			if (strlen($row["sequence"])) {
				fputs($fp,">".$row["eg_id"]." ".ucfirst($row["name"])." ".$description);

				for ($i=0;$i<strlen($sequence);$i++) {
					if (($i % SEQ_LINE)==0) {
						fputs($fp,"\n");
					}
					fputs ($fp,$sequence[$i]);			
					if ($i == strlen($sequence) - 1)
						fputs ($fp,"\n");
				}
			}
		
		}elseif ($lib == 'SEQ') {
			
			if (is_numeric($us) && is_numeric($ds) && ($ds>=$us))
			{
				$main_string='';
				$up_string='';
				$down_string='';
				if ($catRadioA=='FASTA'){
					fputs($fp,">".$row["eg_id"]." ".$row["name"]." ".$description."\n");
				}elseif ($catRadioA=='Tabular')
				{
					fputs($fp, $row["eg_id"]." ".$row["name"]." ".$description."\t");
				}
				$from = $us<0?-$us:$us;
				$to = $ds<0?-$ds:$ds;
				get_sequence($row["eg_id"],$from,$to,$main_string,$up_string, $down_string);
						
				if ($catRadioB=='start')
				{
					$string = $main_string.$down_string;
					if($us<0)
					{
						if($ds<0)
						{
							fputs ($fp,substr($up_string,0, $us-$ds-1));
						}
						else
						{ 
							fputs ($fp,substr($up_string,$us)." ");
							fputs ($fp,substr($string,0,$ds));
						}
					}else
					{
						fputs ($fp,substr($string,$us-1,$ds-$us+1));
					}
					fputs($fp,"\n");
				}elseif ($catRadioB=='stop')
				{
					$string = $up_string.$main_string;
					if($us<0 || $us==0)
					{
						if($ds<0 || $ds==0)
						{						
//							fputs ($fp,substr($string,$us-1, $ds-$us+1));
							fputs ($fp,substr($string,$us, $ds-$us+1));
						}
						else
						{ 
//							fputs ($fp,substr($string,$us-1)." ");
							fputs ($fp,substr($string,$us)." ");
							fputs ($fp,substr($down_string,0,$ds));
						}
					}else{
				
						fputs ($fp,substr($down_string,$us-1,$ds-$us+1));
//						fputs ($fp,substr($down_string,$us,$ds-$us+1));
					}
					fputs($fp,"\n");
				}
			}
		
		}
	 	else {
		
			fputs($fp,">".$row["eg_id"]." ".ucfirst($row["name"])." ".$description);
		
				$main_string='';
				$up_string='';
				$down_string='';
				
								
				get_sequence($row["eg_id"],0,0,$main_string,$up_string, $down_string);
				$sequence = $main_string;
		
			if ($lib == 'RNA')
			{
            	 $sequence = str_replace("T", "U", $main_string);
			}		
	 		for ($i=0;$i<strlen($sequence);$i++) {
				if (($i % SEQ_LINE)==0) {
						fputs($fp,"\n");
				}
				fputs ($fp,$sequence[$i]);			
				if ($i == strlen($sequence) - 1)
						fputs ($fp,"\n");
				}
			}
 	
 	}
 	fclose($fp);

   startdownload($tmpfname); 
}
function download_sequence_return() {
	global $GENOME_LENGTH;
	global $GENOME_LENGTH_20;
	global $GENOME_LENGTH_10;
	global $tmpdr;

	$genome = $_REQUEST['genome_name'];
	$from = $_REQUEST['from'];
	$to = $_REQUEST['to'];
	$reversed = $_REQUEST['radio'];
	
	if($reversed==0){
		$tmpfname = $tmpdr.$from."_".$to;	
		
	}else{
		$tmpfname = $tmpdr.$to."_".$from;
	}
	if($genome=="ECOLI_3"){
		if($to> $GENOME_LENGTH){	$to = $GENOME_LENGTH;	}
		
		$tmpfext = ".txt";
		
	}elseif($genome=="ECOLI_2"){
		if($to> $GENOME_LENGTH_20){	$to = $GENOME_LENGTH_20;	}
		
		$tmpfext = ".txt";
		
	}else
	//if($genome=="ECOLI_1") 
	{
		if($to> $GENOME_LENGTH_10){		$to = $GENOME_LENGTH_10;	}	
		$tmpfext = "_1.txt";
		
	}
	$tmpfname .= $tmpfext;
	$fp = fopen($tmpfname, "w");
	
	get_download_sequence_head($genome,$from,$to,$reversed,$fp);
	get_download_sequence_body($genome,$from,$to,$reversed,$fp);
	fclose($fp);
	
	startdownload($tmpfname);  
	
}

function download_intergene_return() {
	global $GENOME_LENGTH;
	global $GENOME_LENGTH;
	global $tmpdr;
	
	$threshold_length=46;
	$tmpfname = $tmpdr."EcoInterGene".date("mdy-His"); 
		
	$fp = fopen($tmpfname, "w");
	if(!$fp)	return;
	
	db_set_active('ecogene');
	$request = $_REQUEST;
		
    foreach ($request['fields'] as $key => $value)
    {
    	$request[$key] = $value;
    }
    unset($request['fields']);
    
    $overlaps = isset($request['is_overlap'])? 1:0;
    
	$regions = isset($request['is_pseudo'])? 1:0;
	$head_st = array();
	if(isset($request['intergene_table']))
	{
		$format_file = 0;  // dable format
		write_intergene_table_header($fp, $request,$head_st);
	}elseif (isset($request['intergene_sequence']))
	{
		$format_file = 2;  // sequence
		write_intergene_table_header(0, $request,$head_st);
//		print_r($srequest);
		$row = db_query("Select * from t_sequence")->fetchAssoc();
		$sequence = $row['sequence'];
//		print_r(substr($sequence, 0,100));
		
	}
	
	
	if ($regions == 1) //Exclude Intergenes (REP, IRU, etc.) from intergenic sequence files
	{	
	// only id_type 'is' need to be merged / grouped, to get the leftmost and rightmost coordinates
	$querygene = "SELECT '' as name1, g.name as name2,ta.left_end,ta.right_end,ta.orientation, g.eg_id as eg_id,  '' as tier   
				  FROM t_address ta  
				  LEFT OUTER JOIN t_gene g ON (ta.address_id = g.address_id  and g.multi_location <> 'splitgene'  and g.multi_location <> 'split_isgene')	 
				  where  (g.name is not null and g.name!='')
			union

			SELECT '' as name1, g.name as name2,ta.left_end,ta.right_end,ta.orientation, ta.eg_id as eg_id,  '' as tier   
				  FROM t_gene_split_address ta  
			  		LEFT OUTER JOIN t_gene g ON ta.eg_id = g.eg_id 
			 	WHERE  (g.multi_location = 'splitgene'  or g.multi_location = 'split_isgene' )

			union

			SELECT '' as name1, g.name as name2,ta.left_end,ta.right_end,ta.orientation, g.eg_id as eg_id,  '' as tier   
			  FROM t_gene_multi_address ta  
			  LEFT OUTER JOIN t_gene g ON (ta.eg_id = g.eg_id )
			UNION

			  SELECT tis.name2 as name1, '' as name2, ta.left_end, ta.right_end,ta.orientation, '' as eg_id, CONCAT(case when ta.id_type='is' then '' else tis.id end, tis.name2 ) as tier   
              FROM t_is_address tis LEFT JOIN t_address ta ON (ta.address_id = tis.address_id )
			  where  (tis.name2 is not null  and  tis.is_type='IG')  

			ORDER BY left_end";
	} 
	else
	{

		$querygene = "SELECT '' as name1, g.name as name2,ta.left_end,ta.right_end,ta.orientation, g.eg_id as eg_id,  '' as tier   
					  FROM t_address ta  
			  			LEFT OUTER JOIN t_gene g ON (ta.address_id = g.address_id  and g.multi_location <> 'splitgene'  and g.multi_location <> 'split_isgene')	 
			  		where  (g.name is not null and g.name!='')
				union

					SELECT '' as name1, g.name as name2,ta.left_end,ta.right_end,ta.orientation, ta.eg_id as eg_id,  '' as tier   
			  		FROM t_gene_split_address ta  
			  			LEFT OUTER JOIN t_gene g ON ta.eg_id = g.eg_id 
			 		WHERE  (g.multi_location = 'splitgene'  or g.multi_location = 'split_isgene' )

			union

				SELECT '' as name1, g.name as name2,ta.left_end,ta.right_end,ta.orientation, g.eg_id as eg_id,  '' as tier   
			  		FROM t_gene_multi_address ta  
			  		LEFT OUTER JOIN t_gene g ON (ta.eg_id = g.eg_id )
			ORDER BY left_end";

		}

		
 	$query = db_query($querygene);
 	
 	while($row = $query->fetchAssoc())
 		$row_gene[] = $row;
 	
	$row_gene = get_non_nested($row_gene);


 	$row_left_gene = $row_gene[0];
 

//  	
 $row_first_gene = $row_left_gene; // saved to the special case where it circulates

 for ($n=1; $n<count($row_gene); $n++)
 {
 	$row_right_gene = $row_gene[$n];
 	 
 	$left_name = "";
 	$right_name = "";
 	if ($row_left_gene['name1'] != "") {
 		$left_name = $row_left_gene['name1']; 		
 		$row_left_gene['orientation'] = correct_is_orientation($row_gene,$n-1,1);
 		
 	} elseif ($row_left_gene['name2'] != "") {
 		$left_name = $row_left_gene['name2'];
 		
 	}

 	if ($row_right_gene['name1'] != "") {
 		$right_name = $row_right_gene['name1'];
 		$row_right_gene['orientation'] = correct_is_orientation($row_gene,$n,2);
 	} elseif ($row_right_gene['name2'] != "") {
 		$right_name = $row_right_gene['name2'];
 		
 	}

 	if ($right_name != "" and $left_name !="") {

 		
 		if(!strcmp(isPseudogene($row_left_gene['eg_id']),'PSEUDO')){
 			$left_name = $left_name."'";
 		}
 		if(!strcmp(isPseudogene($row_right_gene['eg_id']),'PSEUDO')){
 			$right_name = $right_name."'";
 		} 
 				
 		$name = $left_name.'_'.$right_name; // rule for name

 		if($row_right_gene['left_end']>$row_left_gene['right_end']+1)
 		{

 			$left_end = $row_left_gene['right_end'] + 1;
 			$right_end = $row_right_gene['left_end'] - 1;

 			$length = $right_end - $left_end + 1;
			$centisome = $left_end / $GENOME_LENGTH * 100;

 		}elseif($row_right_gene['left_end']==$row_left_gene['right_end']+1)
 		{
 			$centisome = $row_right_gene['left_end'] / $GENOME_LENGTH * 100;
 			$left_end='NULL';
 			$right_end='NULL';
 			$length=0;
 		}
 		else
 		{
 			if($overlaps==0) continue;

 			$left_end = $row_right_gene['left_end'];
 			$right_end = $row_left_gene['right_end'];


 			$length = $right_end - $left_end + 1;
 			if ($length>0)
 			{
 				$length = -$length;
 			}
 			$centisome = $left_end / $GENOME_LENGTH * 100;

 		}

 		

 		if(($row_left_gene['orientation']=='Clockwise') && ($row_right_gene['orientation']=='Clockwise'))
 		{
 			$orientation = 'Codirectional+';
 		}
 		elseif (($row_left_gene['orientation']=='Clockwise') && ($row_right_gene['orientation']=='Counterclockwise'))
 		{
 			$orientation = 'Convergent';
 		}
 		elseif (($row_left_gene['orientation']=='Counterclockwise' )&& ($row_right_gene['orientation']=='Clockwise'))
 		{
 			$orientation = 'Divergent';
 		}
 		else
 		{
 			$orientation = 'Codirectional-';
 		}

 		// write the file's head
 		if($format_file==0){

 			//			if($head_st[0]) fputs($fp,sprintf("EIG%5d\t",$eig_num));;
 			if($head_st[1]) fputs($fp,$name."\t");
 			if($head_st[2]) fputs($fp,$length."\t");
 			if($head_st[3]) fputs($fp,$orientation."\t");
 			if($head_st[4]) fputs($fp,$left_end."\t");
 			if($head_st[5]) fputs($fp,$right_end."\t");
 			if($head_st[6]) fputs($fp,sprintf("%.2f\t",$centisome));
 			fputs($fp,"\n");
 		}
 		elseif($format_file==2){
 			if (abs($length) > $threshold_length) {
				fputs($fp, ">".$name." ".$length." bp ".$orientation." intergenic region");
				$ig_seq = substr($sequence, $left_end, abs($length));
				for ($i=0;$i<strlen($ig_seq);$i++) {
					if (($i % SEQ_LINE)==0) {
						fputs($fp, "\n");
					}
					fputs($fp, $ig_seq[$i]);
								
					if ($i == strlen($ig_seq) - 1)
					fputs($fp, "\n");
				}	
				if(strlen($ig_seq)<1)	fputs($fp, "\n");
			}	
 		}
 		$row_left_gene = $row_right_gene;
 	}
 }
 
 // This Section is the special case where it circulates
 

 	$row_right_gene = $row_first_gene;
 		
 	$left_name = "";
 	$right_name = "";
 	if ($row_left_gene['name1'] != "") {
 		$left_name = $row_left_gene['name1'];
 	} elseif ($row_left_gene['name2'] != "") {
 		$left_name = $row_left_gene['name2'];
 	}

 	if ($row_right_gene['name1'] != "") {
 		$right_name = $row_right_gene['name1'];
 	} elseif ($row_right_gene['name2'] != "") {
 		$right_name = $row_right_gene['name2'];
 	}	
 	if(!strcmp(isPseudogene($row_left_gene['eg_id']),'PSEUDO')){
 			$left_name = $left_name."'";
 	}
 	if(!strcmp(isPseudogene($row_right_gene['eg_id']),'PSEUDO')){
 			$right_name = $right_name."'";
 	} 
 		
 		
	$name = $left_name.'_'.$right_name;
	$left_end = $row_left_gene['right_end']+1;
	$right_end = $row_right_gene['left_end']-1;

	$first_length = ($GENOME_LENGTH - $left_end)+1;
	$length = $first_length + $right_end;
	$centisome = $left_end / $GENOME_LENGTH * 100;
	
	if(($row_left_gene['orientation']=='Clockwise') && ($row_first_gene['orientation']=='Clockwise'))
	{
		$orientation = 'Codirectional+';
	}
	elseif (($row_left_gene['orientation']=='Clockwise') && ($row_first_gene['orientation']=='Counterclockwise'))
	{
		$orientation = 'Convergent';
	}
	elseif (($row_left_gene['orientation']=='Counterclockwise' )&& ($row_first_gene['orientation']=='Clockwise'))
	{
		$orientation = 'Divergent';
	}
	else
	{
		$orientation = 'Codirectional-';
	}
		
		if($format_file==0){
			
//			if($head_st[0]) fputs($fp,sprintf("EIG%5d\t",$eig_num));; 
			if($head_st[1]) fputs($fp,$name."\t");
			if($head_st[2]) fputs($fp,$length."\t");
			if($head_st[3]) fputs($fp,$orientation."\t");
			if($head_st[4]) fputs($fp,$left_end."\t");
			if($head_st[5]) fputs($fp,$right_end."\t");
			if($head_st[6]) fputs($fp,sprintf("%.2f\t",$centisome));
			fputs($fp,"\n");
		}elseif ($format_file==2)
		{
			if ($length > $threshold_length) {
				fputs($fp, ">".$name." ".$length." bp ".$orientation." intergenic region");

				$ig_seq = substr($sequence, $left_end, $length);
				$ig_seq = $ig_seq.substr($sequence,0, $right_end);

				for ($i=0;$i<strlen($ig_seq);$i++) {
					if (($i % SEQ_LINE)==0) {
						fputs($fp, "\n");
					}
					fputs($fp, $ig_seq[$i]);
					if ($i == strlen($ig_seq) - 1)
					fputs($fp, "\n");
				}
				if(strlen($ig_seq)<1)	fputs($fp, "\n");
		
			}
		}
	
	startdownload($tmpfname);
	db_set_active('default');
}

function write_intergene_table_header($fp, $request, &$head_st)
{
	$head_st = array(0,0,0,0,0,0,0);
//	print_r($request);
	if(isset($request['vname']))	$head_st[1]=1;
	if(isset($request['vlength']))	$head_st[2]=1;
	if(isset($request['vorientation']))	$head_st[3]=1;

	if(isset($request['vleft_end']))	$head_st[4]=1;
	if(isset($request['vright_end']))	$head_st[5]=1;
	if(isset($request['vcentisome']))	$head_st[6]=1;
	
	$head_arr = array('Accession', 'Name', 'Length', 'Orientation', 'L_END', 'R_END' ,'Cs'); 
	
	if($fp){
			
		for($loop=1;$loop<count($head_arr);$loop++)
		{
			
			if(($head_st[$loop])==1)
			{
				
				fputs($fp,$head_arr[$loop]."\t");				
			}
		}
	
		fputs($fp,"\n");
	}
	
}


function download_dbtable_return() {
	global $tmpdr;
	
	$request = $_REQUEST;
	
    foreach ($request['fields_1'] as $key => $value)
    {
    	$request[$key] = $value;
    }
    unset($request['fields_1']);
   
	$tmpfname =  $tmpdr."EcoData".date("mdy-His").".txt"; 
	$fp = fopen($tmpfname, "w");
	
	$head_st = download_dbtable_head($fp, $request);
	download_dbtable_body($fp, $head_st, $request);
	fclose($fp);
	startdownload($tmpfname);
	
}
function download_primer_return() {
	global $tmpdr;
	
	$tmpfname =  $tmpdr."PrimerPairs".date("mdy-His").".txt";; 
	$fp = fopen($tmpfname, "w");
	
	db_set_active("ecogene");
	
//	print_r($_REQUEST);
	$request = primer_get_download_request($_REQUEST);
	
//	print_r($request);
	$warning_msg = "";
	$row_gene = prime_get_gene_content($request,$warning_msg);	
	if(strlen($warning_msg)>0)
	{
		
		fputs($fp,"%\tWarning:\tThe following genes are excluded in the download \t\r\n%\t".$warning_msg."\t \r\n%\t\r\n");
	}
//	print_r($row_gene);
	
	$double_del = array();
	$double_created = array();
	prime_check_double_deletion($row_gene, $request,&$double_del,&$double_created);
	

	primer_put_downlaod_head($request, $fp);	
	prime_put_download_body($row_gene,$request,$fp,$double_del,$double_created);
	
	
	db_set_active("default");
	
	startdownload($tmpfname);    
	
}
function primer_get_download_request($request)
{
	if(strcmp($request['clon_del'],'deletion')==0)
	{
		$request['start_offset'] = is_numeric($request['del_start_offset'])? $request['del_start_offset']:0; 
		$request['stop_offset'] = is_numeric($request['del_stop_offset'])? $request['del_stop_offset']:0; 
		$request['inside'] = true;
		
		if(strcmp($request['del_addon_type'],'no')==0)
		{
			$request['start_add_ons'] = ""; 
			$request['stop_add_ons'] = "";
		}
		elseif (strcmp($request['del_addon_type'],'kan')==0)
		{
			$request['start_add_ons'] = "ATTCCGGGGATCCGTCGACC"; 
			$request['stop_add_ons'] = "GTGTAGGCTGGAGCTGCTTC";
		}elseif (strcmp($request['del_addon_type'],'cat')==0)
		{
			$request['start_add_ons'] = "GTGTAGGCTGGAGCTGCTTC"; 
			$request['stop_add_ons'] = "CATATGAATATCCTCCTTAG";
//		}
//		elseif (strcmp($request['del_addon_type'],'kan')==0)
//		{
//			$request['start_add_ons'] = "GTGTAGGCTGGAGCTGCTTC"; 
//			$request['stop_add_ons'] = "ATGGGAATTAGCCATGGTCC";
		}elseif(strcmp($request['del_addon_type'],'yours')==0)
		{
			$request['start_add_ons'] = $request['del_start_add_ons']; 
			$request['stop_add_ons'] = $request['del_stop_add_ons'];
		}else 
		{
			$request['start_add_ons'] = ""; 
			$request['stop_add_ons'] = "";
		}
		
		
	}elseif(strcmp($request['clon_del'],'cloning')==0)
	{
		$request['start_offset'] = is_numeric($request['clo_start_offset'])? $request['clo_start_offset']:0; 
		$request['stop_offset'] = is_numeric($request['clo_stop_offset'])? $request['clo_stop_offset']:0; 
		
		$request['inside'] = false;
		if(strcmp($request['clo_offset_type'],'inside')==0)
		{
			$request['inside'] = true;
		}
		
		
		if(strcmp($request['clo_addon_type'],'no')==0)
		{
			$request['start_add_ons'] = ""; 
			$request['stop_add_ons'] = "";
		}elseif(strcmp($request['clo_addon_type'],'yours')==0)
		{
			$request['start_add_ons'] = $request['clo_start_add_ons']; 
			$request['stop_add_ons'] = $request['clo_stop_add_ons'];
		}else 
		{
			$request['start_add_ons'] = ""; 
			$request['stop_add_ons'] = "";
		}
	}
	return $request;
}
function primer_put_downlaod_head($request, $fp)
{
	
	if($request['clo_offset_type']=='peptides')
	{
		$head_arr = array('EG_ID','gene','b#','start_primer(fwd)','start_codon(fwd)','mature_length','stop_primer(rev)','stop_codon(rev)');
	}else 
	{
		$head_arr = array('EG_ID','gene','b#','start_primer(fwd)','start_codon(fwd)','gene_length','stop_primer(rev)','stop_codon(rev)');
	}
	
	if(strlen($request['start_add_ons'] )>0)
		$head_arr[3] = 'primer_add_on_'.$head_arr[3];
	if(strlen($request['stop_add_ons'] )>0)
		$head_arr[6] = 'primer_add_on_'.$head_arr[6];
	if(strcmp($request['clon_del'],'deletion')==0)
		$head_arr[5] = 'deletion/gene_length';	
			

	$head_st = array();
	if(isset($request['veg_id']))										$head_st[0]=1;
	if(isset($request['vname']))										$head_st[1]=1;
	if(isset($request['vbnum']))										$head_st[2]=1;

	if(isset($request['vstart_pr']))		$head_st[3]=1;
	if(isset($request['vstart_co']))									$head_st[4]=1;
	if(isset($request['vlength']))										$head_st[5]=1;
	if(isset($request['vstop_pr']))		$head_st[6]=1;
	if(isset($request['vstop_co']))										$head_st[7]=1;	

	fputs($fp,"Primers Info");
	
	if(strcmp($request['clon_del'],'deletion')==0)// inside only for deletion
	{
		
		for($loop=0;$loop<8;$loop++)	
		{
			if(($head_st[$loop])==1)	fputs($fp,"\t");
		}	
		
		fputs($fp,"Double Deletions\t \t \tAll non-overlapping primers\t \r\n");
		
	}
	else {
	
		fputs($fp,"\r\n");
	}
	
	for($loop=0;$loop<count($head_arr);$loop++)
	{
		if(($head_st[$loop])==1)
		{
			fputs($fp,$head_arr[$loop]."\t");

		}
	}
	if(strcmp($request['clon_del'],'deletion')==0)// inside only for deletion
	{
		
		fputs($fp,"Gene Affected\t5' or 3'\t0verlap\tStart primer(fwd)\tStop primer (rev)\t");
		
	}
	fputs($fp,"\r\n");
}

function prime_get_gene_content($request,&$warning_msg)
{

		$exeption_gene = array('EG40121','EG13822','EG11307','EG14228','EG13605','EG10659','EG14236','EG14308','EG12162','EG11986','EG12351','EG14315','EG12811','EG13828','EG13337','EG13205','EG14381','EG14380','EG14252','EG13366','EG13636','EG10373','EG12346','EG11304','EG11016','EG13640','EG14351','EG10507','EG11240','EG14425','EG13365','EG12113','EG14428','EG13555','EG13369','EG12922','EG14081','EG14138','EG14418','EG14259','EG12584','EG13044','EG14308','EG12999');
			 
//	$query_int = db_query("select distinct(gene_interrupted)  from t_gene,  t_is_address where gene_interrupted!='' and length(gene_interrupted)<8 and gene_interrupted in (select eg_id from t_gene_split_address) and t_gene.eg_id=gene_interrupted");
//
//	$searchegs_int = "";
//	while($row_int = db_fetch_array($query_int))
//	{
//		
//			$searchegs_int = $searchegs_int."'".$row_int['gene_interrupted']."',";
//	}
//		//remove "," of last item
//	$searchegs_int = strrev(substr(strrev($searchegs_int),1));

	
	$query = "select t_gene.eg_id,t_gene.name, t_biodatabase_link.accession_id as bnum, t_address.left_end,t_address.right_end, (t_address.right_end-t_address.left_end+1) as length, t_address.orientation
		  from t_gene left join t_address on t_gene.address_id = t_address.address_id left join t_biodatabase_link on 
				t_gene.eg_id= t_biodatabase_link.eg_id
		  where t_gene.address_id = t_address.address_id and t_biodatabase_link.biodatabase_id=3";

	if ( !empty($request['egids'] )&&isset($request['egids'] )) {
		$egidA = array();
		$searchegs = "";
		if ($request['egids'] != "") {
			$egidA = explode(',',$request['egids']);
		}
		foreach($egidA as $value) {
			$searchegs = $searchegs."'".$value."',";
		}
		//remove "," of last item
		$searchegs = strrev(substr(strrev($searchegs),1));

		$query = $query." and t_gene.eg_id in (".$searchegs.")";

	}
//	$query = $query." and t_gene.eg_id not in (".$searchegs_int.")";
	
//	if (!isset($request['is_pseudo'])) {
		$query = $query." and t_gene.eg_id not in (select distinct eg_id from t_pseudogene)";
//	}
	$query = $query." and t_gene.eg_id not in (select distinct eg_id from t_topic_gene_link where topic_id='8')";
	$query = $query." and t_gene.eg_id not in (select distinct eg_id from t_gene where eg_id like 'EG6%')";
	
	if (($request['gene_type']=='prot')) {
		$query = $query." and t_gene.type = 'aa'";

	}
	if (($_POST['gene_type']=='rna')) {
		$query = $query." and t_gene.type = 'nt'";

	}
	$query = $query." group by t_gene.eg_id";
	$query_gene = db_query($query);
	
	
	$i=0;
	while ($row = $query_gene->fetchAssoc()) 
 	{
 		if(strcmp($request['clon_del'],'deletion')==0)// inside only for deletion
 		{
 			if(in_array($row['eg_id'],$exeption_gene))
 			{
 				$warning_msg .= $row['eg_id']." ";
 				continue;
 			}
 		}
	
 		$row_gene[$i] = $row;
 		$row_gene[$i]['length'] = $row_gene[$i]['right_end'] - $row_gene[$i]['left_end'] + 1; 		
 		
 		$i = $i+1;
 		
 	}
 	
 	return $row_gene;
}

function prime_put_download_body($genes,$request,$fp,$double_del,$double_created)
{

	if(strcmp($request['clon_del'],'deletion')==0)// inside only for deletion
	{
		$us2 = $request['start_len']-$request['start_offset'];
		$ds2 = $request['stop_len']-$request['stop_offset']; 
		$us = $us2 > 0 ?$us2 : 0;
		$ds =  $ds2> 0? $ds2 : 0;
	}else // inside or outside for cloning
	{
		if($request['inside'])
		{		
			
			$us = 0;
			$ds = 0;			
			$us2 = -$request['start_offset'] ;
			$ds2 = -$request['stop_offset'];

		}else{
			
			$us = $request['start_offset'];
			$ds = $request['stop_offset'];
			$us2 = 0;
			$ds2 = 0;
		}
		
	
	}
	

	$n_del = 0;
	for($i=0; $i<count($genes);$i++)
	{
		$row_gene = $genes[$i];
		if(isset($request['veg_id'])){
			fputs($fp,rtrim($row_gene['eg_id']."\t"));
			fputs($fp,"\t");
		}

		if(isset($request['vname'])){
			fputs($fp,rtrim($row_gene['name'],"\r\n\t"));
			fputs($fp,"\t");

		}
		if(isset($request['vbnum'])){
			fputs($fp,rtrim($row_gene['bnum'],"\r\n\t"));
			fputs($fp,"\t");

		}
// 	
//		
		$main_string='';
		$up_string='';
		$down_string='';
		get_sequence($row_gene["eg_id"],$us,$ds,&$main_string,&$up_string, &$down_string);
		
		
		
		if($request['clo_offset_type']=='peptides')
		{
			$query = db_query(" select
						tpf.protein_id,
						tpf.predicted, 
						ta1.left_end as left1, 
						ta1.right_end as right1,
 						ta2.left_end as left2,
 						ta2.right_end as right2 
					from t_product_protein_features tpf, t_address ta1, t_product_protein tpr, t_address ta2 
					where tpr.protein_id=tpf.protein_id and ta2.address_id=tpf.address_id and ta1.address_id=tpr.address_id and tpr.eg_id= '%s'",$row_gene["eg_id"]);
			
			
	
			
		 	if($row_att = $query->fetchAssoc())
		 	{
//		 		if($row_att['predicted']==0)
//		 		{
//		 			continue;
//		 		}else 
		 		{
//		 			$shift_bit = $row_att['value']*3;
					$shift_bit = max(abs($row_att['left2']-$row_att['left1']),abs($row_att['right2']-$row_att['right1']));	 			
		 			$up_string = $up_string.substr($main_string,0,$shift_bit);
		 			$up_string = substr($up_string,$shift_bit);
//		 			$up_string = substr($up_string,$shift_bit);
		 			$main_string = substr($main_string,$shift_bit);
		 			$row_gene['length'] = $row_gene['length']-$shift_bit; // 
		 		}
		 	 	
		 	}
		}
		
		if($us2 < 0)
		{
			$seq_str = substr($main_string,-$us2);
			
		}else {
			
			$seq_str = $up_string.$main_string;
		}
		if($ds2 < 0)
		{
			$seq_str = substr($seq_str,0,$ds2);
			
		}else {
			
			$seq_str = $seq_str.$down_string;
		}
		
		$start_pr_str = substr($seq_str,0,$request['start_len']);
		if(isset($request['vstart_pr'])){
			if(strcmp($request['clon_del'],'deletion')==0)
				fputs ($fp,$start_pr_str.$request['start_add_ons']);
			else
				fputs ($fp,$request['start_add_ons'].$start_pr_str);
			fputs($fp,"\t");
			
		}

	
		if(isset($request['vstart_co'])){
			fputs ($fp,substr($main_string,0,3));
			fputs($fp,"\t");

		}
		if(isset($request['vlength'])){
			if(strcmp($request['clon_del'],'deletion')==0)
			{
				$gen_len = rtrim($row_gene['length'],"\r\n\t");
				$del_len = $gen_len - $request['start_offset'] - $request['stop_offset'];
				fputs($fp,$del_len.'/');
				fputs($fp,$gen_len);
				fputs($fp,"\t");
			}
			else {
			fputs($fp,rtrim($row_gene['length'],"\r\n\t"));
			fputs($fp,"\t");
			}

		}
		
		$stop_pr_str = substr($seq_str,-$request['stop_len']);
		$stop_pr_str = str_revc($stop_pr_str);
		if(isset($request['vstop_pr'])){			
			
			if(strcmp($request['clon_del'],'deletion')==0)
				fputs ($fp,$stop_pr_str.$request['stop_add_ons']);
			else
				fputs ($fp,$request['stop_add_ons'].$stop_pr_str);
			fputs($fp,"\t");

		}	
		if(isset($request['vstop_co'])){
			fputs ($fp,str_revc(substr($main_string,-3))." ");
			fputs($fp,"\t");

		}
		if(strcmp($request['clon_del'],'deletion')==0)// inside only for deletion
		{
			if($double_created[$i]==0){
		 		fputs($fp,"OK\t");	
		 		fputs($fp,"OK\t");
		 		fputs($fp,"OK\t");
//		 		fputs($fp,"OK\t");
//		 		fputs($fp,"OK\t");
				fputs ($fp,$start_pr_str.$request['start_add_ons']."\t");
				fputs ($fp,$stop_pr_str.$request['stop_add_ons']."\t");
				
		 		fputs($fp,"\r\n");
			}else{
				for($d_i=0;$d_i<$double_created[$i];$d_i++)
				{
					if($d_i>0)
					{
						if(isset($request['veg_id'])){
							fputs($fp,rtrim($row_gene['eg_id']."\t"));
							fputs($fp,"\t");
						}

						if(isset($request['vname'])){
							fputs($fp,rtrim($row_gene['name'],"\r\n\t"));
							fputs($fp,"\t");

						}
						if(isset($request['vbnum'])){
							fputs($fp,rtrim($row_gene['bnum'],"\r\n\t"));
							fputs($fp,"\t");

						}
						if(isset($request['vstart_pr'])){
							fputs ($fp,$request['start_add_ons'].substr($seq_str,0,$request['start_len']));
							fputs($fp,"\t");

						}


						if(isset($request['vstart_co'])){
							fputs ($fp,substr($main_string,0,3));
							fputs($fp,"\t");

						}
						if(isset($request['vlength'])){
							$gen_len = rtrim($row_gene['length'],"\r\n\t");
								$del_len = $gen_len - $request['start_offset'] - $request['stop_offset'];
								fputs($fp,$del_len.'/');
								fputs($fp,$gen_len);
								fputs($fp,"\t");
//							if((strcmp($double_del[$n_del]['start_stop'],'start')==0))
//							{
//								$start_pr_str = substr($seq_str,$double_del[$n_del]['overlap'],$request['start_len']);
//								fputs ($fp,$request['start_add_ons'].$start_pr_str."\t");
//								fputs ($fp,$request['stop_add_ons'].$stop_pr_str."\t");
//								//						fputs($fp,"OK\t");
//
//							}else
//							{
//								//						fputs($fp,"OK\t");
//								$stop_pr_str = substr($seq_str,-$request['stop_len']-$double_del[$n_del]['overlap'],$request['stop_len']);
//								$stop_pr_str = str_revc($stop_pr_str);
//								fputs ($fp,$request['start_add_ons'].$start_pr_str."\t");
//								fputs ($fp,$request['stop_add_ons'].$stop_pr_str."\t");
//							}

						}

						if(isset($request['vstop_pr'])){
							$stop_pr_str = substr($seq_str,-$request['stop_len']);
							$stop_pr_str = str_revc($stop_pr_str);

							fputs ($fp,$request['stop_add_ons'].$stop_pr_str);
							fputs($fp,"\t");

						}
						if(isset($request['vstop_co'])){
							fputs ($fp,str_revc(substr($main_string,-3))." ");
							fputs($fp,"\t");

						}
					}
					fputs($fp,$double_del[$n_del]['name']."\t");
					fputs($fp,$double_del[$n_del]['end_type']."\t");
					fputs($fp,$double_del[$n_del]['overlap']."\t");
					if((strcmp($double_del[$n_del]['start_stop'],'start')==0))
					{
//						$start_pr_str = substr($seq_str,$double_del[$n_del]['overlap'],$request['start_len']);
//						fputs ($fp,$start_pr_str."\t");		
//						fputs($fp,"OK\t");		
						$str_triple = ceil($double_del[$n_del]['overlap']/3.0)*3;
						$start_pr_str = substr($seq_str,$str_triple,$request['start_len']);
						fputs ($fp,$start_pr_str.$request['start_add_ons']."\t");	
						fputs ($fp,$stop_pr_str.$request['stop_add_ons']."\t");		
						
					}else 
					{
//						fputs($fp,"OK\t");
//						$stop_pr_str = substr($seq_str,-$request['stop_len']-$double_del[$n_del]['overlap'],$request['stop_len']);
//						$stop_pr_str = str_revc($stop_pr_str);
//						fputs ($fp,$stop_pr_str."\t");	
						
						$str_triple = ceil($double_del[$n_del]['overlap']/3.0)*3;
						$stop_pr_str = substr($seq_str,-$request['stop_len']-$str_triple,$request['stop_len']);
						$stop_pr_str = str_revc($stop_pr_str);
						fputs ($fp,$start_pr_str.$request['start_add_ons']."\t");
						fputs ($fp,$stop_pr_str.$request['stop_add_ons']."\t");
					}
					$n_del++;
					fputs($fp,"\r\n");
				}
			}

		}else {
			fputs($fp,"\r\n");
		}	
		
	}

	
}

function prime_check_double_deletion($del_gene, $request,$double_del,$double_created)
{
	global $GENOME_LENGTH;
	if(strcmp($request['clon_del'],'deletion')==0)// inside only for deletion
	{
		$us2 = $request['start_len']-$request['start_offset'];
		$ds2 = $request['stop_len']-$request['stop_offset']; 
		$double_created = array();		
		
	}else{
		return;
	}
	

	
	$extension = 0;
	$query = db_query("select
			g.name ,  g.eg_id, g.eck_id, ta.orientation, ta.left_end, ta.right_end  FROM t_address ta, t_gene g
		WHERE 
			g.address_id = ta.address_id and
			g.eg_id not in (select eg_id from t_gene_multi_address )
		order by ta.left_end
		")	;


	$gene = array();
	$num_gene = 0;
	while ($row =$query->fetchAssoc())
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
	
$num_double = 0;

	for($i=0; $i<count($del_gene); $i++)
	{
		$eg_id = $del_gene[$i]["eg_id"];

		if ( $del_gene[$i]["orientation"] == "Clockwise")
		{
//			$del_gene[$i]["left_end"] = $del_gene[$i]["left_end"] - $us2;
//			$del_gene[$i]["right_end"] = $del_gene[$i]["right_end"] + $ds2;
			$del_gene[$i]["left_end"] = $del_gene[$i]["left_end"] + $request['start_offset'];
			$del_gene[$i]["right_end"] = $del_gene[$i]["right_end"] - $request['stop_offset'];
		}
		elseif ( $del_gene[$i]["orientation"] == "Counterclockwise")
		{
//			$del_gene[$i]["left_end"] = $del_gene[$i]["left_end"] - $ds2;
//			$del_gene[$i]["right_end"] = $del_gene[$i]["right_end"] + $us2;
			$del_gene[$i]["left_end"] = $del_gene[$i]["left_end"] + $request['stop_offset'];
			$del_gene[$i]["right_end"] = $del_gene[$i]["right_end"] - $request['start_offset'];
		}

		$key = array_search($eg_id, $gene_eg);

		if($key>0 && $key<$num_gene-1)
		{
			$double_flag = false;
			$tri_flag = false;
			if(($gene[$key-1]["right_end_ext"] >= $del_gene[$i]["left_end"]) )
			{
				$overlap = $gene[$key-1]["right_end_ext"] - $del_gene[$i]["left_end"] + 1;
				$key_2 = $key-1;
				prime_write_double(&$double_del,&$num_double,$del_gene[$i],$gene[$key_2],$overlap,'left');
				$double_flag = true;

			}
			if(($gene[$key+1]["left_end_ext"] <= $del_gene[$i]["right_end"]) )
			{
				$overlap = $del_gene[$i]["right_end"] - $gene[$key+1]["left_end_ext"] + 1;
				$key_2 = $key+1;
				prime_write_double(&$double_del,&$num_double,$del_gene[$i],$gene[$key_2],$overlap,'right');
				if($double_flag) $tri_flag = true;
				$double_flag = true;
			}
		}
		if($key==0)
		{
			$double_flag = false;
			if(($gene[$num_gene-1]["right_end_ext"]>$GENOME_LENGTH) && (($gene[$num_gene-1]["right_end_ext"]%$GENOME_LENGTH) >= $del_gene[$i]["left_end"]))
			{
				$overlap = $gene[$num_gene-1]["right_end_ext"]%$GENOME_LENGTH - $del_gene[$i]["left_end"] + 1;
				$key_2 = $num_gene-1;
				prime_write_double(&$double_del,&$num_double,$del_gene[$i],$gene[$key_2],$overlap,'left');
				$double_flag = true;

			}
			if(($gene[$key+1]["left_end_ext"] <= $del_gene[$i]["right_end"]))
			{
				$overlap = $del_gene[$i]["right_end"] - $gene[$key+1]["left_end_ext"] + 1;
				$key_2 = $key+1;
				prime_write_double(&$double_del,&$num_double,$del_gene[$i],$gene[$key_2],$overlap,'right');
				if($double_flag) $tri_flag = true;
				$double_flag = true;

			}
		}
		if($key==$num_gene-1)
		{
			$double_flag = false;
			if(($gene[$key-1]["right_end_ext"] >= $del_gene[$i]["left_end"]))
			{
				$overlap = $gene[$key-1]["right_end_ext"] - $del_gene[$i]["left_end"] + 1;
				$key_2 = $key-1;
				prime_write_double(&$double_del,&$num_double,$del_gene[$i],$gene[$key_2],$overlap,'left');
				$double_flag = true;

			}
			if(($gene[0]["left_end_ext"]<0) && ($gene[0]["left_end_ext"]%$GENOME_LENGTH <= $del_gene[$i]["right_end"]))
			{
				$overlap = $del_gene[$i]["right_end"] - $gene[0]["left_end_ext"]%$GENOME_LENGTH + 1;
				$key_2 = 0;
				prime_write_double(&$double_del,&$num_double,$del_gene[$i],$gene[$key_2],$overlap,'right');
				if($double_flag) $tri_flag = true;
				$double_flag = true;


			}
		}
		if($tri_flag){
			$double_created[]=2;
		}
		elseif($double_flag)
		{
			$double_created[]=1;//
			//fputs($fp,$del_gene[$i]['eg_id']."\t".$del_gene[$i]['orientation']."\t".$del_gene[$i]['left_end']."\t".$del_gene[$i]['right_end']."\t".$double_flag."\r\n");
		}else{
			$double_created[]=0;
		}

	}

}
function prime_write_double($double_del,$num_double,$del_gene,$gene_adj,$overlap,$left_right)
 {

 	if((strcmp($left_right,'left')==0))
 	{
 		$end_type = ((strcmp($gene_adj['ori'],'Clockwise')==0)?'3':'5');
 	}else {
 		$end_type = ((strcmp($gene_adj['ori'],'Clockwise')==0)?'5':'3');
 	}
 	if((strcmp($left_right,'left')==0))
 	{
 		$start_stop = ((strcmp($del_gene['orientation'],'Clockwise')==0)?'start':'stop');
 	
 	}else {
 		$start_stop = ((strcmp($del_gene['orientation'],'Clockwise')==0)?'stop':'start');
 	}
 	    

		$double_del[$num_double]["start_stop"] = $start_stop;
		$double_del[$num_double]["overlap"] = $overlap;
		$double_del[$num_double]["end_type"] = $end_type;
	
		$double_del[$num_double]["name"] = $gene_adj["name"];
		$double_del[$num_double]["start"] = 'aa';
		$double_del[$num_double]["stop"] = 'bb';
		$num_double++;		
		
 }

function str_revc($stop_pr_str)
{
			$stop_pr_str = str_replace('A','Z',$stop_pr_str);
			$stop_pr_str = str_replace('T','A',$stop_pr_str);
			$stop_pr_str = str_replace('Z','T',$stop_pr_str);
			$stop_pr_str = str_replace('C','Z',$stop_pr_str);
			$stop_pr_str = str_replace('G','C',$stop_pr_str);
			$stop_pr_str = str_replace('Z','G',$stop_pr_str);
			$stop_pr_str = strrev($stop_pr_str);
			return $stop_pr_str;
}	

function get_non_nested($row_gene)
{
	$neighbor = 20;
 	$num_gene = count($row_gene);
// 	echo $num_gene."<br>";
 	$non_nested=array();
 	$nested=array();
 	$n=0;
 	$nn=0;
 	for ($i=0; $i<$num_gene; $i++)
 	{
 		$flag=true;
 		for($j=$i+1; $j<$num_gene&& $j<$i+$neighbor; $j++)//check its downstring genes
 		{
 				if(($row_gene[$j]['right_end']>$row_gene[$i]['right_end']) && ($row_gene[$j]['left_end']<$row_gene[$i]['left_end'])){
 					$nested_in[$n]=$row_gene[$j];
 					$flag=false;
 				}
 		}
 		for($j=$i-1; $j>=0&&$j>$i-$neighbor; $j--)//check its upstring genes
 		{
 				if(($row_gene[$j]['left_end']<=$row_gene[$i]['left_end']) && ($row_gene[$j]['right_end']>=$row_gene[$i]['right_end'])){
 					$nested_in[$n]=$row_gene[$j];
 					$flag=false;
 				}
 		}
 		if($flag)
 		{
 			$non_nested[$nn]=$row_gene[$i];
 			$nn += 1;
 		}else 
 		{
 			$nested[$n]=$row_gene[$i];
 			$n += 1;
 		}
 		
 	}
		
 	return $non_nested;
}
  
function correct_is_orientation($row_gene,$pos,$type)
{
	$num_gene = count($row_gene);

	if ($type == 1) // is is on left
	{
		for($j=$pos-1; $j>0; $j--)
		{
			if($row_gene[$j]['name2']!="")
			{
				break;
			}

		}
	}
	if ($type == 2) // is is on right
	{
		for($j=$pos+1; $j<$num_gene; $j++)
		{
			if($row_gene[$j]['name2']!="")
			{
				break;
			}
		}
	}

	return $row_gene[$j]['orientation'];
}

function intergenedownload($type) {

	global $GENOME_LENGTH;
	global $tmpdr;
		
	db_set_active('ecogene');
	
	$querysequence = db_query("Select * from t_sequence");
	$row =$querysequence->fetchAssoc();
	$sequence = $row['sequence'];
	
	$tmpfname = $tmpdr."intergene".date("mdy-His"); 
		
	$fp = fopen($tmpfname, "w");
	if(!$fp)	return;
	if($type== 'REP_elements' || $type== 'RIP_elements')
	{
		fputs($fp,"name\tleft_end\tright_end\tlength\torientation\tcentisome\tsequence\tnum_REP\tnum_IHF");
	
	}else {
		
		fputs($fp,"name\tleft_end\tright_end\tlength\torientation\tcentisome\tsequence");
	}
	fputs($fp,"\n");

	
	
	if ($type == 'REP_sequences') 
	{	
		$querygene="select tis.*, tis.name2 as name_a, ta.* from t_is_address tis left join t_address ta on tis.address_id=ta.address_id where tis.name2 like 'REP%'";
	}
	elseif($type == 'REPt_sequence') 
	{
		$querygene="select tis.*, tis.name2 as name_a, ta.* from t_is_address tis left join t_address ta on tis.address_id=ta.address_id where tis.name2 like 'REPt%'";
	}
	elseif($type == 'IHF') 
	{
		$querygene="select tis.*, tis.name2 as name_a, ta.* from t_is_address tis left join t_address ta on tis.address_id=ta.address_id where tis.name2 like 'IHF%'";
	}elseif($type == 'RIP_elements') 
	{
		$querygene="select tis.*, tis.name as name_a, ta.* , 'N/A' as orientation from t_is_address_tmp tis left join t_address ta on tis.address_id=ta.address_id where tis.name like 'RIP%'";
	}elseif($type == 'REP_elements') 
	{
		$querygene="select tis.*, tis.name as name_a, ta.*, 'N/A' as orientation from t_is_address_tmp tis left join t_address ta on tis.address_id=ta.address_id ";
	}elseif($type == 'solo_REPS') 
	{
		$querygene="select tis.*,  tis.name as name_a, ta.*, 'N/A' as orientation from t_is_address_tmp tis left join t_address ta on tis.address_id=ta.address_id where (tis.num_REP+tis.num_IHF)=1";
	}
	
	$query_gene = db_query($querygene);
	
	while ($row = $query_gene->fetchAssoc()) 	
	{
	 	$length = $row['right_end']-$row['left_end']+1;
	 	$ig_seq = substr($sequence, $row['left_end'], abs($length));
	 	if(strcmp(trim($row['orientation']),'Counterclockwise')==0)
	 	{
	 		$ig_seq = strrev($ig_seq);
	
			$ig_seq = str_replace("Z", "T",(str_replace("T", "A",(str_replace("A", "Z", $ig_seq))))); /* A<->T */
	
			$ig_seq = str_replace("Z", "C",(str_replace("C", "G",(str_replace("G", "Z", $ig_seq))))); /* C<->G */
	
			$ig_seq = str_replace("U", "A", $ig_seq); /* U<->A */
	 	}
	 	
	 	fputs($fp,trim($row['name_a'])."\t");
	 	fputs($fp,trim($row['left_end'])."\t");
	 	fputs($fp,trim($row['right_end'])."\t");
	 	fputs($fp,trim($length)."\t");
	 	fputs($fp,trim($row['orientation'])."\t");
	 	fputs($fp,sprintf("%.2f\t",($row['left_end'] / $GENOME_LENGTH * 100)));
	 	fputs($fp,trim($ig_seq));
	 			
	 	if($type== 'REP_elements' || $type== 'RIP_elements')
	 	{
	 			 			 			
	 			fputs($fp,"\t".trim($row['num_rep'])."\t");
	 			fputs($fp,trim($row['num_ihf'])."\t");
	 			
	 		
	 	}fputs($fp,"\n");
	 		
	}
 
 
	fclose($fp);
	
	startdownload($tmpfname);
	db_set_active('default');
}