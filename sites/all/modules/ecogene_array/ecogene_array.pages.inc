<?PHP
include("sites/all/include/ecoFunction.php");
include("sites/all/modules/ecogene_gene/ecogene_gene.array.inc");
define ("SEQ_LINE", 75);


function get_sub_query(&$query_sub,&$params_sub,$value,$Table,$Field,$type_string)
{
	$query_sub = array();
    $params_sub = array();
    		
	$regs=preg_split("/[.,; ]+/",$value);
	foreach ($regs as $value_sub) {
		if(trim($value_sub)!=="")
		$query_sub[] = $Table .'.'.$Field .$type_string;
		$params_sub[] = trim($value_sub);

	}
}

function search_experiment_conditions($array = array(),&$query)
{
	$refTable='r';
	$authorTable = 'author';
	$expTable = 'exp';
	$arrayTable = 't_array';
	if(empty($array))	return;
	
    		
	foreach ($array as $key => $value) {
		
		if($value!='')
    	{	$query_sub = array();
    		$params_sub = array();
    
    		switch ($key)
    		{
    			case 'main_search':
    				$params_sub = preg_split("/[,; ]+/",$value);
    				
    				if(!empty($params_sub))
    				{
    					$subquery = db_and();
    					foreach ($params_sub as $value_sub) {
    						
    						$subquery -> condition($arrayTable.'.platform', $value_sub, 'LIKE');
    					}
    					$query->condition($subquery);
    				}
    				
    			break;	
    			case 'pubmed_ids':
    					$params_sub = preg_split("/[.,; ]+/",$value);
    					if(!empty($params_sub))
    					{
    						$query->condition($expTable.'.pubmed_id', $params_sub, 'IN');
    					}
    				break;
    			
    			case 'authors':
    			
    					$params_sub = preg_split("/[.,; ]+/",$value);
    					if(!empty($params_sub))
    					{
    						$query->condition($authorTable.'.search_last_name', $params_sub, 'IN');
    					}  				
    				break;		
    				
    			case 'variables':
    				$params_sub = preg_split("/[.,; ]+/",$value);
    						
    				if(!empty($params_sub))
    				{
    					foreach ($params_sub as $value_sub) {
    						
    						$query->condition(db_or()->condition($expTable.'.chemical_name', $value_sub, 'LIKE')->condition($expTable.'.basal', $value_sub, 'LIKE')
    						->condition($expTable.'.variable', $value_sub, 'LIKE')->condition($expTable.'.gene_name', $value_sub, 'LIKE'));
    					}
    				}
	
    				break; 
    			
    			case 'exp_types':		    
    				
						$query->condition($expTable.'.exp_type', $value, 'IN');			    				
    					
    				break;
    		}
    	}
	}
}
function ecoarray_search_paper_conditions($filter = array(),&$query)
{
	$refTable='r';
	$authorTable = 'author';
	$arrayTable = 'a';
	
	if(empty($filter))	return;	
    		
	foreach ($filter as $key => $value) {
		
		if($value!='')
    	{	
    		$query_sub = array();
    		$params_sub = array();
    
    		switch ($key)
    		{
    			case 'pubmed_ids':
    					$params_sub = preg_split("/[.,; ]+/",$value);
    					if(!empty($params_sub))
    					{
    						$query->condition($arrayTable.'.pubmed_id', $params_sub, 'IN');
    					}
    				break;
    			
    			case 'authors':
    			
    					$params_sub = preg_split("/[.,; ]+/",$value);
    					if(!empty($params_sub))
    					{
    						$query->condition($authorTable.'.search_last_name', $params_sub, 'IN');
    					}  				
    				break;		
    				
    			case 'keywords':
    					$params_sub = preg_split("/[.,; ]+/",$value);
    					if(!empty($params_sub))
    					{
    						  $query->condition(db_or()-> condition($arrayTable.'.topic', $params_sub, 'IN'));
    				
    					}
    				break;  
    		}
    	}
	}
	
}
function ecoarray_search_gene_conditions($filter = array(),&$query)
{
	$geneTable='t_gene';
	$expTable = 'exp';
	
	if(empty($filter))	return;	
    		
	foreach ($filter as $key => $value) {
		
		if($value!='')
    	{	
    		$query_sub = array();
    		$params_sub = array();
    
    		switch ($key)
    		{
    			case 'genes':
    					$params_sub = preg_split("/[.,; ]+/",$value);
    					if(!empty($params_sub))
    					{
    						  $OR = db_or()-> condition($geneTable.'.eg_id', $params_sub, 'IN');
    						foreach ($params_sub as $value) {
//    						 $query->condition(db_or()->condition($geneTable.'.name', $params_sub, 'IN')->condition($geneTable.'.eg_id', $params_sub, 'IN'));;
							  $OR = $OR ->condition($geneTable.'.name', $value.'%%', 'LIKE');
								
    						}
    						$query->condition($OR);
    					}
    				break;   			
    		
    			case 'exp_types':		    
    				
						$query->condition($expTable.'.exp_type', $value, 'IN');			    				
    					
    				break;
    		}
    	}
	}
	
}
function ecoarray_gene_return()
{
  global $base_url;

  $item_per_page = isset($_REQUEST['item_per_page'])? $_REQUEST['item_per_page']: 20;
  $page = isset($_REQUEST['page'])? $_REQUEST['page'] : 0;
	
  db_set_active("ecogene");

	$header = array(
//	t('Number'),
	array('data' => t('Name'), 'field' => 't_gene.name'),
	array('data' => t('EG ID'), 'field' => 't_gene.eg_id'),
	array('data' => t('Microarray Experiments'), 'field' => 'num_exp','sort' => 'desc')
	);
	
	// base class
	$query = db_select('t_array_data', 'data')-> extend('PagerDefault')-> extend('TableSort') ;
	
	// join gene and exp table
	$query -> join('t_array_exp', 'exp', 'exp.pubmed_id = data.pubmed_id AND exp.exp_id = data.exp_id');
	$query -> join('t_gene', 't_gene', 't_gene.eg_id = data.eg_id');
	
	// add fields
	$query -> addField('t_gene','name', 'name');
	$query -> fields('data'); 
	// add expressions
	$query -> addExpression('count(data.eg_id)', 'num_exp');
	
	$query_count = db_select('t_array_data', 'data');	
	// join gene and exp table
	$query_count -> join('t_array_exp', 'exp', 'exp.pubmed_id = data.pubmed_id AND exp.exp_id = data.exp_id');
	$query_count -> join('t_gene', 't_gene', 't_gene.eg_id = data.eg_id');
	$query_count -> addExpression('count(data.eg_id)', 'num_exp');
	
	search_experiment_conditions($_REQUEST,$query_count);
	$query_count->groupBy('data.eg_id');
	
	$query_count =  $query_count->countQuery()->execute()->fetchField();
	$item_per_page = is_numeric($item_per_page)? $item_per_page: $query_count;
	
	// sort and pagination
	$query -> limit($item_per_page);
	$query -> orderByHeader($header);
	$query->groupBy('data.eg_id');
	
	// User input filter
	ecoarray_search_gene_conditions($_REQUEST, $query);
	

//	echo $query->__toString();
	
//	// fetch the query result
	$records = array();
	$records = $query->execute()->fetchAll();
	
	db_set_active("default");
	
  if(!empty($records))
  {
  		$start_num = $page*+$item_per_page+1;
		$count_on_page = ($item_per_page>count($records))? count($records):$item_per_page;
		$stop_num = $page*+$item_per_page+$count_on_page;
		
		if($query_count>=$count_on_page)
		{
			
			$caption = 'Showing '.$start_num.'-'.$stop_num.' of '.$query_count.' genes';
		}
		else 
		{
			$caption = 'Showing '.$start_num.'-'.$stop_num.' of '.$query_count.' genes';
		}
		
	$order = 0;
	 foreach ($records as $item)
  	 {		
		$order = $order + 1;
   		$rows[] = array(
//		($order),
		('<a href="'.$base_url.'?q=gene/'.$item->eg_id.'">'.$item->name.'</a>'),
		('<a href="'.$base_url.'?q=gene/'.$item->eg_id.'">'.$item->eg_id.'</a>'),
		($item->num_exp. ' experiments <a href="'.$base_url.'?q=gene/'.$item->eg_id.'/array">Experimental data</a>'),		
		);
	}
  	$variables = array(	'header'=>$header, 
						'rows'=>$rows, 
						'sticky'=>NULL, 
						'empty'=>NULL, 
						'caption'=>$caption, 
						'colgroups'=>array(), 
						'attributes'=>array()
		);	
		$content = theme_table($variables);
  }
  else 
  {
		$content = '<em>No record has been found!</em>';
  }
  
  
  
  $build['content'] = array(
		'this_does_not_matter_too_much2' 
			=> array(
				'form' => drupal_get_form('ecoarray_gene_form'),
			),
        'this_does_not_matter_too_much1' 
			=> array(
			'#type' => 'markup',
            '#markup' => $content,
        ),
        
        
    );

  $build['pager'] = array(
    	'#theme' => 'pager',
    	'tags' => array('1'), 
    	'limit' => $item_per_page, 
    	'element' => 0, 
    	'parameters' => array(), 
    	'quantity' => 9,
//    	'#weight' => 1,
	);
   
	return $build;	
	
}
function search_experiment_query(&$records)
{
	global $base_url;

	$conditions = array();
	$params = array();
	
	$page = isset($_REQUEST['page'])? $_REQUEST['page'] : 0;
	$item_per_page = isset($_REQUEST['item_per_page'])? $_REQUEST['item_per_page']: 20;
//	
	db_set_active("ecogene");
	
	$header = array(
		array('data' => t('Reference'), 'field' => 'r.date_published', 
		'sort' => 'desc'),
		array('data' => t('Variable')),
		array('data' => t('Platform'),'field' => 't_array.platform'),
		array('data' => t('Exp type and ID'),'field' => 'exp.exp_type'),
		array('data' => t('Experimental Data')),
	);


	$query_count =  db_select('t_array_exp', 'exp');
	$query_count -> join('t_array', 't_array', 'exp.pubmed_id = t_array.pubmed_id');
	$query_count -> join('t_reference', 'r', 'exp.pubmed_id = r.pubmed_id');
	$query_count -> join('t_reference_author_link', 'ral', 'ral.pubmed_id = r.pubmed_id');
	$query_count -> join('t_author', 'author', 'author.author_id = ral.author_id');
	
	// needed fields
	$query_count -> fields('exp');
	$query_count -> addField('author', 'last_name', 'lastname'); 
	$query_count -> addField('t_array', 'platform', 'platform');
	$query_count -> addExpression('year(r.date_published)', 'year_published');
	$query_count -> condition('ral.seq_no', 1);
	search_experiment_conditions($_REQUEST,$query_count);
	
		// base class
	$query = db_select('t_array_exp', 'exp')-> extend('PagerDefault')-> extend('TableSort') ;
	
	// join to the reference, author and link table
	$query -> join('t_array', 't_array', 'exp.pubmed_id = t_array.pubmed_id');
	$query -> join('t_reference', 'r', 'exp.pubmed_id = r.pubmed_id');
	$query -> join('t_reference_author_link', 'ral', 'ral.pubmed_id = r.pubmed_id');
	$query -> join('t_author', 'author', 'author.author_id = ral.author_id');
	
	// needed fields
	$query -> fields('exp');
	$query -> addField('author', 'last_name', 'lastname'); 
	$query -> addField('t_array', 'platform', 'platform');
	$query -> addExpression('year(r.date_published)', 'year_published');
	$query -> condition('ral.seq_no', 1);
	search_experiment_conditions($_REQUEST,$query);
	
	
	$query_count =  $query_count->countQuery()->execute()->fetchField();
	$item_per_page = is_numeric($item_per_page)? $item_per_page: $query_count;
	
	// sort and pagination
	$query -> limit($item_per_page);
	$query -> orderByHeader($header);

	
	
//	echo $query->__toString();
	
	// fetch the query result
	$records = $query->execute()->fetchAll();
	db_set_active("default");
	return $query_count;
}
function search_experiment_return() {
	global $base_url;

//	$conditions = array();
//	$params = array();
//	
	$page = isset($_REQUEST['page'])? $_REQUEST['page'] : 0;
	$item_per_page = isset($_REQUEST['item_per_page'])? $_REQUEST['item_per_page']: 20;
////	
//	db_set_active("ecogene");
//	
	$header = array(
		array('data' => t('Reference'), 'field' => 'r.date_published', 
		'sort' => 'desc'),
		array('data' => t('Variable')),
		array('data' => t('Platform'),'field' => 't_array.platform'),
		array('data' => t('Exp type and ID'),'field' => 'exp.exp_type'),
		array('data' => t('Experimental Data')),
	);
//
//
//	$query_count =  db_select('t_array_exp', 'exp');
//	$query_count -> join('t_array', 't_array', 'exp.pubmed_id = t_array.pubmed_id');
//	$query_count -> join('t_reference', 'r', 'exp.pubmed_id = r.pubmed_id');
//	$query_count -> join('t_reference_author_link', 'ral', 'ral.pubmed_id = r.pubmed_id');
//	$query_count -> join('t_author', 'author', 'author.author_id = ral.author_id');
//	
//	// needed fields
//	$query_count -> fields('exp');
//	$query_count -> addField('author', 'last_name', 'lastname'); 
//	$query_count -> addField('t_array', 'platform', 'platform');
//	$query_count -> addExpression('year(r.date_published)', 'year_published');
//	$query_count -> condition('ral.seq_no', 1);
//	search_experiment_conditions($_REQUEST,$query_count);
//	
//		// base class
//	$query = db_select('t_array_exp', 'exp')-> extend('PagerDefault')-> extend('TableSort') ;
//	
//	// join to the reference, author and link table
//	$query -> join('t_array', 't_array', 'exp.pubmed_id = t_array.pubmed_id');
//	$query -> join('t_reference', 'r', 'exp.pubmed_id = r.pubmed_id');
//	$query -> join('t_reference_author_link', 'ral', 'ral.pubmed_id = r.pubmed_id');
//	$query -> join('t_author', 'author', 'author.author_id = ral.author_id');
//	
//	// needed fields
//	$query -> fields('exp');
//	$query -> addField('author', 'last_name', 'lastname'); 
//	$query -> addField('t_array', 'platform', 'platform');
//	$query -> addExpression('year(r.date_published)', 'year_published');
//	$query -> condition('ral.seq_no', 1);
//	search_experiment_conditions($_REQUEST,$query);
//	
//	
//	$query_count =  $query_count->countQuery()->execute()->fetchField();
//	$item_per_page = is_numeric($item_per_page)? $item_per_page: $query_count;
//	
//	// sort and pagination
//	$query -> limit($item_per_page);
//	$query -> orderByHeader($header);
//
//	
//	
////	echo $query->__toString();
//	
//	// fetch the query result
//	$records = array();
//	$records = $query->execute()->fetchAll();
//	db_set_active("default");
	$records = array();
	$query_count = search_experiment_query($records);
	if(!empty($records))
	{
		$start_num = $page*+$item_per_page+1;
		$count_on_page = ($item_per_page>count($records))? count($records):$item_per_page;
		$stop_num = $page*+$item_per_page+$count_on_page;
		
		if($query_count>=$count_on_page)
		{
			
			$caption = 'Showing '.$start_num.'-'.$stop_num.' of '.$query_count.' experiments';
		}
		else 
		{
			$caption = 'Showing '.$start_num.'-'.$stop_num.' of '.$query_count.' experiments';
		}
		
		
		foreach ($records as $item) {	
		$rows[] = array(
			'<a href="http://www.ncbi.nlm.nih.gov/entrez/query.fcgi?cmd=Retrieve&db=PubMed&list_uids='.
			$item->pubmed_id.'&dopt=Abstract">'.$item->lastname.','.$item->year_published.'</a>',
		(displayed_name($item)),
		$item->platform,
		t('<a href="'.$base_url.'?q=ecoarray/paper/'.$item->pubmed_id.'" title="show experiment detail">'
			.$item->exp_type.' '.$item->exp_id.'</a>'),
		t('<a href="'.$base_url.'?q=ecoarray/experiment/'.$item->pubmed_id.'/'.$item->exp_id.'">'
			.'Show data</a>'),	
		);
		
		}
	
	
		$variables = array(	'header'=>$header, 
						'rows'=>$rows, 
						'sticky'=>NULL, 
						'empty'=>NULL, 
						'caption'=>$caption, 
						'colgroups'=>array(), 
						'attributes'=>array()
		);	
		$content = theme_table($variables);
	}else 
	{
		$content = t('<em>No record has been found!</em>');
	}


	$build['content'] = array(
		'this_does_not_matter_too_much2' 
			=> array(
				'form' => drupal_get_form('ecoarray_experiment_form'),
			),
        'this_does_not_matter_too_much1' 
			=> array(
			'#type' => 'markup',
            '#markup' => $content,
        ),
        
        
    );

    $build['pager'] = array(
    	'#theme' => 'pager',
    	'tags' => array('1'), 
    	'limit' => $item_per_page, 
    	'element' => 0, 
    	'parameters' => array(), 
    	'quantity' => 9,
//    	'#weight' => 1,
	);
   
	return $build;	

}

