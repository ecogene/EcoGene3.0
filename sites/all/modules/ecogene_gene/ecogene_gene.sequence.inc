<?PHP
include("sites/all/include/format_description.php");
include("sites/all/include/ecoFunction.php");
define ("SEQ_LINE", 75);

function ecogene_gene_dnasequence($eg_id) {
	global $base_url; 
	db_set_active("ecogene");
	
	$string ="/gene/$eg_id/dnasequence". '?'.drupal_http_build_query(drupal_get_query_parameters($_REQUEST, array_merge(array('type','eg_id','tfbs_chk','tfbs_submit'), array_keys($_COOKIE))));
	$content = '';
	$string_destination = '?'.drupal_http_build_query(drupal_get_query_parameters($_REQUEST, array_merge(array('eg_id'), array_keys($_COOKIE))));

//	$type = isset($_REQUEST['type'])? $_REQUEST['type']:"";
	$us = isset($_REQUEST['us'])? $_REQUEST['us']: 0;
	$ds = isset($_REQUEST['ds'])? $_REQUEST['ds']: 0;
	
	if ( isset($_REQUEST['type']) )
	{
		$type = $_REQUEST['type'];
	}elseif( isset($_REQUEST['join_text']) && isset($_REQUEST['join_text']) != "" )
	{
		$type = "coor";
	}else {
		$type = "";
	}
	// decide if the 'DNA Sequence'/'Reconstruction' link toggle is needed
	$dna_toggle_link = '';
	if ( isset($_REQUEST['join_text']) && $_REQUEST['join_text'] != "" )
	{
//		var_dump($_REQUEST);
		// link with 'DNA Sequence' is needed
		$url_para = drupal_get_query_parameters($_REQUEST, array_merge(array('submit','q', 'eg_id','tfbs_chk','tfbs_submit','join_html_text', 'join_title', 'join_text'), array_keys($_COOKIE)));
		$url_para['type'] = $type;		
		
		$toggle_url =  $base_url."/gene/$eg_id/dnasequence?".drupal_http_build_query($url_para);
		$dna_toggle_link = "<div style='text-align:right'><a style='color:#E72801' href=\"".$toggle_url."\">DNA Sequence</a></div>" ;
//		echo $dna_toggle_link;
	}
	else 
	{
		$gene_joint = gene_join_info($eg_id);
		if ( !empty($gene_joint))
		{
			// link with 'Reconstruction' is needed
			
			$url_para = drupal_get_query_parameters($_REQUEST, array_merge(array('submit','q', 'type','eg_id','tfbs_chk','tfbs_submit','join_html_text', 'join_title', 'join_text'), array_keys($_COOKIE)));
			$url_para['type'] = $type;
			$url_para['join_html_text'] = $gene_joint['join_html_text'];
			$url_para['join_text'] = $gene_joint['join_text'];
				
			$toggle_url =  $base_url."/gene/$eg_id/dnasequence?".drupal_http_build_query($url_para);
			$dna_toggle_link = "<div style='text-align:right'><a style='color:#E72801' href=\"".$toggle_url."\">Reconstruction</a></div>" ;
//			echo $dna_toggle_link;
		
		}
	}
	
	switch ($type)
	{
		case 'coor':
			$content.= "<ul class=\"tabs secondary\">
				<li><a href=\"".$base_url.$string."&type=fasta\">FASTA</a></li>
				<li class=\"active\"><a class=\"active\" href=\"".$base_url.$string."&type=coor\">Coordinates</a></li>
				<li><a href=\"".$base_url.$string."&type=tfbs\">TFBS</a></li>
				<li><a href=\"".$base_url.$string."&type=site\">SITES</a></li>
				</ul>";
				$content=$content. "<form name=dnaForm action=$string_destination method=\"post\"><input type=submit value=submit>&nbsp # bp Upstream <input type=text name=us value= $us size=3># bp Downstream <input type=text name=ds value=$ds size=3><BR>"; 
				$content=$content. "</form>";
			$content.=$dna_toggle_link;
			$content.=print_labeled_seq($eg_id);
				
			break;
		case 'tfbs':
			$content.= "<ul class=\"tabs secondary\">
				<li><a href=\"".$base_url.$string."&type=fasta\">FASTA</a></li>
				<li><a href=\"".$base_url.$string."&type=coor\">Coordinates</a></li>
				<li class=\"active\"><a class=\"active\" href=\"".$base_url.$string."&type=tfbs\">TFBS</a></li>
				<li><a href=\"".$base_url.$string."&type=site\">SITES</a></li>
				</ul>";
				$content=$content. "<form name=dnaForm action=$string_destination method=\"post\"><input type=submit value=submit>&nbsp # bp Upstream <input type=text name=us value=$us size=3># bp Downstream <input type=text name=ds value=$ds size=3><BR>"; 
			$content=$content. "</form>";
			$content.=$dna_toggle_link;	
			$content.=print_labeled_tfbs_seq($eg_id);
			break;
		case 'site':
			$content.= "<ul class=\"tabs secondary\">
				<li><a href=\"".$base_url.$string."&type=fasta\">FASTA</a></li>
				<li><a href=\"".$base_url.$string."&type=coor\">Coordinates</a></li>
				<li><a href=\"".$base_url.$string."&type=tfbs\">TFBS</a></li>
				<li class=\"active\"><a class=\"active\" href=\"".$base_url.$string."&type=site\">SITES</a></li>
				</ul>";
				$content=$content. "<form name=dnaForm action=$string_destination method=\"post\"><input type=submit value=submit>&nbsp # bp Upstream <input type=text name=us value=$us size=3># bp Downstream <input type=text name=ds value=$ds size=3><BR>"; 
				$content=$content. "</form>";	
			$content.=$dna_toggle_link;
			$content.=print_labeled_site_seq($eg_id);
			break;
		case 'fasta':		
		default:
			$content.= "<ul class=\"tabs secondary\">
				<li class=\"active\"><a class=\"active\" href=\"".$base_url.$string."&type=fasta\">FASTA</a></li>
				<li><a href=\"".$base_url.$string."&type=coor\">Coordinates</a></li>
				<li><a href=\"".$base_url.$string."&type=tfbs\">TFBS</a></li>
				<li><a href=\"".$base_url.$string."&type=site\">SITES</a></li>
				</ul>";
				$content=$content. "<form name=dnaForm action=$string_destination method=\"post\">
				<input type=submit value=submit>&nbsp # bp Upstream <input type=text name=us value=$us size=3># bp Downstream <input type=text name=ds value=$ds size=3><BR>"; 
				$content=$content. "</form>";
			$content.=$dna_toggle_link;	
			$content.=print_seq($eg_id);
			
			break;	
	}

		
	db_set_active("default");
	return $content;
}
function gene_join_info($eg_id) 
{
	// query for basic gene information
	$query = db_query(
	
		"SELECT g.name 
		FROM t_gene g 
		WHERE g.eg_id =:eg_id",
		array(':eg_id'=>$eg_id)
	);
	
	$gene_info = $query->fetchAssoc();	

	$join_title = '';
	$join_text = '';
	$html_text = '';	
	
    ecogene_gene_get_join($eg_id,$join_title,$join_text, $html_text, $gene_info['name']);	
    	
	if ( !(empty ($join_text) || empty ($join_title)))
	{
		$gene_info ['join_html_text'] = $html_text;
		$gene_info ['join_title'] = $join_title;
		$gene_info ['join_text'] = $join_text;		
	}
	else 
	{
		$gene_info = array();
	}
	
	return $gene_info;

}
function get_random_color()
{
	
	for ($x = 0; $x < 3; $x++) {
		if($x==0)
			$rand = rand(0,200); // to avoid red or white color
		else
			$rand = rand(0,255);
		$hex[$x] = sprintf ("%x", $rand);
		if ($rand <= 9) {
			$hex[$x] = "0" . $hex[$x];
		}
		if ($rand > 9 && $rand < 16) {
			$hex[$x] = "0" . $hex[$x];
		}
	}
	$colors = "#" . $hex[0] . $hex[1] . $hex[2];
	
	return $colors;
}

