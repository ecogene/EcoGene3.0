
<?PHP



include("dblink.php");

$link = dblink();
mysql_select_db("ecogene") or die("Could not select database");

$colnum = 2;

global $numTopic;

$numTopic = 0;

global $strTopic;

$strTopic = '';

global $arrId;

$arrId = array();

global $topic_has_gene;

$topic_has_gene = array();

function sub_topic($query, $link, $id) {
	
	global $strTopic, $arrId, $numTopic;
	global $topic_has_gene;
	
	$rst = mysql_query($query, $link) or die(mysql_error());
	
	if (mysql_num_rows($rst)>0) {
		
		$id_tmp = $id;
		
		echo "<ul>";
		
		while ($row=mysql_fetch_array($rst, MYSQL_ASSOC)) {
			
			$topic_id = $row['topic_id'];
			
			$topic_name = $row['topic_name'];
			
			$name = 'topic'.$topic_id;
			
			$id = $id_tmp."_topic".$topic_id;
			
			$strTopic = $strTopic.";".$topic_id;
			
			if ($row['permission_access']==0) {
				
				array_push($arrId, $id);
			}
			//array_push($arrId,$id);
			
			$query = "select * from t_topic_topic_link tlink, t_topic tt where tlink.linked_topic_id='$topic_id' and tlink.topic_id=tt.topic_id order by tt.topic_order";
			
			$rst_chknum = mysql_query($query, $link) or die(mysql_error());
			
			$idchk = $id."chk";
			
			if (mysql_num_rows($rst_chknum)>0) {

				echo "<li style=\"list-style: none\" align=left id='$id'><INPUT TYPE=CHECKBOX NAME='$name' id='$idchk' OnClick='CheckPermission(\"$name\",\"$idchk\")'";
//				if(!in_array($topic_id, $topic_has_gene))
				if(!array_key_exists($topic_id, $topic_has_gene)) 
				{

					echo " disabled>";

				}
				else
				{
					echo ">";
				}
				echo "<a href=\"javascript:toggle('$id')\" href title=\"Click to hide/review sub-topics.\" style='CURSOR: hand'><IMG src=\"sites/all/images/button_list_open.gif\" border=\"0\" height=\"13\" width=\"15\"></a><A HREF='?q=topic/".$topic_id."'>$topic_name</a>";
				if(array_key_exists($topic_id, $topic_has_gene)) 
				{
				echo " <a href=\"?q=ecosearch/gene/search&search_topic=$topic_id\" style='CURSOR: hand'><font color='#0000FF'>".$topic_has_gene["$topic_id"]."</font></a>";
				}
				
				echo "</li>\n";
			
			} else {
				
				echo "<li style=\"list-style: none\" align=left id =\"$id\"><INPUT TYPE=CHECKBOX NAME='$name' id='$idchk' OnClick='CheckPermission(\"$name\",\"$idchk\")'";
//				if(!in_array($topic_id, $topic_has_gene))
				if(!array_key_exists($topic_id, $topic_has_gene)) 
				{

					echo " disabled>";

				}
				else
				{
					echo ">";
				}
				
				echo "<A HREF='?q=topic/".$topic_id."'>".$topic_name."</a>";
				if(array_key_exists($topic_id, $topic_has_gene)) 
				{
				echo " <a href=\"?q=ecosearch/gene/search&search_topic=$topic_id\" style='CURSOR: hand'><font color='#0000FF'>".$topic_has_gene["$topic_id"]."</font></a>";
				}
				
				echo "</li>\n";
			}
			
			$numTopic ++;
			
			sub_topic($query, $link, $id);
			
		}
		
		echo "</ul>";
		
	} else {
		
		return;
	}
}





$query = "select count(*) as num ,  topic_id from t_topic_gene_link group by topic_id";
$rst = mysql_query($query, $link) or die(mysql_error());
while ($row_topic = mysql_fetch_array($rst, MYSQL_ASSOC)) {
//	$topic_has_gene[] = $row_topic['topic_id'];
$topic_has_gene[$row_topic['topic_id']] = $row_topic['num'];
}

mysql_free_result($rst);

$query = "select distinct topic_type from t_topic";
$rst = mysql_query($query, $link) or die(mysql_error());

