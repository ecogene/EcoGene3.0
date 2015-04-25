<?php
include_once('ecoFunction.php');
global $GENOME_LENGTH;

class ecomapfont
{
	var $color, $orientation, $size,$fontfile;
	function ecomapfont($color, $orientation, $size,$fontfile) {

	 	$this->color = $color;

		$this->orientation = $orientation;		

		$this->size = $size;
		
		$this->fontfile = $fontfile;

    }
}
class ecomap
{
	
	var $img, $width, $height, $left_end, $right_end;
	var $minute_title, $length_title;
	var $fontfile;
	function ecomap($img,$width, $height, $left_end, $right_end)
     {             
                          
                $this->width = $width;
                $this->height = $height;
                $this->img = $img;
                $this->left_end = $left_end;
                $this->right_end = $right_end;   
    }	
    
     
//         
//    function draw_circle_map_title($diameter, $cx, $cy, $color, $type, $line_len, $font)
//	{
//			$img = $this->img;
//					
//			imagearc($this->img, $cx, $cy, $diameter, $diameter,  0, 360, $color);
//			
//			
//			if($type=='minute')
//			{
//				for($i=0; $i<count($this->minute_title->smallstep); $i++ )
//				{
//					$arc = deg2rad(($this->minute_title->smallstep['pos']/$GENOME_LENGTH*360-90)%360);
//					$x1 = $cx + cos($arc)*$diameter/2;
//					$x2 = $cx + cos($arc)*($diameter+$line_len)/2;
//					$y1 = $cy + sin($arc)*$diameter/2;
//					$y2 = $cy + sin($arc)*($diameter+$line_len)/2;
//					
//					$this->imagelinethick($this->img, $x1 ,$y1,  $x2, $y2,$color,1);
//				}
//				
//				$line_len = $line_len * 2;
//				for($i=0; $i<count($this->minute_title->bigstep); $i++ )
//				{
//					$angle = ($this->minute_title->bigstep['pos']/$GENOME_LENGTH*360-90)%360;
//					$arc = deg2rad($angle);
//					$x1 = $cx + cos($arc)*$diameter/2;
//					$x2 = $cx + cos($arc)*($diameter+$line_len)/2;
//					$y1 = $cy + sin($arc)*$diameter/2;
//					$y2 = $cy + sin($arc)*($diameter+$line_len)/2;
//					
//					$this->imagelinethick($this->img, $x1 ,$y1,  $x2, $y2,$color,1);
//					$font->orientation = $angle;
//					imagettftext ($this->img, $font->size, $font->orientation, $x2,$y2, $font->color, $font->fontfile, $minute_title->bigstep['pos'] );
//				}
//			}
			
			
//	} 
	
	function create_minute_title($bigstepmin, $smallstepmin)
	{
			
//		$this->minute_title = new maptitle();
//			
//		if(! is_null($bigstepmin) )
//		{
//			$step_min = $GENOME_LENGTH*$bigstepmin/100;
//			
//			for ($t=ceil($this->left_end/$step_min*100)*$step_min/100;$t<=$this->right_end;$t=$t+$step_min)
//			{
//				$minute_title->bigstep[]['pos'] = $t;
//				$minute_title->bigstep[]['txt'] = intval($t/1000);
//			}
//		}
//		if(! is_null($smallstepmin) )
//		{
//			$step_min = $GENOME_LENGTH*$smallstepmin/100;
//			
//			for ($t=ceil($this->left_end/$step_min*100)*$step_min/100;$t<=$this->right_end;$t=$t+$step_min)
//			{
//				$minute_title->smallstep[]['pos'] = $t;
//				$minute_title->smallstep[]['txt'] = intval($t/1000);
//			}
//		}
			
		
	}     
	
	function create_length_title($bigstep, $smallstep)
	{
		$this->length_title = new maptitle();
		if(! is_null($bigstep) )
		{
			for ($t=ceil($this->left_end/$bigstep)*$bigstep; $t<=$this->right_end; $t=$t+$bigstep)
			{
				$length_title->bigstep[]['pos'] = $t;
				$length_title->bigstep[]['txt'] = floatval($t*100/$GENOME_LENGTH);
			}
		}
		
		if(!is_null($smallstep) )
		{
			for ($t=ceil($this->left_end/$smallstep)*$smallstep; $t<=$this->right_end; $t=$t+$smallstep)
			{
				$length_title->smallstep[]['pos'] = $t;
				$length_title->smallstep[]['txt'] = floatval($t*100/$GENOME_LENGTH);
			}
		}
		
		
		
	}