function search_paper_conditions($array = array(),$conditions,$params)
{
	$refTable='t_reference';
	$authorTable = 't_author';
	$arrayTable = 't_array';
	
	$conditions='';
	$query = array();
    $params = array();
    
    if(empty($array))	return;
    		
	foreach ($array as $key => $value) {
		
		if($value!='')
    	{	$query_sub = array();
    		$params_sub = array();
    
    		switch ($key)
    		{
    			case 'submit_all':
    				if(trim($value)=='Show All')
    					$conditions = '';
						return $conditions;
    				break;
    			case 'author':
    			
    				get_sub_query($query_sub,$params_sub,$value,$authorTable,'search_last_name'," like '%%%s%%'");
    				$query[]= '( '. implode(' or ', $query_sub).' )';
    				$params = array_merge($params,$params_sub);    				
    				break;		
    				
    			case 'pubmed_id':		
    				get_sub_query($query_sub,$params_sub,$value,$arrayTable,'pubmed_id'," = '%d'");
    				$query[]= '( '. implode(' or ', $query_sub).' )';
    				$params = array_merge($params,$params_sub);  
    				break; 
    			
    			case 'key_word':		    
    				
					get_sub_query($query_sub,$params_sub,$value,$arrayTable,'topic'," like '%%%s%%'");
    				$query[]= '( '. implode(' or ', $query_sub).' )';
    				$params = array_merge($params,$params_sub);  		
    				break;
    		}
    	}
	}
	
	$conditions = implode(' AND ', $query);
	return $conditions;
}
function search_paper_return() {
	global $base_url;
	
	$page = isset($_REQUEST['page'])? $_REQUEST['page'] : 0;
	$item_per_page = isset($_REQUEST['item_per_page'])? $_REQUEST['item_per_page']: 10;
	
//	get_search_paper_conditions($_REQUEST,&$conditions,&$params);
	db_set_active("ecogene");
	
	$header = array(
		array('data' => t('Reference'), 'field' => 'r.date_published', 
				'sort' => 'desc'),
		array('data' => t('Experiments Link')),
		array('data' => t('Topic'),'field' => 'a.platform'),
		array('data' => t('Strain'),'field' => 'a.strain'),
		array('data' => t('Platform'),'field' => 'a.platform'),
		array('data' => t('Comments')),
		array('data' => t('Contact')),
	);
	
	// t_array is the base table
  	$query = db_select('t_array', 'a')->extend('PagerDefault')-> extend('TableSort');
	
	// join to the reference, author and link table
	$query -> join('t_reference', 'r', 'r.pubmed_id = a.pubmed_id');
	$query -> join('t_reference_author_link', 'ral', 'ral.pubmed_id = r.pubmed_id');
	$query -> join('t_author', 'author', 'author.author_id = ral.author_id');
	
	// needed fields
	$query -> fields('a') -> addField('author', 'last_name', 'last_name');	 
	$query -> addExpression('year(r.date_published)', 'year_published');
	$query -> condition('ral.seq_no', 1);
	
	
	$query_count = db_select('t_array', 'a');
	$query_count -> join('t_reference', 'r', 'r.pubmed_id = a.pubmed_id');
	$query_count -> join('t_reference_author_link', 'ral', 'ral.pubmed_id = r.pubmed_id');
	$query_count -> join('t_author', 'author', 'author.author_id = ral.author_id');
	$query_count -> fields('a') -> addField('author', 'last_name', 'last_name');	 
	$query_count -> addExpression('year(r.date_published)', 'year_published');
	$query_count -> condition('ral.seq_no', 1);
	ecoarray_search_paper_conditions($_REQUEST, $query_count);
	$query_count =  $query_count->countQuery()->execute()->fetchField();
	$item_per_page = is_numeric($item_per_page)? $item_per_page: $query_count;
	
	// sort and pagination
	$query-> limit($item_per_page);
	$query-> orderByHeader($header) ;
	
	ecoarray_search_paper_conditions($_REQUEST, $query);
	
	// fetch the query result
	$records = array();
	$records = $query->execute()->fetchAll();
	
	db_set_active("default");
	if(!empty($records))
	{
		$start_num = $page*+$item_per_page+1;
		$count_on_page = ($item_per_page>count($records))? count($records):$item_per_page;
		$stop_num = $page*+$item_per_page+$count_on_page;
		
		if($query_count>=$count_on_page)
		{
			
			$caption = 'Showing '.$start_num.'-'.$stop_num.' of '.$query_count.' paper';
		}
		else 
		{
			$caption = 'Showing '.$start_num.'-'.$stop_num.' of '.$query_count.' paper';
		}
			
	foreach ($records as $item) {	
		$rows[] = array(
			'<a href="http://www.ncbi.nlm.nih.gov/entrez/query.fcgi?cmd=Retrieve&db=PubMed&list_uids='.
			$item->pubmed_id.'&dopt=Abstract">'.$item->last_name.','.$item->year_published.'</a>',
			'<a href="'.$base_url.'?q=ecoarray/paper/'.$item->pubmed_id.'">'.t('Experiments').'</a>',
			$item->topic,
			$item->strain,
			$item->platform,
			$item->comments,
		('<a href="mailto:'.$item->contact_email.'">'.$item->contact_fullname.'</a>'),	
		);
		
	}
	
	$variables = array(	'header'=>$header, 
						'rows'=>$rows, 
						'sticky'=>NULL, 
						'empty'=>NULL, 
						'caption'=>$caption, 
						'colgroups'=>array(), 
						'attributes'=>array()
	);
		
	$content = theme_table($variables);
	}
	else 
	{
		$content = t('<em>No record has been found!</em>');
	}


	$build['content'] = array(
		'this_does_not_matter_too_much2' 
			=> drupal_get_form('ecoarray_paper_form'),
        'this_does_not_matter_too_much1' 
			=> array(
            '#markup' => $content,
        ),
        
        
    );
    $build['pager'] = array(
    	'#theme' => 'pager',
    	'tags' => array('1'), 
    	'limit' => 10, 
    	'element' => 0, 
    	'parameters' => array(), 
    	'quantity' => 9,
//    	'#weight' => -1,
	);
   
	return $build;	
}
function search_data_return() {
//	global $base_url;
//	$output='';
//	$numofresults=2500;
//	db_set_active("ecogene");
//	
//	$header = array(
//	array('data' => t('Gene Name'), 'field' => 't_gene.name'),
//	array('data' => t('Value'),'field' => 't_array_data.value', 'sort' => 'desc'),
//	array('data' => t('Validated'),'field' => 't_array_data.validated'),
//	array('data' => t('Note'),'field' => 't_array_data.note'),
//	);
//	
//	$sql_select = " select t_gene.name, t_array_data.*			  				
//			   ";
//	$sql_join = "
// 			FROM 
// 			t_array_data, t_gene   ";
//	$sql_condition.="WHERE t_array_data.eg_id=t_gene.eg_id ";
//	
//	$conditions= " ( t_array_data.pubmed_id= '%d' and t_array_data.exp_id='%d') ";
//	$params[]=$_REQUEST['pubmed_id'];
//	$params[]=$_REQUEST['exp_id'];
//	if(strlen(trim($conditions))>0)
//		$sql_condition.= ' and '. $conditions;
//	$sql_aggr=" ";
//	
//	$sql = $sql_select.$sql_join.$sql_condition.$sql_aggr;
//	$sql .= tablesort_sql($header);
////	print_r($_REQUEST);
////	print_r($sql);
////	print_r($params);
//	
//
//	$query_count = "select count(*)  as count ".$sql_join.$sql_condition;
//	
//	$result = pager_query($sql, $numofresults, 0, $query_count,$params);
//
//	
//	while ($item = db_fetch_object($result)) {		
//		
//   		$rows[] = array(
//		($item->name),
//		(sprintf("%.4f",($item->value))),
//		($item->validated),
//		($item->note)
//		);
//	}
//	
//	
//	db_set_active("default");
//	
//	$output.=theme('table', $header, $rows);
//	$form['pager'] = array('#value' => theme('pager', NULL, $numofresults, 0));	
//	if ($form['pager']['#value']) {
//		$output .= $form['pager']['#value'];//drupal_render($form['pager']);
//	}
//
//	db_set_active("default");
	return $output;
}
function ecoarray_paper_title($pubmed_id)
{
	db_set_active("ecogene");
	
	
	$args = array();
	$args['pubmed_id'] = $pubmed_id;
	
	$result = db_query(
		"SELECT 
			t_author.last_name as lastname,
			year(t_reference.date_published ) as year_published 
		FROM {t_author}, {t_array} , {t_reference_author_link}, {t_reference}
		WHERE 
			 t_array.pubmed_id = t_reference.pubmed_id AND 
	         t_array.pubmed_id = t_reference_author_link.pubmed_id AND
	         t_reference_author_link.author_id = t_author.author_id AND
			 t_reference_author_link.seq_no=1 AND
			 t_array.pubmed_id =:pubmed_id  
		", 
	$args);
	
	$row =  $result->fetchAssoc();
//	echo $pubmed_id;
//	print_r($result);
	db_set_active("default");
	
	if(empty($row)){
		return drupal_access_denied();
	}
	
	$title = '';
	$title = 'Microarray Experiments in '
	.l(	t($row['lastname'].', '.$row['year_published']), 
		'http://www.ncbi.nlm.nih.gov/sites/entrez?cmd=Retrieve&db=PubMed&list_uids='.$args['pubmed_id']);
	
	
  	return;	
}