echo "<center><font class=\"title\"><b>Gene Set Overlap Query</b></font>&nbsp;&nbsp;".
"<br><br><A href='?q=node/9'>Help</A>".
"
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<A id=\"review\" alt = \"Show All\" href='javascript:review_topics()' >Show All</A>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<A href='?q=ecosearch/topic'>Search Topic</A>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<A href='?q=ecosearch/topic/search'>Show All Topic</A>
";
//echo "<input style='height: 25px; width: 100px' type=submit name='TopicByDescription' value='Search Topic' onclick=aform_submit('')>";
echo "</center>";
//echo "<hr>";
echo "<FORM id=bform action='TopicQuery.php' method='POST'>\n";
echo "<table border=0 frame='borders' align='center'>\n";
$query = "select t_topic_topic_link.*, t_topic.permission_access from t_topic_topic_link left join t_topic on t_topic_topic_link.linked_topic_id=t_topic.topic_id order by t_topic_topic_link.linked_topic_id";
while ($row_topic = mysql_fetch_array($rst, MYSQL_ASSOC)) {

	$topic_type = $row_topic['topic_type'];
	echo "<tr><td><b>".$topic_type.":</b></td></tr>\n";
	$count = 0;

	$query_root = "select distinct".
	" t1.topic_id,".
	" t1.topic_name".
	" from".
	" t_topic t1,".
	" t_topic_topic_link t2".
	" where".
	" t1.permission_access=1".
	" and".
	" t1.topic_type='$topic_type'".
//	" and".
//	" t1.topic_id".
//	" in".
//	" (select distinct topic_id from t_topic_gene_link)".  //this topic is about gene
	" and".
	" (t1.topic_id".
	" in".
	" (select distinct".
	" tlink.linked_topic_id".
	" from".
	" t_topic_topic_link tlink, t_topic tt".
	" where".
	" tt.topic_id=tlink.linked_topic_id and tt.permission_access=1)".
	" and".
	" t1.topic_id".
	" not in".
	" (select distinct".
	" tlink.topic_id".
	" from".
	" t_topic_topic_link tlink, t_topic tt".
	" where".
	" tt.topic_id=tlink.linked_topic_id and tt.permission_access=1))".
	" order by t1.topic_order";

	//echo $query;
	$result = mysql_query($query_root, $link) or die(mysql_error());
	if (mysql_num_rows($result)>0) {
		while ($row=mysql_fetch_array($result,MYSQL_ASSOC)) {
			if ($count%$colnum ==0) {
				echo "<tr valign='top'>";
			}
			$topic_id = $row['topic_id'];
			$topic_name = $row['topic_name'];
			$name = 'topic'.$topic_id;
			$id = $name;
			$idchk = $id."chk";
			
			echo "<TD valign=top>".
			"<INPUT TYPE=CHECKBOX NAME='$name' id='$idchk' OnClick='CheckPermission(\"$name\",\"$idchk\")'";
			
//			if(!in_array($topic_id, $topic_has_gene))
			if(!array_key_exists($topic_id, $topic_has_gene)) 
			{
				
				echo " disabled>";
				
				
			}
			else 
			{
				echo ">";
			}
			
			echo "<a href=\"javascript:toggle('$id')\" href title=\"Click to hide/review sub-topics.\" style='CURSOR: hand'><IMG src=\"sites/all/images/button_list_open.gif\" border=\"0\" height=\"13\" width=\"15\"></a>".
			"<A HREF='?q=topic/".$topic_id."'>".$topic_name."</a>\n";
			if(array_key_exists($topic_id, $topic_has_gene)) 
			{
				echo " <a href=\"?q=ecosearch/gene/search&search_topic=$topic_id\" style='CURSOR: hand'><font color='#0000FF'>".$topic_has_gene[$topic_id]."</font></a>";
			}
			$query = "select * from t_topic_topic_link tlink, t_topic tt where tlink.linked_topic_id='$topic_id' and tlink.topic_id=tt.topic_id order by tt.topic_order";

			
			sub_topic($query, $link, $id); //recursive function
			echo "</TD>";
			if ($count%$colnum ==$colnum-1) {
				echo "</tr>\n";
			}
			$count ++;
			$strTopic = $strTopic.";".$topic_id;
			$numTopic ++;


		}
	}

	$query_single = "select distinct".
	" t1.topic_id,".
	" t1.topic_name".
	" from".
	" t_topic t1, t_topic_topic_link t2".
	" where".
	" t1.permission_access=1".
	" and".
	" t1.topic_type='$topic_type'".
	" and".
	" t1.topic_id in (select distinct topic_id from t_topic_gene_link)".
	" and".
	" (t1.topic_id not in (select distinct tlink.linked_topic_id from t_topic_topic_link tlink, t_topic tt where tt.topic_id=tlink.linked_topic_id and tt.permission_access=1)".
	" and".
	" t1.topic_id not in (select distinct tt.topic_id from t_topic_topic_link tlink, t_topic tt where tt.topic_id=tlink.topic_id and tt.permission_access=1))".
	" order by t1.topic_order";

	$result = mysql_query($query_single, $link) or die(mysql_error());
	if (mysql_num_rows($result)>0) {
		while ($row=mysql_fetch_array($result,MYSQL_ASSOC)) {

			if ($count%$colnum ==0) {
				echo "<tr>";
			}
			$topic_id = $row['topic_id'];
			$topic_name = $row['topic_name'];
			$name = 'topic'.$topic_id;
			$id = $name;
			$idchk = $id."chk";
			echo "<TD valign=top><INPUT TYPE=CHECKBOX NAME='$name' id='$idchk' OnClick='CheckPermission(\"$name\",\"$idchk\")'";
			
			if(!array_key_exists($topic_id, $topic_has_gene)) 
			{
				
				echo " disabled>";
				
				
			}
			else 
			{
				echo ">";
			}
			
			echo "<A HREF='?q=topic/".$topic_id."'>$topic_name</A>\n";
			if(array_key_exists($topic_id, $topic_has_gene)) 
			{
				echo " <a href=\"?q=ecosearch/gene/search&search_topic=$topic_id\" style='CURSOR: hand'><font color='#0000FF'>".$topic_has_gene[$topic_id]."</font></a>";
			}
			echo "</TD>";
			if ($count%$colnum ==$colnum-1) {
				echo "</tr>\n";
			}
			$strTopic = $strTopic.";".$topic_id;
			$count ++;
			$numTopic ++;
		}
	}


	for ($i = 0; $i < $colnum - $count%$colnum; $i ++) {
		echo "<td></td>";
	}
	echo "</tr>\n";
}

