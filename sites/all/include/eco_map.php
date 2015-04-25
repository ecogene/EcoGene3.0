<?php
include_once('ecoFunction.php');
global $GENOME_LENGTH;
set_time_limit(100);
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
	var $name; 
	function elemofmap($name, $color, $left_end, $right_end, $orientation, $line_size, $start_y, $space_line, $num_line) 	{

		$this->name = $name;
	 	$this->color = $color;
	 	
        $this->left_end = $left_end;

		$this->right_end = $right_end;

		$this->orientation = $orientation;	
		
		$this->line_size = $line_size;	
		
		$this->start_y = $start_y;
		
		$this->space_line = $space_line;		

		$this->num_line = $num_line;
		

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
    
     
    function draw_circle_element($diameter, $cx, $cy, $color, $elem, textofmap $text_elem, $arrow=null)
		{
			$GENOME_LENGTH =  $GLOBALS['GENOME_LENGTH'];	
			$angle_1 = round(($elem->left_end/$GENOME_LENGTH*360-90)*100)/100;
			
			if($GLOBALS['catRadioLen']=='fixed')
			{
				$angle_2 = $angle_1 + $GLOBALS['fixed_angle'];
			}else 
			{
				$angle_2 = $angle_1 + round((($elem->right_end - $elem->left_end)/$GENOME_LENGTH*360)*100)/100*$GLOBALS['native_times'];
			}
			

			
			$length = $elem->line_size;
			$diameter = $diameter - $length;
			if($arrow)
			{
			$this->imagearcthick($this->img,  $cx, $cy, $diameter, $length, $angle_1, $angle_2, $color,$elem->orientation);
			}
			else 
			{
				$this->imagearcthick($this->img,  $cx, $cy, $diameter, $length, $angle_1, $angle_2, $color);
			}
		
			$arc = deg2rad($angle_1);
			$x1 = round($cx + cos($arc)*($diameter+$length+$text_elem->size*2)/2);
			$y1 = round($cy + sin($arc)*($diameter+$length+$text_elem->size*2)/2);

			
			
			$arc_2 = deg2rad($angle_2);
			$x2 = round($cx + cos($arc_2)*($diameter-$length*2)/2);
			$y2 = round($cy + sin($arc_2)*($diameter-$length*2)/2);
			
			if($angle_1>0 && $angle_1<180){
				$text_elem->x = round($cx + cos($arc_2)*($diameter+$length+$text_elem->size*4)/2);;
				$text_elem->y = round($cy + sin($arc_2)*($diameter+$length+$text_elem->size*4)/2);;
				$text_elem->orientation = -$angle_2 + 90;
				$text_elem->string = $elem->name;
			}else {
				$text_elem->x = $x1;
			$text_elem->y = $y1;
			$text_elem->orientation = -$angle_1 + 270;
			$text_elem->string = $elem->name;
			}
			
			if($y2<$y1)
			{
				$tt = $y1;
				$y1 = $y2;
				$y2 = $tt;
			}
			if($x2<$x1)
			{
				$tt = $x1;
				$x1 = $x2;
				$x2 = $tt;
			}
				 
				$pos['top'] = $y1;    	
    			$pos['bottom'] = $y2;
   
    	
    			$pos['left']  = $x1;
				$pos['right'] = $x2;
				
    		return $pos;
			
	

	}     
	
	
	function imagearcthick($img,  $cx, $cy, $diameter, $line_len, $angle_1, $angle_2, $color, $arrow=null)
	{
		$arc_1 = deg2rad($angle_1);
		$arc_2 = deg2rad($angle_2);
		$arc_1_a = $arc_1;
		$arc_2_a = $arc_2_a;
		$dela_arc = $line_len/$diameter*2/2;
			if(!strcmp($arrow,'Clockwise'))
			{
				 
				$t = $arc_2 - $line_len/$diameter*2 ; 
				
				$arc_2 = ($t>$arc_1? $t+$dela_arc: $arc_1);
				
			}else if(!strcmp($arrow,'Counterclockwise'))
			{
			
				$t = $arc_1 + $line_len/$diameter*2 ; 
				
				$arc_1 = ($t<$arc_2? $t-$dela_arc: $arc_2);
			
			}else if(!strcmp($arrow,'Bidirectional'))
			{
				$t1 = $arc_1 + $line_len/$diameter*2 ; 
				$t2 = $arc_2 - $line_len/$diameter*2 ;
				if($t2>$t1)
				{
					$arc_1 = $t1-$dela_arc;
					$arc_2 = $t2+$dela_arc;
				}
			
			} 
			
		$x1 = round($cx + cos($arc_1)*$diameter/2);
		$y1 = round($cy + sin($arc_1)*$diameter/2);
		
		$x2 = round($cx + cos($arc_2)*$diameter/2);
		$y2 = round($cy + sin($arc_2)*$diameter/2);
		
		$x3 = round($cx + cos($arc_2)*($diameter/2+$line_len));
		$y3 = round($cy + sin($arc_2)*($diameter/2+$line_len));
		
		$x4 = round($cx + cos($arc_1)*($diameter/2+$line_len));
		$y4 = round($cy + sin($arc_1)*($diameter/2+$line_len));
		
		$dx = $x2 - $x1;
		$dy = $y2 - $y1;
		
		if($arc_1<$arc_2)
		{
		if(abs($dy)>abs($dx))
		{
			if($dy>0)
			{
				$r2 = $diameter/2*$diameter/2;
				for($t=$y1; $t<=$y2; $t++)
				{
					$points[]=sqrt($r2-($t-$cy)*($t-$cy))+$cx;
					$points[]=$t;
				}	
				$r2 = ($diameter/2+$line_len)*($diameter/2+$line_len);
				for($t=$y3; $t>=$y4; $t--)
				{
					$points[]=sqrt($r2-($t-$cy)*($t-$cy))+$cx;
					$points[]=$t;
				}	
			}
			else {
				$r2 = $diameter/2*$diameter/2;
				for($t=$y1; $t>=$y2; $t--)
				{
					$points[]=-sqrt($r2-($t-$cy)*($t-$cy))+$cx;
					$points[]=$t;
				}	
				$r2 = ($diameter/2+$line_len)*($diameter/2+$line_len);
				for($t=$y3; $t<=$y4; $t++)
				{
					$points[]=-sqrt($r2-($t-$cy)*($t-$cy))+$cx;
					$points[]=$t;
				}	
			}
		}
		else 
		{
			if($dx>0)
			{
				$r2 = $diameter/2*$diameter/2;
				for($t=$x1; $t<=$x2; $t++)
				{
					$points[]=$t;
					$points[]=-sqrt($r2-($t-$cx)*($t-$cx))+$cy;
					
				}	
				$r2 = ($diameter/2+$line_len)*($diameter/2+$line_len);
				for($t=$x3; $t>=$x4; $t--)
				{
					$points[]=$t;
					$points[]=-sqrt($r2-($t-$cx)*($t-$cx))+$cy;
				}	
			}
			else {
				$r2 = $diameter/2*$diameter/2;
				for($t=$x1; $t>=$x2; $t--)
				{
					$points[]=$t;
					$points[]=sqrt($r2-($t-$cx)*($t-$cx))+$cy;
				}	
				$r2 = ($diameter/2+$line_len)*($diameter/2+$line_len);
				for($t=$x3; $t<=$x4; $t++)
				{
					$points[]=$t;
					$points[]=sqrt($r2-($t-$cx)*($t-$cx))+$cy;
				}	
			}
		}
		if(count($points)/2<3)
		{	
				$points[0]=$x1;
				$points[1]=$y1;
				$points[2]=$x2;
				$points[3]=$y2;
				$points[4]=$x3;
				$points[5]=$y3;
				$points[6]=$x4;
				$points[7]=$y4;
			
//			print_r($points);
		}
		imagefilledpolygon($img, $points, count($points)/2, $color);
		
		}
		if(!strcmp($arrow,'Clockwise'))
			{
			
				$x5 = round($cx + cos($arc_2+$line_len/$diameter*pi()/2)*($diameter/2+$line_len/2));
				$y5 = round($cy + sin($arc_2+$line_len/$diameter*pi()/2)*($diameter/2+$line_len/2));
				$this->arrow($img, ($x3+$x2)/2, ($y3+$y2)/2, $x5,$y5, $line_len, $line_len, $color);
				
			}else if(!strcmp($arrow,'Counterclockwise'))
			{
			
				$x5 = round($cx + cos($arc_1_a)*($diameter/2+$line_len/2));
				$y5 = round($cy + sin($arc_1_a)*($diameter/2+$line_len/2));
				$this->arrow($img, ($x1+$x4)/2, ($y1+$y4)/2, $x5,$y5,$line_len, $line_len, $color);
			
			}else if(!strcmp($arrow,'Bidirectional'))
			{
			
				$x5 = round($cx + cos($arc_2+$line_len/$diameter*pi()/2)*($diameter/2+$line_len/2));
				$y5 = round($cy + sin($arc_2+$line_len/$diameter*pi()/2)*($diameter/2+$line_len/2));
				$this->arrow($img, ($x3+$x2)/2, ($y3+$y2)/2, $x5,$y5, $line_len, $line_len, $color);
				
				$x5 = round($cx + cos($arc_1_a)*($diameter/2+$line_len/2));
				$y5 = round($cy + sin($arc_1_a)*($diameter/2+$line_len/2));
				$this->arrow($img, ($x1+$x4)/2, ($y1+$y4)/2, $x5,$y5,$line_len, $line_len, $color);
			
			} 
//		print_r($points);
//		$points=array(
//		$x1, $y1,$x2, $y2,$x3, $y3,$x4, $y4);
//		print_r($points);
		
//		imagefilledpolygon($img, $points, count($points)/2, $color);
	}
    function draw_circle_map_title($diameter, $cx, $cy, $color, $type, $line_len, $font)
	{
			$GENOME_LENGTH =  $GLOBALS['GENOME_LENGTH'];	
//					
			$color_c = imagecolorallocate($this->img, 0, 0, 0);
			$col_ellipse = imagecolorallocate($this->img, 255, 255, 255);
			imagefilledellipse($this->img, $cx, $cy, $diameter+$line_len/2, $diameter+$line_len/2, $color_c);
			imagefilledellipse($this->img, $cx, $cy, $diameter-$line_len/2, $diameter-$line_len/2, $col_ellipse);
//			
//			imagearc($this->img, $cx, $cy, $diameter, $diameter,  0, 360, $color);
//			imagearc($this->img, $cx, $cy, $diameter-0.5, $diameter-0.5,  0, 360, $color);
//			imagearc($this->img, $cx, $cy, $diameter-1, $diameter-1,  0, 360, $color);
////			
//			imagearc($this->img, 100, 100, 200, 200,  0, 360, $color);
//			imagettftext ($this->img, $font->size, 0, 400 ,100, $font->color, $font->fontfile, 'minute' );

			if($type=='minute')
			{
				
//			imagettftext ($this->img, $font->size+1, 0, $cx+$font->size*3 ,round($cy - $font->size/2-$diameter/2-$line_len*2-2), $font->color, $font->fontfile, 'Minute' );
				for($i=0; $i<count($this->minute_title->smallstep)-1; $i++ )
				{
					$angle = ($this->minute_title->smallstep[$i]['pos']/$GENOME_LENGTH*360-90);
					$arc = deg2rad($angle);
					
					$x1 = round($cx + cos($arc)*($diameter/2));
					$x2 = round($cx + cos($arc)*($diameter/2+$line_len));
					$y1 = round($cy + sin($arc)*($diameter/2));
					$y2 = round($cy + sin($arc)*($diameter/2+$line_len));
					
					$this->imagelinethick($this->img, $x1 ,$y1,  $x2, $y2,$color,round($line_len/4));
				}
				
				$line_len = $line_len * 2;
				for($i=0; $i<count($this->minute_title->bigstep); $i++ )
				{
					$angle = ($this->minute_title->bigstep[$i]['pos']/$GENOME_LENGTH*360-90);
					$arc = deg2rad($angle);
					$x1 = round($cx + cos($arc)*($diameter/2));
					$x2 = round($cx + cos($arc)*($diameter/2+$line_len));
					$y1 = round($cy + sin($arc)*($diameter/2));
					$y2 = round($cy + sin($arc)*($diameter/2+$line_len));
					

					
					$font->orientation = -$angle;
					if ($font->orientation <-90.0000001 && $font->orientation >-269.99)
					{
						$this->imagelinethick($this->img, $x1 ,$y1,  $x2, $y2,$color,$line_len/4);
						$font->orientation = $font->orientation + 180;
						$x2 = intval($cx + cos($arc)*($diameter/2+$font->size*2+$line_len));			
						$y2 = intval($cy + sin($arc)*($diameter/2+$font->size*2+$line_len));
						imagettftext ($this->img, $font->size, $font->orientation, $x2,$y2, $font->color, $font->fontfile, $this->minute_title->bigstep[$i]['txt'] );
					}
					elseif ($font->orientation >=-90.0000001 && $font->orientation <89.999999){
						$this->imagelinethick($this->img, $x1 ,$y1,  $x2, $y2,$color,$line_len/4);
						$x2 = round($cx + cos($arc)*($diameter/2+$line_len+$font->size*0.5));		
						$y2 = round($cy + sin($arc)*($diameter/2+$line_len+$font->size*0.5));
						imagettftext ($this->img, $font->size, $font->orientation, $x2,$y2, $font->color, $font->fontfile, $this->minute_title->bigstep[$i]['txt'] );
					}
					else {
					
					$x1 = round($cx-$line_len/8);
					$x2 =  round($cx-$line_len/8);
					$y1 =  round($cy - $diameter/2-$line_len/4) ;
					$y2 =  round($cy - $diameter/2-$line_len-2);
					
					$this->imagelinethick($this->img, $x1 ,$y1,  $x2, $y2,$color,$line_len/4+2);
					
						imagettftext ($this->img, $font->size+1, 0, $x2-$font->size*2,$y2-$font->size/2, $font->color, $font->fontfile, '0/100'."'" );
					}

					
					
					
				}
				
			}
			if($type=='length')
			{
				imagettftext ($this->img, $font->size+1, 0, $cx+$font->size*3 ,round($cy - $diameter/2+$line_len*2+2+$font->size*2), $font->color, $font->fontfile, '  Kb' );
				
				for($i=0; $i<count($this->length_title->smallstep); $i++ )
				{
					$angle = ($this->length_title->smallstep[$i]['pos']/$GENOME_LENGTH*360-90);
					$arc = deg2rad($angle);
					
					$x1 = round($cx + cos($arc)*($diameter/2));
					$x2 = round($cx + cos($arc)*($diameter/2-$line_len));
					$y1 = round($cy + sin($arc)*($diameter/2));
					$y2 = round($cy + sin($arc)*($diameter/2-$line_len));
					
					$this->imagelinethick($this->img, $x1 ,$y1,  $x2, $y2,$color,round($line_len/4));

				}
				
				$line_len = $line_len * 2;
				for($i=0; $i<count($this->length_title->bigstep); $i++ )
				{
					$angle = ($this->length_title->bigstep[$i]['pos']/$GENOME_LENGTH*360-90);
					$arc = deg2rad($angle);
					$x1 = round($cx + cos($arc)*($diameter/2+1));
					$x2 = round($cx + cos($arc)*($diameter/2-$line_len));
					$y1 = round($cy + sin($arc)*($diameter/2+1));
					$y2 = round($cy + sin($arc)*($diameter/2-$line_len));
					

					
					$font->orientation = -$angle;
					
					$str = $this->length_title->bigstep[$i]['txt'];
					
					if ($font->orientation <-90.0000001 && $font->orientation >-269.99)
					{
						$this->imagelinethick($this->img, $x1 ,$y1,  $x2, $y2,$color,$line_len/4);
						$font->orientation = $font->orientation + 180;
						
												
						$x2 = intval($cx + cos($arc)*($diameter/2-$line_len*1));			
						$y2 = intval($cy + sin($arc)*($diameter/2-$line_len*1));
						imagettftext ($this->img, $font->size, $font->orientation, $x2,$y2, $font->color, $font->fontfile, $str );
					}elseif ($font->orientation >=-90.0000001 && $font->orientation <89.999999){
						$this->imagelinethick($this->img, $x1 ,$y1,  $x2, $y2,$color,$line_len/4);
						$x2 = intval($cx + cos($arc)*($diameter/2-$line_len - strlen($str)* $font->size*0.8));			
						$y2 = intval($cy + sin($arc)*($diameter/2-$line_len - strlen($str)* $font->size*0.8));
						imagettftext ($this->img, $font->size, $font->orientation, $x2,$y2, $font->color, $font->fontfile, $str );
					}
					
				
				
			}
						
					
					
					$x1 = round($cx-$line_len/8);
					$x2 =  round($cx-$line_len/8);
					$y1 =  round($cy - $diameter/2+$line_len/4) ;
					$y2 =  round($cy - $diameter/2+$line_len+2);
					
					$this->imagelinethick($this->img, $x1 ,$y1,  $x2, $y2,$color,$line_len/4+2);
				
					
						imagettftext ($this->img, $font->size+1, 0, $x2-$font->size*4,$y2+$font->size*2, $font->color, $font->fontfile, ($GENOME_LENGTH/1000));	
				
				
			}
			
			
	} 
	
	function create_minute_title($bigstepmin, $smallstepmin)
	{
		$GENOME_LENGTH =  $GLOBALS['GENOME_LENGTH'];	
		
		$this->minute_title = new maptitle();
			
		if(! is_null($bigstepmin) )
		{
			$i=0;
			$step_min = $GENOME_LENGTH*$bigstepmin/100;
			
			for ($t=ceil($this->left_end/$step_min)*$step_min;$t<=$this->right_end;$t=$t+$step_min)
			{
				$this->minute_title->bigstep[$i]['pos'] = $t;
				$this->minute_title->bigstep[$i]['txt'] = floatval($t*100/$GENOME_LENGTH);
				$i++;
			}
		}
		if(! is_null($smallstepmin) )
		{
			$i=0;
			$step_min = $GENOME_LENGTH*$smallstepmin/100;
			
			for ($t=ceil($this->left_end/$step_min)*$step_min;$t<=$this->right_end;$t=$t+$step_min)
			{
				$this->minute_title->smallstep[$i]['pos'] = $t;
				$this->minute_title->smallstep[$i]['txt'] = floatval($t*100/$GENOME_LENGTH);
				$i++;
			}
		}
		
//		print_r($this->minute_title);
			
		
	}     
	
	function create_length_title($bigstep, $smallstep)
	{
		$this->length_title = new maptitle();
		if(! is_null($bigstep) )
		{
			$i=0;
			for ($t=ceil($this->left_end/$bigstep)*$bigstep; $t<=$this->right_end; $t=$t+$bigstep)
			{
				$this->length_title->bigstep[$i]['pos'] = $t;
				$this->length_title->bigstep[$i]['txt'] = floatval($t/1000);
				$i++;
			}
		}
		
		if(!is_null($smallstep) )
		{
			$i=0;
			for ($t=ceil($this->left_end/$smallstep)*$smallstep; $t<=$this->right_end; $t=$t+$smallstep)
			{
				$this->length_title->smallstep[$i]['pos'] = $t;
				$this->length_title->smallstep[$i]['txt'] = floatval($t/1000);
				$i++;
			}
		}
//		print_r($this->length_title);
		
		
		
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
		
//		function drawtext(textofmap $text)
//   		{
//   			
//   			imagettftext ($this->img, $text->size, $text->orientation, $text->x,$text->y, $text->color, $text->fontfile,$text->string );
//    	}
	
}

function gene_elem_line(&$gene, $start, $end, $num_line, $overlap_dis)
{
//	global  $overlap_dis;
	//$num_line = 1;
	$gene[0]['line'] = 1;
	$flag = 1;
	
	while($flag == 1)
	{
		$flag = 0;
		for($k=0; $k<sizeof($gene); $k++)
		{
			
			if($gene[$k]['line']==0)
			{
				
				$flag_over = 0;
//				for($t=0; $t<sizeof($gene); $t++)
				for($t=$start; $t<$end; $t++)
				{
					if(($gene[$t]['line']==$num_line) && 
						( 
						 (($gene[$k]['left_end'] < $gene[$t]['right_end'] +$overlap_dis) && ($gene[$k]['left_end'] > $gene[$t]['left_end'])-$overlap_dis)
						 || (($gene[$k]['right_end'] < $gene[$t]['right_end']+$overlap_dis) && ($gene[$k]['right_end'] > $gene[$t]['left_end']-$overlap_dis))
						 )
						 )
					{
						$flag_over = 1;
						break;
					}
				}
				if($flag_over==0)
				{
					$gene[$k]['line']=$num_line;
					
				}
				
			}
		}
		for($k=0; $k<sizeof($gene); $k++)
		{
			if($gene[$k]['line']==0)
			{
				$flag = 1;
				$num_line = $num_line + 1;
			    
				break;
			}
		}
		
		
		
	}
	
	return $num_line;
}
?>