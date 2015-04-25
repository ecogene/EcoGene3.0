<?php

########################################################################
# EcoGene: The Database of Escherichia coli Sequence and Function
# ================================================================
#
# This program is NOT free software. 
# You can NOT redistribute it and/or modify it.
########################################################################

 function dblink() {
	 /* Connecting, selecting database */
 	$link = mysql_connect("", "", "")
  		or die("Could not connect : " . mysql_error());

	return $link;
   	
}
?>