echo "<input type='hidden' name='func' value='GeneByTopic'>\n";
echo "<input type='hidden' name='strTopic' value='$strTopic'>\n";
echo "<input type='hidden' name='numTopic' value='$numTopic'>\n";
echo  "<tr align='center'><td colspan='$colnum'><input style='height: 25px; width: 100px' type='submit' name='AndQuery' value='In All Chosen'>\n";
echo  "<input style='height: 25px; width: 100px' type='submit' name='OrQuery' value='In Any Chosen'>\n";
//echo  "<input style='height: 25px; width: 100px' type='submit' name='OnlyQuery' value='Only In Chosen'>\n";
echo  "<input style='height: 25px; width: 100px' type='submit' name='NotQuery' value='Not In Chosen'></td></tr>\n";
echo "</table>";
echo "</form>";


echo "<br>";
mysql_close($link);


?>

<SCRIPT LANGUAGE="JavaScript">

<?php

$Element_Num=0;

echo "var id_name = new Array() \n";

foreach ($arrId as $element_id) {

	$Element_List = $element_id;

	echo "document.getElementById('$element_id').style.display=\"none\";\n";

	echo "id_name['$Element_Num']='$element_id'\n";

	$Element_Num++;

}

?>

function popUp_helpWin() {

	window.open ('ShowInfo.php?info_name=Operator Info', 'popwindow', 'height=300, width=500, top=200,  left=250 toolbar=no, menubar=1, scrollbars=1, resizable=1, location=no, status=no');

}


//function check_subset(id, level) {
//
//	var listLength = id_name.length;
//
//	var id_len = id.length;
//
//	id=id.substring(0,id_len-3);
//  
//	for (i=0; i<listLength; i++) {
//
//		var list = id_name[i];
//
//		if(list.match(id+"_")!=null)	{
//
//			document.getElementById(list+"chk").checked = document.getElementById(id+"chk").checked;
//
//		}
//	}
//
//}

