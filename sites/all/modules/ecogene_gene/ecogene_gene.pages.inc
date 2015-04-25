
<?PHP
include("sites/all/include/ecoFunction.php");
include("sites/all/include/format_description.php");
include("sites/all/include/dblink.php");
include("sites/all/include/genepage_map.php");
include("sites/all/include/igr_map.php");
include("sites/all/include/sites_genemap.php");


define ("SEQ_LINE", 75);


/**
 * Enter description here ...
 * Get the information of the gene into an array $gene_info
 * @param unknown_type $eg_id
 * @return multitype: $gene_info
 * $gene_info An associative array containing:
 */
function get_gene_info($eg_id)
{
	
	// prepare the array used fomatting the query
	$add_left_end = isset($_GET['add_left_end'])? $_GET['add_left_end'] : null;
	
	$args = array(':eg_id'=>$eg_id, ':add_left_end'=>$add_left_end); 
	
	// initialize the retun gene information array
	$gene_info = array();
	
	// query for basic gene information
	$query = db_query(
	
		"SELECT g.eg_id, g.name, g.description, g.status, 
			g.comments, g.type,	g.length, g.mnemonic_name, 
			g.eck_id, ta.left_end, ta.right_end, ta.orientation 
		FROM t_gene g, t_address ta 
		WHERE g.address_id = ta.address_id AND g.eg_id =:eg_id",
		array(':eg_id'=>$eg_id)
	);
	
	$gene_info = $query->fetchAssoc();
	
	if ( is_null($gene_info["description"]) ) 	{	

		$gene_info["description"] = "None";
	}
	
	$query = db_query("	SELECT 	gs.name 
					FROM	t_gene_synonym gs 
					WHERE   gs.eg_id = :eg_id",array(':eg_id'=>$eg_id)	);

	
	
	$gene_info['synonyms'] ='';
	$synonyms = $query->fetchAll();
	if($synonyms)
	{
		foreach ($synonyms as $row)
		{
			$gene_info['synonyms'] .= $row->name.'; ';
		}
		$gene_info['synonyms'] = substr($gene_info['synonyms'], 0, strlen($gene_info['synonyms'])-2);
	}else 
	{
		$gene_info['synonyms'] = 'None';
	}
		
	// get the previous and next Alphabetical genes for navigaion
	$gal_row = geneAlphabetical($gene_info['name']); 
	$gene_info['gal_row'] = $gal_row;
	
	// check if the gene is a Pseudogene
	$gene_info['is_pseudo'] = isPseudogene($eg_id);  
	

	// if the gene is a multi address, get the virtual eg_id and address info
	$gene_info['virtual_eg_id'] = NULL;
//	if($add_left_end)
//	{
//		$query = db_query(
//			"select * 
//			from 	t_gene_multi_address 
//			where eg_id=:eg_id and left_end= :add_left_end", 
//			$args
//		);
//		
//		if($row_multi_address = $query->fetchAssoc())
//		{
//			$gene_info['left_end'] = $add_left_end;
//			$gene_info['right_end'] = $row_multi_address['right_end'];
//			$gene_info['virtual_eg_id'] =  $row_multi_address['virtual_eg_id'];
//		}
//	}
//	else
//	{
//		$query = db_query("
//			select * 
//			from t_gene_multi_address 
//			where eg_id=:eg_id order by left_end desc ", 
//			array(':eg_id'=>$eg_id)
//		);
//		
//		if($row_multi_address = $query->fetchAssoc())
//		{
//			$gene_info['virtual_eg_id'] =  $row_multi_address['virtual_eg_id'];
//		}
//	}

	//	query to get the B number
			
	$query = db_select('t_biodatabase_link');
	$query = $query->fields('t_biodatabase_link',array('accession_id'));
	$query = $query->condition('biodatabase_id', 3);
	
	
	if($gene_info['virtual_eg_id'])	{		
		
		$query = $query->condition('eg_id', $gene_info['virtual_eg_id']);		
	}
	else {
		
		$query = $query->condition('eg_id', $gene_info['eg_id']);			
	}
	$query->orderBy('accession_id');
	$records = $query->execute();
	foreach ($records as $id) {
			$b_num[] = $id->accession_id;
	}
	//secondary b_num
	$query = db_select('t_biodatabase_link');
	$query = $query->fields('t_biodatabase_link',array('accession_id'));
	$query = $query->condition('biodatabase_id', 108);
	
	
	if($gene_info['virtual_eg_id'])	{		
		
		$query = $query->condition('eg_id', $gene_info['virtual_eg_id']);		
	}
	else {
		
		$query = $query->condition('eg_id', $gene_info['eg_id']);			
	}
	$query->orderBy('accession_id','DESC');
	$records = $query->execute();
	foreach ($records as $id) {
			$b_num[] = $id->accession_id;
	}

	$gene_info ['b_num'] = $b_num;
	
	// check the verified info

	$query = db_query("
		SELECT * FROM t_gene_reference_link 
		WHERE eg_id = :eg_id and verified=1",
		array(':eg_id'=>$eg_id)
	);
	
	$gene_info ['is_verified'] = ($query->fetchAssoc())? true : false;
	
	
	// Get the join information
	
	$join_title = '';
	$join_text = '';
	$html_text = '';	
	ecogene_gene_get_join($eg_id,$join_title,$join_text, $html_text, $gene_info['name']);
	$gene_info ['join_html_text'] = $html_text;
	$gene_info ['join_title'] = $join_title;
	$gene_info ['join_text'] = $join_text;
	return $gene_info;
}

function get_page_title($gene_info)
{
	$title= '<h1 id="gene_page_title"> GenePage for the <span id="g_name">'.$gene_info['name'];
	
	if($gene_info['is_pseudo']=="PSEUDO")
		$title .=	'\'</span><span id="pseudo_tag"> pseudogene </span> ';	
	else
		$title .= '</span> gene ';
	$title .=  'of <span id="escherichia">Escherichia coli</span> K-12 </h1>';
	return $title;
}
function display_gene_info($gene_info,$intergene_info,$prev_gene,$next_gene)
{
	global $base_url,$GENOME_LENGTH;
	
	$sites = isset($_GET['sites'])? $_GET['sites']:'BamHI, EcoRI, HindIII';

	$sites_array = explode(', ', $sites);
	$map_magnify = isset($_REQUEST['map_magnify'])? $_REQUEST['map_magnify']:1;	
	$extra_para = '&'.drupal_http_build_query(drupal_get_query_parameters(array('sites'=>$sites,'map_magnify'=>$map_magnify)) );
	
	
	
	$eg_id = $gene_info['eg_id'];
	$content = '<div id="gene_info">';
	$b_num='';
	$i = 0; 
	foreach ($gene_info['b_num'] as $value) {
		$i++;
		if($i==1)
			$b_num .= '<strong>'.$value.'</strong>';
		else
			$b_num .= ', '.$value;
	}
	
	$content .= '
	<table>
		<tr>
			<td>
				<A HREF="'.$base_url.'?q=gene/'.
				$gene_info['gal_row'][0].'"><IMG SRC="'.$base_url.'/sites/all/images/cylarrw.gif" 
				title="Alphabetical previous gene"></A>
			</td>
			<td>
				Primary Gene Name: <strong>'.$gene_info['name'];
	$content.=	(strcmp($gene_info['is_pseudo'],"PSEUDO")==0)?'\'':'';
	$content.=	'</strong><br /> 
				EcoGene Accession Number: '. $gene_info['eg_id'].'<br />  
				K-12 Gene Accession Number: '. $gene_info['eck_id'].'<br />
				MG1655 Gene Identifier: '.$b_num.'
			</td>
			<td>
				<A HREF="'.$base_url.'?q=gene/'.
				$gene_info['gal_row'][1].'"><IMG SRC="'.$base_url.'/sites/all/images/cyrarrw.gif" 
				title="Alphabetical next gene"></A>
			</td>
		</tr>
	</table>	
	<table>
	<tr><td>
		
			Gene Name Mnemonic: '. $gene_info['mnemonic_name'].' <br />
			Alternate Gene Symbols: '.$gene_info['synonyms']. '<br />
			Description: '. $gene_info['description'] .'<br />
	</td></tr>';
	if (isset($gene_info["join_html_text"])&&!empty($gene_info["join_html_text"]))
	{
		$dna_button_text = "DNA Reconstruction";
	}
	else
	{
		$dna_button_text = "DNA Sequence";
	}
				$content.=
	'<tr>
		<td>
		<form name="dnaForm" id="dnaForm" action="'.$base_url.'" method="get">
		<input type="hidden" name="q" value="gene/'.$eg_id.'/dnasequence">
		<input class="form-submit" type="submit" name="submit" value="'.$dna_button_text.'">
		<input type=hidden name="eg_id" value="'.$gene_info['eg_id'].'" size=3>
		<input type=hidden name=join_text value="'.$gene_info['join_text'].'">
		<input type=hidden name=join_html_text value="'.$gene_info['join_html_text'].'">
		
		&nbsp # bp Upstream <input type=text name="us" value="20" size=3>
		# bp Downstream <input type=text name="ds" value="40" size="3">
		';
		foreach ($sites_array as $value)
				$content.='<input type="hidden" name="sites[]" value="'.$value.'">';
		$content .='</form>
		</td>
	</tr>';
	$content.='</table>
	<table>
		
	<tr>
		<td width="35%">';
				
//				if( $gene_info['type'] == "aa" ) {
//					$content .= '<A class="button" HREF="'.$base_url.'/?q=gene/'.$eg_id.'/protein#prot_seq">
//					Protein Sequence</A>';
//				}else if( $gene_info['type'] == "nt" ){
//					$content .= '<A HREF="rnaSequence.php?eg_id='.$eg_id.'">
//					RNA Sequence</A>';
//				}
				if ($gene_info['is_verified']) {
					$content .= '<A class="button" HREF="'.$base_url.'?q=gene/'.$eg_id.'/proteinrna">
					Verified Start</a> ';
				}
				$content .= '
			</td>
			<td width="35%">';	
				if( $gene_info['type'] == "aa" ) {
					$content .= 'MW: '.sprintf("%.2f",calculate_MW($eg_id)); 
				}else if( $gene_info['type'] == "nt" ){
					$content .= 'MW: '.sprintf("%.2f",calculate_MW($eg_id,'nt')); 
				}
				$content .= ' ---------'
			. calculate_length($eg_id,$gene_info['type'])
				.' '; 
				if( $gene_info['type'] == "np" ) {
					$content .= "nt"; 
				}else {
					$content .= $gene_info['type'];
				}
				$content .= '
			</td>
	
			<td width="30%">';
				if( $gene_info['type'] == 'aa' ) {
					$content .= '<A HREF="/prerun/'.
						$gene_info['eg_id'].'.html" 
						target="_blank">
					Pre-Run BlastP UniProt</A><br />';
					$content .= '<A HREF="/prerun2/'.
						$gene_info['eg_id'].'.html" 
						target="_blank">Pre-Run BlastP NR+Env</A><br />';
				} else if($gene_info['type'] == 'nt' ){
					$content .= '<A HREF="/prerun/'.
						$gene_info['eg_id'].'.html" 
						target="_blank">Pre-Run BlastN</A><br />';
				}
				$content .= '
			</td>	
		</tr>
	</table>';
				
	$content .= 
	'<table>
		<tr>
			<td>
				<A HREF="'.$base_url.'?q=gene/'.
				$prev_gene['eg_id'].$extra_para.'"><IMG SRC="'.$base_url.'/sites/all/images/cylarrw.gif" 
				title="Previous gene by address"></A>
			</td>
			<td>
				Left End: '.$gene_info['left_end'].' <br />
				<span class="intergene" onmouseover="toggleDiv(\'left_intergene\', 1);" onmouseout="toggleDiv(\'left_intergene\', 0);")">Left Intergenic Region</span>
				<div id="left_intergene" class="intergene">
					<p> Name: '.$intergene_info[0]['name'].'</p>
					<p> Length: '.$intergene_info[0]['length'].'</p>
					<p> Orientation: '.$intergene_info[0]['ori'].'</p>
					<p> Left_end: '.$intergene_info[0]['left_end'].'</p>
					<p> Right_end: '.$intergene_info[0]['right_end'].'</p>
					<p> Centisome: '.$intergene_info[0]['cent'].'</p>
				</div>
			</td>
			<td>
				Genomic Address ';
				if (isset($gene_info["join_html_text"])&&!empty($gene_info["join_html_text"])) {
					$content .= '<span class="intergene" onmouseover="toggleDiv(\'join\', 1);" onmouseout="toggleDiv(\'join\', 0);")"> join </span>';
					$content .=  '<div id="join" class="intergene">'
									.$gene_info["join_html_text"].
								'</div>';
				}
		$content .='	<br />'.$gene_info['orientation'].'<br />'.
				'Minute or Centisome (%) = '.number_format($gene_info["left_end"] / $GENOME_LENGTH * 100,2) .'
			</td>
			<td>
				Right End: '.$gene_info['right_end'].' <br />
				<span class="intergene" onmouseover="toggleDiv(\'right_intergene\', 1);" onmouseout="toggleDiv(\'right_intergene\', 0);")">Right Intergenic Region</span>
				<div id="right_intergene" class="intergene">
				<p> Name: '.$intergene_info[1]['name'].'</p>
					<p> Length: '.$intergene_info[1]['length'].'</p>
					<p> Orientation: '.$intergene_info[1]['ori'].'</p>
					<p> Left_end: '.$intergene_info[1]['left_end'].'</p>
					<p> Right_end: '.$intergene_info[1]['right_end'].'</p>
					<p> Centisome: '.$intergene_info[1]['cent'].'</p>
				</div>
			</td>	
			<td>
				<A HREF="'.$base_url.'?q=gene/'.
				$next_gene['eg_id'].$extra_para.'"><IMG SRC="'.$base_url.'/sites/all/images/cyrarrw.gif" 
				title="Next gene by address"></A>
				
			</td>
		</tr>
	</table>';
	$content .= '</div>';
				
	if (!is_null($gene_info["comments"])&&!empty($gene_info["comments"])) {
		
		$content .= '<div class="EG_CollapsibleMap_TitleDiv">
					<div>
						<a class="EG_CollapsibleMap_TitleAhref" href="javascript:void(0)" onclick="toggleBlockDiv(\'gene_comments\', \'gene_comments_CollapsibleMap\');">
						<img id="gene_comments_CollapsibleMap" class ="cl_CollapsibleArea_collapsing" src="/sites/all/images/empty.gif" border="1">
						<span class="EG_CollapsibleMap_Title">Comments</span></a>
						<div class="EG_CollapsibleMap_HrDiv"> <hr class="EG_CollapsibleMap_Hr"></div>
					</div></div>';
		$content .= '<div id="gene_comments" style="display: block;"><p>'.$gene_info['comments'].'</p></div>';
		

	}


	
	
	return $content;
}
/**
 * Enter description here ...
 * return the main content of the gene page 
 * @param unknown_type $eg_id
 * @return string
 */
