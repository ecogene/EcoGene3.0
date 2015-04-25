<?
	$text = $_GET['text'];
	$image = @ImageCreateFrompng("../images/empty.png");
    $text_color = ImageColorAllocate($image, 255, 255, 255);
    Header("Content-type: image/png");
    ImageString ($image, 3, imagesx($image)/2-strlen($text)* imagefontwidth(3) / 2, 1, $text, $text_color);
    //$tmpfname="temp/testpng3.png";
    Imagepng($image);
	ImageDestroy($image);
?>