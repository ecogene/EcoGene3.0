<?
	define ("MAX_LENGTH", 61);
	function format_description($str) {

		$pos = strpos($str,";");
		if ($pos == false) {
			$description = substr($str,0,MAX_LENGTH);
		} else 
		{

			if ( $pos > MAX_LENGTH )
			{
			   $description = substr($str,0,MAX_LENGTH);
			}
			else
			{
			   $description = substr($str,0,$pos );
			}
		}
		return $description;
	}
	
	function explode_quote($ADelim, $AString, $AEncap = '"') {
       $retval = array();

       //if we have an empty string, don't even bother processing
       if (empty($AString))
           return $retval;
  
       //calculate source length and initialize some variables
       $srcLength = strlen($AString);
       $insideEncap = false;
       $foundEncap = false;
       //echo "length".$srcLength."<BR>";
      
       //0..X string indexing in PHP, we'll be reading +1
       $lastPos = -1;
      
       for ($x=0;$x<$srcLength;$x++) {
       	
       	   //echo $AString[$x];
       	   
       	   if ($AString[$x]== $AEncap) {
       	   		$insideEncap = !$insideEncap;
       	   		$foundEncap = true;
       	   	//echo "Found".$insideEncap;
       	   }
       	   
           
           if (!$insideEncap && $AString[$x]== $ADelim) {
           	   if ($foundEncap) {
           	   		//echo $lastPos,"   ",$x;
              		$retval[] = substr($AString, $lastPos+3, $x-$lastPos-5);
              		$lastPos    = $x;
               		$foundEncap = false;
           	  
           	    } else {
           	   		$retval[] = substr($AString, $lastPos+1, $x-$lastPos-1);	
               		$lastPos    = $x;
           	   }
           	  
           }
           
          //echo "<BR>";
       }
       //print_r($retval);
       
       if ($foundEncap) {
       	   $retval[] = substr($AString, $lastPos+3,$x-$lastPos-5); 
       } elseif ($lastPos!=$srcLength) {
           $retval[] = substr($AString, $lastPos+1, $srcLength);
       }
       return $retval;
   }
	
	function description_array($str) {
		//echo $str."<BR>";
		//print_r (explode_quote(" ",$str));
		//if (substr($str,0,1) == "\\") {
		//	$str = substr($str,2,strlen($str)-4);
		//	$descriptionArray[0]=$str;
			
		//}
		//else {
		$descriptionArray = explode_quote(" ",$str); 
		//}
		//print_r ($descriptionArray);
		return $descriptionArray;
	}
	
	
	// This function must be called when storing a string with quotes "". Otherwise it will put a "/" before the quotes 
	function remove_slash($str) {
		$strLen = strlen($str);
		$strout = "";
		for ($x=0;$x<$strLen;$x++) {		
			if ($str[$x] != chr(92)) {
				//echo $str[$x]."<BR>";
				$strout = $strout.$str[$x];
			}
		}
		//echo($strout);
		return $strout;	
	}
		
?>