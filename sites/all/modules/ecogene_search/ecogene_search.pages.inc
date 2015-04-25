
<?PHP
include("sites/all/include/ecoFunction.php");
include("sites/all/include/format_description.php");
include("sites/all/include/unicode.php");
define ("SEQ_LINE", 75);
function get_search_gene_conditions($array = array(),&$query)
{
//	print_r($array);
	global $GENOME_LENGTH;
	$geneTable='t_gene';
	$synonymTable = 't_gene_synonym';
	$biodatabaseTable = 't_biodatabase_link';
	$addressTable = 't_address';
	$proteinTable = 't_product_protein';
	$rnaTable = 't_product_rna';
	$topicGeneTable = 't_topic_gene_link';
	$refGeneTable = 't_gene_reference_link';
	
	$goGeneTable = 't_go_gene_link';
	// Dynamically compose a SQL query:
	
  	$params = array();
//	print_r($_REQUEST);
	
  	if(!empty($array['go']))
  	{
  		
    					
    	$eg_ids = db_select($goGeneTable,$goGeneTable)
    						->fields($goGeneTable,array('eg_id'))
    						->condition($goGeneTable.'.accession_id', trim($array['go']), '=')
   							->execute()->fetchCol();   							
   		if(!empty($eg_ids))
   		{
			$query->condition($geneTable.'.eg_id', $eg_ids ,'IN');
   		}else 
   		{
   			$query->condition($geneTable.'.eg_id', '');
   		}  						
    	
    	return ;
  	}
  	
	foreach ($array as $key => $value) {
		$value = preg_replace('/[^(\x20-\x7F)]*/','', $value);
		if($value!='')
    	{	
    		$params_sub = array();
    		$value = trim($value);
    		switch ($key)
    		{
    			case 'upload_ids':
    				$params_sub = preg_split("/[.,; ]+/",$value);
				$gene_primary_names = db_select($geneTable,$geneTable)
    						->fields($geneTable,array('name'))
    						->condition($geneTable.'.name', '', '<>')
   							->execute()->fetchCol();
    				
   					$params_sub_str = $params_sub;
    						foreach ($params_sub_str as $ind=>$value) {
    							$params_sub_str[$ind] = "'".$value."'";
    				}     
    						
    				$OR = db_or()->condition($biodatabaseTable.'.accession_id', $params_sub,'IN')
    				->where("( SUBSTRING_INDEX(t_biodatabase_link.accession_id, '.', 1)) IN (". implode(',', $params_sub_str).")")
    									->condition($geneTable.'.eg_id', $params_sub, 'IN')
    									->condition($geneTable.'.name', $params_sub, 'IN')
									->condition(db_and()->condition($synonymTable.'.name', $params_sub, 'IN')->condition($synonymTable.'.name',$gene_primary_names, 'NOT IN'))
    									->condition($geneTable.'.eck_id', $params_sub, 'IN');
//    				print_r($params_sub);
    				$query->condition($OR);
    				break;
    				
    			case 'main_search':		
    					
    					$params_sub = preg_split("/[,; ]+/",$value);
    					if(!empty($params_sub))
    					{
    						$params_sub_str = $params_sub;
    						foreach ($params_sub_str as $ind=>$value) {
    							$params_sub_str[$ind] = "'".$value."'";
    						}     		   						
    					
    						$OR = db_or()->condition($biodatabaseTable.'.accession_id', $params_sub,'IN')
    								->where("( SUBSTRING_INDEX(t_biodatabase_link.accession_id, '.', 1)) IN (". implode(',', $params_sub_str).")")
    									->condition($geneTable.'.eg_id', $params_sub, 'IN')
    									->condition($geneTable.'.eck_id', $params_sub, 'IN');
    						 
    						foreach ($params_sub as $value) {
							  $OR = $OR ->condition($geneTable.'.name', $value.'%%', 'LIKE')
    								->condition($synonymTable.'.name', $value.'%%', 'LIKE');
								
    						}
    						$query->condition($OR);
    					}
    				
    				break;
    				
    			case 'name':
    					$params_sub = preg_split("/[.,; ]+/",$value);
    					if(!empty($params_sub))
    					{
    						 $OR = db_or();
    						foreach ($params_sub as $value) {
							  $OR = $OR ->condition($geneTable.'.name', $value.'%%', 'LIKE');
							  if(isset($array['synonyms']))
							  {
    						 	$OR = $OR ->condition($synonymTable.'.name', $value.'%%', 'LIKE');
							  }
								
    						}
    						$query->condition($OR);
    						
    					}    					
    				break;
    			case 'prot_name':		
    				$params_sub = preg_split("/[\s,]*\\\"([^\\\"]+)\\\"[\s,]*|" . "[\s,]*'([^']+)'[\s,]*|" . "[\s,]+/", $value, 0, PREG_SPLIT_NO_EMPTY | PREG_SPLIT_DELIM_CAPTURE);
    					if(!empty($params_sub))
    					{
    						
    							$AND = db_and();
    							foreach ($params_sub as $value) {
    								$value = trim($value,"\"");
    								if(!empty($value))
							  		$AND = $AND ->condition($proteinTable.'.name', '%%'.$value.'%%', 'LIKE');
								}
    							$query->condition($AND);    						  						
    					}
    				
    				break; 
    			case 'v_minute':
    				
    				$name = trim($value);
    				$b = substr($name, 0, 1);

					if (!(ord($b)<=57 && ord($b)>=48)) /*  Jump by gene's name  */
    				{
    					$params_sub = preg_split("/[,; ]+/",$value);
    					if(!empty($params_sub))
    					{
    						$OR = db_or();
    						foreach ($params_sub as $value) {
							  $OR = $OR ->condition($geneTable.'.name', $value.'%%', 'LIKE');    								
								
    						}
    						$query->condition($OR);
//							$query->condition($geneTable.'.name', $value.'%%', 'LIKE');
    					}
						  
    				}else {
    					$left_min = $value*$GENOME_LENGTH/100;
    					$query ->condition($addressTable.'.left_end', $left_min, '<=');
    					$query ->condition($addressTable.'.right_end', $left_min, '>=');    				
    				}
					
    				break;		
    				
    			case 'egid':
    					$params_sub = preg_split("/[.,; ]+/",$value);
    					
    					if(!empty($params_sub))
    					{
    						$query->condition($geneTable.'.eg_id', $params_sub, 'IN');    						
    					}
    					
    				break;		
    			case 'eckid':		
    					$params_sub = preg_split("/[.,; ]+/",$value);
    					if(!empty($params_sub))
    					{
    						$query->condition($geneTable.'.eck_id', $params_sub, 'IN');    						
    					}   				
    				break;  
    				
    			case 'orientation':	
    				$query ->condition($addressTable.'.orientation', $value,'=');
    				break;
    				
    			case 'rightEnd':	
    				$query ->condition($addressTable.'.left_end', $value,'<=');	 				
    				break;
    				
    			case 'leftEnd':		  
    				$query ->condition($addressTable.'.right_end', $value,'>=');	    				
    				break;
    				
    			case 'p_maximum':		
    				if($array['p_measure']=='p_mw')  
    				{ 				
    					$query->condition(db_or()->condition($proteinTable.'.mw', intval($value), '<=')
    						->condition($rnaTable.'.mw', intval($value), '<='));
    			
    				}elseif ($array['p_measure']=='p_length')
    				{
    					$params_length = array(	':length_11'=> intval(trim($value)),
    											':length_12'=> intval(trim($value)));
    					$query->where(" ( case $geneTable.type 
    									  when 'nt' then 
    									 ($addressTable.right_end - $addressTable.left_end + 1)<= :length_11 
    									 when 'aa' then 
    									 char_length($proteinTable.sequence)<= :length_12  end ) ", $params_length);
    					
    				}
    				break;
    				
    			case 'p_minimum':		
    				if($array['p_measure']=='p_mw')  
    				{ 		
    					$query->condition(db_or()->condition($proteinTable.'.mw', intval($value), '>=')
    						->condition($rnaTable.'.mw', intval($value), '>='));
    						
    				}elseif ($array['p_measure']=='p_length')
    				{
    					$params_length = array(	':length_21'=> intval(trim($value)),
    											':length_22'=> intval(trim($value)));
    					$query->where(" ( case $geneTable.type 
    									  when 'nt' then 
    									 ($addressTable.right_end - $addressTable.left_end + 1)>= :length_21 
    									 when 'aa' then 
    									 char_length($proteinTable.sequence)>= :length_22  end ) ", $params_length);
    				}
    				break;	
    				
    		   				
    			case 'type':		
    				$query ->condition($geneTable.'.type', $value,'=');
    				    				
    				break;  
    				
    			case 'accessionId':		
    				$params_sub = preg_split("/[,; ]+/",$value);
    				
    					if(!empty($params_sub))
    					{
    						$params_sub_str = $params_sub;
    						foreach ($params_sub_str as $ind=>$value) {
    							$params_sub_str[$ind] = "'".$value."'";
    						}    						
     		   						$query->condition(db_or()
    								->condition($biodatabaseTable.'.accession_id', $params_sub, 'IN')
    								->where("( SUBSTRING_INDEX(t_biodatabase_link.accession_id, '.', 1)) IN (". implode(',', $params_sub_str).")")
    								);    
    												
    					}       				
    				break;
    					
    					
    			case 'description':		
//    				$params_sub = preg_split("/[.,; \"]+/",$value, -1, PREG_SPLIT_NO_EMPTY);
    				$params_sub = preg_split("/[\s,]*\\\"([^\\\"]+)\\\"[\s,]*|" . "[\s,]*'([^']+)'[\s,]*|" . "[\s,]+/", $value, 0, PREG_SPLIT_NO_EMPTY | PREG_SPLIT_DELIM_CAPTURE);
    					if(!empty($params_sub))
    					{
    						if(isset($array['description_or']))
    						{    							
    						  	$OR = db_or();
    						  	foreach ($params_sub as $value) {
    						  		$value = trim($value,"\"");
    						  		if(!empty($value))
							  		$OR = $OR ->condition($geneTable.'.description', '%%'.$value.'%%', 'LIKE');
								}
    							$query->condition($OR);
    						}else {
    							$AND = db_and();
    							foreach ($params_sub as $value) {
    								$value = trim($value,"\"");
    								if(!empty($value))
							  		$AND = $AND ->condition($geneTable.'.description', '%%'.$value.'%%', 'LIKE');
								}
    							$query->condition($AND);
    						}    						
    					}
    				
    				break; 
    			case 'comments':	
//    					$params_sub = preg_split("/[.,; ]+/",$value);
    					$params_sub = preg_split("/[\s,]*\\\"([^\\\"]+)\\\"[\s,]*|" . "[\s,]*'([^']+)'[\s,]*|" . "[\s,]+/", $value, 0, PREG_SPLIT_NO_EMPTY | PREG_SPLIT_DELIM_CAPTURE);
    					if(!empty($params_sub))
    					{
    						  $OR = db_and();
    						foreach ($params_sub as $value) {
							  $OR = $OR ->condition($geneTable.'.comments', '%%'.$value.'%%', 'LIKE');
								
    						}
    						$query->condition($OR);
    					}	
    				break;  
    			case 'pubmedID':
    				
    				$eg_ids = db_select($refGeneTable,$refGeneTable)
    						->fields($refGeneTable,array('eg_id'))
    						->condition($refGeneTable.'.pubmed_id', $value, '=')
   							->execute()->fetchCol();
   					
   					if(!empty($eg_ids))
   					{
						$query->condition($geneTable.'.eg_id', $eg_ids ,'IN');
   					}else 
   					{
   						$query->condition($geneTable.'.eg_id', '');
   					}  	
    				break;		
    			case 'search_topic':		
    				$eg_ids = db_select('t_topic_gene_link','t_topic_gene_link')
    						->fields('t_topic_gene_link',array('eg_id'))
    						->condition($topicGeneTable.'.topic_id', $value, '=')
   							->execute()->fetchCol();
   					
   					if(!empty($eg_ids))
   					{
						$query->condition($geneTable.'.eg_id', $eg_ids ,'IN');
   					}else 
   					{
   						$query->condition($geneTable.'.eg_id', '');
   					} 
  							
    				break;
    			case 'topic_ids':	
    				
    				$params_sub = $array[$key];
    				$query_type = $array['topic_gene_query_logic'];
    				if( stristr($query_type, 'any') )
    				{
    					$sub_query = db_or();
    				}elseif(stristr($query_type, 'all'))
    				{
    					$sub_query = db_and();
    				}elseif(stristr($query_type, 'not'))
    				{
    					$sub_query = db_and();
    				}

    				foreach ($params_sub as $value) {
    						$eg_ids = db_select('t_topic_gene_link','t_topic_gene_link')
    						->fields('t_topic_gene_link',array('eg_id'))
    						->condition($topicGeneTable.'.topic_id', $value, '=')
   							->execute()->fetchCol();
   						if(!empty($eg_ids))
   						{
   							if(stristr($query_type, 'not'))
   							{
   								$sub_query = $sub_query->condition($geneTable.'.eg_id', $eg_ids ,'NOT IN');
   							}
							else 
							{
								$sub_query = $sub_query->condition($geneTable.'.eg_id', $eg_ids ,'IN');
							}
   						}else 
   						{
   							continue;
   						} 
							
								
    				}
    				
    				if(array_key_exists('array_exp_ids', $array))
    				{
    					$params_sub = $array['array_exp_ids'];
    					foreach ($params_sub as $value) {
    						$value_array = explode(',', trim($value));
    						$eg_ids = db_select('t_array_data','t_array_data')
    							->fields('t_array_data',array('eg_id'))
    							->condition('t_array_data.pubmed_id', $value_array[0], '=')
    							->condition('t_array_data.exp_id', $value_array[1], '=')
   								->execute()->fetchCol();
   							if(!empty($eg_ids))
   							{
   								if(stristr($query_type, 'not'))
   								{
   									$sub_query = $sub_query->condition($geneTable.'.eg_id', $eg_ids ,'NOT IN');
   								}
								else 
								{
									$sub_query = $sub_query->condition($geneTable.'.eg_id', $eg_ids ,'IN');
								}
   							}					
    					}
    				}
    				$query->condition($sub_query);
    						
							
    				break;	
    				
    			case 'array_exp_ids':	
    				if( (array_key_exists('topic_ids', $array)) && 
    					!empty($array['topic_ids']))	
    				{
    						break;
    				}
    				$params_sub = $array[$key];
    				$query_type = $array['topic_gene_query_logic'];
    				if( stristr($query_type, 'any') )
    				{
    					$sub_query = db_or();
    				}elseif(stristr($query_type, 'all'))
    				{
    					$sub_query = db_and();
    				}elseif(stristr($query_type, 'not'))
    				{
    					$sub_query = db_and();
    				}else
    				{
    					if(!is_array($params_sub))
    					{
    						$value_array = explode(',', trim($params_sub));
    					
    						$eg_ids = db_select('t_array_data','t_array_data')
    						->fields('t_array_data',array('eg_id'))
    						->condition('t_array_data.pubmed_id', $value_array[0], '=')
    						->condition('t_array_data.exp_id', $value_array[1], '=')
   							->execute()->fetchCol();
   							
   							
    						$query->condition($geneTable.'.eg_id', $eg_ids ,'IN');
    					}
    				}

    				
    				if(is_array($params_sub)){
    				foreach ($params_sub as $value) {
    						$value_array = explode(',', trim($value));
    					
    						$eg_ids = db_select('t_array_data','t_array_data')
    						->fields('t_array_data',array('eg_id'))
    						->condition('t_array_data.pubmed_id', $value_array[0], '=')
    						->condition('t_array_data.exp_id', $value_array[1], '=')
   							->execute()->fetchCol();
   						if(!empty($eg_ids))
   						{
   							if(stristr($query_type, 'not'))
   							{
   								$sub_query = $sub_query->condition($geneTable.'.eg_id', $eg_ids ,'NOT IN');
   							}
							else 
							{
								$sub_query = $sub_query->condition($geneTable.'.eg_id', $eg_ids ,'IN');
							}
   						}else 
   						{
   							continue;
   						} 
							
								
    				}
    				$query->condition($sub_query);
    				}
    						
							
    				break;		
			}
		}
	}
	
//echo $query->__toString();
	
}
function get_search_reference_conditions($array = array(),&$query)
{
	$refTable='t_reference';
	$authorTable = 't_author';
	$refAuthorTable = 't_reference_author_link';
//	print_r($array);
	foreach ($array as $key => $value) {
		$value = trim($value);
		if($value!='')
    	{	
    		switch ($key)
    		{
    			case 'main_search':
    				$params_sub = preg_split("/[.,; ]+/",$value);
//    				print_r($params_sub);
    					if(!empty($params_sub))
    					{
    						
    						$OR = db_or() -> condition($refTable.'.pubmed_id', $params_sub, 'IN');
//    								-> condition($refTable.'.medline_id', $params_sub, 'IN'); 
    						$subquery_title = db_and();		   						 
    						foreach ($params_sub as $value_sub) {
								$subquery_title ->condition($refTable.'.title', '%%'.$value_sub.'%%', 'LIKE');
    																
    						}   				
    						$OR ->condition($subquery_title);
    					}
    				
    					$query->condition($OR);
    				 	
    				
    				break;	
    			case 'pubmed_ids_array':
    				$params_sub = preg_split("/[.,; ]+/",$value);
//    				print_r($params_sub);
    				$query->condition($refTable.'.pubmed_id', $params_sub, 'IN');
    				break;
    			case 'pubmedId':
    				$params_sub = preg_split("/[.,; ]+/",$value);
    				if ( $array['idtype'] == "pubmed" ){    					
    					if(!empty($params_sub))
    					{
    						$query->condition($refTable.'.pubmed_id', $params_sub, 'IN');
    					}   
    				}
					elseif($array['idtype'] == "medline"){
						if(!empty($params_sub))
    					{
    						$query->condition($refTable.'.medline_id', $params_sub, 'IN');
    					}   
    					
					}
    				   				
    				break;		
    				
    			case 'title':	
//    					$params_sub = preg_split("/[.,; ]+/",$value);
						$params_sub = preg_split("/[\s,]*\\\"([^\\\"]+)\\\"[\s,]*|" . "[\s,]*'([^']+)'[\s,]*|" . "[\s,]+/", $value, 0, PREG_SPLIT_NO_EMPTY | PREG_SPLIT_DELIM_CAPTURE);
    					if(!empty($params_sub))
    					{
    						$OR = db_and();    						 
    						foreach ($params_sub as $value) {
							  $OR = $OR ->condition($refTable.'.title', '%%'.$value.'%%', 'LIKE');
    																
    						}
    						$query->condition($OR);
    					}   				
    				break; 
    			case 'initials':    				 
							$value = strtolower($value);
    						$author_names = preg_split("/and/",$value);
    						$pubmed_ids = array();
    						
    						foreach ($author_names as $aname) {
    							$aname = trim($aname);
    						
    							$params_sub = preg_split("/[.,; ]+/",$aname);
    							if(!empty($params_sub))
    							{    			
    							 						
  									$_query = db_select($refAuthorTable,$refAuthorTable)		
    												->fields($refAuthorTable,array('pubmed_id'));
									$_query ->leftJoin($authorTable,$authorTable, $refAuthorTable.'.author_id'.' = '.$authorTable.'.author_id');
									
   									
										
    								
									if(count($params_sub)>=2) {
       												  
    									$_query -> condition( db_or()
    															->condition(db_and()->condition($authorTable.'.initials', $params_sub[1].'%%', 'LIKE')
    								 												->condition($authorTable.'.search_last_name', $params_sub[0], 'LIKE')
    								 										)
    															->condition(db_and()->condition($authorTable.'.initials', $params_sub[0].'%%', 'LIKE')
    								 												->condition($authorTable.'.search_last_name', $params_sub[1], 'LIKE')
    								 										)
    								 							->condition($authorTable.'.search_last_name', $params_sub[0].' '.$params_sub[1])
    								 													
    								 					);
   						
    								}else {
    									$_query	-> condition($authorTable.'.search_last_name', $params_sub[0], '=');													
										
    								}
    								$_query ->groupBy($refAuthorTable.'.pubmed_id');
    								$_pubmed_ids = $_query ->execute()->fetchCol();	
									$pubmed_ids = empty($pubmed_ids)? $_pubmed_ids : array_intersect ($pubmed_ids,$_pubmed_ids);								
										
    							}
    						}  
    						
    						$pubmed_ids = empty($pubmed_ids)? array('-1'):$pubmed_ids;
    						$query -> condition($refTable.'.pubmed_id', $pubmed_ids, 'IN');
    				break; 	
    				
    			case 'abstract':		
    				//$params_sub = preg_split("/[.,; ]+/",$value);
    				$params_sub = preg_split("/[\s,]*\\\"([^\\\"]+)\\\"[\s,]*|" . "[\s,]*'([^']+)'[\s,]*|" . "[\s,]+/", $value, 0, PREG_SPLIT_NO_EMPTY | PREG_SPLIT_DELIM_CAPTURE);
    					if(!empty($params_sub))
    					{
    						$OR = db_and();    						 
    						foreach ($params_sub as $value) {
							  $OR = $OR ->condition($refTable.'.abstract', '%%'.$value.'%%', 'LIKE');
    																
    						}
    						$query->condition($OR);
    					}   				
    				break; 
    				
    			case 'journal':		
    				$params_sub = preg_split("/[.,; ]+/",$value);
    					if(!empty($params_sub))
    					{
    						$OR = db_or();    						 
    						foreach ($params_sub as $value) {
							  $OR = $OR ->condition($refTable.'.journal', '%%'.$value.'%%', 'LIKE');
    																
    						}
    						$query->condition($OR);
    					}				
    				break; 
    				
    			case 'fromYear':		
    				$query ->where('year(date_published) >= :fromyear', array(':fromyear'=>$value));    				
    				
    				break;
    			case 'toYear':		    
    				$query ->where('year(date_published)<= :toyear', array(':toyear'=>$value));  
//    				$query ->where('year_published', $value, '<=');				
    				
    				break;
    		}
    	}
	}
//	echo $query->__toString();
	
}
function get_search_reference_author()
{
	$authorTable = 't_author';
	$refAuthorTable='t_reference_author_link';
	$regs=preg_split("/[,;]+[ ]*/",$array['initials']);
	$query_sub=array();
	$params_sub=array();
	foreach ($regs as $value_sub) {
		$name = explode(" ",trim($value_sub));
		if(count($name)>0){
			if(strlen($name[1])>0)
			{
				$query_sub[] = " ($authorTable.search_last_name like '%%%s%%' and initials like '%%%s%%' ) ";
				$params_sub[]=$name[0]; $params_sub[]=$name[1];
			}
			else
			{
				$query_sub[] = " ($authorTable.search_last_name like '%%%s%%') ";
				$params_sub[]=$name[0];
			}
		}
	}
	$conditions = 'where ( '. implode(' or ', $query_sub).' )';
	$author_ids_result = db_query("SELECT  t_author.author_id from $authorTable $conditions", $params_sub);
	while ($item_author_ids = db_fetch_object($author_ids_result)) {
		$author_ids[] = $item_author_ids->author_id;			

	}
	if(count($author_ids)>0)
	{
		$author_ids_result = db_query("SELECT  $refAuthorTable.pubmed_id from $refAuthorTable where $refAuthorTable.s $conditions", $params_sub);
	}
	
}
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
//
function search_gene_return() {
	global $GENOME_LENGTH;
	global $base_url;
	
	$request = $_REQUEST;
	$userfile_ref  = $_FILES['userfile'];

	if ($userfile_ref['name'] != "")		
	{

			$uploadedfile = $userfile_ref['tmp_name'];


			$fp = fopen($uploadedfile, 'r');

			if (!$fp) {

				echo 'Could not open the file ';
			}
			
			$upload_ids = "";
			while (false !== ($string = fgets($fp)))
			{

				$items = explode (" ",$string);

				foreach ($items as $item) {

					// Remove and leading and lagging spaces.

					$item = trim($item);
					$upload_ids .=  $item.',';

				}
				
			}
			if(strlen($upload_ids)>1)
					$upload_ids = strrev(substr(strrev($upload_ids),1));
			$request['upload_ids'] = $upload_ids;

			$request['upload_ids_file'] = true;
			$_SESSION['search_upload_ids'] = $upload_ids;
			
	}elseif(!isset($request['upload_ids_file'])){
		
		unset($_SESSION['search_upload_ids']);
	}elseif(!empty($_SESSION['search_upload_ids'])){
		
		$request['upload_ids'] = $_SESSION['search_upload_ids'];
	}
	
	if($_POST)
    {
    	if($_POST['submit']=='Search Go'){
    		drupal_goto('ecosearch/gene/search',array('query'=>array('go'=>$_POST['go'],'item_per_page'=>10)));
    	}
		$request_url = drupal_get_query_parameters($request, array_merge(array('MAX_FILE_SIZE','upload_ids')));
		drupal_goto('ecosearch/gene/search',array('query'=>$request_url));
    }	
	$item_per_page = isset($request['item_per_page'])? $request['item_per_page']: 10;
	$request['item_per_page'] = $item_per_page;
	
	$header = array(
	array('data' => t('Name'), 'field' => 't_gene.name'),
	array('data' => t('Synonym(s)'), 'field' => 't_gene_synonym.name'),
	array('data' => t('Description'), 'field' => 't_gene.description'),
	array('data' => t('Centisome'), 'field' => 't_address.left_end'),
	array('data' => t('Product Seq'), 'field' => 't_gene.type'),
	array('data' => t('EG ID'), 'field' => 't_gene.eg_id'),
	array('data' => t('b#'), 'field' => 't_biodatabase_link.accession_id'),	
	t('Links')
	);
	
	// Show the protein name clolumn if it is searched
	if (isset($request['prot_name']) && !empty($request['prot_name']))
	{
		$header = array(
		array('data' => t('Name'), 'field' => 't_gene.name'),
		array('data' => t('Synonym(s)'), 'field' => 't_gene_synonym.name'),
		array('data' => t('Description'), 'field' => 't_gene.description'),
		array('data' => t('Protein Name'), 'field' => 't_product_protein.name'),
		array('data' => t('Centisome'), 'field' => 't_address.left_end'),
		array('data' => t('Product Seq'), 'field' => 't_gene.type'),
		array('data' => t('EG ID'), 'field' => 't_gene.eg_id'),
		array('data' => t('b#'), 'field' => 't_biodatabase_link.accession_id'),	
		t('Links')
		);
	}
	
	db_set_active("ecogene");
	
	$query = db_select('t_gene','t_gene')-> extend('PagerDefault')-> extend('TableSort') ;
	$query -> leftJoin('t_biodatabase_link', 't_biodatabase_link_2', 't_biodatabase_link_2.eg_id=t_gene.eg_id');
	
	$query -> leftJoin('t_gene_synonym', 't_gene_synonym', 't_gene_synonym.eg_id=t_gene.eg_id');
	$query -> leftJoin('t_product_protein', 't_product_protein', 't_product_protein.eg_id=t_gene.eg_id');
	$query -> leftJoin('t_product_rna', 't_product_rna', 't_product_rna.eg_id=t_gene.eg_id');
	$query -> leftJoin('t_biodatabase_link', 't_biodatabase_link', 't_biodatabase_link.eg_id=t_gene.eg_id');
	$query -> leftJoin('t_address', 't_address', 't_address.address_id=t_gene.address_id');
	
	
	$query->fields('t_gene');		
	$query-> addField('t_gene_synonym', 'name', 'synonym');
	$query-> addField('t_address', 'orientation', 'orientation');
	$query-> addField('t_address', 'left_end', 'left_end');
	$query-> addField('t_biodatabase_link_2', 'accession_id', 'accession_id');
	$query-> addField('t_product_protein', 'name', 'prot_name');
	
	$query -> condition('t_gene.name', '', '<>');
//	$query -> condition('t_biodatabase_link.biodatabase_id', 3);
	$query -> condition('t_biodatabase_link_2.biodatabase_id', 3);

	get_search_gene_conditions($request,$query);
	$query -> groupBy('t_gene.eg_id');

	$query_all = db_select('t_gene','t_gene');	
	$query_all -> leftJoin('t_gene_synonym', 't_gene_synonym', 't_gene_synonym.eg_id=t_gene.eg_id');
	$query_all -> leftJoin('t_product_protein', 't_product_protein', 't_product_protein.eg_id=t_gene.eg_id');
	$query_all -> leftJoin('t_product_rna', 't_product_rna', 't_product_rna.eg_id=t_gene.eg_id');
	$query_all -> leftJoin('t_biodatabase_link', 't_biodatabase_link', 't_biodatabase_link.eg_id=t_gene.eg_id');
	$query_all -> leftJoin('t_address', 't_address', 't_address.address_id=t_gene.address_id');	
	$query_all ->fields('t_gene');
	$query_all -> condition('t_gene.name', '', '<>');
//	$query_all-> condition('t_biodatabase_link.biodatabase_id', 3);
	
	get_search_gene_conditions($request,$query_all);
	$query_all -> groupBy('t_gene.eg_id');
	$records_all = $query_all->execute()->fetchAll();
	
	$item_per_page = is_numeric($item_per_page)? $item_per_page:count($records_all);
	
	$query->limit($item_per_page)->orderByHeader($header);
	$records = array();
	$records = $query->execute()->fetchAll();
	
	$is_warning = FALSE;
	if(!empty($records_all) && isset( $request['upload_ids']))
  	{
		$query_all_2 = db_select('t_gene','t_gene');	
		$query_all_2 -> leftJoin('t_gene_synonym', 't_gene_synonym', 't_gene_synonym.eg_id=t_gene.eg_id');
		$query_all_2 -> leftJoin('t_biodatabase_link', 't_biodatabase_link', 't_biodatabase_link.eg_id=t_gene.eg_id');
		$query_all_2 ->fields('t_gene');
		$query_all_2 -> condition('t_gene.name', '', '<>');

		$query_all_2 -> groupBy('t_gene.eg_id');
		$records_all_2 = $query_all_2->execute()->fetchAll();
		$is_warning = upload_id_check($records_all_2, $request['upload_ids']);

//  		$is_warning = upload_id_check($records_all, $request['upload_ids']);
  	}
	
	db_set_active("ecogene");
	$pseudo_gene = db_query("SELECT eg_id from t_pseudogene")
					->fetchCol();
	if(!empty($records))
  	{
		$order = 0;
		

	 	foreach ($records as $item)
  	 	{		
			$order = $order + 1;
			$gene_name = (in_array($item->eg_id, $pseudo_gene))? $item->name."'":$item->name;
			
			if(!empty($item->synonym)){
	  	 		$synonym_all = db_query("SELECT GROUP_CONCAT(name SEPARATOR '; ') AS synonym_all 
	  	 									FROM t_gene_synonym WHERE eg_id=:eg_id", array(':eg_id'=>$item->eg_id))
	  	 							->fetchCol();
	  	 	
				if(!empty($synonym_all)) {
					$additional_synonym=strstr($synonym_all[0],'; ');
				}
				if(empty($additional_synonym)){
					$synonym_feild = $item->synonym;
				
				}else{
					$synonym_feild = '<span style="border-bottom: thin dotted; color: #0070B3;" title="'.$item->synonym.$additional_synonym.'">'.$item->synonym.'...</span>';
				}
			}else{
				$synonym_feild = $item->synonym;
			}
			// Show the protein name clolumn if it is searched
	  	 	if (isset($request['prot_name']) && !empty($request['prot_name']))
			{
		  		if($item->type=='aa'){
			   		$rows[] = array(
					
					('<a href="'.$base_url.'/?q=gene/'.$item->eg_id.'">'.$gene_name.'</a>'),
					($synonym_feild),
					($item->description),
					($item->prot_name),
					(number_format($item->left_end/$GENOME_LENGTH * 100,2)),
					("<a href='$base_url/?q=gene/$item->eg_id/proteinrna#protein_seq'>Protein</a>"),
					($item->eg_id),
					($item->accession_id),
					("<a href='$base_url/?q=gene/$item->eg_id/reference'>References</a>"),		
					);
		  		}
				else{
					$rows[] = array(
					
					('<a href="'.$base_url.'/?q=gene/'.$item->eg_id.'">'.$gene_name.'</a>'),
					($synonym_feild),
					($item->description),
					($item->prot_name),
					(number_format($item->left_end/$GENOME_LENGTH * 100,2)),
					("<a href='$base_url/?q=gene/$item->eg_id/proteinrna'>RNA</a>"),
					($item->eg_id),
					($item->accession_id),
					("<a href='$base_url/?q=gene/$item->eg_id/reference'>References</a>"),		
					);
				} 
				continue;
			}
			
			if($item->type=='aa'){
		   		$rows[] = array(
				
				('<a href="'.$base_url.'/?q=gene/'.$item->eg_id.'">'.$gene_name.'</a>'),
				($synonym_feild),
				($item->description),
				(number_format($item->left_end/$GENOME_LENGTH * 100,2)),
				("<a href='$base_url/?q=gene/$item->eg_id/proteinrna#protein_seq'>Protein</a>"),
				($item->eg_id),
				($item->accession_id),
				("<a href='$base_url/?q=gene/$item->eg_id/reference'>References</a>"),		
				);
			}
			else{
					$rows[] = array(
				
				('<a href="'.$base_url.'/?q=gene/'.$item->eg_id.'">'.$gene_name.'</a>'),
				($synonym_feild),
				($item->description),
				(number_format($item->left_end/$GENOME_LENGTH * 100,2)),
				("<a href='$base_url/?q=gene/$item->eg_id/proteinrna'>RNA</a>"),
				($item->eg_id),
				($item->accession_id),
				("<a href='$base_url/?q=gene/$item->eg_id/reference'>References</a>"),		
				);
			} 
		}		
		
  		if(count($rows)==1 && !$is_warning && !isset($request['nonskip']))
		{
		$output .= "<script>	window.location = ('".$GLOBALS['base_url']."/gene/$item->eg_id');</script>";
		db_set_active("default");
		return $output;
		}
		$request['nonskip']=true;
		db_set_active("default");
  		$variables = array(	'header'=>$header, 
						'rows'=>$rows, 
						'sticky'=>NULL, 
						'empty'=>NULL, 
						'caption'=>'', 
						'colgroups'=>array(), 
						'attributes'=>array()
		);	
//		$url = '?q=ecosearch/gene/search&'.drupal_http_build_query(drupal_get_query_parameters($request, array_merge(array('q','MAX_FILE_SIZE','item_per_page'), array_keys($_COOKIE))));
		$url = '?q=ecosearch/gene/search&'.drupal_http_build_query(drupal_get_query_parameters($request, array_merge(array('q','MAX_FILE_SIZE','item_per_page','upload_ids'), array_keys($_COOKIE))));

		$content = '<form name="gene_form" method="post" action="'.$url.'">';
//		$content .= '<input name="pubmed_ids_array" type="hidden" value="'.$request['pubmed_ids_array'].'">';
		
		$content .= 'Show <select name="item_per_page" onChange="this.form.submit()">';

		$content .=	"<option value='5'"; if ($item_per_page == 5) $content .= "SELECTED"; $content .=">5</option>

		    <option value='10'"; if ($item_per_page == 10) $content .= "SELECTED"; $content .=">10</option>

		  	<option value='20'"; if ($item_per_page == 20) $content .= "SELECTED"; $content .=" >20</option>

		    <option value='50'"; if ($item_per_page == 50) $content .= "SELECTED"; $content .=">50</option>

		  	<option value='100'"; if ($item_per_page == 100) $content .= "SELECTED"; $content .=">100</option>

		    <option value='200'"; if ($item_per_page == 200) $content .= "SELECTED"; $content .=">200</option>

		    <option value='500'"; if ($item_per_page == 500) $content .= "SELECTED"; $content .=">500</option>

		  	<option value='ALL'"; if ($request['item_per_page'] == 'ALL') $content .= "SELECTED"; $content .=">ALL</option></select> per page";
		
		$content .= theme_table($variables);
		$content .= '</form>';
//		$parameters = drupal_get_query_parameters($request, array_merge(array('page','order','asc', 'q','MAX_FILE_SIZE'), array_keys($_COOKIE)));
		$parameters = drupal_get_query_parameters($request, array_merge(array('upload_ids','page','order','asc', 'q','MAX_FILE_SIZE'), array_keys($_COOKIE)));

		$content .=  theme('pager', array('quantity' => 10,'parameters' => $parameters, ));
		
  	}
 	else 
  	{
		$content = '<em>No record has been found!</em>';
  	}
  
  	
  	if(!empty($records))
  	{

		$downloadlink = "";
		$downloadname = "";
		
		$downloadlink_aa = "";
		$downloadlink_nt = "";
		
		$downloadnamearray =array();
		
		
//		$records_all = $query_all->execute()->fetchAll();
	
		if(!empty($records_all))
  		{
		
	 		foreach ($records_all as $rownolimit)
	 		{
				$downloadlink = $downloadlink.$rownolimit->eg_id.",";
				$downloadnamearray[] = $rownolimit->name;		
				if($rownolimit->type=='aa')	
				{
					$downloadlink_aa = $downloadlink_aa.$rownolimit->eg_id.",";
				}
				elseif($rownolimit->type=='nt')
				{
					$downloadlink_nt = $downloadlink_nt.$rownolimit->eg_id.",";
				}

			}
			
//			echo $rownolimit->eg_id;
		}
//		
		if(count($downloadnamearray)>0){
			sort($downloadnamearray);
			foreach($downloadnamearray as $value) {
				$downloadname = $downloadname.$value.",";
			}
		}
		$total = $query_all->countQuery()->execute()->fetchField();
		$downloadlink = strrev(substr(strrev($downloadlink),1));// remove the "," because of last item 
		
		$downloadlink_aa = strrev(substr(strrev($downloadlink_aa),1));// remove the "," because of last item 
		$downloadlink_nt = strrev(substr(strrev($downloadlink_nt),1));// remove the "," because of last item 

		$downloadname = strrev(substr(strrev($downloadname),1));// remove the "," because of last item 
		
//
// downloadlib.php is the file that processes the downloading.
		
	db_set_active("default");
	$download_header = array(
	 array('colspan'=>'6', 'data'=> t('Download Search Results:'))
	);
	
	$row_download[] = array(
		array('width'=>'15%', 'data'=> t("<a href=\"/genemap/map.php?egids=$downloadlink\"><img border=0 src=\"".$GLOBALS['base_url']."/sites/all/include/topic_map.php?egids=$downloadlink\" title=\"Genes On Map\"></a>")),
		array('width'=>'35%','data' =>t('<form name="form3" method="post" action="'.$base_url.'/?q=ecodownload/seqlib/download">
		<fieldset><legend>SEQ Download </legend>
		<input name="egids" value="'.$downloadlink.'" type="hidden"><input name="lib" value="SEQ" type="hidden">
		<table><tbody><tr></tr>
		<tr><td colspan="2" align="left">Centered at Gene: <input name="catRadioB" value="start" checked type="radio">Start <input name="catRadioB" value="stop" type="radio">End<br> </td></tr>
		<tr><td colspan="2" align="left">	Format:     <input name="catRadioA" value="FASTA" checked type="radio">FASTA <input name="catRadioA" value="Tabular" type="radio">Tabular<br>  </td></tr>
	    <tr><td colspan="1" valign="top" align="right">Range:<input size="5" name="us" value="From" type="text"> &nbsp;<input size="5" name="ds" value="To" type="text"></td><td valign="top">
		<input value="GET SEQ" type="submit"></td></tr></tbody></table></fieldset></form>')),
		array('width'=>'11%','data' =>t('<form method="post" action="'.$base_url.'/?q=ecodownload/seqlib/download"><input type="hidden" name="egids" value="'.$downloadlink.'"><input type="hidden" name="lib" value="DNA"><input type="submit" name="submit" value="DNA Sequence"></td></form>')),
		array('width'=>'14%','data' =>t('<form method="post" action="'.$base_url.'/?q=ecodownload/seqlib/download"><input type="hidden" name="egids" value="'.$downloadlink_aa.'"><input type="hidden" name="lib" value="Protein"><input type="submit" name="submit" value="Protein Sequence"></form></ br><form method="post" action="'.$base_url.'/?q=ecodownload/seqlib/download"><input type="hidden" name="egids" value="'.$downloadlink_nt.'"><input type="hidden" name="lib" value="RNA"><input type="submit" name="submit" value="&nbsp;&nbsp;RNA  Sequence&nbsp;&nbsp;"></form></td>')),
		array('width'=>'12%','data' =>t('<form method="post" action="'.$base_url.'/?q=ecodownload/primer"><input type="hidden" name="egids" value="'.$downloadlink.'"><input type="submit" name="submit" value="Primer Pairs"></td></form>')),
		array('width'=>'13%','data' =>t('<form method="post" action="'.$base_url.'/?q=ecodownload/dbtable"><input type="hidden" name="egids" value="'.$downloadlink.'"><input type="submit" name="submit" value="Download Page"></td></form>'))
		);
	$variables2 = array(	'header'=>$download_header, 
						'rows'=>$row_download, 
						'sticky'=>NULL, 
						'empty'=>NULL, 
						'caption'=>'', 
						'colgroups'=>array(), 
						'attributes'=>array()
		);	
	$content2=theme_table($variables2);
	
		drupal_set_title(search_gene_page_title($total), PASS_THROUGH);
  	}else 
  	{
  		$content2='';
  	}
  	
  	
  	db_set_active("default");
 	
//  $build['content'] = array(
//		
//        'this_does_not_matter_too_much1' 
//			=> array(
//			'#type' => 'markup',
//            '#markup' => $content,
//        ),
//        'this_does_not_matter_too_much2' 
//			=> array(
//			'#type' => 'markup',
//            '#markup' => $content2,
//        ),
//        
//        
//    );
//
//  $build['pager'] = array(
//    	'#theme' => 'pager',
//    	'tags' => array('0'), 
//    	'limit' => $item_per_page, 
//    	'element' => 0, 
//    	'parameters' => array(), 
//    	'quantity' => 9,
////    	'#weight' => 1,
//	);
   
	$content_all = $content.$content2;
	return $content_all;	
}
function upload_id_check($records, $upload_ids)
{
	
	
	$upload_ids = preg_split("/[,; ]+/",$upload_ids); 
	
	$is_warning = FALSE;
	db_set_active("ecogene");
	$unique_id = array(); // consist of eg_id, gene name, eck_id
	$unique_sys = array();
	$multip_sys= array();
	
	$multip_sys = db_query("SELECT name from t_gene_synonym GROUP BY name  HAVING count(eg_id)>1")->fetchCol();
	$unique_sys =  db_query("SELECT name from t_gene_synonym GROUP BY name  HAVING count(eg_id)=1")->fetchCol();
	foreach ($records as $item)
  	{		
  		$unique_id[] = $item->eg_id;
  		$unique_id[] = $item->name;
  		$unique_id[] = $item->eck_id;		
	}
	
	$upload_ids_chk = array_udiff($upload_ids, $unique_id,"strcasecmp"); 
	$ids_primary = array_uintersect($upload_ids, $unique_id,"strcasecmp");

	$ids_unique_sys = array_uintersect($upload_ids_chk, $unique_sys,"strcasecmp");
	$upload_ids_chk = array_udiff($upload_ids_chk, $unique_sys,"strcasecmp");

	
	$ids_multi_sys = array_uintersect($upload_ids_chk, $multip_sys,"strcasecmp");
	$upload_ids_chk = array_udiff($upload_ids_chk, $multip_sys, "strcasecmp");

	
	$other_acc = db_select('t_biodatabase_link','t_biodatabase_link')
				->condition('accession_id', $upload_ids, 'IN')
				->fields('t_biodatabase_link', array('accession_id'))
				->execute()->fetchCol();
					
	$ids_other_acc = array_uintersect($upload_ids_chk, $other_acc, "strcasecmp");
	$upload_ids_chk = array_udiff($upload_ids_chk, $other_acc, "strcasecmp");

	$params_sub_str = $upload_ids;
    foreach ($params_sub_str as $ind=>$value) {
    		$params_sub_str[$ind] = "'".$value."'";
    }   
	$other_acc = db_select('t_biodatabase_link','t_biodatabase_link')
				->where("( SUBSTRING_INDEX(accession_id, '.', 1)) IN (". implode(',', $params_sub_str).")")	
				->fields('t_biodatabase_link', array('accession_id'))
				->execute()->fetchCol();
	foreach ($other_acc as $ind=>$value) {
		$other_acc[$ind] = substr($value, 0, -2);
	}

	$ids_other_acc = array_uintersect($upload_ids_chk, $other_acc, "strcasecmp");
	$upload_ids_chk = array_udiff($upload_ids_chk, $other_acc, "strcasecmp");			
	
	if($ids_unique_sys) 
	{
		$message = "The following inputs in () are synonyms; the primary gene names are listed in the search results: ";
 		foreach ($ids_unique_sys as $sname) 
 		{
 			$name = db_query("SELECT t_gene.name from t_gene join t_gene_synonym on t_gene.eg_id = t_gene_synonym.eg_id where t_gene_synonym.name=:sname", array(':sname'=>$sname))->fetchCol();
 			$message .= $name[0]."(".$sname.") ";		
		}	
		drupal_set_message($message,'warning');
		$is_warning = TRUE;
	}
	
	if($ids_multi_sys) 
	{
		
		$message = "The following inputs in () are synonyms; the primary gene names are listed in the search results: ";
 		foreach ($ids_multi_sys as $sname) 
 		{
 			$name = db_query("SELECT t_gene.name from t_gene join t_gene_synonym  on t_gene.eg_id = t_gene_synonym.eg_id where t_gene_synonym.name=:sname", array('sname'=>$sname))->fetchCol();
 			foreach ($name as $value) {
 				$message .= $value."(".$sname.") ";	 				
 			}
 				
		}	
		drupal_set_message($message,'warning');
		$is_warning = TRUE;
	}
	if($upload_ids_chk) 
	{
		$message = "The following inputs are not valid ecogene IDs: ";
 		foreach ($upload_ids_chk as $value) 
 		{
 			$message .= $value." ";
		}	
		drupal_set_message($message,'warning');	
		$is_warning = TRUE;
	}
	db_set_active("default");
	return $is_warning;
}
function search_gene_page_title($total)
{
	$title= '<div id="search_page_title"><h1> Gene Search Results <span id="total">'
			.$total.' total</span></h1></div>';
	
	return $title;
}
function search_reference_page_title($total)
{
	$title= '<div id="search_page_title"><h1> Reference Search Results <span id="total">'
			.$total.' total</span></h1></div>';
	
	return $title;
}
function search_reference_return() {
	global $GENOME_LENGTH;
	global $base_url;

	$request = $_REQUEST;
	
	$userfile_ref  = $_FILES['userfile_ref'];
	if ($userfile_ref['name'] != "")		
	{

			$uploadedfile = $userfile_ref['tmp_name'];


			$fp = fopen($uploadedfile, 'r');

			if (!$fp) {

				echo 'Could not open the file ';
			}
			
			$pubmed_ids = "";
			while (false !== ($string = fgets($fp)))
			{

				$items = explode (" ",$string);

				foreach ($items as $item) {

					// Remove and leading and lagging spaces.

					$item = trim($item);
					$pubmed_ids .=  $item.',';

				}
			}
			$request['pubmed_ids_array'] = $pubmed_ids;
	}
	if($_POST)
    {
    	$request = drupal_get_query_parameters($request, array_merge(array('MAX_FILE_SIZE')));
    	drupal_goto('ecosearch/reference/search',array('query'=>$request));
    }
	
	$item_per_page = isset($request['item_per_page'])? $request['item_per_page']: 10;
	$request['item_per_page'] = $item_per_page;
	
	
	db_set_active("ecogene");

	$header = array(
		array('data' => t('Title'), 'field' => 't_reference.title'),
		t('Authors'),
		array('data' => t('Year'), 'field' => 't_reference.date_published', 'sort' => 'desc'),
		array('data' => t('Journal'), 'field' => 't_reference.journal'),
		t('Vol'),
		t('Pages'),
		t('Pubmed ID'),
		t('Links')
	);
	
	db_set_active("ecogene");	
	
//	mysql_query("SET NAMES utf8");
	
	$query_count = db_select('t_reference','t_reference');
	$query_count -> join('t_reference_author_link', 't_reference_author_link', 't_reference.pubmed_id = t_reference_author_link.pubmed_id');
	$query_count -> join('t_author', 't_author', 't_author.author_id = t_reference_author_link.author_id');
	$query_count -> addExpression('year(t_reference.date_published)', 'year_published');
	$query_count ->fields('t_reference',array('pubmed_id'));
	get_search_reference_conditions($request,$query_count);
	$query_count -> groupBy('t_reference.pubmed_id');
	
	$records_pubmed_id = $query_count->execute()->fetchAll();
	$records_total = count($records_pubmed_id);
	
	
	$item_per_page = is_numeric($item_per_page)? $item_per_page:$records_total;
	$query = db_select('t_reference','t_reference')-> extend('PagerDefault')->extend('TableSort');
	$query -> join('t_reference_author_link', 't_reference_author_link', 't_reference.pubmed_id = t_reference_author_link.pubmed_id');
	$query -> join('t_author', 't_author', 't_author.author_id = t_reference_author_link.author_id');
	$query -> addExpression("year(t_reference.date_published)", 'year_published');
	$query->fields('t_reference');
	get_search_reference_conditions($request,$query);
	$query -> groupBy('t_reference.pubmed_id');	
	$query -> limit($item_per_page);
	$query -> orderByHeader($header);
//echo $query->__toString();
	$records = array();
	
	$records = $query->execute()->fetchAll();
	
	if(!empty($records))
  	{
  		
		foreach ($records as $item)
  	 	{		
  	 		
  	 		
  	 		
  	 							
			$author_result = (db_query("SELECT  t_author.last_name, concat(t_author.last_name,' ',t_author.initials) author_name   FROM  t_reference_author_link, t_author  where t_reference_author_link.pubmed_id=:pubmed_id and t_author.author_id=t_reference_author_link.author_id ORDER by t_reference_author_link.seq_no", 
  	 						array(':pubmed_id'=>$item->pubmed_id)));
  	 		
			$athor_info='';
			while ($item_author = $author_result->fetchObject()) {
				$authorName = (remove_accent($item_author->author_name));
//				echo $item_author->author_name;
//				$item_author->author_name = utf8_decode($item_author->author_name);
					
//				$item_author->author_name = utf8_decode( ($item_author->author_name));

//				$item_author->author_name = iconv("UTF-8", "ISO-8859-1", $item_author->author_name);
//				$item_author->author_name = utf8_encode($item_author->author_name);
				$athor_info.=t('<A HREF="'.$base_url.'?q=ecosearch/reference/search&initials='.$authorName.'">'.$item_author->author_name.'</A><BR>');

			}

			$gene_link_count = db_select('t_gene_reference_link', 't_gene_reference_link')->fields('t_gene_reference_link')
  	 							->condition('pubmed_id',$item->pubmed_id)
  	 							->countQuery()->execute()->fetchField(); 
  	 							
			$array_link_count = db_select('t_array', 't_array')->fields('t_array')
  	 							->condition('pubmed_id',$item->pubmed_id)
  	 							->countQuery()->execute()->fetchField();
  	 							
  	 		$topic_link_array = array();
  	 		$topic_link_array = db_select('t_topic_reference_link', 'tfr')->fields('tfr',array('topic_id'))
  	 							->condition('pubmed_id',$item->pubmed_id)
  	 							->execute()->fetchCol();
  	 		$topic_link_count = count($topic_link_array);
  	 		$link_info='';
  	 		if($gene_link_count>0)
  	 		{
  	 			$link_info='<a href="'.$base_url.'/?q=ecosearch/gene/search&pubmedID='.$item->pubmed_id.'">Genes</a>';
  	 		}	
  	 		$n=0;
  	 		if($topic_link_count>0)
  	 		{
  	 			$n++;
  	 			$topic_ids = implode(' ', $topic_link_array);
  	 			if($gene_link_count>0) 	$link_info .= '<br />';
  	 				
  	 			$link_info .= '<a href="'.$base_url.'/?q=/topic/topics&topicids='.$topic_ids.'">Topics</a>';
  	 			
  	 			
  	 		}
  	 		
  	 		if($array_link_count>0)
  	 		{
  	 			if($gene_link_count>0 || $topic_link_count>0) 	$link_info .= '<br />';
  	 			$link_info .= '<a href="'.$base_url.'/?q=ecoarray/paper/'.$item->pubmed_id.'">Microarray</a>';
  	 		}
  	 					
			$rows[] = array(
				("<a href='http://www.ncbi.nlm.nih.gov/entrez/query.fcgi?cmd=Retrieve&db=PubMed&list_uids=$item->pubmed_id&dopt=Abstract'>".$item->title."</a>"),
				($athor_info),
				($item->year_published),
				($item->journal),
				($item->volume),
				($item->pagination),
				($item->pubmed_id),		
				($link_info),
			);
		}

  		$variables = array(	'header'=>$header, 
						'rows'=>$rows, 
						'sticky'=>NULL, 
						'empty'=>NULL, 
						'caption'=>'', 
						'colgroups'=>array(), 
						'attributes'=>array()
		);	
		$url = '?q=ecosearch/reference/search&'.drupal_http_build_query(drupal_get_query_parameters($request, array_merge(array('q','MAX_FILE_SIZE','item_per_page','pubmed_ids_array'), array_keys($_COOKIE))));
		$content = '<form name="ref_form" method="post" action="'.$url.'">';
		$content .= '<input name="pubmed_ids_array" type="hidden" value="'.$request['pubmed_ids_array'].'">';

		$content .= 'Show <select name="item_per_page" onChange="this.form.submit()">';

		$content .=	"<option value='5'"; if ($item_per_page == 5) $content .= "SELECTED"; $content .=">5</option>

		    <option value='10'"; if ($item_per_page == 10) $content .= "SELECTED"; $content .=">10</option>

		  	<option value='20'"; if ($item_per_page == 20) $content .= "SELECTED"; $content .=" >20</option>

		    <option value='50'"; if ($item_per_page == 50) $content .= "SELECTED"; $content .=">50</option>

		  	<option value='100'"; if ($item_per_page == 100) $content .= "SELECTED"; $content .=">100</option>

		    <option value='200'"; if ($item_per_page == 200) $content .= "SELECTED"; $content .=">200</option>

		    <option value='500'"; if ($item_per_page == 500) $content .= "SELECTED"; $content .=">500</option>

		  	<option value='ALL'"; if ($request['item_per_page'] == 'ALL') $content .= "SELECTED"; $content .=">ALL</option></select> per page";
		db_set_active("default");    
		$content .= theme_table($variables);
		$content .= '</form>';
  	}
 	else 
  	{
		$content = '<em>No record has been found!</em>';
  	}
  	db_set_active("default");
  	$parameters = drupal_get_query_parameters($request, array_merge(array('page','q','MAX_FILE_SIZE'), array_keys($_COOKIE)));
	$content .=  theme('pager', array('quantity' => 10,'parameters' => $parameters, ));
	
	
	drupal_set_title(search_reference_page_title($records_total), PASS_THROUGH);
//	$build['content'] = array(
//		
//        'this_does_not_matter_too_much1' 
//			=> array(
//			'#type' => 'markup',
//            '#markup' => $content,
//        )
//    );
//	$variables = array('limit'=>$item_per_page, 'parameters' => $parameters, 'quantity' => 15,);
//    print_r($parameters);
//    
//  	$build['pager'] = array(
//    	'#theme' => 'pager',
//  		'variables' => $variables
////    	'tags' => array('1'), 
////	   	'limit' => $item_per_page, 
////    	'element' => 0, 
////    	'parameters' => $parameters, 
////    	'quantity' => 11,
////    	'#weight' => 1,
//	);

	db_set_active("default");
//	return $build;
	return $content;
}

function get_search_topic_condition($filter=array(), &$query)
{

	$topicTable = 't_topic';
	foreach ($filter as $key => $value) {
    	
    	if($value!='')
    	{
    		switch ($key)
    		{
    			case 'main_search':
    				
    				$params_sub = preg_split("/[.,; ]+/",$value);
    					if(!empty($params_sub))
    					{
    						$subquery = db_and();
    						 
    						foreach ($params_sub as $value) {
							  $subquery = $subquery ->condition($topicTable.'.topic_name', '%%'.$value.'%%', 'LIKE')
    									->condition($topicTable.'.topic_title', '%%'.$value.'%%', 'LIKE')
    									->condition($topicTable.'.topic_text', '%%'.$value.'%%', 'LIKE')
    									->condition($topicTable.'.topic_description', '%%'.$value.'%%', 'LIKE');
								
    						}
    						$query->condition($subquery);
    					}

    			break;
    			
    			case 'title':
    				
    				$params_sub = preg_split("/[.,; ]+/",$value);
    					if(!empty($params_sub))
    					{
    						$OR = db_or();
    						 
    						foreach ($params_sub as $value) {
							  $OR = $OR ->condition($topicTable.'.topic_name', '%%'.$value.'%%', 'LIKE')
    									->condition($topicTable.'.topic_title', '%%'.$value.'%%', 'LIKE');
								
    						}
    						$query->condition($OR);
    					}

    			break;
    				
    			case 'textall':
    				$params_sub = preg_split("/[.,; ]+/",$value);
    					if(!empty($params_sub))
    					{
    						$OR = db_or();
    						 
    						foreach ($params_sub as $value) {
							  $OR = $OR ->condition($topicTable.'.topic_name', '%%'.$value.'%%', 'LIKE')
    									->condition($topicTable.'.topic_title', '%%'.$value.'%%', 'LIKE')
    									->condition($topicTable.'.topic_text', '%%'.$value.'%%', 'LIKE')
    									->condition($topicTable.'.topic_description', '%%'.$value.'%%', 'LIKE');
								
    						}
    						$query->condition($OR);
    					}

    			break;    
    			case 'sub_topic_id':
    				$topic_ids = db_select('t_topic_topic_link','t_topic_topic_link')
    						->fields('t_topic_topic_link',array('linked_topic_id'))
    						->condition('t_topic_topic_link.topic_id', $value, '=')
   							->execute()->fetchCol();
   					
   					if(!empty($topic_ids))
   					{
						$query->condition($topicTable.'.topic_id', $topic_ids ,'IN');
   					} 

    			break; 
    			case 'sup_topic_id':
    				$topic_ids = db_select('t_topic_topic_link','t_topic_topic_link')
    						->fields('t_topic_topic_link',array('topic_id'))
    						->condition('t_topic_topic_link.linked_topic_id', $value, '=')
   							->execute()->fetchCol();
   					
   					if(!empty($topic_ids))
   					{
						$query->condition($topicTable.'.topic_id', $topic_ids ,'IN');
   					} 

    			break;    				
    		}

    	}
  	}
  	
}
function search_topic_return() {
	global $base_url;
	
	$header = array(
			array('data' => t('Name'), 'field' => 't_topic.topic_name'),
			t('Gene Set'),
			t('Super Topics'),
			t('Sub Topics'),
			array('data' => t('Type'), 'field' => 't_topic.topic_type')
	);
	
	$item_per_page = isset($_REQUEST['item_per_page'])? $_REQUEST['item_per_page']: 10;
	
	db_set_active("ecogene");
	
	$query = db_select('t_topic','t_topic')-> extend('PagerDefault')-> extend('TableSort') ;
	$query->fields('t_topic');
	get_search_topic_condition($_REQUEST,$query);
	
	$query_all = db_select('t_topic','t_topic')->fields('t_topic');
	get_search_topic_condition($_REQUEST,$query_all);
	
	$total = $query_all->countQuery()->execute()->fetchField();
				
	$query->limit($item_per_page)->orderByHeader($header);
	$records = array();
	$records = $query->execute()->fetchAll();
  	
	if(!empty($records))
  	{
		
	 	foreach ($records as $item)
  	 	{		
		$num_gene = db_select('t_topic_gene_link','t')
			->condition('t.topic_id',$item->topic_id)
			->groupBy('t.eg_id')
			->countQuery()->execute()->fetchField();
			
		$num_sup = db_select('t_topic_topic_link','t')
				->condition('t.topic_id',$item->topic_id)
				->groupBy('t.linked_topic_id')
				->countQuery()->execute()->fetchField();
		$num_sub = db_select('t_topic_topic_link','t')
				->condition('t.linked_topic_id',$item->topic_id)
				->groupBy('t.topic_id')
				->countQuery()->execute()->fetchField();
						
   		$rows[] = array(
		
		("<a href='$base_url/?q=topic/$item->topic_id'>".$item->topic_name."</a>"),
		("<a href='$base_url/?q=ecosearch/gene/search&search_topic=$item->topic_id'>".$num_gene."</a>"),
		("<a href='$base_url/?q=ecosearch/topic/search&sub_topic_id=$item->topic_id'>".$num_sup."</a>"),
		("<a href='$base_url/?q=ecosearch/topic/search&sup_topic_id=$item->topic_id'>".$num_sub."</a>"),
		($item->topic_type),	
		);
		}
		
//  		if(count($rows)==1)
//		{
//		$output .= "<script>	window.location = ('?q=gene/$item->eg_id');</script>";
//		db_set_active("default");
//		return $output;
//		}
	
  		$variables = array(	'header'=>$header, 
						'rows'=>$rows, 
						'sticky'=>NULL, 
						'empty'=>NULL, 
						'caption'=>'', 
						'colgroups'=>array(), 
						'attributes'=>array()
		);	
		$content = theme_table($variables);
  	}
 	else 
  	{
		$content = '<em>No record has been found!</em>';
  	}
	
	
	
	
	db_set_active("default");
	
	$title= '<div id="search_page_title"><h1>Topic Search Results <span id="total">'
			.$total.' total</span></h1></div>';
			
	drupal_set_title($title, PASS_THROUGH);
  	$build['content'] = array(
		
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