	function arrow($im, $x1, $y1, $x2, $y2, $alength, $awidth, $color) {

			$distance = sqrt(pow($x1 - $x2, 2) + pow($y1 - $y2, 2));

			$dx = $x2 + ($x1 - $x2) * $alength / $distance;
			$dy = $y2 + ($y1 - $y2) * $alength / $distance;

			$k = $awidth / $alength;

			$x2o = $x2 - $dx;
			$y2o = $dy - $y2;

			$x3 = $y2o * $k + $dx;
			$y3 = $x2o * $k + $dy;

			$x4 = $dx - $y2o * $k;
			$y4 = $dy - $x2o * $k;
			
			$points = array(
			$x3, $y3,
			$x4, $y4,
			$x2, $y2,

			);
			imagefilledpolygon($im, $points, 3, $color);

	}
		function imagelinethick($image, $x1, $y1, $x2, $y2, $color, $thick = 1)
		{
			/* this way it works well only for orthogonal lines
			imagesetthickness($image, $thick);
			return imageline($image, $x1, $y1, $x2, $y2, $color);
			*/
			if ($thick == 1) {
				return imageline($image, $x1, $y1, $x2, $y2, $color);
			}
			$t = $thick / 2 - 0.5;
			if ($x1 == $x2 || $y1 == $y2) {
				return imagefilledrectangle($image, round(min($x1, $x2) - $t), round(min($y1, $y2) - $t), round(max($x1, $x2) + $t), round(max($y1, $y2) + $t), $color);
			}
			$k = ($y2 - $y1) / ($x2 - $x1); //y = kx + q
			$a = $t / sqrt(1 + pow($k, 2));
			$points = array(
			round($x1 - (1+$k)*$a), round($y1 + (1-$k)*$a),
			round($x1 - (1-$k)*$a), round($y1 - (1+$k)*$a),
			round($x2 + (1+$k)*$a), round($y2 - (1-$k)*$a),
			round($x2 + (1-$k)*$a), round($y2 + (1+$k)*$a),
			);
			imagefilledpolygon($image, $points, 4, $color);
			return imagepolygon($image, $points, 4, $color);
		}
		
		function drawtext(textofmap $text)
   		{
   			
   			imagettftext ($this->img, $text->size, $text->orientation, $text->x,$text->y, $text->color, $text->fontfile,$text->string );
    	}
	
}

class maptitle
{
	var $bigstep;
	var $smallstep;
	
	function maptitle()
	{
		$this->bigstep = array();
		$this->smallstep = array();
	}
	
	
}


class ecopngmap
{
        var $img; 
        var $left_margin, $right_margin; // left and right margins for additional inforomation of the map
        var $top_margin,  $bottom_margin; // top and bottom margins for possible legends or other information
        var $left_end, $right_end; // range of sequence which is drawn in this map
        var  $imx,$imy;        // the total size or the map imx is the width and imy is the high
       
       
       
       
        function ecopngmap($img, $left_margin, $right_margin, $top_margin,  $bottom_margin, $left_end, $right_end, $imx,$imy)
        {
                $this->left_margin = $left_margin;
                $this->right_margin = $right_margin;
                $this->top_margin = $top_margin;
                $this->bottom_margin = $bottom_margin;
                $this->left_end = $left_end;
                $this->right_end = $right_end;                
                $this->imx = $imx;
                $this->imy = $imy;
            //   $this->fontfile = $fontfile;
           //     $this->text_size = $text_size;
           //     $this->line_size = $line_size;
           //    $this->num_of_lines = $num_of_lines;
           //    $this->dist_line = $dist_line;
                $this->img=$img;
                
		}	
		
/////		
// the elements are draw in the main part of the map
//
//   [$left_margin imx-right_margin] by [top_margin imy-bottom_margin] (width by hight)
//    
//   The elements are supposed to be vertical line and 
//    and their are located from left_end to right_end from the sequence.
//    and the text will be drawn seperated in draw_text function
// 
/////

		function draw_circle_map($img, elemofmap $elem, $radius, $rectangle)
		{
			
			
		}

