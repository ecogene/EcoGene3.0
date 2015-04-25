<html>
<head>
<title>Isoschizomer Name Lookup</title>
<meta http-equiv="Content-Type" content="text/html; charset=gb2312">
</head>
<body bgcolor="#d3e2ea">
<form name=aForm method=post action="<?$self?>">
<p align="center">
Isoschizomer Name:<BR>
<input type="text" name="isoname" value="<?$iso?>">
<input type="hidden" name="_submit_check" value="1"/><BR>
<input type=submit value="Search">
</p>
<?
if (array_key_exists('_submit_check', $_POST)) {

	$iso = $_POST['isoname'];
	if ($iso != ""){
		include("../dblink.php");
		$link = dblink();
		
		mysql_select_db("mapsearch_db") or die("Could not select database");   // MAKE SURE TO CHANGE THIS TO THE PROPER DATABASE
		$query = "Select * from t_enzyme where is_symmetrical=1 and name='$iso'";
		$result = mysql_query($query) or die("Query failed : " . mysql_error());
		$myrow = mysql_fetch_array($result);
		$enzyme = $myrow['name'];
		if ($enzyme != ""){
			Echo "Enzyme exists on list";
		}	
		else {
			$query = "Select isoschtable.enzyme from isoschtable,t_enzyme where t_enzyme.name=isoschtable.enzyme and t_enzyme.is_symmetrical=1 and isoschtable.isoschname='$iso'";
			$result = mysql_query($query) or die("Query failed : " . mysql_error());
			$myrow = mysql_fetch_array($result);
			$enzyme = $myrow['enzyme'];
			if ($enzyme != ""){
				Echo "Enzyme name: $enzyme";
			} else {
			Echo "Could not find Isoschizomer";	
			}
		}
		mysql_close($link);
	} else {
		Echo "Please insert Isoschizomer Name"; 
	}
} else {
	
//	Show_Form();
}

?>
</form>
</body>
</html>