function print_labeled_site_seq($eg_id)
{
	global $GENOME_LENGTH;
	$string = '?'.drupal_http_build_query(drupal_get_query_parameters($_REQUEST, array_merge(array('eg_id','site_chk'), array_keys($_COOKIE))));
//	$string = '?'.	drupal_get_query_parameters($_REQUEST, array_merge(array('eg_id','site_chk'), array_keys($_COOKIE)));
	
	$title = "";
	$down_string="";
	$up_string="";
	$main_string="";
	$join_text="";
	$join_pos = "";
	
	$ds= isset($_REQUEST['ds']) ? $_REQUEST['ds']:0;
	$us= isset($_REQUEST['us']) ? $_REQUEST['us']:0;
	$join_text= isset($_REQUEST['join_text']) ? $_REQUEST['join_text']:"";
	
	if( isset($_REQUEST['right_end']) && isset($_REQUEST['left_end']) )
	{
		$left_end = $_REQUEST['left_end'];
		$right_end = $_REQUEST['right_end'];
		if($left_end<=$right_end)
		{
			$query = db_query("Select substring(s.sequence,".$left_end.",".($right_end-$left_end).") as sequence from t_sequence s");
		}
		else 
		{
			$query = db_query("Select concat(substring(s.sequence,".$left_end.",".($GENOME_LENGTH-$left_end)."), substring(s.sequence,1,".($right_end).")) as sequence from t_sequence s");
		}
		$row = $query->fetchAssoc();
		$sequence = $row["sequence"];
	
		
		$left_pos  =	$left_end;
		$right_pos = 	$right_end;
		$up_string='';
		$down_string='';
		$main_string = $sequence;
		$orientation = 'Clockwise';

 		$pos_text="";		// Coordinates
 		$label_text="";	// Relative Coordinates according to the main stream
 
	}
	else 
	{
		get_string($down_string,$up_string,$main_string,$description,$addspace,$orientation, $left_pos, $right_pos,$es,$bs,$eg_id,$us,$ds,$join_text);
		
	}
	  
	  
	$sites_name_global = array();
	$sites_color_global = array();
	$sites_info=array();
	$sites_name = array();


	
	if(isset($_REQUEST['site_submit']))
	{	
		$site_chk = isset($_REQUEST['site_chk'])? $_REQUEST['site_chk']:array();
//		$site_seq_form.=print_r($_REQUEST);
		
		

	}else{
//		if( isset($_REQUEST['site_chk']))
//		{
		for ($k=0; $k<count($_REQUEST['sites']);$k++)	
		{			
			$site_chk[$k]=$_REQUEST['sites'][$k];			
		}		
//		}
	}
	$searchegs='';
	if(isset($_REQUEST['sites']))
	{
	for ($k=0; $k<count($_REQUEST['sites']);$k++)	
	{
		$searchegs = $searchegs."'".trim($_REQUEST['sites'][$k])."',";
	}
	}
	$searchegs = strrev(substr(strrev($searchegs),1));//remove "," of last item
	
	$site_seq_form="<form name=siteForm action=$string method=\"post\">
	<input type=hidden name=site_submit value=1>";
//	$site_seq_form.="<input type=hidden name=sites[] value='$sites'>";	
	
	if($searchegs!='')
//	get_sites_info(&$sites_info, &$sites_name, $left_pos,$searchegs, $up_string, $down_string, $main_string,$right_pos,$orientation);
		get_sites_info($sites_info, $sites_name, $left_pos,$searchegs, $up_string, $down_string, $main_string,$right_pos,$orientation);
	
	$title.= "<P style=\"font-size:14px \" id=\"sites_name\">	";	
	for ($j=0; $j<count($sites_name);$j++)
	{	
		$sites_color_global[$j] = get_random_color();
		$sites_name_global[$j] = $sites_name[$j];
		
		
		if (in_array($sites_name_global[$j],$site_chk))
		{
		$site_seq_form.= "<input type='checkbox'  name='site_chk[]'  value='$sites_name_global[$j]' onclick=javascript:document.siteForm.submit() checked><font style=\"background-color:".$sites_color_global[$j]."; font-family:'Courier New'; font-size:14px\" color=\"white\">".$sites_name_global[$j]." </font>";
		}
		else {
			$site_seq_form.= "<input type='checkbox' name='site_chk[]'  value='$sites_name_global[$j]' onclick=javascript:document.siteForm.submit() unchecked><font style=\"background-color:".$sites_color_global[$j]."; font-family:'Courier New'; font-size:14px\" color=\"white\">".$sites_name_global[$j]." </font>";
		}
		
	}
	$site_seq_form.= "&nbsp;&nbsp;<font style=\"background-color:red; font-family:'Courier New'; font-size:14px\" color=\"white\">Overlap</font>";
	$site_seq_form .=" <a href=\"javascript:;\" onclick=\"javascript:toggleProductInfoDiv('sites_overlay');toggleProductInfoDiv('modalBack'); return false;\" style='CURSOR: hand'>Sites Positions</a></P>";;
	
	$site_seq_form.="<div id=\"sites_overlay\" class=\"info_overlay\" style=\"display:none;\"> <div>
    <a href=\"javascript:;\" onclick=\"javascript:toggleProductInfoDiv('sites_overlay');toggleProductInfoDiv('modalBack'); return false;\" class=\"btn_close\">Close </a>
    <h3>Site Positions start at ";
	if($orientation=="Clockwise") $title.= $left_pos; else  $title.=$right_pos; 
	$site_seq_form.= "</h3> <p align=\"left\">	<table class='detail_description' style='width:80%' align='center'>
		<tr><td>Name</td><td>Length</td><td>Strand</td><td>Sequence</td><td>Left End Position</td></tr>";
	
	for ($j=0; $j<count($sites_info);$j++)
	{	
		if (in_array($sites_info[$j]["name"],$site_chk))
		$site_seq_form.= "<tr><td>".$sites_info[$j]["name"]."</td><td>".strlen($sites_info[$j]["seq"])."</td><td>".$sites_info[$j]["ori"]."</td><td>".$sites_info[$j]["seq"]."</td><td>".$sites_info[$j]["relative"]."</td></tr>";
		
		
	}	

$site_seq_form.="</table><p><a href=\"javascript:;\" onclick=\"javascript:toggleProductInfoDiv('sites_overlay');toggleProductInfoDiv('modalBack'); return false;\" class=\"btn_close\">Close </a> </p> </div></div>";
	
	$sites_info_2 = $sites_info;
 	$sites_info = array();
	$i = 0;
	for ($k=0; $k<count($sites_info_2);$k++)
	 if(in_array($sites_info_2[$k]["name"],$site_chk))
	 {
	 	$sites_info[$i]=$sites_info_2[$k];
		$i++;
	 }

	$pos_text='';
	
	 get_labeled_seq($labeled_seq, $label_text, $pos_text,$down_string,$up_string,$main_string,$addspace,$orientation, $left_pos, $right_pos,$join_text);
	
	 get_labeled_sites_seq($sites_info,$labeled_sites_seq,$pos_text,$label_text,$up_string, $down_string, $main_string, $addspace,$left_pos,$right_pos,$orientation,$join_pos,$sites_name_global,$sites_color_global);
//	 get_labeled_seq($labeled_seq, $label_text, $pos_text,$down_string,$up_string,$main_string,$addspace,$orientation, $left_pos, $right_pos,$join_text);

	 $reconstruction_info_text = $_REQUEST["join_html_text"]. seq_inframe_stop_text($eg_id);
	$site_seq_form .= "<pre  style=\"font-size:14px\">{$reconstruction_info_text}<TABLE border=\"0\"><tr valign=\"bottom\">";
	$site_seq_form .="<td><PRE style=\"font-size:14px ; background-color:white;\">".$pos_text."</PRE></td>";
	$site_seq_form .="<td><PRE style=\"font-size:14px ; background-color:white;\">".$labeled_sites_seq."</PRE></td>";
	$site_seq_form .="<td><PRE style=\"font-size:14px ; background-color:white;\">".$label_text."</PRE></td>";
	$site_seq_form .="</TR></TABLE></pre></form>";
	return $site_seq_form;
	
}
//function get_sites_info($sites_info, $sites_name, $left_pos,$searchegs, $up_string, $down_string, $main_string,$right_pos,$orientation)
function get_sites_info(&$sites_info, &$sites_name, $left_pos,$searchegs, $up_string, $down_string, $main_string,$right_pos,$orientation)
{
	
	global $GENOME_LENGTH;
	$sequence = $up_string.$main_string.$down_string;
	if($orientation=="Clockwise")
	{
		$start_end = ($left_pos - strlen($up_string)+$GENOME_LENGTH)%$GENOME_LENGTH;
	}
	else {
		
		$start_end = ($right_pos + strlen($up_string)+$GENOME_LENGTH)%$GENOME_LENGTH;	
	}	
	
//	db_set_active("mapsearch_db");
	$query = db_query("Select name, sequence, seq,  is_symmetrical FROM mapsearch_db.t_enzyme WHERE  name in (".$searchegs.") ORDER by name ASC");

	 $i=0;
	 $t = 0;
	 
 While ($row = $query->fetchAssoc()) {
 	
 	$enzyme_name = $row['name'];
	$pattern_disply = $row['sequence'];
	$is_symmetrical = $row['is_symmetrical'];
	$pattern = trim($row['seq']);
	$sites_name[$i] =  $enzyme_name;
	
	$detected = search_site($sequence, $pattern, $is_symmetrical);
	

 	for($k=0; $k<count($detected); $k++)
 	{
 		if($orientation=="Clockwise")
		{
			$sites_info[$t]["left_end"]=($detected[$k]['pos']+$start_end+$GENOME_LENGTH)%$GENOME_LENGTH;
			$sites_info[$t]["right_end"]=($detected[$k]['pos']+$start_end+strlen($pattern)-1+$GENOME_LENGTH)%$GENOME_LENGTH;					
			
			
			if($detected[$k]['ori']=="Clockwise")
			{
				$sites_info[$t]["ori"]='Plus';
				$sites_info[$t]["relative"]=($sites_info[$t]["left_end"]-$left_pos+$GENOME_LENGTH)%$GENOME_LENGTH;
			}
			else if($detected[$k]['ori']=="Counterclockwise") {
				$sites_info[$t]["ori"]='Minus';
				$sites_info[$t]["relative"]=($sites_info[$t]["left_end"]-$left_pos+$GENOME_LENGTH)%$GENOME_LENGTH;
//				$sites_info[$t]["relative"]=($sites_info[$t]["right_end"]-$left_pos+$GENOME_LENGTH)%$GENOME_LENGTH;
			
			}
			else 
			{
				$sites_info[$t]["ori"]='Both';
				$sites_info[$t]["relative"]=($sites_info[$t]["left_end"]-$left_pos+$GENOME_LENGTH)%$GENOME_LENGTH;
			}
		}
		else {
			$sites_info[$t]["right_end"]=(-$detected[$k]['pos']+$start_end+$GENOME_LENGTH)%$GENOME_LENGTH;
			$sites_info[$t]["left_end"]=(-$detected[$k]['pos']+$start_end-strlen($pattern)+1+$GENOME_LENGTH)%$GENOME_LENGTH;
			
			if($detected[$k]['ori']=="Clockwise")
			{
				$sites_info[$t]["ori"]='Minus';
//				$sites_info[$t]["relative"]=($sites_info[$t]["right_end"]-$right_pos+$GENOME_LENGTH)%$GENOME_LENGTH;
				$sites_info[$t]["relative"]=($sites_info[$t]["left_end"]-$right_pos+$GENOME_LENGTH)%$GENOME_LENGTH;
			}
			else if($detected[$k]['ori']=="Counterclockwise") {
				$sites_info[$t]["ori"]='Plus';
				$sites_info[$t]["relative"]=($sites_info[$t]["left_end"]-$right_pos+$GENOME_LENGTH)%$GENOME_LENGTH;
			
			}
			else 
			{
				$sites_info[$t]["ori"]='Both';
				$sites_info[$t]["relative"]=($sites_info[$t]["left_end"]-$right_pos+$GENOME_LENGTH)%$GENOME_LENGTH;
			}
			
		}
		if ($sites_info[$t]["relative"]>$GENOME_LENGTH/2)
		{
			$sites_info[$t]["relative"] = $sites_info[$t]["relative"]-$GENOME_LENGTH;
		}
		
		$sites_info[$t]["name"]=$enzyme_name;
		$sites_info[$t]["seq"]=$detected[$k]['seq'];
 		$t++;
 		
 	}
	$i++;
 }
db_set_active("ecogene");
	
}
function print_labeled_tfbs_seq($eg_id)
{
	$string = '?'.drupal_http_build_query(drupal_get_query_parameters($_REQUEST, array_merge(array('eg_id','','tfbs_submit','tfbs_chk'), array_keys($_COOKIE))));
		
	$down_string="";
	$up_string="";
	$main_string="";
	$join_text="";
	$join_pos="";
	
	$ds= isset($_REQUEST['ds']) ? $_REQUEST['ds']:0;
	$us= isset($_REQUEST['us']) ? $_REQUEST['us']:0;
	$join_text= isset($_REQUEST['join_text']) ? $_REQUEST['join_text']:"";
	
	get_string($down_string,$up_string,$main_string,$description,$addspace,$orientation, $left_pos, $right_pos,$es,$bs,$eg_id,$us,$ds,$join_text);
//	get_string(&$down_string,&$up_string,&$main_string,&$description,&$addspace,&$orientation, &$left_pos, &$right_pos,&$es,&$bs,$eg_id,$us,$ds,$join_text);
//	if(~isset($tfbs))
	{
		$tfbs_info=array();
		get_tfbs_info($tfbs_info, $left_pos,$right_pos,$es,$bs,$us,$ds,$orientation);
		$tfbs_color_global = array();
		$tfbs_name_global = array();
		$tfbs_chk = array();

		//Get the all the TFBS names in the range Uniquely
		$i=0;
		
		for ($k=0; $k<count($tfbs_info);$k++)	{
			if(!(in_array($tfbs_info[$k]["name"],$tfbs_name_global)) )
			{
				$tfbs_color_global[$i] = get_random_color();
				$tfbs_name_global[$i] = $tfbs_info[$k]["name"];				
				$i++;
			}
			$tfbs_chk[$k]=$tfbs_info[$k]["name"];
		}
	}
	
	if(isset($_REQUEST['tfbs_submit']))
	{	$title=$_REQUEST['tfbs_submit'];
		$tfbs_chk = isset($_REQUEST['tfbs_chk'])? $_REQUEST['tfbs_chk']:array();

	}
	
	$tfbs_seq_form = "<form name=tfbsForm action=$string method=\"post\"><input type=hidden name=tfbs_submit value=1>";

	$title =  "";
	$title .= "<P style=\"font-size:14px \" id=\"tfbs_name\">	";
	for ($j=0; $j<count($tfbs_name_global);$j++)
	{
		if (in_array($tfbs_name_global[$j],$tfbs_chk))
		$title .= "<input type='checkbox' name='tfbs_chk[]'  value='$tfbs_name_global[$j]' onclick=javascript:document.tfbsForm.submit() checked><font style=\"background-color:".$tfbs_color_global[$j]."; font-family:'Courier New'; font-size:14px\" color=\"white\">".$tfbs_name_global[$j]." </font>";
		else {
			$title .= "<input type='checkbox' name='tfbs_chk[]' value='$tfbs_name_global[$j]' onclick=javascript:document.tfbsForm.submit() unchecked><font style=\"background-color:".$tfbs_color_global[$j]."; font-family:'Courier New'; font-size:14px\" color=\"white\">".$tfbs_name_global[$j]." </font>";
		}

	}
	
	
	$title .= "&nbsp;&nbsp;<font style=\"background-color:red; font-family:'Courier New'; font-size:14px\" color=\"white\">Overlap</font>";
	$title .=" <a href=\"javascript:;\" onclick=\"javascript:toggleProductInfoDiv('tfbs_overlay');toggleProductInfoDiv('modalBack'); return false;\" style='CURSOR: hand'>Positions</a></P>";
	
	$detail="
	<div id=\"tfbs_overlay\" class=\"info_overlay\" style=\"display:none;\">   
    <div>
	 <a href=\"javascript:;\" onclick=\"javascript:toggleProductInfoDiv('tfbs_overlay');toggleProductInfoDiv('modalBack'); return false;\" class=\"btn_close\">Close </a>
 	<h3>TFBS Positions</h3>
    <p align=\"left\">     
	
		<table class='detail_description' width=800 border=0 align='left'>";
	for ($j=0; $j<count($tfbs_info);$j++)
	{	
		if (in_array($tfbs_info[$j]["name"],$tfbs_chk))
		$detail.= "<tr><td>".$tfbs_info[$j]["name"]."</td><td>".$tfbs_info[$j]["left_end"]."</td><td>".$tfbs_info[$j]["right_end"]."</td></tr>";		
	}
	$detail.="</table><p align=\"left\"> ";
//	$detail.=" <a href=\"javascript:;\" onclick=\"javascript:toggleProductInfoDiv('tfbs_overlay');toggleProductInfoDiv('modalBack'); return false;\" class=\"btn_close\">Close </a> </p></div></div>";
   $detail.=" <a href=\"javascript:;\" onclick=\"javascript:toggleProductInfoDiv('tfbs_overlay');toggleProductInfoDiv('modalBack'); return false;\" class=\"btn_close\">Close </a> </p></div></div>";
    
	$tfbs_seq_form.=$title.$detail;
	
	$tfbs_info_2 = $tfbs_info;
	$tfbs_info = array();
	$i = 0;
	for ($k=0; $k<count($tfbs_info_2);$k++)
	 if(in_array($tfbs_info_2[$k]["name"],$tfbs_chk))
	 {
	 	$tfbs_info[$i]=$tfbs_info_2[$k];
		$i++;
	 }
//	 get_labeled_seq(&$labeled_seq, &$label_text, &$pos_text,$down_string,$up_string,$main_string,$addspace,$orientation, $left_pos, $right_pos,$join_text);
//	 get_labeled_tfbs_seq($tfbs_info, &$labeled_tfbs_seq,$pos_text,$label_text,$up_string, $down_string, $main_string, $addspace,$left_pos,$right_pos,$orientation,$join_pos,$tfbs_name_global,$tfbs_color_global);
//	 
 	get_labeled_seq($labeled_seq, $label_text, $pos_text,$down_string,$up_string,$main_string,$addspace,$orientation, $left_pos, $right_pos,$join_text);
	 get_labeled_tfbs_seq($tfbs_info, $labeled_tfbs_seq,$pos_text,$label_text,$up_string, $down_string, $main_string, $addspace,$left_pos,$right_pos,$orientation,$join_pos,$tfbs_name_global,$tfbs_color_global);
	 
	$reconstruction_info_text = $_REQUEST["join_html_text"]. seq_inframe_stop_text($eg_id);
	$tfbs_seq_form .= "<pre  style=\"font-size:14px\">{$reconstruction_info_text}<TABLE border=\"0\"><tr valign=\"bottom\"><TD><PRE style=\"font-size:14px;background-color:white; \">" .$pos_text."</PRE></TD>";
	$tfbs_seq_form .="<td><PRE style=\"font-size:14px ; background-color:white;\">".$labeled_tfbs_seq."</PRE></td>";
	$tfbs_seq_form .="<td><PRE style=\"font-size:14px ; background-color:white;\">".$label_text."</PRE></td>";
	$tfbs_seq_form .="</TR></TABLE></pre>";
	return $tfbs_seq_form;
	
}
function seq_inframe_stop_text($eg_id)
{
	$gene_joint = gene_join_info($eg_id);
	if ( empty($_REQUEST['join_text']) && !empty($gene_joint))
	{
		return;
	}
	$inframe_stp_info_text = '';
	$query = db_query("SELECT name from t_gene where eg_id=:eg_id",array('eg_id'=>$eg_id));
	if($row = $query->fetchAssoc())
		$name = $row['name'];
		
		
	$join_text= isset($_REQUEST['join_text']) ? $_REQUEST['join_text']:"";
	
	$query = db_query("	SELECT pseudo.*, ta.*  FROM t_pseudogene_detail pseudo
						LEFT JOIN t_address ta ON pseudo.address_id = ta.address_id
						WHERE pseudo.type = 'STP' AND eg_id=:eg_id",
						array('eg_id'=>$eg_id));
						
	$stp_codon_str = array();
	$stp_codon_pos = array();
	while ($row = $query->fetchAssoc()) {
		$left = $row['left_end'];
		$codon_n = $row['codon'];
		$orientation = $row['orientation'];
		$codon_str='';
		
		$query_codon_str = db_query(" SELECT 
			CASE '".$orientation."'
				WHEN 'Clockwise' THEN replace(SUBSTR(s.sequence, $left, 3), 'T', 'U')
				WHEN 'Counterclockwise' THEN replace(replace(replace(replace(replace(replace(reverse( SUBSTR(s.sequence, $left, 3) ), 'A','Z'),'T','A'),'Z','U'),'G','Z'),'C','G'),'Z','C')
  			END AS s_codon
		FROM t_sequence s
					");
		if( $r_codon = $query_codon_str->fetchAssoc() )
		{			
			$stp_codon_str[] = $r_codon['s_codon'];
			$stp_codon_pos[] = $codon_n;
			//var_dump($r_codon);
		} 	
			
	}
	if ( count($stp_codon_pos) == 1 )
	{
		$inframe_stp_info_text = "A ".$stp_codon_str[0]." in-frame stop at codon ".$stp_codon_pos[0]." of the ".$name."' pseudogene is translated as X in the reconstructed ".$name."' protein sequence, which is extended to the authentic stop codon.";
	}
	elseif ( count($stp_codon_pos) > 1 )
	{
		$inframe_stp_info_text = "A {".implode($stp_codon_str, ", ")."} in-frame stop at codon {".implode($stp_codon_pos, ", ")."} of the ".$name."' pseudogene is translated as X in the reconstructed ".$name."' protein sequence.";
	}
	return $inframe_stp_info_text.'<br />';
}

function print_seq($eg_id)
{
	$down_string="";
	$up_string="";
	$main_string="";
	$join_text="";
	
	$ds= isset($_REQUEST['ds']) ? $_REQUEST['ds']:0;
	$us= isset($_REQUEST['us']) ? $_REQUEST['us']:0;
	$join_text= isset($_REQUEST['join_text']) ? $_REQUEST['join_text']:"";
	
	get_string($down_string,$up_string,$main_string,$description,$addspace,$orientation, $left_pos, $right_pos,$es,$bs,$eg_id,$us,$ds,$join_text);
//	get_string(&$down_string,&$up_string,&$main_string,&$description,&$addspace,&$orientation, &$left_pos, &$right_pos,&$es,&$bs,$eg_id,$us,$ds,$join_text);
		
		$FASTA_seq = "";
		$line_len = 57;
		$line_len_2 = 75;
		if($addspace==true)
		{
			for ($i=0; $i<strlen($up_string);$i++) {

				if (($i % $line_len_2)==0) { 

				$FASTA_seq = $FASTA_seq."\n";

				}
				$FASTA_seq = $FASTA_seq. $up_string[$i];
			}
			$FASTA_seq = $FASTA_seq. "\n";
			
			$FASTA_seq_tmp = ''; 
			for ($i=0; $i<strlen($main_string);$i++) {

				if (($i % $line_len)==0) { 

				$FASTA_seq_tmp = $FASTA_seq_tmp. "\n";

				}
				else
				{
					if (($addspace == true) && (($i % 3)==0)) {

						$FASTA_seq_tmp = $FASTA_seq_tmp. " ";

					}
				}
				$FASTA_seq_tmp = $FASTA_seq_tmp. $main_string[$i];
			}
			$FASTA_seq_updated = FASTA_pseudogene_label_stp_($FASTA_seq_tmp, $eg_id);
			if (!empty($FASTA_seq_updated))
			{
				$FASTA_seq .= $FASTA_seq_updated;
			}else 
			{
				$FASTA_seq .= $FASTA_seq_tmp;
			}
			
			$FASTA_seq = $FASTA_seq. "\n";
			for ($i=0; $i<strlen($down_string);$i++) {

				if (($i % $line_len_2)==0) { 

				$FASTA_seq = $FASTA_seq. "\n";

				}
				$FASTA_seq = $FASTA_seq. $down_string[$i];
			}	

		}
		else {
			$line_len = 75;
			for ($i=0; $i<strlen($main_string);$i++) {

				if (($i % $line_len)==0) { 

				$FASTA_seq = $FASTA_seq. "\n";

				}
				$FASTA_seq = $FASTA_seq. $main_string[$i];
			}

		}
		
	
	$query = db_query("SELECT name from t_gene where eg_id=:eg_id",array('eg_id'=>$eg_id));
	
	if($row = $query->fetchAssoc())
		$name = $row['name'];	

	$reconstruction_info_text = $_REQUEST["join_html_text"]. seq_inframe_stop_text($eg_id);
	$FASTA_seq = "<PRE style=\"font-size:14px\">{$reconstruction_info_text}"."<br />>".$eg_id." ".$name." ".$description.$FASTA_seq;
	$FASTA_seq .="</PRE>";
	return  $FASTA_seq;

}
function FASTA_pseudogene_label_stp_($FASTA_seq, $eg_id)
{
	
	$FASTA_seq_updated = '';
	$gene_joint = gene_join_info($eg_id);
		
	if ( empty($_REQUEST['join_text']) && (!empty($gene_joint)) )
	{
		return $FASTA_seq_updated;
	}
	
	$query = db_query("	SELECT pseudo.*, ta.*  FROM t_pseudogene_detail pseudo
						LEFT JOIN t_address ta ON pseudo.address_id = ta.address_id
						WHERE pseudo.type = 'STP' AND eg_id=:eg_id",
						array('eg_id'=>$eg_id));
						
	$stp_codon_pos = array();
	while ($row = $query->fetchAssoc()) {
		$stp_codon_pos[] = $row['codon'] * 3;	
	}	
	
		
	if (!empty($stp_codon_pos) )
	{
		
		$pos = 0; 
		
		for ( $i=0; $i<strlen($FASTA_seq); $i++) {
			
			$c = $FASTA_seq[$i];
			
			if( $c != "\n" && $c != " ")
			{
//								
				if ( in_array($pos+3, $stp_codon_pos, true) ) {
//					
					$FASTA_seq_updated = $FASTA_seq_updated."<font color='red'>".$c;
					
				}elseif( in_array($pos+1, $stp_codon_pos, true) )
				{
					$FASTA_seq_updated = $FASTA_seq_updated.$c."</font>";
					
				}else {
					
					$FASTA_seq_updated = $FASTA_seq_updated.$c;
				}
				
				$pos++;
			}
			else 
			{				
				$FASTA_seq_updated = $FASTA_seq_updated.$c;
			}
		}
	}
	
	return $FASTA_seq_updated;
}
function print_labeled_seq($eg_id)
{
	global $GENOME_LENGTH;
	$down_string="";
	$up_string="";
	$main_string="";
	$join_text="";
	
	$ds= isset($_REQUEST['ds']) ? $_REQUEST['ds']:0;
	$us= isset($_REQUEST['us']) ? $_REQUEST['us']:0;
	$join_text= isset($_REQUEST['join_text']) ? $_REQUEST['join_text']:"";
	
	
//	get_string(&$down_string,&$up_string,&$main_string,&$description,&$addspace,&$orientation, &$left_pos, &$right_pos,&$es,&$bs,$eg_id,$us,$ds,$join_text);
//	get_labeled_seq(&$labeled_seq, &$label_text, &$pos_text,$down_string,$up_string,$main_string,$addspace,$orientation, $left_pos, $right_pos,$join_text);

	get_string($down_string,$up_string,$main_string,$description,$addspace,$orientation, $left_pos, $right_pos,$es,$bs,$eg_id,$us,$ds,$join_text);
	get_labeled_seq($labeled_seq, $label_text, $pos_text,$down_string,$up_string,$main_string,$addspace,$orientation, $left_pos, $right_pos,$join_text);
	
	$reconstruction_info_text = $_REQUEST["join_html_text"]. seq_inframe_stop_text($eg_id);
		$Coordinates = "<pre  style=\"font-size:14px\">{$reconstruction_info_text}<TABLE border=\"0\"><tr valign=\"bottom\"><TD><PRE style=\"font-size:14px;background-color:white; \">" .$pos_text."</PRE></TD>";
		$Coordinates .="<td><PRE style=\"font-size:14px ; background-color:white;\">".$labeled_seq."</PRE></td>";
		$Coordinates .="<td><PRE style=\"font-size:14px ; background-color:white;\">".$label_text."</PRE></td>";
		$Coordinates .="</TR></TABLE></pre>";
		
		return $Coordinates;
}
 
//function get_string($down_string,$up_string,$main_string,$description,$addspace,$orientation, $left_pos, $right_pos,$es,$bs,$eg_id,$us=0,$ds=0,$join_text="")
function get_string(&$down_string,&$up_string,&$main_string,&$description,&$addspace,&$orientation, &$left_pos, &$right_pos,&$es,&$bs,$eg_id,$us=0,$ds=0,$join_text="")
{

	
	global $GENOME_LENGTH;
	// control the fasta format, if there is up or down stream
	// space will be added in the displayed sequence
	
	$addspace = true;
	// Added this line to have no triplet separation in fasta display when no us and ds are included
	if ($ds == 0 && $us == 0)
	{
		$addspace = false;
	}
	$query = db_query("SELECT a.left_end, a.right_end, a.orientation FROM t_gene g, t_address a WHERE g.address_id = a.address_id and g.eg_id = :eg_id",array(':eg_id'=>$eg_id));

	if($row = $query->fetchAssoc())
	{
		$left_pos  = $row["left_end"];
		$right_pos = $row["right_end"];
		$orientation =  $row["orientation"];
	}


	//get the position of the up and down streams in the genome sequence
	//according the up and down stream length $us and $ds and orientations
	//need to go circle if necessary (gennome is a circle)

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
		$query=db_query($query);
		if($row1 = $query->fetchAssoc())
		{
			$bstring = $row1['sequence'];
		}
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


		$query=db_query($query);
		if($row1 = $query->fetchAssoc())
		{
			$estring = $row1['sequence'];
		}
	}

	include("sites/all/include/seq_format.php");
	$query = seq_format($eg_id, $us, $ds);
	
	$sequence = "";
	$query=db_query($query);
	if($row = $query->fetchAssoc())
	{
		$sequence .= $row["sequence"];
		$description = format_description($row["description"]);
		
	}
	

//	 This portion of the code generates the extra upstream sequences and wraps them
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

function get_tfbs_info(&$tfbs_info, $left_pos,$right_pos,$es,$bs,$us,$ds,$orientation)
{
	global $GENOME_LENGTH;
	if($orientation=="Clockwise")
	{
	$query = "SELECT * FROM  t_tfbs ".
			"WHERE ".
			"(( right_end > ".$left_pos."-".$us ." AND left_end <". $right_pos."+".$ds." ) "; 
	}
	else {
		$query = "SELECT * FROM  t_tfbs  ".
			"WHERE ".
			"(( right_end > ".$left_pos."-".$ds ." AND left_end < ".$right_pos."+".$us." ) ";
			
	}
	
	if($bs!=0)
	{
		$query = $query."  OR left_end < $bs  "; 
	}
	if($es!=$GENOME_LENGTH)
	{
		$query = $query."  OR right_end > $es  "; 
	}
	$query = $query. " ) ORDER BY left_end ";
	
	$query = db_query($query);
	
	$i=0;
	while ($row = $query->fetchAssoc())
	{
		$tfbs_info[$i]["left_end"]=$row["left_end"];
		$tfbs_info[$i]["right_end"]=$row["right_end"];
		$tfbs_info[$i]["name"]=$row["name"];
		$tfbs_info[$i]["shown_left_end"]=$row["left_end"];
		$tfbs_info[$i]["shown_right_end"]=$row["right_end"];
		$i = $i+1;
	}
	
}
//function get_labeled_tfbs_seq($tfbs_info,$labeled_tfbs_seq,$pos_text,$label_text,$up_string, $down_string, $main_string, $addspace,$left_pos,$right_pos,$orientation,$join_pos,$tfbs_name_global,$tfbs_color_global)
function get_labeled_tfbs_seq($tfbs_info, &$labeled_tfbs_seq,$pos_text,$label_text,$up_string, $down_string, $main_string, $addspace,$left_pos,$right_pos,$orientation,$join_pos,$tfbs_name_global,$tfbs_color_global)
{
	global $GENOME_LENGTH;
//	global $tfbs_name_global;
//	global $tfbs_color_global;
	
	$labeled_tfbs_seq="<FONT style=\"background-color:white; font-family:'Courier New'; font-size:14px\" color=\"black\">";
	$pos_text="<FONT style=\"background-color:white; font-family:'Courier New'; font-size:14px\" color=\"black\">";
	$labeled_seq ="<FONT style=\"background-color:white; font-family:'Courier New'; font-size:14px\" color=\"black\">";
		$line_len =50;
		$sep_len = 10;
		
	$tfbs_info_num = count($tfbs_info);
	$i = 0;
	$tfbs_left_end=array();$tfbs_right_end=array();$tfbs_color=array();$tfbs_name=array();
	for ($k=0; $k<$tfbs_info_num;$k++)
		{
			$tfbs_left_end[$k]=$tfbs_info[$k]["left_end"];
			$tfbs_right_end[$k]=$tfbs_info[$k]["right_end"];
			
			if(in_array($tfbs_info[$k]["name"],$tfbs_name_global))
			{
				$key = array_search($tfbs_info[$k]["name"],$tfbs_name_global);				
				$tfbs_color[$k]=$tfbs_color_global[$key];	
				
			}
			else
			{
				$tfbs_color[$k]= get_random_color();
							
			}	
			
			$tfbs_name[$k]=$tfbs_info[$k]["name"];
			
		}

// echo "\$tfbs_name:<br>";		
// print_r($tfbs_name);
// echo "<br>\$tfbs_left_end:<br>";
// print_r($tfbs_left_end);
// echo "<br>\$tfbs_right_end:<br>";
// print_r($tfbs_right_end);
// echo "<br>\$tfbs_color:<br>";
// print_r($tfbs_color);
 
	
	$stack = array();
	$stack_font = array();
	
//	draw up string		
	$up_str_len = strlen($up_string);
	$pad_num = 0; 	
	
	
	if(strlen($up_string)%$line_len!=0)
	{
		$pad_num = $line_len-strlen($up_string)%$line_len;
		$up_string = str_pad($up_string,strlen($up_string)+$line_len-strlen($up_string)%$line_len, " ", STR_PAD_LEFT);
	}
	
	$str_len = strlen($up_string);
			
	for ($i=0; $i<strlen($up_string);$i++)
	{

		if($i==0){

			$labeled_tfbs_seq = $labeled_tfbs_seq;
		}


		if (!($i % $line_len)==0 && ($i % $sep_len)==0) {

			$labeled_tfbs_seq = $labeled_tfbs_seq. " ";
		}
		$abs_pos=0;
		$keys = array();

		if($i>$pad_num)
		{

			if($orientation=="Clockwise")
			{
				$abs_pos = intval(-$up_str_len)+intval(-$pad_num)+$left_pos+$i;
				if($abs_pos<1)
				{
					$abs_pos = $abs_pos + $GENOME_LENGTH;
				}
				$keys = array_keys($tfbs_left_end,$abs_pos);
				//					if($keys)
				//				echo "<br>in open\$abs_pos ".$abs_pos."";
			}
			else
			{
				$abs_pos = intval($up_str_len)+intval($pad_num) + $right_pos -$i;
				if($abs_pos > $GENOME_LENGTH)
				{
					$abs_pos = $abs_pos - $GENOME_LENGTH;
				}

				$keys = array_keys($tfbs_right_end,$abs_pos);
			}

			if(count($keys)>0){
				push_font($keys, $tfbs_color, $tfbs_name, $tfbs_left_end, $tfbs_right_end, $stack, $stack_font, $labeled_tfbs_seq);
//				push_font($keys, $tfbs_color, $tfbs_name, $tfbs_left_end, $tfbs_right_end, &$stack, &$stack_font, &$labeled_tfbs_seq);

			}
		}
		$labeled_tfbs_seq = $labeled_tfbs_seq. $up_string[$i];
		if($i>$pad_num)
		{
			$keys = array();

			if($orientation=="Clockwise")
			{
				$keys = array_keys($tfbs_right_end, $abs_pos);
				
			}
			else
			{

				$keys = array_keys($tfbs_left_end,$abs_pos);
			}

			if(count($keys)>0)
			{
//				pop_font($keys, &$stack, &$stack_font, &$labeled_tfbs_seq,$tfbs_name, $tfbs_left_end, $tfbs_right_end);
				pop_font($keys, $stack, $stack_font, $labeled_tfbs_seq,$tfbs_name, $tfbs_left_end, $tfbs_right_end);

			}
		}

		if (((($i+1) % ($line_len))==0) && ($i>0)) {

			$labeled_tfbs_seq = $labeled_tfbs_seq. "\n";
		}

	}

	if($i>0)
	{
		$labeled_tfbs_seq = $labeled_tfbs_seq. "\n";
	}		
			
			
			
			
			
//main string here
			$main_str_len = strlen($main_string);
						
			for ($i=0; $i<$main_str_len;$i++) {

				if (($i % $line_len)==0) { 
					
						
					if($i>0){
						$labeled_tfbs_seq = $labeled_tfbs_seq. "\n";
					}else{
						$labeled_tfbs_seq = $labeled_tfbs_seq;
					}
				}
				elseif (($i % $sep_len)==0) {

						$labeled_tfbs_seq = $labeled_tfbs_seq. " ";
				}		
				$abs_pos=0;
				$keys = array();				
				if($orientation=="Clockwise")
				{
					$abs_pos =$left_pos+$i;
					if($abs_pos > $GENOME_LENGTH)
					{
						$abs_pos = $abs_pos - $GENOME_LENGTH;
					}
					
					$keys = array_keys($tfbs_left_end, $abs_pos);
				}
				else
				{
					$abs_pos = $right_pos -$i;
					
					if($abs_pos<1)
					{
						$abs_pos = $abs_pos + $GENOME_LENGTH;
					}
					
					$keys = array_keys($tfbs_right_end,$abs_pos);
				}
				
				if(count($keys)>0)
				{
					push_font($keys, $tfbs_color, $tfbs_name, $tfbs_left_end, $tfbs_right_end, $stack, $stack_font, $labeled_tfbs_seq);		
//					push_font($keys, $tfbs_color, $tfbs_name, $tfbs_left_end, $tfbs_right_end, &$stack, &$stack_font, &$labeled_tfbs_seq);	
				}
					
					
					$labeled_tfbs_seq = $labeled_tfbs_seq. $main_string[$i];
					
					
				$keys = array();	
					if($orientation=="Clockwise")
					{
						
						$keys = array_keys($tfbs_right_end,$abs_pos);
					}
					else
					{
						
						$keys = array_keys($tfbs_left_end,$abs_pos);
					}

				if(count($keys)>0)
				{
//					 pop_font($keys, &$stack, &$stack_font, &$labeled_tfbs_seq,$tfbs_name, $tfbs_left_end, $tfbs_right_end);
					 pop_font($keys, $stack, $stack_font, $labeled_tfbs_seq,$tfbs_name, $tfbs_left_end, $tfbs_right_end);
			
				}
			}
			
			
			
//			draw the down string part for clock wise gene
//			       or actually up string part counterclockwise gene 
//			
			$down_str_len = strlen($down_string);
			if(strlen($down_string)%$line_len!=0)
			{
				$down_string = str_pad($down_string,strlen($down_string)+$line_len-strlen($down_string)%$line_len, " ", STR_PAD_RIGHT);
			}
			if($down_str_len>0)
			{
				$labeled_tfbs_seq = $labeled_tfbs_seq. "\n";
			}
			
			for ($i=0; $i<strlen($down_string);$i++) {

				if (($i % $line_len)==0) {

					$labeled_tfbs_seq = $labeled_tfbs_seq. "\n";

				}
				elseif (($i % $sep_len)==0 && $i<$down_str_len) {

					$labeled_tfbs_seq = $labeled_tfbs_seq. " ";
				}
				if($i<$down_str_len)
				{
					$abs_pos=0;
					$keys = array();
					if($orientation=="Clockwise")
					{
						$abs_pos =$right_pos+$i+1;
						if($abs_pos > $GENOME_LENGTH)
						{
							$abs_pos = $abs_pos - $GENOME_LENGTH;
						}

						$keys = array_keys($tfbs_left_end, $abs_pos);
					}
					else
					{
						$abs_pos = $left_pos -$i-1;

						if($abs_pos<1)
						{
							$abs_pos = $abs_pos + $GENOME_LENGTH;
						}


						$keys = array_keys($tfbs_right_end, $abs_pos);
					}

					if(count($keys)>0)
					{
						push_font($keys, $tfbs_color, $tfbs_name, $tfbs_left_end, $tfbs_right_end, $stack, $stack_font, $labeled_tfbs_seq);
					}
//				}

				$labeled_tfbs_seq = $labeled_tfbs_seq. $down_string[$i];
				
//				if($i<$down_str_len)
//				{
					$keys = array();
					if($orientation=="Clockwise")
					{

						$keys = array_keys($tfbs_right_end,$abs_pos);
					}
					else
					{

						$keys = array_keys($tfbs_left_end,$abs_pos);
					}
					if(count($keys)>0)
					{
//						pop_font($keys, &$stack, &$stack_font, &$labeled_tfbs_seq,$tfbs_name, $tfbs_left_end, $tfbs_right_end);
						pop_font($keys, $stack, $stack_font, $labeled_tfbs_seq,$tfbs_name, $tfbs_left_end, $tfbs_right_end);

					}
				}

			}
			
}
function push_font($keys, $tfbs_color, $tfbs_name, $tfbs_left_end, $tfbs_right_end, &$stack, &$stack_font, &$labeled_tfbs_seq)
{
	for($k1=0;$k1<count($keys);$k1++)
			{
				$key = $keys[$k1];
												
					array_push($stack, $key);
//					$colors = get_random_color();
					$font_temp = "<FONT title=\"Name: ".$tfbs_name[$key]." Left End: ".$tfbs_left_end[$key]." Right End:".$tfbs_right_end[$key]."\" style=\"background-color:".$tfbs_color[$key]."; font-family:'Courier New'; font-size:14px\" color=\"white\">";
					array_push($stack_font, $font_temp);
					
					
					if( sizeof($stack)==1)
					{
						
						$labeled_tfbs_seq = $labeled_tfbs_seq.$font_temp;
					}
					else {
//						
						$labeled_tfbs_seq = $labeled_tfbs_seq. "<FONT title=\"Name: ".$tfbs_name[$key]." Left End: ".$tfbs_left_end[$key]." Right End:".$tfbs_right_end[$key]."\" style=\"background-color:red; font-family:'Courier New'; font-size:14px\" color=\"white\">";
					}
					
				
			}
}
//function pop_font($keys, $stack, $stack_font, $labeled_tfbs_seq, $tfbs_name, $tfbs_left_end, $tfbs_right_end)
function pop_font($keys, &$stack, &$stack_font, &$labeled_tfbs_seq,$tfbs_name, $tfbs_left_end, $tfbs_right_end)
{
	if(sizeof($stack)>1)
			{
				$labeled_tfbs_seq = $labeled_tfbs_seq."</font>";
			}
	 for($k1=0;$k1<count($keys);$k1++)
			{
				$key = $keys[$k1];
				$temp_stack = array();
				$temp_stack_font = array();
				
//				echo "<br>in close key:\t".$key."<br>";
//				echo "  <br><0> \$stack: = ";
//				print_r($stack)."<br>";
//				echo "  <br><0> \$stack_font: = ";
//				print_r(str_replace("<FONT","(font",$stack_font))."<br>";
				while (count($stack)>0)
				{
					$temp_key=array_pop($stack);
					$temp_font=array_pop($stack_font);

					if($temp_key==$key)
					{
						$labeled_tfbs_seq = $labeled_tfbs_seq."</font>";
						break;
					}
					else
					{	
						array_push($temp_stack, $temp_key);
						array_push($temp_stack_font, $temp_font);
						$labeled_tfbs_seq = $labeled_tfbs_seq."</font>";
					}				
					
				}
				while (count($temp_stack)>0)
				{
					$temp_key=array_pop($temp_stack);
					$temp_font=array_pop($temp_stack_font);
					array_push($stack, $temp_key);
					array_push($stack_font, $temp_font);				
				
				}
			}
//			echo "  <br><2> \$stack: = ";
//			print_r($stack)."<br>";
//			
//			echo "  <br><3> \$stack: = ";
//			print_r($stack)."<br>";
			if(sizeof($stack)>1)
			{
				
				$key = $stack[sizeof($stack)-1];
				
				$labeled_tfbs_seq = $labeled_tfbs_seq. "<FONT title=\"Name: ".$tfbs_name[$key]." Left End: ".$tfbs_left_end[$key]." Right End:".$tfbs_right_end[$key]."\" style=\"background-color:red; font-family:'Courier New'; font-size:14px\" color=\"white\">";
			}
			elseif(sizeof($stack)>0)
			{
				$labeled_tfbs_seq = $labeled_tfbs_seq.$stack_font[0];
			}
}
//function get_labeled_seq($labeled_seq, $label_text, $pos_text,$down_string,$up_string,$main_string,$addspace,$orientation, $left_pos, $right_pos,$join_text)
function get_labeled_seq(&$labeled_seq, &$label_text, &$pos_text,$down_string,$up_string,$main_string,$addspace,$orientation, $left_pos, $right_pos,$join_text)
{
	global $GENOME_LENGTH;
	
	$join_pos = $join_text;
	
	$label_text="<FONT style=\"background-color:white; font-family:'Courier New'; font-size:14px\" color=\"black\">";
	$pos_text="<FONT style=\"background-color:white; font-family:'Courier New'; font-size:14px\" color=\"black\">";
	$labeled_seq ="<FONT style=\"background-color:white; font-family:'Courier New'; font-size:14px\" color=\"black\">";

		$line_len =50;
		$sep_len = 10;
		
	
	//draw up string

			$up_str_len = strlen($up_string);
		
			if(strlen($up_string)%$line_len!=0)
			{
				$up_string = str_pad($up_string,strlen($up_string)+$line_len-strlen($up_string)%$line_len, " ", STR_PAD_LEFT);
			}
			$str_len = strlen($up_string);
			
			for ($i=0; $i<strlen($up_string);$i++) {
				$text="";
				if (($i % $line_len)==0) { 
					if($orientation=="Clockwise")
					{
						if($i<$line_len) {
							$text = intval(-$up_str_len)+$left_pos;
							if($text<1)
							{
								$text = $text + $GENOME_LENGTH;
							}
						}else{
							$text = intval(-($str_len-$i))+$left_pos;
							if($text<1)
							{
								$text = $text + $GENOME_LENGTH;
							}
						}
					}
					elseif($orientation=="Counterclockwise")
					{
						if($i<$line_len) {
							$text = intval($up_str_len)+$right_pos;
							if($text > $GENOME_LENGTH)
							{
								$text = $text - $GENOME_LENGTH;
							}
						}else{
							$text = intval(($str_len-$i)) + $right_pos;
							if($text > $GENOME_LENGTH)
							{
								$text = $text - $GENOME_LENGTH;
							}
						}					
					}
					$text = str_pad($text,7," ",STR_PAD_LEFT);
					$pos_text = $pos_text.$text." \n";
					if($i==0)
					{
//						$labeled_seq = $labeled_seq;
					}
							
				}
				elseif (($i % $sep_len)==0) {
					
						$labeled_seq = $labeled_seq. " ";
				}
				
				$labeled_seq = $labeled_seq. $up_string[$i];
				if (((($i+1) % ($line_len))==0) && ($i>0)) { 
					
					if($i<$line_len) {
						$text = intval(-($str_len-$i));
						$text2 = -$up_str_len;
					}
					else{
						$text = intval(-($str_len-$i));
						$text2 = $text - $line_len + 1;
					}
					$text = str_pad(" [".$text2." ".$text."]",12," ",STR_PAD_RIGHT);

					$label_text = $label_text.$text."\n";
					
					if($i!=strlen(($up_string)-1))
						$labeled_seq = $labeled_seq. "\n";
				}
			}
// draw the main string
			if($i>0)
			{
				$labeled_seq = $labeled_seq. "\n";
				$pos_text = $pos_text. " \n";
				$label_text = $label_text."\n";
			}
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
		$join_num = $i;
		for ($i=0; $i<$join_num;$i++)
		{
			$join_array[$i]['left_end']	= $join_array[$i]['left_end'] ;
			$join_array[$i]['right_end'] =$join_array[$i]['right_end'] ;
			if($orientation=="Clockwise")
			{
				$seq_len[$i]=$join_array[$i]['right_end']-$join_array[$i]['left_end']+1;
			}elseif($orientation=="Counterclockwise")
			{
				$seq_len[$join_num-$i-1]=$join_array[$i]['right_end']-$join_array[$i]['left_end']+1;
			}

		}
		for ($i=1; $i<$join_num;$i++)
		{
			$seq_len[$i]=$seq_len[$i]+$seq_len[$i-1];
		}
		$font_color=array("Blue","Red","Teal","purple","Green");
		$seq_num=0;
		$color = $font_color[$seq_num];
		$labeled_seq = $labeled_seq. "<FONT style=\"background-color:white; font-family:'Courier New'; font-size:14px\" color=\"".$color."\">";
		$pos_text = $pos_text."</Font><FONT style=\"background-color:white; font-family:'Courier New'; font-size:14px\" color=\"".$color."\">";
			
	}
			
			$main_str_len = strlen($main_string);
			if(strlen($main_string)%$line_len!=0)
			{
				$main_string = str_pad($main_string,strlen($main_string)+$line_len-strlen($main_string)%$line_len, " ", STR_PAD_RIGHT);
			}
			$ii = 0;
			for ($i=0; $i<strlen($main_string);$i++) {

				if (($i % $line_len)==0) { 
					if((count($row_join)>1))
					{
						
						if($orientation=="Clockwise")
						{
							$text = intval($ii)+$join_array[$seq_num]["left_end"];
						}
						elseif($orientation=="Counterclockwise")
						{
							$text = intval(-$ii)+$join_array[count($seq_len)-$seq_num-1]["right_end"];
						}

						$text = str_pad($text,7," ",STR_PAD_LEFT);
						$pos_text = $pos_text.$text." \n";
						
					}
					else{
						if($orientation=="Clockwise")
						{
							$text = intval($i)+$left_pos;
						}
						elseif($orientation=="Counterclockwise")
						{
							$text = intval(-$i)+$right_pos;
						}

						$text = str_pad($text,7," ",STR_PAD_LEFT);
						$pos_text = $pos_text.$text." \n";
						
					}
					
					if($i>0){
						$labeled_seq = $labeled_seq. "\n";
					}
				}
				elseif (($i % $sep_len)==0) {

						$labeled_seq = $labeled_seq. " ";
				}
				if((count($row_join)>1) && ($i>=$seq_len[$seq_num]) && ($i<$main_str_len))
				{
					$ii=0;
					$seq_num = $seq_num+1;
					$color = $font_color[$seq_num];
					 $labeled_seq = $labeled_seq."</Font><FONT style=\"background-color:white; font-family:'Courier New'; font-size:14px\" color=\"".$color."\">";
					$pos_text = $pos_text."</Font><FONT style=\"background-color:white; font-family:'Courier New'; font-size:14px\" color=\"".$color."\">";
				}
				$labeled_seq = $labeled_seq. $main_string[$i];
				$ii = $ii+1;
				if (((($i+1) % ($line_len))==0) && ($i>0)) {
					if(($i+1)>$main_str_len) {
						$text = $main_str_len;
						$text2 = intval($i+1) - $line_len + 1;
					}
					else{
						$text = intval($i+1);
						$text2 = intval($i+1) - $line_len + 1;
					}
					$text = str_pad(" [".$text2." ".$text."]",12," ",STR_PAD_RIGHT);
					$labeled_seq = $labeled_seq;
//					echo "  ".$text;
					$label_text = $label_text.$text."\n";

				}
			}
			
			//draw the down string part for clock wise gene
			//       or actually up string part counterclockwise gene 
			$labeled_seq = $labeled_seq."</Font>"; 
			$pos_text = $pos_text."</Font>";

			$down_str_len = strlen($down_string);
			if(strlen($down_string)%$line_len!=0)
			{
				$down_string = str_pad($down_string,strlen($down_string)+$line_len-strlen($down_string)%$line_len, " ", STR_PAD_RIGHT);
			}
			if($down_str_len>0)
			{
			$labeled_seq = $labeled_seq. "\n";
			$labeled_seq = $labeled_seq."<FONT style=\"background-color:white; font-family:'Courier New'; font-size:14px\" color=\"black\">";
			$pos_text = $pos_text. " \n";
			$label_text = $label_text."\n";
			}
			for ($i=0; $i<strlen($down_string);$i++) {
				if (($i % $line_len)==0) { 
					if($orientation=="Clockwise")
					{
						$text = intval($i+1) + $right_pos;
						if($text > $GENOME_LENGTH)
							{
								$text = $text - $GENOME_LENGTH;
							}
					}
					elseif($orientation=="Counterclockwise")
					{
						$text = intval(-$i-1) + $left_pos;
						if($text < 1)
							{
								$text = $text + $GENOME_LENGTH;
							}
					}
					
					
					$text = str_pad($text,7," ",STR_PAD_LEFT);
					$pos_text = $pos_text.$text." \n";
					$labeled_seq = $labeled_seq. "\n";
					
				}
				elseif (($i % $sep_len)==0 && $i<$down_str_len) {

						$labeled_seq = $labeled_seq. " ";
				}
				if($i<$down_str_len)
					$labeled_seq = $labeled_seq. $down_string[$i];
				
					if (((($i+1) % ($line_len))==0) && ($i>0)) { 
					
					if(($i+1)>$down_str_len) {
						$text = $down_str_len+ $main_str_len;
						$text2 = intval($i+1) + $main_str_len- $line_len + 1;
					}
					else{
						$text = intval($i+1)+ $main_str_len;
						$text2 = intval($i+1) + $main_str_len - $line_len + 1;
					}
					
					
					$text = str_pad(" [".$text2." ".$text."]",12," ",STR_PAD_RIGHT);
					$label_text = $label_text.$text."\n";

				}
			}
}

