<?php

function get_intergene_info($row_left_gene, $row_right_gene)
{
	global $GENOME_LENGTH;
	$intgene_name = $row_left_gene['name'].'_'.$row_right_gene['name'];
	
	if(($row_left_gene[orientation]=='Clockwise') && ($row_right_gene[orientation]=='Clockwise'))
	{
		$intgene_orientation = 'Codirectional+';
	}
	elseif (($row_left_gene[orientation]=='Clockwise') && ($row_right_gene[orientation]=='Counterclockwise'))
	{
		$intgene_orientation = 'Convergent';
	}
	elseif (($row_left_gene[orientation]=='Counterclockwise' )&& ($row_right_gene[orientation]=='Clockwise'))
	{
		$intgene_orientation = 'Divergent';
	}
	else
	{
		$intgene_orientation = 'Codirectional-';
	}
	
	if($row_right_gene[left_end]>$row_left_gene[right_end]+1){
		$overLap = 0;
		$left_end = $row_left_gene[right_end] + 1;
		$right_end = $row_right_gene[left_end] - 1;
		$intgene_length = $right_end - $left_end + 1;
		$intgene_length = $intgene_length.' bp';
		$centisome = $left_end / $GENOME_LENGTH * 100;

	}
	elseif($row_right_gene[left_end]<$row_left_gene[right_end]+1){
		$overLap = 1;
		$left_end = $row_right_gene[left_end];
		$right_end = $row_left_gene[right_end];
		$intgene_length = $right_end - $left_end + 1;
		if($intgene_length>$GENOME_LENGTH/2)//beginning of the genome
		{
			$left_end = $row_left_gene[right_end]+1;
			$right_end = $row_right_gene[left_end]-1;
			$intgene_length = $right_end - $left_end + 1 + $GENOME_LENGTH;
			$intgene_length = $intgene_length.' bp';
		}else 
		{
			$intgene_length = $intgene_length.' bp overlap';
		}	
		
		$centisome = $left_end / $GENOME_LENGTH * 100;

	}else {
		$intgene_length = '0 bp';
		$left_end = 'Null';
		$right_end = 'Null';
		$centisome = $row_right_gene[left_end] / $GENOME_LENGTH * 100;
		
	}
	$intergene_info['name'] = $intgene_name;
	$intergene_info['length'] = $intgene_length;
	$intergene_info['ori']= $intgene_orientation;
	$intergene_info['left_end'] = $left_end;
	$intergene_info['right_end'] = $right_end;
	$intergene_info['cent'] = $centisome;
	return $intergene_info;
}

global $GENOME_LENGTH;

	$row_left_gene = $curr_gene;		
	$row_right_gene = $next_gene;
	$intgene_name = $row_left_gene['name'].'_'.$row_right_gene['name'];
	if(($row_left_gene[orientation]=='Clockwise') && ($row_right_gene[orientation]=='Clockwise'))
	{
		$intgene_orientation = 'Codirectional+';
	}
	elseif (($row_left_gene[orientation]=='Clockwise') && ($row_right_gene[orientation]=='Counterclockwise'))
	{
		$intgene_orientation = 'Convergent';
	}
	elseif (($row_left_gene[orientation]=='Counterclockwise' )&& ($row_right_gene[orientation]=='Clockwise'))
	{
		$intgene_orientation = 'Divergent';
	}
	else
	{
		$intgene_orientation = 'Codirectional-';
	}
	
	if($row_right_gene[left_end]>$row_left_gene[right_end]+1){
		$overLap = 0;
		$left_end = $row_left_gene[right_end] + 1;
		$right_end = $row_right_gene[left_end] - 1;
		$intgene_length = $right_end - $left_end + 1;
		$intgene_length = $intgene_length.' bp';
		$centisome = $left_end / $GENOME_LENGTH * 100;

	}
	elseif($row_right_gene[left_end]<$row_left_gene[right_end]+1){
		$overLap = 1;
		$left_end = $row_right_gene[left_end];
		$right_end = $row_left_gene[right_end];
		$intgene_length = $right_end - $left_end + 1;
		if($intgene_length>$GENOME_LENGTH/2)//beginning of the genome
		{
			$left_end = $row_left_gene[right_end]+1;
			$right_end = $row_right_gene[left_end]-1;
			$intgene_length = $right_end - $left_end + 1 + $GENOME_LENGTH;
			$intgene_length = $intgene_length.' bp';
		}else 
		{
			$intgene_length = $intgene_length.' bp overlap';
		}	
		
		$centisome = $left_end / $GENOME_LENGTH * 100;

	}else {
		$intgene_length = '0 bp';
		$left_end = 'Null';
		$right_end = 'Null';
		$centisome = $row_right_gene[left_end] / $GENOME_LENGTH * 100;
		
	}

	

?>





<script type="text/javascript">

/***********************************************
* AnyLink Drop Down Menu- ? Dynamic Drive (www.dynamicdrive.com)
* This notice MUST stay intact for legal use
* Visit http://www.dynamicdrive.com/ for full source code
***********************************************/

//Contents for menu 2
var menu3 = new Array()
<?php 

//echo "menu2[0]= '<a>InterGene Info</a>'\n";
echo "menu3[1]= '<a>Name:    "."   $intgene_name </a>'\n";
echo "menu3[2]= '<a>Length:  "."   $intgene_length</a>'\n";
echo "menu3[3]= '<a>Orientation: "."$intgene_orientation</a>'\n";
echo "menu3[4]= '<a>Left_end:    "."$left_end</a>'\n";
echo "menu3[5]= '<a>Right_end:   "."$right_end'\n";
echo "menu3[6]= '<a>Centisome:   ".sprintf("%.2f",$centisome)."</a>'\n";


 ?>
//Contents for menu 2, and so on



</script>