		function draw_element($img, elemofmap $elem)
		{
			$GENOME_LENGTH =  $GLOBALS['GENOME_LENGTH'];
			 $ims = imagesx($this->img) - ($this->left_margin + $this->right_margin);  //phisycal map horizontal draw range
			$map_range = ($this->right_end - $this->left_end+$GENOME_LENGTH)%$GENOME_LENGTH; // sequence range in the map
			
			$pos_1 = round($this->left_margin + (($elem->left_end - $this->left_end)+$GENOME_LENGTH)%$GENOME_LENGTH/$map_range*$ims);
			$pos_2 = round($this->left_margin + (($elem->right_end - $this->left_end)+$GENOME_LENGTH)%$GENOME_LENGTH/$map_range*$ims);
		
		
			$y1 = $this->top_margin + $elem->start_y + $elem->num_line*$elem->space_line;
//			echo "\$this->left_end ".$this->left_end."<br>";
//			echo "\$this->right_end ".$this->right_end."<br>";
//			echo "\$elem->left_end ".$elem->left_end."<br>";
//			echo "\$elem->right_end ".$elem->right_end."<br>";
//			echo "\$pos_1 ".$pos_1."<br>";
//			echo "\$pos_2  ".$pos_2."<br>";
//			echo "\$y1 ".$y1."<br>";
	
			//imageline($img, $pos_1, $y1, $pos_2, $y1, $color);
		
			if(!strcmp($elem->orientation,'Clockwise'))
			{
			
//				
				$this->arrow($this->img, $pos_2-$elem->line_size, $y1, $pos_2, $y1, $elem->line_size, $elem->line_size, $elem->color);
				
				if($pos_2-$pos_2>$elem->line_size)
				{
					$this->imagelinethick ($this->img, $pos_1 ,$y1,  $pos_2, $y1,$elem->color,$elem->line_size);
				}
				
			}else if(!strcmp($elem->orientation,'Counterclockwise'))
			{
			
				
				$this->arrow($this->img, $pos_1+$elem->line_size, $y1, $pos_1, $y1, $elem->line_size, $elem->line_size, $elem->color);
				if($pos_2-$pos_2>$elem->line_size)
				{
					$this->imagelinethick ( $this->img,$pos_1+$elem->line_size ,$y1,  $pos_2, $y1,$elem->color,$elem->line_size );
				}
			
			}else if(!strcmp($elem->orientation,'Bidirectional'))
			{
			
				$this->imagelinethick ( $this->img,$pos_1+$elem->line_size ,$y1,  $pos_2-$elem->line_size, $y1,$elem->color,$elem->line_size );
				$this->arrow($this->img, $pos_2-$elem->line_size, $y1, $pos_2, $y1, $elem->line_size, $elem->line_size, $elem->color);
				$this->arrow($this->img, $pos_1+$elem->line_size, $y1, $pos_1, $y1, $elem->line_size, $elem->line_size, $elem->color);
			
			} else {
				
				$this->imagelinethick ( $this->img,$pos_1 ,$y1,  $pos_2, $y1,$elem->color,$elem->line_size );
//				echo "\$pos_1 ".$pos_1."\$pos_2 ".$pos_2."<br>";
			}
	

		}
		
