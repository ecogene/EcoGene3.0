<?php 

function ecogene_cross_ref_form($form, &$form_state) {
	  	$form['#tree'] = TRUE;
	
   
  $form['file_field'] = array(
	    '#type' => 'fieldset',
  		'#collapsible' => false,
  		'#title'=>'Choose genes from one of the following 3 options'	, 
  		'#prefix' => '<div id="crossref_uploadfile-controls">',
    	'#suffix' => '</div>', 		
    );
   
	
  $form["file_field"]["get_all"] = array(
      	'#type' => 'checkbox',
     	'#title' => 'All genes',   	
		'#default_value' =>0,
    );

    
    $form["file_field"]["uploadfile"] = array(
   		'#attributes' => array('enctype' => "multipart/form-data"),
     	'#type' => 'file',
    	'#size' =>40,
    	'#name' => "files[accessions_file_uploadfile]",
    	'#title' => 'Upload gene names or accessions from file',
  		'#description'=>'One name or accession per line',
	
    );
      
    $form["file_field"]["data_input"] = array(
      	'#type' => 'textarea',
     	'#rows' => 8,
     	'#cols' => 10,
     	'#title' => 'Input gene names or accessions in the text area',
   		'#description'=>'Separate names or accessions using a space or new line',
     	
	
    );
    
    $form['db_field'] = array(
   		'#type' => 'fieldset',
   		'#collapsible' => false,
   		'#title' => 'Choose identifiers',
     	'#prefix' => '<div id="crossref_db_controls">',
    	'#suffix' => '</div>',
    );
    $form['db_field']['dblist'] = array(
  	 '#type' => 'tableselect',
   	 '#multiple' =>TRUE,
   	 '#header' => array('db_name' => t('DB Name')),    
     '#options' =>  ecogene_xref_dblist(),
     '#default_value' => array('eg_id'=>1,'name'=>1) ,
     '#empty' => t('No GeneSet available.'),
  	
		
 	);	

    $form['mapping_field']['search'] = array(
  		'#value'=>t('Submit'),
	    '#type' => 'submit',
  		'#ajax' => array(
      		'callback' => 'ajax_ecogene_xref_callback',
  			'wrapper' => 'crossref_result_controls',
    	),	    	
 	);
   
	$form['mapping_result']['table'] = array(
     	'#prefix' => '<div id="crossref_result_controls">',
    	'#suffix' => '</div>',
    );
	
    
    
    return $form;
 
}


function ecogene_xref_dblist()
{
	$db_list = array('eg_id'=>array('db_name'=>'EcoGene Accession (EG)'),
					'name'=>array('db_name'=>'Gene Name'),
					'eck_id'=>array('db_name'=>'K-12 Gene Accession (ECK)'),
					'synonym'=>array('db_name'=>'Synonym(s)'),
	);
	db_set_active("ecogene");
//AmiGO
//COG
//Enzyme
//NiceZyme
//TCDB
//TIGRFam
//are excluded for having 1:n mapping

//Pseudogene.org
//CDD-COGs
//are excluded too
//	$excluded = array(9,17,8,16,18,19,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,2,53,54,55,56,22,65,34,80);
	$table_bio_db = 't_biodatabase';
	$query = db_select($table_bio_db, 'dbxref', array('fetch' => PDO::FETCH_ASSOC));
 	$query->fields('dbxref', array('name','biodatabase_id'));
 	$query->where('biodatabase_id=biodatabase_link');
// 	$query->condition('display',1,'<=');
 	$query->condition('xrefdb', 1);
// 	$query->condition('biodatabase_id',$excluded,'NOT IN');
 	$query->orderBy('name');
 	$query->orderBy('biodatabase_id');
	
     
   	$records = $query->execute()->fetchAll();
    
   
    foreach ($records as $key => $value) {
    	$db_list[$value['biodatabase_id']] = array(
    		'db_name'=>$value['name']
    	);

    	// add GO Component, Function Process with Names options

		if( $value['biodatabase_id'] == 111)
		{
			$db_list['GO Component (With Name)'] = array(
    			'db_name'=>'GO Component (With Name)'
    		);
			$db_list['GO Function (With Name)'] = array(
    			'db_name'=>'GO Function (With Name)'
    		);
    		$db_list['GO Process (With Name)'] = array(
    			'db_name'=>'GO Process (With Name)'
    		);
    		
		}
		
    }
    db_set_active("default");
    return $db_list;
}