//function get_labeled_sites_seq($sites_info,$labeled_sites_seq,$pos_text,$label_text,$up_string, $down_string, $main_string, $addspace,$left_pos,$right_pos,$orientation,$join_pos,$sites_name_global,$sites_color_global)
function get_labeled_sites_seq($sites_info,&$labeled_sites_seq,$pos_text,$label_text,$up_string, $down_string, $main_string, $addspace,$left_pos,$right_pos,$orientation,$join_pos,$sites_name_global,$sites_color_global)
{
	global $GENOME_LENGTH;
//	global $sites_name_global;
//	global $sites_color_global;
	$labeled_sites_seq="<FONT style=\"background-color:white; font-family:'Courier New'; font-size:14px\" color=\"black\">";
//	$pos_text="<FONT style=\"background-color:white; font-family:'Courier New'; font-size:14px\" color=\"black\">";
//	$labeled_seq ="<FONT style=\"background-color:white; font-family:'Courier New'; font-size:14px\" color=\"black\">";
		$line_len =50;
		$sep_len = 10;
	$sites_left_end=array();$sites_right_end=array();$sites_color=array();$sites_name=array();
		
	$sites_info_num = count($sites_info);
	$i = 0;
	for ($k=0; $k<$sites_info_num;$k++)
		{
			$sites_left_end[$k]=$sites_info[$k]["left_end"];
			$sites_right_end[$k]=$sites_info[$k]["right_end"];
			
			if(in_array($sites_info[$k]["name"],$sites_name_global))
			{
				$key = array_search($sites_info[$k]["name"],$sites_name_global);				
				$sites_color[$k]=$sites_color_global[$key];	
				
			}
			else
			{
				$sites_color[$k]= get_random_color();
							
			}	
			
			$sites_name[$k]=$sites_info[$k]["name"];
			
		}

	
	$stack = array();
	$stack_font = array();
	
//	draw up string		
	$up_str_len = strlen($up_string);
	$pad_num = 0; 	
	
	
	if(strlen($up_string)%$line_len!=0)
	{
		$pad_num = $line_len-strlen($up_string)%$line_len;
		$up_string = str_pad($up_string,strlen($up_string)+$line_len-strlen($up_string)%$line_len, " ", STR_PAD_LEFT);
	}
	
	$str_len = strlen($up_string);
			
	for ($i=0; $i<strlen($up_string);$i++)
	{

		if($i==0){

			$labeled_sites_seq = $labeled_sites_seq;
		}


		if (!($i % $line_len)==0 && ($i % $sep_len)==0) {

			$labeled_sites_seq = $labeled_sites_seq. " ";
		}
		$abs_pos=0;
		$keys = array();

		if($i>$pad_num)
		{

			if($orientation=="Clockwise")
			{
				$abs_pos = intval(-$up_str_len)+intval(-$pad_num)+$left_pos+$i;
				if($abs_pos<1)
				{
					$abs_pos = $abs_pos + $GENOME_LENGTH;
				}
				$keys = array_keys($sites_left_end,$abs_pos);
				//					if($keys)
				//				echo "<br>in open\$abs_pos ".$abs_pos."";
			}
			else
			{
				$abs_pos = intval($up_str_len)+intval($pad_num) + $right_pos -$i;
				if($abs_pos > $GENOME_LENGTH)
				{
					$abs_pos = $abs_pos - $GENOME_LENGTH;
				}

				$keys = array_keys($sites_right_end,$abs_pos);
			}

			if(count($keys)>0){
//				push_font($keys, $sites_color, $sites_name, $sites_left_end, $sites_right_end, &$stack, &$stack_font, &$labeled_sites_seq);
				push_font($keys, $sites_color, $sites_name, $sites_left_end, $sites_right_end, $stack, $stack_font, $labeled_sites_seq);

			}
		}
		$labeled_sites_seq = $labeled_sites_seq. $up_string[$i];
		if($i>$pad_num)
		{
			$keys = array();

			if($orientation=="Clockwise")
			{
				$keys = array_keys($sites_right_end, $abs_pos);
				
			}
			else
			{

				$keys = array_keys($sites_left_end,$abs_pos);
			}

			if(count($keys)>0)
			{
				pop_font($keys, $stack, $stack_font, $labeled_sites_seq,$sites_name, $sites_left_end, $sites_right_end);
//				pop_font($keys, &$stack, &$stack_font, &$labeled_sites_seq,$sites_name, $sites_left_end, $sites_right_end);

			}
		}

		if (((($i+1) % ($line_len))==0) && ($i>0)) {

			$labeled_sites_seq = $labeled_sites_seq. "\n";
		}

	}

	if($i>0)
	{
		$labeled_sites_seq = $labeled_sites_seq. "\n";
	}		
			
			
			
			
			
//main string here
			$main_str_len = strlen($main_string);
						
			for ($i=0; $i<$main_str_len;$i++) {

				if (($i % $line_len)==0) { 
					
						
					if($i>0){
						$labeled_sites_seq = $labeled_sites_seq. "\n";
					}else{
						$labeled_sites_seq = $labeled_sites_seq;
					}
				}
				elseif (($i % $sep_len)==0) {

						$labeled_sites_seq = $labeled_sites_seq. " ";
				}		
				$abs_pos=0;
				$keys = array();				
				if($orientation=="Clockwise")
				{
					$abs_pos =$left_pos+$i;
					if($abs_pos > $GENOME_LENGTH)
					{
						$abs_pos = $abs_pos - $GENOME_LENGTH;
					}
					
					$keys = array_keys($sites_left_end, $abs_pos);
				}
				else
				{
					$abs_pos = $right_pos -$i;
					
					if($abs_pos<1)
					{
						$abs_pos = $abs_pos + $GENOME_LENGTH;
					}
					
					$keys = array_keys($sites_right_end,$abs_pos);
				}
				
				if(count($keys)>0)
				{
//					push_font($keys, $sites_color, $sites_name, $sites_left_end, $sites_right_end, &$stack, &$stack_font, &$labeled_sites_seq);
					push_font($keys, $sites_color, $sites_name, $sites_left_end, $sites_right_end, $stack, $stack_font, $labeled_sites_seq);
				}
					
					
					$labeled_sites_seq = $labeled_sites_seq. $main_string[$i];
					
					
				$keys = array();	
					if($orientation=="Clockwise")
					{
						
						$keys = array_keys($sites_right_end,$abs_pos);
					}
					else
					{
						
						$keys = array_keys($sites_left_end,$abs_pos);
					}

				if(count($keys)>0)
				{
					 pop_font($keys, $stack, $stack_font, $labeled_sites_seq,$sites_name, $sites_left_end, $sites_right_end);
//					 pop_font($keys, &$stack, &$stack_font, &$labeled_sites_seq,$sites_name, $sites_left_end, $sites_right_end);
			
				}
			}
			
			
			
//			draw the down string part for clock wise gene
//			       or actually up string part counterclockwise gene 
//			
			$down_str_len = strlen($down_string);
			if(strlen($down_string)%$line_len!=0)
			{
				$down_string = str_pad($down_string,strlen($down_string)+$line_len-strlen($down_string)%$line_len, " ", STR_PAD_RIGHT);
			}
			if($down_str_len>0)
			{
				$labeled_sites_seq = $labeled_sites_seq. "\n";
			}
			
			for ($i=0; $i<strlen($down_string);$i++) {

				if (($i % $line_len)==0) {

					$labeled_sites_seq = $labeled_sites_seq. "\n";

				}
				elseif (($i % $sep_len)==0 && $i<$down_str_len) {

					$labeled_sites_seq = $labeled_sites_seq. " ";
				}
				if($i<$down_str_len)
				{
					$abs_pos=0;
					$keys = array();
					if($orientation=="Clockwise")
					{
						$abs_pos =$right_pos+$i+1;
						if($abs_pos > $GENOME_LENGTH)
						{
							$abs_pos = $abs_pos - $GENOME_LENGTH;
						}

						$keys = array_keys($sites_left_end, $abs_pos);
					}
					else
					{
						$abs_pos = $left_pos -$i-1;

						if($abs_pos<1)
						{
							$abs_pos = $abs_pos + $GENOME_LENGTH;
						}


						$keys = array_keys($sites_right_end, $abs_pos);
					}

					if(count($keys)>0)
					{
						push_font($keys, $sites_color, $sites_name, $sites_left_end, $sites_right_end, $stack, $stack_font, $labeled_sites_seq);
//						push_font($keys, $sites_color, $sites_name, $sites_left_end, $sites_right_end, &$stack, &$stack_font, &$labeled_sites_seq);
					}
//				}

				$labeled_sites_seq = $labeled_sites_seq. $down_string[$i];
				
//				if($i<$down_str_len)
//				{
					$keys = array();
					if($orientation=="Clockwise")
					{

						$keys = array_keys($sites_right_end,$abs_pos);
					}
					else
					{

						$keys = array_keys($sites_left_end,$abs_pos);
					}
					if(count($keys)>0)
					{
//						pop_font($keys, &$stack, &$stack_font, &$labeled_sites_seq,$sites_name, $sites_left_end, $sites_right_end);
						pop_font($keys, $stack, $stack_font, $labeled_sites_seq,$sites_name, $sites_left_end, $sites_right_end);

					}
				}

			}
			
}