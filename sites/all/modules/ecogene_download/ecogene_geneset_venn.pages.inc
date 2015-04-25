<?PHP
function geneset_upload_form($form, &$form_state) {
 
	$form_state['allowable_genesets'] = 10; // Maximum allowable number of genesets.
  	$form_state['ecogene_genesets'] = false; // different with the ecogene genesets
  
  
  	$form['#tree'] = TRUE;
	
 
  	$form['upload_step'] = array(
	    '#type' => 'markup',
  		'#markup'=>'<h2>Step 1: Upload GeneSets</h2>'	
  		
    );
    
  	$form['file_field'] = array(
	    '#type' => 'fieldset',
  		'#title'=>'Upload GeneSet from CSV File or Tab delimited TXT File.'	, 
  		'#prefix' => '<div id="uploadfile-controls">',
    	'#suffix' => '</div>', 		
    );
   
    $form['file_field']['name'] = array(
	    '#type' => 'textfield',
  		'#title'=>'GeneSet Name',
    	'#size' => 30,
	
    );
    
    $form['file_field']['uploadfile'] = array(
   		'#attributes' => array('enctype' => "multipart/form-data"),
	    '#type' => 'file',
  		'#description'=>'GeneSet file (One gene per line). The first column must be the IDs',
	
    );
    
    $form['file_field']['first_row'] = array(
   		'#type' => 'checkbox',
  		'#title'=>'First row of the file contains column name.',
    	'#default_value'=>1,
	
    );

  $form['file_field']['uploadfile_command'] = array(
    '#value' => t("Upload File"),
    '#type' => 'submit',
    '#submit' => array('geneset_file_upload'),
    '#ajax' => array(
  		'callback' => 'ajax_file_upload_callback',
      	'wrapper' => 'genesets_query_result_controls',
    	),	
    
  );
 
  $form['manage_step'] = array(
	    '#type' => 'markup',
  		'#markup'=>'<h2>Step 2: Uploaded GeneSets</h2>'	
    );
      
   $form['geneset_field'] = array(
     	'#prefix' => '<div id="genesets_controls">',
    	'#suffix' => '</div>',
    );
  if (empty($form_state['num_genesets'])) {
    	$form_state['num_genesets'] = 0;
  }
  	
  $options = array();
 	
  for ($i = 1; $i <= $form_state['num_genesets']; $i++) {
         
    $options[$i] = array('list'=>t('!url', array('!url' => t($form_state['storage']['genesets'][$i]['set_name']))),
 						   'num'=>$form_state['storage']['genesets'][$i]['num'],
    						'unique'=>$form_state['storage']['genesets'][$i]['unique'],
 						 
 	);
  }

  $form['geneset_field']['geneset_table'] = array(
  	 '#type' => 'tableselect',
   	 '#multiple' =>TRUE,
   	 '#header' => array('list' => t('GeneSet Name'),'num' => t('Records'),'unique' => t('Unique (Duplicated) Records')),
     '#options' => $options,
     '#empty' => t('No GeneSet available.'),
  	
		
 	);	
   
  $form['query_step'] = array(
  		'#markup'=>'<h2>Step 3: Execute Boolean Query</h2><br />'	
    );
 
  $form['query_result']['venn'] = array(
  		'#value'=>t('Venn Diagram'),
	    '#type' => 'submit',
  		'#ajax' => array(
      		'callback' => 'ajax_geneset_venn_callback',
  			'wrapper' => 'genesets_query_result_controls',
    	),	
 );
      
  $form['query_result']['inall'] = array(
  		'#value'=>t('In All Chosen'),
	    '#type' => 'submit',
  		'#ajax' => array(
      		'callback' => 'ajax_geneset_query_callback',
  			'wrapper' => 'genesets_query_result_controls',
    	),	
    );
  $form['query_result']['inany'] = array(
  		'#value'=>t('In Any Chosen'),
	    '#type' => 'submit',
  		'#ajax' => array(
      		'callback' => 'ajax_geneset_query_callback',
  			'wrapper' => 'genesets_query_result_controls',
    	),	
    );  
  
	$form['query_result']['table'] = array(
     	'#prefix' => '<div id="genesets_query_result_controls">',
    	'#suffix' => '</div>',
    );
    
    return $form;

  
}
function ajax_geneset_venn_callback($form, $form_state) {
	
//	 $commands[] = ajax_command_append("#genesets_query_result_controls", "wo wo");
//   return array('#type' => 'ajax', '#commands' => $commands);
   
	$genesets = array();
	$header = array();
	$ids = array();
	$names = array();
	for ($i = 1; $i <= $form_state['num_genesets']; $i++) {
	 	if($form_state['values']['geneset_field']['geneset_table'][$i]){
	 		$set_names[]=$form_state['storage']['genesets'][$i]['set_name'];
			$genesets[] = array_combine($form_state['storage']['genesets'][$i]['ids'], 
				$form_state['storage']['genesets'][$i]['records']);
			$ids[] = $form_state['storage']['genesets'][$i]['ids'];
			$header += drupal_map_assoc(array_values($form_state['storage']['genesets'][$i]['header']));
			
	 	}	 	
	 }
	$markup = '';
	if(count($genesets)<2){
		
		$sub_title = 'At least 2 GeneSets have to be selected!';
		
	}elseif(count($genesets)>=2){
//		module_load_include('inc', 'ecogene_download', 'ecogene_venndiagram');
		$sub_title = 'Interactive Venn Diagram for Selected GeneSets';
		$re = ecogene_create_venndiagram($ids);
		$imageFile=$re['destination'];
		$imageFileBW=$re['bw_destination'];
		
		$markup = '<input id="chk_venn_image" type="checkbox" onchange="switchVennImage(\''.file_create_url($imageFile).'\', \''.file_create_url($imageFileBW).'\')" name="Black and white image" > Switch to Black and White Image';
		$markup .= '<p>';
		foreach ($set_names as $ind=>$value) {
			if($ind>2) break;
			$markup .= chr($ind+65).': '.$value." <br />";			
		}
		
		$markup .= '</p>';
		$markup .= '<div id="venn_map" style="width:'.($re['width']+200).'px; height:'.($re['height']+150).'px; background-repeat:no-repeat; position: relative; background-image: url(\''.file_create_url($imageFile).'\');">';
  		
    	$map =$re['map'];
    	$title = $re['title'];
    	$query_set = $re['query_set'];
    	$query_set_html="";
  		foreach ($map as $t=>$value) {
  			// save the result to temporary file
  			$rows = merge_gene($genesets,$query_set[$t],$header);
  			$url = query_result_to_csv($rows,$header);
  			
  			$markup.= '<a id="set-id-'.$t.'" class="bullet green" href="javascript:void(0)" onclick="showPopup(this.id)"
  						style="position:absolute; width:'.($map[$t]['x2']-$map[$t]['x1']).'px; height:'.($map[$t]['y2']-$map[$t]['y1']).'px; display: block; left: '.$map[$t]['x1'].'px; top: '. $map[$t]['y1'].'px" rel="'.$map[$t]['x1'].'-'.$map[$t]['y1'].'"> </a>';

  			$markup.='<div id="set-id-'.$t.'-box" class="popup" style="padding: 10px; background-color:#00ee9e; display:none; z-index:10; position:absolute; width:175px; height:145px; left: '.$map[$t]['x1'].'px; top: '. ($map[$t]['y1']/2+$map[$t]['y2']/2).'px">
  							<div style="" class="popupcontent">
  								<p>'.str_replace("-", "<br />", $title[$t]).'</p>
  								<p>
    						    <a href="javascript:void(0)" onclick="ShowThisSet(\'query_set_'.$t.'\')">Show results</a> <br />
    						    '.l('Download',$url).'
    						    </p>    							
  							</div>
  							<a href="javascript:void(0)"  class="close">close</a>
						</div>';
  			

  			
  			if(!empty($url)&&$url)
  			{
  				$caption = $title[$t].' '.l('Download',$url);
  			}else{
				$caption = $title[$t];
  			}	
  			$query_set_html .= '<div class="query_set" id="query_set_'.$t.'" style="position:absolute; top:0; right:0; height: 600px; width: 450px; overflow: auto; display:none">'.theme('table',array(
				'caption'=>$caption,
  				'header'=>$header,
				'rows'=>$rows,
			)).'</div>';
  			
  		}	
  		$markup .="</div>";
		$markup = '<div style="position:relative">'.$markup.$query_set_html.'</div>';

	}
	if(count($genesets)>3){
		
		$sub_title = 'Interactive Venn Diagram for First Three Selected GeneSets';
	}
	 
//	$sub_title = 'wo wo';
	$element =$form['query_result']['table'];
	$element['#attached']['js'] = array(
  		drupal_get_path('module', 'ecogene_download') . '/ecogene_download.js',
  		);
	$element['title'] = array(
		'#markup'=>'<h2>'.$sub_title.'</h2>',
		'#suffix' => '<div id="geneset_query_result" style="width: 880px; height: 800px">',
    	
	);

	$element['table'] = array(
		'#markup'=>$markup,
		'#suffix' => '</div>',
	);
	 $commands[] = ajax_command_replace("#genesets_query_result_controls", drupal_render($element));

   return array('#type' => 'ajax', '#commands' => $commands);
}
function ajax_file_upload_callback($form, $form_state) {
	
	 $commands[] = ajax_command_replace("#genesets_controls", drupal_render( $form['geneset_field']));
	 $commands[] = ajax_command_replace("#uploadfile-controls", drupal_render( $form['file_field']));

	 if(($form_state['warning_genesets'])){
			$commands[] = ajax_command_before("#genesets_controls","<font color='red'>Maximum allowable number of genesets has reached!</font>");
	}
	if($form_state['warning_upload_file']===true) {
		
			$commands[] = ajax_command_after("#genesets_controls","<font color='red'>Duplicated records found in the upload.</font>");
	} 
  return array('#type' => 'ajax', '#commands' => $commands);
  
}
function merge_gene($genesets,$ids,$header)
{
	$rows=array();
	foreach ($genesets as $geneset)		{
			foreach ($geneset as $key=>$value) {
				if(in_array($key, $ids)){
					if(!array_key_exists($key, $rows)){
						foreach ($header as $h) {
							$rows[$key][$h] = array_key_exists($h, $value)?$value[$h]:NULL;
						}
						
					}else{
						foreach ($value as $field=>$f_value) {
					
							if($rows[$key][$field]===NULL){
								$rows[$key][$field] = $f_value;	
							}
						}
					}							
				}
			}
	}
	return $rows;
}
function ajax_geneset_query_callback($form, $form_state) {
	
    $genesets = array();
	$header = array();
	$ids = NULL;
	$sub_title = '';
	if ($form_state['triggering_element']['#value'] == t('In All Chosen')){
					
		$sub_title = 'In All Choosen: ';
	}
	if ($form_state['triggering_element']['#value'] == t('In Any Chosen')){
					
		$sub_title = 'In Any Choosen: ';
	}
	
	for ($i = 1; $i <= $form_state['num_genesets']; $i++) {
	 	if($form_state['values']['geneset_field']['geneset_table'][$i]){
			$genesets[] = array_combine($form_state['storage']['genesets'][$i]['ids'], 
				$form_state['storage']['genesets'][$i]['records']);
			
			$header += drupal_map_assoc(array_values($form_state['storage']['genesets'][$i]['header']));
			if($ids===NULL){
				$ids =  $form_state['storage']['genesets'][$i]['ids'];
			}else{
				if ($form_state['triggering_element']['#value'] == t('In All Chosen')){
					$ids = array_intersect($ids,$form_state['storage']['genesets'][$i]['ids']);
				}
				if ($form_state['triggering_element']['#value'] == t('In Any Chosen')){
					$ids = array_merge($ids,$form_state['storage']['genesets'][$i]['ids']);
				}
				
			}
	 	}
	 	
	 }
	 	 
	if(count($genesets)<1)
	{
		$sub_title = '';
		$markup = 'At least 1 GeneSets have to be selected!';
	} else{
//		drupal_set_message("You have uploaded: <pre>".print_r($ids, TRUE)."</pre>");
//		$rows = array();
		$rows = merge_gene($genesets,$ids,$header);
		
		
		
//		drupal_set_message("You have uploaded: <pre>".print_r($genesets, TRUE)."</pre>");	

	
  		$url = query_result_to_csv($rows,$header);
  		if(!empty($url)&&$url)
  		{
  			$sub_title .= '('.count($rows).') ' .l('Download',$url);
  		}else{
			$sub_title .= ' ('.count($rows).')';
  		}	
		$sub_title = '<h2>'.$sub_title.'</h2>';	 
		$markup = theme('table',array(
				'header'=>$header,
				'rows'=>$rows,
		));
//		$markup = implode('<br />', $result_records);
	}
	
	
	
	$element =$form['query_result']['table'];
	$element['title'] = array(
		'#markup'=>$sub_title,
		'#suffix' => '<div id="geneset_query_result" style="width: 880px; height: 800px; overflow: auto">',
    	
	);
	$element['table'] = array(
		'#markup'=>$markup,
		'#suffix' => '</div>',
	);
	 $commands[] = ajax_command_replace("#genesets_query_result_controls", drupal_render($element));
//	 $commands[] = ajax_command_append("#genesets_query_result_controls", "oooo");

  return array('#type' => 'ajax', '#commands' => $commands);
	
	
}