function accession_file_upload($form, &$form_state) {

	
	$uploadfilefield = "accessions_file_uploadfile";
		
	$validators = array('file_validate_extensions' =>array('txt csv'),);
	$file = file_save_upload($uploadfilefield,$validators,'temporary://');	
  
  	
  	 // now read from it
  $realpath = drupal_realpath($file->uri);
  $file_extension = array_pop(explode('.', $file->filename));
  
  $fh = fopen($realpath, 'r');
  	
  $ids = array();
  if(strcmp($file_extension,'csv')==0){
	while(($row = fgetcsv($fh, NULL,",")) !== FALSE){
  	
	    $ids[] = $row[0];
    
	}  
  }elseif(strcmp($file_extension,'txt')==0){
  	
  	while(($buffer   = fgets($fh)) !== FALSE){
  	
  		$row = preg_split('/[\s,]+/', trim($buffer));

		if(empty($row[0])){
			continue;	
		} 
		    
	    $ids[] = $row[0];
    
	}  
	
  }
  
  fclose($fh);

  return $ids;
}

function ajax_ecogene_xref_callback($form, &$form_state) {
	global $base_url;
	
	$text_input = trim($form_state['values']['file_field']['data_input']);

	if($form_state['values']['file_field']['get_all']==0){
		if(!empty($text_input)){
			$accessions =  preg_split("/[ \r\n]+/",$text_input);
			
		}else{
			$accessions = accession_file_upload($form, &$form_state);
		}	
		$accessions = array_unique($accessions);
			
		if(empty($accessions))	return;
	}
		
	//The first column of the download cross refercence table is reserved 
	//	for the user uploaded accessions.
	$table_header = array('Input');
	
	//The rest columns are choosen by the user	
	
	foreach ($form_state['values']['db_field']['dblist'] as $key=>$value) {
		if($value){
			$table_header[] = $key;
			
		}
	}
	
	
	
	
	db_set_active("ecogene");
	$table_gene = 't_gene';
	$table_synonym = 't_gene_synonym';
	$table_db = 't_biodatabase';
	$table_db_link = 't_biodatabase_link';
	
	$table_go_term = 'term';
	
	$genes_xref = array();
	if($form_state['values']['file_field']['get_all']==1)
	{	
			

		$records = db_query( "SELECT eg_id FROM t_gene WHERE eck_id is not null and eck_id!='' " )->fetchAll();		
		foreach ($records as $value) {
			$genes_xref[$value->eg_id] = $value->eg_id;
		}
	
//		
	}else 
	{
//	1. Search in gene table for accessions
	$records = db_select($table_gene,$table_gene,array('fetch' => PDO::FETCH_ASSOC))
				->condition(db_or()
							->condition('eg_id', $accessions, 'IN')
							->condition('name', $accessions, 'IN')
							->condition('eck_id', $accessions, 'IN')
							)
				->fields($table_gene, array('eg_id','name','eck_id'))
				->execute()->fetchAll();
	$temp_xref_hash = array();	
		
	foreach ($records as $value) {	
						
		$temp_xref_hash[strtolower($value['eg_id'])] = $value['eg_id'];
		$temp_xref_hash[strtolower($value['name'])] = $value['eg_id'];
		$temp_xref_hash[strtolower($value['eck_id'])] = $value['eg_id'];
		
	}	
			
	$found_id = array();
	foreach ($accessions as $value) {
				$v = trim($value);
				if(array_key_exists(strtolower($v),$temp_xref_hash)){					
					$found_id[] = $value;				
					$genes_xref[$value]=$temp_xref_hash[strtolower($v)];
				}else{
//					$genes_xref[$value]=$temp_xref_hash[strtolower($v)]='';
					$genes_xref[$value]='';
				}	
	}
	
	
	$accessions = array_udiff($accessions, $found_id,"strcasecmp");
	
	//	2. Search in sysonym table for accessions
	if(!empty($accessions)){
		$records = db_select($table_synonym,$table_synonym,array('fetch' => PDO::FETCH_ASSOC))
					->condition('name', $accessions, 'IN')
					->fields($table_synonym, array('eg_id','name'))
					->execute()->fetchAll();	
		$temp_xref_hash = array();		
		$found_id = array();	
		foreach ($records as $value) {				
			$temp_xref_hash[strtolower($value['name'])] = $value['eg_id'];
			
		}	
		foreach ($accessions as $value) {
					$v = trim($value);
					if(array_key_exists(strtolower($v),$temp_xref_hash)){					
						$found_id[] = $value;		
						if($genes_xref[$value]=='')	{	
							$genes_xref[$value]=$temp_xref_hash[strtolower($v)];
						}
					}	
		}
		
		$accessions = array_udiff($accessions, $found_id,"strcasecmp");
	}
//	3. Search in sysonym table for accessions
	if(!empty($accessions)){
		$records = db_select($table_db_link,$table_db_link,array('fetch' => PDO::FETCH_ASSOC))
					->fields($table_db_link, array('eg_id','accession_id'))
					->condition('accession_id', $accessions, 'IN')
					->execute()->fetchAll();	
		$temp_xref_hash = array();	
		$found_id = array();			
		foreach ($records as $value) {				
			$temp_xref_hash[strtolower($value['accession_id'])] = $value['eg_id'];
			
		}	
		foreach ($accessions as $value) {
					$v = trim($value);
					if(array_key_exists(strtolower($v),$temp_xref_hash)){					
						$found_id[] = $value;		
						if($genes_xref[$value]=='')	{	
							$genes_xref[$value]=$temp_xref_hash[strtolower($v)];
						}
					}	
		}	
		
		$accessions = array_udiff($accessions, $found_id,"strcasecmp");
	}

	}	
// get gene name, eg_id, eck_id, synonym and accessstions for founded input accessions.		
	$all_genes_xref = array();		
	$query = db_select($table_gene,'gene',array('fetch' => PDO::FETCH_ASSOC));
	$query -> fields('gene', array('eg_id', 'name','eck_id'));
	$query ->condition('eg_id', $genes_xref, 'IN');
	$records = $query->execute()->fetchAll();
	
	foreach ($records as $value) {
				
		$all_genes_xref[$value['eg_id']]=array('eg_id'=>$value['eg_id'],
												'name'=>$value['name'],
												'eck_id'=>$value['eck_id'],
												'synonym'=>array(),
//												'accession_id' => array(),
										 );
		
	}
	
	
	$query = db_select($table_synonym,'synonym',array('fetch' => PDO::FETCH_ASSOC));
	$query -> fields('synonym', array('eg_id', 'name'));
	$query ->condition('eg_id', $genes_xref, 'IN');
	$records = $query->execute()->fetchAll();
	
	foreach ($records as $value) {
		if(array_key_exists($value['eg_id'],$all_genes_xref)){
			$all_genes_xref[$value['eg_id']]['synonym'][]=$value['name'];	
		}	
		
	}

	$query = db_select($table_db_link,'db_link',array('fetch' => PDO::FETCH_ASSOC));
	$query -> fields('db_link', array('eg_id', 'accession_id','biodatabase_id'));
	$query ->condition('eg_id', $genes_xref, 'IN');
	$result = $query->execute();
	while($value = $result->fetch()) {
	if(array_key_exists($value['eg_id'],$all_genes_xref)){
			if(empty($all_genes_xref[$value['eg_id']][$value['biodatabase_id']]))
				$all_genes_xref[$value['eg_id']][$value['biodatabase_id']]=$value['accession_id'];
			else 
				$all_genes_xref[$value['eg_id']][$value['biodatabase_id']].=';'.$value['accession_id'];
		}
    }

    // GO Component, Process, Function Names for found genes
	foreach ($table_header as $_key=>$h)
	{	
			 			
    	if( ($h == 'GO Component (With Name)')
    		|| ($h == 'GO Process (With Name)') 
    		|| ($h == 'GO Function (With Name)')
    	)
    	{
    		$_biodatabse = 0;
    	
    		switch ($h) {
    			case 'GO Function (With Name)':
    				$_biodatabse = 110;
    			break;
    			case 'GO Process (With Name)':
    				$_biodatabse = 111;
    			break;
    			case 'GO Component (With Name)':
    				$_biodatabse = 112;
    			break;
    			default:
    				;
    			break;
    		}
    		
     		$query = db_select($table_db_link,'db_link',array('fetch' => PDO::FETCH_ASSOC));
     		$query -> join($table_go_term, 'term', 'db_link.accession_id = term.acc');
     		$query -> fields('term', array('name'));
			$query -> fields('db_link', array('eg_id', 'accession_id','biodatabase_id'));
			$query ->condition('db_link.biodatabase_id', $_biodatabse);
			$query ->condition('db_link.eg_id', $genes_xref, 'IN');
			
			$result = $query->execute();
			
			while($value = $result->fetch()) {
				
				if(array_key_exists($value['eg_id'],$all_genes_xref)){
					if(empty($all_genes_xref[$value['eg_id']][$h]))
						$all_genes_xref[$value['eg_id']][$h]=$value['accession_id']." ".$value['name'];
					else 
						$all_genes_xref[$value['eg_id']][$h].=';'.$value['accession_id']." ".$value['name'];
				}
    		}
    	}
	}					

					
	//	format the output 
			$rows = array();
			
			foreach ($genes_xref as $v=>$eg_id) {
				$v = trim($v);
				if(!array_key_exists($eg_id,$all_genes_xref)){
					continue;
				}
				$gene_xref = array();  
				$gene_xref[] = $v;  
			

				$refs = $all_genes_xref[$eg_id];		
						foreach ($table_header as $_key=>$h){
							if($_key==0) continue;
							if(empty($refs[$h])){						
								$gene_xref[] = NULL;  
							}elseif($h=='synonym'){
								$gene_xref[] = implode('; ',$refs[$h]);
							}
							else{
								$gene_xref[] = $refs[$h];
							}
						}

						$rows[] = ($gene_xref);	
							
			}
	
			
//$markup = $markup = print_r($table_header,true).print_r($all_genes_xref,true);
//	$markup='Results:';
	db_set_active("default");
	
	$cache_id = 'xref_'.time();
	$db_name = ecogene_xref_dblist();
	foreach ($table_header as $_key=>$h){
		if($_key==0) continue;
		$table_header[$_key] = $db_name[$h]['db_name'];
	}
	
	array_unshift($rows,$table_header);
  	$csv_data = ecogene_download_store_data('ecodownload_'.$cache_id.'_csv', $rows,'csv');
  	$txt_data = ecogene_download_store_data('ecodownload_'.$cache_id.'_txt', $rows,'txt');
  	
  	
  	array_shift($rows);
//  	
	$element =$form['mapping_result']['table'];
	$markup2 = count($rows).' gene(s) are found'.'<br />';
	$markup2 .= count($accessions)>0?implode(", ", $accessions)." not found.":"";
	$markup2 .= '<br />';
  	$element['text'] = array(
		'#markup' => $markup2,
		);
		
	$element['links']['export'] = array(
		
    	'#markup' => '<strong>'.t('Download Result in : ').'</strong>'. l(t(' TXT file '), "ecodownload/report/".$txt_data['report']."/".$txt_data['user']."/txt").
   					 t(' or ').l(t('CSV file'), "ecodownload/report/".$csv_data['report']."/".$csv_data['user']."/csv"),
   		
  	);
	
	

//	$element['report'] = array(
//    	'#theme' => 'table',
//    	'#header' => $table_header,
//    	'#rows' => $rows,
//  		'#caption'=>count($rows).' gene(s) are found',
//    	'#attributes' => array('width' => '100%',  'class' => array('')),
//  	);
//  	
	$commands[] = ajax_command_replace("#crossref_result_controls", drupal_render($element));

   return array('#type' => 'ajax', '#commands' => $commands);
}

