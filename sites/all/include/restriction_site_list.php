<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" >
<head><title>Select restriction sites</title>
<script type="text/javascript" language="JavaScript1.1">
<!--Hide from older browsers
function R(evt, des){
	var strResult = document.getElementById(des).value;
	var str;
	var obj = strResult.split(",");
	str = document.getElementById(evt).innerHTML;
	str = str.substr(0,str.indexOf(','));
	
	var flag=1;
for (i=0; i<obj.length; i++)
{
	
	var id = obj[i];
	if(id.search(str)>-1)
	{
		flag=0;
		break;
	}
	
}
if(flag==1)
{
	if(strResult!='')
	{
		str = ', '+str;
	}
	strResult = strResult+str;
	if(strResult.split(",").length<8)
		document.getElementById(des).value = strResult;
}

}

function submit_to_opener(){

//	var o = window.opener.document.getElementById('sites');
	var url = window.opener.location.href;
	var pos_1 = url.indexOf('&sites=');

	while(pos_1!=-1)
	{
		var pos_2 = url.indexOf('&', pos_1+6);
		if(pos_2==-1)
		{
			url = url.substr(0,pos_1) ;
		}else
		{
			url = url.substr(0,pos_1) + url.substr(pos_2) ;
		}
		pos_1 = url.indexOf('&sites=');
	}
	pos_q = url.indexOf('?');
	if(pos_q==-1) url = url + '?&sites=' + document.getElementById('sites').value;
	else url = url + '&sites=' + document.getElementById('sites').value;
	window.opener.location.replace((url));
//	window.opener.location.reload();
	
	window.close();
}

function NewList(condition){

	window.location="restriction_site_list.php?palindromic="+condition
}
// -->
</script>
<style>
A:hover	{	
		text-decoration:	underline;
	}
A:link	{	
	text-decoration:	none;
	}	
A:visited	{	
	text-decoration:	none;
	}	
body {
	font-family: Verdana, Arial, Tahoma, Helvetica, sans-serif;
	font-size: 10px;
	margin-left: 10px;
	margin-top: 10px;
	margin-right: 10px;
	margin-bottom: 10px;
}
.pre {
	font-family: Courier New, courier, monospace;
	font-size: 12px;
	letter-spacing: 4px;
}
.title {
	font-family: Verdana, Arial, Tahoma, Helvetica, sans-serif;
	font-size: 12px;
	font-weight: bold;
}
</style>
</head>
<script>
function  lookUpWin()

{

 window.open ('getisoschi.php', 'isoschnamecheckup', 'height=130, width=300, top=100,  left=100 toolbar=no, menubar=1, scrollbars=1, resizable=1, location=no, status=no');

}
</script>
<body>

<a name="TOP"></a>

<?PHP
echo "<p>This full list of restriction enzymes and their DNA recognition sequences is obtained from REBASE <a href='http://www.ncbi.nlm.nih.gov/pubmed/19846593'>(Roberts et al., 2010)</a>. 
	Clicking on the restriction enzyme name next to the restriction maps links to additional information at <a href='http://rebase.neb.com'>REBASE</a>.</p>";
if (!array_key_exists("palindromic", $_GET)) 
{	
	echo "<div class='title'>Restriction Sites Selection </div> (7 sites maximum)<br/>";
}
elseif($_GET['palindromic']==1)
{
	echo "<div class='title'>Restriction Sites Selection </div>(7 sites maximum)<br><br><div class='title'> Palindromic | <a href=JavaScript:NewList('0');>All Sites </a></div><br/>";
}

elseif($_GET['palindromic']==0)
{
	echo "<div class='title'>Restriction Sites Selection </div>(7 sites maximum)<br><br><div class='title'> <a href=JavaScript:NewList('1');>Palindromic</a> | All Sites </div><br/>";
}
?>

<div class="pre"><a href="#A">A</a>|<a href="#B">B</a>|<a href="#C">C</a>|<a href="#D">D</a>|<a href="#E">E</a>|<a href="#F">F</a>|<a href="#G">G</a>|<a href="#H">H</a>|<a href="#I">I</a>|<a href="#J">J</a>|<a href="#K">K</a>|<a href="#L">L</a>|<a href="#M">M</a><br/>

<a href="#N">N</a>|<a href="#O">O</a>|<a href="#P">P</a>|<a href="#Q">Q</a>|<a href="#R">R</a>|<a href="#S">S</a>|<a href="#T">T</a>|<a href="#U">U</a>|<a href="#V">V</a>|<a href="#W">W</a>|<a href="#X">X</a>|<a href="#Y">Y</a>|<a href="#Z">Z</a></div>
<br>

<div class="title">Selected Sites (Click name to select site)</div><br>
<form id="sites_slection" action="" method="post">
<input type='text' name="sites" size="50" id='sites'><br><br>
<INPUT type="submit" onclick="javascript:submit_to_opener()" value="Submit"> <INPUT type="reset" value="Clear Input">
<input type="button" name="Click here!" value="Isoschizomer Lookup" onClick="lookUpWin()">


</FORM>




<?PHP
		$letter = array("A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z");
		
		include("dblink.php");
		$link = dblink();
		mysql_select_db("mapsearch_db") or die("Could not select database");  
		  
		if($_GET['palindromic']==1 || !array_key_exists('palindromic',$_GET)) 	
		{
			$query = "Select name, seq, is_default, is_symmetrical  FROM t_enzyme WHERE is_symmetrical=1 ORDER by name ASC";
			$query_2 = "Select t_isosch.* from t_isosch,t_enzyme where t_enzyme.name=t_isosch.enzyme and t_enzyme.is_symmetrical=1";
		}
		else
		{
			$query = "Select name, seq, is_default, is_symmetrical  FROM t_enzyme ORDER by name ASC";
			$query_2 = "Select t_isosch.* from t_isosch,t_enzyme where t_enzyme.name=t_isosch.enzyme";
		}
		
		$result = mysql_query($query) or die("Query failed : " . mysql_error());
		$number = mysql_numrows($result);
		
		$letter_count = 0;
		$i = 0;
		$name = '';
		
		
		While ($i < $number) {

			
			$enzyme_name = mysql_result($result,$i,'name');
			$sequence = mysql_result($result,$i,'seq');
			$is_default = mysql_result($result,$i, 'is_default');		
			$is_symmetrical = mysql_result($result,$i, 'is_symmetrical');		
			while ($name<$enzyme_name[0]) {
				
				$name = $letter[$letter_count];
				$letter_count++;
				echo "<a name=$name></a><br><table width=\"100%\" cellpadding=0 cellspacing=0 border=0><tr><td><b>$name</b></td><td align=right><a href=\"#TOP\">Top</a></td></tr>
</table><br>";

			}
			if(($is_symmetrical)==0)
				echo "<a style='color: red' href=Javascript:R('$enzyme_name','sites'); id=$enzyme_name>$enzyme_name, $sequence</a><br/>";
			else
				echo "<a href=Javascript:R('$enzyme_name','sites'); id=$enzyme_name>$enzyme_name, $sequence</a><br/>";
			
			$i ++;
		}
		while ($letter_count < count($letter)) {				
				$name = $letter[$letter_count];
				$letter_count++;
				echo "<a name=$name></a><br><table width=\"100%\" cellpadding=0 cellspacing=0 border=0><tr><td><b>$name</b></td><td align=right><a href=\"#TOP\">Top</a></td></tr>
</table><br>";

		}
		mysql_close($link);


?>
</body>
</html>