function ecogene_gene_content($eg_id) {
	
	global $base_url,$GENOME_LENGTH;
	global $map_magnify;
		// connect to the ecogene database
	db_set_active("ecogene");
	
	
	// get all the information of the gene
	$gene_info = get_gene_info($eg_id);
	$prev_gene = array();
	$curr_gene = array();
	$next_gene = array();
	get_address_gene($eg_id, $gene_info['left_end'],$prev_gene,$curr_gene,$next_gene);
	$intergene_info[0] = get_intergene_info($prev_gene, $curr_gene);
	$intergene_info[1] = get_intergene_info($curr_gene, $next_gene);
	
	// title of the gene page	
	drupal_set_title(get_page_title($gene_info), PASS_THROUGH);
	
	//content of the gene information
	$content = display_gene_info($gene_info, $intergene_info,$prev_gene,$next_gene);

	// gene map
	$map_magnify = isset($_REQUEST['map_magnify'])? $_REQUEST['map_magnify']:1;		
	$map_magnify = $map_magnify>3? 3:$map_magnify;
	$map_magnify = $map_magnify<1? 1:$map_magnify;
	
	$sites = isset($_GET['sites'])? $_GET['sites']:'BamHI, EcoRI, HindIII';
	$sites_checked = isset($_REQUEST['sites_checked'])? $_REQUEST['sites_checked']:$sites;
	$extra_para = '&'.drupal_http_build_query(drupal_get_query_parameters(array('sites'=>$sites,'map_magnify'=>$map_magnify)) );
		
	$tmpfname=file_directory_temp()."/".$eg_id.$map_magnify.$gene_info['left_end'].".png";
	$map = array();
	genepage_map($tmpfname, $map, $map_magnify, $eg_id, $gene_info['left_end']);
	

	$content.= "<MAP NAME=\"map1\">";
		for ($t=0;$t<count($map);$t++)
		{

			$thelink = $map[$t]['link']."?".$map[$t]['id_name']."=".$map[$t]['id'];//.$map[$t]['extra_link'];
			$thelink = str_replace("geneInfo.php?eg_id=", $base_url."?q=gene/",$thelink).$extra_para;
			$thelink = str_replace("topic.php?topic_id=","?q=topic/",$thelink);
			
			if(strchr($thelink,'regulon.php?eg_id='))
			{
				$thelink = str_replace("regulon.php?eg_id=","?q=gene/",$thelink);
				$thelink .= '&view_igr=yes#id_CollapsibleMap';
			}
			if( isset( $map[$t]['left_end'] ) && isset( $map[$t]['right_end'] ))
			{	
				$content.= "<AREA NAME=\"area".$t."\" COORDS=\"".$map[$t]['x1'].",".$map[$t]['y1'].",".$map[$t]['x2'].",".$map[$t]['y2']."\" title=\"Left End: ". $map[$t]['left_end'] ."  Right End: ".$map[$t]['right_end']."\" HREF=\"$thelink\" target=\"_parent\">";
			}
			else {
				
				$content.= "<AREA NAME=\"area".$t."\" COORDS=\"".$map[$t]['x1'].",".$map[$t]['y1'].",".$map[$t]['x2'].",".$map[$t]['y2']."\" alt=\"Go to GenePage\" HREF=\"$thelink\" target=\"_parent\">";
			}

		}
		$content.="</MAP>";
	$url_curr = drupal_http_build_query(drupal_get_query_parameters($_GET,array_merge(array('map_magnify'), array_keys($_COOKIE))));
	$url_map_plus = '?'.$url_curr.'&map_magnify='.($map_magnify>2? 3:$map_magnify+1);
	$url_map_minus = '?'.$url_curr.'&map_magnify='.($map_magnify<2? 1:$map_magnify-1);
	$content.= '<div class="EG_CollapsibleMap_TitleDiv">
					<div>
						<a class="EG_CollapsibleMap_TitleAhref" href="javascript:void(0)" onclick="toggleBlockDiv(\'gene_map\', \'gene_CollapsibleMap\');">
						<img id="gene_CollapsibleMap"  class ="cl_CollapsibleArea_collapsing" src="/sites/all/images/empty.gif">
						<span class="EG_CollapsibleMap_Title">Gene Map</span></a>
						
						<a class="button EG_CollapsibleMap_TitleAhref" href="javascript: void(0)" onClick="javascript:window.open(\'http://www.ecogene.org/PrintMap/printmap150k.php?eg_id='.$eg_id.'\', \'EcoMap\', \'width=400,height=200,scrollbars=yes,resizable=yes,toolbar=yes\')">
						PDF EcoMap</a>
						<div class="EG_CollapsibleMap_HrDiv"> <hr class="EG_CollapsibleMap_Hr"></div>
					</div></div>';
	$content.= '<div id="gene_map"><p><img SRC="'.file_create_url($tmpfname).'" usemap="#map1" /></p>';
	$content.='<form action="?q=ecosearch/gene/search" method="POST">	
		<a href ="'.$url_map_plus.'">
		<img src="'.file_create_url("sites/all/images/mag_2.png").'" title="zoom in" alt="zoom in" width="24" height="24">
		</a>
		<a href ="'.$url_map_minus.'">
		<img src="'.file_create_url("sites/all/images/mag_1.png").'" title="zoom out" alt="zoom out" width="24" height="24">
   		</a><br />
		<input title="Search for gene or min(Cs)" type="text" name="v_minute"  size="2" value=""> 
   		<br /><INPUT type="submit" name="submit" value="Find">
   		</form></div>';

	$tmpfname=file_directory_temp()."/".$eg_id."sites".rand().".png";
	$mapofsites = array();
	
	
	if(sites_map($tmpfname, $sites_checked, $map_magnify,$gene_info['left_end'],$mapofsites))
	{
		
		$content.= '<map name="map_site">';
		for ($t=0;$t<count($mapofsites);$t++){
			$content.= "<AREA NAME=\"area_site".$t."\" 
				COORDS=\"".$mapofsites[$t]->left.",".$mapofsites[$t]->top.",".$mapofsites[$t]->right.",".$mapofsites[$t]->bottom."\" 
				title=\"".$mapofsites[$t]->alt."\" target=\"_parent\" HREF=\"".$mapofsites[$t]->link."\"".">";
		}
		$content.= "</map>";
		$content.= '<div class="EG_CollapsibleMap_TitleDiv">
					<div>
						<a class="EG_CollapsibleMap_TitleAhref" href="javascript:void(0)" onclick="toggleBlockDiv(\'sites_map\', \'sites_CollapsibleMap\');">
						<img id="sites_CollapsibleMap" class ="cl_CollapsibleArea_collapsing" src="/sites/all/images/empty.gif" border="1">
						<span class="EG_CollapsibleMap_Title">Sites Map</span></a>
						<div class="EG_CollapsibleMap_HrDiv"> <hr class="EG_CollapsibleMap_Hr"></div>
					</div></div>';
		
		$content.= '<div id="sites_map">';	
		$content.= '<img src="'.file_create_url($tmpfname).'" usemap="#map_site" />';
		$content.='';
		
		$sites_array = explode(', ', $sites);
		
		$get = drupal_get_query_parameters($_GET,array('sites','q'));
		
		$site_form_action = drupal_http_build_query(drupal_get_query_parameters(array_merge(array('sites'=>$sites), $get), array_keys($_COOKIE)));
		
		$content.= '<div id="sites_select"><form name="site_form" method="post" action="?q='.$_GET['q'].'&'.$site_form_action.'"><p>';	
		$content.= ' <input type="hidden" name="sites_checked", id="sites_checked" value ="'.$sites_checked.'">';	
		
		for ($i = 0; $i < count($sites_array); $i++) {
			$id = trim($sites_array[$i]);
			if(stristr($sites_checked, $id))
				$content.= ' <input type="checkbox" id ="'.$id.'" name="sites" checked onclick="change_sites(this,\'sites_checked\')"><span> '.$id.' </span> ';
			else
				$content.= ' <input type="checkbox" id ="'.$id.'" name="sites" unchecked onclick="change_sites(this,\'sites_checked\')"><span> '.$id.' </span> ';
		}	
		
		$gene_neighbor = intval(10000/pow(2,$map_magnify-1));

		$site_left_end = $gene_info['left_end']-$gene_neighbor/2;
		$site_right_end = $gene_info['left_end']+$gene_neighbor/2;
	
		
		$request= array_merge(array('sites'=>$sites_array, 'type'=>'site','left_end'=>$site_left_end, 'right_end'=>$site_right_end),$get);
		$url_curr = drupal_http_build_query(drupal_get_query_parameters($request,array_keys($_COOKIE)));
	
		$content.='</p></form></div><div><a class="button" href="?q='.$_GET['q'].'/dnasequence&'.$url_curr.'">View Sequence</a>
		<a class="button" onclick="javascript:window.open(\'/sites/all/include/restriction_site_list.php?palindromic=1\', \'popwindow\', \'height=500, width=360, top=100,  left=100 toolbar=no, menubar=1, scrollbars=1, resizable=1, location=no, status=no\')">
		Select Sites</a>
		</div></div>';
		
		
	}
	
	
	
	
	$gene_neighbor_left = $curr_gene['left_end']>$prev_gene['right_end']+1? $prev_gene['right_end']-200:$curr_gene['left_end']-200;
	$gene_neighbor_right = $next_gene['left_end']>$curr_gene['right_end']+1? $next_gene['left_end']+200:$curr_gene['left_end']+200;
	$igr = array();
	$tfbs = array();
	is_in_range_address($gene_neighbor_left, $gene_neighbor_right,$igr);
	tfbs_in_range_address($gene_neighbor_left, $gene_neighbor_right,$tfbs);
	
	$left_end = $curr_gene['left_end']? $curr_gene['left_end']:"";

	if((count($igr)+count($tfbs))>0)
	{
		$content.= '<div class="EG_CollapsibleMap_TitleDiv" id="id_CollapsibleMap">
					<div>
						<a class="EG_CollapsibleMap_TitleAhref" href="javascript:void(0)" onclick="toggleBlockDiv(\'igr_map\', \'igr_CollapsibleMap\');">
						<img id="igr_CollapsibleMap"  class ="cl_CollapsibleArea_collapsing" src="/sites/all/images/empty.gif">
						<span class="EG_CollapsibleMap_Title">Intergenic Region</span></a>
						<div class="EG_CollapsibleMap_HrDiv"> <hr class="EG_CollapsibleMap_Hr"></div>
					</div></div>';
		$url_curr = $base_url.'?'.drupal_http_build_query(drupal_get_query_parameters($_GET,array_keys($_COOKIE)));
		$content.= '<div id="igr_map">';		
		$content .= genepage_igr_map($gene_neighbor_left,$gene_neighbor_right,$eg_id, $url_curr); 	
		$content.='</div>';

	}
	
	db_set_active("default");
	
	return $content;
}