		function arrow($im, $x1, $y1, $x2, $y2, $alength, $awidth, $color) {

			$distance = sqrt(pow($x1 - $x2, 2) + pow($y1 - $y2, 2));

			$dx = $x2 + ($x1 - $x2) * $alength / $distance;
			$dy = $y2 + ($y1 - $y2) * $alength / $distance;

			$k = $awidth / $alength;

			$x2o = $x2 - $dx;
			$y2o = $dy - $y2;

			$x3 = $y2o * $k + $dx;
			$y3 = $x2o * $k + $dy;

			$x4 = $dx - $y2o * $k;
			$y4 = $dy - $x2o * $k;
			
			$points = array(
			$x3, $y3,
			$x4, $y4,
			$x2, $y2,

			);
			imagefilledpolygon($im, $points, 3, $color);

		}
		function imagelinethick($image, $x1, $y1, $x2, $y2, $color, $thick = 1)
		{
			/* this way it works well only for orthogonal lines
			imagesetthickness($image, $thick);
			return imageline($image, $x1, $y1, $x2, $y2, $color);
			*/
			if ($thick == 1) {
				return imageline($image, $x1, $y1, $x2, $y2, $color);
			}
			$t = $thick / 2 - 0.5;
			if($x1 == $x2|| $y1 == $y2)
			{
				
				return imagefilledrectangle($image, round(min($x1, $x2) - $t/3), round(min($y1, $y2) - $t), round(max($x1, $x2) + $t/3), round(max($y1, $y2) + $t), $color);
			}
//			if ($x1 == $x2 || $y1 == $y2) {
//				return imagefilledrectangle($image, round(min($x1, $x2) - $t), round(min($y1, $y2) - $t), round(max($x1, $x2) + $t), round(max($y1, $y2) + $t), $color);
//			}
			
			$k = ($y2 - $y1) / ($x2 - $x1); //y = kx + q
			$a = $t / sqrt(1 + pow($k, 2));
			$points = array(
			round($x1 - (1+$k)*$a), round($y1 + (1-$k)*$a),
			round($x1 - (1-$k)*$a), round($y1 - (1+$k)*$a),
			round($x2 + (1+$k)*$a), round($y2 - (1-$k)*$a),
			round($x2 + (1-$k)*$a), round($y2 + (1+$k)*$a),
			);
			imagefilledpolygon($image, $points, 4, $color);
			return imagepolygon($image, $points, 4, $color);
		}
		
		function drawtext(textofmap $text)
   		{
   			
   			imagettftext ($this->img, $text->size, $text->orientation, $text->x,$text->y, $text->color, $text->fontfile,$text->string );
    	}

}

	
class mapofmap{
	var $link, $alt;
	var $top, $bottom, $left, $right; 	
	function mapofmap($top, $bottom, $left, $right,$link, $alt)
	{
		$this->alt = $alt;
		$this->link = $link;
		
		$this->top = $top;
		$this->bottom = $bottom;
		$this->left = $left;
		$this->right = $right;
	}
}
class elemofmap{
	
	var $color, $left_end, $right_end, $orientation, $line_size, $start_y, $space_line, $num_line;
	 
	function elemofmap($color, $left_end, $right_end, $orientation, $line_size, $start_y, $space_line, $num_line) 	{

	 	$this->color = $color;
	 	
        $this->left_end = $left_end;

		$this->right_end = $right_end;

		$this->orientation = $orientation;	
		
		$this->line_size = $line_size;	
		
		$this->start_y = $start_y;
		
		$this->space_line = $space_line;		

		$this->num_line = $num_line;
		

    }
    
    function getmapposition(ecopngmap $map)
    {
    	 $GENOME_LENGTH =  $GLOBALS['GENOME_LENGTH'];
    	
    	$pos = array();
    	
    	$map_range = ($map->right_end - $map->left_end+$GENOME_LENGTH)%$GENOME_LENGTH;
    	$ims = imagesx($map->img) - ($map->left_margin + $map->right_margin);
    	 
    	$pos['top'] = $map->top_margin + $this->start_y + $this->num_line*$this->space_line;
    	
    	$pos['bottom'] = $map->top_margin + $this->start_y + $this->num_line*$this->space_line+$this->line_size;
   
    	
    	$pos['left']  = round($map->left_margin + (($this->left_end - $map->left_end)+$GENOME_LENGTH)%$GENOME_LENGTH/$map_range*$ims);
		$pos['right'] = round($map->left_margin + (($this->right_end - $map->left_end)+$GENOME_LENGTH)%$GENOME_LENGTH/$map_range*$ims);
		if($pos['right']-$pos['left']<$this->line_size)
		{
			$pos['right'] = $pos['right'] + $this->line_size/2;
			$pos['left'] = $pos['left'] - $this->line_size/2;
		}	
		
    	return $pos;
    	
    }
}
 class textofmap{
	 var $color, $x, $y, $orientation, $string, $fontfile, $size;
//	 var $link, $alt;
	 function textofmap($color, $x, $y, $orientation, $string, $size,$fontfile) {

	 	$this->color = $color;
	 	
        $this->x = $x;

		$this->y = $y;

		$this->orientation = $orientation;		
		
		$this->string = $string;

		$this->size = $size;
		
//		$this->link = $link;
//
//		$this->alt = $alt;
		
		$this->fontfile = $fontfile;

    }
    
	 
}
?>