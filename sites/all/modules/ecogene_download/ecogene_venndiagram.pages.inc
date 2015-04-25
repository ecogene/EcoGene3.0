<?php 
// venn digram for 2 or 3 sets

function ecogene_create_venndiagram($ids, $color=TRUE)
{
//	global $fontfile, $imgFileVenn, $imgFileVennBW, $imgFileVenn2, $imgFileVenn2BW;
	 $fontfile = 'sites/all/include/verdana.ttf';
	// temporary file to store the venn diagram
	$destination = drupal_tempnam('temporary://', 'venn_').'.png';
 
    
//	if(count($ids)>=3){
//		if($color){ 
//			$srcimage = @ImageCreateFrompng("sites/all/images/venn3.png")
//      			or die("Cannot Initialize new GD image stream"); 	
//		}else{
//			$srcimage = @ImageCreateFrompng("sites/all/images/venn3_bw.png")
//      			or die("Cannot Initialize new GD image stream");
//		}
//      		
//      	// A and B
//	    $two_set[0] = array_intersect($ids[0], $ids[1]);
//	    // A and C
//	    $two_set[1] = array_intersect($ids[0], $ids[2]);
//	    // B and C
//	    $two_set[2] = array_intersect($ids[1], $ids[2]);
//	    
//	    // A, not B, not C
//	    $gene_sets[0] = array_diff(array_diff($ids[0], $ids[1]), $ids[2]);
//	    // B, not A, not C
//	    $gene_sets[1] = array_diff(array_diff($ids[1], $ids[0]), $ids[2]);
//	    // C, not B, not A
//	    $gene_sets[2] = array_diff(array_diff($ids[2], $ids[0]), $ids[1]);
//	    
//	    //A and B not C
//	    $gene_sets[3] = array_diff($two_set[0], $ids[2]);
//	     //B and C not A
//	    $gene_sets[4] = array_diff($two_set[2], $ids[0]);
//	     //A and C not B
//	    $gene_sets[5] = array_diff($two_set[1], $ids[1]);
//	    
//	    //A and B and C
//	    $gene_sets[6] = array_intersect($two_set[0], $two_set[1]);
//	    
//	    // Image map information of venn diagram of 3 sets
//	 	$map = array(
//			array("x1"=>46, "y1"=>50, "x2"=>98, "y2"=>131),
//			array("x1"=>250, "y1"=>50, "x2"=>300, "y2"=>131),
//			array("x1"=>120, "y1"=>250, "x2"=>240, "y2"=>290),
//			array("x1"=>150, "y1"=>58, "x2"=>211, "y2"=>95),
//			array("x1"=>219, "y1"=>183, "x2"=>270, "y2"=>214),
//			array("x1"=>81, "y1"=>180, "x2"=>136, "y2"=>220),
//			array("x1"=>142, "y1"=>134, "x2"=>205, "y2"=>178),			
//			
//		);
//		$title = array(
//		'(A) and (not B) and (not C) - '.count($gene_sets[0]). ' genes.',
//		'(B) and (not C) and (not A) - '.count($gene_sets[1]). ' genes.',
//		'(C) and (not A) and (not B) - '.count($gene_sets[2]). ' genes.',
//		'(A) and (B) and (not C) - '.count($gene_sets[3]). ' genes.',
//		'(B) and (C) and (not A) - '.count($gene_sets[4]). ' genes.',
//		'(C) and (A) and (not B) - '.count($gene_sets[5]). ' genes.',
//		'(A) and (B) and (C) - '.count($gene_sets[6]). ' genes.',
//		);		
//	    
//	}else{
//		if($color){ 
//			$srcimage = @ImageCreateFrompng("sites/all/images/venn2.png")
//      			or die("Cannot Initialize new GD image stream"); 	
//		}else{
//			$srcimage = @ImageCreateFrompng("sites/all/images/venn2_bw.png")
//      			or die("Cannot Initialize new GD image stream");
//		}
//  
//		// A not B
//		$gene_sets[0] = array_diff($ids[0], $ids[1]);
//	     //B not A
//	    $gene_sets[1] = array_diff($ids[1], $ids[0]);
//	     //A and B
//	    $gene_sets[2] = array_intersect($ids[0], $ids[1]);
//	    
//    	$map = array(
//			array("x1"=>46, "y1"=>50, "x2"=>98, "y2"=>220),
//			array("x1"=>250, "y1"=>50, "x2"=>300, "y2"=>220),
//			array("x1"=>150, "y1"=>58, "x2"=>211, "y2"=>210),
//		);		
//		$title = array(
//			'(A) and (not B) - '.count($gene_sets[0]). ' genes.',
//			'(B) and (not A) - '.count($gene_sets[1]). ' genes.',
//			'(A) and (B) - '.count($gene_sets[2]). ' genes.',
//			
//		);	
//    
//	}	
//	
//	$width = 346;
//	$height = 332;
//		
//	header ("Content-type: image/png");
//	$img=imagecreatetruecolor($width,$height);
//	imagecopyresampled($img,$srcimage,0,0,0,0, $width,$height,$width,$height);
//	$background = imagecolorallocate ($img,  0, 0, 0);	
//    	     						
//    $text_size =11;
//	
// 
//    for ($t=0;$t<count($map);$t++)
//    {
//    	$number = count($gene_sets[$t]);
//		imagettftext($img, $text_size, 0, $map[$t]['x1']/2+$map[$t]['x2']/2-$text_size, 
//						$map[$t]['y1']/2+ $map[$t]['y2']/2-$text_size/2,$background, $fontfile, $number);
//    }
//	imagettftext($img, $text_size+5, 0, 20, 30,$background, $fontfile, 'A');
//	imagettftext($img, $text_size+5, 0, $width-20, 30,$background, $fontfile, 'B');
//	imagettftext($img, $text_size+5, 0, $width/2+80, $height-10,$background, $fontfile, 'C');
//	
//	Imagepng($img,$destination);
//	ImageDestroy($img);		
	
	return array('destination'=> $destination,	'map'=> $map, 'title'=>$title, 'query_set'=>$gene_sets);
}