function ecogene_gene_reference($eg_id) {
	
	$review=empty($_GET['review'])?'no':'yes';
	$content = '';
	db_set_active("ecogene");
	
	//query to get the total number of refrence;
	$args = array();
	$args['eg_id'] = $eg_id;
		
	$result = db_query("SELECT COUNT(pubmed_id) total FROM {t_gene_reference_link} WHERE eg_id =:eg_id ",$args);
	if($row = $result->fetchAssoc())
		$total = $row['total'];
		
	
	$query_gene_reference_link = "SELECT ".
			"r.title, ".
			"date_format(r.date_published,'%Y') year, ".
			"r.pubmed_id, r.medline_id, pagination, journal, volume, ".
		 	"concat(a.last_name,' ',a.initials) as author_name, r.xml_link, r.publication_type ".
		 "FROM ".
			"t_reference r,t_gene_reference_link grl, ".
			"t_reference_author_link ral, t_author a ".
		 "WHERE ".
			"grl.eg_id = :eg_id and ".
			"grl.pubmed_id = r.pubmed_id and ".
			"r.pubmed_id = ral.pubmed_id and ".
			"ral.author_id = a.author_id ";
				
	
	
	if($review=='yes')
    	$query_gene_reference_link = $query_gene_reference_link." and ".
				"( r.publication_type like '%Review%' or ".
				" r.publication_type like '%Comment%' ) ";		     

    $query_gene_reference_link = $query_gene_reference_link."ORDER BY ".
			"date_published desc, pubmed_id desc,title asc,ral.seq_no asc ";
			

 	$result = db_query($query_gene_reference_link, $args);
//	
    
    $content = $content. "<H2>Bibliography ("	.$total." total) : ";
	
    if($review=='no')
 	{
    	$content = $content. "<A HREF='?q=gene/$eg_id/reference&review=yes"."' ><font color='#3333FF'>Review Only"."</font> </A>&nbsp <br /></H3>";
 	}
 	if($review=='yes')
 	{
    	$content = $content. "<A HREF='?q=gene/$eg_id/reference"."' ><font color='#3333FF'>All References"." </font></A>&nbsp <br /></H3>";
 	}
	
	
 	$row_previous = $result->fetchAssoc();
 	$author_name = $row_previous["author_name"];
 	$years=array();
 	$lists=array();
	while ($row = $result->fetchAssoc())
	{	
		
		if ($row_previous["pubmed_id"] == $row["pubmed_id"]) {
			$row["author_name"] = $row_previous["author_name"]. ", ".$row["author_name"];
		} else 
		{
			$list = $row_previous["author_name"]. " (".$row_previous['year'].") ".$row_previous["title"]." ".$row_previous["journal"]." ".$row_previous["volume"].":".$row_previous["pagination"];
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
		$list = $row_previous["author_name"]. " (".$row_previous['year'].") ".$row_previous["title"]." ".$row_previous["journal"]." ".$row_previous["volume"].":".$row_previous["pagination"];
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

	if($years){
	$content .='<div id="ref_year_list"><p>';
	for ($i=0;$i<count($years);$i++)
	{
		if($i==count($years)-1)
		{
			$content .= "<a href=\"#year_$years[$i]\">$years[$i] </a>";
		}
		elseif ($years[$i]!=$years[$i+1])
		{
			$content .= "<a href=\"#year_$years[$i]\">$years[$i] </a>";
		}
	}
	$content .="</p></div>";
	
	for ($i=0;$i<count($years);$i++)
	{
		$refs[]= $lists[$i];
		if($i==count($years)-1)
		{
			
			$content .= theme_item_list(array('items'=>$refs, 'title'=>'<a name="year_'.$years[$i].'"></a>'.$years[$i].' <a href="#"> Top</a>', 'type'=>'ul', 'attributes' => array()));
			unset($refs);
			

		}elseif ($years[$i]!=$years[$i+1])
		{
			
			$variables = array('items'=>$refs, 'title'=>'<a name="year_'.$years[$i].'"></a>'.$years[$i].' <a href="#"> Top</a>', 'type'=>'ul', 'attributes' => array());
//			$items = $variables['items'];
			$content .= theme_item_list($variables);
			unset($refs);

		}
	}
	}


db_set_active("default");
return $content;
}
function get_intergene_info($row_left_gene, $row_right_gene)
{
	global $GENOME_LENGTH;
	$intgene_name = $row_left_gene['name'].'_'.$row_right_gene['name'];
	
	if(($row_left_gene['orientation']=='Clockwise') && ($row_right_gene['orientation']=='Clockwise'))
	{
		$intgene_orientation = 'Codirectional+';
	}
	elseif (($row_left_gene['orientation']=='Clockwise') && ($row_right_gene['orientation']=='Counterclockwise'))
	{
		$intgene_orientation = 'Convergent';
	}
	elseif (($row_left_gene['orientation']=='Counterclockwise' )&& ($row_right_gene['orientation']=='Clockwise'))
	{
		$intgene_orientation = 'Divergent';
	}
	else
	{
		$intgene_orientation = 'Codirectional-';
	}
	
	if($row_right_gene['left_end']>$row_left_gene['right_end']+1){
		$overLap = 0;
		$left_end = $row_left_gene['right_end'] + 1;
		$right_end = $row_right_gene['left_end'] - 1;
		$intgene_length = $right_end - $left_end + 1;
		$intgene_length = $intgene_length.' bp gap';
		$centisome = $left_end / $GENOME_LENGTH * 100;

	}
	elseif($row_right_gene['left_end']<$row_left_gene['right_end']+1){
		$overLap = 1;
		$left_end = $row_right_gene['left_end'];
		$right_end = $row_left_gene['right_end'];
		$intgene_length = $right_end - $left_end + 1;
		if($intgene_length>$GENOME_LENGTH/2)//beginning of the genome
		{
			$left_end = $row_left_gene['right_end']+1;
			$right_end = $row_right_gene['left_end']-1;
			$intgene_length = $right_end - $left_end + 1 + $GENOME_LENGTH;
			$intgene_length = $intgene_length.' bp gap';
		}else 
		{
			$intgene_length = $intgene_length.' bp overlap';
		}	
		
		$centisome = $left_end / $GENOME_LENGTH * 100;

	}else {
		$intgene_length = '0 bp';
		$left_end = 'Null';
		$right_end = 'Null';
		$centisome = $row_right_gene['left_end'] / $GENOME_LENGTH * 100;
		
	}
	$centisome = sprintf("%.2f",$centisome);
	$intergene_info['name'] = $intgene_name;
	$intergene_info['length'] = $intgene_length;
	$intergene_info['ori']= $intgene_orientation;
	$intergene_info['left_end'] = $left_end;
	$intergene_info['right_end'] = $right_end;
	$intergene_info['cent'] = $centisome;
	return $intergene_info;
}


function ecogene_gene_askakeio($eg_id)
{
	
	$content = "";
	$content_comments = "";
	db_set_active("ecogene");
	
	$keio_array = array();
	$aska_array = array();
	
	$is_keio = check_keio($eg_id, $keio_array);
	$is_aska = check_aska($eg_id, $aska_array);
	
	
	
	if ($is_keio || $is_aska) 
	{
		$query = db_query(" 
		SELECT
			name
		FROM 
			t_gene
		WHERE 
			t_gene.eg_id = :eg_id
		", 
		array(':eg_id'=>$eg_id)
		);
	
		$name = $query->fetchField();
		
//		$content.= "<p style='padding-left: 2em'>Location of ASKA Clones <A HREF='http://www.ncbi.nlm.nih.gov/entrez/query.fcgi?cmd=Retrieve&db=PubMed&list_uids=16769691' target='_new'>(Kitagawa, 2005)</a> and
//		      KEIO mutant strains <A HREF='http://www.ncbi.nlm.nih.gov/entrez/query.fcgi?cmd=Retrieve&db=PubMed&list_uids=16738554' target='_new'>(Baba, 2006)</a> in microtiter plate
//		 	  collections (<A HREF='http://ecoli.naist.jp/gb6/Resource_download.html' target='_new'>Genobase Resource</a>):<br /><br />";
		
		$query = db_query("SELECT accession_id FROM t_biodatabase_link where eg_id = '$eg_id' and biodatabase_id = 10 ");
		$link = $query->fetchObject()->accession_id;
		$content.= "<p style='padding-left: 2em'>Location of ASKA Clones <A HREF='http://www.ncbi.nlm.nih.gov/entrez/query.fcgi?cmd=Retrieve&db=PubMed&list_uids=16769691' target='_new'>(Kitagawa, 2005)</a> and
		      KEIO mutant strains <A HREF='http://www.ncbi.nlm.nih.gov/entrez/query.fcgi?cmd=Retrieve&db=PubMed&list_uids=16738554' target='_new'>(Baba, 2006)</a> in microtiter plate
		 	  collections (<A HREF='http://ecoli.naist.jp/GB6/info.jsp?id=$link' target='_new'>Genobase</a>):<br /><br />";
	
		
	
		
		
		
	}elseif(strncmp($eg_id,'EG3',3)==0){
		$query = db_query(" 
		SELECT
			name
		FROM 
			t_gene
		WHERE 
			t_gene.eg_id = :eg_id
		", 
		array(':eg_id'=>$eg_id)
		);
	
		$name = $query->fetchField();
		$content .= "$name is an RNA gene and is not included in the Keio mutant collection or the ASKA clone library.";
		db_set_active("default");
		return $content;
	}
	else 
	{
		db_set_active("default");
		return $content;
	}
		
	if ($is_keio) 
	{
		
		$content.= "Deletion strain #1 for ".$name.": Plate ".$keio_array['koplate']." Well ".$keio_array['korow'].$keio_array['kocolumn']."<br />";  
		$content.= "Deletion strain #2 for ".$name.": Plate ".$keio_array['koplate_2']." Well ".$keio_array['korow_2'].$keio_array['kocolumn_2']."<br /><br />"; 
		if ($keio_array['kocomment'] != "") 
		{
			$content_comments .=  "KEIO comments: ".$keio_array['kocomment']."<br />";
		}
	}
	if ($is_aska) 
	{
		$content.= "ASKA +GFP clone for ".$name.": Plate ".$aska_array['askaplate']." Well ".$aska_array['askarow'].$aska_array['askacolumn']."<br />";
		$content.= "ASKA -GFP clone for ".$name.": Plate ".$aska_array['askaplate']." Well ".$aska_array['askarow'].$aska_array['askacolumn']."<br /><br />";
		$content.= "ASKA Clone primers: <br /><br />C-Terminus: ".$aska_array['c_terminal_primer']."<br />N-Terminus: ".$aska_array['n_terminal_primer']."<br /><br />";
		
		if ($aska_array['askacomment'] != "") {	
					
			$content_comments .= "ASKA comments: ".$aska_array['askacomment'].'<br />';
		}
				
	}
	$other_txt = ""; 
	if ($is_keio && !$is_aska) 
	{
		
		
	}elseif (!$is_keio && $is_aska)
	{
		$other_txt = 'No KEIO for '.$name;
	}			
		
	$keio_image_content="";
// For now, don't show the keio picture
global $user;
	if ($user->uid != 0 && in_array('administrator', $user->roles)) {
		$keio_image_content = keio_image($eg_id,$name);
	}
	db_set_active("default");
	return $content.$content_comments.$other_txt.$keio_image_content;
}
function keio_image($eg_id,$name)
{
//	$eg_id = 'EG11755';
	db_set_active("ecogene");
	
	$keio_image_content="";
	
	$query = db_query("
	SELECT picture, score, x, y,height, width  
		FROM t_keio_gene_score 
	WHERE eg_id=:eg_id",
	array(':eg_id'=>$eg_id)
	);
	
	if($row = $query->fetchAssoc())	
	{
		global $base_url;
//		var_dump($row);
		$image_name= $base_url."/".drupal_get_path('module', 'ecogene_gene')."/keio_image/".$row['picture'].".png";
		$score = strtoupper ($row['score']);
		switch ($score) {
			case 'WT':
			$score = "PCR of the <i>flhDC</i> promoter region from the Keio <i>". $name ."</i> mutant #1 amplifies a <b><i>WT band</i></b> indicating no IS element is present.";
			break;
			case 'IS1':
			$score = "PCR of the <i>flhDC</i> promoter region from the Keio <i>". $name ."</i> mutant #1 amplifies a band larger than WT, indicating an <b><i>IS1 element</i></b> is present causing hypermotility.";
			break;
			case 'IS5':
			$score = "PCR of the <i>flhDC</i> promoter region from the Keio <i>". $name ."</i> mutant #1 amplifies a band larger than WT, indicating an <b><i>IS5 element</i></b> is present causing hypermotility.";
			break;
			case 'MIX1':
			$score = "PCR of the <i>flhDC</i> promoter region from the Keio <i>". $name ."</i> mutant #1 amplifies both a <b><i>WT (wildtype) and IS1 band</i></b>, indicating a mixed population of both semi-motile and hypermotile cells.";
			break;
			case 'MIX5':
			$score = "PCR of the <i>flhDC</i> promoter region from the Keio <i>". $name ."</i> mutant #1 amplifies both a <b><i>WT (wildtype) and IS5 band</i></b>, indicating a mixed population of both semi-motile and hypermotile cells.";
			break;
			default:
			$score = "PCR of the <i>flhDC</i> promoter region from the Keio <i>". $name ."</i> mutant #1 amplifies a <b><i>WT band</i></b> indicating no IS element is present.";
			break;
		};
		
		$keio_image_content .= 	"<div></p><span class='keio_image'>
		<p class='keio_image_gene_name'>$name</p>
		<p class='keio_image_gene_border'></p>
		
  		</span><p>$score</p></div>";
		
		$width= round($row['width']*10);
		$height=141;//round($row['height']*2);
		
		$y = $row['y']-70; //$row['height'];
		$x = max(($row['x'] - round($row['width']*5)),1);
		
		$yy = -20;
		$xx = $row['x'] - $x;
		$hh = 136;// $height-3;
		
		drupal_add_css(
      	'	p.keio_image_gene_border {position:relative;
  						display:block; 
  						top:'.$yy.'px; left:'.$xx.'px; width:'.$row['width'].'px; height:'.$hh.'px;
  						border:1px solid red;
  						margin:0px;
  						
			}
			p.keio_image_gene_name {position:relative;
  						display:block; 
  						top:'.$yy.'px; left:'.$xx.'px; 
  						color:red;
  						margin:0px;
			}
      		.keio_image {
      				display:block;
					width: '.$width.'px;
					height: '.$height.'px;
					background-repeat: no-repeat;
					background-position: -'.$x.'px -'.$y.'px;
					background-image: url('.$image_name.');
			}
		',
      array(
        'type' => 'inline',
      )
    );
    
	}	
	
	db_set_active("default");
	return $keio_image_content;
}
