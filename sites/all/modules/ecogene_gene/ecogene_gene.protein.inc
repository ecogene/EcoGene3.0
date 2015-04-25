<?PHP
include("sites/all/include/format_description.php");
include_once("sites/all/include/seq_format.php");
include_once("sites/all/include/ecoFunction.php");

$tmpdr = 'temp/';

function ecogene_gene_protein_rna($eg_id) {

	db_set_active("ecogene");

	$args = array();
	$args['eg_id'] = $eg_id;
	
	$result = db_query("SELECT type FROM {t_gene} WHERE eg_id=:eg_id", $args);	
	$record = $result->fetchAssoc();
	db_set_active("default");
	
	$content = $record['type'] == "aa" ? ecogene_gene_protein($eg_id) : ecogene_gene_rna($eg_id);
  	return $content;	
}
define ("SEQ_LINE", 75);
function ecogene_gene_protein($eg_id) {
	
	set_protein_title($eg_id);
	$content = '<div class="gene_protein">';
	$content .= ecogene_gene_protein_link($eg_id);
	$content .= ecogene_gene_protein_verified($eg_id);
	$content .= ecogene_gene_protein_sequence($eg_id);
	$content .= '</div>';
	return $content;
}
function set_protein_title($eg_id)
{
	db_set_active('ecogene');
	$args['eg_id'] = $eg_id;
	$result = db_query("SELECT 
							g.eg_id,g.name as g_name, p.name as p_name 
						FROM 
							t_gene g, t_product_protein p 
						WHERE 
							g.eg_id = p.eg_id and g.eg_id = :eg_id", $args);
	$row = $result->fetchAssoc();	
	if($row)
	{
		$prot_title = ucfirst($row['g_name']).": ". $row['p_name'];
		drupal_set_title($prot_title, PASS_THROUGH);
	}
	db_set_active('default');
		
}
function set_rna_title($eg_id)
{
	db_set_active('ecogene');
	$args['eg_id'] = $eg_id;
	$result = db_query("SELECT 
							g.eg_id,g.name as g_name, p.name as p_name 
						FROM 
							t_gene g, t_product_rna p 
						WHERE 
							g.eg_id = p.eg_id and g.eg_id = :eg_id", $args);
	$row = $result->fetchAssoc();	
	if($row)
	{
		$pos = strpos($row['p_name'], ";");
		if($pos){
			$rna_title = ucfirst($row['g_name']).": ". substr($row['p_name'],0,$pos);
		}
		else{
			$rna_title = ucfirst($row['g_name']).": ". $row['p_name'];
		} 
				
		drupal_set_title($rna_title, PASS_THROUGH);
	}
	db_set_active('default');
}

function ecogene_gene_rna($eg_id) {
	set_rna_title($eg_id);
	$content = '<div class="gene_protein">';
	$content .= ecogene_gene_rna_link($eg_id);
	$content .= ecogene_gene_rna_sequence($eg_id);
	$content .= '</div>';
	return $content;
}
function verified_set()
{
		global $tmpdr;
		db_set_active('ecogene');
   		
	
		$result = db_query("select distinct(g.eg_id),g.name,g.length,g.status
				, GROUP_CONCAT(rl.pubmed_id SEPARATOR ';') as pubmed
			  from t_gene g, t_gene_reference_link rl
			  where g.eg_id = rl.eg_id
			  and rl.verified=1
			  GROUP BY g.eg_id
			  ")->fetchAll();
	
		
		$num_rows = count($result);
//		print_r ($result);
		$date = date("mdy-His");
//		$tmpfname = $tmpdr.'verified'.$date;
		$tmpfname = file_directory_temp().'/verified'.$date;
		
		$fpout = fopen($tmpfname, "w");
		fwrite($fpout, "The Verified Set\n");
		fwrite($fpout, $num_rows." proteins of E.coli K-12 with sequenced N-terminal amino acids\n");
		fwrite($fpout, "EG#\tGene\t#AA\t#AA cleaved\tPubmed ID\r\n");
	
		foreach ($result as $row) {	
	
			
			$fileegid = $row->eg_id;
			$filename = $row->name;
			$row2 = db_query("select tg.length, tppa.value
					 from t_product_protein tpp, t_product_protein_attributes tppa, t_gene tg
			 		where tpp.protein_id = tppa.protein_id and
              	  			 tppa.attribute_id = 1 and 
           	  	   			tpp.eg_id = '$fileegid' and
				   			tg.eg_id = tpp.eg_id")->fetch();
	
			
			
			$filelength = $row2->length;
			$fileaa_cleaved = $row2->value;
			
			fwrite($fpout,$fileegid."\t".$filename."\t".$filelength."\t".$fileaa_cleaved."\t".$row->pubmed."\r\n");
		}
   		fclose($fpout);
   		db_set_active('default');
   		startdownload($tmpfname);
   		
}
function ecogene_gene_rna_link($eg_id)
{
	$args = array();
	$args['eg_id'] = $eg_id;
	
	$content = '';
	db_set_active("ecogene");

	//go_function
	$result = db_query("SELECT accession_id, biodatabase_url FROM t_biodatabase_link left join t_biodatabase on t_biodatabase_link.biodatabase_id = t_biodatabase.biodatabase_link where eg_id = :eg_id and t_biodatabase.biodatabase_id='110' ",$args);
	$row_5 = $result->fetchAll();
	//go_process
	$result = db_query("SELECT accession_id, biodatabase_url FROM t_biodatabase_link left join t_biodatabase on t_biodatabase_link.biodatabase_id = t_biodatabase.biodatabase_link where eg_id = :eg_id and t_biodatabase.biodatabase_id='111' ",$args);
	$row_6 = $result->fetchAll();
	//go_component
	$result = db_query("SELECT accession_id, biodatabase_url FROM t_biodatabase_link left join t_biodatabase on t_biodatabase_link.biodatabase_id = t_biodatabase.biodatabase_link where eg_id = :eg_id and t_biodatabase.biodatabase_id='112' ",$args);
	$row_7 = $result->fetchAll();
	
	$result = db_query("SELECT 
							p.function as p_function, p.name as p_name 
						FROM 
							t_product_rna p 
						WHERE 
							p.eg_id = :eg_id", $args);
	$row_0 = $result->fetchAssoc();
		
	if($row_0)
	{
		$function_str = str_replace("|","; ", $row_0['p_function']);
		$function_str = ucfirst($function_str);
		if(!empty($function_str))
			$content .= "<H2> Function:  ". $function_str."</H2>";
	}	
		

	
	if($row_5)
	{
		$content .= "<br />GO Molecular Function:";
		foreach ($row_5 as $value) {
			$go_title = go_term_info($value->accession_id);
			$content .= "&nbsp;&nbsp;<a class=\"go_term_info\" href=\"".$value->biodatabase_url.$value->accession_id."\">".$value->accession_id."<span>".$go_title."</span></a> ";
			
		}
		
	}
	if($row_6)
	{
		$content .= "<br />GO Biological Process:";
		foreach ($row_6 as $value) {
			$go_title = go_term_info($value->accession_id);
			$content .= "&nbsp;&nbsp;<a class=\"go_term_info\" href=\"".$value->biodatabase_url.$value->accession_id."\">".$value->accession_id."<span>".$go_title."</span></a> ";
		}
		
	}
	if($row_7)
	{
		$content .= "<br />GO Cellular Component:";
		foreach ($row_7 as $value) {
			$go_title = go_term_info($value->accession_id);
			$content .= "&nbsp;&nbsp;<a class=\"go_term_info\" href=\"".$value->biodatabase_url.$value->accession_id."\">".$value->accession_id."<span>".$go_title."</span></a> ";
		}
		
	}	

	db_set_active("default");	
	return $content;
}
function go_term_info($go)
{
	db_set_active("ecogene");
	$result = db_query("SELECT 
							term.name, term_definition.term_definition
						FROM 
							term join term_definition on term.id=term_definition.term_id
						WHERE 
							term.acc = :go", array(':go'=>$go));
	$term = $result->fetchAssoc();
	$go_title = "Name: ".$term['name']."<br /><br />"."Definition: ".$term['term_definition'];
	db_set_active("default");	
	return $go_title;
}
function go_gene_number($go)
{
	db_set_active("ecogene");
		$result = db_query("SELECT count(*) as num FROM t_go_gene_link where accession_id='$go'");
		$row = $result->fetchObject();
		$number = $row->num;
		
	db_set_active("default");
	return $number;	
}

function inferred_go_terms($eg_id,$biodatabase_id)
{
		db_set_active("ecogene");
		$result = db_query("SELECT biodatabase_url FROM t_biodatabase where biodatabase_id=$biodatabase_id");
		$row = $result->fetchObject();
		$biodatabase_url = $row->biodatabase_url;
		
		$result = db_query("SELECT term.acc, term.id, tbl.eg_id 
					FROM t_biodatabase_link tbl JOIN term ON tbl.accession_id=term.acc 
					WHERE tbl.biodatabase_id=$biodatabase_id					
					AND tbl.eg_id= :eg_id", array(':eg_id'=>$eg_id));
					
		$output = "";
		$records = array();		
		$list_ancestor = array();
		while($row = $result->fetchAssoc())
		{

			$term_id = intval($row['id']);
			$term_acc = trim($row['acc']);
			
			$ancestors = term_ancestor_details($term_id);

			
			if(!empty($ancestors)){
				foreach ($ancestors as $ancestor) {
					$acc = $ancestor['acc'];
					if(!in_array($acc, $list_ancestor))
					{
						$list_ancestor[]=$acc;
						$records[$acc] =  array(
							'acc' =>  $ancestor['acc'],
							'name' =>  $ancestor['name'],
							'definition' =>  $ancestor['definition'],
							'go_genes' => go_gene_number($ancestor['acc']),
							'inferred' => $term_acc,
						
						);
					}else{
						$records[$acc]['inferred'] .=  '<br />'.$term_acc;
					}	
				}
				
			}
		}
		global $base_url;
		if(!empty($records))
		{
			$output = "<table><tr><th>Accession</th><th>Name</th><th>Definition</th><th>Inferred By</th></tr>";
			
			foreach ($records as $ancestor) {
					$output .= "<tr><td><a href=\"".$biodatabase_url.$ancestor['acc']."\">".$ancestor['acc']."</a> 
					<br /><a href='".$base_url."?q=ecosearch/gene/search&item_per_page=10&go=".$ancestor['acc']."'>
					".$ancestor['go_genes']." genes</a></td><td>".$ancestor['name']."</td>
					<td>".$ancestor['definition']."</td><td>".$ancestor['inferred']."</td></tr>";
			}
		}	
				
		if(!empty($output)){
			$output_id = "inferred_go_".$biodatabase_id;
			$output .= "</table>";
			$output = '
					<div>
						<a href="javascript:toggleBlockDiv(\''.$output_id.'\', \''.$output_id.'_CollapsibleMap\');">
						<img  id="'.$output_id.'_CollapsibleMap" class ="cl_CollapsibleArea_expanding" src="/sites/all/images/empty.gif" border="1">
						<span>Show less specific ancestors</span></a>
						
					</div>
					<div  id="'.$output_id.'" style="display: none; padding: 0px 20px;">'.$output.'</div>';
		
		}		
				
		return $output;
}
function term_ancestor_details($term_id)
{
	db_set_active("ecogene");		
	$terms = array();
	$ancestor_acc = term_ancestor($term_id);
	
	foreach ($ancestor_acc as $acc) {
		
		$result = db_query("SELECT 
							term.name AS name, term_definition.term_definition AS definition
						FROM 
							term join term_definition on term.id=term_definition.term_id
						WHERE 
							term.acc = :acc",
						array(':acc'=>$acc));
		

		if(!$result)	continue;
			 
		$term = $result->fetchAssoc();
		
		if (strcasecmp($acc, "GO:0003674") == 0) continue;
		if (strcasecmp($term['name'], "molecular_function4") == 0) continue;
		 
		
		$terms[] = array(
		'name'=>$term['name'],
		'definition'=>$term['definition'],
		'acc'=>$acc,
		);
		
	}
		
	
	return $terms;
	
	
}

function term_ancestor($term_id)
{
	$stack = array();     // stack for ancestors ids
	$ancestor_acc = array(); // all ancestors go accession number, e.g. go num
	
	array_push($stack,$term_id);
	db_set_active("ecogene");
	while(!empty($stack)) {
		
		$id = array_shift($stack);		
		
		$result = db_query("SELECT tt.term1_id, t.acc 
					FROM term2term tt JOIN term t on tt.term1_id=t.id  
					WHERE relationship_type_id=1 and term2_id=:term_id",array(':term_id'=>$id));	
			
		
		
		
		if(!$result){
		
			continue;
			 
		}else{
			
			
//			
			while ($row=$result->fetchAssoc()) {
								
				$parent_id = $row['term1_id'];
				$parent_acc = trim($row['acc']);
				array_push($stack,$parent_id);
				
//				var_dump($stack);
				if(strncmp($parent_acc, 'GO:', 3) ==0){
					array_push($ancestor_acc,$parent_acc);
					$ancestor_acc = array_unique($ancestor_acc);
				}
				
//				var_dump($ancestor_acc);
			}			
		}
	}
	
	return $ancestor_acc;
	
	
}
function ecogene_gene_protein_link($eg_id)
{
		$args = array();
	$args['eg_id'] = $eg_id;
	
	$content = '';
	db_set_active("ecogene");
		
		$EC_mouse_over = "The EC number is assigned by the Enzyme Commission. Additional information can be obtained from the ENZYME database.";
	$UniProtKB_mouse_over = "UniProtKB is a collaboration among the Swiss-Prot, PIR and NCBI protein databases.";
	$GenPept_mouse_over = "GenPept is a collection of protein translations derived from the corresponding genome RefSeq record.";
	$GenPept_GenBank_mouse_over = "GenPept-GenBank is a collection of protein translations derived from the corresponding genome GenBank record.";
	//uniprot
	$result = db_query("SELECT accession_id, biodatabase_url FROM t_biodatabase_link left join t_biodatabase on t_biodatabase_link.biodatabase_id = t_biodatabase.biodatabase_link where eg_id = :eg_id and t_biodatabase.biodatabase_id='81'",$args);
	$row = $result->fetchObject();
	//genbank-genepept
	$result = db_query("SELECT accession_id, biodatabase_url FROM t_biodatabase_link left join t_biodatabase on t_biodatabase_link.biodatabase_id = t_biodatabase.biodatabase_link where eg_id = :eg_id and t_biodatabase.biodatabase_id='72' ",$args);
	$row_2 = $result->fetchObject();
	//genpept (refseq)
	$result = db_query("SELECT accession_id, biodatabase_url FROM t_biodatabase_link left join t_biodatabase on t_biodatabase_link.biodatabase_id = t_biodatabase.biodatabase_link where eg_id = :eg_id and t_biodatabase.biodatabase_id='73' ",$args);
	$row_3 = $result->fetchObject();
	//ec number
	$result = db_query("SELECT accession_id, biodatabase_url FROM t_biodatabase_link left join t_biodatabase on t_biodatabase_link.biodatabase_id = t_biodatabase.biodatabase_link where eg_id = :eg_id and t_biodatabase.biodatabase_id='19' ",$args);
	$row_4 = $result->fetchAll();
	//go_function
	$result = db_query("SELECT accession_id, biodatabase_url FROM t_biodatabase_link left join t_biodatabase on t_biodatabase_link.biodatabase_id = t_biodatabase.biodatabase_link where eg_id = :eg_id and t_biodatabase.biodatabase_id='110' ",$args);
	$row_5 = $result->fetchAll();
	//go_process
	$result = db_query("SELECT accession_id, biodatabase_url FROM t_biodatabase_link left join t_biodatabase on t_biodatabase_link.biodatabase_id = t_biodatabase.biodatabase_link where eg_id = :eg_id and t_biodatabase.biodatabase_id='111' ",$args);
	$row_6 = $result->fetchAll();
	//go_component
	$result = db_query("SELECT accession_id, biodatabase_url FROM t_biodatabase_link left join t_biodatabase on t_biodatabase_link.biodatabase_id = t_biodatabase.biodatabase_link where eg_id = :eg_id and t_biodatabase.biodatabase_id='112' ",$args);
	$row_7 = $result->fetchAll();
	
	$result = db_query("SELECT 
							p.function as p_function, p.name as p_name 
						FROM 
							t_product_protein p 
						WHERE 
							p.eg_id = :eg_id", $args);
	$row_0 = $result->fetchAssoc();
		
	if($row_0)
	{
		$function_str = str_replace("|","; ", $row_0['p_function']);
		$function_str = ucfirst($function_str);
		if(!empty($function_str))
			$content .= "<H2> Function:  ". $function_str."</H2>";
	}	
	if($row || $row_2 || $row_3)
		$content .= "<br />Protein Databases: ";
	if($row)
	{
		$uniprot_acc = $row->accession_id;
		$content .= "UniProtKB <a title =\"$UniProtKB_mouse_over\" href=\"$row->biodatabase_url$row->accession_id\">$row->accession_id</a> ";
	}
		
	if($row_2)
	{
		$content .= " &nbsp;&nbsp;&nbsp;&nbsp;GenPept-RefSeq <a title =\"$GenPept_mouse_over\" href=\"$row_2->biodatabase_url$row_2->accession_id\">$row_2->accession_id</a> ";
	}
	if($row_3)
	{
		$content .= " &nbsp;&nbsp;&nbsp;&nbsp;GenPept-GenBank <a title =\"$GenPept_GenBank_mouse_over\" href=\"$row_3->biodatabase_url$row_3->accession_id\">$row_3->accession_id</a> ";
	}
	

	if($row_4)
	{
		$content .= "<br /><span style=\"border-bottom: thin dotted; color: #0070B3;\" title =\"$EC_mouse_over\">EC Number</span>:";
		foreach ($row_4 as $value) {
			$content .= "&nbsp;&nbsp;<a href=\"".$value->biodatabase_url.$value->accession_id."\">".$value->accession_id."</a> ";
		}
		
	}
	//kegg-motif
	$result = db_query("SELECT name, accession_id, biodatabase_url FROM t_biodatabase_link left join t_biodatabase on t_biodatabase_link.biodatabase_id = t_biodatabase.biodatabase_link where eg_id = :eg_id and t_biodatabase.biodatabase_id='115' ",$args);
	$row_21 = $result->fetchObject();
	//cd-search
	$result = db_query("SELECT name, accession_id, biodatabase_url FROM t_biodatabase_link left join t_biodatabase on t_biodatabase_link.biodatabase_id = t_biodatabase.biodatabase_link where eg_id = :eg_id and t_biodatabase.biodatabase_id='114' ",$args);
	$row_22 = $result->fetchObject();
	//pfam
	$result = db_query("SELECT name, accession_id, biodatabase_url FROM t_biodatabase_link left join t_biodatabase on t_biodatabase_link.biodatabase_id = t_biodatabase.biodatabase_link where eg_id = :eg_id and t_biodatabase.biodatabase_id='31' ",$args);
	$row_23 = $result->fetchAll();
	//interpro
	$result = db_query("SELECT name, accession_id, biodatabase_url FROM t_biodatabase_link left join t_biodatabase on t_biodatabase_link.biodatabase_id = t_biodatabase.biodatabase_link where eg_id = :eg_id and t_biodatabase.biodatabase_id='129' ",$args);
	$row_24 = $result->fetchObject();
	
	//TIGRFam & TIGR

	$result = db_query("SELECT name, accession_id, biodatabase_url FROM t_biodatabase_link left join t_biodatabase on t_biodatabase_link.biodatabase_id = t_biodatabase.biodatabase_link where eg_id = :eg_id and ( t_biodatabase.biodatabase_id='56' )",$args);
	$row_255 = $result->fetchAll();
	
	$result = db_query("SELECT name, accession_id, biodatabase_url FROM t_biodatabase_link left join t_biodatabase on t_biodatabase_link.biodatabase_id = t_biodatabase.biodatabase_link where eg_id = :eg_id and ( t_biodatabase.biodatabase_id='52' or t_biodatabase.biodatabase_id='53' or t_biodatabase.biodatabase_id='54' or t_biodatabase.biodatabase_id='55' )",$args);
	$row_25 = $result->fetchAll();
	//ecogene3d
	$result = db_query("SELECT name, accession_id, biodatabase_url FROM t_biodatabase_link left join t_biodatabase on t_biodatabase_link.biodatabase_id = t_biodatabase.biodatabase_link where eg_id = :eg_id and t_biodatabase.biodatabase_id='131' ",$args);
	$row_26 = $result->fetchAll();
	//ecogene3d
	$result = db_query("SELECT name, accession_id, biodatabase_url FROM t_biodatabase_link left join t_biodatabase on t_biodatabase_link.biodatabase_id = t_biodatabase.biodatabase_link where eg_id = :eg_id and t_biodatabase.biodatabase_id='132' ",$args);
	$row_27 = $result->fetchAll();
	//ecogene3d
	$result = db_query("SELECT name, accession_id, biodatabase_url FROM t_biodatabase_link left join t_biodatabase on t_biodatabase_link.biodatabase_id = t_biodatabase.biodatabase_link where eg_id = :eg_id and t_biodatabase.biodatabase_id='133' ",$args);
	$row_28 = $result->fetchAll();
	
	
	if($row_21 || $row_22 || $row_23 || $row_24)
		$content .= "<br />Motif Databases: ";
	if($row_21)
	{
		$content .= "$row_21->name <a title =\"\" href=\"$row_21->biodatabase_url$row_21->accession_id\">$row_21->accession_id</a> ";
	}
		
	if($row_22)
	{
		$content .= " &nbsp;&nbsp;&nbsp;&nbsp;$row_22->name <a title =\"\" href=\"$row_22->biodatabase_url$row_22->accession_id\">$row_22->accession_id</a> ";
	}
	if($row_23)
	{
		$content .= " &nbsp;&nbsp;&nbsp;&nbsp;Pfam ";
		$content_pham = array();
		foreach ($row_23 as $value) {
			$content_pham []= "<a href=\"".$value->biodatabase_url.$value->accession_id."\">".$value->accession_id."</a> ";
		}		
		if (!empty($uniprot_acc))
		{
			$content .= "&nbsp;&nbsp;<a href=\"http://pfam.sanger.ac.uk/protein/".$uniprot_acc."\">".$uniprot_acc."</a> (".implode(" ",$content_pham).")";
		}
	}
	if($row_24)
	{
		$content .= " &nbsp;&nbsp;&nbsp;&nbsp;$row_24->name <a title =\"\" href=\"$row_24->biodatabase_url$row_24->accession_id\">$row_24->accession_id</a> ";
	}	
	if (!empty($uniprot_acc))
	{
			$content .= "&nbsp;&nbsp;&nbsp;&nbsp;SMART&nbsp;&nbsp;<a href=\"http://smart.embl.de/smart/show_motifs.pl?ID=".$uniprot_acc."\">".$uniprot_acc."</a> ";
	}
	if (!empty($uniprot_acc))
	{
			$content .= "&nbsp;&nbsp;&nbsp;&nbsp;PROSITE&nbsp;&nbsp;<a href=\"http://prosite.expasy.org/cgi-bin/prosite/PSScan.cgi?output=nice&seq=".$uniprot_acc."\">".$uniprot_acc."</a> ";
	}
	
	if (!empty($row_255)){
		$content .= " &nbsp;&nbsp;&nbsp;&nbsp;TIGR ";
		foreach ($row_255 as $value) {
				$content .= "&nbsp;&nbsp;<a href=\"".$value->biodatabase_url.$value->accession_id."\">".$value->accession_id."</a> ";
			}
	}
	if($row_25)
	{
		
		$content_p = array();
		foreach ($row_25 as $value) {
			$content_p []= "<a href=\"".$value->biodatabase_url.$value->accession_id."\">".$value->accession_id."</a> ";
		}				
		if (!empty($row_255))
		{			
			$content .= " (".implode("  ",$content_p).")";
		}
	}
	if($row_26)
	{
		$content .= " &nbsp;&nbsp;&nbsp;&nbsp;HAMAP ";
		$content_p = array();
		foreach ($row_26 as $value) {
			$content_p []= "<a href=\"".$value->biodatabase_url.$value->accession_id."\">".$value->accession_id."</a> ";
		}		
		if (!empty($uniprot_acc))
		{
			$content .= implode("  ",$content_p);
		}
	}
	if($row_27)
	{
		$content .= " &nbsp;&nbsp;&nbsp;&nbsp;PRINTS ";
		$content_p = array();
		foreach ($row_27 as $value) {
			$content_p []= "<a href=\"".$value->biodatabase_url.$value->accession_id."\">".$value->accession_id."</a> ";
		}		
		if (!empty($uniprot_acc))
		{
			$content .= implode("  ",$content_p);
		}
	}
	if($row_28)
	{
		$content .= " &nbsp;&nbsp;&nbsp;&nbsp;SUPFAM ";
		$content_p = array();
		foreach ($row_28 as $value) {
			$content_p []= "<a href=\"".$value->biodatabase_url.$value->accession_id."\">".$value->accession_id."</a> ";
		}		
		if (!empty($uniprot_acc))
		{
			$content .= implode("  ",$content_p);
		}
	}
	
	global $base_url;
	if($row_5 || $row_6 || $row_7)
	{
		$content .= "<H3> Gene Ontology (GO):  </H3>";
		$content .= "<div id= 'gene_ontology' style='margin-left: 2em'>";		
	}
	if($row_5)
	{
		$content .= "GO Molecular Function:";
		foreach ($row_5 as $value) {
			$go_title = go_term_info($value->accession_id);
			$content .= "&nbsp;&nbsp;<a class=\"go_term_info\" href=\"".$value->biodatabase_url.$value->accession_id."\">".$value->accession_id."<span>".$go_title."</span></a>  
			<a href='".$base_url."?q=ecosearch/gene/search&item_per_page=10&go=".$value->accession_id."'>(".go_gene_number($value->accession_id)." genes)</a>";

		}
		$content .= inferred_go_terms($eg_id,110);
		
	}
	if($row_6)
	{
		$content .= "<br />GO Biological Process:";
		foreach ($row_6 as $value) {
			$go_title = go_term_info($value->accession_id);
			$content .= "&nbsp;&nbsp;<a class=\"go_term_info\"  href=\"".$value->biodatabase_url.$value->accession_id."\">".$value->accession_id."<span>".$go_title."</span></a>  
			<a href='".$base_url."?q=ecosearch/gene/search&item_per_page=10&go=".$value->accession_id."'>(".go_gene_number($value->accession_id)." genes)</a>";
			
		}
		$content .= inferred_go_terms($eg_id,111);
	}
	if($row_7)
	{
		$content .= "<br />GO Cellular Component:";
		foreach ($row_7 as $value) {
			$go_title = go_term_info($value->accession_id);
			$content .= "&nbsp;&nbsp;<a class=\"go_term_info\" href=\"".$value->biodatabase_url.$value->accession_id."\">".$value->accession_id."<span>".$go_title."</span></a> 
			<a href='".$base_url."?q=ecosearch/gene/search&item_per_page=10&go=".$value->accession_id."'>(".go_gene_number($value->accession_id)." genes)</a>";
			
		}
		$content .= inferred_go_terms($eg_id,112);
	}
	// QickGo display
	db_set_active("ecogene");	
	$query = db_query("SELECT * FROM t_biodatabase Where biodatabase_id = 92 and display = 1 and biodatabase_url <> ''");
	if ($row = $query->fetchAssoc()) {
						
			$name = $row["name"];
			$url = $row["biodatabase_url"];
			$biodatabase_link = $row["biodatabase_link"];
			
			$query = db_query("SELECT accession_id FROM t_biodatabase_link where eg_id = '$eg_id' and biodatabase_id = '$biodatabase_link' ");
			$row = $query->fetchAll();
			if ($row)	{				
					$content .= "<br />$name:";
					foreach ($row as $value) { 
						$content .= " <a href=\"".$url.$value->accession_id."\">".$value->accession_id ."</a>";
					}
			}
			
	}
	if($row_5 || $row_6 || $row_7)
	{
		$content .= "</div>";
	}

	db_set_active("default");	
	return $content;
}
function go_term_genes($id)
{
		$result = db_query("SELECT 
							g.eg_id,g.name,g.description,p.sequence 
						FROM 
							t_gene g, t_product_protein p 
						WHERE 
							g.eg_id = p.eg_id and g.eg_id = :eg_id", $args);
	
	$row = $result->fetchAssoc();
}
function ecogene_gene_protein_verified($eg_id)
{
	global $base_url;
	$args = array();
	$args['eg_id'] = $eg_id;
	
	$content = '';
	db_set_active("ecogene");
	$result = db_query("SELECT count(*) as num FROM t_gene_reference_link WHERE eg_id = :eg_id and verified=1", $args);
	$row = $result->fetchAssoc();	
	if($row['num']>0)
	{
		$result = db_query("select tg.name, tg.length, tppa.value  from t_product_protein tpp, t_product_protein_attributes tppa, t_gene tg where tpp.protein_id = tppa.protein_id and  tppa.attribute_id = 1 and tpp.eg_id = :eg_id and tg.eg_id = tpp.eg_id", $args);
		$row = $result->fetchAssoc();

		$length = $row['length'];
		$aa_cleaved = $row['value'];
		$name = $row['name'];
		$content .= "<H2> Verified Info:  </H2> <div id= 'prot_verified'>
    <p> 
	This protein has been Verified by N-terminal protein sequencing, 
	as reported in the publications and/or personal communications listed below.
	 $aa_cleaved N-terminal amino ";

		if($aa_cleaved < 2) {
			$content .=" acid is cleaved by methionine aminopeptidase ";
		} elseif ($aa_cleaved > 10) {
			$content .= " acids are cleaved by signal peptidase I ";
		} else {
			$content .= " acids are cleaved";
		}
		$content .=	'from the  '.$length.' aa '.ucwords($name).
		' protein to form the mature '.
		($length-$aa_cleaved).' aa '.ucwords($name).' protein.';
	$content .=' <br />
			<strong>A complete listing of the <a href="'.$base_url.'?q=verified_set">Verified Set</a> is available for downloading. </strong>
		</p>	
	';
	}
	// List the references
	$query = "SELECT ".
			"r.title, ".
			"date_format(r.date_published,'%Y') year, ".
			"r.pubmed_id, r.medline_id, pagination, journal, volume, ".
		 	"concat(a.last_name,' ',a.initials) as author_name, r.xml_link ".
		 "FROM ".
			"t_reference r,t_gene_reference_link grl, ".
			"t_reference_author_link ral, t_author a ".
		 "WHERE ".
			"grl.eg_id = :eg_id and ".
			"grl.pubmed_id = r.pubmed_id and ".
			"r.pubmed_id = ral.pubmed_id and ".
			"ral.author_id = a.author_id and ".
			"grl.verified = 1 ".
		"ORDER BY ".
			"date_published asc, pubmed_id desc,title asc,ral.seq_no asc ";
	
			
	$result = db_query($query,array(':eg_id'=>$eg_id));
	$row_previous = $result->fetchAssoc();
 	
		while ($row = $result->fetchAssoc())
		{	
		
			if ($row_previous["pubmed_id"] == $row["pubmed_id"]) {
				$row["author_name"] = $row_previous["author_name"]. ", ".$row["author_name"];
			} else 
			{
				$list = $row_previous["author_name"]. " (".$row_previous['year'].") ".$row_previous["title"]." ".$row_previous["journal"]." ".$row_previous["volume"].":".$row_previous["pagination"];
				if ($row_previous["xml_link"]=='Y') {
					$list = "<A HREF='http://www.ncbi.nlm.nih.gov/entrez/query.fcgi?cmd=Retrieve&db=PubMed&list_uids=".$row_previous["pubmed_id"]."&dopt=Abstract&target='v'>".$list."</A>";
				}
				$lists[]=$list;
			}
			$row_previous = $row;
		}
		if($row_previous)
 		{
			$list = $row_previous["author_name"]. " (".$row_previous['year'].") ".$row_previous["title"]." ".$row_previous["journal"]." ".$row_previous["volume"].":".$row_previous["pagination"];
			if ($row_previous["xml_link"]=='Y') {
					$list = "<A HREF='http://www.ncbi.nlm.nih.gov/entrez/query.fcgi?cmd=Retrieve&db=PubMed&list_uids=".$row_previous["pubmed_id"]."&dopt=Abstract&target='v'>".$list."</A>";
			}
				$lists[]=$list; 
		}
	$variables = array('items'=>$lists, 'title'=>'', 'type'=>'ol', 'attributes' => array());	
	$content .= theme_item_list($variables);
	
	$content .= '</div>';
	db_set_active("default");	
	return $content;
}
function ecogene_gene_protein_sequence($eg_id) {
	global $base_url;
	$args = array();
	$args[':eg_id'] = $eg_id;
	
	
	db_set_active("ecogene");
	$result = db_query("SELECT 
							special_title 
						FROM 
							t_gene_special_link 
						WHERE 							
							special_title LIKE 'IMemProt%' and eg_id='$eg_id'");
	
//	special_id <=738 and eg_id='$eg_id'");
	
	$row = $result->fetchAssoc();	
	if($row){
		$content .= '<H2> Inner Membrane Protein:  ';
		$content .= "<a href=\"$base_url/?q=topic/1\" >". $row["special_title"]."</a></H2>";
	}
	$result = db_query("SELECT 
							g.eg_id,g.name,g.description,p.sequence 
						FROM 
							t_gene g, t_product_protein p 
						WHERE 
							g.eg_id = p.eg_id and g.eg_id = :eg_id", $args);
	
	$row = $result->fetchAssoc();	
	$content .= '<a name="prot_seq"></a>';
	if($row){
		$content .= '<H2> Protein Sequence:  </H2>';
		$description = format_description($row["description"]);	
		$name = ucfirst($row["name"]);	
		$content .= '<pre id="protein_seq">>'.$row["eg_id"].' '.$name.' '.$description;
		$sequence = $row["sequence"];
	
		for ($i=0;$i<strlen($sequence);$i++) {
			if (($i % SEQ_LINE)==0) {
				$content=$content. '<br />';
			}
		$content=$content. $sequence[$i];			
		}
	
	 	$content=$content."</pre>";
	}else{
		$content .="<em>Protein Sequence does not exist.</em>";
	}
	db_set_active("default");
	
	return $content;
}
function ecogene_gene_rna_sequence($eg_id) {
	
	$args = array();
	$args[':eg_id'] = $eg_id;
	
	db_set_active("ecogene");
	$result = db_query("SELECT g.type FROM	t_gene g WHERE g.eg_id = :eg_id", $args);
	$row = $result->fetchAssoc();	
	
	if( $row['type'] != "nt" )
	{
		$content ="<em>RNA Sequence does not exist.</em>";
		 db_set_active("default");
		return $content;
	}
	
	$query = seq_format($eg_id);	
	
	$query=db_query($query);
	$row = $query->fetchAssoc();
	
	if($row){
		$content = '<H2> RNA Sequence:  </H2>';
	$description = format_description($row["description"]);	
	$name = ucfirst($row["name"]);	
	$content .= '<pre id="rna_seq">>'.$row["eg_id"].' '.$name.' '.$description;
	$sequence = $row["sequence"];
	$sequence = str_replace("T", "U", $sequence);
	
	for ($i=0;$i<strlen($sequence);$i++) {
		if (($i % SEQ_LINE)==0) {
			$content=$content. '<br />';
		}
		$content=$content. $sequence[$i];			
	}
	
	 $content=$content."</pre>";
	}else{
		$content .="<em>RNA Sequence does not exist.</em>";
	}
	 
	 db_set_active("default");
	
	return $content;
}

function ecogene_gene_structure($eg_id) {
	
	set_protein_title($eg_id);
	
	$args = array();
	$args['eg_id'] = $eg_id;
	
	
	db_set_active("ecogene");
	
	$content='';
	
	// => CATH
	$result = db_query("SELECT name FROM t_biodatabase where t_biodatabase.biodatabase_id='130'");
	$row = $result->fetchAssoc();
	if($row)
	{
		$db_name = $row['name'];		
	}
	$result = db_query("SELECT name, accession_id, biodatabase_url FROM t_biodatabase_link left join t_biodatabase on t_biodatabase_link.biodatabase_id = t_biodatabase.biodatabase_link where eg_id = :eg_id and t_biodatabase.biodatabase_id='130' ",$args);
	$row = $result->fetchAll();
	if($row)
	{
		
		$content_p = array();
		foreach ($row as $value) {
			$content_p []= "<a href=\"".$value->biodatabase_url.$value->accession_id."\">".$value->accession_id."</a> ";
		}		
		if (!empty($content_p))
		{
			$content .= " &nbsp;&nbsp;&nbsp;&nbsp;$db_name ";
			$content .= implode("  ",$content_p);
		}
	}
	
	// => QUARK
	$result = db_query("SELECT name FROM t_biodatabase where t_biodatabase.biodatabase_id='134'");
	$row = $result->fetchAssoc();
	if($row)
	{
		$db_name = $row['name'];		
	}
	
	$result = db_query("SELECT name, accession_id, biodatabase_url FROM t_biodatabase_link left join t_biodatabase on t_biodatabase_link.biodatabase_id = t_biodatabase.biodatabase_link where eg_id = :eg_id and (t_biodatabase.biodatabase_id='134' or t_biodatabase.biodatabase_id='135')",$args);
	$row = $result->fetchAll();
	if($row)
	{
		
		$content_p = array();
		foreach ($row as $value) {
			$content_p []= "<a href=\"".$value->biodatabase_url.$value->accession_id."\">".$value->accession_id."</a> ";
		}		
		if (!empty($content_p))
		{
			$content .= " &nbsp;&nbsp;&nbsp;&nbsp;$db_name ";
			$content .= implode("  ",$content_p);
		}
	}
	$content = $content."<br />";
	
	$result = db_query("select count(ga.accession_id) as num FROM t_biodatabase_link ga  WHERE ga.eg_id = :eg_id and ga.biodatabase_id = '7'",$args);
//	$result_PDBe = db_query("select count(ga.accession_id) as num FROM t_biodatabase_link ga  WHERE ga.eg_id = :eg_id and ga.biodatabase_id = '7'",$args);
//	$result_PDBj = db_query("select count(ga.accession_id) as num FROM t_biodatabase_link ga  WHERE ga.eg_id = :eg_id and ga.biodatabase_id = '7'",$args);
	if($row = $result->fetchAssoc())
		$num = $row['num'];

	if($num>0)	
	{
		$result = db_query("select biodatabase_url FROM t_biodatabase  WHERE biodatabase_id = '7'");	
		if($row = $result->fetchAssoc())
			$PDBLink = $row['biodatabase_url'];
		
		$PDBLink_e = db_select('t_biodatabase','t_biodatabase')
    						->fields('t_biodatabase',array('biodatabase_url'))
    						->condition('t_biodatabase.biodatabase_id', 93, '=')
    						->execute()->fetchField();
   							
		$PDBLink_j = db_select('t_biodatabase','t_biodatabase')
    						->fields('t_biodatabase',array('biodatabase_url'))
    						->condition('t_biodatabase.biodatabase_id', 94, '=')
    						->execute()->fetchField();
    						
    	$Jmol = db_select('t_biodatabase','t_biodatabase')
    						->fields('t_biodatabase',array('biodatabase_url'))
    						->condition('t_biodatabase.biodatabase_id', 126, '=')
    						->execute()->fetchField();
    						
    	$Proteopedia = db_select('t_biodatabase','t_biodatabase')
    						->fields('t_biodatabase',array('biodatabase_url'))
    						->condition('t_biodatabase.biodatabase_id', 125, '=')
    						->execute()->fetchField();
        							
		$content .= '<table align="left" width="800" bgcolor="white">';	
		$n_structure = db_query("select count(ga.accession_id) as count FROM t_biodatabase_link ga  WHERE ga.eg_id = :eg_id and ga.biodatabase_id = '7'",$args)
		->fetchField();
		if(!empty($n_structure))  $content .= $n_structure>1? "<caption>$n_structure structures</caption>" :"<caption>$n_structure structure</caption>";

		$result = db_query("select ga.accession_id FROM t_biodatabase_link ga  WHERE ga.eg_id = :eg_id and ga.biodatabase_id = '7' ORDER BY ga.accession_id DESC",$args);
		$i=0;
		while($row = $result->fetchObject())
		{
			$i = $i+1;
			if($i%5==1)		
				$content .= "<tr valign=\"middle\">";
	
			$content .= '<td width = "20%" style ="text-align: center;">
				<a href="'. $PDBLink.$row->accession_id.'"> 
				<img src="http://www.pdb.org/pdb/images/'.($row->accession_id).'_bio_r_500.jpg" height="130" width="130" alt="'.$PDBLink.$row->accession_id.'"></a>
				<br />&nbsp;&nbsp;<a href="'. $PDBLink.$row->accession_id.'">'.$row->accession_id.'</a>
				&nbsp;&nbsp;<a href="'. $PDBLink_e.$row->accession_id.'">PDBe</a>
				&nbsp;&nbsp;<a href="'. $PDBLink_j.$row->accession_id.'">PDBj</a>
				&nbsp;&nbsp;<a href="'. $Jmol.$row->accession_id.'">Jmol</a>
				&nbsp;&nbsp;<a href="'. $Proteopedia.strtolower($row->accession_id).'">Proteopedia</a>
				</td> ';
			if($i%5==0){		
				$content .= "</tr>";
			}
			
		}
		$content .= "</table><br>";
	
	}else {
	
		$result = db_query("SELECT g.name FROM	t_gene g WHERE g.eg_id = :eg_id", $args);
		$row = $result->fetchAssoc();
		$content .= "No PDB structures are available for ".ucfirst($row['name']) ." from <i> E. coli</i> K-12.";	
       }
		
	db_set_active("default");	
	return $content;
}
