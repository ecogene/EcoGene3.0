<?php 

function ecogene_spanup_form($form, &$form_state) {
	  	$form['#tree'] = TRUE;
	
   
  $form['file_field'] = array(
	    '#type' => 'fieldset',
  		'#collapsible' => false,
  		'#title'=>'Gene Interval Updater'	, 
  		'#prefix' => '<div id="crossref_uploadfile-controls">
  		Gene Interval Updater is a tool that can be used to convert U00096.2 gene (and feature) genomic addresses to U00096.3 coordinates.
  		',
    	'#suffix' => '</div>', 		
    );
   
	
  $form['file_field']['original'] = array(
  	'#type' => 'select',
  	'#title' => t('Select Original Interval System'),
	'#default_value' => variable_get('database_name','ECOLI_2'),
  	'#options' => array(
    	'ECOLI_2' => t('E. coli K-12 MG1655 U00096.2'),
  		'ECOLI_1' => t('E. coli K-12 MG1655 U00096.1'),
  	),
  	
	);
	
	$form['file_field']['target'] = array(
  	'#type' => 'select',
  	'#title' => t('Select Target Interval System'),
	'#default_value' => variable_get('database_name','ECOLI_3'),
  	'#options' => array(
    	'ECOLI_3' => t('E. coli K-12 MG1655 U00096.3'),
  		'ECOLI_2' => t('E. coli K-12 MG1655 U00096.2'),
  	),
  	
	);
    
    $form["file_field"]["uploadfile"] = array(
   		'#attributes' => array('enctype' => "multipart/form-data"),
     	'#type' => 'file',
    	'#size' =>40,
    	'#name' => "files[geneinterval_file_uploadfile]",
    	'#title' => 'Upload interval file',
  		'#description'=>'Interval file can be text(.txt) or csv(.csv) file. The program will update all the numeric columns in the file.',
	
    );

    
  

    $form['mapping_field']['search'] = array(
  		'#value'=>t('Submit'),
	    '#type' => 'submit',
  		'#ajax' => array(
      		'callback' => 'ajax_ecogene_spanup_callback',
  			'wrapper' => 'spanup_result_controls',
    	),	    	
 	);
   
	$form['mapping_result']['table'] = array(
     	'#prefix' => '<div id="spanup_result_controls">',
    	'#suffix' => '</div>',
    );
	
    
    
    return $form;
 
}




