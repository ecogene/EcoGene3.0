<?
function seq_format($eg_id, $us=0, $ds=0, $is_split=0, $split_left=0, $split_right=0)
{

if($is_split==0){
	

$query = "SELECT ".

    		 "	g.eg_id,g.name,g.description,ta.left_end,ta.right_end,ta.orientation,".

    		 "	CASE ta.orientation ".

		 "		WHEN 'Clockwise' THEN substring( s.sequence, ta.left_end - (".$us."), ta.right_end - ta.left_end + 1 + (".$us.")+(".$ds.") ) ".

		 "		WHEN 'Counterclockwise' THEN ".

		 "			replace(replace(replace(replace(replace(replace(reverse(substring( s.sequence, ta.left_end -(".$ds."),ta.right_end - ta.left_end + 1 +(".$us.")+(".$ds.") )),'A','Z'),'T','A'),'Z','T'),'G','Z'),'C','G'),'Z','C') ".

		 "	END as sequence ".

		 "FROM ".

		 "	t_sequence s, ".

		 "	t_gene g, ".

		 "	t_address ta ".

		 "WHERE ".

		 "	g.address_id = ta.address_id and ".

		 "	g.eg_id = '$eg_id' ";
}

else 
{
$query = "SELECT ".

    		 "	g.eg_id,g.name,g.description,ta.left_end,ta.right_end,ta.orientation,".

    		 "	CASE ta.orientation ".

		 "		WHEN 'Clockwise' THEN substring( s.sequence, $split_left - (".$us."), $split_right - $split_left + 1 + (".$us.")+(".$ds.") ) ".

		 "		WHEN 'Counterclockwise' THEN ".

		 "			replace(replace(replace(replace(replace(replace(reverse(substring( s.sequence, $split_left -(".$ds."), $split_right - $split_left + 1 +(".$us.")+(".$ds.") )),'A','Z'),'T','A'),'Z','T'),'G','Z'),'C','G'),'Z','C') ".

		 "	END as sequence ".

		 "FROM ".

		 "	t_sequence s, ".

		 "	t_gene g, ".

		 "	t_address ta ".

		 "WHERE ".

		 "	g.address_id = ta.address_id and ".

		 "	g.eg_id = '$eg_id' ";
// avoid using t_gene_address table, used t_address instead 
// Dian Fan 5/25/07
// pass test in Navicat, the query results are identical.
// the query is saved in Navicat as Name="sequent_format"
	
}
return $query;
//
}
