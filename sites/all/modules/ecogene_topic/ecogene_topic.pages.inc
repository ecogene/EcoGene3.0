<?PHP
include_once(drupal_get_path('module', 'ecogene_array') .'/ecogene_array.pages.inc');
include_once(drupal_get_path('module', 'ecogene_gene') .'/ecogene_gene.array.inc');
// 3 gene sets
function venndiagram_gene_sets($tmpfname, $map, $gene_sets)
{
	header ("Content-type: image/png");
	$width = 346;
	$height = 332;

    $srcimage = @ImageCreateFrompng("sites/all/images/venn3.png")
      	or die("Cannot Initialize new GD image stream"); 	
      	
	$img=imagecreatetruecolor($width,$height);
	imagecopyresampled($img,$srcimage,0,0,0,0, $width,$height,$width,$height);
	
    $background = imagecolorallocate ($img,  0, 0, 0);	
    $fontfile = 'sites/all/include/verdana.ttf'; 	     						
    $text_size =11;
		
//    
//	  $background_color = imagecolorallocate ($img,  0, 146, 193);
//    $foreground_color = imagecolorallocate($img, 255, 255, 255);
//    $gene_color = imagecolorallocate($img, 255, 0, 0);
    
    for ($t=0;$t<count($map);$t++)
    {
    	$number = count($gene_sets[$t]);
		imagettftext($img, $text_size, 0, $map[$t]['x1']/2+$map[$t]['x2']/2-$text_size, 
						$map[$t]['y1']/2+ $map[$t]['y2']/2-$text_size/2,$background, $fontfile, $number);
    }
	imagettftext($img, $text_size+5, 0, 20, 30,$background, $fontfile, 'A');
	imagettftext($img, $text_size+5, 0, $width-20, 30,$background, $fontfile, 'B');
	imagettftext($img, $text_size+5, 0, $width/2+80, $height-10,$background, $fontfile, 'C');
	
	Imagepng($img,$tmpfname);
	ImageDestroy($img);		
}
function venndiagram_gene_sets_bw($tmpfname, $map, $gene_sets)
{
	header ("Content-type: image/png");
	$width = 346;
	$height = 332;

    $srcimage = @ImageCreateFrompng("sites/all/images/venn3_bw.png")
      	or die("Cannot Initialize new GD image stream"); 	
      	
	$img=imagecreatetruecolor($width,$height);
	imagecopyresampled($img,$srcimage,0,0,0,0, $width,$height,$width,$height);
	
    $background = imagecolorallocate ($img,  0, 0, 0);	
    $fontfile = 'sites/all/include/verdana.ttf'; 	     						
    $text_size =11;
		
//    
//	  $background_color = imagecolorallocate ($img,  0, 146, 193);
//    $foreground_color = imagecolorallocate($img, 255, 255, 255);
//    $gene_color = imagecolorallocate($img, 255, 0, 0);
    
    for ($t=0;$t<count($map);$t++)
    {
    	$number = count($gene_sets[$t]);
		imagettftext($img, $text_size, 0, $map[$t]['x1']/2+$map[$t]['x2']/2-$text_size, 
						$map[$t]['y1']/2+ $map[$t]['y2']/2-$text_size/2,$background, $fontfile, $number);
    }
	imagettftext($img, $text_size+5, 0, 20, 30,$background, $fontfile, 'A');
	imagettftext($img, $text_size+5, 0, $width-20, 30,$background, $fontfile, 'B');
	imagettftext($img, $text_size+5, 0, $width/2+80, $height-10,$background, $fontfile, 'C');
	
	Imagepng($img,$tmpfname);
	ImageDestroy($img);		
}
// 2 gene sets
function venndiagram_gene_sets_2($tmpfname, $map, $gene_sets)
{
	header ("Content-type: image/png");
	$width = 346;
	$height = 332;

    $srcimage = @ImageCreateFrompng("sites/all/images/venn2.png")
      	or die("Cannot Initialize new GD image stream"); 	
      	
	$img=imagecreatetruecolor($width,$height);
	imagecopyresampled($img,$srcimage,0,0,0,0, $width,$height,$width,$height);
	
    $background = imagecolorallocate ($img,  0, 0, 0);	
    $fontfile = 'sites/all/include/verdana.ttf'; 	     						
    $text_size =11;

    for ($t=0;$t<count($map);$t++)
    {
    	$number = count($gene_sets[$t]);
		imagettftext($img, $text_size, 0, $map[$t]['x1']/2+$map[$t]['x2']/2-$text_size, 
						$map[$t]['y1']/2+ $map[$t]['y2']/2-$text_size/2,$background, $fontfile, $number);
    }
	imagettftext($img, $text_size+5, 0, 20, 30,$background, $fontfile, 'A');
	imagettftext($img, $text_size+5, 0, $width-20, 30,$background, $fontfile, 'B');
	
	
	Imagepng($img,$tmpfname);
	ImageDestroy($img);		
}
function venndiagram_gene_sets_2_bw($tmpfname, $map, $gene_sets)
{
	header ("Content-type: image/png");
	$width = 346;
	$height = 332;

    $srcimage = @ImageCreateFrompng("sites/all/images/venn2_bw.png")
      	or die("Cannot Initialize new GD image stream"); 	
      	
	$img=imagecreatetruecolor($width,$height);
	imagecopyresampled($img,$srcimage,0,0,0,0, $width,$height,$width,$height);
	
    $background = imagecolorallocate ($img,  0, 0, 0);	
    $fontfile = 'sites/all/include/verdana.ttf'; 	     						
    $text_size =11;

    for ($t=0;$t<count($map);$t++)
    {
    	$number = count($gene_sets[$t]);
		imagettftext($img, $text_size, 0, $map[$t]['x1']/2+$map[$t]['x2']/2-$text_size, 
						$map[$t]['y1']/2+ $map[$t]['y2']/2-$text_size/2,$background, $fontfile, $number);
    }
	imagettftext($img, $text_size+5, 0, 20, 30,$background, $fontfile, 'A');
	imagettftext($img, $text_size+5, 0, $width-20, 30,$background, $fontfile, 'B');
	
	
	Imagepng($img,$tmpfname);
	ImageDestroy($img);		
}
function array_topic_gene_venndiagram()
{
	global $base_url;
	
	drupal_set_title('Gene Set Venn Diagram');
	
	db_set_active("ecogene");
	$topic_ids = $_REQUEST['topic_ids'];
	$array_ids = $_REQUEST['array_exp_ids'];
	
	$eg_ids = array(array(),array(),array());
	$title_name=array('','','');
	$title_url=array('','','');
	$n = 0;
	foreach ($topic_ids as $value) {
			
    						$query = db_select('t_topic_gene_link','t_topic_gene_link');
    						$query ->join('t_gene', 't_gene', 't_gene.eg_id=t_topic_gene_link.eg_id');
    						$query ->addField('t_topic_gene_link','eg_id','eg_id');
    						$query ->condition('t_topic_gene_link.topic_id', $value, '=');
   							$eg_ids[$n] = $query ->execute()->fetchCol();
   							
   							$title_name[$n] = db_select('t_topic','t_topic')
    						->fields('t_topic',array('topic_name'))
    						->condition('t_topic.topic_id', $value, '=')
   							->execute()->fetchCol();
   							$title_name[$n] = 'Topic - "'.$title_name[$n][0].'"';
   							$title_url[$n] = $base_url.'/?q=ecosearch/gene/search&search_topic='.$value;
   						$n++;
   					
    }
	foreach ($array_ids as $value) {
			
							$value_array = explode(',', trim($value));
							
    						$eg_ids[$n] = db_select('t_array_data','t_array_data')
    						->fields('t_array_data',array('eg_id'))
    						->condition('t_array_data.pubmed_id', $value_array[0], '=')
    						->condition('t_array_data.exp_id', $value_array[1], '=')
   							->execute()->fetchCol();
   							
   							  							
   							$item = db_select('t_array_exp','t_array_exp')
    						->fields('t_array_exp')
    						->condition('t_array_exp.pubmed_id', $value_array[0], '=')
    						->condition('t_array_exp.exp_id', $value_array[1], '=')
   							->execute()->fetch();
   							$title_name[$n] = 'EcoArray Experiment - "'. displayed_name($item).'"';
   							
   							$title_url[$n] = $base_url.'/?q=ecosearch/gene/search&array_exp_ids='.$value;
   						$n++;
   					
    }
    
    db_set_active("default");
    
    // A and B
    $two_set[0] = array_intersect($eg_ids[0], $eg_ids[1]);
    // A and C
    $two_set[1] = array_intersect($eg_ids[0], $eg_ids[2]);
    // B and C
    $two_set[2] = array_intersect($eg_ids[1], $eg_ids[2]);
    
    // A, not B, not C
    $gene_sets[0] = array_diff(array_diff($eg_ids[0], $eg_ids[1]), $eg_ids[2]);
    // B, not A, not C
    $gene_sets[1] = array_diff(array_diff($eg_ids[1], $eg_ids[0]), $eg_ids[2]);
    // C, not B, not A
    $gene_sets[2] = array_diff(array_diff($eg_ids[2], $eg_ids[0]), $eg_ids[1]);
    
    //A and B not C
    $gene_sets[3] = array_diff($two_set[0], $eg_ids[2]);
     //B and C not A
    $gene_sets[4] = array_diff($two_set[2], $eg_ids[0]);
     //A and C not B
    $gene_sets[5] = array_diff($two_set[1], $eg_ids[1]);
    
    //A and B and C
    $gene_sets[6] = array_intersect($two_set[0], $two_set[1]);
    
//    print_r($topic_eg_ids);
//    print_r($topic_name);
  
    
	$tmpfname="temp/venn_".rand(1, 100000).".png";
	$tmpfname_bw="temp/venn_bw_".rand(1, 100000).".png";
	if(empty($eg_ids[0]) || empty($eg_ids[1]))
	{
		$content = 'Please select at least two gene sets!';
		return $content;
	}elseif(empty($eg_ids[2]))
	{
		$content.= '<div>';
		$content.= '<p>A Set: '.$title_name[0].'. <a href="'.$title_url[0].'">'.count($eg_ids[0]) .' genes</a> </p>';
		$content.= '<p>B Set: '.$title_name[1].'. <a href="'.$title_url[1].'">'.count($eg_ids[1]) .' genes</a> </p>';

		$content.= '</div>';
		$map = array(
			array("x1"=>46, "y1"=>50, "x2"=>98, "y2"=>220),
			array("x1"=>250, "y1"=>50, "x2"=>300, "y2"=>220),
			array("x1"=>150, "y1"=>58, "x2"=>211, "y2"=>210),
		);
		$title = array(
			'(A) and (not B) - '.count($gene_sets[0]). ' genes.',
			'(B) and (not A) - '.count($gene_sets[1]). ' genes.',
			'(A) and (B) - '.count($two_set[0]). ' genes.',
			
		);	
		$gene_sets[2] = $two_set[0];
		$gene_sets = array_slice($gene_sets, 0, 3);
		
		venndiagram_gene_sets_2($tmpfname, $map, $gene_sets);
		venndiagram_gene_sets_2_bw($tmpfname_bw, $map, $gene_sets);
	}else
	{
	$content.= '<div>';
	$content.= '<p>A Set: '.$title_name[0].'. <a href="'.$title_url[0].'">'.count($eg_ids[0]) .' genes</a> </p>';
	$content.= '<p>B Set: '.$title_name[1].'. <a href="'.$title_url[1].'">'.count($eg_ids[1]) .' genes</a> </p>';	
	$content.= '<p>C Set: '.$title_name[2].'. <a href="'.$title_url[2].'">'.count($eg_ids[2]) .' genes</a> </p>';
	$content.= '</div>';
	
	$map = array(
			array("x1"=>46, "y1"=>50, "x2"=>98, "y2"=>131),
			array("x1"=>250, "y1"=>50, "x2"=>300, "y2"=>131),
			array("x1"=>120, "y1"=>250, "x2"=>240, "y2"=>290),
			array("x1"=>150, "y1"=>58, "x2"=>211, "y2"=>95),
			array("x1"=>219, "y1"=>183, "x2"=>270, "y2"=>214),
			array("x1"=>81, "y1"=>180, "x2"=>136, "y2"=>220),
			array("x1"=>142, "y1"=>134, "x2"=>205, "y2"=>178),
			
			
			
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
	
	venndiagram_gene_sets($tmpfname, $map, $gene_sets);
	venndiagram_gene_sets_bw($tmpfname_bw, $map, $gene_sets);
	}
	
	$content.= '<form method="post" name="aform" action="'.$base_url.'/?q=ecosearch/gene/search'.'">
				<input type="hidden" name="egid" value = "">	
				<map name="map1">';
		for ($t=0;$t<count($map);$t++)
		{

		$searchegs="Null,";
		foreach($gene_sets[$t] as $value) {
			$searchegs = $searchegs.$value.",";
		}
		//remove "," of last item
		$searchegs = strrev(substr(strrev($searchegs),1));
		
			$egid = $searchegs; 
			$content.= '<area name="area'.$t.'" 
			coords="'.$map[$t]['x1'].','.$map[$t]['y1'].','.$map[$t]['x2'].','.$map[$t]['y2'].'" 
			title="'.$title[$t].'" href="javascript: document.aform.submit()" onClick="document.aform.egid.value=\''.$egid.'\'">';


		}
		$content.="</map></form>";
		
		$content.= '<div id="venn"><img SRC="'.$tmpfname.'" usemap="#map1" /></div>';
				$content .= '<div class="EG_CollapsibleMap_TitleDiv">
					<div>
						<a class="EG_CollapsibleMap_TitleAhref" href="javascript:void(0)" onclick="toggleBlockDiv(\'venn_bw\', \'bw_vene_CollapsibleMap\');">
						<img id="bw_vene_CollapsibleMap" class ="cl_CollapsibleArea_expanding" src="sites/all/images/empty.gif" border="1">
						<span class="EG_CollapsibleMap_Title">Display black white version</span></a>
						<div class="EG_CollapsibleMap_HrDiv"> <hr class="EG_CollapsibleMap_Hr"></div>
					</div></div>';
		$content .= '<div id="venn_bw" style="display: none;"><img SRC="'.$tmpfname_bw.'" usemap="#map1" /></div>';
				
		return $content;
}
function search_topic_conditions($filter = array(),&$query)
{
	$topicTable='t_topic';
	
	if(empty($filter))	return;
	
	if(isset($filter['topictext'])) $text = $filter['topictext'];
//	else return ;
    		
	foreach ($filter as $key => $value) {
	
		if($key ==='topicradios')
    	{	$query_sub = array();
    		$params_sub = array();
    
    		switch ($value)
    		{
    			
    			case 'title':
    				$params_sub = preg_split("/[.,; ]+/",$text);
    						
    				if(!empty($params_sub))
    				{
    					foreach ($params_sub as $value_sub) {
    						$value_sub = '%%'.$value_sub.'%%';
    						$query->condition(db_or()->condition($topicTable.'.topic_name', $value_sub, 'LIKE')->condition($topicTable.'.topic_title', $value_sub, 'LIKE'));
    					}
    				}
	
    				break; 
    			
    			case 'alltext':
    				$params_sub = preg_split("/[.,; ]+/",$text);
    						
    				if(!empty($params_sub))
    				{
    					foreach ($params_sub as $value_sub) {
    						$value_sub = '%%'.$value_sub.'%%';
    						$query->condition(db_or()->condition($topicTable.'.topic_name', $value_sub, 'LIKE')->condition($topicTable.'.topic_title', $value_sub, 'LIKE')->condition($topicTable.'.topic_text', $value_sub, 'LIKE')->condition($topicTable.'.topic_description', $value_sub, 'LIKE'));
    						    						
    					}
    				}
	
    				break; 	 
    			
    		}
    	}
    	
  		if($key ==='topicids' && strlen(trim($value))>0)
    	{	$query_sub = array();
    		$params_sub = array();
    		$params_sub = preg_split("/[.,; ]+/",$value);
    		if(!empty($params_sub))
    		{	
    			$query->condition($topicTable.'.topic_id', $params_sub, 'IN');
    		}
    		
    	}
	if($key ==='topic_id_array')
    	{	
    		if(!empty($value))
    		{
    				$query->condition($topicTable.'.topic_id', $value, 'IN');
    		}
    		
    	}
	}
}
function ecogene_topic_all_content()
{

	
	global $base_url;
	$item_per_page = isset($_REQUEST['item_per_page'])? $_REQUEST['item_per_page']: 20;
	$page = isset($_REQUEST['page'])? $_REQUEST['page'] : 0;
	db_set_active("ecogene");

	$header = array(
	array('data' => t('Topic ID'), 'field' => 't_topic.topic_id'),
	array('data' => t('Name'), 'field' => 't_topic.topic_name'),
	array('data' => t('Gene Set'), 'field' => 'num_gene'),
	array('data' => t('Super Topics'), 'field' => 'num_sup'),
	array('data' => t('Sub Topics'), 'field' => 'num_sub'),
	array('data' => t('Type'), 'field' => 't_topic.topic_type')
	);
	
	$query_count = db_select('t_topic')->fields('t_topic');
	
	search_topic_conditions($_REQUEST,$query_count);
	
	
	$query_count =  $query_count->countQuery()->execute()->fetchField();
	$item_per_page = is_numeric($item_per_page)? $item_per_page: $query_count;
	
	
	if(is_numeric($item_per_page))
	{ 
		$query = db_select('t_topic')-> extend('PagerDefault')-> extend('TableSort') ;	
		$query->limit($item_per_page)->orderByHeader($header);
		
	}else{
		$query = db_select('t_topic')->extend('TableSort') ;		
		$query->orderByHeader($header);
	}
		$query -> leftJoin('t_topic_gene_link', 'tg', 't_topic.topic_id=tg.topic_id'); 
		$query -> leftJoin('t_gene', 't_gene', 't_gene.eg_id = tg.eg_id');
		$query -> leftJoin('t_topic_topic_link', 'tt', ' t_topic.topic_id=tt.topic_id');
		$query -> leftJoin('t_topic_topic_link', 'tt2', 't_topic.topic_id=tt2.linked_topic_id');
	
		$query -> addExpression('count(distinct tg.eg_id )', 'num_gene');
		$query -> addExpression('count(distinct tt2.topic_id )', 'num_sub');
		$query -> addExpression('count(distinct tt.linked_topic_id )', 'num_sup');
		$query->fields('t_topic');
		$query->groupBy('t_topic.topic_id');
		
	search_topic_conditions($_REQUEST,$query);
	
	
	$records = array();
	$records = $query->execute()->fetchAll();
	

	if(!empty($records))
	{
		$start_num = $page*$item_per_page+1;
		$count_on_page = ($item_per_page>count($records))? count($records):$item_per_page;
		$stop_num = $page*$item_per_page+$count_on_page;
		
		if($query_count>=$count_on_page)
		{
			
			$caption = 'Showing '.$start_num.'-'.$stop_num.' of '.$query_count.' topics';
		}
		else 
		{
			$caption = 'Showing '.$start_num.'-'.$stop_num.' of '.$query_count.' topics';
		}
	foreach ($records as $item) {		
					
   		$num_gene = $item->num_gene;
   		$num_sub = $item->num_sub;
   		$num_sup = $item->num_sup;
   		$rows[] = array(
   		("<a href='$base_url/?q=topic/$item->topic_id'>".$item->topic_id."</a>"),
		("<a href='$base_url/?q=topic/$item->topic_id'>".$item->topic_name."</a>"),
		("<a href='$base_url/?q=ecosearch/gene/search&search_topic=$item->topic_id'>".$num_gene."</a>"),
		("<a href='$base_url/?q=topic/$item->topic_id'>".$num_sup."</a>"),
		("<a href='$base_url/?q=topic/$item->topic_id'>".$num_sub."</a>"),
		($item->topic_type),
		);	
	}
	db_set_active("default");
	$variables = array('header'=>$header, 'rows'=>$rows, 'sticky'=>NULL, 'empty'=>NULL, 'caption'=>$caption, 'colgroups'=>array(), 'attributes'=>array());
	$content = theme_table($variables);
	}else {
		$content = '<em>No record has been found!</em>';
		
	}
	db_set_active("default");
	

	
		$build['content'] = array(
		'this_does_not_matter_too_much2' 
			=> array(
				'form' => drupal_get_form('ecogene_topic_search_form'),
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
function array_topic_gene_query()
{
	global $base_url;
	$item_per_page = isset($_REQUEST['item_per_page'])? $_REQUEST['item_per_page']: 20;
	
	db_set_active("default");

	$content = '<form method="post">';
	
	$content .='<input class="form-submit" type="submit" value="Choose Topic Gene Set" title="Choose gene sets associated with topics" 
				onClick="array_topic_gene_set(\''.$base_url.'\', this.form, \'topic\')">';
	$content .='<input class="form-submit" onClick="array_topic_gene_set(\''.$base_url.'\', this.form)" type="submit" name="topic_gene_query_logic" value="In all chosen">';
	$content .='<input class="form-submit" onClick="array_topic_gene_set(\''.$base_url.'\', this.form)" type="submit" name="topic_gene_query_logic" value="In any chosen">';
	$content .='<input class="form-submit" onClick="array_topic_gene_set(\''.$base_url.'\', this.form)" type="submit" name="topic_gene_query_logic" value="Not in chosen">';
	$content .='<input class="form-submit" onClick="array_topic_gene_set(\''.$base_url.'\', this.form, \'venn\')" type="submit" value="Show Venn diagram" title="Choose 3 sets then click. Only works for 2 or 3 sets" onClick="gene_set_query_form_submit(\''.$base_url.'\', this.form)">';

	$content_2 = select_array_gene();
	
	
	if(isset($_REQUEST['array_exp_ids']))	
	{
		$array_ids = $_REQUEST['array_exp_ids'];
		
		if(is_string($array_ids))
		{
			$array_ids = explode(' ', trim($array_ids));
		}
		$array_ids = array_unique($array_ids);
		$n=0;
		foreach ($array_ids as $value) {
			$value_array = explode(',', trim($value));
   			$pubmed_id[$n] = $value_array[0];
   			$exp_id[$n] = $value_array[1];
   					
   			$n++;

		}
		if(!empty($pubmed_id)&& ! empty($exp_id))
		{
    		$content .= select_array_gene($pubmed_id, $exp_id);
		}
		$content .='<input class="form-submit" type="submit" value="Update Selection" title="Choose more gene sets associated with Ecoarray Experiments" 
				onClick="array_topic_gene_set(\''.$base_url.'\', this.form, \'array\')">';
	}else{
		$content .='<input class="form-submit" type="submit" value="Show Selection" title="Choose more gene sets associated with Ecoarray Experiments" 
				onClick="array_topic_gene_set(\''.$base_url.'\', this.form, \'array\')">';
	}
	
	
	
		
	$content_3 = '</form>';
	
	
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
        'this_does_not_matter_too_much3' 
			=> array(
			'#type' => 'markup',
            '#markup' => $content_2,
        ),
        'this_does_not_matter_too_much4' 
			=> array(
			'#type' => 'markup',
            '#markup' => $content_3,
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
function array_experiment_gene_query(&$records, &$item_per_page, $header, $pubmed_id=array(), $exp_id=array())
{
	global $base_url;

	
	$page = isset($_REQUEST['page'])? $_REQUEST['page'] : 0;
	$item_per_page = isset($_REQUEST['item_per_page'])? $_REQUEST['item_per_page']: 20;
//	
	db_set_active("ecogene");
	
	$query_count =  db_select('t_array_exp', 'exp');
	$query_count -> join('t_array_data', 't_data', 't_data.exp_id = exp.exp_id and t_data.pubmed_id = exp.pubmed_id');
	$query_count -> addExpression('count(distinct t_data.eg_id )', 'num_gene');
	
	// needed fields
	search_experiment_conditions($_REQUEST,$query_count);

	
		// base class
	$query = db_select('t_array_exp', 'exp')-> extend('PagerDefault')-> extend('TableSort') ;
	
	// join to the reference, author and link table
	
	$query -> join('t_array', 't_array', 'exp.pubmed_id = t_array.pubmed_id');
	$query -> join('t_reference', 'r', 'exp.pubmed_id = r.pubmed_id');
	$query -> join('t_reference_author_link', 'ral', 'ral.pubmed_id = r.pubmed_id');
	$query -> join('t_author', 'author', 'author.author_id = ral.author_id');
	$query -> join('t_array_data', 't_data', 't_data.exp_id = exp.exp_id and t_data.pubmed_id = exp.pubmed_id');
	$query -> addExpression('count(distinct t_data.eg_id )', 'num_gene');
	
	// needed fields
	
	$query -> fields('exp');
	$query -> addField('author', 'last_name', 'lastname'); 
	$query -> addField('t_array', 'platform', 'platform');
	$query -> addExpression('year(r.date_published)', 'year_published');
	$query -> condition('ral.seq_no', 1);
	search_experiment_conditions($_REQUEST,$query);
	

	if(!empty($pubmed_id) && !empty($exp_id))	
	{
		$sub_q = db_or();
		
		for($i=0; $i<count($pubmed_id);$i++)
		{
			
			$sub_q -> condition(db_and()->condition('exp.pubmed_id', $pubmed_id[$i])->condition('exp.exp_id', $exp_id[$i]));
		}
		$query_count->condition($sub_q);
		$query->condition($sub_q);
//		$query_count->condition('exp.pubmed_id',$pubmed_id,'IN');
//		$query_count->condition('exp.exp_id',$exp_id,'IN');
//		$query->condition('exp.pubmed_id',$pubmed_id,'IN');
//		$query->condition('exp.exp_id',$exp_id,'IN');
	}
	
	$query_count->groupBy('exp.pubmed_id, exp.exp_id');
	$query->groupBy('exp.pubmed_id, exp.exp_id');
	
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
function select_array_gene($pubmed_id=array(), $exp_id=array())
{
	global $base_url;
	$page = isset($_REQUEST['page'])? $_REQUEST['page'] : 0;
	$item_per_page = isset($_REQUEST['item_per_page'])? $_REQUEST['item_per_page']: 20;
	
	$records = array();
	$header = array(
		array('data' => t('Reference'), 'field' => 'r.date_published'),
		array('data' => t('Variable')),
		array('data' => t('Platform'),'field' => 't_array.platform'),
		array('data' => t('Exp type and ID'),'field' => 'exp.exp_type'),
		array('data' => t('Gene Set'), 'field' => 'num_gene',
		'sort' => 'desc'),
	);
	$query_count = array_experiment_gene_query($records,$item_per_page, $header, $pubmed_id, $exp_id);
	
	
	
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
		if(!empty($pubmed_id) && !empty($exp_id))	
		{	
			$gene_set_sel = ($item->num_gene>0)? '<a class="gene_set_number" href="'.$base_url.'/?q=ecoarray/experiment/'.$item->pubmed_id.'/'.$item->exp_id.'">'.$item->num_gene.'</a>'
												.'<input type="checkbox" name="array_exp_ids[]" checked value="'.$item->pubmed_id.','.$item->exp_id.'"'
   							:'0';
		}
   		else
   		{
   			$gene_set_sel = ($item->num_gene>0)? '<a class="gene_set_number" href="'.$base_url.'/?q=ecoarray/experiment/'.$item->pubmed_id.'/'.$item->exp_id.'">'.$item->num_gene.'</a>'
												.'<input type="checkbox" name="array_exp_ids[]" value="'.$item->pubmed_id.','.$item->exp_id.'"'
   							:'0';
   		}					
		$rows[] = array(
		
			'<a href="http://www.ncbi.nlm.nih.gov/entrez/query.fcgi?cmd=Retrieve&db=PubMed&list_uids='.
			$item->pubmed_id.'&dopt=Abstract">'.$item->lastname.','.$item->year_published.'</a>',
		(displayed_name($item)),
		$item->platform,
		t('<a href="'.$base_url.'?q=ecoarray/paper/'.$item->pubmed_id.'" title="show experiment detail">'
			.$item->exp_type.' '.$item->exp_id.'</a>'),
		$gene_set_sel,	
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
	return $content;
}
function select_topic_gene($request)
{
//	if(!empty($topic_id_array))	{
//		$request = array_merge($_REQUEST, array('topic_id_array'=> $topic_id_array));
//	}else{
//		$request = $_REQUEST;
//	}
	
	
	global $base_url;
	$item_per_page = isset($_REQUEST['item_per_page'])? $_REQUEST['item_per_page']: 20;
	$page = isset($_REQUEST['page'])? $_REQUEST['page'] : 0;
	db_set_active("ecogene");

	$header = array(
	array('data' => t('Topic ID'), 'field' => 't_topic.topic_id'),
	array('data' => t('Name'), 'field' => 't_topic.topic_name'),
	array('data' => t('Type'), 'field' => 't_topic.topic_type'),
	array('data' => t('Gene Set & Select'), 'field' => 'num_gene',
	'sort' => 'desc'),
	);
	
	$query_count = db_select('t_topic');
	$query_count->join('t_topic_gene_link', 'tg', 't_topic.topic_id=tg.topic_id');
	$query_count ->join('t_gene', 't_gene', 't_gene.eg_id = tg.eg_id');
	$query_count ->fields('t_topic');
	
	search_topic_conditions($request,$query_count);
	$query_count->groupBy('t_topic.topic_id');
	
	$query_count =  $query_count->countQuery()->execute()->fetchField();
	$item_per_page = is_numeric($item_per_page)? $item_per_page: $query_count;
	
	$query = db_select('t_topic')-> extend('PagerDefault')-> extend('TableSort') ;
	$query -> join('t_topic_gene_link', 'tg', 't_topic.topic_id=tg.topic_id'); 
	$query -> join('t_gene', 't_gene', 't_gene.eg_id = tg.eg_id');
	$query -> addExpression('count(distinct tg.eg_id )', 'num_gene');
	$query->fields('t_topic');
	$query->groupBy('t_topic.topic_id');
	
	$query->limit($item_per_page)->orderByHeader($header);
	
	search_topic_conditions($request,$query);
		
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
			
			$caption = 'Showing '.$start_num.'-'.$stop_num.' of '.$query_count.' topics';
		}
		else 
		{
			$caption = 'Showing '.$start_num.'-'.$stop_num.' of '.$query_count.' topics';
		}
	foreach ($records as $item) {		
					
   		$num_gene = $item->num_gene;
//   		$num_sub = $item->num_sub;
//   		$num_sup = $item->num_sup;
   		if(isset($request['topic_id_array']))	
		{	
			$gene_set_sel = ($item->num_gene>0)? '<a class="gene_set_number" href="'.$base_url.'/?q=ecosearch/gene/search&search_topic='.$item->topic_id.'">'.$num_gene.'</a>'.'<input type="checkbox" checked name="topic_ids[]" value="'.$item->topic_id.'"'
   		:'0';
		}
   		else
   		{
   			$gene_set_sel = ($item->num_gene>0)? '<a class="gene_set_number" href="'.$base_url.'/?q=ecosearch/gene/search&search_topic='.$item->topic_id.'">'.$num_gene.'</a>'.'<input type="checkbox" name="topic_ids[]" value="'.$item->topic_id.'"'
   		:'0';
   		}	
   		
   		
   		$rows[] = array(
   		("<a href='$base_url/?q=topic/$item->topic_id'>".$item->topic_id."</a>"),
		("<a href='$base_url/?q=topic/$item->topic_id'>".$item->topic_name."</a>"),
		($item->topic_type),
		$gene_set_sel,
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
	
	}else {
		$content = '<em>No record has been found!</em>';
	}
	
	

	

	return $content;
}
function ecogene_topic_gene_query()
{
	global $base_url;
	$item_per_page = isset($_REQUEST['item_per_page'])? $_REQUEST['item_per_page']: 20;

	db_set_active("default");
	$content_3 = "";
	$content = '<form method="post" name="topic_gene_set">';
	
// Check if the page has ecoarray experiments paseed by.
	
		
	$content .='<input class="form-submit" onClick="array_topic_gene_set(\''.$base_url.'\', this.form)" type="submit" name="topic_gene_query_logic" value="In all chosen">';
	$content .='<input class="form-submit" onClick="array_topic_gene_set(\''.$base_url.'\', this.form)" type="submit" name="topic_gene_query_logic" value="In any chosen">';
	$content .='<input class="form-submit" onClick="array_topic_gene_set(\''.$base_url.'\', this.form)" type="submit" name="topic_gene_query_logic" value="Not in chosen">';
	$content .='<input class="form-submit" onClick="array_topic_gene_set(\''.$base_url.'\', this.form, \'venn\')" type="submit" value="Show Venn diagram" title="Choose 3 sets then click. Only works for 2 or 3 sets" onClick="gene_set_query_form_submit(\''.$base_url.'\', this.form)">';
	
	
	
	$content_2 = select_topic_gene($_REQUEST);

	if(isset($_REQUEST['array_exp_ids']))	
	{
		$array_ids = $_REQUEST['array_exp_ids'];
		
		if(is_string($array_ids))
		{
			$array_ids = explode(' ', trim($array_ids));
		}
		$array_ids = array_unique($array_ids);
		
		$n=0;
		foreach ($array_ids as $value) {
			$value_array = explode(',', trim($value));
   			$pubmed_id[$n] = $value_array[0];
   			$exp_id[$n] = $value_array[1];
   					
   			$n++;

		}
		if(!empty($pubmed_id)&& ! empty($exp_id))
		{
    		$content .= select_array_gene($pubmed_id, $exp_id);
		}
		
	}
	
	if(isset($_REQUEST['topic_ids']))	
	{
		
		$topic_ids = $_REQUEST['topic_ids'];
		if(is_string($topic_ids))
		{
			$topic_ids = explode(' ', trim($topic_ids));
		}
		
		$content .= select_topic_gene(array('topic_id_array'=> $topic_ids));
	
		$content .='<input class="form-submit" type="submit" value="Update Selection" title="Choose more gene sets associated with Topics" 
				onClick="array_topic_gene_set(\''.$base_url.'\', this.form, \'topic\')">';
	}else{
		$content .='<br /><input class="form-submit" type="submit" value="Show Selection" title="Choose more gene sets associated with Topics" 
				onClick="array_topic_gene_set(\''.$base_url.'\', this.form, \'topic\')">';
	}
	
	$content_3 .= '</form>';	
	$build['content'] = array(
		'this_does_not_matter_too_much2' 
			=> array(
				'form' => drupal_get_form('ecogene_topic_gene_query_form'),
			),
        'this_does_not_matter_too_much1' 
			=> array(
			'#type' => 'markup',
            '#markup' => $content,
        ),
        'this_does_not_matter_too_much3' 
			=> array(
			'#type' => 'markup',
            '#markup' => $content_2,
        ),
        'this_does_not_matter_too_much4' 
			=> array(
			'#type' => 'markup',
            '#markup' => $content_3,
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

function ecogene_topic_content($topic_id) {
	global $base_url;
	// Make sure you don't trust the URL to be safe! Always check for exploits.
	if (!is_numeric($topic_id)) {
		// We will just show a standard "access denied" page in this case.
		return drupal_access_denied();
	}
	
	$args = array();
	$args['topic_id'] = $topic_id;
	
	db_set_active("ecogene");
	$result = db_query("SELECT * FROM {t_topic} WHERE topic_id=:topic_id",$args);
	
	
	$content = '';
	$row = $result->fetchAssoc();
	if($row)
	{
		$n_genes = db_query("SELECT count(*) FROM {t_topic_gene_link} WHERE topic_id=:topic_id",$args)->fetchField();
		
		$content .= '<div id="pic_des_container"><div id="topic_gene_pic">';
//		$content .= '<div id="pic_des_container"><div id="topic_gene_pic"><a HREF="'.$base_url.'?q=/ecosearch/gene/search&search_topic='.$topic_id.'">
		if($n_genes<250)
		{
			$content .='<a HREF="http://www.ecogene.org/genemap/map.php?search_topic='.$topic_id.'">
					<img width="90" height="90" src="'.$GLOBALS['base_url'].'/sites/all/include/topic_map.php?topic_id='.$topic_id.'" alt="View Genes On Map"></a></div>';
		}
		else 
		{
			$content .=' <img title="Only GeneSet with up to 250 genes can be viewed on circle map" width="90" height="90" src="'.$GLOBALS['base_url'].'/sites/all/include/topic_map.php?topic_id='.$topic_id.'" alt="View Genes On Map"></div>';
		}
		$content .= '<div id="topic_description">';
		$content .= '<H2>Description:<br></H2><p>';

		
		$row['topic_description'] = str_ireplace("intergenedownload.php?type=", "?q=ecodownload/intergenedownload/", $row['topic_description']);
		$row['topic_text'] = str_ireplace("InterGeneTable.php", "?q=ecodownload/intergene/", $row['topic_text']);
		$row['topic_text'] = str_ireplace("InterGeneTable.php", "?q=ecodownload/intergene/", $row['topic_text']);
		
		$content .= $row['topic_description'].'</p></div></div><div id="topic_detail">';
		$content .= "<H2>Details:<br></H2><p>";
		$content .= $row['topic_text'];
		$content .='</p></div>';
	}
	
	db_set_active("default");
	return $content;
	
}

function ecogene_topic_reference($topic_id,$review) {
	// Make sure you don't trust the URL to be safe! Always check for exploits.
	if (!is_numeric($topic_id)) {
		// We will just show a standard "access denied" page in this case.
		return drupal_access_denied();
	}
	$content = '';
	db_set_active("ecogene");
	//query to get the total number of refrence;
	$args = array();
	$args['topic_id'] = $topic_id;
	$result = db_query("SELECT COUNT(pubmed_id) total FROM {t_topic_reference_link} WHERE topic_id =:topic_id ",$args);	
	
	if($row = $result->fetchAssoc())
		$total = $row['total'];
	
		
	$query_topic_reference_link = "SELECT ".
			"r.title, ".
			"date_format(r.date_published,'%Y') year, ".
			"r.pubmed_id, r.medline_id, pagination, journal, volume, ".
		 	"concat(a.last_name,' ',a.initials) as author_name, r.xml_link, r.publication_type ".
		 "FROM ".
			"t_reference r,t_topic_reference_link grl, ".
			"t_reference_author_link ral, t_author a ".
		 "WHERE ".
			"grl.topic_id = :topic_id and ".
			"grl.pubmed_id = r.pubmed_id and ".
			"r.pubmed_id = ral.pubmed_id and ".
			"ral.author_id = a.author_id ";
	
	if($review=='review')
    	$query_topic_reference_link = $query_topic_reference_link." and ".
				"( r.publication_type like '%Review%' or ".
				" r.publication_type like '%Comment%' ) ";		     

    $query_topic_reference_link = $query_topic_reference_link."ORDER BY ".
			"date_published desc, pubmed_id desc,title asc,ral.seq_no asc ";
			
	$result = db_query($query_topic_reference_link,$args);
//	
    
    $content = $content. "<H2>Bibliography (".$total." total) : ";
 	if($review=='reference')
 	{
    	$content = $content. " <A HREF='?q=topic/$topic_id/review"."' >Review Only"."</A></H2>";
 	}
 	if($review=='review')
 	{
    	$content = $content. "<A HREF='?q=topic/$topic_id/reference"."' >All References"." </A></H2>";
 	}
	
 	$row_previous = $result->fetchAssoc();
 	$author_name = $row_previous["author_name"];
 	unset($years);
 	unset($lists);
	while ($row = $result->fetchAssoc())
	{	
		
		if ($row_previous["pubmed_id"] == $row["pubmed_id"]) {
			$row["author_name"] = $row_previous["author_name"]. ", ".$row["author_name"];
		} else 
		{
			$list = "<font>".$row_previous["author_name"]. " (".$row_previous['year'].") ".$row_previous["title"]." ".$row_previous["journal"]." ".$row_previous["volume"].":".$row_previous["pagination"]."</font>";
			if ( substr_count($row_previous["publication_type"], 'Review')> 0 )
			{
				$list = $list. " <font color='#3333FF'>Review</font>";
			}
			else if ( substr_count($row_previous["publication_type"], 'Comment')>0 )
			{
				$list = $list. " <font color='#3333FF'>Comment</font>";
			}
			if ($row_previous["xml_link"]=='Y') {
				$list = "<A HREF='http://www.ncbi.nlm.nih.gov/entrez/query.fcgi?cmd=Retrieve&db=PubMed&list_uids=".$row_previous["pubmed_id"]."&dopt=Abstract&target='v'>".$list."</A>";
			}
			$lists[]=$list;
			$years[]=$row_previous['year'];
		}
		$row_previous = $row;
	}
	
	
	if($row_previous)
	{
		$list = "<font>".$row_previous["author_name"]. " (".$row_previous['year'].") ".$row_previous["title"]." ".$row_previous["journal"]." ".$row_previous["volume"].":".$row_previous["pagination"]."</font>";
			if ( substr_count($row_previous["publication_type"], 'Review')> 0 )
			{
				$list = $list. " <font color='#3333FF'>Review</font>";
			}
			else if ( substr_count($row_previous["publication_type"], 'Comment')>0 )
			{
				$list = $list. " <font color='#3333FF'>Comment</font>";
			}	
			if ($row_previous["xml_link"]=='Y') {
				$list = "<A HREF='http://www.ncbi.nlm.nih.gov/entrez/query.fcgi?cmd=Retrieve&db=PubMed&list_uids=".$row_previous["pubmed_id"]."&dopt=Abstract&target='v'>".$list."</A>";
			}
			$lists[]=$list;
			$years[]=$row_previous['year'];
	}
	
	for ($i=0;$i<count($years);$i++)
	{
		$refs[]=$lists[$i];
		if($i==count($years)-1)
		{
			
			$content = $content.theme_item_list(array('items'=>$refs, 'title'=>$years[$i], 'type'=>'ol', 'attributes' => array()));
			unset($refs);

		}elseif ($years[$i]!=$years[$i+1])
		{
			
			$variables = array('items'=>$refs, 'title'=>$years[$i], 'type'=>'ol', 'attributes' => array());
			$items = $variables['items'];
			$content = $content.theme_item_list($variables);
			unset($refs);

		}
	}





db_set_active("default");
return $content;
}