//function toggle(type_name) {
//
//	var listLength = id_name.length;
//	
//	var status_nextLevel="block";
//	
//	var len = type_name.length + 12; //10?  if the topic_id is bigger than 1000, it'll will be a prob!
//
//	for (i=0; i<listLength; i++)
//	{
//
//		var list = id_name[i];		
//
//		if(list.match(type_name+"_")!=null && list.length<len)	{
//
//			if (document.getElementById(list).style.display=='none') {
//
//				status_nextLevel = "none";
//				
//				break;
//
//			} /*else {
//				
//				status_nextLevel = "block";
//				
//				break;
//
//			}*/
//
//		}
//	}
//
//	if (status_nextLevel == "block") {
//		
//		for (i=0; i<listLength; i++) {
//
//			var list = id_name[i];
//
//			if(list.match(type_name+"_")!=null)	{			
//				
//				if (document.getElementById(id_name[i]+"chk").checked == false) {
//					document.getElementById(id_name[i]).style.display = "none";
//					
//				}
//
////				document.getElementById(list).style.display='none';
////				
////				document.getElementById(list+"chk").checked = false;
//
//			}
//		}
//	} else {
//		
//		for (i=0; i<listLength; i++) {
//
//			var list = id_name[i];
//
//			if(list.match(type_name+"_")!=null && list.length<len)	{
//
//				document.getElementById(list).style.display='block';
//					
//			}
//		}
//
//	}
//
//}
function toggle(type_name) {

	var listLength = id_name.length;
	
	var status_nextLevel = "block";
	
	var len = type_name.length + 12; //10?  if the topic_id is bigger than 1000, it'll will be a prob!

	for (i=0; i<listLength; i++)
	{

		var list = id_name[i];		

		if(list.match(type_name+"_")!=null && list.length<len)	{

			if (document.getElementById(list).style.display=='none') {

				status_nextLevel = "none";
				
				break;

			} /*else {
				
				status_nextLevel = "block";
				
				break;

			}*/

		}
	}

	if (status_nextLevel == "block") {
		
		for (i=0; i<listLength; i++) {

			var list = id_name[i];

			if(list.match(type_name+"_")!=null && document.getElementById(list+"chk").checked == false)	{		
				
				//if (document.getElementById(id_name[i]+"chk").checked == false) {
					//document.write(document.getElementById(id_name[i]+"chk").checked+":"+list);
					document.getElementById(id_name[i]).style.display = "none";
				//}
				
					
				//document.getElementById(list).style.display='none';
				
				//document.getElementById(list+"chk").checked = false;

			}
		}
	} else {
		
		for (i=0; i<listLength; i++) {

			var list = id_name[i];

			if(list.match(type_name+"_")!=null && list.length<len)	{

				document.getElementById(list).style.display='block';
					
			}
		}

	}

}

function aform_submit(value)
{
	
	document.aform.ShowAllTopics.value=value;
	document.aform.submit();
} 

function review_topics(){ 
	
	if(document.getElementById) {
		
		var el = document.getElementById('review');
		
			if(el.alt == "Show All"){
				
				for (var i=0; i<id_name.length; i++){
					document.getElementById(id_name[i]).style.display = "block";
				}
				el.alt = "Hide";
				el.innerHTML ="Hide Unchecked";
				//el.src="getImage.php?text=Hide Unchecked";
			}else{
				for (var i=0; i<id_name.length; i++){
					if (document.getElementById(id_name[i]+"chk").checked == false) 
						document.getElementById(id_name[i]).style.display = "none";
					//document.getElementById(id_name[i]).style.display = "none";
				}
				el.alt = "Show All";
				el.innerHTML ="Show All";
				//el.src="getImage.php?text=Show All";
			}
			//document.write(el.alt);
		}
}

//function CheckPermission(name,idchk) {
//	var x=document.getElementsByName(name);
//	
//	var CurrentCheck=document.getElementById(idchk).checked;
//	
//	for (var i=0; i<x.length; i++) {
//		document.getElementsByName(name)[i].checked = CurrentCheck;
//	}
//}

</script>

</BODY>
</HTML>