function query_result_to_csv($rows, $header=NULL, $destination=NULL){
	if($destination===NULL){
		 $destination = drupal_tempnam('public://', 'temp_file').'.csv';
	}
 

  // With all traditional PHP functions we can use the stream wrapper notation
  // for a file as well.
  $fp = fopen($destination, 'w');

  // To demonstrate the fact that everything is based on streams, we'll do
  // multiple 5-character writes to put this to the file. We could easily
  // (and far more conveniently) write it in a single statement with
  // fwrite($fp, $data).
 

  if($fp==FALSE) return false;
  
	if($header!==NULL){
		 fputcsv($fp, $header);
	}
	
   foreach ($rows as $fields) {
    	fputcsv($fp, $fields);
	}

	fclose($fp);
	
  	$url = file_create_url($destination);
  
  	return $url;
  		
//  drupal_set_message(t('Saved file as %filename (accessible via !url, uri=<span id="uri">@uri</span>)', array('%filename' => $destination, '@uri' => $destination, '!url' => l(t('this URL'), $url))));
		
  
  
}
function geneset_file_upload($form, &$form_state) {
	if($form_state['num_genesets']>=$form_state['allowable_genesets']){
			$form_state['warning_genesets']=TRUE;
			$form_state['rebuild'] = TRUE;
			return;
		}

	$validators = array('file_validate_extensions' =>array('txt csv'),);
	$file = file_save_upload('file_field',$validators);	
    	
	if (!$file) {
      form_set_error('file_field][file', 'Error uploading file.');
      return;
    }
    
  	
  	 // now read from it
  $realpath = drupal_realpath($file->uri);
  $file_extension = array_pop(explode('.', $file->filename));
  
  $fh = fopen($realpath, 'r');
  	
  $table_header = NULL;
  $records = array();
  if(strcmp($file_extension,'csv')==0){
	while(($row = fgetcsv($fh, NULL,",")) !== FALSE){
  	
	    if($table_header == NULL) {
	    	if($form_state['values']['file_field']['first_row']){
	    		
		      		foreach($row as $field) {
		        		$table_header[] = $field;
		      		};	  
				continue;  		
	    	}
  			else{  				
	  		 		for($i = 0; $i < count($row) ; $i++) {
	      				$table_header[] = 'Col_'.($i+1);
	    			}	    			
  			}
  			
	    }
  		
		if(empty($row[0])){
			continue;	
		} 
		    
	    $table_row = array();   
	    
	    for($i = 0; $i < count($table_header) ; $i++) {
	      $table_row[$table_header[$i]] = $row[$i];
	    };
	
	    $records[] = $table_row;
	    $ids[] = $row[0];
    
	}  
  }elseif(strcmp($file_extension,'txt')==0){
  	
  	while(($buffer   = fgets($fh)) !== FALSE){
  	
  		$row = preg_split('/[\s,]+/', trim($buffer));
  		
	    if($table_header == NULL) {
	    	if($form_state['values']['file_field']['first_row']){
	    		
		      		foreach($row as $field) {
		        		$table_header[] = $field;
		      		};	    		
  			continue;
	    	}
  		else{  				
	  		 		for($i = 0; $i < count($row) ; $i++) {
	      				$table_header[] = 'Col_'.($i+1);
	    			}	    			
  		}

	    }
  		
		if(empty($row[0])){
			continue;	
		} 
		    
	    $table_row = array();   
	    
	    for($i = 0; $i < count($table_header) ; $i++) {
	      $table_row[$table_header[$i]] = $row[$i];
	    };
	
	    $records[] = $table_row;
	    $ids[] = $row[0];
    
	}  
	
  }
  
  fclose($fh);
  
  $file_url = null;
  $num_unique = count(array_unique($ids));  
  $form_state['warning_upload_file'] = false;
  $duplicated = null;
  
  if($num_unique!=count($ids)){
  	$records = array_combine($ids, $records);  	
  	
//  	$ids2 = array_unique($ids);
  	$duplicated = array_diff_assoc($ids, array_unique($ids));  
  	foreach ($duplicated as $ind=>$value) {
  		$duplicated[$ind] = array($value);
  	}	
  	$ids = array_unique($ids);
  	
  	
  	$file_url = l(count($records), query_result_to_csv($records, $table_header));
  	$file_url .= '('.l(count($duplicated), query_result_to_csv($duplicated)).')';
  	 	
  	$form_state['warning_upload_file']=true;
  	
  }
  
  if($form_state['ecogene_genesets']===true){
  	db_set_active("ecogene");
		$result = db_query("
   			SELECT 	 g.eg_id  as eg_id, g.name, a.orientation, a.left_end, a.right_end
   			FROM	{t_gene} AS g
   					JOIN {t_address} AS a on g.address_id = a.address_id
    			WHERE g.eg_id IN (:eg_ids)", array(':eg_ids'=>$ids ));
   
  		$eg_id = array();
  		$table_header = array('eg_id','name','left_end', 'right_end','orientation');
  		$records = array();
  		foreach ($result as $row) {
  			    $eg_id[] = $row->eg_id;
  			    $records[] = array('eg_id'=>$row->eg_id,
  			    'name'=>$row->name,
  			    'left_end'=>$row->left_end,
  			    'right_end'=>$row->right_end,
  			    'orientation'=>$row->orientation,
  			   
  			    );
  		}
  		db_set_active("default");
  }
  
	$form_state['num_genesets']++;
  	$index = $form_state['num_genesets'];
  	
  	$form_state['storage']['genesets'][$index]=array(
  				'set_name'=> empty($form_state['values']['file_field']['name'])? $file->filename:$form_state['values']['file_field']['name'],
  				'source'=>'User Uploaded',
  				'ids' => $ids,
  				'records' => $records,
  				'num'=>count($ids)+count($duplicated),
  				//'num'=>l(count($ids),$file_url),
  				'header'=>$table_header,
  				'unique'=>is_null($file_url)?$num_unique : $file_url,
  				
  	);
//  	drupal_set_message(print_r($form_state['input'],true));
	unset($form_state['input']['file_field']);
	
	$form_state['rebuild'] = TRUE;
}


function geneset_form($form, &$form_state) {
 

  $form_state['allowable_genesets'] = 10; // Maximum allowable number of genesets.
  $form_state['ecogene_genesets'] = true; // different with the general genesets
  
  
  $form['#tree'] = TRUE;
 
  $form['upload_step'] = array(
	    '#type' => 'markup',
  		'#markup'=>'<h2>Step 1: Upload GeneSets</h2>'	
  );
    
  $form['topic_field'] = array(
	    '#type' => 'fieldset',
  		'#collapsible' => TRUE,
    	'#collapsed' => (empty($form_state['values'])||$form_state['values']['topic_field']['#collapsed']) ? TRUE:FALSE,
  		'#title'=>'Option 1: GeneSet from EcoTopic (Click to Open).',
  		 '#prefix' => '<div id="topic-controls">',
        '#suffix' => '</div>', 
  );
        

  $form['topic_field']['topic_controls'] = array();
//	  drupal_set_message(print_r($form['topic_field']['topic_controls'],true));
  $form['topic_field']['topic_controls'] += geneset_topic_select_form($form, $form_state);
 
  $form['array_field'] = array(
	    '#type' => 'fieldset',
  		'#collapsible' => TRUE,
    	'#collapsed' => (empty($form_state['values'])||$form_state['values']['array_field']['#collapsed']) ? TRUE:FALSE,
  		'#title'=>'Option 2: GeneSet from EcoArray (Click to Open).'	, 
  		'#prefix' => '<div id="array-controls">',
        '#suffix' => '</div>',
  );
        

  $form['array_field']['array_controls'] = array();
////	  drupal_set_message(print_r($form['topic_field']['topic_controls'],true));
  $form['array_field']['array_controls'] += geneset_array_select_form($form['array_field']['array_controls'], $form_state);


  	$form['file_field'] = array(
	    '#type' => 'fieldset',
  		'#collapsible' => TRUE,
    	'#collapsed' => (empty($form_state['values'])||$form_state['values']['file_field']['#collapsed']) ? TRUE:FALSE,
  		'#title'=>'Option 3: Upload GeneSet from CSV File or Tab delimited TXT File (Click to Open).'	, 
  		'#prefix' => '<div id="uploadfile-controls">',
    	'#suffix' => '</div>', 		
    );
   
    $form['file_field']['name'] = array(
	    '#type' => 'textfield',
  		'#title'=>'GeneSet Name',
    	'#size' => 30,
	
    );
    
    $form['file_field']['uploadfile'] = array(
   		'#attributes' => array('enctype' => "multipart/form-data"),
	    '#type' => 'file',
  		'#description'=>'GeneSet file (One gene per line). The first column must be the EG IDs',
	
    );
    
    $form['file_field']['first_row'] = array(
   		'#type' => 'checkbox',
  		'#title'=>'First row of the file contains column name.',
    	'#default_value'=>1,
	
    );

  $form['file_field']['uploadfile_command'] = array(
    '#value' => t("Upload File"),
    '#type' => 'submit',
    '#submit' => array('geneset_file_upload'),
    '#ajax' => array(
  		'callback' => 'ajax_file_upload_callback',
      	'wrapper' => 'genesets_query_result_controls',
    	),	
    
  );
 
  $form['manage_step'] = array(
	    '#type' => 'markup',
  		'#markup'=>'<h2>Step 2: Select Uploaded GeneSets</h2>'	
    );
      
   $form['geneset_field'] = array(
     	'#prefix' => '<div id="genesets_controls">',
    	'#suffix' => '</div>',
    );
  if (empty($form_state['num_genesets'])) {
    	$form_state['num_genesets'] = 0;
  }
  	
  $options = array();
 	
  for ($i = 1; $i <= $form_state['num_genesets']; $i++) {
         
    $options[$i] = array('list'=>t('!url', array('!url' => t($form_state['storage']['genesets'][$i]['set_name']))),
 						   'source'=>$form_state['storage']['genesets'][$i]['source'],
    						'num'=>$form_state['storage']['genesets'][$i]['num'],
    						'unique'=>$form_state['storage']['genesets'][$i]['unique'],
 						 
 	);
  }

  $form['geneset_field']['geneset_table'] = array(
  	 '#type' => 'tableselect',
   	 '#multiple' =>TRUE,
   	 '#header' => array('list' => t('GeneSet Name'),'source' => t('Source'),'num' => t('Records'),'unique' => t('Unique (Duplicated) Records')),
     '#options' => $options,
     '#empty' => t('No GeneSet available.'),
  	
		
 	);	
   
  $form['query_step'] = array(
  		'#markup'=>'<h2>Step 3: Execute Boolean Query on the Selected GeneSets</h2><br />'	
    );
 
  $form['query_result']['venn'] = array(
  		'#value'=>t('Venn Diagram'),
	    '#type' => 'submit',
  		'#ajax' => array(
      		'callback' => 'ajax_geneset_venn_callback',
  			'wrapper' => 'genesets_query_result_controls',
    	),	
 );
      
  $form['query_result']['inall'] = array(
  		'#value'=>t('In All Chosen'),
	    '#type' => 'submit',
  		'#ajax' => array(
      		'callback' => 'ajax_geneset_query_callback',
  			'wrapper' => 'genesets_query_result_controls',
    	),	
    );
  $form['query_result']['inany'] = array(
  		'#value'=>t('In Any Chosen'),
	    '#type' => 'submit',
  		'#ajax' => array(
      		'callback' => 'ajax_geneset_query_callback',
  			'wrapper' => 'genesets_query_result_controls',
    	),	
    );  
  
	$form['query_result']['table'] = array(
     	'#prefix' => '<div id="genesets_query_result_controls">',
    	'#suffix' => '</div>',
    );
    
    return $form;
    
}


function geneset_array_select_form($form, &$form_state) {

  if(isset($form_state['array_options']) && $form_state['triggering_element']['#value']!=t('Search Array')) {
  	$options = $form_state['array_options'];
  }else {
  	$options = geneset_array_select_options($form, $form_state);
  }

  $form['array_id'] = array(
  	 '#type' => 'tableselect',
   	 '#multiple' =>TRUE,
   	 '#header' => array('author_year' => t('Reference'),'variable' => t('Variable'),'platform' => t('Platform'),'exp_type_id' => t('Exp type'),'array_num_gene' => t('Number of Genes')),
     '#options' => $options,
     '#empty' => t('No GeneSet available.'),
  	 '#prefix' => '<div id="geneset_array_tableselect" style="height: 150px; overflow: auto">',
  	 '#suffix' => '</div>',
		
 	);	
 	
  $form['array_search'] = array(
    '#type' => 'textfield',
    '#description' => t('Search by Pubmed ID, Author, Platform or Experimental Variables (* is the wildcard)'),
  );
 

  $form['actions'] = array('#type' => 'actions');
  
  $form['actions']['search'] = array(
    '#type' => 'submit',
    '#value' => t('Search Array'),
    '#submit' => array('geneset_array_select_options'),
    
  	'#ajax' => array(
  	  'callback' => 'ajax_geneset_array_callback',
      'wrapper' =>  'array-controls',     
    ),
    '#weight' => 1,
  );

  
  $form['actions']['select'] = array(
    '#type' => 'submit',
    '#value' => t('Add Array GeneSet'),
//    '#validate' => array('geneset_array_select_form_validate'),
    '#submit' => array('geneset_array_add'),
    '#ajax' => array(
  	  'callback' => 'ajax_geneset_array_callback',
      'wrapper' =>  'geneset-controls',     
    ),
    '#weight' => 2,
  );

  return $form;
}

function geneset_array_select_options($form, &$form_state)
{
	global $base_url;
	
	
	if (!empty($form_state['values']['array_field']['array_controls']['array_search'])) {
		
    	$search = strtolower(str_replace('*', '%', $form_state['values']['array_field']['array_controls']['array_search']));
   	 	$search_args = array(
      		':platform' => $search,
      		':name' => $search,
      		':text' => $search,
   	 		':pubmed_id' => $search,
    	);
    	
    	db_set_active("ecogene");
		$sql = "SELECT   year(r.date_published) AS year_published, author.last_name AS lastname, t_array.platform AS platform, exp.exp_id, exp.pubmed_id, exp.exp_type, exp.chemical_name, exp.basal,exp.variable,exp.gene_name\n"
	    . "FROM t_array_exp exp \n"
	    . "INNER JOIN t_array t_array ON exp.pubmed_id = t_array.pubmed_id \n"
	    . "INNER JOIN t_reference r ON exp.pubmed_id = r.pubmed_id \n"
	    . "INNER JOIN t_reference_author_link ral ON ral.pubmed_id = r.pubmed_id \n"
	    . "INNER JOIN t_author author ON author.author_id = ral.author_id \n"
	    . "WHERE (ral.seq_no = 1) \n"
	    . "AND (exp.chemical_name LIKE :text OR exp.gene_name LIKE :text OR exp.variable LIKE :text OR exp.basal LIKE :text "
	    . " OR author.last_name LIKE :name OR exp.pubmed_id = :pubmed_id OR t_array.platform LIKE :platform ) \n"
	    . "GROUP BY exp.pubmed_id, exp.exp_id ";
	    
	    $result = db_query($sql, $search_args);
	    $records=array();
	    
	    foreach ($result as $row) {
	    	$args = array(
      		':pubmed_id' => $row->pubmed_id,
      		':exp_id' => $row->exp_id,
    		);
    	
	    	$num_gene = db_query(
	    				"SELECT count(distinct t_array_data.eg_id ) AS num_gene "
	    				."FROM t_array_data "
	    				."WHERE pubmed_id=:pubmed_id AND exp_id=:exp_id",
	    				$args
	    		)->fetch()->num_gene;
//	    	drupal_set_message(print_r($num_gene, true));
	    	if($num_gene>0){
	    		$row->num_gene =$num_gene;
	    		$records[] = $row;
	    	}
	    	
	    }
	    
	    
	    db_set_active("default");
	}else{
		
		
    	db_set_active("ecogene");
		$sql = "SELECT count(distinct t_data.eg_id ) AS num_gene, year(r.date_published) AS year_published, author.last_name AS lastname, t_array.platform AS platform, exp.exp_id, exp.pubmed_id, exp.exp_type, exp.chemical_name, exp.basal,exp.variable,exp.gene_name\n"
	    . "FROM t_array_data t_data \n"
	    . "INNER JOIN t_array_exp exp ON t_data.exp_id = exp.exp_id and t_data.pubmed_id = exp.pubmed_id \n"
	    . "INNER JOIN t_array t_array ON exp.pubmed_id = t_array.pubmed_id \n"
	    . "INNER JOIN t_reference r ON exp.pubmed_id = r.pubmed_id \n"
	    . "INNER JOIN t_reference_author_link ral ON ral.pubmed_id = r.pubmed_id \n"
	    . "INNER JOIN t_author author ON author.author_id = ral.author_id \n"
	    . "WHERE (ral.seq_no = 1) \n"
	    . "GROUP BY exp.pubmed_id, exp.exp_id ";
	    
	    $records = db_query($sql);
	    db_set_active("default");
	}
    
	$options = array();
  	foreach ($records as $row) {
  		
  			if($row->exp_type=='chemical'){
				$variable =  $row->chemical_name;
			}elseif ($row->exp_type=='growth'){
				$variable = $row->basal." vs ".$row->variable;
			}else{
				$variable = trim($row->gene_name);
			}
			
  		$options[$row->pubmed_id.'_'.$row->exp_id] = array(
    		'author_year'=> l("$row->lastname , $row->year_published", "http://www.ncbi.nlm.nih.gov/entrez/query.fcgi?cmd=Retrieve&db=PubMed&list_uids=$row->pubmed_id"."&dopt=Abstract"),
    		'variable'=>$variable,
    		'platform'=> $row->platform,
    		'exp_type_id'=>$row->exp_type,
    		'array_num_gene'=>l($row->num_gene, "ecoarray/experiment/{$row->pubmed_id}/{$row->exp_id}")
    	);  		
  	}

  $form_state['array_options'] = $options;
    
  if($form_state['triggering_element']['#value'] == t('Search Array')){  	
    $form_state['rebuild'] = TRUE;
  }
    
	return $options;
	
}

function geneset_array_select_form_validate($form, &$form_state) {
	
	$valid = FALSE;
	if(!empty($form_state['values']['array_field']['array_controls']['array_id'])){
		$topic_ids = $form_state['values']['array_field']['array_controls']['array_id'];
		foreach ($topic_ids as $value) {
			if($value>0){
				$valid = TRUE;
			}
		}
	}
	
  if ($valid===FALSE) {
    form_set_error($form['array_field']['array_controls']['array_id'], t('Please select an experiment.'));
  }
  
}

function geneset_topic_select_form($form, &$form_state) {
	
 $options = array();
  if(isset($form_state['array_options']) && $form_state['triggering_element']['#value']!=t('Search Topic')) {
  	$options = $form_state['topic_options'];
  }else {
  	$options = geneset_topics_select_options($form, $form_state);
  }
//  $options =   geneset_topics_select_options($form, $form_state);
    
  $form = array();		
  $form['topic_id'] = array(
  	 '#type' => 'tableselect',
   	 '#multiple' =>TRUE,
   	 '#header' => array('topic_id' => t('Topic ID'),'topic_name' => t('Topic Name'),'topic_num_gene' => t('Number of Genes')),
     '#options' => $options,
     '#empty' => t('No GeneSet available.'),
  	 '#prefix' => '<div id="geneset_topic_tableselect" style="height: 150px; overflow: auto">',
  	 '#suffix' => '</div>',
		
 	);	
 	
  $form['topic_search'] = array(
    '#type' => 'textfield',
    '#description' => t('Search by topic name or title (* is the wildcard)'),
  );

  $form['actions'] = array('#type' => 'actions');
  
  $form['actions']['search'] = array(
    '#type' => 'submit',
    '#value' => t('Search Topic'),
    '#submit' => array('geneset_topics_select_options'),
  	'#ajax' => array(
  	  'callback' => 'ajax_geneset_topic_callback',
      'wrapper' =>  'topic-controls',     
    ),
    '#weight' => 1,
  );
  
  $form['actions']['select'] = array(
    '#type' => 'submit',
    '#value' => t('Add Topic GeneSet'),
    '#validate' => array('geneset_topic_select_form_validate'),
    '#submit' => array('geneset_topics_add'),
    '#ajax' => array(
  	  'callback' => 'ajax_geneset_topic_callback',
      'wrapper' =>  'geneset-controls',     
    ),
    '#weight' => 2,
  );

  return $form;
}
function geneset_topics_select_options($form, &$form_state) {
  
  $options = array();

//  drupal_set_message(print_r($form_state['triggering_element'],true));
  
  if (!empty($form_state['values']['topic_field']['topic_controls']['topic_search'])) {
    $search = strtolower(str_replace('*', '%', $form_state['values']['topic_field']['topic_controls']['topic_search']));
    $search_args = array(
      ':title' => $search,
      ':name' => $search,
      ':text' => $search,
    );
//	drupal_set_message(print_r($search_args,true));
    db_set_active('ecogene');
	
    $result = db_query("SELECT n.topic_id, n.topic_name, count(distinct p.eg_id ) as num_gene FROM {t_topic} AS n LEFT JOIN 
    			{t_topic_gene_link} AS p ON n.topic_id = p.topic_id 
    			WHERE (n.topic_text LIKE :text OR n.topic_name LIKE :name OR n.topic_title LIKE :title)"
      . " GROUP BY n.topic_id HAVING num_gene>0  ORDER BY n.topic_id", $search_args);
    
   db_set_active('default');  
  }
  else {
  	db_set_active('ecogene');
    $result = db_query("SELECT n.topic_id, n.topic_name, count(distinct p.eg_id ) as num_gene FROM {t_topic} AS n LEFT JOIN 
    			{t_topic_gene_link} AS p ON n.topic_id = p.topic_id 
    			 GROUP BY n.topic_id  HAVING num_gene>0  ORDER BY n.topic_id");
    db_set_active('default');  
  }
  
 
  $options = array();
  foreach ($result as $row) {
    $options[$row->topic_id] = array(
    		'topic_id'=> $row->topic_id,
    		'topic_name'=>$row->topic_name,
    		'topic_num_gene'=>$row->num_gene,
    );
    
//    sprintf("[%03d]",$row->topic_id).' '.$row->topic_name.' ('.$row->num_gene.' genes)';
  }

  $form_state['topic_options'] = $options;
  
  if($form_state['triggering_element']['#value'] == t('Search Topic')){
    $form_state['rebuild'] = TRUE;
  }
	return $options;
}

function geneset_array_add($form, &$form_state) {
	
	$my_value=0;	
	$array_ids = $form_state['values']['array_field']['array_controls']['array_id'];
	
	$array_ids = array_filter($array_ids, function($array_ids) use ($my_value) { 
		return ($array_ids != $my_value); 
	} ); 
	
		
	db_set_active('ecogene');
	foreach ($array_ids as $value) {
		if($form_state['num_genesets']>=$form_state['allowable_genesets']){
			$form_state['warning_genesets']=TRUE;
			break;
		}
		
		$form_state['num_genesets']++;
	  	
	  	
		$v = explode("_", $value);
		
		$search_args = array(':pubmed_id' => $v[0],  ':exp_id' => $v[1]);
   		$result = db_query("SELECT *  FROM  {t_array_exp} WHERE pubmed_id=:pubmed_id AND exp_id=:exp_id", $search_args);
   		
   		if($result){   	
   			$row= $result->fetch();
   				if($row->exp_type=='chemical'){
				$variable =  $row->chemical_name;
			}elseif ($row->exp_type=='growth'){
				$variable = $row->basal." vs ".$row->variable;
			}else{
				$variable = trim($row->gene_name);
			}
   			$title = $variable;
   		}


   	
  		
   		$result = db_query("
   				SELECT distinct p.eg_id, p.fold_value,  g.name,  
   					a.orientation, a.left_end, a.right_end
   				FROM  
    				{t_array_data} AS p 
    				JOIN {t_gene} AS g on p.eg_id=g.eg_id 
   					JOIN {t_address} AS a on g.address_id = a.address_id
    			WHERE pubmed_id=:pubmed_id AND exp_id=:exp_id", $search_args);
   		db_set_active("default");
  		$eg_id = array();
  		$table_header = array('eg_id','name','left_end', 'right_end','orientation','fold');
  		$records = array();
  		foreach ($result as $row) {
  			   $eg_id[] = $row->eg_id;
  			    $records[] = array('eg_id'=>$row->eg_id,
  			    'fold'=>sprintf("%.2f",$row->fold_value),
  			    'name'=>$row->name,
  			    'left_end'=>$row->left_end,
  			    'right_end'=>$row->right_end,
  			    'orientation'=>$row->orientation,
  			   
  			    );
  		}
   
		
	  	$index = $form_state['num_genesets'];	  	
	  	$form_state['storage']['genesets'][$index]=array(
//	  				'set_name'=> empty($form_state['values']['list_field']['name'])? chr($index+64):$form_state['values']['list_field']['name'],
	  				'set_name'=>$title,
	  				'source'=>'EcoArray',
	  				'ids' => $eg_id,
	  				'num'=>count($eg_id),  	
	  				'unique'=>count(array_unique($eg_id)),		
	  				'records' => $records,	
	  				'header'=>$table_header,
	  	);
	}
	db_set_active('default'); 
//  	
	unset($form_state['input']['array_field']['array_controls']['array_id']);
	$form_state['rebuild'] = TRUE;  
}

function geneset_topics_add($form, &$form_state) {
	
	$my_value=0;	
	$topic_ids = $form_state['values']['topic_field']['topic_controls']['topic_id'];
	
	$topic_ids = array_filter($topic_ids, function($topic_ids) use ($my_value) { 
		return ($topic_ids != $my_value); 
	} ); 
	
		
	
	foreach ($topic_ids as $value) {
		if($form_state['num_genesets']>=$form_state['allowable_genesets']){
			$form_state['warning_genesets']=TRUE;
			break;
		}
		$search_args = array(':topic_id' => $value   );
		db_set_active('ecogene');
   		$result = db_query("SELECT topic_name  FROM  {t_topic} WHERE topic_id=:topic_id", $search_args);
   		
   		if($result){   	
   			$title = $result->fetch()->topic_name;
   
   		}
		
		
   		$result = db_query("
   			SELECT 	distinct p.eg_id  as eg_id, g.name, a.orientation, a.left_end, a.right_end
   			FROM	{t_topic_gene_link} AS p
   					JOIN {t_gene} AS g on p.eg_id=g.eg_id 
   					JOIN {t_address} AS a on g.address_id = a.address_id
    			WHERE p.topic_id=:topic_id", $search_args);
   		db_set_active("default");
  		$eg_id = array();
  		$table_header = array('eg_id','name','left_end', 'right_end','orientation');
  		$records = array();
  		foreach ($result as $row) {
  			    $eg_id[] = $row->eg_id;
  			    $records[] = array('eg_id'=>$row->eg_id,
  			    'name'=>$row->name,
  			    'left_end'=>$row->left_end,
  			    'right_end'=>$row->right_end,
  			    'orientation'=>$row->orientation,
  			   
  			    );
  		}
   
   
  		
	
		
	  	$form_state['num_genesets']++;
	  	$index = $form_state['num_genesets'];
	  	
	  	$form_state['storage']['genesets'][$index]=array(
//	  				'set_name'=> empty($form_state['values']['list_field']['name'])? chr($index+64):$form_state['values']['list_field']['name'],
	  				'set_name'=>$title,
	  				'source'=>'EcoTopic',
	  				'ids' => $eg_id,
	  				'num'=>count($eg_id),  	
	  				'unique'=>count(array_unique($eg_id)),		
	  				'records' => $records,	
	  				'header'=>$table_header,
	  				'link'=>l(count($eg_id), 'ecosearch/gene/search&search_topic='.$value),
	  	);
	}
	db_set_active('default'); 
//  	
	unset($form_state['input']['topic_field']['topic_controls']['topic_id']);
	$form_state['rebuild'] = TRUE;  
}

function ajax_geneset_topic_callback($form, &$form_state) {


//	$debug=array(
//			'#markup'=>print_r($topic_filed_form,true),
//	);
	
	if($form_state['triggering_element']['#value'] == t('Search Topic')){
		$commands[] = ajax_command_replace('#topic-controls', drupal_render($form['topic_field']));
		
		
	}elseif($form_state['triggering_element']['#value'] == t('Add Topic GeneSet')){
		$commands[] = ajax_command_replace("#genesets_controls", drupal_render( $form['geneset_field']));
//		$commands[] = ajax_command_replace("#genesets_controls", drupal_render($debug));
		$commands[] = ajax_command_replace('#topic-controls', drupal_render($form['topic_field']));
		
		if(($form_state['warning_genesets'])){
			$commands[] = ajax_command_before("#genesets_controls","<font color='red'>Maximum allowable number of genesets has reached!</font>");
		}
	}
	


  return array('#type' => 'ajax', '#commands' => $commands);
}

function ajax_geneset_array_callback($form, &$form_state) {

	if($form_state['triggering_element']['#value'] == t('Search Array')){
		$commands[] = ajax_command_replace('#array-controls', drupal_render($form['array_field']));
		
		
	}elseif($form_state['triggering_element']['#value'] == t('Add Array GeneSet')){
		$commands[] = ajax_command_replace("#genesets_controls", drupal_render( $form['geneset_field']));
		$commands[] = ajax_command_replace('#array-controls', drupal_render($form['array_field']));
		if(($form_state['warning_genesets'])){
			$commands[] = ajax_command_before("#genesets_controls","<font color='red'>Maximum allowable number of genesets has reached!</font>");
		}
		
	}
	


  return array('#type' => 'ajax', '#commands' => $commands);
}

/**
 * Validation handler for geneset_topic_select_form().
 */
function geneset_topic_select_form_validate($form, &$form_state) {
	$valid = FALSE;
	if(!empty($form_state['values']['topic_field']['topic_controls']['topic_id'])){
		$topic_ids = $form_state['values']['topic_field']['topic_controls']['topic_id'];
		foreach ($topic_ids as $value) {
			if($value>0){
				$valid = TRUE;
			}
		}
	}
	
  if ($valid===FALSE) {
    form_set_error($form['topic_field']['topic_controls']['topic_id'], t('Please select a topic.'));
  }
}


// venn diagram for 2 or 3 sets

function ecogene_create_venndiagram($ids)
{
//	global $fontfile, $imgFileVenn, $imgFileVennBW, $imgFileVenn2, $imgFileVenn2BW;
	 $fontfile = 'sites/all/include/verdana.ttf';
	// temporary file to store the venn diagram
	$destination = drupal_tempnam('public://', 'temp_venn').'.png';
 	$bw_destination = drupal_tempnam('public://', 'temp_vennbw').'.png';
    
	if(count($ids)>=3){
//		if($color){ 
			$srcimage = @ImageCreateFrompng("sites/all/images/venn3.png")
      			or die("Cannot Initialize new GD image stream"); 	
//		}else{
			$srcimage_bw = @ImageCreateFrompng("sites/all/images/venn3_bw.png")
      			or die("Cannot Initialize new GD image stream");
//		}
      		
      	// A and B
	    $two_set[0] = array_intersect($ids[0], $ids[1]);
	    // A and C
	    $two_set[1] = array_intersect($ids[0], $ids[2]);
	    // B and C
	    $two_set[2] = array_intersect($ids[1], $ids[2]);
	    
	    // A, not B, not C
	    $gene_sets[0] = array_diff(array_diff($ids[0], $ids[1]), $ids[2]);
	    // B, not A, not C
	    $gene_sets[1] = array_diff(array_diff($ids[1], $ids[0]), $ids[2]);
	    // C, not B, not A
	    $gene_sets[2] = array_diff(array_diff($ids[2], $ids[0]), $ids[1]);
	    
	    //A and B not C
	    $gene_sets[3] = array_diff($two_set[0], $ids[2]);
	     //B and C not A
	    $gene_sets[4] = array_diff($two_set[2], $ids[0]);
	     //A and C not B
	    $gene_sets[5] = array_diff($two_set[1], $ids[1]);
	    
	    //A and B and C
	    $gene_sets[6] = array_intersect($two_set[0], $two_set[1]);
	    
	    // Image map information of venn diagram of 3 sets
	 	$map = array(
			array("x1"=>46, "y1"=>50, "x2"=>98, "y2"=>131),
			array("x1"=>250, "y1"=>50, "x2"=>300, "y2"=>131),
			array("x1"=>120, "y1"=>250, "x2"=>240, "y2"=>290),
			array("x1"=>150, "y1"=>58, "x2"=>211, "y2"=>95),
			array("x1"=>219, "y1"=>183, "x2"=>270, "y2"=>214),
			array("x1"=>81, "y1"=>180, "x2"=>136, "y2"=>220),
			array("x1"=>142, "y1"=>134, "x2"=>225, "y2"=>178),			
			
		);
		$title = array(
		'(A) and (not B) and (not C) - '.count($gene_sets[0]). ' genes.',
		'(B) and (not C) and (not A) - '.count($gene_sets[1]). ' genes.',
		'(C) and (not A) and (not B) - '.count($gene_sets[2]). ' genes.',
		'(A) and (B) and (not C) - '.count($gene_sets[3]). ' genes.',
		'(B) and (C) and (not A) - '.count($gene_sets[4]). ' genes.',
		'(C) and (A) and (not B) - '.count($gene_sets[5]). ' genes.',
		'(A) and (B) and (C) - '.count($gene_sets[6]). ' genes.',
		);		
	    
	}else{
//		if($color){ 
			$srcimage = @ImageCreateFrompng("sites/all/images/venn2.png")
      			or die("Cannot Initialize new GD image stream"); 	
//		}else{
			$srcimage_bw = @ImageCreateFrompng("sites/all/images/venn2_bw.png")
      			or die("Cannot Initialize new GD image stream");
//		}
  
		// A not B
		$gene_sets[0] = array_diff($ids[0], $ids[1]);
	     //B not A
	    $gene_sets[1] = array_diff($ids[1], $ids[0]);
	     //A and B
	    $gene_sets[2] = array_intersect($ids[0], $ids[1]);
	    
    	$map = array(
			array("x1"=>46, "y1"=>50, "x2"=>98, "y2"=>220),
			array("x1"=>250, "y1"=>50, "x2"=>300, "y2"=>220),
			array("x1"=>150, "y1"=>58, "x2"=>211, "y2"=>210),
		);		
		$title = array(
			'(A) and (not B) - '.count($gene_sets[0]). ' genes.',
			'(B) and (not A) - '.count($gene_sets[1]). ' genes.',
			'(A) and (B) - '.count($gene_sets[2]). ' genes.',
			
		);	
    
	}	
	
	$width = 346;
	$height = 332;

	header ("Content-type: image/png");
	$img=imagecreatetruecolor($width,$height);
	imagecopyresampled($img,$srcimage,0,0,0,0, $width,$height,$width,$height);
	
	$background = imagecolorallocate ($img,  0, 0, 0);	
//    	     						
    $text_size =11;
	
 
    for ($t=0;$t<count($map);$t++)
    {
    	$number = count($gene_sets[$t]);
		imagettftext($img, $text_size, 0, $map[$t]['x1']/2+$map[$t]['x2']/2-$text_size, 
						$map[$t]['y1']/2+ $map[$t]['y2']/2-$text_size/2,$background, $fontfile, $number);
    }
	imagettftext($img, $text_size+5, 0, 20, 30,$background, $fontfile, 'A');
	imagettftext($img, $text_size+5, 0, $width-20, 30,$background, $fontfile, 'B');
	if(count($ids)>=3){
		imagettftext($img, $text_size+5, 0, $width/2+80, $height-10,$background, $fontfile, 'C');
	}
	
	
	$flag = Imagepng($img,drupal_realpath($destination));
	
	ImageDestroy($img);		
	// this part is for black white version
	header ("Content-type: image/png");
	$img=imagecreatetruecolor($width,$height);
	imagecopyresampled($img,$srcimage_bw,0,0,0,0, $width,$height,$width,$height);
	
	$background = imagecolorallocate ($img,  0, 0, 0);	
//    	     						
    $text_size =11;
	
 
    for ($t=0;$t<count($map);$t++)
    {
    	$number = count($gene_sets[$t]);
		imagettftext($img, $text_size, 0, $map[$t]['x1']/2+$map[$t]['x2']/2-$text_size, 
						$map[$t]['y1']/2+ $map[$t]['y2']/2-$text_size/2,$background, $fontfile, $number);
    }
	imagettftext($img, $text_size+5, 0, 20, 30,$background, $fontfile, 'A');
	imagettftext($img, $text_size+5, 0, $width-20, 30,$background, $fontfile, 'B');
	if(count($ids)>=3){
		imagettftext($img, $text_size+5, 0, $width/2+80, $height-10,$background, $fontfile, 'C');
	}
	
	
	$flag = Imagepng($img,drupal_realpath($bw_destination));
	
	ImageDestroy($img);	
	
	return array('destination'=> ($destination), 'bw_destination'=> ($bw_destination),	'height'=>332, 'width'=>346, 'map'=> $map, 'title'=>$title, 'query_set'=>$gene_sets);
}