function geneinterval_file_upload($form, &$form_state) {

	
	$uploadfilefield = "geneinterval_file_uploadfile";
		
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

		if(empty($row)){
			continue;	
		} 
		    
	    $ids[] = $row;
    
	}  
	
  }
  
  fclose($fh);

  return $ids;
}
function SpanUp($pos_old, $change_t)
{

	$numChange = count($change_t);

	if($pos_old<$change_t[0][0])
	{$pos_new=$pos_old;}

	else if($pos_old< $change_t[$numChange-1][0])
	{
		for($i=0; $i<$numChange-1; $i++)
		{
			if (($pos_old>=$change_t[$i][0]) && ($pos_old<$change_t[$i+1][0]))
			{
				$pos_new = $pos_old + $change_t[$i][1];

				break;
			}
		}
	}

	else
	{
		$pos_new = $pos_old + $change_t[$numChange-1][1];

	}
	return $pos_new;
}
function ajax_ecogene_spanup_callback($form, &$form_state) {
	global $base_url;
	
	$coor_change_info_2_to_3 = array(
		array( 257908, 776),
		array( 547833, 777),
		array( 1298722, 1976),
		array( 2171387, 1978),
		array( 3558478, 1977)
	);

	$coor_change_info_1_to_2 = array(
	array( 1980940, 1),
	array( 2008480, 2),
	array( 2728587, 1),
	array( 2729373, 2),
	array( 2747389, 1),
	array( 2781020, 2),
	array( 2785180, 1),
	array( 2812859, 0),
	array( 2820144,-1),
	array( 2838044, 0),
	array( 2838055,-1),
	array( 2896819, 0),
	array( 3005399, 1),
	array( 3025676, 2),
	array( 3075220, 3),
	array( 3077514, 2),
	array( 3077640, 3),
	array( 3099661, 4),
	array( 3107469, 5),
	array( 3115116, 6),
	array( 3123585, 7),
	array( 3152247, 8),
	array( 3156257, 5),
	array( 3158202, 6),
	array( 3192847, 381),
	array( 3224879, 380),
	array( 3224926, 381),
	array( 3236177, 380),
	array( 3236735, 381),
	array( 3236758, 380),
	array( 3237242, 381),
	array( 3237247, 382),
	array( 3253742, 381),
	array( 3255594, 380),
	array( 3255616, 379),
	array( 3256893, 380),
	array( 3257838, 381),
	array( 3349263, 387),
	array( 3389812, 386),
	array( 3393106, 385),
	array( 3418840, 386),
	array( 3421120, 385),
	array( 3421712, 386),
	array( 3421872, 387),
	array( 3421879, 386),
	array( 3422485, 385),
	array( 3429527, 384),
	array( 3430453, 385),
	array( 3500309, 384),
	array( 3509958, 383),
	array( 3510121, 384),
	array( 3537405, 386),
	array( 3544357, 387),
	array( 3544360, 388),
	array( 3544363, 389),
	array( 3554088, 390),
	array( 3573700, 391),
	array( 3575446, 392),
	array( 3627142, 393),
	array( 3686775, 394),
	array( 3743603, 393),
	array( 3754317, 396),
	array( 3870631, 395),
	array( 3870765, 396),
	array( 3870876, 395),
	array( 3872450, 396),
	array( 3873627, 395),
	array( 3881483, 394),
	array( 3896324, 395),
	array( 3927062, 396),
	array( 3930457, 395),
	array( 3930568, 396),
	array( 3930677, 397),
	array( 3938584, 400),
	array( 3940332, 401),
	array( 3941734, 400),
	array( 3943707, 399),
	array( 3943773, 400),
	array( 3944125, 399),
	array( 3945351, 400),
	array( 3951166, 401),
	array( 3951168, 402),
	array( 3957092, 404),
	array( 3957124, 405),
	array( 3957549, 407),
	array( 3957560, 408),
	array( 3968317, 411),
	array( 3975313, 410),
	array( 3979535, 411),
	array( 3979545, 410),
	array( 3998290, 411),
	array( 4001717, 412),
	array( 4004979, 413),
	array( 4004984, 415),
	array( 4005601, 416),
	array( 4006483, 417),
	array( 4007907, 419),
	array( 4008375, 420),
	array( 4008391, 421),
	array( 4008398, 425),
	array( 4008402, 426),
	array( 4008410, 427),
	array( 4008421, 428),
	array( 4008429, 430),
	array( 4008469, 432),
	array( 4008525, 433),
	array( 4013175, 434),
	array( 4013717, 435),
	array( 4013772, 436),
	array( 4019860, 434),
	array( 4021684, 433),
	array( 4031984, 434),
	array( 4035072, 436),
	array( 4035075, 443),
	array( 4039960, 442),
	array( 4041671, 443),
	array( 4051144, 442),
	array( 4051170, 443),
	array( 4057759, 444),
	array( 4061961, 443),
	array( 4085675, 442),
	array( 4091920, 443),
	array( 4091926, 442),
	array( 4091927, 441),
	array( 4092012, 442),
	array( 4103519, 443),
	array( 4139669, 444),
	array( 4175949, 445),
	array( 4207465, 444),
	array( 4311496, 445),
	array( 4435684, 446),
	array( 4448541, 447),
	array( 4448543, 448),
	array( 4449738, 449),
	array( 4474826, 450),
	array( 4474848, 458),
	array( 4474857, 459),
	array( 4474866, 458),
	array( 4474868, 460),
	array( 4477041, 459),
	array( 4477068, 460),
	array( 4477281, 459),
	array( 4477321, 458),
	array( 4478503, 457),
	array( 4478511, 456),
	array( 4478527, 455),
	array( 4542643, 454),
	array( 4573190, 453),
	array( 4593336, 454));
	
	$input_lines = geneinterval_file_upload($form, &$form_state);
		
	$rows = array();
	if(count($input_lines)>0){
			foreach ($input_lines as $line){
				
				foreach ($line as $_key => $f) {
					if(is_numeric($f)){
						if($form_state['values']['file_field']['original']=='ECOLI_2' && 
										$form_state['values']['file_field']['target']=='ECOLI_3')
						{
							$new_coor = SpanUp($f,$coor_change_info_2_to_3);	
							$line[$_key] = $new_coor;
								
						}elseif($form_state['values']['file_field']['original']=='ECOLI_1' && 
										$form_state['values']['file_field']['target']=='ECOLI_2')
						{
							$new_coor = SpanUp($f,$coor_change_info_1_to_2);	
							$line[$_key] = $new_coor;
						}elseif($form_state['values']['file_field']['original']=='ECOLI_1' && 
										$form_state['values']['file_field']['target']=='ECOLI_3')
						{
							$new_coor = SpanUp($f,$coor_change_info_1_to_2);	
							$new_coor = SpanUp($new_coor,$coor_change_info_2_to_3);
							$line[$_key] = $new_coor;
						}	
					}
				
				}
				
				$rows[] = $line;
			}
		}
			
//$markup = $markup = print_r($table_header,true).print_r($all_genes_xref,true);
//	$markup='Results:';
	db_set_active("default");
	
	$cache_id = 'xref_'.time();
	

  	$csv_data = ecogene_download_store_data('ecodownload_'.$cache_id.'_csv', $rows,'csv');
  	$txt_data = ecogene_download_store_data('ecodownload_'.$cache_id.'_txt', $rows,'txt');  	
  	

  	$element =$form['mapping_result']['table'];
//	$markup2 = ' gene(s) are found'.'<br />';
//	
//	$markup2 .= '<br />';
//  	$element['text'] = array(	
//		'#markup' => $markup2,
//		);
//		
		

	$element['links']['export'] = array(
		
    	'#markup' => '<strong>'.t('Download Result in : ').'</strong>'. l(t(' TXT file '), "ecodownload/report/".$txt_data['report']."/".$txt_data['user']."/txt").
   					 t(' or ').l(t('CSV file'), "ecodownload/report/".$csv_data['report']."/".$csv_data['user']."/csv"),
   		
  	);


	$commands[] = ajax_command_replace("#spanup_result_controls", drupal_render($element));

   return array('#type' => 'ajax', '#commands' => $commands);
}