function ecoarray_paper_details($pubmed_id)
{
		
	$content = '';
	db_set_active("ecogene");
	
	
	$args = array();
	$args['pubmed_id'] = $pubmed_id;
	
	$query_paper = db_query(
		"SELECT 
			t_array.*,
			t_author.last_name as lastname,
			year(t_reference.date_published ) as year_published 
		FROM {t_author}, {t_array} , {t_reference_author_link}, {t_reference}
		WHERE 
			 t_array.pubmed_id = t_reference.pubmed_id AND 
	         t_array.pubmed_id = t_reference_author_link.pubmed_id AND
	         t_reference_author_link.author_id = t_author.author_id AND
			 t_reference_author_link.seq_no=1 AND
			 t_array.pubmed_id =:pubmed_id  
		", 
	$args);
	
	$row_paper =  $query_paper->fetchAssoc();
//	echo $pubmed_id;
//	print_r($result);
	
	
	if(empty($row_paper)){
		db_set_active("default");
		return drupal_access_denied();
	}
	
	$header_paper = array(
		array('data' => t('Reference')),
		array('data' => t('Topic')),
		array('data' => t('Strain')),
		array('data' => t('Platform')),
		array('data' => t('Comments')),
		array('data' => t('Contact')),
	);
	
		
	$header_chemical = array(
		array('data' => t('Exp Type')),
		array('data' => t('Exp ID')),
		array('data' => t('Exp Data')),
		array('data' => t('Name')),
		array('data' => t('Concentration')),
		array('data' => t('Time added')),
		array('data' => t('Duration')),
		array('data' => t('Comments')),
	);
	
	$header_growth = array(
		array('data' => t('Exp Type')),
		array('data' => t('Exp ID')),
		array('data' => t('Exp Data')),
		array('data' => t('Name')),
		array('data' => t('Variable')),
		array('data' => t('Shift time')),
		array('data' => t('Duration')),
		array('data' => t('Comments')),
	);
	
	$header_gene = array(
		array('data' => t('Exp Type')),
		array('data' => t('Exp ID')),
		array('data' => t('Exp Data')),
		array('data' => t('Gene name')),
		array('data' => t('Allele')),
		array('data' => t('Comments')),
	);
	
	$query_chemical = db_select('t_array_exp','exp') -> fields('exp') -> condition('exp_type', 'chemical') -> condition('pubmed_id', $pubmed_id)->orderBy('exp_id');
	$query_growth = db_select('t_array_exp','exp') -> fields('exp') -> condition('exp_type', 'growth') -> condition('pubmed_id', $pubmed_id)->orderBy('exp_id');
	$query_gene =  db_select('t_array_exp','exp') -> fields('exp') -> condition('exp_type', 'gene') -> condition('pubmed_id', $pubmed_id)->orderBy('exp_id');
	$num_rows_chemical = $query_chemical->countQuery()->execute()->fetchField();
	$num_rows_growth = $query_growth->countQuery()->execute()->fetchField();
	$num_rows_gene = $query_gene->countQuery()->execute()->fetchField();
	
	db_set_active("default");
	
	$title = 'Microarray Experiments in 
	<a href ="http://www.ncbi.nlm.nih.gov/sites/entrez?cmd=Retrieve&db=PubMed&list_uids='.$row_paper['pubmed_id'].'">'.
	t($row_paper['lastname'].' '.$row_paper['year_published']).'</a>';
	
	drupal_set_title($title, PASS_THROUGH);
	
	$content = '';
	
		$variables = array(	
		'header'=>$header_paper, 
		'rows'=>array(
			array(
			'<a href ="http://www.ncbi.nlm.nih.gov/sites/entrez?cmd=Retrieve&db=PubMed&list_uids='.$row_paper['pubmed_id'].'">'.t($row_paper['lastname'].' '.$row_paper['year_published']).'</a>',
			t($row_paper['topic']),
			t($row_paper['strain']),
			t($row_paper['platform']),
			t($row_paper['comments']),
			'<a href="mailto:'.$row_paper['contact_email'].'"> '.$row_paper['contact_fullname'].'</a>',
			)
			
		), 
						'sticky'=>NULL, 
						'empty'=>NULL, 
						'caption'=>t(''), 
						'colgroups'=>array(), 
						'attributes'=>array()
		);	
		$content .= theme_table($variables);
		
	
	if ($num_rows_chemical>0)
	{
		$records = $query_chemical->execute()->fetchAll();
		foreach ($records as $item) {	
		$rows_chemical[] = array(			
			t('Chemical'),
			t($item->exp_id),
			l(t('Show experimental data'),'ecoarray/experiment/'.$item->pubmed_id.'/'.$item->exp_id),
			t($item->chemical_name),
			t($item->concentration),
			t($item->method_added),
			t($item->duration),
			t($item->comments),
		);
		
		}
		
		$variables = array(	'header'=>$header_chemical, 
						'rows'=>$rows_chemical, 
						'sticky'=>NULL, 
						'empty'=>NULL, 
						'caption'=>t($num_rows_chemical. ' chemical experiments'), 
						'colgroups'=>array(), 
						'attributes'=>array()
		);	
		$content .= theme_table($variables);
	}
	if ($num_rows_growth>0)
	{
						
		$records = $query_growth->execute()->fetchAll();
		foreach ($records as $item) {	
		$rows_growth[] = array(
			t('Growth'),
			t($item->exp_id),
			l(t('Show experimental data'),'ecoarray/experiment/'.$item->pubmed_id.'/'.$item->exp_id),
			t($item->basal.' vs '.$item->variable),
			t($item->variable),
			t($item->shift_time),
			t($item->duration),
			t($item->comments),
		);
		
		}
		$variables = array(	'header'=>$header_growth, 
						'rows'=>$rows_growth, 
						'sticky'=>NULL, 
						'empty'=>NULL, 
						'caption'=>t($num_rows_growth. ' growth experiments'), 
						'colgroups'=>array(), 
						'attributes'=>array()
		);	
		$content .= theme_table($variables);
	}
	if ($num_rows_gene>0)
	{
		$records = $query_gene->execute()->fetchAll();
		foreach ($records as $item) {	
		$rows_gene[] = array(
			t('Gene'),
			t($item->exp_id),
			l(t('Show experimental data'),'ecoarray/experiment/'.$item->pubmed_id.'/'.$item->exp_id),
			t($item->gene_name),
			t($item->allele.$item->mutation_kind),
			t($item->comments),
		);
		
		}
		
		$variables = array(	'header'=>$header_gene, 
						'rows'=>$rows_gene, 
						'sticky'=>NULL, 
						'empty'=>NULL, 
						'caption'=>t($num_rows_gene. ' gene experiments'), 
						'colgroups'=>array(), 
						'attributes'=>array()
		);	
		
		$content .= theme_table($variables);
		
	}
	
	db_set_active("default");
	return $content;
}

