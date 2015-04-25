<?PHP

function ecogene_gene_array($eg_id) {
	global $base_url;
	$numofresults=1000;
	db_set_active("ecogene");

	$header = array(
//	array('data' => t('Reference'), 'field' => 'reference.date_published', 'sort' => 'desc', 'field' => 'reference.pubmed_id','sort' => 'asc'),
	array('data' => t('Reference'), 'field' => 'reference.date_published'),
	array('data' => t('Variable')),
	array('data' => t('Value'), 'field' => 'abs_fold_value','sort' => 'desc'),
	array('data' => t('Experiments'), 'field' => 'exp.exp_type'),
	array('data' => t('Contact'), 'field' => 'array.contact_lastname')
	);
	
//	$sql_select ="select t_array_data.value, t_array_data.validated, t_array_data.form, t_array_exp.*, 
//	t_author.last_name, t_array.contact_fullname, t_array.contact_email, year(t_reference.date_published) as year_published ";
//
//	$sql_join = " FROM t_array_data left join t_array_exp on t_array_data.pubmed_id=t_array_exp.pubmed_id 
//	and t_array_data.exp_id= t_array_exp.exp_id 
//	left join t_array on t_array_data.pubmed_id=t_array.pubmed_id 
//	left join t_reference on t_array_data.pubmed_id=t_reference.pubmed_id 
//	left join	t_reference_author_link	on t_reference_author_link.pubmed_id=t_reference.pubmed_id left 
//	join t_author on t_author.author_id = t_reference_author_link.author_id ";


	$query = db_select('t_array_data','data')-> extend('TableSort') ;
	
	$query->leftjoin('t_array_exp', 'exp', 	'data.pubmed_id = exp.pubmed_id and data.exp_id= exp.exp_id');
	
	$query->leftjoin('t_array', 	'array','data.pubmed_id = array.pubmed_id');
	
	$query->leftjoin('t_reference', 'reference','data.pubmed_id = reference.pubmed_id');
	
	$query->leftjoin('t_reference_author_link', 't_reference_author_link', 'reference.pubmed_id = t_reference_author_link.pubmed_id');
	
	$query->leftjoin('t_author', 'author', 	'author.author_id = t_reference_author_link.author_id');
	
	$query->fields('data', array('fold_value', 'validated', 'form'));
	$query->fields('exp');
	$query->fields('array', array('contact_fullname','contact_email'));
	$query->fields('author', array('last_name'));
	$query->addExpression('year(reference.date_published)', 'year_published');
	$query->addExpression('abs(data.fold_value)', 'abs_fold_value');
	
	$arguments = array(':eg_id'=>$eg_id, ':seq_no'=>1);
	$query->where('t_reference_author_link.seq_no = :seq_no AND data.eg_id = :eg_id', $arguments);
	$query->orderByHeader($header);
//	
//	$query->condition('t_reference_author_link.seq_no', 1);
//	$query->condition('data.eg_id', $eg_id);
//	$query->execute();
//	print_r($query->__toString());
//	$count_query = $query->countQuery();
//	echo $num_rows = $query->countQuery()->execute()->fetchField();
//	
	$record  = $query->execute()->fetchAll();
//	print_r($record);
	
//	$record = $query->fetchObject()->execute();
//	echo $record->last_name;

				
	
	$output = "";
	foreach  ($record as $item) {		
//		$value = round(transToRatio($item->form,$item->value),3);
		$value = $item->fold_value;
		
   		$rows[] = array(
		("<a href='http://www.ncbi.nlm.nih.gov/entrez/query.fcgi?cmd=Retrieve&db=PubMed&list_uids=$item->pubmed_id&dopt=Abstract'>".$item->last_name.",".$item->year_published."</a>"),
		(displayed_name($item)),
		($item->validated!=""? $value."<br><b>validated</b>": $value),
		("<a href='#$item->pubmed_id$item->exp_type'>".$item->exp_type." : ".$item->exp_id."</a>&nbsp;&nbsp;&nbsp;".
		" <a href=\"$base_url/?q=ecoarray/experiment/$item->pubmed_id/$item->exp_id\">Exp Data</a>"),
		("<a href='mailto:$item->contact_email'>$item->contact_fullname</a>"),		
		);
	}
	
	
	$output.= array_gene_title($eg_id);
	db_set_active("default");
	$variables = array('header'=>$header, 'rows'=>$rows, 'sticky'=>NULL, 'empty'=>NULL, 'caption'=>'', 'colgroups'=>array(), 'attributes'=>array('align' => 'left', 'width' => '100%'));
	
	$output.=theme_table($variables);

	db_set_active("ecogene");

	$output = $output.detail_exp($record);
	
	db_set_active("default");
	return $output;
}
function array_gene_title($eg_id) {

//	db_set_active("ecogene");
global $base_url;
	$query = db_query("SELECT name FROM {t_gene} WHERE eg_id=:eg_id",array('eg_id'=>$eg_id));	
	if($row = $query->fetchAssoc())
		$title = "<center><h2>Microarray Experiment Search Results for <A HREF=\"$base_url?q=gene/$eg_id\">".$row['name']."</A> ";
	$query_count = db_query("select count(*) as num from t_array_data where eg_id=:eg_id",array('eg_id'=>$eg_id));	
	if($row = $query_count->fetchAssoc())
		$title .= " (".$row['num']." total)</center></h2>";
//	db_set_active("default");
  	return $title;	
}
function detail_exp(&$result)
{
	global $base_url;

	$header_chemical = array(
	array('data' => t('Exp ID'), 'width' => t('10%')),
	array('data' => t('Exp Data'), 'width' => t('10%')),
	array('data' => t('Name'), 'width' => t('10%')),
	array('data' => t('Concentration'), 'width' => t('10%')),
	array('data' => t('Time added'), 'width' => t('10%')),
	array('data' => t('Duration'), 'width' => t('10%')),
	array('data' => t('Comments'), 'width' => t('40%'))
	);
	$header_growth = array(
	array('data' => t('Exp ID'), 'width' => t('10%')),
	array('data' => t('Exp Data'), 'width' => t('10%')),
	array('data' => t('Name'), 'width' => t('15%')),
	array('data' => t('Variable'), 'width' => t('15%')),
	array('data' => t('Shift time'), 'width' => t('10%')),
	array('data' => t('Duration'), 'width' => t('10%')),
	array('data' => t('Comments'), 'width' => t('40%'))
	);
	$header_gene = array(
	array('data' => t('Exp ID'), 'width' => t('10%')),
	array('data' => t('Exp Data'), 'width' => t('10%')),
	array('data' => t('Gene Name'), 'width' => t('15%')),
	array('data' => t('Allele'), 'width' => t('25%')),
	array('data' => t('Comments'), 'width' => t('40%'))
	);
	$output="";
	
	$exp_type_last='';
	$exp_id_last='';
	$pubmed_id_last='';
	$rows = array();
	
	$variables = array('sticky'=>NULL, 'empty'=>NULL,  'colgroups'=>array(), 'attributes'=>array('id' => 'itsthetable', 'align' => 'left', 'width' => '100%'));
	
	
	foreach ( $result as $item ) {	
		$exp_type = $item->exp_type;
		$pubmed_id = $item->pubmed_id;

		if((($exp_type != $exp_type_last)||($pubmed_id != $pubmed_id_last))&&$pubmed_id_last!=="")
		{
			$caption = $exp_type_last." experimental details: ";
			$output .= "<br><A  name=\"".$pubmed_id_last.$exp_type_last."\"></A>";
			
			$variables['rows'] = $rows;
			$variables['caption'] = $caption;
			if( $exp_type_last=='chemical')
			{
				$variables['header'] = $header_chemical;				
				
			}
			if( $exp_type_last=='growth')
			{
				$variables['header'] = $header_growth;
//				$output.=theme('table', $header_growth, $rows,array('id' => 'itsthetable', 'align' => 'left', 'width' => '100%'),$caption);
			}
			if( $exp_type_last=='gene')
			{
				$variables['header'] = $header_gene;
//				$output.=theme('table', $header_gene, $rows,array('id' => 'itsthetable', 'align' => 'left', 'width' => '100%'),$caption);
			}
			
			$output.=theme_table($variables);
			$rows = array();
		}
		if($pubmed_id != $pubmed_id_last)
		{
			$output .= "<center><b><br><a href='http://www.ncbi.nlm.nih.gov/sites/entrez?cmd=Retrieve&db=PubMed&list_uids=".$item->pubmed_id."'>".$item->last_name.",".$item->year_published."</a> 	";
			$output .= "<a href=\"$base_url?q=ecoarray/paper/".$item->pubmed_id."\"> All Experiments</a> 	";
			$output .= "<A  HREF='#main'> Top</A></b> </center>";
		}
		
		if( $exp_type=='chemical')
		{
			$rows[] = array(($item->exp_id),('Show Data'),	(displayed_name($item)), ($item->concentration), ($item->method_added),($item->duration),($item->comments));
		}
		if( $exp_type=='growth')
		{
			$rows[] = array(($item->exp_id),('Show Data'),	(displayed_name($item)), ($item->variable), ($item->shift_time),($item->duration),($item->comments));
		}
		if( $exp_type=='gene')
		{
			$allele= ($item->allele!=""? ($item->mutation_kind!=""? $item->allele."(".$item->mutation_kind.")" : $item->allele ) : $item->mutation_kind);
			$rows[] = array(($item->exp_id),('Show Data'),	(displayed_name($item)), ($allele), ($item->comments));
		}	
		
		$exp_type_last = $exp_type;
		$pubmed_id_last = $pubmed_id;
	}
	if($pubmed_id_last!=="")
	{
		$caption = $exp_type_last." experimental details: ";
		$output .= "<br><A  name=\"".$pubmed_id_last.$exp_type_last."\"></A>";

		$variables['rows'] = $rows;
		$variables['caption'] = $caption;
			
		if( $exp_type_last=='chemical')
		{
			$variables['header'] = $header_chemical;
//			$output.=theme('table', $header_chemical, $rows,array('id' => 'itsthetable', 'align' => 'left', 'width' => '100%'),$caption);
		}
		if( $exp_type_last=='growth')
		{
			$variables['header'] = $header_growth;
//			$output.=theme('table', $header_growth, $rows,array('id' => 'itsthetable', 'align' => 'left', 'width' => '100%'),$caption);
		}
		if( $exp_type_last=='gene')
		{
			$variables['header'] = $header_gene;
//			$output.=theme('table', $header_gene, $rows,array('id' => 'itsthetable', 'align' => 'left', 'width' => '100%'),$caption);
		}
		$output.=theme_table($variables);
		
	}
	return $output;
}
function displayed_name($item)
{
	global $base_url;
	if($item->exp_type=='chemical')
	{
		return $item->chemical_name;
	}elseif ($item->exp_type=='growth')
	{
		return $item->basal." vs ".$item->variable;
	}else
	{
		$output="";
		$names = trim($item->gene_name);
		$regs=preg_split("/[.,; ]+/",$names);
		for($index=0;$index<count($regs);$index++)
		{
			$gene_name = $regs[$index];
			$output .= t("<A HREF=\"$base_url?q=ecosearch/gene/search&name=$gene_name\">$gene_name</A> ");
			if($index!=count($regs)-1)
				$output .= "; ";
		}
		return $output;
	}
}
//from ratio to the others forms
// 1. fold 2. ratio, 3. log10, 4. log2, 5. ratio
//
function transToForm($form,$value)
{
	switch ($form) 
	{
	case 1: // fold
	    if ( $value > 1)
	        return $value;
	    else
	        return 0 -(1 / $value);
	    break;
	case 2:
	    return $value;
	case 3:
	    return log($value);
	    break;
	case 4:
	    return log($value)/log(2);
	    break;
	case 5:
	    return $value;
	    break;    
	} 
}

function transToRatio($form,$value)
{
	switch ($form) 
	{
	case 1: // fold
	    if ( $value > 0)
	        return $value;
	    else if ( $value < 0)
	        return 0 -(1 / $value);
	    break;
	case 2:
	    return $value;
	case 3:
	    return exp($value);
	    break;
	case 4:
	    return pow(2,$value);
	    break;
	case 5:
	    return $value;
	    break;    
	} 
}