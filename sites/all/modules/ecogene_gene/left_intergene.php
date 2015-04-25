<?php


global $GENOME_LENGTH;
		
		
	$row_left_gene = $prev_gene;		
	$row_right_gene = $curr_gene;
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