function ecoarray_experiment_title($pubmed_id, $exp_id)
{
	db_set_active("ecogene");
	$query = db_select('t_array_exp', 'exp') ;
	
	// join to the reference, author and link table
	$query -> join('t_array', 't_array', 'exp.pubmed_id = t_array.pubmed_id');
	$query -> join('t_reference', 'r', 'exp.pubmed_id = r.pubmed_id');
	$query -> join('t_reference_author_link', 'ral', 'ral.pubmed_id = r.pubmed_id');
	$query -> join('t_author', 'author', 'author.author_id = ral.author_id');
	
	// needed fields
	$query -> fields('exp');
	$query -> addField('author', 'last_name', 'lastname'); 
	$query -> addField('t_array', 'platform', 'platform');
	$query -> addExpression('year(r.date_published)', 'year_published');
	$query -> condition('ral.seq_no', 1);
	$query -> condition('exp.pubmed_id', $pubmed_id);
	$query -> condition('exp.exp_id', $exp_id);
	
	$records = array();
	$records = $query->execute()->fetchAll();
	db_set_active("default");
	if(empty($records)){
		return drupal_access_denied();
	}
	
	if(!empty($records))
	{
		foreach ($records as $item) {	
		$name = displayed_name($item);		
		$title = 'Microarray experimental data for genes <br /> affected by  <em>'. $name .'</em>'
		.' in '
		.l(	t($item->lastname.', '.$item->year_published), 
		'http://www.ncbi.nlm.nih.gov/sites/entrez?cmd=Retrieve&db=PubMed&list_uids='.$item->pubmed_id);
		
		break;
		}
	}

	
  	return $title;	
}

function ecoarray_experiment_details($pubmed_id, $exp_id)
{
	global $base_url;
	$content = "";
	db_set_active("ecogene");
	
//	$header = array(
//		array('data' => t('Gene Name'), 'field' => 't_gene.name'),
//		array('data' => t('Fold Value'),'field' => 't_array_data.fold_value', 'sort' => 'desc'),
//		array('data' => t('Log2 Ratio')),
//		array('data' => t('Validated'),'field' => 't_array_data.validated'),
//		array('data' => t('Note'),'field' => 't_array_data.note'),
//	);
	
       $header = array(
		array('data' => t('Gene Name'), 'field' => 't_gene.name'),
		array('data' => t('Fold Value'),'field' => 't_array_data.fold_value', 'sort' => 'desc'),		
		array('data' => t('Log2 Ratio'),'field' => 't_array_data.fold_value'),
		array('data' => t('Ln Ratio'),'field' => 't_array_data.fold_value'),
		array('data' => t('Ratio'),'field' => 't_array_data.fold_value'),
		array('data' => t('Validated'),'field' => 't_array_data.validated'),
		array('data' => t('Note'),'field' => 't_array_data.note'),
	);
	
	$query_select = db_select('t_array_data', 't_array_data')-> extend('TableSort') ;
	
	// join to the reference, author and link table
	$query_select -> join('t_gene', 't_gene', 't_array_data.eg_id=t_gene.eg_id');
	
	// needed fields
	$query_select -> fields('t_array_data')-> addField('t_gene', 'name', 'name');
	
	$query_select -> condition('t_array_data.exp_id', $exp_id)-> condition('t_array_data.pubmed_id', $pubmed_id);

	if (isset($_GET['order'])) {
    	if (strcasecmp('Fold Value', $_GET['order']) == 0) 
    		$query_select -> orderBy('abs(t_array_data.fold_value)', $_GET['sort']);
  	}else{
  		    $query_select -> orderBy('abs(t_array_data.fold_value)', 'desc');
  	}
	$query_select -> orderByHeader($header);
	
	$records = $query_select->execute()->fetchAll();
	db_set_active('default');
	drupal_set_title(ecoarray_experiment_title($pubmed_id, $exp_id), PASS_THROUGH);
	
	$egidA = array();
	$rows = array();	
	if(!empty($records))
	{
	foreach ($records as $item) {
		if($item->form==4)	$log2 = $item->value;
		else {
			// from fold value to log2 ratio 
			$log2 =  $item->fold_value>0? log($item->fold_value)/log(2): log(-1/ $item->fold_value)/log(2);
			
		}
		$ratio = pow(2, $log2);
		$ln = $log2/log(2);
			
//   		$rows[] = array(
//		t($item->name),
//		(sprintf("%.4f",($item->fold_value))),
//		(sprintf("%.4f",($log2))),
//		t($item->validated),
//		t($item->note)
//		);
 		$rows[] = array(
		t($item->name),
		(sprintf("%.4f",($item->fold_value))),
		(sprintf("%.4f",($log2))),
		(sprintf("%.4f",($ln))),
		(sprintf("%.4f",($ratio))),
		t($item->validated),
		t($item->note)
		);
		$egidA[] = $item->eg_id;
	}
	
	$variables = array(	'header'=>$header, 
						'rows'=>$rows, 
						'sticky'=>NULL, 
						'empty'=>NULL, 
						'caption'=>t(''), 
						'colgroups'=>array(), 
						'attributes'=>array()
		);	
	
	if(count($egidA)<=250)
	{
		$downloadlink = implode(",", $egidA);
		$content .="<a href=\"/genemap/map.php?egids=$downloadlink\"><img border=0 src=\"".$GLOBALS['base_url']."/sites/all/include/topic_map.php?egids=$downloadlink\" title=\"Genes On Map\"></a>";
	}else{
		
		$content .="<img border=0 src=\"".$GLOBALS['base_url']."/sites/all/include/topic_map.php?pubmed_id=$pubmed_id&exp_id=$exp_id\" title=\"Only GeneSet with up to 250 genes can be viewed on circle map\">";
	}	
	
	$content .= theme_table($variables);
	}else 
	{
		$content .= t('<em>No record has been found!</em>');
	}
	return $content;
}

